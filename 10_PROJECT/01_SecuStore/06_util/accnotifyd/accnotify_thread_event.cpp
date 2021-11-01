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
#include "accnotify_dlg.h"
#include "accnotify_thread_event.h"

#define FAN_OPEN_PERM   0x00010000

/////////////////////////////////////////////////////////////////////////////
// CThreadAccNotifyEvent

CThreadAccNotifyEvent*	t_ThreadAccNotifyEvent = NULL;

CThreadAccNotifyEvent::CThreadAccNotifyEvent()
{
	m_nShmId = -1;
	m_nAccNotifyCount = 0;
	m_nAccNotifyIndex = 0;
	m_pNotifyWnd = NULL;
	m_pString = NULL;
	m_bIsConnected = FALSE;
	pthread_mutex_init(&m_EventMutex, NULL);
}

CThreadAccNotifyEvent::~CThreadAccNotifyEvent()
{
	pthread_mutex_destroy(&m_EventMutex);
}

INT32	CThreadAccNotifyEvent::SetNotifyWnd(HWND pNotifyWnd)
{
	if(pNotifyWnd == NULL)
	{
		return -1;
	}
	m_pNotifyWnd = pNotifyWnd;
	return 0;
}

INT32	CThreadAccNotifyEvent::InitShmEnv()
{
	INT32 nRetVal = 0;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;

	do{
		if(m_nShmId == -1)
		{
			m_nShmId = shmget((key_t)SM_CHK_ACCESS_FILE_KEY, 0, NULL);
			if(m_nShmId == -1)
			{
				if(t_AccNotifyDlg != NULL)
					t_AccNotifyDlg->WriteLogE("fail to get shared memory(%d)", errno);
				nRetVal = -1;
				break;
			}
		}

		m_pString = (char *)shmat(m_nShmId, NULL, 0);
		if(m_pString == (char *)-1)
		{
			if(t_AccNotifyDlg != NULL)
				t_AccNotifyDlg->WriteLogE("fail to attach shared memory(%d)", errno);
			m_pString = NULL;
			nRetVal = -2;
			break;
		}

		m_bIsConnected = TRUE;
		nRetVal = 0;
	}while(FALSE);

	if(nRetVal < 0)
	{
		UninitShmEnv();
	}
	return nRetVal;
}

VOID	CThreadAccNotifyEvent::UninitShmEnv()
{
	if(m_pString != NULL)
	{
		shmdt(m_pString);
		m_pString = NULL;
	}
	m_bIsConnected = FALSE;
}

INT32 CThreadAccNotifyEvent::ShmWrite(PVOID pWriteData, INT32 nReqSize)
{
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	if(pWriteData == NULL || nReqSize != sizeof(ASI_CHK_FILE_PROC) || m_pString == NULL)
	{
		return -1;
	}
	pChkFileProc = (PASI_CHK_FILE_PROC)pWriteData;
	pChkFileProc->nState = SM_READ_SERVER_FLAG;
	memcpy(m_pString, pWriteData, sizeof(ASI_CHK_FILE_PROC));
	return 0;
}


INT32 CThreadAccNotifyEvent::ShmRecv(PVOID pRecvData, INT32 nReqSize)
{
	INT32 nContinueCount = 0;
	INT32 nRetVal = 0;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	if(pRecvData == NULL || nReqSize != sizeof(ASI_CHK_FILE_PROC) || m_pString == NULL)
	{
		return -1;
	}

	pChkFileProc = (PASI_CHK_FILE_PROC)m_pString;
	while (nContinueCount < 200)
	{
		if(pChkFileProc->nState == SM_READ_CLIENT_FLAG)
		{
			memcpy(pRecvData, m_pString, sizeof(ASI_CHK_FILE_PROC));
			pChkFileProc->nState = SM_WRITE_CLIENT_FLAG;
			return 0;
		}
		nContinueCount++;
		Sleep(10);
	}
	return 1;
}


