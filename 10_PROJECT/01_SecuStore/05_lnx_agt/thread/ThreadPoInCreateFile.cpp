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
#include "ThreadPoInCreateFile.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInCreateFile

CThreadPoInCreateFile*	t_ThreadPoInCreateFile = NULL;

CThreadPoInCreateFile::CThreadPoInCreateFile()
{
	m_nCreateNotifyFd = -1;
	m_nCreateNotifyCount = 0;
	m_nCreateNotifyIndex = 0;
	pthread_mutex_init(&m_EventMutex, NULL);
}

CThreadPoInCreateFile::~CThreadPoInCreateFile()
{
	pthread_mutex_destroy(&m_EventMutex);
}

INT32	CThreadPoInCreateFile::InitCreateNotifyEvent(INT32 &nNotifyFd)
{
	INT32 nRetVal = 0;
	INT32 i, nPolUse = 0;
	INT32 nNFd = -1;

	nRetVal = CheckWhitePatternFile();
	if(nRetVal < 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	do{
		if(InitCreateNotifyFd(nNFd) == FALSE)
		{
			WriteLogE("[%s] fail to init acc notify fd (%d)", errno, m_strThreadName.c_str());
			nRetVal = -2;
			break;
		}

		nRetVal = AddWatchCreateNotify(1, nNFd, ASI_FANOTIFY_PATH);
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
		UninitCreateNotifyEvent();
	}
	return nRetVal;
}

VOID	CThreadPoInCreateFile::UninitCreateNotifyEvent()
{
	ClearWatchCreateNotify();
	CloseCreateNotifyFd();
}


BOOL CThreadPoInCreateFile::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoInCreateFile::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInCreateFile message handlers

INT32 CThreadPoInCreateFile::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 nRetVal = 0;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	INT32 nNotifyFd = -1;
	char *pcBuffer = NULL;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	pcBuffer = (char *)malloc(ASI_FANOTIFY_BUFF_MAX+1);
	if(pcBuffer == NULL)
	{
		WriteLogE("[%s] fail to allocate memory : [%d]", m_strThreadName.c_str(), errno);
		return -1;
	}
	memset(pcBuffer, 0, ASI_FANOTIFY_BUFF_MAX+1);

	pChkFileProc = (PASI_CHK_FILE_PROC)malloc(nSize);
	if(pChkFileProc == NULL)
	{
		WriteLogE("[%s] fail to allocate memory : [%d]", m_strThreadName.c_str(), errno);
		safe_free(pcBuffer);
		return -2;
	}
	m_nPause = 1;
	nRetVal = InitCreateNotifyEvent(nNotifyFd);
	if(nRetVal == 0)
	{
		m_nPause = 0;
	}

	m_nRunFlag = 1;

	while(GetContinue())
	{
		if(!m_nPause)
		{
			memset(pChkFileProc, 0, nSize);
			nRetVal = CheckCreateNotifyFile(nNotifyFd, pcBuffer, pChkFileProc);
			if(nRetVal != 0)
			{
				if(nRetVal == -1)
				{
					break;
				}
				else
				{
					UninitCreateNotifyEvent();
					nNotifyFd = -1;
					m_nPause = 1;
					Sleep(1000);
				}
			}
		}
		else
		{
			nRetVal = InitCreateNotifyEvent(nNotifyFd);
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
	WriteLogN("[%s] stop thread by sub continue flag", m_strThreadName.c_str());
	UninitCreateNotifyEvent();
	safe_free(pChkFileProc);
	safe_free(pcBuffer);
	return 0;
}

INT32 CThreadPoInCreateFile::CheckWhitePatternFile()
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

BOOL CThreadPoInCreateFile::InitCreateNotifyFd(INT32 &nNotifyFd)
{
	BOOL bRetVal = TRUE;
	pthread_mutex_lock (&m_EventMutex);
	if ( m_nCreateNotifyFd < 0 )
	{
		m_nCreateNotifyFd = fanotify_init(FAN_CLOEXEC | FAN_CLASS_CONTENT, O_RDONLY | O_CLOEXEC | O_LARGEFILE | O_NOATIME);
		if ( m_nCreateNotifyFd < 0 )
		{
			bRetVal = FALSE;
			nNotifyFd = -1;
		}
		else
		{
			nNotifyFd = m_nCreateNotifyFd;
		}
	}
	pthread_mutex_unlock (&m_EventMutex);
	return bRetVal;
}

BOOL CThreadPoInCreateFile::GetCreateNotifyFd(INT32 &nNotifyFd)
{
	BOOL bRetVal = FALSE;
	pthread_mutex_lock (&m_EventMutex);
	if ( m_nCreateNotifyFd < 0 )
	{
		bRetVal = FALSE;
	}
	else
	{
		nNotifyFd = m_nCreateNotifyFd;
		bRetVal = TRUE;
	}
	pthread_mutex_unlock (&m_EventMutex);
	return bRetVal;
}

VOID CThreadPoInCreateFile::CloseCreateNotifyFd()
{
	pthread_mutex_lock (&m_EventMutex);
	if ( m_nCreateNotifyFd >= 0 )
	{
		close(m_nCreateNotifyFd);
		m_nCreateNotifyFd = -1;
	}
	pthread_mutex_unlock (&m_EventMutex);
}

INT32 CThreadPoInCreateFile::AddCreateNotifyMark(INT32 nNotifyFd, PFANOTIFY_PATH pCreateNotifyPath)
{
	INT32 nRetVal = 0;
	UINT64 ddwEventMask = FAN_CLOSE_WRITE;
	if(nNotifyFd < 0 || pCreateNotifyPath == NULL || pCreateNotifyPath->acNotifyPath[0] == 0)
	{
		WriteLogE("[AddCreateNotifyMark] invalid input data (%d)", nNotifyFd);
		return -1;
	}

	if(IsCreateNotifyPath(pCreateNotifyPath->acNotifyPath) == TRUE)
	{
		ModCreateNotifyPath(pCreateNotifyPath);
		return 0;
	}

	nRetVal = fanotify_mark(nNotifyFd, FAN_MARK_ADD | FAN_MARK_MOUNT, ddwEventMask, AT_FDCWD, ASI_FANOTIFY_PATH);
	if(nRetVal < 0)
	{
		WriteLogE("[AddCreateNotifyMark] fail to add CreateNotify watch [%s] : [%d][%d]", pCreateNotifyPath->acNotifyPath, nNotifyFd, errno);
		return -3;
	}

	if(AddCreateNotifyPath(pCreateNotifyPath) == FALSE)
	{
		fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, ASI_FANOTIFY_PATH);
		return -4;
	}
	return 0;
}

INT32 CThreadPoInCreateFile::RemoveCreateNotifyMark(PFANOTIFY_PATH pCreateNotifyPath)
{
	INT32 nNotifyFd = -1;
	INT32 nRetVal = 0;
	UINT64 ddwEventMask = FAN_CLOSE_WRITE;
	if(pCreateNotifyPath == NULL || pCreateNotifyPath->acNotifyPath[0] == 0)
	{
		return -1;
	}

	if(GetCreateNotifyFd(nNotifyFd) == FALSE)
	{
		return -2;
	}

	if(GetCreateNotifyPath(pCreateNotifyPath) == FALSE)
	{
		return -3;
	}

	fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, ASI_FANOTIFY_PATH);

	if(DelCreateNotifyPath(pCreateNotifyPath->nIndex) == FALSE)
	{
		WriteLogE("[RemoveCreateNotifyMark] fail to del CreateNotify path [%s][%d]", pCreateNotifyPath->acNotifyPath, pCreateNotifyPath->nIndex);
		fanotify_mark(nNotifyFd, FAN_MARK_ADD | FAN_MARK_MOUNT, ddwEventMask, AT_FDCWD, pCreateNotifyPath->acNotifyPath);
		return -5;
	}
	return 0;
}

