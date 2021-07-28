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
#include "ThreadPoInPtnFile.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInPtnFile

CThreadPoInPtnFile*	t_ThreadPoInPtnFile = NULL;

CThreadPoInPtnFile::CThreadPoInPtnFile()
{
	m_nOrderID = 0;
	m_nFaNotifyFd = -1;
	m_nFaNotifyCount = 0;
	m_nFaNotifyIndex = 0;
	pthread_mutex_init(&m_FaNotifyMutex, NULL);

	if(InitFaNotifyFd() == FALSE)
	{
		WriteLogE("fail to init notify fd (%d)", errno);
	}
}

CThreadPoInPtnFile::~CThreadPoInPtnFile()
{
	CloseFaNotifyFd();
	pthread_mutex_destroy(&m_FaNotifyMutex);
}

BOOL CThreadPoInPtnFile::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoInPtnFile::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInPtnFile message handlers

INT32 CThreadPoInPtnFile::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 nRetVal = 0;
	INT32 nNotifyFd = -1;
	char *pcBuffer = NULL;
	PASI_CHK_PTN_FILE pChkPtnFile = NULL;

	if(GetFaNotifyFd(nNotifyFd) == FALSE)
	{
		WriteLogE("fail to get fanotify fd : [%s]", m_strThreadName.c_str());
		return -1;
	}

	pcBuffer = (char *)malloc(ASI_FANOTIFY_BUFF_MAX+1);
	if(pcBuffer == NULL)
	{
		WriteLogE("fail to allocate memory : [%s][%d]", m_strThreadName.c_str(), errno);
		return -2;
	}
	memset(pcBuffer, 0, ASI_FANOTIFY_BUFF_MAX+1);

	pChkPtnFile = (PASI_CHK_PTN_FILE)malloc(sizeof(ASI_CHK_PTN_FILE));
	if(pChkPtnFile == NULL)
	{
		WriteLogE("fail to allocate memory : [%s][%d]", m_strThreadName.c_str(), errno);
		safe_free(pcBuffer);
		return -3;
	}

	nRetVal = LoadWhitePattern();
	if(nRetVal < 0)
	{
		safe_free(pcBuffer);
		safe_free(pChkPtnFile);
		return -4;
	}

	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{	
			memset(pChkPtnFile, 0, sizeof(ASI_CHK_PTN_FILE));
			nRetVal = CheckFaNotifyFile(nNotifyFd, pcBuffer, pChkPtnFile);
			if(nRetVal != 0)
			{
				if(nRetVal == -1001)
				{
					WriteLogE("exit thread because of queue overflow : [%s]", m_strThreadName.c_str());
					break;
				}
				Sleep(100);
			}
		}
		else
		{
			Sleep(100);
		}
	}

	if(!t_EnvInfoOp)							
		WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())
		WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())
		WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	m_tOrderIDMap.clear();
	UnloadWhitePattern();
	ClearFaNotifyPath();
	safe_free(pcBuffer);
	safe_free(pChkPtnFile);
	return 0;
}


INT32 CThreadPoInPtnFile::LoadWhitePattern()
{
	INT32 nRetVal = 0;
	CHAR acPath[MAX_PATH] = {0, };

	snprintf(acPath, MAX_PATH-1, "%s/asi_fi.so", t_EnvInfo->m_strDLLPath.c_str());
	acPath[MAX_PATH-1] = 0;
	nRetVal = m_tFIDLLUtil.LoadLibraryExt(acPath);
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to load library  : [%s][%d][%d]", m_strThreadName.c_str(), acPath, nRetVal, errno);
		return -1;
	}

	snprintf(acPath, MAX_PATH-1, "%s/asi_weng.so", t_EnvInfo->m_strDLLPath.c_str());
	acPath[MAX_PATH-1] = 0;

	nRetVal = m_tWEDLLUtil.LoadLibraryExt(acPath);
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to load library  : [%s][%d][%d]", m_strThreadName.c_str(), acPath, nRetVal, errno);
		return -2;
	}
	nRetVal = m_tWEDLLUtil.Init();
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to init we dll  : [%d]", m_strThreadName.c_str(), nRetVal);
		return -3;
	}
	nRetVal = m_tWEDLLUtil.InitDB();
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to init we db  : [%d]", m_strThreadName.c_str(), nRetVal);
		return -4;
	}

	snprintf(acPath, MAX_PATH-1, "%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE);
	acPath[MAX_PATH-1] = 0;

	nRetVal = m_tWEDLLUtil.LoadDB(acPath);
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to load we db  : [%d]", m_strThreadName.c_str(), acPath, nRetVal);
		return -5;
	}

	m_tFileTypeMap.clear();
	
	return 0;
}
//---------------------------------------------------------------------------

