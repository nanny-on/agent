/*
 * Copyright (C) 2021-2026 ASHINi Corp. 
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; either 
 * version 3 of the License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public 
 * License along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 * * 
 */

// ThreadPoInPtnFile.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "fanotify_dlg.h"
#include "fanotify_thread_event.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadFaNotifyEvent

CThreadFaNotifyEvent*	t_ThreadFaNotifyEvent = NULL;

CThreadFaNotifyEvent::CThreadFaNotifyEvent()
{
	m_nFaNotifyFd = -1;
	m_nClientFd = -1;
	m_nSendTime = 0;
	m_nFaNotifyCount = 0;
	m_nFaNotifyIndex = 0;
	m_pNotifyWnd = NULL;
	m_bIsConnected = FALSE;
	pthread_mutex_init(&m_EventMutex, NULL);
	pthread_mutex_init(&m_SockMutex, NULL);
}

CThreadFaNotifyEvent::~CThreadFaNotifyEvent()
{
	pthread_mutex_destroy(&m_EventMutex);
	pthread_mutex_destroy(&m_SockMutex);
}

INT32	CThreadFaNotifyEvent::SetNotifyWnd(HWND pNotifyWnd)
{
	if(pNotifyWnd == NULL)
	{
		return -1;
	}
	m_pNotifyWnd = pNotifyWnd;
	return 0;
}

INT32	CThreadFaNotifyEvent::InitSockEnv(INT32 &nClientFd)
{
	INT32 nRetVal = 0;
	INT32 nClientLen = 0;
	struct sockaddr_un stClientAddr;
	char acSockPath[MAX_PATH] = {0,};
	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_FILE);
	acSockPath[MAX_FILE_NAME-1] = 0;

	if(is_file(acSockPath) != SOCK_FILE)
	{
		return -1;
	}

	pthread_mutex_lock (&m_SockMutex);
	do{
		m_nClientFd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (m_nClientFd == -1)
		{
			nRetVal = -2;
			break;
		}
		nClientLen = sizeof(stClientAddr);
		memset(&stClientAddr, 0, nClientLen);
		stClientAddr.sun_family = AF_UNIX;
		strncpy(stClientAddr.sun_path, acSockPath, MAX_FILE_NAME-1);
		nRetVal = connect(m_nClientFd, (struct sockaddr *)&stClientAddr, nClientLen);
		if (nRetVal < 0)
		{
			nRetVal = -3;
			break;
		}
		nClientFd = m_nClientFd;
		m_bIsConnected = TRUE;
		m_nSendTime = 0;
		nRetVal = 0;
	}while(FALSE);
	pthread_mutex_unlock (&m_SockMutex);

	if(nRetVal < 0)
	{
		UninitSockEnv();
	}
	return nRetVal;
}

VOID	CThreadFaNotifyEvent::GetClientFd(INT32 &nClientFd)
{
	pthread_mutex_lock (&m_SockMutex);
	nClientFd = m_nClientFd;
	pthread_mutex_unlock (&m_SockMutex);
}


BOOL	CThreadFaNotifyEvent::IsSockConnected()
{
	BOOL IsConnected = FALSE;
	pthread_mutex_lock (&m_SockMutex);
	IsConnected = m_bIsConnected;
	pthread_mutex_unlock (&m_SockMutex);
	return IsConnected;
}

VOID	CThreadFaNotifyEvent::UninitSockEnv()
{
	pthread_mutex_lock (&m_SockMutex);
	if(m_nClientFd != -1)
	{
		close(m_nClientFd);
		m_nClientFd = -1;
	}
	m_nSendTime = 0;
	m_bIsConnected = FALSE;
	pthread_mutex_unlock (&m_SockMutex);
}

INT32 CThreadFaNotifyEvent::SockRecv(INT32 nFd, PVOID pRecvData, INT32 nReqSize)
{
	INT32 nOnceRecvSize = 0;
	INT32 nRecvedSize = 0;
	INT32 nRecvSize = 0;
	INT32 nContinueCount = 0;
	INT32 nRetVal = 0;
	char *pcRecv = NULL;
	struct timeval tv;
	fd_set fds;

	if(pRecvData == NULL || nReqSize < 1 || nFd < -1)
	{
		return -1;
	}
	if(nFd < -1)
		return -2;

	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(nFd, &fds);
	nRetVal = select(FD_SETSIZE, &fds, NULL, NULL, &tv);
	if (nRetVal == 0)
	{
		return -3;
	}

	pcRecv = (char *)pRecvData;
	while (nRecvedSize < nReqSize)
	{
		nOnceRecvSize = nReqSize - nRecvedSize;

		nRecvSize = read(nFd, &pcRecv[nRecvedSize], nOnceRecvSize);
		if (nRecvSize == -1)
		{
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nContinueCount++;
				if(nContinueCount > 20)
				{
					return -4;
				}
				Sleep(20);
				continue;
			}
			else
			{
				return -5;
			}
		}
		else if (nRecvSize == 0)
		{
			return -6;
		}
		else
		{
			nRecvedSize += nRecvSize;
		}
	}
	return 0;
}