INT32	CThreadAccNotifyEvent::InitAccNotifyEvent(INT32 &nNotifyFd, INT32 nTestMode)
{
	INT32 nRetVal = 0;
	INT32 nNFd = -1;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;

	nRetVal = CheckWhitePatternFile();
	if(nRetVal < 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	do{
		nRetVal = InitShmEnv();
		if(nRetVal < 0)
		{
			nRetVal -= 20;
			break;
		}

		if(nTestMode == 1)
		{
			nRetVal = 0;
			break;
		}

		if(InitAccNotifyFd(nNFd) == FALSE)
		{
			if(t_AccNotifyDlg != NULL)
				t_AccNotifyDlg->WriteLogE("fail to init acc notify fd (%d)", errno);
			nRetVal = -3;
			break;
		}

		nRetVal = AddWatchAccNotify(1, nNFd, ASI_ACCNOTIFY_PATH);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}

		nNotifyFd = nNFd;
		memset(m_acSrcPath, 0, MAX_PATH);
		nRetVal = 0;
	}while(FALSE);

	if(nRetVal != 0)
	{
		UninitAccNotifyEvent(nTestMode);
	}
	return nRetVal;
}

VOID	CThreadAccNotifyEvent::UninitAccNotifyEvent(INT32 nTestMode)
{
	memset(m_acSrcPath, 0, MAX_PATH);
	if(nTestMode == 0)
	{
		ClearWatchAccNotify();
		CloseAccNotifyFd();
	}
	UninitShmEnv();
}


BOOL CThreadAccNotifyEvent::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadAccNotifyEvent::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadAccNotifyEvent message handlers

INT32 CThreadAccNotifyEvent::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 nRetVal = 0;
	INT32 nNotifyFd = -1;
	INT32 nTestMode = 0;
	INT32 i, nTestCount = 0;
	char *pcBuffer = NULL;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	pid_t tid = 0;
	for(i=0; i<30; i++)
	{
		if(check_proc_exist_by_name(NANNY_AGENT_IDENT, 0) == ASI_PROC_EXIST)
		{
			Sleep(1000);
			break;
		}
		Sleep(1000);
	}

	tid = syscall(SYS_gettid);
	nRetVal = setpriority(PRIO_PROCESS, tid, -10);
	if(nRetVal < 0)
	{
		if(t_AccNotifyDlg != NULL)
			t_AccNotifyDlg->WriteLogE("fail to set priority: [%s][%d]", m_strThreadName.c_str(), errno);
		return -1;
	}

	pcBuffer = (char *)malloc(ASI_FANOTIFY_BUFF_MAX+1);
	if(pcBuffer == NULL)
	{
		if(t_AccNotifyDlg != NULL)
			t_AccNotifyDlg->WriteLogE("fail to allocate memory : [%s][%d]", m_strThreadName.c_str(), errno);
		return -2;
	}
	memset(pcBuffer, 0, ASI_FANOTIFY_BUFF_MAX+1);

	pChkFileProc = (PASI_CHK_FILE_PROC)malloc(sizeof(ASI_CHK_FILE_PROC));
	if(pChkFileProc == NULL)
	{
		if(t_AccNotifyDlg != NULL)
			t_AccNotifyDlg->WriteLogE("fail to allocate memory : [%s][%d]", m_strThreadName.c_str(), errno);
		safe_free(pcBuffer);
		return -3;
	}
	m_nPause = 1;
	nRetVal = InitAccNotifyEvent(nNotifyFd, nTestMode);
	if(nRetVal == 0)
	{
		m_nPause = 0;
		nRetVal = SendReqPoliyToServer(pChkFileProc);
		if(nRetVal < 0)
			m_nPause = 1;
	}

	m_nRunFlag = 1;

	while(GetContinue())
	{
		if(!m_nPause)
		{
			memset(pChkFileProc, 0, sizeof(ASI_CHK_FILE_PROC));
			if(nTestMode == 0)
				nRetVal = CheckAccNotifyFile(nNotifyFd, pcBuffer, pChkFileProc);
			else
				nRetVal = AnalyzeAccNotifyEvent2(pChkFileProc, nTestCount++%6);
			if(nRetVal != 0)
			{
				if(nRetVal == -1)
				{
					break;
				}
				else
				{
					UninitAccNotifyEvent(nTestMode);
					if(t_AccNotifyDlg != NULL)
					{
						t_AccNotifyDlg->ClearPolicy();
						t_AccNotifyDlg->WriteLogN("reconnect nannysvc : [%s][%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
					}
					nNotifyFd = -1;
					m_nPause = 1;
					Sleep(1000);
				}
			}
		}
		else
		{
			nRetVal = InitAccNotifyEvent(nNotifyFd, nTestMode);
			if(nRetVal == 0)
			{
				m_nPause = 0;
				nRetVal = SendReqPoliyToServer(pChkFileProc);
				if(nRetVal < 0)
					m_nPause = 1;
			}
			else
			{
				m_nPause = 1;
				Sleep(1000);
			}
		}
	}
	UninitAccNotifyEvent(nTestMode);
	if(t_AccNotifyDlg != NULL)
	{
		if(!GetContinue())
			t_AccNotifyDlg->WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());
	}
	safe_free(pcBuffer);
	safe_free(pChkFileProc);
	return 0;
}

