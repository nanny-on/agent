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
#include "LogicMgrAuth.h"

//---------------------------------------------------------------------------

CLogicMgrAuth*		t_LogicMgrAuth = NULL;

//---------------------------------------------------------------------------

CLogicMgrAuth::CLogicMgrAuth()
{
	m_strLogicName = "logic mgr svr";
}
//---------------------------------------------------------------------------

CLogicMgrAuth::~CLogicMgrAuth()
{
}
//---------------------------------------------------------------------------

void		CLogicMgrAuth::AnalyzePkt_Auth(PPKT_DATA pkt_data)
{
	InitBaseMember(pkt_data);
	switch(m_nPktCode)
    {
		case AM_CODE_AUTH_LOGIN:		AnalyzePkt_Auth_Login();		break;
		case AM_CODE_AUTH_LAST:			AnalyzePkt_Auth_Last();			break;
		default:
        {
        	WriteLogE("[%s] not define pkt code : [%d][%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode);
			break;
        }
    }
	InitBaseMember();
    return;
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

void		CLogicMgrAuth::AnalyzePkt_Auth_Login()
{
	UINT32 nEnableTimerID = 0;
	UINT32 nLogoffOption = 0;
	BOOL bValidPkt = TRUE;
    UINT32 nSysInfoOpt = 0;
	
	if(!RecvToken.TokenDel_32(m_nRecvValue))
	{
		bValidPkt = FALSE; 
	}
	else
	{
		switch(m_nRecvValue)
		{
			case HOST_AUTH_RESULT_NO_ERROR:
			{
				if(!RecvToken.TokenDel_32(nLogoffOption))
				{
					bValidPkt = FALSE;
				}
				else
				{
					if((nLogoffOption | SS_HOST_OPTION_TYPE_LOGOFF_NOT_USED) != 0)
					{
						t_LogicMgrUser->SetUserLogout(nLogoffOption);
						t_LogicUser->SendPkt_Logout(nLogoffOption);
					}
					WriteLogN("[%s] login success", m_strLogicName.c_str());
                    RecvToken.TokenDel_32(nSysInfoOpt);
					SendMgrInitData(nSysInfoOpt);
					t_EnvInfoOp->SetMgrSvrAuthStatus(CLIENT_CON_STATUS_CONNECTED);
					bValidPkt = TRUE;
				}
				break;
			}
			case HOST_AUTH_RESULT_FAIL_RETRY_REGISTER:
			{
				WriteLogN("[%s] login fail : retry register on svr", m_strLogicName.c_str());
				nEnableTimerID = TIMER_ID_LOGIN_SVR_AUTH;
				t_EnvInfo->SetReg_HostRegTime(0);
				bValidPkt = TRUE;
				break;
			}
			case HOST_AUTH_RESULT_INVALID_PKT:
			{
				WriteLogN("[%s] login fail : retry connect on svr.. becault invalid pkt", m_strLogicName.c_str());
				nEnableTimerID = TIMER_ID_LOGIN_SVR_AUTH;
				bValidPkt = TRUE;
				break;
			}
			default:
			{
				WriteLogE("[%s] fail login on mgr svr : [%d]", m_strLogicName.c_str(), m_nRecvValue);
				bValidPkt = TRUE;
				break;
			}
		}	
	}

	if(bValidPkt == TRUE)
	{
		t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_MANAGER_SVR_AUTH_LAST);
		t_ThreadTimer->t_TimerUtil.EnableTimer(nEnableTimerID);
	}
	else
	{
		WriteLogE("[%s] recv invalid pkt : [%d][%d]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
		m_tPktData->sock_evt.nErrorCode = ERR_SOCKET_SVR_TO_CLT_INVALID_PKT;
		t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_MANAGER_SVR_AUTH_LAST);
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
	}
}
//--------------------------------------------------------------------------- 

void		CLogicMgrAuth::AnalyzePkt_Auth_Last()
{
	UINT32 nEnableTimerID = 0;
	BOOL bValidPkt = TRUE;
	INT32 nMgrSvrAuthStatus = CLIENT_CON_STATUS_DISCONNECTED;
	if(!RecvToken.TokenDel_32(m_nRecvValue))
	{
		bValidPkt = FALSE;
	}
	else
	{
		switch(m_nRecvValue)
		{
			case HOST_AUTH_RESULT_NO_ERROR:
			{
				if(!RecvToken.TokenDel_32(m_nRecvValue))
				{
					bValidPkt = FALSE;
				}
				else
				{
					WriteLogN("[%s] login auth last success : [%u]", m_strLogicName.c_str(), m_nRecvValue);
					t_ValidTimeUtil->InitValidTime(m_nRecvValue);
                    t_EnvInfoOp->GetMgrSvrAuthStatus(nMgrSvrAuthStatus);
                    if(nMgrSvrAuthStatus == CLIENT_CON_STATUS_CONNECTED)
                    	SendMgrInitData_Sync();

					t_EnvInfoOp->m_nApplyPolicyStatus = STATUS_USED_MODE_ON;
					t_LogicApplyPolicy->CheckRunEnv();
					t_LogicApplyPolicy->ApplyPolicy();
					{
						if(t_LogicMgrPoHostWebSock)
							t_LogicMgrPoHostWebSock->ApplyPolicy();
						t_LogicMgrPoInVulnOp->PreScanVuln();
					}
					t_LogicMgrHost->SendPkt_SyncEnd();

					bValidPkt = TRUE;
				}
				break;
			}
			case HOST_AUTH_RESULT_SERVER_ERROR:
			{
				WriteLogN("[%s] login fail : retry connect on svr.. becault svr system error", m_strLogicName.c_str());
				nEnableTimerID = TIMER_ID_LOGIN_SVR_AUTH;
				bValidPkt = TRUE;
				break;
			}
			default:
			{
				WriteLogE("[%s] fail login on mgr svr : [%d]", m_strLogicName.c_str(), m_nRecvValue);
				nEnableTimerID = TIMER_ID_LOGIN_SVR_AUTH;
				bValidPkt = TRUE;
				break;
			}
		}	
	}
	if(bValidPkt == TRUE)
	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(nEnableTimerID);
	}
	else
	{
		m_tPktData->sock_evt.nErrorCode = ERR_SOCKET_SVR_TO_CLT_INVALID_PKT;
		WriteLogE("[%s] recv invalid pkt : [%d][%d]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
	}
}
//--------------------------------------------------------------------------- 
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrAuth::SendAuthLogin()
{
	String strCurIP = GetConnectAddr_Mgr();

	WriteLogN("[%s] mgr current net hardware info : [%s]", m_strLogicName.c_str(), strCurIP.c_str());
	m_tMutex.Lock();
	SendToken.Clear();
	SendToken.TokenAdd_String(t_EnvInfo->m_strHostKey);
	SendToken.TokenAdd_String(strCurIP);

	SendData_Mgr(AM_TYPE_AUTH, AM_CODE_AUTH_LOGIN, SendToken);
	m_tMutex.UnLock();
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrAuth::SendMgrInitData(UINT32 nOpt)
{
	{
		t_LogicMgrHost->SendPkt_InIt();
		t_LogicMgrHostSys->SendPkt_Edit();
		if(nOpt == HOST_AUTH_SYSINFO_EMPTY)
		{
			t_LogicMgrHostSys->SendPkt_SysInfo();
			t_LogicMgrHostHw->SendPkt_HostHw(FALSE);
			t_LogicMgrHostSw->SendPkt_HostSw_Cur();
		}
		else
		{
			t_LogicMgrHostHw->SendPkt_HostHw();
			t_LogicMgrHostSw->SendPkt_HostSw();
		}
		t_LogicMgrHostStatusPo->SendPkt_HostStatusPo();
		if(t_LogicMgrHostStatusPoNc)
			t_LogicMgrHostStatusPoNc->SendPkt_HostStatusPoNc();
		t_LogicMgrHostKey->SendPkt_Hash();
	}

	{
		if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NPMS, 0, 0))
		{			
			t_LogicMgrHostPatch->ScanPatchList();
		}

		if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_POLICY_TYPE_IN_VULN_OP, 0))
		{
			m_tMutex.Lock();
			if(!t_ManageHostVulnRst->SetPktSync(SendToken))	 
				SendData_Mgr(G_TYPE_HOST_VULN_RST, G_CODE_COMMON_EDIT, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}
	}

	{
		{
			m_tMutex.Lock();
			SendToken.TokenAdd_String(t_ManageEnvLicense->GetHash());

			SendData_Mgr(G_TYPE_ENV_LICENSE, G_CODE_COMMON_HASH, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}

		{
			m_tMutex.Lock();
			SendToken.TokenAdd_String(t_ManageEnvLog->GetHash());

			SendData_Mgr(G_TYPE_ENV_LOG, G_CODE_COMMON_HASH, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}
		{
			m_tMutex.Lock();
			SendToken.TokenAdd_String(t_ManageEnvLogUnit->GetHash());

			SendData_Mgr(G_TYPE_ENV_LOG_UNIT, G_CODE_COMMON_HASH, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}

		{
			m_tMutex.Lock();
			SendToken.TokenAdd_String(t_ManageEnvNotifyInfo->GetHash());

			SendData_Mgr(G_TYPE_ENV_NOTIFY_INFO, G_CODE_COMMON_HASH, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}

		{
			m_tMutex.UnLock();
			SendToken.TokenAdd_String(t_ManageEnvSelfProtectAgt->GetHash());

			SendData_Mgr(G_TYPE_ENV_SELF_PROTECT_AGT, G_CODE_COMMON_HASH, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}

		{
			m_tMutex.UnLock();
			SendToken.TokenAdd_String(t_ManageEnvTrustSubject->GetHash());

			SendData_Mgr(G_TYPE_ENV_TRUST_SUBJECT, G_CODE_COMMON_HASH, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}

		{
			m_tMutex.Lock();
			SendToken.TokenAdd_String(t_ManageEnvSOrgLink->GetHash());

			SendData_Mgr(G_TYPE_ENV_SORG_LINK, G_CODE_COMMON_HASH, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}

		{
			m_tMutex.Lock();
			SendToken.TokenAdd_String(t_ManageEnvSocket->GetHash());

			SendData_Mgr(G_TYPE_ENV_SOCKET, G_CODE_COMMON_HASH, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}

		{
			{
				m_tMutex.Lock();
				SendToken.TokenAdd_String(t_ManageSiteVuln->GetHash());
				SendData_Mgr(G_TYPE_SITE_VULN, G_CODE_COMMON_HASH, SendToken);
				SendToken.Clear();
				m_tMutex.UnLock();
			}
			{
				m_tMutex.Lock();
				SendToken.TokenAdd_String(t_ManageSiteVulnScan->GetHash());
				SendData_Mgr(G_TYPE_SITE_VULN_SCAN, G_CODE_COMMON_HASH, SendToken);
				SendToken.Clear();
				m_tMutex.UnLock();
			}
		}

		{
			m_tMutex.Lock();
			SendToken.TokenAdd_String(t_ManagePoInDevOInfo->GetHash());

			SendData_Mgr(G_TYPE_PO_DV_INFO, G_CODE_COMMON_HASH, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}
	}

	//-----------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------
	SendMgrInitData_Polcy();
	
	// send last echo pkt
	m_tMutex.Lock();
	SendToken.TokenAdd_32(1);
	SendData_Mgr(AM_TYPE_AUTH, AM_CODE_AUTH_LAST, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrAuth::SendMgrInitData_Polcy()
{
	
	//-----------------------------------------------------------------------------------

	{
		m_tMutex.Lock();
		SendToken.Clear();
		t_ManageUser->SetPktMgr(SendToken);
		SendData_Mgr(G_TYPE_USER, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_HOST_RM_INFO pCurPolicy = (PDB_PO_HOST_RM_INFO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_RM_INFO);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_HOST_RM_INFO, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_HOST_RUN pCurPolicy = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_RUN);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_HOST_RUN, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		DB_PO_SVR_INFO_LGN tCurPolicy;
		PDB_PO_SVR_INFO_LGN pCurPolicy = (PDB_PO_SVR_INFO_LGN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_LGN);
		if(pCurPolicy)	tCurPolicy = *pCurPolicy;

		m_tMutex.Lock();
		if(tCurPolicy.tDPH.nUsedMode != STATUS_USED_MODE_ON || 
			tCurPolicy.strSvrInfoList.empty())	
		{
			SendToken.TokenAdd_String(t_EnvInfo->m_strLgnSvrInfoList);
		}
		else
		{
			SendToken.TokenAdd_String(tCurPolicy.tDPH.strHash);
		}
		SendToken.TokenAdd_32(tCurPolicy.tDPH.nID);
		SendToken.TokenAdd_32(tCurPolicy.tDPH.nSeqNo);

		SendData_Mgr(G_TYPE_PO_SVR_INFO_LGN, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		DB_PO_SVR_INFO_UDT tCurPolicy;
		PDB_PO_SVR_INFO_UDT pCurPolicy = (PDB_PO_SVR_INFO_UDT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_UDT);
		if(pCurPolicy)		tCurPolicy = *pCurPolicy;
		m_tMutex.Lock();
		SendToken.TokenAdd_String(tCurPolicy.tDPH.strHash);
		SendToken.TokenAdd_32(tCurPolicy.tDPH.nID);
		SendToken.TokenAdd_32(tCurPolicy.tDPH.nSeqNo);

		SendData_Mgr(G_TYPE_PO_SVR_INFO_UDT, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	{
		PDB_PO_CTL_PROC pCurPolicy = (PDB_PO_CTL_PROC)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_CTL_PROC);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_CTL_PROC, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_SELF_PROTECT pCurPolicy = (PDB_PO_SELF_PROTECT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SELF_PROTECT);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_SELF_PROTECT, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	{
		PDB_PO_POWER pCurPolicy = (PDB_PO_POWER)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_POWER);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_POWER, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_WIN_RUN pCurPolicy = (PDB_PO_WIN_RUN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_WIN_RUN);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_WIN_RUN, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_DEPLOY_FILE pCurPolicy = (PDB_PO_DEPLOY_FILE)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_DEPLOY_FILE);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_DEPLOY_FILE, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_CTL_PANEL pCurPolicy = (PDB_PO_CTL_PANEL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_CTL_PANEL);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_CTL_PANEL, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	{
		PDB_PO_HOST_NOTIFY pCurPolicy = (PDB_PO_HOST_NOTIFY)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_NOTIFY);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_HOST_NOTIFY, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	//-----------------------------------------------------------------------------------

	{
		PDB_PO_HOST_WEB_SOCK pCurPolicy = (PDB_PO_HOST_WEB_SOCK)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_WEB_SOCK);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_HOST_WEB_SOCK, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	{
		PDB_PO_FA_OP pCurPolicy = (PDB_PO_FA_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_OP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FA_OP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_FA_CLEAR pCurPolicy = (PDB_PO_FA_CLEAR)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_CLEAR);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FA_CLEAR, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_FA_ENV pCurPolicy = (PDB_PO_FA_ENV)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_ENV);

		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FA_ENV, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_FA_PROC pCurPolicy = (PDB_PO_FA_PROC)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_PROC);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FA_PROC, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_FA_BK pCurPolicy = (PDB_PO_FA_BK)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_BK);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FA_BK, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_FA_NOTIFY pCurPolicy = (PDB_PO_FA_NOTIFY)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_NOTIFY);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FA_NOTIFY, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	{
		PDB_PO_FE_PTN_OP pCurPolicy = (PDB_PO_FE_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_OP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FE_PTN_OP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	
	{
		PDB_PO_FE_PTN_LO pCurPolicy = (PDB_PO_FE_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_LO);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FE_PTN_LO, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_FE_PTN_BL pCurPolicy = (PDB_PO_FE_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_BL);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FE_PTN_BL, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_FE_PTN_WL pCurPolicy = (PDB_PO_FE_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_WL);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FE_PTN_WL, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_FE_EXCLUDE pCurPolicy = (PDB_PO_FE_EXCLUDE)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_EXCLUDE);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FE_EXCLUDE, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_FE_NOTIFY pCurPolicy = (PDB_PO_FE_NOTIFY)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_NOTIFY);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FE_NOTIFY, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_FE_SINGLE_PTN pCurPolicy = (PDB_PO_FE_SINGLE_PTN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_SINGLE_PTN);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_FE_SINGLE_PTN, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	{
		PDB_PO_IN_PTN_OP pCurPolicy = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_PTN_OP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_IN_PTN_BL pCurPolicy = (PDB_PO_IN_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_BL);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_PTN_BL, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_IN_PTN_WL pCurPolicy = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_WL);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_PTN_WL, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_IN_PTN_EX pCurPolicy = (PDB_PO_IN_PTN_EX)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_EX);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_PTN_EX, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_IN_PTN_SP pCurPolicy = (PDB_PO_IN_PTN_SP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_SP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_PTN_SP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		String strHash = t_ManagePoInPtnSPRule->GetHash();	
		m_tMutex.Lock();
		SendToken.TokenAdd_String(strHash);

		SendData_Mgr(G_TYPE_PO_IN_PTN_SP_RULE, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_IN_PTN_NO pCurPolicy = (PDB_PO_IN_PTN_NO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_NO);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_PTN_NO, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
// 	{
// 		PDB_PO_IN_AC_DOC pCurPolicy = (PDB_PO_IN_AC_DOC)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_AC_DOC);
// 		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
// 		else			SendToken.TokenAdd_String("empty");
// 
// 		SendData_Mgr(G_TYPE_PO_IN_AC_DOC, G_CODE_COMMON_HASH, SendToken);
// 		SendToken.Clear();
// 	}
// 
// 	{
// 		PDB_PO_IN_AC_SF pCurPolicy = (PDB_PO_IN_AC_SF)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_AC_SF);
// 		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
// 		else			SendToken.TokenAdd_String("empty");
// 
// 		SendData_Mgr(G_TYPE_PO_IN_AC_SF, G_CODE_COMMON_HASH, SendToken);
// 		SendToken.Clear();
// 	}
// 
// 	{
// 		PDB_PO_IN_AC_FILE pCurPolicy = (PDB_PO_IN_AC_FILE)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_AC_FILE);
// 		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
// 		else			SendToken.TokenAdd_String("empty");
// 
// 		SendData_Mgr(G_TYPE_PO_IN_AC_FILE, G_CODE_COMMON_HASH, SendToken);
// 		SendToken.Clear();
// 	}

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	{
		PDB_PO_IN_VULN_AX pCurPolicy = (PDB_PO_IN_VULN_AX)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_AX);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_VULN_AX, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_VULN_EDIT_APP pCurPolicy = (PDB_PO_IN_VULN_EDIT_APP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_EDIT_APP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_VULN_EDIT_APP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_VULN_OP pCurPolicy = (PDB_PO_IN_VULN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_OP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_VULN_OP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_VULN_QNA pCurPolicy = (PDB_PO_IN_VULN_QNA)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_QNA);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_VULN_QNA, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_VULN_SCAN pCurPolicy = (PDB_PO_IN_VULN_SCAN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_SCAN);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_VULN_SCAN, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_VULN_SECU_USB pCurPolicy = (PDB_PO_IN_VULN_SECU_USB)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_SECU_USB);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_VULN_SECU_USB, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_VULN_SW pCurPolicy = (PDB_PO_IN_VULN_SW)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_SW);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_VULN_SW, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_VULN_PATCH_EXCEPTION pCurPolicy = (PDB_PO_IN_VULN_PATCH_EXCEPTION)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_PATCH_EXCEPTION);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_VULN_PATCH_EXCEPTION, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	{
		PDB_PO_IN_RS_OP pCurPolicy = (PDB_PO_IN_RS_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_RS_OP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_RS_OP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_RS_BK pCurPolicy = (PDB_PO_IN_RS_BK)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_RS_BK);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_RS_BK, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_RS_NO pCurPolicy = (PDB_PO_IN_RS_NO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_RS_NO);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_IN_RS_NO, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	{
		PDB_PO_IN_DEVO_OP pCurPolicy = (PDB_PO_IN_DEVO_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_DV_OP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_DV_OP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_DEVO_BL pCurPolicy = (PDB_PO_IN_DEVO_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_DV_BL);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_DV_BL, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_DEVO_WL pCurPolicy = (PDB_PO_IN_DEVO_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_DV_WL);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_DV_WL, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_DEVO_EX pCurPolicy = (PDB_PO_IN_DEVO_EX)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_DV_EX);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_DV_EX, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_IN_DEVO_NOTIFY pCurPolicy = (PDB_PO_IN_DEVO_NOTIFY)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_DV_NOTIFY);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_DV_NOTIFY, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------

	{
		PDB_PO_PM_OP pCurPolicy = (PDB_PO_PM_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_PM_OP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_PM_OP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_PM_DM pCurPolicy = (PDB_PO_PM_DM)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_PM_DM);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_PM_DM, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_PM_SCAN pCurPolicy = (PDB_PO_PM_SCAN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_PM_SCAN);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_PM_SCAN, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_PM_EX pCurPolicy = (PDB_PO_PM_EX)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_PM_EX);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_PM_EX, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	{
		PDB_PO_PM_NO pCurPolicy = (PDB_PO_PM_NO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_PM_NO);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_PM_NO, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	{
		PDB_PO_NC_PTN_OP pCurPolicy = (PDB_PO_NC_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_OP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_NC_PTN_OP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_NC_PTN_LO pCurPolicy = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_LO);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_NC_PTN_LO, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_NC_PTN_BL pCurPolicy = (PDB_PO_NC_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_BL);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_NC_PTN_BL, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_NC_PTN_WL pCurPolicy = (PDB_PO_NC_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_WL);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_NC_PTN_WL, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_NC_PTN_EX pCurPolicy = (PDB_PO_NC_PTN_EX)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_EX);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_NC_PTN_EX, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_NC_PTN_SP pCurPolicy = (PDB_PO_NC_PTN_SP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_SP);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_NC_PTN_SP, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		String strHash = t_ManagePoNcPtnSPRule->GetHash();	
		m_tMutex.Lock();
		SendToken.TokenAdd_String(strHash);

		SendData_Mgr(G_TYPE_PO_NC_PTN_SP_RULE, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_NC_PTN_NO pCurPolicy = (PDB_PO_NC_PTN_NO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_NO);
		m_tMutex.Lock();
		if(pCurPolicy)	SendToken.TokenAdd_String(pCurPolicy->tDPH.strHash);
		else			SendToken.TokenAdd_String("empty");

		SendData_Mgr(G_TYPE_PO_NC_PTN_NO, G_CODE_COMMON_HASH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
}



void		CLogicMgrAuth::SendMgrInitData_Sync()
{
//	SendToken.Clear();
	//-----------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	
// 	{
// 		if(!t_ManageLogSecu->SetPkt(SendToken))			SendData_Mgr(G_TYPE_LOG_SECU, G_CODE_COMMON_SYNC, SendToken);
// 		SendToken.Clear();
// 	}
// 
// 	{
// 		if(!t_ManageLogDoc->SetPkt(SendToken))			SendData_Mgr(G_TYPE_LOG_DOC, G_CODE_COMMON_SYNC, SendToken);
// 		SendToken.Clear();
// 	}
// 
// 	{
// 		if(!t_ManageLogDeployFile->SetPkt(SendToken))	SendData_Mgr(G_TYPE_LOG_DEPLOY_FILE, G_CODE_COMMON_SYNC, SendToken);
// 		SendToken.Clear();
// 	}
// 
// 	{
// 		if(!t_ManageLogEvent->SetPktSync(SendToken))	SendData_Mgr(G_TYPE_LOG_EVENT_HOST, G_CODE_COMMON_SYNC, SendToken);
// 		SendToken.Clear();
// 	}
// 
// 	{
// 		if(!t_ManageLogDevice->SetPkt(SendToken))	SendData_Mgr(G_TYPE_LOG_DEVICE, G_CODE_COMMON_SYNC, SendToken);
// 		SendToken.Clear();
// 	}

	INT32 nOncePktNum = 1000;
	PDB_PO_HOST_RUN pCurPolicy = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_RUN);
	if(pCurPolicy)
	{
		nOncePktNum = pCurPolicy->nLnxOncePktNum;
	}
	WriteLogN("[%s] sync data once pkt num : [%d]", m_strLogicName.c_str(), nOncePktNum);

	{
		t_LogicMgrLogSecu->SendPkt_Sync(nOncePktNum);
//		SendToken.Clear();
	}
	{
		t_LogicMgrLogDoc->SendPkt_Sync(nOncePktNum);
//		SendToken.Clear();
	}
	{
		t_LogicMgrLogDeployFile->SendPkt_Sync(nOncePktNum);
		//SendToken.Clear();
	}
	{
		t_LogicMgrLogEvent->SendPkt_Sync(nOncePktNum);
		//SendToken.Clear();
	}
	{
		t_LogicMgrLogDevice->SendPkt_Sync(nOncePktNum);
		//SendToken.Clear();
	}

	{
		t_LogicMgrSiteFile->SendPkt_Sync(nOncePktNum);
		//SendToken.Clear();

		t_LogicMgrSiteFileAlias->SendPkt_Sync(nOncePktNum);
		//SendToken.Clear();
	}

	{
		t_LogicMgrPoInPtnSPRule->SendPkt_Sync(nOncePktNum);
		//SendToken.Clear();
	}

	{
		if(t_LogicMgrPoNcPtnSPRule)
		{
			t_LogicMgrPoNcPtnSPRule->SendPkt_Sync(nOncePktNum);
			//SendToken.Clear();
		}
	}
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
