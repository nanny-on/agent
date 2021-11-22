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
#include "LogicMgrPoFePtnOp.h"

//---------------------------------------------------------------------------

CLogicMgrPoFePtnOp*		t_LogicMgrPoFePtnOp = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoFePtnOp::CLogicMgrPoFePtnOp()
{
	t_ManagePoFePtnOp	= new CManagePoFePtnOp();
	t_ManagePoFePtnOp->LoadDBMS();
	t_ManagePoFePtnOp->InitHash();

	m_strLogicName		= "mgr po fe ptn op";

	m_nPolicyType		= SS_POLICY_TYPE_FE_PTN_OP;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_FE_PTN_OP;
}
//---------------------------------------------------------------------------

CLogicMgrPoFePtnOp::~CLogicMgrPoFePtnOp()
{
	SAFE_DELETE(t_ManagePoFePtnOp);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnOp::AnalyzePkt_FromMgr_Ext()
{
// 	switch(m_nPktCode)
// 	{
// 		break;
// 	}
	
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnOp::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_FE_PTN_OP pdpfpo = NULL;
	DB_PO_FE_PTN_OP dpfpo;
	m_tDPH = &(dpfpo.tDPH);

	if( t_ManagePoFePtnOp->GetPkt(RecvToken, dpfpo))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	{
		dpfpo.nPtnScanTime = GetCurrentDateTimeInt();
	}

	pdpfpo = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);

	if(pdpfpo && pdpfpo->tDPH.nID == dpfpo.tDPH.nID && pdpfpo->nPtnSchTime == dpfpo.nPtnSchTime)
	{
		dpfpo.nPtnScanTime = pdpfpo->nPtnScanTime;
	}

	if(pdpfpo)
	{
		t_ManagePoFePtnOp->DelPoFePtnOp(pdpfpo->tDPH.nID);
	}
	
	{
		if(SetER(t_ManagePoFePtnOp->ApplyPoFePtnOp(dpfpo)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnOp::ScanDefaultOption()
{
	PDB_PO_FE_PTN_OP pdb_po = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdb_po)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}	

	{
		UINT32 nLastChkTime = pdb_po->nPtnScanTime;		
		WriteLogN("start(check) fe ptn op demand : [uid:%d]", pdb_po->tDPH.nID);
		pdb_po->nPtnScanTime = nLastChkTime;
		t_ManagePoFePtnOp->EditPoFePtnOp(*pdb_po);

		{
			MEM_FIND_ORDER_INFO tMFOI;
			{
				tMFOI.nID		= t_ThreadPoFeScan->GetNextOrderID();

				tMFOI.nPoType	= m_nPolicyType;
				tMFOI.nPoID		= pdb_po->tDPH.nID;
				tMFOI.nOpType	= G_CODE_COMMON_SCAN;
			}
			t_ThreadPoFeScan->AddWork(&tMFOI);
		}
	}
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnOp::ApplyPolicy()
{
	PDB_PO_FE_PTN_OP pdpfpo = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdpfpo)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnOp::OnTimer_Logic()
{
	PDB_PO_FE_PTN_OP pdb_po = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdb_po)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	if(pdb_po->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
	{
		return 0;
	}

	{
		UINT32 nLastChkTime = pdb_po->nPtnScanTime;
		U64_SCHEDULE tIS;
		tIS.all = pdb_po->nPtnSchTime;

		if(IsValidSchedule(tIS.all, nLastChkTime) == 0)
		{
			return -1;
		}

		WriteLogN("start(check) fe ptn op schedule : [uid:%d]", pdb_po->tDPH.nID);
		pdb_po->nPtnScanTime = nLastChkTime;
		t_ManagePoFePtnOp->EditPoFePtnOp(*pdb_po);

		{
			MEM_FIND_ORDER_INFO tMFOI;
			{
				tMFOI.nID		= t_ThreadPoFeScan->GetNextOrderID();

				tMFOI.nPoType	= m_nPolicyType;
				tMFOI.nPoID		= pdb_po->tDPH.nID;
				tMFOI.nOpType	= G_CODE_COMMON_SCAN;
			}
			t_ThreadPoFeScan->AddWork(&tMFOI);
		}
	}	
	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