VOID CThreadPoInPtnFile::UnloadWhitePattern()
{
	m_tFileTypeMap.clear();
	m_tWEDLLUtil.ClearFile();
	m_tWEDLLUtil.FreeLibraryExt();
	m_tFIDLLUtil.FreeLibraryExt();
}



BOOL CThreadPoInPtnFile::IsFaNotifyPath(char *pPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	if(pPath == NULL || pPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_FaNotifyMutex);
	begin = m_tFaNofifyPathMap.begin();	end = m_tFaNofifyPathMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(!strcmp(pFindPath->acNotifyPath, pPath))
		{
			pthread_mutex_unlock (&m_FaNotifyMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_FaNotifyMutex);
	return FALSE;
}

BOOL CThreadPoInPtnFile::ModFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;

	if(pFaNotifyPath == NULL || pFaNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_FaNotifyMutex);
	begin = m_tFaNofifyPathMap.begin();	end = m_tFaNofifyPathMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(!strcmp(pFindPath->acNotifyPath, pFaNotifyPath->acNotifyPath))
		{
			pFindPath->nOrderID = pFaNotifyPath->nOrderID;
			pthread_mutex_unlock (&m_FaNotifyMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_FaNotifyMutex);
	return FALSE;
}


BOOL CThreadPoInPtnFile::AddFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath)
{
	if(pFaNotifyPath == NULL || pFaNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_FaNotifyMutex);
	pFaNotifyPath->nIndex = m_nFaNotifyIndex;
	m_tFaNofifyPathMap[m_nFaNotifyIndex] = *pFaNotifyPath;
	m_nFaNotifyIndex++;
	m_nFaNotifyCount++;
	pthread_mutex_unlock (&m_FaNotifyMutex);
	return TRUE;
}

BOOL CThreadPoInPtnFile::GetFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath)
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	if(pFaNotifyPath == NULL || pFaNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_FaNotifyMutex);
	begin = m_tFaNofifyPathMap.begin();	end = m_tFaNofifyPathMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(!strcmp(pFindPath->acNotifyPath, pFaNotifyPath->acNotifyPath))
		{
			memcpy(pFaNotifyPath, pFindPath, sizeof(FANOTIFY_PATH));
			pthread_mutex_unlock (&m_FaNotifyMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_FaNotifyMutex);
	return FALSE;
}


UINT32 CThreadPoInPtnFile::GetFaNotifyPathCount()
{
	UINT32 nCount = 0;
	pthread_mutex_lock (&m_FaNotifyMutex);
	nCount = m_nFaNotifyCount;
	pthread_mutex_unlock (&m_FaNotifyMutex);
	return nCount;
}

BOOL CThreadPoInPtnFile::DelFaNotifyPath(INT32 nIndex)
{
	if(nIndex < 0)
		return FALSE;
	pthread_mutex_lock (&m_FaNotifyMutex);
	m_tFaNofifyPathMap.erase(nIndex);
	if(m_nFaNotifyCount > 0)
		m_nFaNotifyCount--;
	pthread_mutex_unlock (&m_FaNotifyMutex);
	return TRUE;
}