INT32 CThreadAccNotifyEvent::CheckWhitePatternFile()
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

BOOL CThreadAccNotifyEvent::InitAccNotifyFd(INT32 &nNotifyFd)
{
	BOOL bRetVal = TRUE;
	pthread_mutex_lock (&m_EventMutex);
	if ( m_nAccNotifyFd < 1 )
	{
		m_nAccNotifyFd = fanotify_init(FAN_CLOEXEC | FAN_CLASS_CONTENT | FAN_NONBLOCK, O_RDONLY | O_LARGEFILE);
		if ( m_nAccNotifyFd < 0 )
		{
			bRetVal = FALSE;
		}
		else
		{
			nNotifyFd = m_nAccNotifyFd;
		}
	}
	pthread_mutex_unlock (&m_EventMutex);
	return bRetVal;
}

BOOL CThreadAccNotifyEvent::GetAccNotifyFd(INT32 &nNotifyFd)
{
	BOOL bRetVal = FALSE;
	pthread_mutex_lock (&m_EventMutex);
	if ( m_nAccNotifyFd < 0 )
	{
		bRetVal = FALSE;
	}
	else
	{
		nNotifyFd = m_nAccNotifyFd;
		bRetVal = TRUE;
	}
	pthread_mutex_unlock (&m_EventMutex);
	return bRetVal;
}

VOID CThreadAccNotifyEvent::CloseAccNotifyFd()
{
	pthread_mutex_lock (&m_EventMutex);
	if ( m_nAccNotifyFd >= 0 )
	{
		close(m_nAccNotifyFd);
		m_nAccNotifyFd = -1;
	}
	pthread_mutex_unlock (&m_EventMutex);
}

INT32 CThreadAccNotifyEvent::AddAccNotifyMark(INT32 nNotifyFd, PFANOTIFY_PATH pAccNotifyPath)
{
	INT32 nRetVal = 0;
	UINT64 ddwEventMask = FAN_OPEN_PERM;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	if(nNotifyFd < 0 || pAccNotifyPath == NULL || pAccNotifyPath->acNotifyPath[0] == 0)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[AddAccNotifyMark] invalid input data (%d)", nNotifyFd);
		return -1;
	}

	if(IsAccNotifyPath(pAccNotifyPath->acNotifyPath) == TRUE)
	{
		ModAccNotifyPath(pAccNotifyPath);
		return 0;
	}

	nRetVal = fanotify_mark(nNotifyFd, FAN_MARK_ADD | FAN_MARK_MOUNT, ddwEventMask, AT_FDCWD, pAccNotifyPath->acNotifyPath);
	if(nRetVal < 0)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[AddAccNotifyMark] fail to add accnotify watch [%s] : [%d][%d]", pAccNotifyPath->acNotifyPath, nNotifyFd, errno);
		return -3;
	}

	if(AddAccNotifyPath(pAccNotifyPath) == FALSE)
	{
		fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, pAccNotifyPath->acNotifyPath);
		return -4;
	}
	return 0;
}