INT32 CThreadFaNotifyEvent::SockWrite(INT32 nFd, PVOID pWriteData, INT32 nReqSize)
{
	INT32 nOnceWriteSize = 0;
	INT32 nWritedSize = 0;
	INT32 nWriteSize = 0;
	INT32 nContinueCount = 0;
	INT32 nRetVal = 0;
	char *pcWrite = NULL;
	if(pWriteData == NULL || nReqSize < 1 || nFd < -1)
	{
		return -1;
	}
	pthread_mutex_lock (&m_SockMutex);
	pcWrite = (char *)pWriteData;
	while (nWritedSize < nReqSize)
	{
		nOnceWriteSize = nReqSize - nWritedSize;

		nWriteSize = write(nFd, &pcWrite[nWritedSize], nOnceWriteSize);
		if (nWriteSize == -1)
		{
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nContinueCount++;
				if(nContinueCount > 20)
				{
					nRetVal = -3;
					break;
				}
				Sleep(30);
				continue;
			}
			else
			{
				nRetVal = -4;
				break;
			}
		}
		else if (nWriteSize == 0)
		{
			nRetVal = -5;
			break;
		}
		else
		{
			nWritedSize += nWriteSize;
		}
		nRetVal = 0;
	}
	if(nRetVal == 0)
		m_nSendTime = GetCurrentDateTimeInt();
	pthread_mutex_unlock (&m_SockMutex);
	return 0;
}


UINT32 CThreadFaNotifyEvent::GetSendTime()
{
	UINT32 nTime = 0;
	pthread_mutex_lock (&m_SockMutex);
	nTime = m_nSendTime;
	pthread_mutex_unlock (&m_SockMutex);
	return nTime;
}