void CThreadPoInPtnFile::ClearFaNotifyPath()
{
	pthread_mutex_lock (&m_FaNotifyMutex);
	if(m_nFaNotifyCount != 0)
	{
		m_tFaNofifyPathMap.clear();
		m_nFaNotifyCount = 0;
	}
	pthread_mutex_unlock (&m_FaNotifyMutex);
}


BOOL CThreadPoInPtnFile::InitFaNotifyFd()
{
	BOOL bRetVal = TRUE;
	pthread_mutex_lock (&m_FaNotifyMutex);
	if ( m_nFaNotifyFd < 0 )
	{
		m_nFaNotifyFd = fanotify_init(FAN_CLOEXEC | FAN_CLASS_CONTENT, O_RDONLY | O_CLOEXEC | O_LARGEFILE | O_NOATIME);
		if ( m_nFaNotifyFd < 0 )
		{
			WriteLogE("[%s] fail to init fanotify : [%d]", m_strThreadName.c_str(), errno);
			bRetVal = FALSE;
		}
	}
	pthread_mutex_unlock (&m_FaNotifyMutex);
	return bRetVal;
}

BOOL CThreadPoInPtnFile::GetFaNotifyFd(INT32 &nNotifyFd)
{
	BOOL bRetVal = FALSE;
	pthread_mutex_lock (&m_FaNotifyMutex);
	if ( m_nFaNotifyFd < 0 )
	{
		bRetVal = FALSE;
	}
	else
	{
		nNotifyFd = m_nFaNotifyFd;
		bRetVal = TRUE;
	}
	pthread_mutex_unlock (&m_FaNotifyMutex);
	return bRetVal;
}

VOID CThreadPoInPtnFile::CloseFaNotifyFd()
{
	pthread_mutex_lock (&m_FaNotifyMutex);
	if ( m_nFaNotifyFd >= 0 )
	{
		close(m_nFaNotifyFd);
		m_nFaNotifyFd = -1;
	}
	pthread_mutex_unlock (&m_FaNotifyMutex);
}

INT32 CThreadPoInPtnFile::AddFaNotifyMark(PFANOTIFY_PATH pFaNotifyPath)
{
	INT32 nNotifyFd = -1;
	INT32 nRetVal = 0;
	UINT64 ddwEventMask = FAN_OPEN_EXEC;
	if(pFaNotifyPath == NULL || pFaNotifyPath->acNotifyPath[0] == 0)
	{
		WriteLogE("[AddWatchFaNotify] invalid input data");
		return -1;
	}

	if(GetFaNotifyFd(nNotifyFd) == FALSE)
	{
		WriteLogE("[AddWatchFaNotify] invalid fanotify fd");
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
		WriteLogE("[AddWatchFaNotify] fail to add fanotify watch [%s] : [%d]", pFaNotifyPath->acNotifyPath, errno);
		return -3;
	}

	if(AddFaNotifyPath(pFaNotifyPath) == FALSE)
	{
		WriteLogE("[AddWatchFaNotify] fail to add fanotify path [%s]", pFaNotifyPath->acNotifyPath);
		fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, pFaNotifyPath->acNotifyPath);
		return -4;
	}
	return 0;
}

