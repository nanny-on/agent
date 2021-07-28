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
 * License along with this program. If not, see <https://www.gnu.org/licenses/>.   
 *  
 * 
 */

// ThreadPoFaClear.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadPoFaOp.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFaOp

CThreadPoFaOp*	t_ThreadPoFaOp = NULL;

CThreadPoFaOp::CThreadPoFaOp()
{
	m_nOrderID = 0;
	m_nNotifyFd = -1;
	pthread_mutex_init(&m_NotifyMutex, NULL);

	if(InitNotifyFd() == FALSE)
	{
		WriteLogE("fail to init notify fd (%d)", errno);
	}
}

CThreadPoFaOp::~CThreadPoFaOp()
{
	CloseNotifyFd();
	pthread_mutex_destroy(&m_NotifyMutex);
}

BOOL CThreadPoFaOp::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoFaOp::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFaOp message handlers

INT32 CThreadPoFaOp::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 nRetVal = 0;
	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{	
			nRetVal = CheckNotifyFile();
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
	ClearWatchNotify();
	return 0;
}

BOOL CThreadPoFaOp::IsNotifyPath(INT32 nWatchd)
{
	BOOL bFind = FALSE;
	TMapNotifyPathItor find;
	if(nWatchd < 0)
		return FALSE;

	pthread_mutex_lock (&m_NotifyMutex);
	find = m_tNofifyPathMap.find(nWatchd);
	if(find == m_tNofifyPathMap.end())
		bFind = FALSE;
	else
		bFind = TRUE;
	pthread_mutex_unlock (&m_NotifyMutex);
	return bFind;
}

BOOL CThreadPoFaOp::IsNotifyPathWithPath(char *pPath)
{
	PNOTIFY_PATH pFindPath = NULL;
	TMapNotifyPathItor begin, end;
	if(pPath == NULL || pPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_NotifyMutex);
	begin = m_tNofifyPathMap.begin();	end = m_tNofifyPathMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
//		if(!_stricmp(pFindPath->acNotifyPath, pPath))
		if(!strcmp(pFindPath->acNotifyPath, pPath))
		{
			pthread_mutex_unlock (&m_NotifyMutex);
			return TRUE;
		}
	}
	pthread_mutex_unlock (&m_NotifyMutex);
	return FALSE;
}

BOOL CThreadPoFaOp::ModNotifyPath(PNOTIFY_PATH pNotifyPath)
{
	PNOTIFY_PATH pFindPath = NULL;
	TMapNotifyPathItor begin, end;
	if(pNotifyPath == NULL)
		return FALSE;

	if(pNotifyPath->nWatchd == 0 && pNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_NotifyMutex);
	if(pNotifyPath->nWatchd > 0)
	{
		begin = m_tNofifyPathMap.find(pNotifyPath->nWatchd);
		end = m_tNofifyPathMap.end();
		if(begin != end)
		{
			pFindPath = &(begin->second);
			pFindPath->nOrderID = pNotifyPath->nOrderID;
			pFindPath->nExtOption = pNotifyPath->nExtOption;
			pthread_mutex_unlock (&m_NotifyMutex);
			return TRUE;
		}
	}
	else
	{
		begin = m_tNofifyPathMap.begin();	end = m_tNofifyPathMap.end();
		for (; begin != end; begin++)
		{
			pFindPath = &(begin->second);
//			if(!_stricmp(pFindPath->acNotifyPath, pNotifyPath->acNotifyPath))
			if(!strcmp(pFindPath->acNotifyPath, pNotifyPath->acNotifyPath))
			{
				pFindPath->nOrderID = pNotifyPath->nOrderID;
				pFindPath->nExtOption = pNotifyPath->nExtOption;
				pthread_mutex_unlock (&m_NotifyMutex);
				return TRUE;
			}
		}
	}
	pthread_mutex_unlock (&m_NotifyMutex);
	return FALSE;
}


