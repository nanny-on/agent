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

#include "stdafx.h"
#include "com_struct.h"
#include "LogicApplyPolicy.h"

//---------------------------------------------------------------------------

CLogicApplyPolicy*	t_LogicApplyPolicy = NULL;

CLogicApplyPolicy::CLogicApplyPolicy()
{
	m_strLogicName			= "logic apply policy";
	m_nChkTSProtectMode		= t_EnvInfo->GetReg_LocalEnv_TSProtectMode();
	
//	m_nDrvStartMode			= SERVICE_DEMAND_START;
	m_tEPSHkPIDList.clear();
}
//---------------------------------------------------------------------------

CLogicApplyPolicy::~CLogicApplyPolicy()
{	
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::CheckRunEnv()
{
	if(t_EnvInfoOp->m_nApplyPolicyStatus == STATUS_USED_MODE_OFF)	
	{
		return 0;
	}

	t_LogicMgrPoFePtnLo->CheckRunEnv();
	t_LogicMgrPoFePtnBL->CheckRunEnv();
	t_LogicMgrPoFePtnWL->CheckRunEnv();

	t_LogicMgrPoInPtnBL->CheckRunEnv();
	t_LogicMgrPoInPtnWL->CheckRunEnv();

	if(t_LogicMgrPoNcPtnLo)
		t_LogicMgrPoNcPtnLo->CheckRunEnv();
	if(t_LogicMgrPoNcPtnBL)
		t_LogicMgrPoNcPtnBL->CheckRunEnv();
	if(t_LogicMgrPoNcPtnWL)
		t_LogicMgrPoNcPtnWL->CheckRunEnv();

	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NPMS, 0, 0))
	{
		t_LogicMgrPoPmOp->CheckRunEnv();
	}

	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_POLICY_TYPE_IN_VULN_OP, 0))
	{
		t_LogicMgrPoInVulnOp->CheckRunEnv();
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::ApplyPolicy()
{
	if(t_EnvInfoOp->m_nApplyPolicyStatus == STATUS_USED_MODE_OFF)	
	{
		return 0;
	}

	ApplyPolicy_Timer();

	t_EnvInfoOp->m_nApplyPolicyStatus = STATUS_USED_MODE_ON;
	return 0;
}

//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::ApplyBootPolicy()
{	
	if(t_ManagePoHostRun->IsApplyPolicyFromBootTime())
	{
		WriteLogN("enable boot apply policy.");
		t_EnvInfoOp->m_nApplyPolicyStatus = STATUS_USED_MODE_ON;
		CheckRunEnv();
		t_LogicApplyPolicy->ResetEPSDrv();

		{
			if(t_LogicMgrPoHostWebSock)
				t_LogicMgrPoHostWebSock->ApplyPolicy();
			t_LogicMgrPoInVulnOp->PreScanVuln();
		}

		t_EnvInfoOp->m_nApplyPolicyStatus = STATUS_USED_MODE_OFF;
	}

	return 0;
}

//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::ApplyPolicy_Timer()
{
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);

	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_CHECK);

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::IsActiveEPS()
{
	UINT32 nUsedMode = STATUS_USED_MODE_OFF;
	UINT32 nReadyStatus = STATUS_USED_MODE_ON;
	CFileUtil tFileUtil;

	if(m_nChkTSProtectMode)
	{
		WriteLogN("[%s] eps protect function disabled by technical supporter [ts_pm].", m_strLogicName.c_str());
		return nUsedMode;
	}

	{
		nReadyStatus = STATUS_USED_MODE_ON;
		PDB_PO_CTL_PANEL pCurPolicy = (PDB_PO_CTL_PANEL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_CTL_PANEL);
		if(pCurPolicy)	
		{
			nUsedMode += pCurPolicy->tDPH.nUsedMode;
			m_tStatusMap[SS_POLICY_TYPE_CTL_PANEL] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
		}
	}
	{
		nReadyStatus = STATUS_USED_MODE_ON;
		PDB_PO_CTL_PROC pCurPolicy = (PDB_PO_CTL_PROC)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_CTL_PROC);
		if(pCurPolicy)	
		{
			nUsedMode += pCurPolicy->tDPH.nUsedMode;
			m_tStatusMap[SS_POLICY_TYPE_CTL_PROC] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
		}
	}
	{
		nReadyStatus = STATUS_USED_MODE_ON;
		PDB_PO_SELF_PROTECT pCurPolicy = (PDB_PO_SELF_PROTECT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SELF_PROTECT);
		if(pCurPolicy)
		{
			nUsedMode += pCurPolicy->tDPH.nUsedMode;
			m_tStatusMap[SS_POLICY_TYPE_SELF_PROTECT] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
		}
	}


	{
		nReadyStatus = STATUS_USED_MODE_ON;
		PDB_PO_FA_OP pCurPolicy = (PDB_PO_FA_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_OP);
		if(pCurPolicy)
		{
			nUsedMode += pCurPolicy->tDPH.nUsedMode;
			m_tStatusMap[SS_POLICY_TYPE_FA_OP] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
		}
	}
	{
		nReadyStatus = STATUS_USED_MODE_ON;
		PDB_PO_FA_PROC pCurPolicy = (PDB_PO_FA_PROC)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_PROC);
		if(pCurPolicy)
		{
			nUsedMode += pCurPolicy->tDPH.nUsedMode;
			m_tStatusMap[SS_POLICY_TYPE_FA_PROC] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
		}
	}

	{
		nReadyStatus = STATUS_USED_MODE_ON;
		PDB_PO_FA_CLEAR pCurPolicy = (PDB_PO_FA_CLEAR)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_CLEAR);
		if(pCurPolicy)
		{
			if(pCurPolicy->tDPH.nUsedMode == STATUS_USED_MODE_ON && pCurPolicy->tDPH.nExtOption & SS_PO_FA_CLEAR_EXT_OPTION_FLAG_SECU_DELETE_AUTO)
			{
				nUsedMode += STATUS_USED_MODE_ON;
				m_tStatusMap[SS_POLICY_TYPE_FA_CLEAR] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] fa clear policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
			else
			{
				WriteLogN("[%s] fa clear policy used mode : [%d]", m_strLogicName.c_str(), 0);
			}
		}
	}

	{
		nReadyStatus = STATUS_USED_MODE_ON;
		PDB_PO_FE_PTN_OP pCurPolicy = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_OP);
		if(pCurPolicy)	
		{
			if(t_ManagePoFePtnOp->IsValidPtnFile(pCurPolicy->tDPH.nID))
			{
				nUsedMode += pCurPolicy->tDPH.nUsedMode;
			}
			else
			{
				nReadyStatus = STATUS_USED_MODE_OFF;
			}

			m_tStatusMap[SS_POLICY_TYPE_FE_PTN_OP] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
			WriteLogN("[%s] fe ptn op policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
		}
	}

	{
		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_FE_PTN_LO pCurPolicy = (PDB_PO_FE_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_LO);
			if(pCurPolicy)	
			{
				if(t_ManagePoFePtnLo->IsValidPtnFile(pCurPolicy->tDPH.nID))
				{
					nUsedMode += pCurPolicy->tDPH.nUsedMode;
				}
				else
				{
					nReadyStatus = STATUS_USED_MODE_OFF;
				}

				m_tStatusMap[SS_POLICY_TYPE_FE_PTN_LO] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] fe ptn lo policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}
		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_FE_PTN_BL pCurPolicy = (PDB_PO_FE_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_BL);
			if(pCurPolicy)	
			{
				if(t_ManagePoFePtnBL->IsValidPtnFile(pCurPolicy->tDPH.nID))
				{
					nUsedMode += pCurPolicy->tDPH.nUsedMode;
				}
				else
				{
					nReadyStatus = STATUS_USED_MODE_OFF;
				}
				m_tStatusMap[SS_POLICY_TYPE_FE_PTN_BL] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] fe ptn bl policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}
		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_FE_PTN_WL pCurPolicy = (PDB_PO_FE_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_WL);
			if(pCurPolicy)	
			{
				if(t_ManagePoFePtnWL->IsValidPtnFile(pCurPolicy->tDPH.nID))
				{
					nUsedMode += pCurPolicy->tDPH.nUsedMode;
				}
				else
				{
					nReadyStatus = STATUS_USED_MODE_OFF;
				}
				m_tStatusMap[SS_POLICY_TYPE_FE_PTN_WL] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] fe ptn wl policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}
	}

	{
		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_NC_PTN_OP pCurPolicy = (PDB_PO_NC_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_OP);
			if(pCurPolicy)
			{
				if(t_ManagePoNcPtnOp->IsValidPtnFile(pCurPolicy->tDPH.nID))
				{
					nUsedMode += pCurPolicy->tDPH.nUsedMode;
				}
				else
				{
					nReadyStatus = STATUS_USED_MODE_OFF;
				}
				m_tStatusMap[SS_POLICY_TYPE_NC_PTN_OP] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] nc ptn op policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}

		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_NC_PTN_LO pCurPolicy = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_LO);
			if(pCurPolicy)
			{
				if(t_ManagePoNcPtnLo->IsValidPtnFile(pCurPolicy->tDPH.nID) && t_LogicMgrPtnLWC->IsInitLogic())
				{
					nUsedMode += pCurPolicy->tDPH.nUsedMode;
				}
				else
				{
					nReadyStatus = STATUS_USED_MODE_OFF;
				}
				m_tStatusMap[SS_POLICY_TYPE_NC_PTN_LO] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] nc ptn lo policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}

		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_NC_PTN_BL pCurPolicy = (PDB_PO_NC_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_BL);
			if(pCurPolicy)
			{
				if(t_ManagePoNcPtnBL->IsValidPtnFile(pCurPolicy->tDPH.nID) && t_LogicMgrPtnGBC->IsInitLogic())
				{
					nUsedMode += pCurPolicy->tDPH.nUsedMode;
				}
				else
				{
					nReadyStatus = STATUS_USED_MODE_OFF;
				}
				m_tStatusMap[SS_POLICY_TYPE_NC_PTN_BL] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] nc ptn bl policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}

		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_NC_PTN_WL pCurPolicy = (PDB_PO_NC_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_WL);
			if(pCurPolicy)
			{
				if(t_ManagePoNcPtnWL->IsValidPtnFile(pCurPolicy->tDPH.nID) && t_LogicMgrPtnGWC->IsInitLogic())
				{
					nUsedMode += pCurPolicy->tDPH.nUsedMode;
				}
				else
				{
					nReadyStatus = STATUS_USED_MODE_OFF;
				}
				m_tStatusMap[SS_POLICY_TYPE_NC_PTN_WL] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] nc ptn wl policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}
	}

	{
		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_IN_PTN_OP pCurPolicy = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
			if(pCurPolicy)
			{
				if(t_ManagePoInPtnOp->IsValidPtnFile(pCurPolicy->tDPH.nID))
				{
					nUsedMode += pCurPolicy->tDPH.nUsedMode;
				}
				else
				{
					nReadyStatus = STATUS_USED_MODE_OFF;
				}
				m_tStatusMap[SS_POLICY_TYPE_IN_PTN_OP] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] in ptn op policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}

		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_IN_PTN_BL pCurPolicy = (PDB_PO_IN_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_BL);
			if(pCurPolicy)
			{
				if(t_ManagePoInPtnBL->IsValidPtnFile(pCurPolicy->tDPH.nID) && t_LogicMgrPtnGBO->IsInitLogic())
				{
					nUsedMode += pCurPolicy->tDPH.nUsedMode;
				}
				else
				{
					nReadyStatus = STATUS_USED_MODE_OFF;
				}
				m_tStatusMap[SS_POLICY_TYPE_IN_PTN_BL] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] in ptn bl policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}

		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_IN_PTN_WL pCurPolicy = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_WL);
			if(pCurPolicy)
			{
				if(t_ManagePoInPtnWL->IsValidPtnFile(pCurPolicy->tDPH.nID) && t_LogicMgrPtnGWO->IsInitLogic())
				{
					nUsedMode += pCurPolicy->tDPH.nUsedMode;
				}
				else
				{
					nReadyStatus = STATUS_USED_MODE_OFF;
				}
				m_tStatusMap[SS_POLICY_TYPE_IN_PTN_WL] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] in ptn wl policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}

		{
			nReadyStatus = STATUS_USED_MODE_ON;
			PDB_PO_IN_RS_OP pCurPolicy = (PDB_PO_IN_RS_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_RS_OP);
			if(pCurPolicy)
			{
				nUsedMode += pCurPolicy->tDPH.nUsedMode;
				m_tStatusMap[SS_POLICY_TYPE_IN_RS_OP] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
				WriteLogN("[%s] in rs op policy used mode : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nUsedMode);
			}
		}
	}

	{
		nReadyStatus = STATUS_USED_MODE_ON;
		PDB_PO_IN_DEVO_OP pCurPolicy = (PDB_PO_IN_DEVO_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_DV_OP);
		if(pCurPolicy)
		{
			nUsedMode += pCurPolicy->tDPH.nUsedMode;
			m_tStatusMap[SS_POLICY_TYPE_DV_OP] = MAKEUINT64(pCurPolicy->tDPH.nUsedMode, nReadyStatus);
		}
	}

	return nUsedMode;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::SetEPSSvcDisable()
{
	return SetEPSSvcPolicy(STATUS_USED_MODE_ON);
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

INT32		CLogicApplyPolicy::SetEPSSvcPolicy(UINT32 nUserMode)
{
//	t_NannyAgtSvc.SetControlAccept(SERVICE_ACCEPT_STOP, TRUE);
	if(nUserMode)	
	{
		WriteLogN("[%s] set eps svc protect by update : [%s].", m_strLogicName.c_str(), "unlock");
		return 0;
	}

	if(m_nChkTSProtectMode)
	{
		WriteLogN("[%s] set eps svc protect by ts_pm [%d]: [%s].", m_strLogicName.c_str(), m_nChkTSProtectMode, "unlock");
		return 0;
	}

	if(t_EnvInfoOp->m_nModuleUpdateMode == STATUS_USED_MODE_ON)
	{
		WriteLogN("[%s] set eps svc protect by update #2 : [%s].", m_strLogicName.c_str(), "unlock");
		return 0;
	}

	PDB_PO_SELF_PROTECT pCurPolicy = (PDB_PO_SELF_PROTECT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SELF_PROTECT);
	if(!pCurPolicy)
	{
		WriteLogE("[%s] set eps svc protect : not find current policy.", m_strLogicName.c_str());
		return -1;
	}

	if(pCurPolicy->tDPH.nUsedMode != STATUS_USED_MODE_ON)
	{
		return 0;
	}

	DWORD dwCtlAcceptMode = TRUE;
	if(pCurPolicy->nProtectMode & SS_PO_SELF_PROTECT_MODE_SERVICE)
	{
		dwCtlAcceptMode = FALSE;
	}

//	t_NannyAgtSvc.SetControlAccept(SERVICE_ACCEPT_STOP, dwCtlAcceptMode);
	WriteLogN("[%s] set eps svc protect : [%s].", m_strLogicName.c_str(), (dwCtlAcceptMode ? "unlock" : "lock"));
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::SetEPSHookPolicy(INT32 nMode)
{
	INT32 nRtn = 0;
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::SetEPSHookModuleByTS(INT32 nMode, UINT32 nHookPID)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::SetEPSDrvPolicy()
{
	INT32 nRetVal = 0;
	{
		MemToken stSendToken(2048);
		if(t_ManageEnvSelfProtectAgt->Count())
		{		
			t_ManageEnvSelfProtectAgt->SetPkt(stSendToken);
			WriteLogN("[%s] set env self protect agt success : [%d]", m_strLogicName.c_str(), t_ManageEnvSelfProtectAgt->Count());
			stSendToken.Clear();
		}

		if(t_ManageEnvTrustSubject->Count())
		{		
			t_ManageEnvTrustSubject->SetPkt(stSendToken);
			WriteLogN("[%s] set env trust subject success : [%d]", m_strLogicName.c_str(), t_ManageEnvTrustSubject->Count());
			stSendToken.Clear();
		}

		if(t_ManageSiteFile->Count())
		{		
			t_ManageSiteFile->SetPkt(stSendToken);
			WriteLogN("[%s] set site file success : [%d]", m_strLogicName.c_str(), t_ManageSiteFile->Count());
			stSendToken.Clear();
		}

		do
		{		
			PDB_PO_CTL_PANEL pCurPolicy = (PDB_PO_CTL_PANEL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_CTL_PANEL);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po ctl panel fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			// 20200724 edit jhjung
		//	t_ManagePoCtlPanel->SetPktHost(pCurPolicy->tDPH.nID, SendToken);
			t_ManagePoCtlPanel->SetPktHost_EPS(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po ctl panel success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

		}while(FALSE);

		do
		{		
			PDB_PO_CTL_PROC pCurPolicy = (PDB_PO_CTL_PROC)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_CTL_PROC);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po ctl proc fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			// 20200724 edit jhjung
		//	t_ManagePoCtlProc->SetPktHost(pCurPolicy->tDPH.nID, SendToken);
			t_ManagePoCtlProc->SetPktHost_EPS(pCurPolicy->tDPH.nID, stSendToken);
/*			
			if(SetER(t_ASIEPSAPPDLLUtil->SetPoCtlProc(SendToken.GetData(), SendToken.GetLength())))
			{
				WriteLogE("[%s] set po ctl proc fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				return -1;
			}
*/
			WriteLogN("[%s] set po ctl proc success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

		}while(FALSE);

		do
		{		
			PDB_PO_SELF_PROTECT pCurPolicy = (PDB_PO_SELF_PROTECT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SELF_PROTECT);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po self protect fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			// 20200724 edit jhjung		
		//	t_ManagePoSelfProtect->SetPktHost(pCurPolicy->tDPH.nID, SendToken);	
			t_ManagePoSelfProtect->SetPktHost_EPS(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po self protect success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

		}while(FALSE);

		do
		{		
			PDB_PO_FA_ENV pCurPolicy = (PDB_PO_FA_ENV)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_ENV);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po fa env fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			// 20200724 edit jhjung		
		//	t_ManagePoFaEnv->SetPktHost(pCurPolicy->tDPH.nID, SendToken);
			t_ManagePoFaEnv->SetPktHost_EPS(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po fa env success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

		}while(FALSE);

		do
		{		
			PDB_PO_FA_OP pCurPolicy = (PDB_PO_FA_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_OP);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po fa op fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			// 20200724 edit jhjung		
		//	t_ManagePoFaOp->SetPktHost(pCurPolicy->tDPH.nID, SendToken);
			nRetVal = t_ManagePoFaOp->SetPktHost_EPS(pCurPolicy->tDPH.nID, stSendToken);
			if(nRetVal == 0)
				WriteLogN("[%s] set po fa op success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			else
				WriteLogE("[%s] fail to set po fa op : [%d] [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID, nRetVal);
			stSendToken.Clear();

			nRetVal = t_ManagePoFaOp->SetInotifyFaOp(pCurPolicy->tDPH.nID);
			if(nRetVal == 0)
				WriteLogN("[%s] success to set inotify fa op  : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			else
				WriteLogE("[%s] fail to set inotify fa op : [%d] [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID, nRetVal);
		}while(FALSE);

		do
		{		
			PDB_PO_FA_PROC pCurPolicy = (PDB_PO_FA_PROC)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_PROC);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po fa proc fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			// 20200724 edit jhjung		
		//	t_ManagePoFaProc->SetPktHost(pCurPolicy->tDPH.nID, SendToken);	
			t_ManagePoFaProc->SetPktHost_EPS(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po fa proc success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

		}while(FALSE);

		do
		{		
			PDB_PO_FE_PTN_OP pCurPolicy = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_OP);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po fe ptn op fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoFePtnOp->SetPktHost(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po fe ptn op success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear(); 

		}while(FALSE);

		do
		{		
			PDB_PO_FE_PTN_LO pCurPolicy = (PDB_PO_FE_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_LO);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po fe ptn lo fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoFePtnLo->SetPktHost(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po fe ptn lo success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

		}while(FALSE);

		do
		{		
			PDB_PO_FE_PTN_BL pCurPolicy = (PDB_PO_FE_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_BL);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po fe ptn bl fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoFePtnBL->SetPkt(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po fe ptn bl success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

		}while(FALSE);

		do
		{		
			PDB_PO_FE_PTN_WL pCurPolicy = (PDB_PO_FE_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_WL);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po fe ptn wl fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoFePtnWL->SetPkt(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po fe ptn wl success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

		}while(FALSE);

		do
		{		
			PDB_PO_FE_EXCLUDE pCurPolicy = (PDB_PO_FE_EXCLUDE)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_EXCLUDE);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po fe exclude fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoFeExclude->SetPktHost(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po fe exclude success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

		}while(FALSE);

		do
		{		
			PDB_PO_FE_SINGLE_PTN pCurPolicy = (PDB_PO_FE_SINGLE_PTN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_SINGLE_PTN);
			if(!pCurPolicy)
			{
//				WriteLogE("[%s] set po fe single ptn fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoFeSinglePtn->SetPktHost(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po fe single ptn success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

		}while(FALSE);

		do
		{		
			PDB_PO_IN_PTN_OP pCurPolicy = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po in ptn op fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoInPtnOp->SetPktHost(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po in ptn op success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();
			if(t_ThreadPoInAccFile != NULL)
				t_ThreadPoInAccFile->SendPolicy(AS_SEND_POLICY_INFO);

		}while(FALSE);		

		do
		{		
			PDB_PO_IN_PTN_BL pCurPolicy = (PDB_PO_IN_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_BL);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po in ptn bl fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoInPtnBL->SetPktHost(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po in ptn bl success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

			if(pCurPolicy->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
			{
				if(t_ManagePoInPtnBL->IsValidPtnFile(pCurPolicy) && t_LogicMgrPtnGBO->IsInitLogic() == 0)
				{
					WriteLogE("[%s] po in ptn bl ptn init fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					return -1;
				}
/*
				t_MMPPGBO->t_ManagePtnProcFile->SetPkt(SendToken);
				if(SetER(t_ASIEPSAPPDLLUtil->SetPoInPtnBLPtn(SendToken.GetData(), SendToken.GetLength())))
				{
					WriteLogE("[%s] set po in ptn bl ptn fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					return -1;
				}
*/
				WriteLogN("[%s] set po in ptn bl ptn success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
				stSendToken.Clear();
				if(t_ThreadPoInAccFile != NULL)
					t_ThreadPoInAccFile->SendPolicy(AS_SEND_POLICY_INFO);
			}			

		}while(FALSE);

		do
		{		
			PDB_PO_IN_PTN_WL pCurPolicy = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_WL);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po in ptn wl fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoInPtnWL->SetPktHost(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po in ptn wl success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

			if(pCurPolicy->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
			{
				if(t_ManagePoInPtnWL->IsValidPtnFile(pCurPolicy) && t_LogicMgrPtnGWO->IsInitLogic() == 0)
				{
					WriteLogE("[%s] po in ptn wl ptn init fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					return -1;
				}
				if(t_MMPPGWO)
					t_MMPPGWO->t_ManagePtnProcFile->SetPkt(stSendToken);
/*
				if(SetER(t_ASIEPSAPPDLLUtil->SetPoInPtnWLPtn(SendToken.GetData(), SendToken.GetLength())))
				{
					WriteLogE("[%s] set po in ptn wl ptn fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					return -1;
				}
*/
				WriteLogN("[%s] set po in ptn wl ptn success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
				stSendToken.Clear();
				if(t_ThreadPoInAccFile != NULL)
					t_ThreadPoInAccFile->SendPolicy(AS_SEND_POLICY_INFO);
			}			

		}while(FALSE);

		do
		{		
			PDB_PO_IN_PTN_EX pCurPolicy = (PDB_PO_IN_PTN_EX)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_EX);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po in ptn ex fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoInPtnEx->SetPktHost(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po in ptn ex success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

			t_ManagePoInPtnExTrust->SetPkt(stSendToken);
			WriteLogN("[%s] set po in ptn ex trust success : [%d]", m_strLogicName.c_str(), t_ManagePoInPtnExTrust->Count());
			stSendToken.Clear();
			if(t_ThreadPoInAccFile != NULL)
				t_ThreadPoInAccFile->SendPolicy(AS_SEND_POLICY_INFO);
		}while(FALSE);

		do
		{		
			PDB_PO_IN_PTN_SP pCurPolicy = (PDB_PO_IN_PTN_SP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_SP);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po in ptn sp fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoInPtnSP->SetPktHost(pCurPolicy->tDPH.nID, stSendToken);
			WriteLogN("[%s] set po in ptn sp success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();

			t_ManagePoInPtnSPRule->SetPkt(stSendToken);
			WriteLogN("[%s] set po in ptn sp rule success : [%d]", m_strLogicName.c_str(), t_ManagePoInPtnSPRule->Count());
			stSendToken.Clear();
			if(t_ThreadPoInAccFile != NULL)
				t_ThreadPoInAccFile->SendPolicy(AS_SEND_POLICY_INFO);
		}while(FALSE);
		//--------------------------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------------------------		

		do
		{		
			PDB_PO_IN_RS_OP pCurPolicy = (PDB_PO_IN_RS_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_RS_OP);
			if(!pCurPolicy)
			{
				WriteLogE("[%s] set po in rs op fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoInRsOp->SetPktHost(pCurPolicy->tDPH.nID, stSendToken);
/*
			if(SetER(t_ASIEPSAPPDLLUtil->SetPoInRsOp(SendToken.GetData(), SendToken.GetLength())))
			{
				WriteLogE("[%s] set po in rs op fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				return -1;
			}
*/
			WriteLogN("[%s] set po in rs op success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();
		}while(FALSE);

		do
		{		
			PDB_PO_IN_RS_BK pCurPolicy = (PDB_PO_IN_RS_BK)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_RS_BK);
			if(!pCurPolicy)
			{
//				WriteLogE("[%s] set po in rs bk fail : not find cur policy", m_strLogicName.c_str());
				break;
			}

			t_ManagePoInRsBk->SetPkt(pCurPolicy->tDPH.nID, stSendToken);
/*
			if(SetER(t_ASIEPSAPPDLLUtil->SetPoInRsBk(SendToken.GetData(), SendToken.GetLength())))
			{
				WriteLogE("[%s] set po in rs bk fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				return -1;
			}
*/
			WriteLogN("[%s] set po in rs bk success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
			stSendToken.Clear();
		}while(FALSE);
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::SetEPSDrvPolicy_PoDv()
{
	MemToken stSendToken(2048);

	do
	{		
		PDB_PO_IN_DEVO_OP pCurPolicy = (PDB_PO_IN_DEVO_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_DV_OP);
		if(!pCurPolicy)
		{
			WriteLogE("[%s] set po in dv fail : not find cur policy", m_strLogicName.c_str());
			break;
		}

		t_ManageDevOInfo->SetPkt(stSendToken);
/*
		if(SetER(t_ASIEPSAPPDLLUtil->SetPoDvOp(SendToken.GetData(), SendToken.GetLength())))
		{
			WriteLogE("[%s] set po in dv fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return -1;
		}
*/
		WriteLogN("[%s] set po in dv success : [%d]", m_strLogicName.c_str(), pCurPolicy->tDPH.nID);
		stSendToken.Clear();

	}while(FALSE);

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::ChkTSProtectMode()
{
	UINT32 nCurMode = t_EnvInfo->GetReg_LocalEnv_TSProtectMode();
	if(m_nChkTSProtectMode == nCurMode)		return 0;

	WriteLogN("[%s] check ts protect mode : [%u][%u]", m_strLogicName.c_str(), m_nChkTSProtectMode, nCurMode);

	if(nCurMode == 2)
	{
		t_EnvInfo->SetReg_LocalEnv_TSProtectMode(0);
//		t_NannyAgtSvc.Stop();
	}
	else
	{
		m_nChkTSProtectMode = nCurMode;
		ResetEPSDrv();
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::ForceResetEPSDrv()
{
	UINT32 nOldAPS = t_EnvInfoOp->m_nApplyPolicyStatus;
	t_EnvInfoOp->m_nApplyPolicyStatus = STATUS_USED_MODE_ON;
	ResetEPSDrv();
	t_EnvInfoOp->m_nApplyPolicyStatus = nOldAPS;
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::ResetEPSDrv()
{
	INT32 nRtn = 0;
	INT32 nStartDrv = 0;

	do 
	{
		if(t_EnvInfoOp->m_nApplyPolicyStatus == STATUS_USED_MODE_OFF)	
		{
			break;
		}

		if(t_EnvInfoOp->IsSysOffMode())
		{
			WriteLogN("[%s] sys off mode enabled..skip reset eps drv", m_strLogicName.c_str());
			break;
		}

		m_tStatusMap.clear();

		if(SetER(SetEPSSvcPolicy()))
		{
			WriteLogE("[%s] set eps svc protect fail..", m_strLogicName.c_str());
		}

		if(SetER(SetEPSHookPolicy()))
		{
			WriteLogE("[%s] set eps hook process fail..", m_strLogicName.c_str());
		}

		if(IsActiveEPS() == STATUS_USED_MODE_OFF)
		{
			WriteLogN("[%s] skip reset eps drv..", m_strLogicName.c_str());
			break;
		}

		if(SetER(SetEPSDrvPolicy()))
		{
			WriteLogE("[%s] set eps drv policy fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			break;
		}
		nStartDrv = 1;
		WriteLogN("[%s] reset eps drv success..", m_strLogicName.c_str());	

	} while (FALSE);

	{
		DB_HOST_STATUS_PO dhsp;
		PDB_HOST_STATUS_PO pdhsp = t_ManageHostStatusPo->FirstItem();
		if(pdhsp)	dhsp = *pdhsp;

		{
			dhsp.nFaODrv	= GetDrvStatus(SS_POLICY_TYPE_FA_OP, nStartDrv);
			dhsp.nFeOpDrv	= GetDrvStatus(SS_POLICY_TYPE_FE_PTN_OP, nStartDrv);
			dhsp.nFeLoDrv	= GetDrvStatus(SS_POLICY_TYPE_FE_PTN_LO, nStartDrv);
			dhsp.nFeGbDrv	= GetDrvStatus(SS_POLICY_TYPE_FE_PTN_BL, nStartDrv);
			dhsp.nFeGwDrv	= GetDrvStatus(SS_POLICY_TYPE_FE_PTN_WL, nStartDrv);

			{
				PDB_PO_FE_PTN_BL pdpfpb = (PDB_PO_FE_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_BL);
				if(pdpfpb && pdpfpb->tDPH.nUsedMode != STATUS_USED_MODE_OFF)	
				{
					dhsp.strFeGbPtn = pdpfpb->tDPFI.strPtnVersion;		
				}

				PDB_PO_FE_PTN_WL pdpfpw = (PDB_PO_FE_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_WL);
				if(pdpfpw && pdpfpw->tDPH.nUsedMode != STATUS_USED_MODE_OFF)	
				{
					dhsp.strFeGwPtn = pdpfpw->tDPFI.strPtnVersion;		
				}
			}

			t_LogicMgrHostStatusPo->SendPkt_HostStatusPo(dhsp);
		}
	}
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::GetDrvStatus(UINT32 nPoType, UINT32 nStartDrv)
{
	TMapID64Itor find = m_tStatusMap.find(nPoType);
	if(find == m_tStatusMap.end())
	{
		return SS_ORG_HOST_STATUS_PO_DRV_TYPE_NOT_USED;
	}

	if(LOUINT64(find->second) == STATUS_USED_MODE_ON)
	{
		if(HIUINT64(find->second) == STATUS_USED_MODE_OFF)
		{
			return SS_ORG_HOST_STATUS_PO_DRV_TYPE_STOP;
		}

		return (nStartDrv ? SS_ORG_HOST_STATUS_PO_DRV_TYPE_START : SS_ORG_HOST_STATUS_PO_DRV_TYPE_STOP);
	}

	return SS_ORG_HOST_STATUS_PO_DRV_TYPE_NOT_USED;
}
//---------------------------------------------------------------------------

INT32		CLogicApplyPolicy::SetUser32Hook(BOOL bInstall)
{
	return 0;
}