INT32 CThreadPoInCreateFile::AddWatchCreateNotify(INT32 nOrderID, INT32 nNotifyFd, char *pPath)
{
	INT32 nLen = 0;
	INT32 nRetVal = 0;
	PFANOTIFY_PATH pCreateNotifyPath = NULL;
	if(pPath == NULL || pPath[0] == 0 || nNotifyFd < 0)
	{
		WriteLogE("[AddWatchCreateNotify] invalid input data %d", nNotifyFd);
		return -1;
	}
	nLen = (INT32)strlen(pPath);
	if(nLen < 1 || nLen > MAX_PATH-2)
	{
		WriteLogE("[AddWatchCreateNotify] invalid path len (%d)", nLen);
		return -2;
	}

	pCreateNotifyPath = (PFANOTIFY_PATH)malloc(sizeof(FANOTIFY_PATH));
	if(pCreateNotifyPath == NULL)
	{
		WriteLogE("[AddWatchCreateNotify] fail to allocate memory : [%d]", errno);
		return -3;
	}
	memset(pCreateNotifyPath, 0, sizeof(FANOTIFY_PATH));

	strncpy(pCreateNotifyPath->acNotifyPath, pPath, MAX_PATH-1);
	pCreateNotifyPath->acNotifyPath[MAX_PATH-1] = 0;

	if(nLen > 2 && pCreateNotifyPath->acNotifyPath[nLen-1] == '/')
	{
		pCreateNotifyPath->acNotifyPath[nLen-1] = '\0';
		nLen--;
	}

	pCreateNotifyPath->nPathLen = nLen;
	pCreateNotifyPath->nOrderID = nOrderID;

	nRetVal = AddCreateNotifyMark(nNotifyFd, pCreateNotifyPath);
	if(nRetVal != 0)
	{
		nRetVal -= 10;
		safe_free(pCreateNotifyPath);
		return nRetVal;
	}
	safe_free(pCreateNotifyPath);
	return 0;
}