BOOL CThreadPoFaOp::InsertNotifyPath(PNOTIFY_PATH pNotifyPath)
{
	INT32 nWatchd = 0;
	if(pNotifyPath == NULL)
		return FALSE;
	
	nWatchd = pNotifyPath->nWatchd;
	if(nWatchd < 0)
		return FALSE;

	if(IsNotifyPath(nWatchd) == TRUE)
		return TRUE;

	pthread_mutex_lock (&m_NotifyMutex);
	m_tNofifyPathMap[nWatchd] = *pNotifyPath;
	m_nNotifyCount++;
	pthread_mutex_unlock (&m_NotifyMutex);
	return TRUE;
}

BOOL CThreadPoFaOp::GetNotifyPath(PNOTIFY_PATH pNotifyPath)
{
	PNOTIFY_PATH pFindPath = NULL;
	TMapNotifyPathItor begin, end;
	if(pNotifyPath == NULL)
		return FALSE;

	if(pNotifyPath->nWatchd == 0 && pNotifyPath->acNotifyPath[0] == 0)
		return FALSE;

	pthread_mutex_lock (&m_NotifyMutex);
	if(pNotifyPath->nWatchd > 0)
	{
		begin = m_tNofifyPathMap.find(pNotifyPath->nWatchd);
		end = m_tNofifyPathMap.end();
		if(begin != end)
		{
			pFindPath = &(begin->second);
			memcpy(pNotifyPath, pFindPath, sizeof(NOTIFY_PATH));
			pthread_mutex_unlock (&m_NotifyMutex);
			return TRUE;
		}
	}
	else
	{
		begin = m_tNofifyPathMap.begin();	end = m_tNofifyPathMap.end();
		for (; begin != end; begin++)
		{
			pFindPath = &(begin->second);
//			if(!_stricmp(pFindPath->acNotifyPath, pNotifyPath->acNotifyPath))
			if(!strcmp(pFindPath->acNotifyPath, pNotifyPath->acNotifyPath))
			{
				memcpy(pNotifyPath, pFindPath, sizeof(NOTIFY_PATH));
				pthread_mutex_unlock (&m_NotifyMutex);
				return TRUE;
			}
		}
	}
	pthread_mutex_unlock (&m_NotifyMutex);
	return FALSE;
}


UINT32 CThreadPoFaOp::GetNotifyPathCount()
{
	UINT32 nCount = 0;
	pthread_mutex_lock (&m_NotifyMutex);
	nCount = m_nNotifyCount;
	pthread_mutex_unlock (&m_NotifyMutex);
	return nCount;
}

BOOL CThreadPoFaOp::DeleteNotifyPath(INT32 nWatchd)
{
	if(nWatchd < 0)
		return FALSE;
	if(IsNotifyPath(nWatchd) == FALSE)
		return FALSE;
	pthread_mutex_lock (&m_NotifyMutex);
	m_tNofifyPathMap.erase(nWatchd);
	if(m_nNotifyCount > 0)
		m_nNotifyCount--;
	pthread_mutex_unlock (&m_NotifyMutex);
	return TRUE;
}

void CThreadPoFaOp::ClearNotifyPath()
{
	pthread_mutex_lock (&m_NotifyMutex);
	m_tNofifyPathMap.clear();
	m_nNotifyCount = 0;
	pthread_mutex_unlock (&m_NotifyMutex);
}


BOOL CThreadPoFaOp::InitNotifyFd()
{
	m_nNotifyFd = inotify_init();
	if ( m_nNotifyFd < 0 )
	{
		WriteLogE("[%s] fail to init inotify : [%d]", m_strThreadName.c_str(), errno);
		return FALSE;
	}
	return TRUE;
}

BOOL CThreadPoFaOp::CloseNotifyFd()
{
	if ( m_nNotifyFd >= 0 )
	{
		close(m_nNotifyFd);
		m_nNotifyFd = -1;
	}
}

