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
#include "ManageHostStatusPoNc.h"

//---------------------------------------------------------------------------

CManageHostStatusPoNc*	t_ManageHostStatusPoNc = NULL;

//---------------------------------------------------------------------------

CManageHostStatusPoNc::CManageHostStatusPoNc()
{
	t_DBMgrHostStatusPoNc	= new CDBMgrHostStatusPoNc();
}
//---------------------------------------------------------------------------

CManageHostStatusPoNc::~CManageHostStatusPoNc()
{
	if(t_DBMgrHostStatusPoNc)		{	delete t_DBMgrHostStatusPoNc;		t_DBMgrHostStatusPoNc = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageHostStatusPoNc::LoadDBMS()
{
	TListDBHostStatusPoNc tDBHostStatusPoNcList;
    TListDBHostStatusPoNcItor begin, end;
	if(SetER(t_DBMgrHostStatusPoNc->LoadExecute(&tDBHostStatusPoNcList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBHostStatusPoNcList.begin();	end = tDBHostStatusPoNcList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);

		AddKeyID(begin->nHostID, begin->nID);

		if(ClearDrvAndRunStatus(begin->nID))
			WriteLogE("host status po nc clear drv and run status fail [id:%d]", begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostStatusPoNc::AddHostStatusPoNc(DB_HOST_STATUS_PO_NC&	data)
{
	if(SetER(t_DBMgrHostStatusPoNc->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.nID, data);     

	AddKeyID(data.nHostID, data.nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostStatusPoNc::EditHostStatusPoNc(DB_HOST_STATUS_PO_NC&	data)
{
    if(t_DBMgrHostStatusPoNc->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostStatusPoNc::DelHostStatusPoNc(UINT32 nID)
{
	PDB_HOST_STATUS_PO_NC pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrHostStatusPoNc->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyID(pdata->nHostID);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32	CManageHostStatusPoNc::ApplyHostStatusPoNc(DB_HOST_STATUS_PO_NC& data)
{
	if(FirstItem())		
	{
		return EditHostStatusPoNc(data);
	}
	return AddHostStatusPoNc(data);
}
//---------------------------------------------------------------------------

INT32	CManageHostStatusPoNc::ClearDrvAndRunStatus(UINT32 nID)
{
	PDB_HOST_STATUS_PO_NC pdata = FindItem(nID);
	if(!pdata)
	{
		WriteLogE("not find host status po nc [id:%d]", nID);
		return -1;
	}

	pdata->tWorkModeMap.clear();
	HexToMap("", pdata->tWorkModeMap, SS_HOST_STATUS_PO_NC_INFO_INDEX_LAST);
	
	EditHostStatusPoNc(*pdata);

	return 0;
}
//---------------------------------------------------------------------------

INT32	CManageHostStatusPoNc::GetPkt(MemToken& RecvToken, DB_HOST_STATUS_PO_NC& data)
{
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedFlag))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nHostID))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_IDMap(data.tWorkModeMap) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_IDMapStr(data.tPtnVerMap) < 0)		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManageHostStatusPoNc::SetPkt(PDB_HOST_STATUS_PO_NC pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedFlag);
	SendToken.TokenAdd_32(pdata->nHostID);

	SendToken.TokenAdd_IDMap(pdata->tWorkModeMap);
	SendToken.TokenAdd_IDMapStr(pdata->tPtnVerMap);


	SendToken.TokenSet_Block();
    
 	return 0;
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

void 		CManageHostStatusPoNc::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][host_status_po_nc] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





