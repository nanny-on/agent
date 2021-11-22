/*
 * Copyright (C) 2021-2026 ASHINi Corp. 
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; either 
 * version 3 of the License, or any later version, with the additional
 * special exception to link portions of this program with the OpenSSL
 * library. See LICENSE for more details. 
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
#include "accnotify_thread_client_socket.h"
#include "accnotify_thread_server_socket.h"
#include "accnotify_thread_event.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadAccNotifyEvent

CThreadAccNotifyEvent*	t_ThreadAccNotifyEvent = NULL;

static char					g_acSrcPath[MAX_PATH];

CThreadAccNotifyEvent::CThreadAccNotifyEvent()
{
	m_nAccNotifyFd = -1;
	m_nAccNotifyCount = 0;
	m_nAccNotifyIndex = 0;
	m_pNotifyWnd = NULL;
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

INT32	CThreadAccNotifyEvent::InitAccNotifyEvent(INT32 &nNotifyFd, INT32 nTestMode)
{
	INT32 nRetVal = 0;
	INT32 i, nPolUse = 0;
	INT32 nNFd = -1;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;

	nRetVal = CheckWhitePatternFile();
	if(nRetVal < 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	for(i=0; i<50; i++)
	{
		if(t_ThreadAccServerSocket != NULL)
			t_ThreadAccServerSocket->GetPolicyUse(nPolUse);
		if(nPolUse == 1)
			break;
		Sleep(500);
	}

	if(nPolUse == 0)
	{
		if(t_AccNotifyDlg != NULL)
			t_AccNotifyDlg->WriteLogE("[%s] fail to load policy info", m_strThreadName.c_str());

		return -1;
	}

	do{
		if(nTestMode == 1)
		{
			nRetVal = 0;
			break;
		}

		if(InitAccNotifyFd(nNFd) == FALSE)
		{
			if(t_AccNotifyDlg != NULL)
				t_AccNotifyDlg->WriteLogE("[%s] fail to init acc notify fd (%d)", errno, m_strThreadName.c_str());
			nRetVal = -2;
			break;
		}

		nRetVal = AddWatchAccNotify(1, nNFd, ASI_ACCNOTIFY_PATH);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}

		nNotifyFd = nNFd;
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
	if(nTestMode == 0)
	{
		ClearWatchAccNotify();
		CloseAccNotifyFd();
	}
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
	INT32 nTestCount = 0;
	char *pcBuffer = NULL;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	pid_t tid = 0;
	tid = syscall(SYS_gettid);
	nRetVal = setpriority(PRIO_PROCESS, tid, -10);
	if(nRetVal < 0)
	{
		if(t_AccNotifyDlg != NULL)
			t_AccNotifyDlg->WriteLogE("[%s] fail to set priority : [%d]", m_strThreadName.c_str(), errno);
		return -1;
	}

	pcBuffer = (char *)malloc(ASI_FANOTIFY_BUFF_MAX+1);
	if(pcBuffer == NULL)
	{
		if(t_AccNotifyDlg != NULL)
			t_AccNotifyDlg->WriteLogE("[%s] fail to allocate memory : [%d]", m_strThreadName.c_str(), errno);
		return -2;
	}
	memset(pcBuffer, 0, ASI_FANOTIFY_BUFF_MAX+1);

	pChkFileProc = (PASI_CHK_FILE_PROC)malloc(sizeof(ASI_CHK_FILE_PROC));
	if(pChkFileProc == NULL)
	{
		if(t_AccNotifyDlg != NULL)
			t_AccNotifyDlg->WriteLogE("[%s] fail to allocate memory : [%d]", m_strThreadName.c_str(), errno);
		safe_free(pcBuffer);
		return -3;
	}
	m_nPause = 1;
	nRetVal = InitAccNotifyEvent(nNotifyFd, nTestMode);
	if(nRetVal == 0)
	{
		memset(g_acSrcPath, 0, MAX_PATH);
		m_nPause = 0;
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
				memset(g_acSrcPath, 0, MAX_PATH);
				if(nRetVal == -1)
				{
					break;
				}
				else
				{
					UninitAccNotifyEvent(nTestMode);
					if(t_AccNotifyDlg != NULL)
					{
						t_AccNotifyDlg->WriteLogN("[%s] reconnect nannysvc : [%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
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
				memset(g_acSrcPath, 0, MAX_PATH);
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
		t_AccNotifyDlg->WriteLogN("[%s] stop thread by sub continue flag", m_strThreadName.c_str());
	}
	safe_free(pChkFileProc);
	safe_free(pcBuffer);
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
	if ( m_nAccNotifyFd < 0 )
	{
//		m_nAccNotifyFd = fanotify_init(FAN_CLOEXEC | FAN_CLASS_CONTENT | FAN_NONBLOCK, O_RDONLY | O_LARGEFILE);
		m_nAccNotifyFd = fanotify_init(FAN_CLOEXEC | FAN_CLASS_CONTENT, O_RDONLY | O_CLOEXEC | O_LARGEFILE | O_NOATIME);
		if ( m_nAccNotifyFd < 0 )
		{
			bRetVal = FALSE;
			nNotifyFd = -1;
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
	if(pPath == NULL || pPath[0] == 0 || nNotifyFd < 0)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[AddWatchAccNotify] invalid input data %d", nNotifyFd);
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
	BOOL bIsDot = FALSE;
	if (pInfo == NULL || pInfo->acFullPath[0] == 0 || pInfo->nLen < 1)
	{
		return FALSE;
	}
	nLen = pInfo->nLen;
	bIsDot = FALSE;
	for(i=nLen-1; i>=0; i--)
	{
		if(pInfo->acFullPath[i] == '.')	
		{
			bIsDot = TRUE;
		}
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
	if(bIsDot == TRUE)
	{
		if(_stricmp(pInfo->acFile, "a.out"))
			return FALSE;
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
	if(nLen > 3)
	{
		if(!_stricmp(&pFileInfo->acFullPath[nLen-3], ".so") || !_stricmp(&pFileInfo->acFullPath[nLen-2], ".o"))
		{
			return FALSE;
		}
	}

	if(ParseFilePath(pFileInfo) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
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

INT32		CThreadAccNotifyEvent::ChkInPtnEx(PASI_CHK_FILE_PROC pChkFileProc, CString strFullPath, ASI_POLICY_INFO &stPolicyInfo, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType)
{
	INT32 nDenyCount = 0;
	INT32 nAllowCount = 0;
	CString strHash;
	if(pChkFileProc == NULL || t_ThreadAccServerSocket == NULL)
		return 0;

	if(stPolicyInfo.nOpMode == STATUS_USED_MODE_OFF)
	{
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		nIsWarning = 1;
		return 1;
	}

	if(stPolicyInfo.nOpMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	nDenyCount = t_ThreadAccServerSocket->GetDenyExPathCount();
	nAllowCount = t_ThreadAccServerSocket->GetAllowExPathCount();
	if(nDenyCount < 1 && nAllowCount < 1)
		return 0;

	if(stPolicyInfo.nExUsedMode == STATUS_USED_MODE_OFF)
		return 0;
	if(stPolicyInfo.nExUsedMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	if(nAllowCount != 0 && t_ThreadAccServerSocket->FindAllowExPathStr(strFullPath, strHash))
	{
		pChkFileProc->stFileInfo.nPtn = PTN_FB_PTN_RISK_EX;
		strncpy(pChkFileProc->stRetInfo.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
		pChkFileProc->stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		return 1;
	}
	if(nDenyCount != 0 && t_ThreadAccServerSocket->FindDenyExPathStr(strFullPath, strHash))
	{
		pChkFileProc->stFileInfo.nPtn = PTN_FB_PTN_RISK_EX;
		strncpy(pChkFileProc->stRetInfo.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
		pChkFileProc->stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
		return 1;
	}
	return 0;
}

INT32		CThreadAccNotifyEvent::ChkInPtnSP(PASI_CHK_FILE_PROC pChkFileProc, CString strFullPath, ASI_POLICY_INFO &stPolicyInfo, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType)
{
	CString strHash;
	INT32 nDenyCount = 0;
	INT32 nAllowCount = 0;
	if(pChkFileProc == NULL || t_ThreadAccServerSocket == NULL)
		return 0;

	if(stPolicyInfo.nOpMode == STATUS_USED_MODE_OFF)
	{
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		nIsWarning = 1;
		return 1;
	}
	if(stPolicyInfo.nOpMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	if(stPolicyInfo.nSpUsedMode == STATUS_USED_MODE_OFF)
		return 0;
	if(stPolicyInfo.nSpUsedMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	nDenyCount = t_ThreadAccServerSocket->GetDenySpPathCount();
	nAllowCount = t_ThreadAccServerSocket->GetAllowSpPathCount();
	if(nDenyCount < 1 && nAllowCount < 1)
		return 0;


	if(nAllowCount != 0 && t_ThreadAccServerSocket->FindAllowSpPathStr(strFullPath, strHash))
	{
		pChkFileProc->stFileInfo.nPtn = PTN_FB_PTN_RISK_SP;
		strncpy(pChkFileProc->stRetInfo.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
		pChkFileProc->stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		return 1;
	}
	if(nDenyCount != 0 && t_ThreadAccServerSocket->FindDenySpPathStr(strFullPath, strHash))
	{
		pChkFileProc->stFileInfo.nPtn = PTN_FB_PTN_RISK_SP;
		strncpy(pChkFileProc->stRetInfo.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
		pChkFileProc->stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
		return 1;
	}

	return 0;
}

INT32		CThreadAccNotifyEvent::ChkInPtn(PASI_CHK_FILE_PROC pChkFileProc, CString strFullPath, ASI_POLICY_INFO &stPolicyInfo, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType)
{
	UINT32 nBLCount = 0;
	UINT32 nWLCount = 0;
	UINT32 nCLCount = 0;
	UINT32 nPtnRisk = PTN_FB_PTN_RISK_UNKNOW;
	CString strHash;
	if(pChkFileProc == NULL || t_ThreadAccServerSocket == NULL)
		return 0;

	if(stPolicyInfo.nOpMode == STATUS_USED_MODE_OFF)
		return 0;

	nBLCount = t_ThreadAccServerSocket->GetBlPathCount();
	if(nBLCount == 0)
	{
		stPolicyInfo.nBLUsedMode = STATUS_USED_MODE_OFF;
	}

	nWLCount = t_ThreadAccServerSocket->GetWlPathCount();
	if(nWLCount == 0)
	{
		stPolicyInfo.nWLUsedMode = STATUS_USED_MODE_OFF;
	}

	nCLCount = t_ThreadAccServerSocket->GetCrPathCount();

	if(stPolicyInfo.nBLUsedMode == STATUS_USED_MODE_OFF && stPolicyInfo.nWLUsedMode == STATUS_USED_MODE_OFF)
		return 0;

	if(stPolicyInfo.nOpMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	nPtnRisk = PTN_FB_PTN_RISK_UNKNOW;
	if(stPolicyInfo.nBLUsedMode != STATUS_USED_MODE_OFF)
	{
		if(t_ThreadAccServerSocket->FindBlPathStr(strFullPath, strHash))
		{
			nPtnRisk = PTN_FB_PTN_RISK_BLACK;
			pChkFileProc->stFileInfo.nPtn = PTN_FB_PTN_RISK_BLACK;
			strncpy(pChkFileProc->stRetInfo.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
			pChkFileProc->stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		}
	}

	if(nPtnRisk == PTN_FB_PTN_RISK_UNKNOW && stPolicyInfo.nWLUsedMode != STATUS_USED_MODE_OFF)
	{
		if(t_ThreadAccServerSocket->FindWlPathStr(strFullPath, strHash))
		{
			nPtnRisk = PTN_FB_PTN_RISK_WHITE;
			pChkFileProc->stFileInfo.nPtn = PTN_FB_PTN_RISK_WHITE;
			strncpy(pChkFileProc->stRetInfo.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
			pChkFileProc->stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		}
	}

	if(!_stricmp(pChkFileProc->stProcInfo.acFile, "cp_test") && !_strnicmp(pChkFileProc->stFileInfo.acFile, "tc", 2))
	{
		if(pChkFileProc->stFileInfo.nPtn == PTN_FB_PTN_RISK_WHITE)
		{
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_WL + stPolicyInfo.nWLID;
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
			return 1;
		}
		else if(pChkFileProc->stFileInfo.nPtn == PTN_FB_PTN_RISK_BLACK)
		{
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_BL + stPolicyInfo.nWLID;
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
			return 1;
		}
	}


	if(nPtnRisk == PTN_FB_PTN_RISK_UNKNOW && nCLCount != 0)
	{
		if(t_ThreadAccServerSocket->FindCrPathStr(strFullPath, strHash))
		{
			nPtnRisk = PTN_FB_PTN_RISK_YELLOW;
			pChkFileProc->stFileInfo.nPtn = PTN_FB_PTN_RISK_YELLOW;
			strncpy(pChkFileProc->stRetInfo.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
			pChkFileProc->stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		}
	}

	if(stPolicyInfo.nOpBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW)
	{
		if(stPolicyInfo.nBLUsedMode == STATUS_USED_MODE_OFF)
			return 0;
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		if(nPtnRisk == PTN_FB_PTN_RISK_BLACK)
		{
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_BL + stPolicyInfo.nBLID;
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
		}
		else if(nPtnRisk == PTN_FB_PTN_RISK_WHITE)
		{
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_WL + stPolicyInfo.nWLID;
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		}
	}
	else
	{
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;

		if(nPtnRisk == PTN_FB_PTN_RISK_BLACK)
		{
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_BL + stPolicyInfo.nBLID;
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
		}
		else if(nPtnRisk == PTN_FB_PTN_RISK_YELLOW)
		{
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_OP + stPolicyInfo.nOpID;
		}
		else if(nPtnRisk == PTN_FB_PTN_RISK_WHITE)
		{
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_WL + stPolicyInfo.nWLID;
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		}
	}

	return 1;
}

VOID	CThreadAccNotifyEvent::SetReturnValue(ASI_RET_INFO *pstRetInfo, INT32 nAcVal, INT32 nBlockMode, INT32 nIsWarning, INT32 nPolicyType)
{
	if(pstRetInfo != NULL)
	{
		pstRetInfo->nAcVal = nAcVal;
		pstRetInfo->nBlockMode = nBlockMode;
		pstRetInfo->nPolicyType = nPolicyType;
		pstRetInfo->nIsWarning = nIsWarning;
	}
}

INT32	CThreadAccNotifyEvent::CheckPolicyInfo(ASI_POLICY_INFO &stPolicyInfo)
{
	INT32 PolicyUse = 0;
	if(t_ThreadAccServerSocket == NULL)
		return -1;

	t_ThreadAccServerSocket->GetPolicyUse(PolicyUse);
	if(PolicyUse == 0)
		return -2;

	t_ThreadAccServerSocket->GetPolicyInfo(&stPolicyInfo);
	return 0;
}



INT32	CThreadAccNotifyEvent::AnalyzeAccEvent(PASI_CHK_FILE_PROC pChkFileProc)
{
	INT32 nRetVal = 0;
	INT32 nBlockMode = SS_PO_CTL_PROC_BLOCK_MODE_ALLOW;
	INT32 nAcVal = RET_NONE;
	INT32 nIsWarning = 0;
	INT32 nPolicyType = 0;
	ASI_POLICY_INFO stPolicyInfo;
	CString strFullPath;
	if(pChkFileProc == NULL)
	{
		return RET_NONE;
	}
	memset(&stPolicyInfo, 0, sizeof(ASI_POLICY_INFO));
	do{

		nRetVal = CheckPolicyInfo(stPolicyInfo);
		if(nRetVal < 0)
		{
			nAcVal = RET_NONE;
			break;
		}
		strFullPath.Format("%s", pChkFileProc->stFileInfo.acFullPath);

		nRetVal = ChkInPtnEx(pChkFileProc, strFullPath, stPolicyInfo, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal == 1)
		{
			nAcVal = RET_ALLOW;
			if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
			{
				if(nIsWarning == 1)
					nAcVal = RET_WARN;
				else
					nAcVal = RET_DENY;
			}
			SetReturnValue(&pChkFileProc->stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
			break;
		}

		nRetVal = ChkInPtnSP(pChkFileProc, strFullPath, stPolicyInfo, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal == 1)
		{
			nAcVal = RET_ALLOW;
			if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
			{
				if(nIsWarning == 1)
					nAcVal = RET_WARN;
				else
					nAcVal = RET_DENY;
			}
			SetReturnValue(&pChkFileProc->stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
			break;
		}

		nRetVal = ChkInPtn(pChkFileProc, strFullPath, stPolicyInfo, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal == 1)
		{
			nAcVal = RET_ALLOW;
			if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
			{
				if(nIsWarning == 1)
					nAcVal = RET_WARN;
				else
					nAcVal = RET_DENY;
			}
			SetReturnValue(&pChkFileProc->stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
			break;
		}
		nAcVal = RET_NONE;
	}while(FALSE);

	return nAcVal;
}


INT32	CThreadAccNotifyEvent::BypassObjectPath(PASI_CHK_FILE_PROC pChkFileProc)
{
	if(pChkFileProc == NULL)
		return -1;
	if(!_strnicmp(pChkFileProc->stProcInfo.acFullPath, NANNY_INSTALL_DIR, NANNY_INSTALL_DIR_LEN))
	{
		return 0;
	}
	else if(!_strnicmp(pChkFileProc->stFileInfo.acFullPath, NANNY_INSTALL_DIR, NANNY_INSTALL_DIR_LEN))
	{
		return 0;
	}
	else if(!_stricmp(pChkFileProc->stProcInfo.acFile, REQ_WHITE_NAME))
	{
		return 0;
	}
	else if(!_stricmp(pChkFileProc->stFileInfo.acFile, REQ_WHITE_NAME))
	{
		return 0;
	}
	else if(!_stricmp(pChkFileProc->stProcInfo.acFile, MAKE_TEST_NAME))
	{
		return 0;
	}
	else if(!_stricmp(pChkFileProc->stProcInfo.acFile, LD_BIN_NAME))
	{
		return 0;
	}
	else if(!_stricmp(pChkFileProc->stFileInfo.acFile, MAKE_TEST_NAME))
	{
		return 0;
	}
	else if(!_stricmp(pChkFileProc->stFileInfo.acFile, CP_TEST_NAME))
	{
		return 0;
	}
	else if(!_stricmp(pChkFileProc->stFileInfo.acFile, EXE_TEST_NAME))
	{
		return 0;
	}
	else if(!_stricmp(pChkFileProc->stFileInfo.acFile, WRITE_TEST_NAME))
	{
		return 0;
	}
	else if(!_strnicmp(pChkFileProc->stFileInfo.acFullPath, "/sys", 4))
	{
		return 0;
	}
	else if(!_strnicmp(pChkFileProc->stFileInfo.acFullPath, "/lib", 4))
	{
		return 0;
	}
	else if(!_strnicmp(pChkFileProc->stFileInfo.acFullPath, "/proc", 5))
	{
		return 0;
	}
	else if(!_strnicmp(pChkFileProc->stFileInfo.acFullPath, "/usr/src", 8))
	{
		return 0;
	}
	else if(!_strnicmp(pChkFileProc->stFileInfo.acFullPath, "/var/lib", 8))
	{
		return 0;
	}
	else if(!_strnicmp(pChkFileProc->stFileInfo.acFullPath, "/usr/include", 12))
	{
		return 0;
	}
	return -2;
}


INT32	CThreadAccNotifyEvent::AnalyzeWithPolicyServer(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc)
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
		if(BypassObjectPath(pChkFileProc) == 0)
		{
			nAcVal = RET_NONE;
			nRetVal = 0;
			break;
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
		if(!_stricmp(pChkFileProc->stProcInfo.acFile, "cp_test"))
		{
			pChkFileProc->nCmdId = CMD_PIPE_PO_IN_ACCESS_FILE;
		}
		else
		{
			pChkFileProc->nCmdId = CMD_PIPE_PO_IN_EXECUTE_FILE;
		}
		if(t_ThreadAccClientSocket != NULL)
		{
			nRetVal = t_ThreadAccClientSocket->AddChkFileProcList(pChkFileProc);
			if(nRetVal != 0)
			{
				return -3;
			}
		}
	}
	else
	{
		if(!_stricmp(pChkFileProc->stProcInfo.acFile, "exe_test_pgm"))
		{
			if(pChkFileProc->stFileInfo.nPtn == PTN_FB_PTN_RISK_WHITE)
			{
				pChkFileProc->nCmdId = CMD_PIPE_PO_IN_EXECUTE_FILE;
				pChkFileProc->stRetInfo.nAcVal = RET_ALLOW;
				nRetVal = t_ThreadAccClientSocket->AddChkFileProcList(pChkFileProc);
				if(nRetVal != 0)
				{
					return -4;
				}
			}
		}
	}
	return nRetVal;
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
		nRetVal = AnalyzeWithPolicyServer(nNotifyFd, pMetaData, pChkFileProc);
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
		if(nCount == 0)
		{
			strncpy(pChkFileProc->stProcInfo.acFullPath, "/bin/zsh", MAX_PATH-1);
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/bin/ping", MAX_PATH-1);
		}
		else if(nCount == 1)
		{
			strncpy(pChkFileProc->stProcInfo.acFullPath, "/bin/zsh", MAX_PATH-1);
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/home/tc/tc9/tc9_file_004", MAX_PATH-1);
		}	
		else if(nCount == 2)
		{
			strncpy(pChkFileProc->stProcInfo.acFullPath, "/bin/zsh", MAX_PATH-1);
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/home/tc/tc9/tc9_file_005", MAX_PATH-1);
		}
		else if(nCount == 3)
		{
			strncpy(pChkFileProc->stProcInfo.acFullPath, "/bin/zsh", MAX_PATH-1);
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/sys/tc9/tc9_file_003", MAX_PATH-1);
		}
		else if(nCount == 4)
		{
			strncpy(pChkFileProc->stProcInfo.acFullPath, "/bin/cp_test", MAX_PATH-1);
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/usr/bin/vim.tiny", MAX_PATH-1);
		}
		else if(nCount == 5)
		{
			strncpy(pChkFileProc->stProcInfo.acFullPath, "/bin/cp_test", MAX_PATH-1);
			strncpy(pChkFileProc->stFileInfo.acFullPath, "/usr/share/autojump/autojump", MAX_PATH-1);
		}
		else
		{
			nAcVal = RET_NONE;
			nRetVal = 0;
			break;
		}

		pChkFileProc->stProcInfo.acFullPath[MAX_PATH-1] = 0;
		pChkFileProc->stProcInfo.nLen = (INT32)strlen(pChkFileProc->stProcInfo.acFullPath);

		if(ParseFilePath(&pChkFileProc->stProcInfo) == FALSE)
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

		nAcVal = AnalyzeAccEvent(pChkFileProc);
		nRetVal = 0;
	}while(FALSE);

	if(nAcVal == RET_DENY || nAcVal == RET_WARN)
	{
		if(!_stricmp(pChkFileProc->stProcInfo.acFile, "cp_test"))
		{
			pChkFileProc->nCmdId = CMD_PIPE_PO_IN_ACCESS_FILE;
		}
		else
		{
			pChkFileProc->nCmdId = CMD_PIPE_PO_IN_EXECUTE_FILE;
		}
		if(t_ThreadAccClientSocket != NULL)
		{
			nRetVal = t_ThreadAccClientSocket->AddChkFileProcList(pChkFileProc);
			if(nRetVal < 0)
				return -3;
		}
	}
	Sleep(2000);
	return nRetVal;
}