INT32 CThreadPoFaOp::AddSubDirWatchNotify(INT32 nOrderID, INT32 nExtOption, char *pPath)
{
	INT32 nRetVal, nLen = 0;
	DIR *pDir = NULL;
	struct dirent *pDirEnt = NULL;
	PNOTIFY_PATH pNotifyPath = NULL;

	if(pPath == NULL || pPath[0] == 0)
	{
		return -1;
	}

	pNotifyPath = (PNOTIFY_PATH)malloc(sizeof(NOTIFY_PATH));
	if(pNotifyPath == NULL)
	{
		WriteLogE("[%s] fail to allocate memory : [%d]", m_strThreadName.c_str(), errno);
		return -2;
	}

	pDir = opendir(pPath);
	if (pDir == NULL)
	{
		WriteLogE("[%s] fail to open path [%s] : [%d]", m_strThreadName.c_str(), pPath, errno);
		safe_free(pNotifyPath);
		return -3;
	}

	while((pDirEnt = readdir(pDir)) != NULL)
	{
		if(!_stricmp(pDirEnt->d_name, ".") || !_stricmp(pDirEnt->d_name, ".."))
		{
			continue;
		}
		if(DT_DIR == pDirEnt->d_type)
		{
			memset(pNotifyPath, 0, sizeof(NOTIFY_PATH));
			snprintf(pNotifyPath->acNotifyPath, CHAR_MAX_SIZE-1, "%s/%s", pPath, pDirEnt->d_name);
			pNotifyPath->acNotifyPath[CHAR_MAX_SIZE-1] = 0;
			pNotifyPath->nPathLen = strlen(pNotifyPath->acNotifyPath);
			pNotifyPath->nExtOption = nExtOption;
			pNotifyPath->nOrderID = nOrderID;
			nRetVal = AddDirWatchNotify(pNotifyPath);
			if(nRetVal != 0)
			{
				safe_free(pNotifyPath);
				nRetVal -= 10;
				return nRetVal;
			}
		}
	}
	closedir(pDir);
	safe_free(pNotifyPath);
	return 0;
}


INT32 CThreadPoFaOp::AddDirWatchNotify(PNOTIFY_PATH pNotifyPath)
{
	UINT32 dwMask = IN_Q_OVERFLOW | IN_MOVE | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF;

	if(m_nNotifyFd < 0 || pNotifyPath == NULL)
		return -1;

	if(IsNotifyPathWithPath(pNotifyPath->acNotifyPath) == TRUE)
	{
		ModNotifyPath(pNotifyPath);
		return 0;
	}

	pNotifyPath->nWatchd = inotify_add_watch( m_nNotifyFd, pNotifyPath->acNotifyPath, dwMask );
	if( pNotifyPath->nWatchd < 0 )
	{
		WriteLogE("[%s] fail to add inotify watch [%s] : [%d]", m_strThreadName.c_str(), pNotifyPath->acNotifyPath, errno);
		return -2;
	}

	if(InsertNotifyPath(pNotifyPath) == FALSE)
	{
		inotify_rm_watch( m_nNotifyFd, pNotifyPath->nWatchd );
		return -3;
	}
	return 0;
}

INT32 CThreadPoFaOp::AddWatchNotify(INT32 nOrderID, INT32 nExtOption, char *pPath)
{
	INT32 nLen = 0;
	INT32 nRetVal = 0;
	PNOTIFY_PATH pNotifyPath = NULL;

	if(pPath == NULL)
	{
		return -1;
	}

	pNotifyPath = (PNOTIFY_PATH)malloc(sizeof(NOTIFY_PATH));
	if(pNotifyPath == NULL)
	{
		WriteLogE("[%s] fail to allocate memory : [%d]", m_strThreadName.c_str(), errno);
		return -2;
	}
	memset(pNotifyPath, 0, sizeof(NOTIFY_PATH));

	strncpy(pNotifyPath->acNotifyPath, pPath, CHAR_MAX_SIZE-1);
	pNotifyPath->acNotifyPath[CHAR_MAX_SIZE-1] = 0;
	nLen = strlen(pNotifyPath->acNotifyPath);
	if(nLen < 2)
	{
		WriteLogE("[%s] invalid notify path len [%d]", m_strThreadName.c_str(), nLen);
		safe_free(pNotifyPath);
		return -3;
	}

	if(pNotifyPath->acNotifyPath[nLen-1] == '/')
	{
		pNotifyPath->acNotifyPath[nLen-1] = '\0';
		nLen--;
	}
	pNotifyPath->nPathLen = nLen;
	pNotifyPath->nExtOption = nExtOption;
	pNotifyPath->nOrderID = nOrderID;

	nRetVal = AddDirWatchNotify(pNotifyPath);
	if(nRetVal != 0)
	{
		nRetVal -= 10;
		safe_free(pNotifyPath);
		return nRetVal;
	}
	if(nExtOption == 1)
	{
		nRetVal = AddSubDirWatchNotify(nOrderID, nExtOption, pNotifyPath->acNotifyPath);
		if(nRetVal != 0)
		{
			safe_free(pNotifyPath);
			nRetVal -= 20;
			return nRetVal;
		}
	}
	safe_free(pNotifyPath);
	return 0;
}