INT32 CThreadFaNotifyEvent::SendAliveMessage()
{
	BOOL bIsConnected = FALSE;
	INT32 nRetVal = 0;
	INT32 nClientFd = -1;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	PASI_CHK_FILE_PROC pChkFileProc = NULL;

	GetClientFd(nClientFd);
	bIsConnected = IsSockConnected();
	if(bIsConnected == FALSE || nClientFd == -1)
		return 1;
	do{
		pChkFileProc = (PASI_CHK_FILE_PROC)malloc(nSize);
		if(pChkFileProc == NULL)
		{
			nRetVal = -2;
			break;
		}
		memset(pChkFileProc, 0, nSize);
		pChkFileProc->nCmdId = CMD_PIPE_ALIVE_MESSAGE;
		nRetVal = SockWrite(nClientFd,(PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			nRetVal = -3;
			break;
		}

		nRetVal = 0;
	}while(FALSE);

	safe_free(pChkFileProc);
	return nRetVal;
}


INT32	CThreadFaNotifyEvent::InitNotifyEvent(INT32 &nNotifyFd, INT32 &nClientFd, INT32 nTestMode)
{
	INT32 nRetVal = 0;
	INT32 nNFd = -1;
	INT32 nCFd = -1;
	CFaNotifyDlg* t_NotifyDlg = (CFaNotifyDlg *)m_pNotifyWnd;
	do{
		if(t_NotifyDlg != NULL)
			t_NotifyDlg->WriteLogN("CheckWhitePatternFile");
		nRetVal = CheckWhitePatternFile();
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}
		if(t_NotifyDlg != NULL)
			t_NotifyDlg->WriteLogN("InitSockEnv");

		nRetVal = InitSockEnv(nCFd);
		if(nRetVal < 0 || nCFd == -1)
		{
			if(t_NotifyDlg != NULL)
				t_NotifyDlg->WriteLogE("fail to init sock env (%d) (%d)", nRetVal, errno);
			nRetVal -= 20;
			break;
		}

		if(t_NotifyDlg != NULL)
			t_NotifyDlg->WriteLogN("nTestMode : %d", nTestMode);
		if(nTestMode == 0)
		{
			if(t_NotifyDlg != NULL)
				t_NotifyDlg->WriteLogN("InitFaNotifyFd");

			if(InitFaNotifyFd(nNFd) == FALSE)
			{
				if(t_NotifyDlg != NULL)
					t_NotifyDlg->WriteLogE("fail to init notify fd (%d)", errno);
				nRetVal = -3;
				UninitSockEnv();
				break;
			}
			if(t_NotifyDlg != NULL)
				t_NotifyDlg->WriteLogN("AddWatchFaNotify");


			nRetVal = AddWatchFaNotify(1, ASI_FANOTIFY_PATH);
			if(nRetVal < 0)
			{
				nRetVal -= 100;
				UninitSockEnv();
				CloseFaNotifyFd();
				break;
			}
			if(t_NotifyDlg != NULL)
				t_NotifyDlg->WriteLogN("after AddWatchFaNotify");
			nNotifyFd = nNFd;
		}
		nClientFd = nCFd;
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

VOID	CThreadFaNotifyEvent::UninitNotifyEvent(INT32 nTestMode)
{
	if(nTestMode == 0)
	{
		ClearWatchFaNotify();
		CloseFaNotifyFd();
	}
	UninitSockEnv();
}


BOOL CThreadFaNotifyEvent::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadFaNotifyEvent::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadFaNotifyEvent message handlers

INT32 CThreadFaNotifyEvent::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 nRetVal = 0;
	INT32 nNotifyFd = -1;
	INT32 nTestMode = 0;
	INT32 nTestCount = 0;
	INT32 nClientFd = -1;
	char *pcBuffer = NULL;
	PASI_CHK_FILE_PROC pChkFileProc;
	CFaNotifyDlg* t_NotifyDlg = (CFaNotifyDlg *)m_pNotifyWnd;
	pid_t tid = syscall(SYS_gettid);
	nRetVal = setpriority(PRIO_PROCESS, tid, -10);
	if(nRetVal < 0)
	{
		if(t_NotifyDlg != NULL)
			t_NotifyDlg->WriteLogE("fail to set priority: [%s][%d]", m_strThreadName.c_str(), errno);
		return -1;
	}

	pcBuffer = (char *)malloc(ASI_FANOTIFY_BUFF_MAX+1);
	if(pcBuffer == NULL)
	{
		if(t_NotifyDlg != NULL)
			t_NotifyDlg->WriteLogE("fail to allocate memory : [%s][%d]", m_strThreadName.c_str(), errno);
		return -2;
	}
	memset(pcBuffer, 0, ASI_FANOTIFY_BUFF_MAX+1);

	pChkFileProc = (PASI_CHK_FILE_PROC)malloc(sizeof(ASI_CHK_FILE_PROC));
	if(pChkFileProc == NULL)
	{
		if(t_NotifyDlg != NULL)
			t_NotifyDlg->WriteLogE("fail to allocate memory : [%s][%d]", m_strThreadName.c_str(), errno);
		safe_free(pcBuffer);
		return -3;
	}
	m_nPause = 1;
	nRetVal = InitNotifyEvent(nNotifyFd, nClientFd, nTestMode);
	if(nRetVal == 0)
	{
		m_nPause = 0;
	}
	else
	{
		if(t_NotifyDlg != NULL)
			t_NotifyDlg->WriteLogE("fail to init notify event : [%s][%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
	}

	m_nRunFlag = 1;
	if(t_NotifyDlg != NULL)
		t_NotifyDlg->WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(GetContinue())
	{
		if(!m_nPause)
		{
			memset(pChkFileProc, 0, sizeof(ASI_CHK_FILE_PROC));

			if(nTestMode == 1)
			{
				nRetVal = AnalyzeExecEvent2(nClientFd, pChkFileProc, nTestCount++%6);
				if(nRetVal < 0)
				{
					UninitNotifyEvent(nTestMode);
					nNotifyFd = -1;
					nClientFd = -1;
					m_nPause = 1;
					Sleep(1000);
				}
				Sleep(2000);
				continue;
			}
			nRetVal = CheckFaNotifyFile(nNotifyFd, nClientFd, pcBuffer, pChkFileProc);
			if(nRetVal != 0)
			{
				if(nRetVal == -1)
				{
					if(t_NotifyDlg != NULL)
						t_NotifyDlg->WriteLogE("invalid input data.exit thread : [%s]", m_strThreadName.c_str());
					break;
				}
				else
				{
					if(t_NotifyDlg != NULL)
						t_NotifyDlg->WriteLogN("reconnect nannysvc : [%s][%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
					UninitNotifyEvent(nTestMode);
					nNotifyFd = -1;
					nClientFd = -1;
					m_nPause = 1;
					Sleep(1000);
				}
			}
		}
		else
		{
			nRetVal = InitNotifyEvent(nNotifyFd, nClientFd, nTestMode);
			if(nRetVal == 0)
			{
				m_nPause = 0;
			}
			else
			{
				m_nPause = 1;
				Sleep(1000);
			}
		}
	}
	if(t_NotifyDlg != NULL)
	{
		if(!GetContinue())
			t_NotifyDlg->WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());
	}
	UninitNotifyEvent(nTestMode);
	safe_free(pcBuffer);
	safe_free(pChkFileProc);
	return 0;
}

INT32 CThreadFaNotifyEvent::CheckWhitePatternFile()
{
	INT32 nRetVal = 0;
	DIR *pDir = NULL;
	struct dirent *pDrp = NULL;
	CHAR acPath[MAX_PATH] = {0, };
	snprintf(acPath, MAX_PATH-1, "%s/%s/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, STR_WENG_PTN_FILE);
	acPath[MAX_PATH-1] = 0;

	nRetVal = is_file(acPath);
	if(nRetVal < 0)
	{
		return -1;
	}
	pDir = opendir(acPath);
	if (pDir == NULL)
		return -2;

	while ((pDrp = readdir(pDir)) != NULL)
	{
		if(!_stricmp(pDrp->d_name, ".") || !_stricmp(pDrp->d_name, ".."))
		{
			continue;
		}

		if(DT_REG == pDrp->d_type)
		{
			closedir(pDir);
			return 0;
		}
	}

	closedir(pDir);
	return -3;
}

BOOL CThreadFaNotifyEvent::InitFaNotifyFd(INT32 &nNotifyFd)
{
	BOOL bRetVal = TRUE;
	pthread_mutex_lock (&m_EventMutex);
	if ( m_nFaNotifyFd < 0 )
	{
		m_nFaNotifyFd = fanotify_init(FAN_CLOEXEC | FAN_CLASS_CONTENT, O_RDONLY | O_CLOEXEC | O_LARGEFILE | O_NOATIME);
		if ( m_nFaNotifyFd < 0 )
		{
			bRetVal = FALSE;
		}
		else
		{
			nNotifyFd = m_nFaNotifyFd;
		}
	}
	pthread_mutex_unlock (&m_EventMutex);
	return bRetVal;
}

BOOL CThreadFaNotifyEvent::GetFaNotifyFd(INT32 &nNotifyFd)
{
	BOOL bRetVal = FALSE;
	pthread_mutex_lock (&m_EventMutex);
	if ( m_nFaNotifyFd < 0 )
	{
		bRetVal = FALSE;
	}
	else
	{
		nNotifyFd = m_nFaNotifyFd;
		bRetVal = TRUE;
	}
	pthread_mutex_unlock (&m_EventMutex);
	return bRetVal;
}

VOID CThreadFaNotifyEvent::CloseFaNotifyFd()
{
	pthread_mutex_lock (&m_EventMutex);
	if ( m_nFaNotifyFd >= 0 )
	{
		close(m_nFaNotifyFd);
		m_nFaNotifyFd = -1;
	}
	pthread_mutex_unlock (&m_EventMutex);
}

INT32 CThreadFaNotifyEvent::AddFaNotifyMark(PFANOTIFY_PATH pFaNotifyPath)
{
	INT32 nNotifyFd = -1;
	INT32 nRetVal = 0;
	UINT64 ddwEventMask = FAN_CLOSE_WRITE | FAN_OPEN_EXEC_PERM;
	CFaNotifyDlg* t_NotifyDlg = (CFaNotifyDlg *)m_pNotifyWnd;
	if(pFaNotifyPath == NULL || pFaNotifyPath->acNotifyPath[0] == 0)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[AddWatchFaNotify] invalid input data");
		return -1;
	}

	if(GetFaNotifyFd(nNotifyFd) == FALSE)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[AddWatchFaNotify] invalid fanotify fd");
		return -2;
	}

	if(IsFaNotifyPath(pFaNotifyPath->acNotifyPath) == TRUE)
	{
		ModFaNotifyPath(pFaNotifyPath);
		return 0;
	}

	nRetVal = fanotify_mark(nNotifyFd, FAN_MARK_ADD | FAN_MARK_MOUNT, ddwEventMask, AT_FDCWD, pFaNotifyPath->acNotifyPath);
	if(nRetVal < 0)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[AddWatchFaNotify] fail to add fanotify watch [%s] : [%d]", pFaNotifyPath->acNotifyPath, errno);
		return -3;
	}

	if(AddFaNotifyPath(pFaNotifyPath) == FALSE)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[AddWatchFaNotify] fail to add fanotify path [%s]", pFaNotifyPath->acNotifyPath);
		fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, pFaNotifyPath->acNotifyPath);
		return -4;
	}
	return 0;
}

