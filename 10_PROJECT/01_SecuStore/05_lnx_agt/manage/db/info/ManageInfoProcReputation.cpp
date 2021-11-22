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
#include "ManageInfoProcReputation.h"

//---------------------------------------------------------------------------

CManageInfoProcReputation*	t_ManageInfoProcReputation = NULL;

//---------------------------------------------------------------------------

CManageInfoProcReputation::CManageInfoProcReputation()
{
	t_DBMgrInfoProcReputation	= new CDBMgrInfoProcReputation();	
}
//---------------------------------------------------------------------------

CManageInfoProcReputation::~CManageInfoProcReputation()
{
	SAFE_DELETE(t_DBMgrInfoProcReputation);
}
//---------------------------------------------------------------------------

INT32		CManageInfoProcReputation::LoadDBMS()
{
	TListDBInfoProcReputation tDBInfoProcReputationList;
	TListDBInfoProcReputationItor begin, end;
	if(SetER(t_DBMgrInfoProcReputation->LoadExecute(&tDBInfoProcReputationList)))
	{
		return g_nErrRtn;
	}
	
	begin = tDBInfoProcReputationList.begin();	end = tDBInfoProcReputationList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->nID, *begin);
		AddKeyID(begin->nSiteFileID, begin->nID);
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageInfoProcReputation::InitHash()
{
	return 0;
}
//---------------------------------------------------------------------------

String					CManageInfoProcReputation::GetHash()
{
	return "";
}
//---------------------------------------------------------------------------

INT32					CManageInfoProcReputation::AddInfoProcReputation(DB_INFO_PROC_REPUTATION&	data)
{	
	if(SetER(t_DBMgrInfoProcReputation->InsertExecute(&data)))
	{
		return g_nErrRtn;
	}

	AddItem(data.nID, data);
	AddKeyID(data.nSiteFileID, data.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageInfoProcReputation::EditInfoProcReputation(DB_INFO_PROC_REPUTATION&	data)
{
	PDB_INFO_PROC_REPUTATION pdata = FindItem(data.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;	

	if(SetER(t_DBMgrInfoProcReputation->UpdateExecute(&data)))
	{
		return g_nErrRtn;
	}

	EditItem(data.nID, data);
	InitHash();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageInfoProcReputation::DelInfoProcReputation(UINT32 nID)
{
	PDB_INFO_PROC_REPUTATION pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;


	if(SetER(t_DBMgrInfoProcReputation->DeleteExecute(pdata->nID)))
	{
		return g_nErrRtn;
	}
	DelKeyID(pdata->nSiteFileID);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageInfoProcReputation::ApplyInfoProcReputation(DB_INFO_PROC_REPUTATION&	data)
{
	if(FindItem(data.nID))
	{
		return EditInfoProcReputation(data);
	}
	return AddInfoProcReputation(data);
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

INT32					CManageInfoProcReputation::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBInfoProcReputationItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageInfoProcReputation::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_INFO_PROC_REPUTATION pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageInfoProcReputation::SetPkt(PDB_INFO_PROC_REPUTATION pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_String(pdata->strFileKey);
	SendToken.TokenAdd_32(pdata->nReputation);
	SendToken.TokenAdd_32(pdata->nNumOfUser);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageInfoProcReputation::GetPkt(MemToken& RecvToken, DB_INFO_PROC_REPUTATION& data)
{
	if( RecvToken.TokenDel_String(data.strFileKey) < 0)			goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nReputation))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nNumOfUser))					goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



