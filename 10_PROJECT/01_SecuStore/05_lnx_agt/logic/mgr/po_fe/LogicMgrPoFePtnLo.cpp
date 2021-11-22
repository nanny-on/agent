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
#include "LogicMgrPoFePtnLo.h"

//---------------------------------------------------------------------------

CLogicMgrPoFePtnLo*		t_LogicMgrPoFePtnLo = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoFePtnLo::CLogicMgrPoFePtnLo()
{
	t_ManagePoFePtnLo	= new CManagePoFePtnLo();
	t_ManagePoFePtnLo->LoadDBMS();
	t_ManagePoFePtnLo->InitHash();

	m_strLogicName		= "mgr po fe ptn lo";

	m_nPolicyType		= SS_POLICY_TYPE_FE_PTN_LO;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_FE_PTN_LO;
}
//---------------------------------------------------------------------------

CLogicMgrPoFePtnLo::~CLogicMgrPoFePtnLo()
{
	SAFE_DELETE(t_ManagePoFePtnLo);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnLo::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnLo::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_FE_PTN_LO pdpfpl = NULL;
	DB_PO_FE_PTN_LO dpfpl;
	m_tDPH = &(dpfpl.tDPH);

	if( t_ManagePoFePtnLo->GetPkt(RecvToken, dpfpl))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	pdpfpl = (PDB_PO_FE_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdpfpl)
	{
		if( dpfpl.nPtnRangeType == pdpfpl->nPtnRangeType &&
			dpfpl.nPtnObjType	== pdpfpl->nPtnObjType)
		{
			dpfpl.strSavePath	= pdpfpl->strSavePath;
			dpfpl.strSaveName	= pdpfpl->strSaveName;
			dpfpl.strPtnVersion	= pdpfpl->strPtnVersion;
			dpfpl.strFileHash	= pdpfpl->strFileHash;
			dpfpl.nPtnChkValue	= pdpfpl->nPtnChkValue;
			dpfpl.nPtnUnitNum	= pdpfpl->nPtnUnitNum;
		}
		
		t_ManagePoFePtnLo->DelPoFePtnLo(pdpfpl->tDPH.nID);
	}
	
	{
		if(SetER(t_ManagePoFePtnLo->ApplyPoFePtnLo(dpfpl)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnLo::ApplyPolicy()
{
	PDB_PO_FE_PTN_LO pdpfpl = (PDB_PO_FE_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdpfpl)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		if(pdpfpl->tDPH.nUsedMode != STATUS_USED_MODE_OFF && pdpfpl->strSaveName.empty())
		{
			if(t_EnvInfoOp->m_nApplyPolicyStatus)	t_ThreadPoFePtnLo->AddWork(MAKEUINT64(pdpfpl->tDPH.nID, pdpfpl->tDPH.nSeqNo));
		}
		else
		{
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnLo::CheckRunEnv()
{
	PDB_PO_FE_PTN_LO pdpfpl = (PDB_PO_FE_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdpfpl)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		if(pdpfpl->tDPH.nUsedMode != STATUS_USED_MODE_OFF && t_ManagePoFePtnLo->IsValidPtnFile(pdpfpl) == 0)
		{
			t_ThreadPoFePtnLo->AddWork(MAKEUINT64(pdpfpl->tDPH.nID, pdpfpl->tDPH.nSeqNo));
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnLo::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFePtnLo::MakePtnEnd(PDB_PO_FE_PTN_LO pDPFPO)
{
	PDB_PO_FE_PTN_LO pdpfpl = (PDB_PO_FE_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdpfpl || pdpfpl->tDPH.nSeqNo != pDPFPO->tDPH.nSeqNo || pdpfpl->tDPH.nID != pDPFPO->tDPH.nID)
	{
		WriteLogE("[%s] invalid policy information : do not policy update", m_strLogicName.c_str());
		return -2;
	}

	{
		pdpfpl->strSavePath		= pDPFPO->strSavePath;
		pdpfpl->strSaveName		= pDPFPO->strSaveName;
		pdpfpl->strPtnVersion	= pDPFPO->strPtnVersion;
		pdpfpl->strFileHash		= pDPFPO->strFileHash;
		pdpfpl->nPtnUnitNum		= pDPFPO->nPtnUnitNum;
	}

	{
		if(SetER(t_ManagePoFePtnLo->ApplyPoFePtnLo(*pdpfpl)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return -1;
		}
	}	

	{
		DB_HOST_STATUS_PO dhsp;
		PDB_HOST_STATUS_PO pdhsp = t_ManageHostStatusPo->FirstItem();
		if(pdhsp)	dhsp = *pdhsp;

		{
			dhsp.strFeLoPtn	= pdpfpl->strPtnVersion;

			t_LogicMgrHostStatusPo->SendPkt_HostStatusPo(dhsp);
		}
	}

	{
		if(t_ThreadPoFePtnLo->IsWork() == 0)
		{
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