INT32 CThreadFaNotifyEvent::RemoveFaNotifyMark(PFANOTIFY_PATH pFaNotifyPath)
{
	INT32 nNotifyFd = -1;
	INT32 nRetVal = 0;
	UINT64 ddwEventMask = FAN_CLOSE_WRITE | FAN_OPEN_EXEC_PERM;
	CFaNotifyDlg* t_NotifyDlg = (CFaNotifyDlg *)m_pNotifyWnd;
	if(pFaNotifyPath == NULL || pFaNotifyPath->acNotifyPath[0] == 0)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[RemoveFaNotifyMark] invalid input data");
		return -1;
	}

	if(GetFaNotifyFd(nNotifyFd) == FALSE)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[RemoveFaNotifyMark] invalid fanotify fd");
		return -2;
	}

	if(GetFaNotifyPath(pFaNotifyPath) == FALSE)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[RemoveFaNotifyMark] fail to find fanotify path (%s)", pFaNotifyPath->acNotifyPath);
		return -3;
	}

	fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, pFaNotifyPath->acNotifyPath);

	if(DelFaNotifyPath(pFaNotifyPath->nIndex) == FALSE)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[RemoveFaNotifyMark] fail to del fanotify path [%s][%d]", pFaNotifyPath->acNotifyPath, pFaNotifyPath->nIndex);
		fanotify_mark(nNotifyFd, FAN_MARK_ADD | FAN_MARK_MOUNT, ddwEventMask, AT_FDCWD, pFaNotifyPath->acNotifyPath);
		return -5;
	}
	return 0;
}