INT32 CThreadAccNotifyEvent::RemoveAccNotifyMark(PFANOTIFY_PATH pAccNotifyPath)
{
	INT32 nNotifyFd = -1;
	INT32 nRetVal = 0;
	UINT64 ddwEventMask = FAN_OPEN_PERM;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	if(pAccNotifyPath == NULL || pAccNotifyPath->acNotifyPath[0] == 0)
	{
		return -1;
	}

	if(GetAccNotifyFd(nNotifyFd) == FALSE)
	{
		return -2;
	}

	if(GetAccNotifyPath(pAccNotifyPath) == FALSE)
	{
		return -3;
	}

	fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, pAccNotifyPath->acNotifyPath);

	if(DelAccNotifyPath(pAccNotifyPath->nIndex) == FALSE)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[RemoveAccNotifyMark] fail to del accnotify path [%s][%d]", pAccNotifyPath->acNotifyPath, pAccNotifyPath->nIndex);
		fanotify_mark(nNotifyFd, FAN_MARK_ADD | FAN_MARK_MOUNT, ddwEventMask, AT_FDCWD, pAccNotifyPath->acNotifyPath);
		return -5;
	}
	return 0;
}

INT32 CThreadAccNotifyEvent::AddWatchAccNotify(INT32 nOrderID, INT32 nNotifyFd, char *pPath)
{
	INT32 nLen = 0;
	INT32 nRetVal = 0;
	PFANOTIFY_PATH pAccNotifyPath = NULL;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	if(pPath == NULL || pPath[0] == 0 || nNotifyFd < 1)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[AddWatchAccNotify] invalid input data");
		return -1;
	}
	nLen = (INT32)strlen(pPath);
	if(nLen < 1 || nLen > MAX_PATH-2)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[AddWatchAccNotify] invalid path len (%d)", nLen);
		return -2;
	}

	pAccNotifyPath = (PFANOTIFY_PATH)malloc(sizeof(FANOTIFY_PATH));
	if(pAccNotifyPath == NULL)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[AddWatchAccNotify] fail to allocate memory : [%d]", errno);
		return -3;
	}
	memset(pAccNotifyPath, 0, sizeof(FANOTIFY_PATH));

	strncpy(pAccNotifyPath->acNotifyPath, pPath, MAX_PATH-1);
	pAccNotifyPath->acNotifyPath[MAX_PATH-1] = 0;

	if(nLen > 2 && pAccNotifyPath->acNotifyPath[nLen-1] == '/')
	{
		pAccNotifyPath->acNotifyPath[nLen-1] = '\0';
		nLen--;
	}

	pAccNotifyPath->nPathLen = nLen;
	pAccNotifyPath->nOrderID = nOrderID;

	nRetVal = AddAccNotifyMark(nNotifyFd, pAccNotifyPath);
	if(nRetVal != 0)
	{
		nRetVal -= 10;
		safe_free(pAccNotifyPath);
		return nRetVal;
	}
	safe_free(pAccNotifyPath);
	return 0;
}