INT32 CThreadPoFaOp::DelWatchNotify(char *pPath)
{
	INT32 nLen = 0;
	NOTIFY_PATH stNotifyPath;;

	if(pPath == NULL || m_nNotifyFd < 0)
	{
		return -1;
	}

	memset(&stNotifyPath, 0, sizeof(stNotifyPath));
	strncpy(stNotifyPath.acNotifyPath, pPath, CHAR_MAX_SIZE-1);
	stNotifyPath.acNotifyPath[CHAR_MAX_SIZE-1] = 0;
	nLen = strlen(stNotifyPath.acNotifyPath);
	if(nLen < 1)
	{
		return -2;
	}

	if(stNotifyPath.acNotifyPath[nLen-1] == '/')
	{
		stNotifyPath.acNotifyPath[nLen-1] = '\0';
	}

	if(GetNotifyPath(&stNotifyPath) == FALSE)
	{
		return -3;
	}

	inotify_rm_watch( m_nNotifyFd, stNotifyPath.nWatchd );

	if(DeleteNotifyPath(stNotifyPath.nWatchd) == false)
	{
		return -4;
	}

	return 0;
}

void CThreadPoFaOp::ClearWatchNotify()
{
	PNOTIFY_PATH pFindPath = NULL;
	TMapNotifyPathItor begin, end;

	if(GetNotifyPathCount() == 0)
		return;

	if(m_nNotifyFd < 0)
	{
		return;
	}

	pthread_mutex_lock (&m_NotifyMutex);
	begin = m_tNofifyPathMap.begin();	end = m_tNofifyPathMap.end();
	for (; begin != end; begin++)
	{
		pFindPath = &(begin->second);
		if(pFindPath->nWatchd >= 0)
			inotify_rm_watch( m_nNotifyFd, pFindPath->nWatchd );
	}
	pthread_mutex_unlock (&m_NotifyMutex);

	ClearNotifyPath();
}


INT32		CThreadPoFaOp::AddOrderID(PVOID pPFOI)
{
	if(pPFOI == NULL)
		return -1;
//	m_tOrderIDMap[pMFOI->nID] = *pMFOI;
	return 0;
}