INT32 CThreadPoInPtnFile::RemoveFaNotifyMark(PFANOTIFY_PATH pFaNotifyPath)
{
	INT32 nNotifyFd = -1;
	INT32 nRetVal = 0;
	UINT64 ddwEventMask = FAN_OPEN_EXEC;
	if(pFaNotifyPath == NULL || pFaNotifyPath->acNotifyPath[0] == 0)
	{
		WriteLogE("[RemoveFaNotifyMark] invalid input data");
		return -1;
	}

	if(GetFaNotifyFd(nNotifyFd) == FALSE)
	{
		WriteLogE("[RemoveFaNotifyMark] invalid fanotify fd");
		return -2;
	}

	if(GetFaNotifyPath(pFaNotifyPath) == FALSE)
	{
		WriteLogE("[RemoveFaNotifyMark] fail to find fanotify path (%s)", pFaNotifyPath->acNotifyPath);
		return -3;
	}

	nRetVal = fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, pFaNotifyPath->acNotifyPath);
	if(nRetVal < 0)
	{
		WriteLogE("[RemoveFaNotifyMark] fail to remove fanotify watch [%s] : [%d]", pFaNotifyPath->acNotifyPath, errno);
		return -4;
	}

	if(DelFaNotifyPath(pFaNotifyPath->nIndex) == FALSE)
	{
		WriteLogE("[RemoveFaNotifyMark] fail to del fanotify path [%s][%d]", pFaNotifyPath->acNotifyPath, pFaNotifyPath->nIndex);
		fanotify_mark(nNotifyFd, FAN_MARK_ADD | FAN_MARK_MOUNT, ddwEventMask, AT_FDCWD, pFaNotifyPath->acNotifyPath);
		return -5;
	}
	return 0;
}

INT32 CThreadPoInPtnFile::AddWatchFaNotify(INT32 nOrderID, char *pPath)
{
	INT32 nLen = 0;
	INT32 nRetVal = 0;
	PFANOTIFY_PATH pFaNotifyPath = NULL;

	if(pPath == NULL || pPath[0] == 0)
	{
		WriteLogE("[AddWatchFaNotify] invalid input data");
		return -1;
	}
	nLen = (INT32)strlen(pPath);
	if(nLen < 1 || nLen > MAX_PATH-2)
	{
		WriteLogE("[AddWatchFaNotify] invalid path len (%d)", nLen);
		return -2;
	}

	pFaNotifyPath = (PFANOTIFY_PATH)malloc(sizeof(FANOTIFY_PATH));
	if(pFaNotifyPath == NULL)
	{
		WriteLogE("[AddWatchFaNotify] fail to allocate memory : [%d]", errno);
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
		WriteLogE("[AddWatchFaNotify] fail to add fanotify mark : [%d]", nRetVal);
		nRetVal -= 10;
		safe_free(pFaNotifyPath);
		return nRetVal;
	}
	safe_free(pFaNotifyPath);
	return 0;
}

INT32 CThreadPoInPtnFile::RemoveWatchFaNotify(char *pPath)
{
	INT32 nLen = 0;
	INT32 nRetVal = 0;
	PFANOTIFY_PATH pFaNotifyPath = NULL;

	if(pPath == NULL || pPath[0] == 0)
	{
		WriteLogE("[DelWatchFaNotify] invalid input data");
		return -1;
	}

	nLen = (INT32)strlen(pPath);
	if(nLen < 1 || nLen > MAX_PATH-2)
	{
		WriteLogE("[DelWatchFaNotify] invalid path len (%d)", nLen);
		return -2;
	}

	pFaNotifyPath = (PFANOTIFY_PATH)malloc(sizeof(FANOTIFY_PATH));
	if(pFaNotifyPath == NULL)
	{
		WriteLogE("[DelWatchFaNotify] fail to allocate memory : [%d]", errno);
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
		WriteLogE("[AddWatchFaNotify] fail to remove fanotify mark : [%d]", nRetVal);
		nRetVal -= 10;
		safe_free(pFaNotifyPath);
		return nRetVal;
	}
	safe_free(pFaNotifyPath);
	return 0;
}

void CThreadPoInPtnFile::ClearWatchFaNotify()
{
	PFANOTIFY_PATH pFindPath = NULL;
	TMapFaNotifyPathItor begin, end;
	INT32 nNotifyFd = -1;
	UINT64 ddwEventMask = FAN_OPEN_EXEC;

	if(GetFaNotifyPathCount() == 0)
		return;

	if(GetFaNotifyFd(nNotifyFd) == FALSE)
	{
		ClearFaNotifyPath();
		return;
	}

	pthread_mutex_lock (&m_FaNotifyMutex);
	begin = m_tFaNofifyPathMap.begin();	end = m_tFaNofifyPathMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(pFindPath->acNotifyPath[0] != 0)
		{
			if(fanotify_mark(nNotifyFd, FAN_MARK_REMOVE, ddwEventMask, AT_FDCWD, pFindPath->acNotifyPath) < 0)
			{
				WriteLogE("[ClearWatchFaNotify] fail to remove fanotify watch [%s] : [%d]", pFindPath->acNotifyPath, errno);
			}
		}
	}
	pthread_mutex_unlock (&m_FaNotifyMutex);

	ClearFaNotifyPath();
}

