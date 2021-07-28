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
#include "LogicMgrPoInDevOOp.h"

//---------------------------------------------------------------------------

CLogicMgrPoInDevOOp*		t_LogicMgrPoInDevOOp = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInDevOOp::CLogicMgrPoInDevOOp()
{
	t_ManagePoInDevOOp			= new CManagePoInDevOOp();

	t_ManagePoInDevOOp->LoadDBMS();

	t_ManagePoInDevOOp->InitHash();

	m_strLogicName		= "mgr agt po in devo op";
	
	m_nPolicyType		= SS_POLICY_TYPE_DV_OP;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_DV_OP;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_DV_OP;
}
//---------------------------------------------------------------------------

CLogicMgrPoInDevOOp::~CLogicMgrPoInDevOOp()
{
	SAFE_DELETE(t_ManagePoInDevOOp);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInDevOOp::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInDevOOp::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_DEVO_OP ppcp = NULL;
	DB_PO_IN_DEVO_OP data;

	m_tDPH = &(data.tDPH);
	
	if( t_ManagePoInDevOOp->GetPkt(RecvToken, data))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	ppcp = (PDB_PO_IN_DEVO_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(ppcp)
	{		
		t_ManagePoInDevOOp->DelPoInDevOOp(ppcp->tDPH.nID);
	}

	{
		if(SetER(t_ManagePoInDevOOp->ApplyPoInDevOOp(data)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}
//	t_ASIEPSAPPDLLUtil->Stop();
	t_ManageDevOInfo->CheckDvPolicy(TRUE);
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