INT32 CThreadAccNotifyEvent::RemoveWatchAccNotify(char *pPath)
{
	INT32 nLen = 0;
	INT32 nRetVal = 0;
	PFANOTIFY_PATH pAccNotifyPath = NULL;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	if(pPath == NULL || pPath[0] == 0)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[DelWatchAccNotify] invalid input data");
		return -1;
	}

	nLen = (INT32)strlen(pPath);
	if(nLen < 1 || nLen > MAX_PATH-2)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[DelWatchAccNotify] invalid path len (%d)", nLen);
		return -2;
	}

	pAccNotifyPath = (PFANOTIFY_PATH)malloc(sizeof(FANOTIFY_PATH));
	if(pAccNotifyPath == NULL)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[DelWatchAccNotify] fail to allocate memory : [%d]", errno);
		return -3;
	}
	memset(pAccNotifyPath, 0, sizeof(FANOTIFY_PATH));

	strncpy(pAccNotifyPath->acNotifyPath, pPath, MAX_PATH-1);
	pAccNotifyPath->acNotifyPath[MAX_PATH-1] = 0;

	if(nLen > 2 && pAccNotifyPath->acNotifyPath[nLen-1] == '/')
	{
		pAccNotifyPath->acNotifyPath[nLen-1] = '\0';
	}

	nRetVal = RemoveAccNotifyMark(pAccNotifyPath);
	if(nRetVal != 0)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[AddWatchAccNotify] fail to remove accnotify mark : [%d]", nRetVal);
		nRetVal -= 10;
		safe_free(pAccNotifyPath);
		return nRetVal;
	}
	safe_free(pAccNotifyPath);
	return 0;
}