INT32 CThreadFaNotifyEvent::AddWatchFaNotify(INT32 nOrderID, char *pPath)
{
	INT32 nLen = 0;
	INT32 nRetVal = 0;
	PFANOTIFY_PATH pFaNotifyPath = NULL;
	CFaNotifyDlg* t_NotifyDlg = (CFaNotifyDlg *)m_pNotifyWnd;
	if(pPath == NULL || pPath[0] == 0)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[AddWatchFaNotify] invalid input data");
		return -1;
	}
	nLen = (INT32)strlen(pPath);
	if(nLen < 1 || nLen > MAX_PATH-2)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[AddWatchFaNotify] invalid path len (%d)", nLen);
		return -2;
	}

	pFaNotifyPath = (PFANOTIFY_PATH)malloc(sizeof(FANOTIFY_PATH));
	if(pFaNotifyPath == NULL)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[AddWatchFaNotify] fail to allocate memory : [%d]", errno);
		return -3;
	}
	memset(pFaNotifyPath, 0, sizeof(FANOTIFY_PATH));

	strncpy(pFaNotifyPath->acNotifyPath, pPath, MAX_PATH-1);
	pFaNotifyPath->acNotifyPath[MAX_PATH-1] = 0;

	if(nLen > 2 && pFaNotifyPath->acNotifyPath[nLen-1] == '/')
	{
		pFaNotifyPath->acNotifyPath[nLen-1] = '\0';
		nLen--;
	}

	pFaNotifyPath->nPathLen = nLen;
	pFaNotifyPath->nOrderID = nOrderID;

	nRetVal = AddFaNotifyMark(pFaNotifyPath);
	if(nRetVal != 0)
	{
		nRetVal -= 10;
		safe_free(pFaNotifyPath);
		return nRetVal;
	}
	safe_free(pFaNotifyPath);
	if(t_NotifyDlg)
		t_NotifyDlg->WriteLogN("success to add fa watch %s", pPath);
	return 0;
}

INT32 CThreadFaNotifyEvent::RemoveWatchFaNotify(char *pPath)
{
	INT32 nLen = 0;
	INT32 nRetVal = 0;
	PFANOTIFY_PATH pFaNotifyPath = NULL;
	CFaNotifyDlg* t_NotifyDlg = (CFaNotifyDlg *)m_pNotifyWnd;
	if(pPath == NULL || pPath[0] == 0)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[DelWatchFaNotify] invalid input data");
		return -1;
	}

	nLen = (INT32)strlen(pPath);
	if(nLen < 1 || nLen > MAX_PATH-2)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[DelWatchFaNotify] invalid path len (%d)", nLen);
		return -2;
	}

	pFaNotifyPath = (PFANOTIFY_PATH)malloc(sizeof(FANOTIFY_PATH));
	if(pFaNotifyPath == NULL)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[DelWatchFaNotify] fail to allocate memory : [%d]", errno);
		return -3;
	}
	memset(pFaNotifyPath, 0, sizeof(FANOTIFY_PATH));

	strncpy(pFaNotifyPath->acNotifyPath, pPath, MAX_PATH-1);
	pFaNotifyPath->acNotifyPath[MAX_PATH-1] = 0;

	if(nLen > 2 && pFaNotifyPath->acNotifyPath[nLen-1] == '/')
	{
		pFaNotifyPath->acNotifyPath[nLen-1] = '\0';
	}

	nRetVal = RemoveFaNotifyMark(pFaNotifyPath);
	if(nRetVal != 0)
	{
		if(t_NotifyDlg)
			t_NotifyDlg->WriteLogE("[AddWatchFaNotify] fail to remove fanotify mark : [%d]", nRetVal);
		nRetVal -= 10;
		safe_free(pFaNotifyPath);
		return nRetVal;
	}
	safe_free(pFaNotifyPath);
	return 0;
}