INT32 CThreadPoInCreateFile::RemoveWatchCreateNotify(char *pPath)
{
	INT32 nLen = 0;
	INT32 nRetVal = 0;
	PFANOTIFY_PATH pCreateNotifyPath = NULL;
	if(pPath == NULL || pPath[0] == 0)
	{
		WriteLogE("[RemoveWatchCreateNotify] invalid input data");
		return -1;
	}

	nLen = (INT32)strlen(pPath);
	if(nLen < 1 || nLen > MAX_PATH-2)
	{
		WriteLogE("[RemoveWatchCreateNotify] invalid path len (%d)", nLen);
		return -2;
	}

	pCreateNotifyPath = (PFANOTIFY_PATH)malloc(sizeof(FANOTIFY_PATH));
	if(pCreateNotifyPath == NULL)
	{
		WriteLogE("[RemoveWatchCreateNotify] fail to allocate memory : [%d]", errno);
		return -3;
	}
	memset(pCreateNotifyPath, 0, sizeof(FANOTIFY_PATH));

	strncpy(pCreateNotifyPath->acNotifyPath, pPath, MAX_PATH-1);
	pCreateNotifyPath->acNotifyPath[MAX_PATH-1] = 0;

	if(nLen > 2 && pCreateNotifyPath->acNotifyPath[nLen-1] == '/')
	{
		pCreateNotifyPath->acNotifyPath[nLen-1] = '\0';
	}

	nRetVal = RemoveCreateNotifyMark(pCreateNotifyPath);
	if(nRetVal != 0)
	{
		WriteLogE("[RemoveWatchCreateNotify] fail to remove CreateNotify mark : [%d]", nRetVal);
		nRetVal -= 10;
		safe_free(pCreateNotifyPath);
		return nRetVal;
	}
	safe_free(pCreateNotifyPath);
	return 0;
}


