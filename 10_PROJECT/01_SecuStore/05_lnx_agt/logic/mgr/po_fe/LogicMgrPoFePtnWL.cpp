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
#include "LogicMgrPoFePtnWL.h"

//---------------------------------------------------------------------------

CLogicMgrPoFePtnWL*		t_LogicMgrPoFePtnWL = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoFePtnWL::CLogicMgrPoFePtnWL()
{
	t_ManagePoFePtnWL	= new CManagePoFePtnWL();
	t_ManagePoFePtnWL->LoadDBMS();
	t_ManagePoFePtnWL->InitHash();

	m_strLogicName		= "mgr po fe ptn wl";

	m_nPolicyType		= SS_POLICY_TYPE_FE_PTN_WL;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_FE_PTN_WL;
}
//---------------------------------------------------------------------------

CLogicMgrPoFePtnWL::~CLogicMgrPoFePtnWL()
{
	SAFE_DELETE(t_ManagePoFePtnWL);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnWL::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnWL::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_FE_PTN_WL pdpfpw = NULL;
	DB_PO_FE_PTN_WL dpfpw, dpfpw_old;
	m_tDPH = &(dpfpw.tDPH);

	if( t_ManagePoFePtnWL->GetPkt(RecvToken, dpfpw))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	pdpfpw = (PDB_PO_FE_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdpfpw)
	{
		dpfpw_old = *pdpfpw;
		t_ManagePoFePtnWL->DelPoFePtnWL(pdpfpw->tDPH.nID);
	}
	
	{
		if(SetER(t_ManagePoFePtnWL->ApplyPoFePtnWL(dpfpw)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}

		if(dpfpw_old.tDPH.nID != dpfpw.tDPH.nID)
		{
			m_nForceApplyPolicy = STATUS_MODE_ON;
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnWL::ApplyPolicy()
{
	PDB_PO_FE_PTN_WL pdpfpw = (PDB_PO_FE_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdpfpw)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		if(pdpfpw->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(t_ManagePoFePtnWL->IsValidPtnFile(pdpfpw) == 0 && t_EnvInfoOp->m_nApplyPolicyStatus)
			{
				AddDpDownInfo();;
			}
			else if(m_nForceApplyPolicy)
			{
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);		
			}
		}
		else
		{
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnWL::CheckRunEnv()
{
	PDB_PO_FE_PTN_WL pdpfpw = (PDB_PO_FE_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdpfpw)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		if(pdpfpw->tDPH.nUsedMode != STATUS_USED_MODE_OFF && t_ManagePoFePtnWL->IsValidPtnFile(pdpfpw) == 0)
		{
			AddDpDownInfo();
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnWL::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnWL::AddDpDownInfo()
{
	PDB_PO_SVR_INFO_UDT pdpsiu = (PDB_PO_SVR_INFO_UDT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_UDT);
	if(!pdpsiu)
	{
		WriteLogE("[%s] not find policy svr info udt", m_strLogicName.c_str());
		return -1;
	}
// 	else if(pdpsiu->tDPH.nUsedMode != STATUS_USED_MODE_ON)
// 	{
// 		WriteLogE("[%s] not used udt chk svr info list", m_strLogicName.c_str());
// 		return -2;
// 	}
	else if(pdpsiu->strDataSvrInfoList.empty())
	{
		WriteLogE("[%s] not exists udt chk svr info list", m_strLogicName.c_str());
		return -2;
	}

	PDB_PO_FE_PTN_WL pdpfpw = (PDB_PO_FE_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdpfpw)
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return -3;
	}

	if(pdpfpw->tDPFI.strSaveName.empty() || pdpfpw->tDPFI.strFileHash.empty())
	{
//		WriteLogE("[%s] invalid ptn file info [%s][%s]", m_strLogicName.c_str(), pdpfpw->tDPFI.strSaveName.c_str(), pdpfpw->tDPFI.strFileHash.c_str());
		return -4;
	}

	{
		ASI_FDL_INFO tAFI;

		tAFI.nID = t_EnvInfoOp->GetGlobalID();
		tAFI.nItemType = ASIFDL_DL_FILE_TYPE_WPTN_GWL;
		tAFI.nItemID = 0;	
		tAFI.nPolicyID = pdpfpw->tDPH.nID;
		tAFI.nDLSvrUsedFlag	= (ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE);

		strncpy(tAFI.szDLPath, STR_DEPLOY_FILE_HOME_WPTN, MAX_PATH);

		{	
			strncpy(tAFI.szFileName, pdpfpw->tDPFI.strSaveName.c_str(), MAX_PATH-1);
			strncpy(tAFI.szFileHash, pdpfpw->tDPFI.strFileHash.c_str(), MAX_PATH-1);
		}	

		if(t_ManageFileDown->IsExistDnInfo(tAFI.nItemType, tAFI.nPolicyID, tAFI.nItemID))
		{
			WriteLogN("already exists fe ptn bl download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList.c_str());
			return 0;
		}

		WriteLogN("start fe ptn bl download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList.c_str());
/*
		if(t_ASIFDLDLLUtil)
		{
			t_ASIFDLDLLUtil->SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList.c_str());
			t_ASIFDLDLLUtil->AddDLInfo(&tAFI);
		}
*/
		SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList.c_str());
		AddDLInfo(&tAFI);

		t_ManageFileDown->AddItem(tAFI);
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