BOOL	CThreadPoInPtnFile::GetFilePathFromFd(INT32 nFd, PASI_CHK_PTN_FILE pChkPtnFile)
{
	ssize_t i, nLen = 0;
	BOOL bIsSep = FALSE;
	char acPath[MAX_TYPE_LEN] = {0,};

	if (nFd < 1 || pChkPtnFile == NULL)
	{
		return FALSE;
	}

	snprintf (acPath, MAX_TYPE_LEN-1, "/proc/self/fd/%d", nFd);
	acPath[MAX_TYPE_LEN-1] = 0;
	nLen = readlink (acPath, pChkPtnFile->acFilePath, MAX_PATH-1);
	if (nLen < 2 || nLen > MAX_PATH-1)
	{
		return FALSE;
	}
	pChkPtnFile->acFilePath[nLen] = '\0';

	if(nLen > 3 && !strcmp(&pChkPtnFile->acFilePath[nLen-3], ".so"))
	{
		return FALSE;
	}

	for(i=nLen-1; i>=0; i--)
	{
		if(pChkPtnFile->acFilePath[i] == '/')	
		{
			bIsSep = TRUE;
			break;
		}
	}
	if(bIsSep == FALSE || i<0)
		return FALSE;

	if(i==0)
	{
		pChkPtnFile->acPath[0] = '/';
		pChkPtnFile->acPath[1] = '\0';
		strncpy(pChkPtnFile->acFile, &pChkPtnFile->acFilePath[1], nLen-1);
		pChkPtnFile->acFile[nLen-1] = '\0'; 
	}
	else
	{
		strncpy(pChkPtnFile->acFile, &pChkPtnFile->acFilePath[i+1], MAX_PATH-1);
		pChkPtnFile->acFile[MAX_PATH-1] = '\0'; 
		strncpy(pChkPtnFile->acPath, pChkPtnFile->acFilePath, i);
		pChkPtnFile->acPath[i] = '\0'; 
	}
	return TRUE;
}

BOOL	CThreadPoInPtnFile::GetProcPathFromPid(INT32 nPid, char *pcBuffer, INT32 nSize)
{
	ssize_t nLen = 0;;
	char acPath[MAX_TYPE_LEN] = {0,};

	if (nPid < 1 || pcBuffer == NULL || nSize < 1)
	{
		return FALSE;
	}

	snprintf (acPath, MAX_TYPE_LEN-1, "/proc/%d/exe", nPid);
	acPath[MAX_TYPE_LEN-1] = 0;
	nLen = readlink (acPath, pcBuffer, nSize-1);
	if (nLen < 2 || nLen > nSize-1)
	{
		return FALSE;
	}
	pcBuffer[nLen] = '\0';
	return TRUE;
}

