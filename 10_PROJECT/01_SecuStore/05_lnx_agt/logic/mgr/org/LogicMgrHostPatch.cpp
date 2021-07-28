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
#include "LogicMgrHostPatch.h"

//---------------------------------------------------------------------------

CLogicMgrHostPatch*		t_LogicMgrHostPatch = NULL;

//---------------------------------------------------------------------------

CLogicMgrHostPatch::CLogicMgrHostPatch()
{
	t_ManageHostPatch		= new CManageHostPatch();

	t_ManageHostPatch->LoadDBMS();

	m_strLogicName = "logic mgr host patch";

	m_nControlType	= SS_CONTROL_TYPE_ORG_HOST;
	m_nEvtObjType	= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode	= EVENT_OBJECT_CODE_HOST_PATCH;
}
//---------------------------------------------------------------------------

CLogicMgrHostPatch::~CLogicMgrHostPatch()
{
	SAFE_DELETE(t_ManageHostPatch);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrHostPatch::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
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

INT32		CLogicMgrHostPatch::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_HOST_PATCH pdata = NULL;
	TListDBHostPatch tDBHostPatchList;
	DB_HOST_PATCH data;

	if(!RecvToken.TokenDel_32(m_nRecvNum))		return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		if( t_ManageHostPatch->GetPkt(RecvToken, data))		return AZPKT_CB_RTN_PKT_INVALID;

		tDBHostPatchList.push_back(data);
	}

	{
		SendToken.TokenAdd_32(ERR_SUCCESS);	
		t_ManageHostPatch->SetPkt(SendToken);

		SendData_Link(G_TYPE_HOST_PATCH, G_CODE_COMMON_EDIT, SendToken);
		SendToken.Clear();
	}

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrHostPatch::ScanPatchList(UINT32 nPolicy, UINT32 nItemID)
{
}
//---------------------------------------------------------------------------

void		CLogicMgrHostPatch::SendPkt_HostPatch(UINT32 nWorkID)
{
}
//---------------------------------------------------------------------------

void		CLogicMgrHostPatch::MakePatchInfo(TListDBHostPatch& tUnitList)
{
}
//---------------------------------------------------------------------------

void		CLogicMgrHostPatch::SendPkt_Sync()
{
	SendToken.Clear();
	t_ManageHostPatch->SetPkt(SendToken);
	SendData_Mgr(G_TYPE_HOST_PATCH, G_CODE_COMMON_SYNC, SendToken);
	return;
}
//---------------------------------------------------------------------------
