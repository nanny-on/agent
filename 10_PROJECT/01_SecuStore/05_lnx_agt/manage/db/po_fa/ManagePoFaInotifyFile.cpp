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
 * License along with this program. If not, see <https://www.gnu.org/licenses/>.   
 *  
 * 
 */

//---------------------------------------------------------------------------

#include "stdafx.h"
#include "com_struct.h"
#include "ManagePoFaInotifyFile.h"

//---------------------------------------------------------------------------

CManagePoFaInotifyFile*	t_ManagePoFaInotifyFile = NULL;


//---------------------------------------------------------------------------

CManagePoFaInotifyFile::CManagePoFaInotifyFile()
{
	t_DBMgrPoFaInotifyFile	= new CDBMgrPoFaInotifyFile();
}
//---------------------------------------------------------------------------

CManagePoFaInotifyFile::~CManagePoFaInotifyFile()
{
	if(t_DBMgrPoFaInotifyFile)
	{
		delete t_DBMgrPoFaInotifyFile;
		t_DBMgrPoFaInotifyFile = NULL;
	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaInotifyFile::LoadDBMS()
{
	TListLogNotifyFile tDBLogNotifyFileList;
    TListLogNotifyFileItor begin, end;
	INT32 nCount = 0;

	if(t_DBMgrPoFaInotifyFile == NULL)
	{
		return -1;
	}
	if(SetER(t_DBMgrPoFaInotifyFile->LoadExecute(&tDBLogNotifyFileList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBLogNotifyFileList.begin();	end = tDBLogNotifyFileList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
    }
	nCount = GetInotifyPathCount();
    return nCount;
}
//---------------------------------------------------------------------------

INT32	CManagePoFaInotifyFile::GetInotifyPathCount()
{
	return (INT32)m_tMap.size();
}

INT32		CManagePoFaInotifyFile::FindID(char *pPath)
{
	TMapLogNotifyFileItor begin, end;
	PDB_LOG_NOTIFY_FILE pLogNotify = NULL;
	if(pPath == NULL)
		return -1;

	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		pLogNotify = &(begin->second);
//		if(!_stricmp(pLogNotify->strNotifyFilePath.c_str(), pPath))
		if(!strcmp(pLogNotify->strNotifyFilePath.c_str(), pPath))
			return pLogNotify->nID;
	}
	return -2;
}

INT32	CManagePoFaInotifyFile::GetAllInotifyPath(PNOTIFY_PATH pNotifyPath, INT32 nTotalCount)
{
	TMapLogNotifyFileItor begin, end;
	PDB_LOG_NOTIFY_FILE pLogNotify = NULL;
	INT32 i = 0;
	if(pNotifyPath == NULL || nTotalCount < 1)
		return -1;

	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		pLogNotify = &(begin->second);
		strncpy(pNotifyPath[i].acNotifyPath, pLogNotify->strNotifyFilePath.c_str(), CHAR_MAX_SIZE-1);
		pNotifyPath[i].acNotifyPath[CHAR_MAX_SIZE-1] = 0;
		pNotifyPath[i].nOrderID = pLogNotify->nOrderID;
		pNotifyPath[i].nWatchd = pLogNotify->nID;
		if(++i >= nTotalCount)
			break;
	}
	return 0;
}

INT32	CManagePoFaInotifyFile::DeleteInotifyPath(PNOTIFY_PATH pNotifyPath)
{
	INT32 nRetVal = 0;
	INT32 nID = 0;
	if(t_DBMgrPoFaInotifyFile == NULL || pNotifyPath == NULL)
		return -1;

	nID = FindID(pNotifyPath->acNotifyPath);
	if(nID < 0)
		return -2;

	nRetVal = t_DBMgrPoFaInotifyFile->DeleteExecute(nID);
	if(nRetVal != 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	nRetVal = DeleteItem(nID);
	if(nRetVal != 0)
	{
		nRetVal -= 20;
		return nRetVal;
	}
	return 0;
}

INT32	CManagePoFaInotifyFile::InsertInotifyPath(PNOTIFY_PATH pNotifyPath)
{
	INT32 nRetVal = 0;
	INT32 nID = 0;
	DB_LOG_NOTIFY_FILE stLogNotify;
	if(t_DBMgrPoFaInotifyFile == NULL || pNotifyPath == NULL)
		return -1;

	memset(&stLogNotify, 0, sizeof(stLogNotify));

	stLogNotify.nUsedFlag = 1;
	stLogNotify.nRegDate = GetCurrentDateTimeInt();
	stLogNotify.nOrderID = pNotifyPath->nOrderID;
	stLogNotify.strNotifyFilePath = pNotifyPath->acNotifyPath;

	nRetVal = t_DBMgrPoFaInotifyFile->InsertExecute((PVOID)&stLogNotify);
	if(nRetVal != 0)
	{
		WriteLogE("fail to insert inotify path %s (%d)", stLogNotify.strNotifyFilePath.c_str(), nRetVal);
		nRetVal -= 10;
		return nRetVal;
	}
	nRetVal = AddItem(stLogNotify.nID, stLogNotify);
	if(nRetVal != 0)
	{
		WriteLogE("fail to add item inotify path %s (%d)", stLogNotify.strNotifyFilePath.c_str(), nRetVal);
		nRetVal -= 20;
	}
	return nRetVal;
}

VOID	CManagePoFaInotifyFile::DelAllInotifyPath(PNOTIFY_PATH pNotifyPath, INT32 nCount)
{
	INT32 i, nRetVal = 0;
	if(t_DBMgrPoFaInotifyFile == NULL || pNotifyPath == NULL || nCount < 1)
	{
		return;
	}
	for(i=0; i<nCount; i++)
	{
		if(pNotifyPath[i].nWatchd > 0)
		{
			nRetVal = t_DBMgrPoFaInotifyFile->DeleteExecute(pNotifyPath[i].nWatchd);
			if(nRetVal != 0)
			{
				WriteLogE("fail to delete excute inotify path %d (%d)", pNotifyPath[i].nWatchd, nRetVal);
			}

			DeleteItem(pNotifyPath[i].nWatchd);
		}
	}
}

//---------------------------------------------------------------------------


