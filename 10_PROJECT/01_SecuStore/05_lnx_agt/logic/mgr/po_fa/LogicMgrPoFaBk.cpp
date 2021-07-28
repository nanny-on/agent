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
#include "LogicMgrPoFaBk.h"

//---------------------------------------------------------------------------

CLogicMgrPoFaBk*		t_LogicMgrPoFaBk = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoFaBk::CLogicMgrPoFaBk()
{
	t_ManagePoFaBk	= new CManagePoFaBk();
	t_ManagePoFaBk->LoadDBMS();
	t_ManagePoFaBk->InitHash();

	m_strLogicName		= "mgr po fa bk";

	m_nPolicyType		= SS_POLICY_TYPE_FA_BK;	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_FA_BK;
}
//---------------------------------------------------------------------------

CLogicMgrPoFaBk::~CLogicMgrPoFaBk()
{
	SAFE_DELETE(t_ManagePoFaBk);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFaBk::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFaBk::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_FA_BK pdpfb = NULL;
	DB_PO_FA_BK dpfb;
	m_tDPH = &(dpfb.tDPH);

	if( t_ManagePoFaBk->GetPkt(RecvToken, dpfb))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

/*
	pdpfb = (PDB_PO_FA_BK)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdpfb)
	{
		t_ManagePoFaBk->DelPoFaBk(pdpfb->tDPH.nID);
	}
*/	
	t_ManagePoFaBk->ClearPoFaBk();

	{
		if(SetER(t_ManagePoFaBk->ApplyPoFaBk(dpfb)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	{
		if(dpfb.tDPH.nUsedMode == STATUS_USED_MODE_OFF)	
		{
			t_LogicMgrLogDoc->ChkDelBackupOp(t_EnvInfoOp->GetStopOpBySysOff());
			t_DocBackupUtil->RemoveBackupFilesByNonExistLog(t_EnvInfoOp->GetStopOpBySysOff());
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFaBk::ApplyPolicy()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFaBk::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFaBk::EndFindOrder(PMEM_FIND_ORDER_INFO pMFOI)
{
	switch(pMFOI->nOpType)
	{
	case SS_LOG_DOC_OP_TYPE_SCHEDULE:
		{
			if(UINT16(pMFOI->nNextOp) == G_CODE_COMMON_DEL)
			{
				t_ManageDocDeleteInfo->UpdateDocDeleteInfo();
				t_LogicDocDeleteInfo->SendPkt_DocDeleteInfo_Edit();
				t_LogicMgrLogDoc->ChkDelBackupOp(t_EnvInfoOp->GetStopOpBySysOff());
				t_DocBackupUtil->RemoveBackupFilesByNonExistLog(t_EnvInfoOp->GetStopOpBySysOff());
			}
			
			break;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