BOOL CThreadPoInCreateFile::IsCreateNotifyPath(char *pPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	if(pPath == NULL || pPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tCreateNotifyEventMap.begin();	end = m_tCreateNotifyEventMap.end();
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

BOOL CThreadPoInCreateFile::AddCreateNotifyPath(PFANOTIFY_PATH pCreateNotifyPath)
{
	if(pCreateNotifyPath == NULL || pCreateNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	pCreateNotifyPath->nIndex = m_nCreateNotifyIndex;
	m_tCreateNotifyEventMap[m_nCreateNotifyIndex] = *pCreateNotifyPath;
	m_nCreateNotifyIndex++;
	m_nCreateNotifyCount++;
	pthread_mutex_unlock (&m_EventMutex);
	return TRUE;
}

BOOL CThreadPoInCreateFile::ModCreateNotifyPath(PFANOTIFY_PATH pCreateNotifyPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;

	if(pCreateNotifyPath == NULL || pCreateNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tCreateNotifyEventMap.begin();	end = m_tCreateNotifyEventMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(!strcmp(pFindPath->acNotifyPath, pCreateNotifyPath->acNotifyPath))
		{
			pFindPath->nOrderID = pCreateNotifyPath->nOrderID;
			pthread_mutex_unlock (&m_EventMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_EventMutex);
	return FALSE;
}


BOOL CThreadPoInCreateFile::GetCreateNotifyPath(PFANOTIFY_PATH pCreateNotifyPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	if(pCreateNotifyPath == NULL || pCreateNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tCreateNotifyEventMap.begin();	end = m_tCreateNotifyEventMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(!strcmp(pFindPath->acNotifyPath, pCreateNotifyPath->acNotifyPath))
		{
			memcpy(pCreateNotifyPath, pFindPath, sizeof(FANOTIFY_PATH));
			pthread_mutex_unlock (&m_EventMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_EventMutex);
	return FALSE;
}


UINT32 CThreadPoInCreateFile::GetCreateNotifyPathCount()
{
	UINT32 nCount = 0;
	pthread_mutex_lock (&m_EventMutex);
	nCount = m_nCreateNotifyCount;
	pthread_mutex_unlock (&m_EventMutex);
	return nCount;
}

BOOL CThreadPoInCreateFile::DelCreateNotifyPath(INT32 nIndex)
{
	if(nIndex < 0)
		return FALSE;
	pthread_mutex_lock (&m_EventMutex);
	m_tCreateNotifyEventMap.erase(nIndex);
	if(m_nCreateNotifyCount > 0)
		m_nCreateNotifyCount--;
	pthread_mutex_unlock (&m_EventMutex);
	return TRUE;
}


void CThreadPoInCreateFile::ClearCreateNotifyPath()
{
	pthread_mutex_lock (&m_EventMutex);
	if(m_nCreateNotifyCount != 0)
	{
		m_tCreateNotifyEventMap.clear();
		m_nCreateNotifyCount = 0;
	}
	pthread_mutex_unlock (&m_EventMutex);
}



void CThreadPoInCreateFile::ClearWatchCreateNotify()
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	INT32 nNotifyFd = -1;
	UINT64 ddwEventMask = FAN_CLOSE_WRITE;

	if(GetCreateNotifyPathCount() == 0)
		return;

	if(GetCreateNotifyFd(nNotifyFd) == FALSE)
	{
		ClearCreateNotifyPath();
		return;
	}

	pthread_mutex_lock (&m_EventMutex);
	begin = m_tCreateNotifyEventMap.begin();	end = m_tCreateNotifyEventMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(pFindPath->acNotifyPath[0] != 0)
		{
			fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, pFindPath->acNotifyPath);
		}
	}
	pthread_mutex_unlock (&m_EventMutex);

	ClearCreateNotifyPath();
}


BOOL	CThreadPoInCreateFile::ParseFilePath(PASI_CHK_INFO pInfo)
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

BOOL	CThreadPoInCreateFile::GetFilePathFromFd(INT32 nFd, PASI_CHK_INFO pFileInfo)
{
	ssize_t nLen = 0;
	char acPath[MAX_TYPE_LEN] = {0,};

	if (nFd < 1 || pFileInfo == NULL)
	{
		return FALSE;
	}
	memset(pFileInfo->acFullPath, 0, MAX_PATH);
	snprintf (acPath, MAX_TYPE_LEN-1, "/proc/self/fd/%d", nFd);
	acPath[MAX_TYPE_LEN-1] = 0;
	nLen = readlink (acPath, pFileInfo->acFullPath, MAX_PATH-1);
	if (nLen < 2 || nLen > MAX_PATH-1)
	{
		return FALSE;
	}

	pFileInfo->acFullPath[nLen] = '\0';
	pFileInfo->nLen = (INT32)nLen;
	if(ParseFilePath(pFileInfo) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

INT32	CThreadPoInCreateFile::AnalyzeCreateFile(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc)
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
		snprintf(pChkFileProc->stProcInfo.acFullPath, MAX_PATH-1, "pid/%d", pEvent->pid);
		strncpy(pChkFileProc->stProcInfo.acPath, "pid", MAX_PATH-1);
		snprintf(pChkFileProc->stProcInfo.acFile, MAX_FILE_NAME-1, "%d", pEvent->pid);
		if(GetFilePathFromFd(pEvent->fd, &pChkFileProc->stFileInfo) == FALSE)
		{
			nAcVal = RET_NONE;
			nRetVal = 0;
			break;
		}

		if(t_ThreadPoInCreateLog != NULL)
		{
			nRetVal = t_ThreadPoInCreateLog->AddCreateFileList(pChkFileProc);
			if(nRetVal != 0)
			{
				nRetVal = -3;
				break;
			}
		}

		nRetVal = 0;
	}while(FALSE);

	stAccess.fd = pEvent->fd;
	stAccess.response = FAN_ALLOW;
	write (nNotifyFd, &stAccess, sizeof(stAccess));
	if(pEvent->fd > 0)
	{
		close(pEvent->fd);
		pEvent->fd = -1;
	}

	return nRetVal;
}

INT32	CThreadPoInCreateFile::AnalyzeCreateNotifyEvent(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc)
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

	if (pEvent->mask & FAN_CLOSE_WRITE)
	{
		nRetVal = AnalyzeCreateFile(nNotifyFd, pMetaData, pChkFileProc);
	}
	return nRetVal;
}
//---------------------------------------------------------------------------

INT32		CThreadPoInCreateFile::CheckCreateNotifyFile(INT32 nNotifyFd, char *pcBuffer, PASI_CHK_FILE_PROC pChkFileProc)
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
				nRetVal = AnalyzeCreateNotifyEvent(nNotifyFd, (PVOID)pMetaData, pChkFileProc);
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

