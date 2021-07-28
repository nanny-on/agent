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

//---------------------------------------------------------------------------

#include "stdafx.h"
#include "com_struct.h"
#include "ManageLocalEnvLog.h"

//---------------------------------------------------------------------------

CManageLocalEnvLog*	t_ManageLocalEnvLog = NULL;

//---------------------------------------------------------------------------

CManageLocalEnvLog::CManageLocalEnvLog()
{
	t_DBMgrLocalEnvLog	= new CDBMgrLocalEnvLog();	
}
//---------------------------------------------------------------------------

CManageLocalEnvLog::~CManageLocalEnvLog()
{
	if(t_DBMgrLocalEnvLog)	{	delete t_DBMgrLocalEnvLog;	t_DBMgrLocalEnvLog = NULL;	}	
}
//---------------------------------------------------------------------------

INT32		CManageLocalEnvLog::LoadDBMS()
{
	DB_LOCAL_ENV_LOG dlel;
	TListDBLocalEnvLog tDBLocalEnvLogList;
	TListDBLocalEnvLogItor begin, end;
	
	if(SetER(t_DBMgrLocalEnvLog->LoadExecute(&tDBLocalEnvLogList)))
	{
		if(tDBLocalEnvLogList.size() == 0)
		{
			memset(&dlel, 0, sizeof(DB_LOCAL_ENV_LOG));
			AddLocalEnvLog(dlel);
		}
		return g_nErrRtn;
	}

	begin = tDBLocalEnvLogList.begin();	end = tDBLocalEnvLogList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->nID, *begin);
	}

	if(tDBLocalEnvLogList.size() == 0)
	{
		memset(&dlel, 0, sizeof(DB_LOCAL_ENV_LOG));
		AddLocalEnvLog(dlel);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvLog::InitHash()
{
	return 0;
}
//---------------------------------------------------------------------------

String					CManageLocalEnvLog::GetHash()
{
	return "";
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvLog::AddLocalEnvLog(DB_LOCAL_ENV_LOG&	dlel)
{	
	if(SetER(t_DBMgrLocalEnvLog->InsertExecute(&dlel)))
	{
		return g_nErrRtn;
	}

	AddItem(dlel.nID, dlel);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvLog::EditLocalEnvLog(DB_LOCAL_ENV_LOG&	dlel)
{
	PDB_LOCAL_ENV_LOG pdlel = FindItem(dlel.nID);
	if(!pdlel)	return ERR_DBMS_UPDATE_FAIL;	

	if(SetER(t_DBMgrLocalEnvLog->UpdateExecute(&dlel)))
	{
		return g_nErrRtn;
	}

	EditItem(dlel.nID, dlel);
	InitHash();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvLog::DelLocalEnvLog(UINT32 nID)
{
	PDB_LOCAL_ENV_LOG pdlel = FindItem(nID);
	if(!pdlel)	return ERR_DBMS_DELETE_FAIL;


	if(SetER(t_DBMgrLocalEnvLog->DeleteExecute(pdlel->nID)))
	{
		return g_nErrRtn;
	}
	
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvLog::ApplyLocalEnvLog(DB_LOCAL_ENV_LOG&	dlel)
{
	if(FindItem(dlel.nID))
	{
		return EditLocalEnvLog(dlel);
	}
	return AddLocalEnvLog(dlel);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageLocalEnvLog::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBLocalEnvLogItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvLog::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_LOCAL_ENV_LOG pdlel = FindItem(nID);
	if(!pdlel)	return -1;

	return SetPkt(pdlel, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvLog::SetPkt(PDB_LOCAL_ENV_LOG pdlel, MemToken& SendToken)
{

	SendToken.TokenAdd_32(pdlel->nID);
	SendToken.TokenAdd_32(pdlel->nUsedFlag);
	SendToken.TokenAdd_32(pdlel->nLogDocID);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvLog::GetPkt(MemToken& RecvToken, DB_LOCAL_ENV_LOG& dlel)
{
	if (!RecvToken.TokenDel_32(dlel.nID))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dlel.nUsedFlag))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dlel.nLogDocID))					goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