INT32	CThreadPoInPtnFile::AnalyzeExecEvent(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_PTN_FILE pChkPtnFile)
{
	struct fanotify_response stAccess;
	INT32 nRetVal = 0;
	UINT32 nElfType = 0;
	DWORD dwFileType = AS_INVALID_FILE;
	String strPath;
	TMapStrIDItor find;
	struct fanotify_event_metadata *pEvent = NULL;

	if(pMetaData == NULL || pChkPtnFile == NULL || nNotifyFd < 0)
		return -1;

	pEvent = (struct fanotify_event_metadata *)pMetaData;

	do{
		if(GetProcPathFromPid(pEvent->pid, pChkPtnFile->acProcPath, MAX_PATH) == FALSE)
		{
			strncpy(pChkPtnFile->acProcPath, "unknown", CHAR_MAX_SIZE-1);
		}
		pChkPtnFile->acProcPath[MAX_PATH-1] = 0;

		if(GetFilePathFromFd(pEvent->fd, pChkPtnFile) == FALSE)
		{
			dwFileType = AS_INVALID_FILE;
			break;
		}
		pChkPtnFile->acFilePath[MAX_PATH-1] = 0;

		strPath = pChkPtnFile->acFilePath;
		find = m_tFileTypeMap.find(strPath);
		if(find != m_tFileTypeMap.end())
		{
			dwFileType = find->second;
			break;
		}

		nElfType = m_tFIDLLUtil.ASIFI_GetFileElfType(pChkPtnFile->acPath, pChkPtnFile->acFile);
		if(nElfType != ASI_FI_ELF_TYPE_EXE && nElfType != ASI_FI_ELF_TYPE_SCRIPTS)
		{
			dwFileType = AS_INVALID_FILE;
			m_tFileTypeMap[strPath] = dwFileType;
			break;
		}

		nRetVal = m_tWEDLLUtil.GetWL(pChkPtnFile->acFilePath, (PVOID)&pChkPtnFile->stAWWE, sizeof(ASI_WENG_WL_EX), &dwFileType);
		if(nRetVal < 0)
		{
			dwFileType = AS_INVALID_FILE;
			m_tFileTypeMap[strPath] = dwFileType;
			break;
		}
		m_tFileTypeMap[strPath] = dwFileType;
	}while(FALSE);

	stAccess.fd = pEvent->fd;
	stAccess.response = FAN_ALLOW;
	if(dwFileType == AS_ELF_FILE)
	{
		stAccess.response = FAN_DENY;
	}
	stAccess.fd = pEvent->fd;
	write (nNotifyFd, &stAccess, sizeof(stAccess));
	close(pEvent->fd);
	pEvent->fd = -1;

	if(dwFileType == AS_ELF_FILE)
	{
		//log
		CMainDlg* pMainDlg = (CMainDlg*)t_EnvInfoOp->GetMainHandle();

	}
	return 0;
}

INT32	CThreadPoInPtnFile::AnalyzeFaNotifyEvent(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_PTN_FILE pChkPtnFile)
{
	struct fanotify_event_metadata *pEvent = NULL;
	INT32 nRetVal = 0;

	if(pMetaData == NULL || pChkPtnFile == NULL || nNotifyFd < 0)
		return -1;

	pEvent = (struct fanotify_event_metadata *)pMetaData;
	if(pEvent->fd < 1)
		return -2;

	if (pEvent->mask & FAN_OPEN_EXEC)
	{
		nRetVal = AnalyzeExecEvent(nNotifyFd, pMetaData, pChkPtnFile);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
		}
	}
	else
	{
		close(pEvent->fd);
		pEvent->fd = -1;
	}
	return nRetVal;
}
//---------------------------------------------------------------------------

INT32		CThreadPoInPtnFile::CheckFaNotifyFile(INT32 nNotifyFd, char *pcBuffer, PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nRetVal = 0;
	ssize_t nLength = 0;
	struct pollfd stPollFd;
	struct fanotify_event_metadata *pMetaData = NULL;

	if(nNotifyFd < 0 || pcBuffer == NULL || pChkPtnFile == NULL)
	{
		WriteLogE("[CheckFaNotifyFile] invalid input data");
		return -1;
	}

	stPollFd.fd = nNotifyFd;
	stPollFd.events = POLLIN;

	nRetVal = poll(&stPollFd, 1, 5000);
	if (nRetVal < 0)
	{
		WriteLogE("[CheckFaNotifyFile] fail to poll fd (%d)", errno);
		return -2;
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
				AnalyzeFaNotifyEvent(nNotifyFd, (PVOID)pMetaData, pChkPtnFile);
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