BOOL CThreadFaNotifyEvent::IsFaNotifyPath(char *pPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	if(pPath == NULL || pPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tFaNotifyEventMap.begin();	end = m_tFaNotifyEventMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(!strcmp(pFindPath->acNotifyPath, pPath))
		{
			pthread_mutex_unlock (&m_EventMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_EventMutex);
	return FALSE;
}

BOOL CThreadFaNotifyEvent::AddFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath)
{
	if(pFaNotifyPath == NULL || pFaNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	pFaNotifyPath->nIndex = m_nFaNotifyIndex;
	m_tFaNotifyEventMap[m_nFaNotifyIndex] = *pFaNotifyPath;
	m_nFaNotifyIndex++;
	m_nFaNotifyCount++;
	pthread_mutex_unlock (&m_EventMutex);
	return TRUE;
}

BOOL CThreadFaNotifyEvent::ModFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;

	if(pFaNotifyPath == NULL || pFaNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tFaNotifyEventMap.begin();	end = m_tFaNotifyEventMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(!strcmp(pFindPath->acNotifyPath, pFaNotifyPath->acNotifyPath))
		{
			pFindPath->nOrderID = pFaNotifyPath->nOrderID;
			pthread_mutex_unlock (&m_EventMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_EventMutex);
	return FALSE;
}


BOOL CThreadFaNotifyEvent::GetFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	if(pFaNotifyPath == NULL || pFaNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tFaNotifyEventMap.begin();	end = m_tFaNotifyEventMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(!strcmp(pFindPath->acNotifyPath, pFaNotifyPath->acNotifyPath))
		{
			memcpy(pFaNotifyPath, pFindPath, sizeof(FANOTIFY_PATH));
			pthread_mutex_unlock (&m_EventMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_EventMutex);
	return FALSE;
}


UINT32 CThreadFaNotifyEvent::GetFaNotifyPathCount()
{
	UINT32 nCount = 0;
	pthread_mutex_lock (&m_EventMutex);
	nCount = m_nFaNotifyCount;
	pthread_mutex_unlock (&m_EventMutex);
	return nCount;
}

BOOL CThreadFaNotifyEvent::DelFaNotifyPath(INT32 nIndex)
{
	if(nIndex < 0)
		return FALSE;
	pthread_mutex_lock (&m_EventMutex);
	m_tFaNotifyEventMap.erase(nIndex);
	if(m_nFaNotifyCount > 0)
		m_nFaNotifyCount--;
	pthread_mutex_unlock (&m_EventMutex);
	return TRUE;
}


void CThreadFaNotifyEvent::ClearFaNotifyPath()
{
	pthread_mutex_lock (&m_EventMutex);
	if(m_nFaNotifyCount != 0)
	{
		m_tFaNotifyEventMap.clear();
		m_nFaNotifyCount = 0;
	}
	pthread_mutex_unlock (&m_EventMutex);
}



void CThreadFaNotifyEvent::ClearWatchFaNotify()
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	INT32 nNotifyFd = -1;
	UINT64 ddwEventMask = FAN_CLOSE_WRITE | FAN_OPEN_EXEC_PERM;
	CFaNotifyDlg* t_NotifyDlg = (CFaNotifyDlg *)m_pNotifyWnd;
	if(GetFaNotifyPathCount() == 0)
		return;

	if(GetFaNotifyFd(nNotifyFd) == FALSE)
	{
		ClearFaNotifyPath();
		return;
	}

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tFaNotifyEventMap.begin();	end = m_tFaNotifyEventMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(pFindPath->acNotifyPath[0] != 0)
		{
			fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, pFindPath->acNotifyPath);
		}
	}
	pthread_mutex_unlock (&m_EventMutex);

	ClearFaNotifyPath();
}


BOOL	CThreadFaNotifyEvent::ParseFilePath(PASI_CHK_INFO pInfo)
{
	INT32 i, nLen = 0;
	BOOL bIsSep = FALSE;
	if (pInfo == NULL || pInfo->acFullPath[0] == 0 || pInfo->nLen < 1)
	{
		return FALSE;
	}
	nLen = pInfo->nLen;
	for(i=nLen-1; i>=0; i--)
	{
		if(pInfo->acFullPath[i] == '/')	
		{
			bIsSep = TRUE;
			break;
		}
	}
	if(bIsSep == FALSE || i<0)
		return FALSE;

	if(i==0)
	{
		pInfo->acPath[0] = '/';
		pInfo->acPath[1] = '\0';
		if(nLen < MAX_FILE_NAME)
		{
			strncpy(pInfo->acFile, &pInfo->acFullPath[1], nLen-1);
			pInfo->acFile[nLen-1] = '\0'; 
		}
		else
		{
			strncpy(pInfo->acFile, &pInfo->acFullPath[1], MAX_FILE_NAME-1);
			pInfo->acFile[MAX_FILE_NAME-1] = '\0'; 
		}
	}
	else
	{
		strncpy(pInfo->acFile, &pInfo->acFullPath[i+1], MAX_FILE_NAME-1);
		pInfo->acFile[MAX_FILE_NAME-1] = '\0'; 
		strncpy(pInfo->acPath, pInfo->acFullPath, i);
		pInfo->acPath[i] = '\0'; 
	}
	return TRUE;
}

BOOL	CThreadFaNotifyEvent::GetFilePathFromFd(INT32 nFd, PASI_CHK_INFO pFileInfo)
{
	ssize_t nLen = 0;
	char acPath[MAX_TYPE_LEN] = {0,};

	if (nFd < 1 || pFileInfo == NULL)
	{
		return FALSE;
	}

	snprintf (acPath, MAX_TYPE_LEN-1, "/proc/self/fd/%d", nFd);
	acPath[MAX_TYPE_LEN-1] = 0;
	nLen = readlink (acPath, pFileInfo->acFullPath, MAX_PATH-1);
	if (nLen < 2 || nLen > MAX_PATH-1)
	{
		return FALSE;
	}
	pFileInfo->acFullPath[nLen] = '\0';
	pFileInfo->nLen = (INT32)nLen;

	if(nLen > 3)
	{
		if(!_stricmp(&pFileInfo->acFullPath[nLen-3], ".so"))
			return FALSE;
		else if(!_stricmp(&pFileInfo->acFullPath[nLen-2], ".o"))
			return FALSE;
	}
	if(ParseFilePath(pFileInfo) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL	CThreadFaNotifyEvent::GetProcPathFromPid(INT32 nPid, PASI_CHK_INFO pProcInfo)
{
	ssize_t nLen = 0;
	char acPath[MAX_TYPE_LEN] = {0,};

	if (nPid < 1 || pProcInfo == NULL)
	{
		return FALSE;
	}

	snprintf (acPath, MAX_TYPE_LEN-1, "/proc/%d/exe", nPid);
	acPath[MAX_TYPE_LEN-1] = 0;
	nLen = readlink (acPath, pProcInfo->acFullPath, MAX_PATH-1);
	if (nLen < 2 || nLen > MAX_PATH-1)
	{
		return FALSE;
	}
	pProcInfo->acFullPath[nLen] = '\0';
	pProcInfo->nLen = (INT32)nLen;
	if(ParseFilePath(pProcInfo) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

INT32	CThreadFaNotifyEvent::AnalyzeExecEvent2(INT32 nClientFd, PASI_CHK_FILE_PROC pChkFileProc, INT32 nCount)
{
	INT32 nRetVal = 0;
	INT32 nAcVal = RET_NONE;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	if(nClientFd == -1 || pChkFileProc == NULL)
	{
		printf("invalid input data");
		return -1;
	}

	do{
		strncpy(pChkFileProc->stProcInfo.acFullPath, "/usr/local/test", MAX_PATH-1);
		pChkFileProc->stProcInfo.acFullPath[MAX_PATH-1] = 0;
		pChkFileProc->stProcInfo.nLen = (INT32)strlen(pChkFileProc->stProcInfo.acFullPath);

		if(ParseFilePath(&pChkFileProc->stProcInfo) == FALSE)
		{
			nAcVal = RET_NONE;
			nRetVal = 0;
			break;
		}
		if(nCount == 0)
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/bin/ping", MAX_PATH-1);
		else if(nCount == 1)
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/bin/kmod", MAX_PATH-1);
		else if(nCount == 2)
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/bin/true", MAX_PATH-1);
		else if(nCount == 3)
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/usr/bin/tail", MAX_PATH-1);
		else if(nCount == 4)
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/usr/bin/vim.tiny", MAX_PATH-1);
		else if(nCount == 5)
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/usr/share/autojump/autojump", MAX_PATH-1);
		else
		{
			nAcVal = RET_NONE;
			nRetVal = 0;
			break;
		}

		pChkFileProc->stFileInfo.acFullPath[MAX_PATH-1] = 0;
		pChkFileProc->stFileInfo.nLen = (INT32)strlen(pChkFileProc->stFileInfo.acFullPath);
		if(ParseFilePath(&pChkFileProc->stFileInfo) == FALSE)
		{
			nAcVal = RET_NONE;
			break;
		}

		pChkFileProc->nCmdId = CMD_PIPE_PO_IN_CHK_WHITE;
		nRetVal = SockWrite(nClientFd,(PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			nAcVal = RET_NONE;
			nRetVal -= 10;
			break;
		}

		nRetVal = SockRecv(nClientFd,(PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			nAcVal = RET_NONE;
			nRetVal -= 20;
			break;
		}
		nAcVal = pChkFileProc->stRetInfo.nAcVal;
		nRetVal = 0;

	}while(FALSE);

	if(nAcVal == RET_DENY || nAcVal == RET_WARN)
	{
		//log
		printf("%s deny or warn", pChkFileProc->stFileInfo.acFullPath);
	}
	Sleep(2000);
	return nRetVal;
}


INT32	CThreadFaNotifyEvent::SendEventToServer(INT32 nNotifyFd, INT32 nClientFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc)
{
	struct fanotify_response stAccess;
	INT32 nRetVal = 0;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	INT32 nAcVal = RET_NONE;
	BOOL bIsAc = FALSE;
	struct fanotify_event_metadata *pEvent = (struct fanotify_event_metadata *)pMetaData;

	if(pEvent == NULL || pChkFileProc == NULL)
	{
		return -1;
	}

	if(nNotifyFd < 0 || nClientFd < 0)
	{
		return -2;
	}
	do{
		if (pEvent->mask & FAN_OPEN_EXEC_PERM)
		{
			pChkFileProc->nCmdId = CMD_PIPE_PO_IN_CHK_WHITE;
			bIsAc = TRUE;
		}
		else if(pEvent->mask & FAN_CLOSE_WRITE)
		{
			pChkFileProc->nCmdId = CMD_PIPE_PO_IN_CREATE_FILE;
		}
		else
		{
			nAcVal = RET_NONE;
			break;
		}

		if(GetProcPathFromPid(pEvent->pid, &pChkFileProc->stProcInfo) == FALSE)
		{
			snprintf(pChkFileProc->stProcInfo.acFullPath, MAX_PATH-1, "pid/%d", pEvent->pid);
			strncpy(pChkFileProc->stProcInfo.acPath, "pid", MAX_PATH-1);
			snprintf(pChkFileProc->stProcInfo.acFile, MAX_FILE_NAME-1, "%d", pEvent->pid);
		}

		if(GetFilePathFromFd(pEvent->fd, &pChkFileProc->stFileInfo) == FALSE)
		{
			nAcVal = RET_NONE;
			nRetVal = 0;
			break;
		}

		nRetVal = SockWrite(nClientFd,(PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			nAcVal = RET_NONE;
			nRetVal -= 10;
			break;
		}

		nRetVal = SockRecv(nClientFd,(PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			nAcVal = RET_NONE;
			nRetVal -= 20;
			break;
		}
		if (bIsAc == TRUE)
			nAcVal = pChkFileProc->stRetInfo.nAcVal;
		else
			nAcVal = RET_NONE;
		nRetVal = 0;
	}while(FALSE);


	stAccess.fd = pEvent->fd;
	stAccess.response = FAN_ALLOW;
	if(nAcVal == RET_DENY)
	{
		stAccess.response = FAN_DENY;
	}
	write (nNotifyFd, &stAccess, sizeof(stAccess));
	if(pEvent->fd > 0)
	{
		close(pEvent->fd);
		pEvent->fd = -1;
	}
	return nRetVal;
}

INT32	CThreadFaNotifyEvent::AnalyzeFaNotifyEvent(INT32 nNotifyFd, INT32 nClientFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc)
{
	struct fanotify_event_metadata *pEvent = NULL;
	INT32 nRetVal = 0;

	if(pMetaData == NULL || pChkFileProc == NULL)
	{
		return -1;
	}

	if(nNotifyFd < 0 || nClientFd < 0)
	{
		return -2;
	}
	pEvent = (struct fanotify_event_metadata *)pMetaData;
	if(pEvent->fd < 1)
	{
		return 0;
	}

	if (pEvent->mask & FAN_CLOSE_WRITE || pEvent->mask & FAN_OPEN_EXEC_PERM)
	{
		nRetVal = SendEventToServer(nNotifyFd, nClientFd, pMetaData, pChkFileProc);
	}

	if(pEvent->fd > 0)
	{
		close(pEvent->fd);
		pEvent->fd = -1;
	}
	return nRetVal;
}
//---------------------------------------------------------------------------

INT32		CThreadFaNotifyEvent::CheckFaNotifyFile(INT32 nNotifyFd, INT32 nClientFd, char *pcBuffer, PASI_CHK_FILE_PROC pChkFileProc)
{
	INT32 nRetVal = 0;
	ssize_t nLength = 0;
	struct pollfd stPollFd;
	struct fanotify_event_metadata *pMetaData = NULL;

	if(pcBuffer == NULL || pChkFileProc == NULL)
	{
		return -1;
	}

	if(nNotifyFd < 0 || nClientFd < 0)
	{
		return -2;
	}

	stPollFd.fd = nNotifyFd;
	stPollFd.events = POLLIN;

	nRetVal = poll(&stPollFd, 1, 1000);
	if (nRetVal < 0)
	{
		return -3;
	}
	else if(nRetVal == 0)
	{
		return 0;
	}

	if (stPollFd.revents & POLLIN)
	{
		nLength = read (stPollFd.fd, pcBuffer, ASI_FANOTIFY_BUFF_MAX);
		if (nLength > 0)
		{
			pcBuffer[ASI_FANOTIFY_BUFF_MAX] = 0;
			pMetaData = (struct fanotify_event_metadata *)pcBuffer;
			while (FAN_EVENT_OK (pMetaData, nLength))
			{
				nRetVal = AnalyzeFaNotifyEvent(nNotifyFd, nClientFd, (PVOID)pMetaData, pChkFileProc);
				if(nRetVal < 0)
				{
					nRetVal -= 100;
					return nRetVal;
				}
				if (pMetaData->fd > 0)
				{
					close(pMetaData->fd);
					pMetaData->fd = -1;
				}
				pMetaData = FAN_EVENT_NEXT (pMetaData, nLength);
			}
		}
	}

	return 0;
}
