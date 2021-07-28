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
#include "ManageHostStatusPo.h"

//---------------------------------------------------------------------------

CManageHostStatusPo*	t_ManageHostStatusPo = NULL;

//---------------------------------------------------------------------------

CManageHostStatusPo::CManageHostStatusPo()
{
	t_DBMgrHostStatusPo	= new CDBMgrHostStatusPo();
}
//---------------------------------------------------------------------------

CManageHostStatusPo::~CManageHostStatusPo()
{
	if(t_DBMgrHostStatusPo)		{	delete t_DBMgrHostStatusPo;		t_DBMgrHostStatusPo = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageHostStatusPo::LoadDBMS()
{
	TListDBHostStatusPo tDBHostStatusPoList;
    TListDBHostStatusPoItor begin, end;
	if(SetER(t_DBMgrHostStatusPo->LoadExecute(&tDBHostStatusPoList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBHostStatusPoList.begin();	end = tDBHostStatusPoList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);

		AddKeyID(begin->nHostID, begin->nID);

		if(ClearDrvAndRunStatus(begin->nID))
			WriteLogE("host status po clear drv and run status fail [id:%d]", begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostStatusPo::AddHostStatusPo(DB_HOST_STATUS_PO&	dhsp)
{
	if(SetER(t_DBMgrHostStatusPo->InsertExecute(&dhsp)))
    {
    	return g_nErrRtn;
    }

    AddItem(dhsp.nID, dhsp);     

	AddKeyID(dhsp.nHostID, dhsp.nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostStatusPo::EditHostStatusPo(DB_HOST_STATUS_PO&	dhsp)
{
    if(t_DBMgrHostStatusPo->UpdateExecute(&dhsp))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(dhsp.nID, dhsp);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostStatusPo::DelHostStatusPo(UINT32 nID)
{
	PDB_HOST_STATUS_PO pdhsp = FindItem(nID);
	if(!pdhsp)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrHostStatusPo->DeleteExecute(pdhsp->nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyID(pdhsp->nHostID);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32	CManageHostStatusPo::ApplyHostStatusPo(DB_HOST_STATUS_PO& dhsp)
{
	if(FirstItem())		
	{
		return EditHostStatusPo(dhsp);
	}
	return AddHostStatusPo(dhsp);
}
//---------------------------------------------------------------------------

INT32	CManageHostStatusPo::ClearDrvAndRunStatus(UINT32 nID)
{
	PDB_HOST_STATUS_PO pdhsp = FindItem(nID);
	if(!pdhsp)
	{
		WriteLogE("not find host status po [id:%d]", nID);
		return -1;
	}

	pdhsp->nFaODrv			= 0;

	pdhsp->nDfRun			= 0;

	pdhsp->nFeOpDrv			= 0;
	pdhsp->nFeLoDrv			= 0;
	pdhsp->nFeLoRun			= 0;
	pdhsp->nFeGbDrv			= 0;
	pdhsp->nFeGbRun			= 0;
	pdhsp->nFeGwDrv			= 0;
	pdhsp->nFeGwRun			= 0;

	EditHostStatusPo(*pdhsp);

	return 0;
}
//---------------------------------------------------------------------------

INT32	CManageHostStatusPo::GetPkt(MemToken& RecvToken, DB_HOST_STATUS_PO& dhsp)
{
	if (!RecvToken.TokenDel_32(dhsp.nID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhsp.nUsedFlag))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhsp.nHostID))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(dhsp.nFaODrv))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhsp.nFaCUnit))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhsp.nDfRun))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhsp.nDfUnit))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(dhsp.nFeOpDrv))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(dhsp.nFeLoDrv))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhsp.nFeLoRun))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dhsp.strFeLoPtn) < 0)		goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(dhsp.nFeGbDrv))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhsp.nFeGbRun))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dhsp.strFeGbPtn) < 0)		goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(dhsp.nFeGwDrv))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhsp.nFeGwRun))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dhsp.strFeGwPtn) < 0)		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManageHostStatusPo::SetPkt(PDB_HOST_STATUS_PO pdhsp, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdhsp->nID);
	SendToken.TokenAdd_32(pdhsp->nUsedFlag);
	SendToken.TokenAdd_32(pdhsp->nHostID);

	SendToken.TokenAdd_32(pdhsp->nFaODrv);
	SendToken.TokenAdd_32(pdhsp->nFaCUnit);
	SendToken.TokenAdd_32(pdhsp->nDfRun);
	SendToken.TokenAdd_32(pdhsp->nDfUnit);

	SendToken.TokenAdd_32(pdhsp->nFeOpDrv);

	SendToken.TokenAdd_32(pdhsp->nFeLoDrv);
	SendToken.TokenAdd_32(pdhsp->nFeLoRun);
	SendToken.TokenAdd_String(pdhsp->strFeLoPtn);

	SendToken.TokenAdd_32(pdhsp->nFeGbDrv);
	SendToken.TokenAdd_32(pdhsp->nFeGbRun);
	SendToken.TokenAdd_String(pdhsp->strFeGbPtn);

	SendToken.TokenAdd_32(pdhsp->nFeGwDrv);
	SendToken.TokenAdd_32(pdhsp->nFeGwRun);
	SendToken.TokenAdd_String(pdhsp->strFeGwPtn);

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

void 		CManageHostStatusPo::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][host_status_po] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