INT32		CThreadPoFaOp::AnalyzeEvent(PVOID pNotifyEvent)
{
	PNOTIFY_PATH pNotifyPath = NULL;
	INT32 nTotalLen = 0;
	INT32 nRetVal = 0;
	struct inotify_event *pEvent = (struct inotify_event *)pNotifyEvent;
	if(pEvent == NULL)
		return -1;

	pNotifyPath = (PNOTIFY_PATH)malloc(sizeof(NOTIFY_PATH));
	if(pNotifyPath == NULL)
		return -2;

	memset(pNotifyPath, 0, sizeof(NOTIFY_PATH));
	

	do{
		pNotifyPath->nWatchd = pEvent->wd;
		if(GetNotifyPath(pNotifyPath) == FALSE)
		{
			nRetVal = -3;
			break;
		}

		if( pEvent->len != 0 )
		{
			nTotalLen = strlen((char *)pEvent->name) + pNotifyPath->nPathLen + 1;
			if(nTotalLen > MAX_PATH - 1)
			{
				nRetVal = -4;
				break;
			}
			strcat(pNotifyPath->acNotifyPath, "/");
			strcat(pNotifyPath->acNotifyPath, (char *)pEvent->name);
			pNotifyPath->acNotifyPath[nTotalLen] = 0;
			pNotifyPath->nPathLen = nTotalLen;
		}

		if( pEvent->mask & IN_ISDIR )
		{
			if(pNotifyPath->nExtOption == TRUE)
			{
				if( pEvent->mask & IN_CREATE || pEvent->mask & IN_MOVED_TO )
				{
					nRetVal = AddWatchNotify(pNotifyPath->nOrderID, pNotifyPath->nExtOption, pNotifyPath->acNotifyPath);
					if(nRetVal == 0)
					{
						WriteLogN("success to add watch %s : [%s]", pNotifyPath->acNotifyPath, m_strThreadName.c_str());
					}
					else
					{
						WriteLogE("fail to add watch %s (%d) : [%s]", pNotifyPath->acNotifyPath, nRetVal, m_strThreadName.c_str());
					}
				}
				else if( pEvent->mask & IN_DELETE || pEvent->mask & IN_MOVED_FROM || pEvent->mask & IN_DELETE_SELF || pEvent->mask & IN_MOVE_SELF )
				{
					nRetVal = DelWatchNotify(pNotifyPath->acNotifyPath);
					if(nRetVal == 0)
					{
						WriteLogN("success to delete watch  %s : [%s]", pNotifyPath->acNotifyPath, m_strThreadName.c_str());
					}
					else
					{
						WriteLogE("fail to delete watch %s (%d) : [%s]", pNotifyPath->acNotifyPath, nRetVal, m_strThreadName.c_str());
					}
				}
			}
		}
		else
		{
			CMainDlg* pMainDlg = (CMainDlg*)t_EnvInfoOp->GetMainHandle();
			if(pMainDlg != NULL)
			{
				if( pEvent->mask & IN_CREATE || pEvent->mask & IN_MOVED_TO )
				{
					pMainDlg->OnNotifyFileLog(FILE_NOTIFY_TYPE_CREATE, pNotifyPath);
				}
				else if( pEvent->mask & IN_DELETE || pEvent->mask & IN_MOVED_FROM || pEvent->mask & IN_DELETE_SELF || pEvent->mask & IN_MOVE_SELF )
				{
					pMainDlg->OnNotifyFileLog(FILE_NOTIFY_TYPE_DELETE, pNotifyPath);
				}
			}
		}
		if( pEvent->mask & IN_Q_OVERFLOW )
		{
			WriteLogE("inotify queue overflow : [%s]", m_strThreadName.c_str());
			nRetVal = -1001;
			break;
		}
		nRetVal = 0;
	}while(FALSE);

	safe_free(pNotifyPath);
	return nRetVal;
}

//---------------------------------------------------------------------------

INT32		CThreadPoFaOp::CheckNotifyFile()
{
	INT32 i, nSize, nRetVal, nLength = 0;
	UINT32 nCount = 0;
	struct inotify_event *pEvent = NULL;
	char *pBuffer = NULL;

	if(m_nNotifyFd < 0)
	{
		return -1;
	}

	nCount = GetNotifyPathCount();
	if(nCount == 0)
	{
		return -2;
	}

	ioctl( m_nNotifyFd, FIONREAD, &nLength );
	if( nLength <= 0 )
	{
		return -2;
	}

	pBuffer = (char *)calloc( nLength+1, sizeof(char) );
	if(pBuffer == NULL)
	{
		WriteLogE("fail to allocate memory (%d) : [%s]", errno, m_strThreadName.c_str());
		return -3;
	}


	nRetVal = read( m_nNotifyFd, pBuffer, nLength );
	if(nRetVal == -1)
	{
		WriteLogE("fail to read notify (%d) : [%s]", errno, m_strThreadName.c_str());
		safe_free(pBuffer);
		return -4;
	}
	nSize = sizeof(struct inotify_event);
	for(i=0; i<nLength; )
	{
		pEvent = (struct inotify_event *) &(pBuffer[i]);
		if( pEvent == NULL )
		{
			break;
		}

		nRetVal = AnalyzeEvent( (PVOID)pEvent );
		if(nRetVal != 0)
		{
			if(nRetVal == -1001)
			{
				safe_free(pBuffer);
				return nRetVal;
			}
		}

		i += nSize + pEvent->len;
	}
	safe_free(pBuffer);
	return 0;
}