BOOL CThreadAccNotifyEvent::IsAccNotifyPath(char *pPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	if(pPath == NULL || pPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tAccNotifyEventMap.begin();	end = m_tAccNotifyEventMap.end();
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

BOOL CThreadAccNotifyEvent::AddAccNotifyPath(PFANOTIFY_PATH pAccNotifyPath)
{
	if(pAccNotifyPath == NULL || pAccNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	pAccNotifyPath->nIndex = m_nAccNotifyIndex;
	m_tAccNotifyEventMap[m_nAccNotifyIndex] = *pAccNotifyPath;
	m_nAccNotifyIndex++;
	m_nAccNotifyCount++;
	pthread_mutex_unlock (&m_EventMutex);
	return TRUE;
}

BOOL CThreadAccNotifyEvent::ModAccNotifyPath(PFANOTIFY_PATH pAccNotifyPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;

	if(pAccNotifyPath == NULL || pAccNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tAccNotifyEventMap.begin();	end = m_tAccNotifyEventMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(!strcmp(pFindPath->acNotifyPath, pAccNotifyPath->acNotifyPath))
		{
			pFindPath->nOrderID = pAccNotifyPath->nOrderID;
			pthread_mutex_unlock (&m_EventMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_EventMutex);
	return FALSE;
}


BOOL CThreadAccNotifyEvent::GetAccNotifyPath(PFANOTIFY_PATH pAccNotifyPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	if(pAccNotifyPath == NULL || pAccNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tAccNotifyEventMap.begin();	end = m_tAccNotifyEventMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(!strcmp(pFindPath->acNotifyPath, pAccNotifyPath->acNotifyPath))
		{
			memcpy(pAccNotifyPath, pFindPath, sizeof(FANOTIFY_PATH));
			pthread_mutex_unlock (&m_EventMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_EventMutex);
	return FALSE;
}


UINT32 CThreadAccNotifyEvent::GetAccNotifyPathCount()
{
	UINT32 nCount = 0;
	pthread_mutex_lock (&m_EventMutex);
	nCount = m_nAccNotifyCount;
	pthread_mutex_unlock (&m_EventMutex);
	return nCount;
}

BOOL CThreadAccNotifyEvent::DelAccNotifyPath(INT32 nIndex)
{
	if(nIndex < 0)
		return FALSE;
	pthread_mutex_lock (&m_EventMutex);
	m_tAccNotifyEventMap.erase(nIndex);
	if(m_nAccNotifyCount > 0)
		m_nAccNotifyCount--;
	pthread_mutex_unlock (&m_EventMutex);
	return TRUE;
}


void CThreadAccNotifyEvent::ClearAccNotifyPath()
{
	pthread_mutex_lock (&m_EventMutex);
	if(m_nAccNotifyCount != 0)
	{
		m_tAccNotifyEventMap.clear();
		m_nAccNotifyCount = 0;
	}
	pthread_mutex_unlock (&m_EventMutex);
}



void CThreadAccNotifyEvent::ClearWatchAccNotify()
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	INT32 nNotifyFd = -1;
	UINT64 ddwEventMask = FAN_OPEN_PERM;

	if(GetAccNotifyPathCount() == 0)
		return;

	if(GetAccNotifyFd(nNotifyFd) == FALSE)
	{
		ClearAccNotifyPath();
		return;
	}

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tAccNotifyEventMap.begin();	end = m_tAccNotifyEventMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(pFindPath->acNotifyPath[0] != 0)
		{
			fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, pFindPath->acNotifyPath);
		}
	}
	pthread_mutex_unlock (&m_EventMutex);

	ClearAccNotifyPath();
}


BOOL	CThreadAccNotifyEvent::ParseFilePath(PASI_CHK_INFO pInfo)
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

BOOL	CThreadAccNotifyEvent::GetFilePathFromFd(INT32 nFd, PASI_CHK_INFO pFileInfo)
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

	if(nLen > 4)
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

	if(!_strnicmp(pFileInfo->acFullPath, "/bin/", 5))
		return TRUE;

	if(nLen > 9 && !_strnicmp(&pFileInfo->acFullPath[nLen-9], "_file_", 6))
		return TRUE;

	return FALSE;
}

BOOL	CThreadAccNotifyEvent::GetProcPathFromPid(INT32 nPid, PASI_CHK_INFO pProcInfo)
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

INT32		CThreadAccNotifyEvent::ChkInPtn(PASI_CHK_FILE_PROC pChkFileProc, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType)
{
	UINT32 nBLCount = 0;
	UINT32 nWLCount = 0;
	UINT32 nPtnRisk = PTN_FB_PTN_RISK_UNKNOW;
	CString strFullPath;
	CString strHash;
	ASI_POLICY_INFO stPolicyInfo;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	if(pChkFileProc == NULL || t_AccNotifyDlg == NULL)
		return 0;

	memset(&stPolicyInfo, 0, sizeof(ASI_POLICY_INFO));
	t_AccNotifyDlg->GetPolicyInfo(&stPolicyInfo);

	if(stPolicyInfo.nOpMode == STATUS_USED_MODE_OFF)
		return 0;

	nBLCount = t_AccNotifyDlg->GetBLPathCnt();
	if(nBLCount == 0)
	{
		stPolicyInfo.nBLUsedMode  = STATUS_USED_MODE_OFF;
	}

	nWLCount = t_AccNotifyDlg->GetWLPathCnt();
	if(nWLCount == 0)
	{
		stPolicyInfo.nWLUsedMode = STATUS_USED_MODE_OFF;
	}

	if(stPolicyInfo.nBLUsedMode == STATUS_USED_MODE_OFF && stPolicyInfo.nWLUsedMode == STATUS_USED_MODE_OFF)
		return 0;

	if(stPolicyInfo.nOpMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	strFullPath.Format("%s", pChkFileProc->stFileInfo.acFullPath);
	nPtnRisk = PTN_FB_PTN_RISK_UNKNOW;
	if(stPolicyInfo.nWLUsedMode != STATUS_USED_MODE_OFF && nWLCount != 0)
	{
		if(t_AccNotifyDlg->FindWLPathStr(strFullPath, strHash))
		{
			nPtnRisk = PTN_FB_PTN_RISK_WHITE;
			strncpy(pChkFileProc->stRetInfo.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
			pChkFileProc->stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		}
	}

	if(nPtnRisk == PTN_FB_PTN_RISK_UNKNOW && stPolicyInfo.nBLUsedMode != STATUS_USED_MODE_OFF && nBLCount != 0)
	{
		if(t_AccNotifyDlg->FindBLPathStr(strFullPath, strHash))
		{
			nPtnRisk = PTN_FB_PTN_RISK_BLACK;
			strncpy(pChkFileProc->stRetInfo.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
			pChkFileProc->stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		}
	}

	nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
	if(nPtnRisk == PTN_FB_PTN_RISK_WHITE)
	{
		nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_WL + stPolicyInfo.nWLID;
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
	}
	else if(nPtnRisk == PTN_FB_PTN_RISK_BLACK)
	{
		nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_BL + stPolicyInfo.nBLID;
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
	}
	return 1;
}

VOID	CThreadAccNotifyEvent::SetRetValValue(ASI_RET_INFO *pstRetInfo, INT32 nAcVal, INT32 nBlockMode, INT32 nIsWarning, INT32 nPolicyType)
{
	if(pstRetInfo != NULL)
	{
		pstRetInfo->nAcVal = nAcVal;
		pstRetInfo->nBlockMode = nBlockMode;
		pstRetInfo->nPolicyType = nPolicyType;
		pstRetInfo->nIsWarning = nIsWarning;
	}
}


INT32	CThreadAccNotifyEvent::AnalyzeAccEvent(PASI_CHK_FILE_PROC pChkFileProc)
{
	INT32 nRetVal = 0;
	INT32 nBlockMode = SS_PO_CTL_PROC_BLOCK_MODE_ALLOW;
	INT32 nAcVal = RET_NONE;
	INT32 nIsWarning = 0;
	INT32 nPolicyType = 0;

	if(pChkFileProc == NULL)
	{
		return RET_NONE;
	}

	do{
		nRetVal = ChkInPtn(pChkFileProc, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal != 1)
		{
			nAcVal = RET_NONE;
			break;
		}
		if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
		{
			if(nIsWarning == 1)
				nAcVal = RET_WARN;
			else
				nAcVal = RET_DENY;
			SetRetValValue(&pChkFileProc->stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
		}
		else
		{
			nAcVal = RET_NONE;
			SetRetValValue(&pChkFileProc->stRetInfo, nAcVal, 0, 0, 0);
		}
	}while(FALSE);

	return nAcVal;
}



INT32	CThreadAccNotifyEvent::SendEventToServer(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc)
{
	struct fanotify_response stAccess;
	INT32 nRetVal = 0;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	INT32 nAcVal = RET_NONE;
	struct fanotify_event_metadata *pEvent = (struct fanotify_event_metadata *)pMetaData;

	if(pEvent == NULL || pChkFileProc == NULL)
	{
		return -1;
	}

	if(nNotifyFd < 0)
	{
		return -2;
	}
	do{
		if(GetProcPathFromPid(pEvent->pid, &pChkFileProc->stProcInfo) == FALSE)
		{
			nAcVal = RET_NONE;
			nRetVal = 0;
			break;
		}

		if(GetFilePathFromFd(pEvent->fd, &pChkFileProc->stFileInfo) == FALSE)
		{
			nAcVal = RET_NONE;
			nRetVal = 0;
			break;
		}

		if(_stricmp(pChkFileProc->stProcInfo.acFile, "cp_test"))
		{
			nAcVal = RET_NONE;
			nRetVal = 0;
			break;
		}

		if(m_acSrcPath[0] == 0)
		{
			strncpy(m_acSrcPath, pChkFileProc->stFileInfo.acFullPath, MAX_PATH-1);
			m_acSrcPath[MAX_PATH-1] = 0;
			nAcVal = RET_NONE;
			nRetVal = 0;
			break;
		}
		else
		{
			memset(m_acSrcPath, 0, MAX_PATH);
		}

		nAcVal = AnalyzeAccEvent(pChkFileProc);
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

	if(nAcVal == RET_DENY || nAcVal == RET_WARN)
	{
		if(m_acSrcPath[0] != 0)
		{
			strncpy(pChkFileProc->stFileInfo.acPath, m_acSrcPath, MAX_PATH-1);
			pChkFileProc->stFileInfo.acPath[MAX_PATH-1] = 0;
		}
		pChkFileProc->nCmdId = CMD_PIPE_PO_IN_ACCESS_FILE;
		nRetVal = ShmWrite((PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			return nRetVal;
		}
	}
	return nRetVal;
}

INT32	CThreadAccNotifyEvent::SendReqPoliyToServer(PASI_CHK_FILE_PROC pChkFileProc)
{
	INT32 i, nRetVal = 0;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;

	if(pChkFileProc == NULL)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[SendReqPoliyToServer] invalid input data");
		return -1;
	}
	pChkFileProc->nCmdId = CMD_PIPE_REQ_ACCESS_INFO;
	nRetVal = ShmWrite((PVOID)pChkFileProc, nSize);
	if(nRetVal < 0)
	{
		nRetVal -= 10;
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[SendReqPoliyToServer] fail to sock write %d %d", nRetVal, errno);
		return nRetVal;
	}
	for(i=0; i<100; i++)
	{
		nRetVal = t_AccNotifyDlg->GetPolicyState();
		if(nRetVal == 1)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogN("success to recv acc notidy policy");
			break;
		}
		Sleep(100);
	}
	return 0;
}


INT32	CThreadAccNotifyEvent::AnalyzeAccNotifyEvent(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc)
{
	struct fanotify_event_metadata *pEvent = NULL;
	INT32 nRetVal = 0;

	if(pMetaData == NULL || pChkFileProc == NULL)
	{
		return -1;
	}

	if(nNotifyFd < 0)
	{
		return -2;
	}
	pEvent = (struct fanotify_event_metadata *)pMetaData;
	if(pEvent->fd < 1)
	{
		return 0;
	}

	if (pEvent->mask & FAN_OPEN_PERM)
	{
		nRetVal = SendEventToServer(nNotifyFd, pMetaData, pChkFileProc);
	}

	if(pEvent->fd > 0)
	{
		close(pEvent->fd);
		pEvent->fd = -1;
	}
	return nRetVal;
}
//---------------------------------------------------------------------------

INT32		CThreadAccNotifyEvent::CheckAccNotifyFile(INT32 nNotifyFd, char *pcBuffer, PASI_CHK_FILE_PROC pChkFileProc)
{
	INT32 nRetVal = 0;
	ssize_t nLength = 0;
	struct pollfd stPollFd;
	struct fanotify_event_metadata *pMetaData = NULL;

	if(pcBuffer == NULL || pChkFileProc == NULL)
	{
		return -1;
	}

	if(nNotifyFd < 0)
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
				nRetVal = AnalyzeAccNotifyEvent(nNotifyFd, (PVOID)pMetaData, pChkFileProc);
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

INT32	CThreadAccNotifyEvent::AnalyzeAccNotifyEvent2(PASI_CHK_FILE_PROC pChkFileProc, INT32 nCount)
{
	INT32 nRetVal = 0;
	INT32 nAcVal = RET_NONE;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	INT32 nBlockMode = 0;
	INT32 nIsWarning = 0;
	INT32 nPolicyType = 0;
	if(pChkFileProc == NULL)
	{
		printf("invalid input data");
		return -1;
	}

	do{
		strncpy(pChkFileProc->stProcInfo.acFullPath, "/bin/cp_test", MAX_PATH-1);
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

		nRetVal = ChkInPtn(pChkFileProc, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal != 1)
		{
			nAcVal = RET_NONE;
			break;
		}
		if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
		{
			if(nIsWarning == 1)
				nAcVal = RET_WARN;
			else
				nAcVal = RET_DENY;
			SetRetValValue(&pChkFileProc->stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
		}
		else
		{
			nAcVal = RET_NONE;
			SetRetValValue(&pChkFileProc->stRetInfo, nAcVal, 0, 0, 0);
		}
		nRetVal = 0;
	}while(FALSE);

	if(nAcVal == RET_DENY || nAcVal == RET_WARN)
	{
		pChkFileProc->nCmdId = CMD_PIPE_PO_IN_ACCESS_FILE;
		nRetVal = ShmWrite((PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
		}
		//log
		fprintf(stderr, "%s deny or warn\n", pChkFileProc->stFileInfo.acFullPath);
	}
	Sleep(2000);
	return nRetVal;
}
