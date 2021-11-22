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

// MainRun.cpp: implementation of the CMainRun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "MainDlg.h"

//--------------------------------------------------------------------

DWORD		CMainDlg::OnThreadTimer(WPARAM wParam, LPARAM lParam)
{	
	switch(wParam)
	{
		case TIMER_ID_LOGIN_SVR_AUTH:
		{
			SOCK_INIT_DATA tSID = t_EnvInfoOp->GetCltLgnSID();

			WriteLogN("select lgn svr : [%s][%d]", tSID.szOpenAddress, tSID.nOpenPort);
			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE);
			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE);
			WriteLogN("start lgn client");
			if(t_LgnCltUtil->StartClient_CLD(&tSID, NULL, "lgn_clt"))
			{
				WriteLogE("lgn svr connect fail : [%s][%d]", tSID.szOpenAddress, tSID.nOpenPort);
				t_EnvInfoOp->NextCltSvrIdx(SS_ENV_SVR_INFO_SVR_TYPE_LGN);
				break;
			}

			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_ENABLE);
			
			if(t_EnvInfo->m_nHostRegTime)
			{
				t_LogicLgnAuth->SendAuthLogin();
			}
			else
			{
				t_LogicLgnAuth->SendAuthReg();
			}

			t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_LOGIN_SVR_AUTH);
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH_LAST);
			break;
		}
		case TIMER_ID_LOGIN_SVR_AUTH_LAST:
		{			
			t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_LOGIN_SVR_AUTH_LAST);

			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE);
			WriteLogE("lgn svr disconnect because timer over");

			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
			break;
		}
		case TIMER_ID_MANAGER_SVR_AUTH:
		{
			SOCK_INIT_DATA tSID = t_EnvInfoOp->GetCltMgrSID();

			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE);
			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE);			

			WriteLogN("start mgr client");
			if(t_MgrCltUtil->StartClient_CLD(&tSID, NULL, "mgr_clt"))
			{
				WriteLogE("mgr svr connect fail : [%s][%d]", tSID.szOpenAddress, tSID.nOpenPort);
				t_EnvInfoOp->NextCltSvrIdx(SS_ENV_SVR_INFO_SVR_TYPE_MGR);
				t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_MANAGER_SVR_AUTH);
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
				break;
			}

			t_MgrCltUtil->ClearAllPkt_CLD();

			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_ENABLE);
			t_LogicMgrAuth->SendAuthLogin();

			t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_MANAGER_SVR_AUTH);
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_MANAGER_SVR_AUTH_LAST);
			break;
		}
		case TIMER_ID_MANAGER_SVR_AUTH_LAST:
		{			
			t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_MANAGER_SVR_AUTH_LAST);

			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE);		
			WriteLogE("mgr svr disconnect because timer over");

			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
			break;
		}		
		case TIMER_ID_UPDATE_SVR_AUTH:
		{
			SOCK_INIT_DATA tSID = t_EnvInfoOp->GetCltUdtSID();

			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE);
			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_UDT, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE);
			WriteLogN("start udt client");
			if(t_UdtCltUtil->StartClient_CLD(&tSID, NULL, "udt_clt"))
			{
				WriteLogE("udt svr connect fail : [%s][%d]", tSID.szOpenAddress, tSID.nOpenPort);
				t_EnvInfoOp->NextCltSvrIdx(SS_ENV_SVR_INFO_SVR_TYPE_UDT_CHK);
				t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_UPDATE_SVR_AUTH);
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
				break;
			}

			t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_UDT, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_ENABLE);
			t_LogicUdtAuth->SendAuthLogin();

			t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_UPDATE_SVR_AUTH);
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_UPDATE_SVR_AUTH_LAST);
			break;
		}
		case TIMER_ID_UPDATE_SVR_AUTH_LAST:
		{			
			if (t_ThreadTimer)
				t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_UPDATE_SVR_AUTH_LAST);

			if (t_ThreadSocket)
				t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_UDT, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE);
			WriteLogE("udt svr disconnect because timer over");

			if (t_ThreadTimer)
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
			break;
		}
		case TIMER_ID_DOWNLOAD_UPDATE_FILE:
		{
			if (t_ThreadTimer)	t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_DOWNLOAD_UPDATE_FILE);

			if (t_EnvInfoOp)
			{
				ASI_FDL_INFO tAFI = t_EnvInfoOp->GetAgtUpFileInfo();
				String strSvrInfo = t_EnvInfoOp->GetUdtDataSvrInfo();

				if(tAFI.szFileName[0] == 0 || tAFI.szFileHash[0] == 0)
				{
					if (t_ThreadTimer)	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
					break;
				}
				
				WriteLogN("start up file download : [%s]:[%s]", tAFI.szFileName, strSvrInfo.c_str());
/*
				if (t_ASIFDLDLLUtil)	
				{
					t_ASIFDLDLLUtil->SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, strSvrInfo.c_str());
					t_ASIFDLDLLUtil->AddDLInfo(&tAFI);
				}
*/
				SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, strSvrInfo.c_str());
				AddDLInfo(&tAFI);

				if (t_ManageFileDown)
					t_ManageFileDown->AddItem(tAFI);
			}
			
			break;
		}
		case TIMER_ID_SYS_BOOT_OPERATION_END:
		{
			UINT32 nBootTime = uptime();
//			if(GetCurrentDateTimeInt() - t_EnvInfo->m_nBootChkTime > TIMER_INTERVAL_TIME_SYS_BOOT)
			if(nBootTime > TIMER_INTERVAL_TIME_SYS_BOOT)
			{
				if (t_EnvInfoOp)
					t_EnvInfoOp->SetSysBootChkMode();
				WriteLogN("set system boot time disable..");
				if (t_ThreadTimer)
					t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_SYS_BOOT_OPERATION_END);
			}			
			break;
		}
		case TIMER_ID_SYS_OFF_OPERATION:
		{
			
			if (t_LogicMgrPoFaOp) t_LogicMgrPoFaOp->OnTimer_Logic();			
			if(t_LogicMgrPoFaClear && t_LogicMgrPoFaClear->IsExistSysOffSch())
			{
				if (t_LogicMgrPoFaClear) t_LogicMgrPoFaClear->OnTimer_Logic();
			}
			
			if (t_ThreadTimer)	t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_SYS_OFF_OPERATION);
			if (t_ThreadTimer)	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_SYS_OFF_OPERATION_END);
			break;
		}
		case TIMER_ID_SYS_OFF_OPERATION_END:
		{
			if (!t_ThreadChkHkNoti || !t_EnvInfoOp)
				return -1;

			if(t_ThreadPoFaClear && !t_ThreadPoFaClear->IsEndWork()
				&& t_EnvInfoOp->GetInjectFlag() & INJECT_FLAG_BOOT && !(t_EnvInfoOp->GetInjectFlag() & INJECT_FLAG_SHUTDOWN)
				&& t_EnvInfoOp->GetStopOpBySysOff())
			{
				t_ASIFFDLLUtil->ASIFF_StopFindFileWorkAll();
				WriteLogN("wait sys log off operation end : stop findfile work all by set up only boot schedule [flag:%x]", t_EnvInfoOp->GetInjectFlag());
			}

			if(t_ThreadPoFaClear && t_ThreadPoFaClear->IsEndWork())
			{
				if (!t_ThreadChkHkNoti || !t_EnvInfoOp)
					return -1;

				t_ThreadChkHkNoti->InitMemFile_ExitWin(2);
				
				UINT32 nFlag = t_EnvInfoOp->m_nSysOffFlag;
				if(nFlag & EWX_POWEROFF)
				{
					nFlag = EWX_POWEROFF;
				}
				else if(nFlag & EWX_REBOOT)
				{
					nFlag = EWX_REBOOT;
				}
				
				CPowerUtil tPowerUtil;
				if(tPowerUtil.SetSystemPower(nFlag, t_EnvInfoOp->m_nSysOffReason) == FALSE)
				{
					WriteLogE("set system power fail : [%d]", GetLastError());
				}
				{
					t_EnvInfoOp->SetStopOpBySysOff(0);
					t_EnvInfoOp->SetSysOffMode(0);
				}
				if (t_ThreadTimer)	t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_SYS_OFF_OPERATION_END);				
				WriteLogN("sys log off operation end : [%d]:[%x->%x][%x]:[%d]", t_ThreadPoFaClear->IsEndWork(), t_EnvInfoOp->m_nSysOffFlag, nFlag, t_EnvInfoOp->m_nSysOffReason, GetLastError());
			}			
			break;
		}
/*
		case TIMER_ID_SYS_OFF_OPERATION_CHG_SESSION:
		{
			if(t_ManageWinSession.IsWinSessionChgHK())
			{
				//current used winlogon.exe, neccessary time used explorer.exe
				//t_LogicApplyPolicy->SetEPSHookPolicy();
			}
			break;
		}
*/
		case TIMER_ID_CHECK_CHANGE_NET_DRIVE:
		{
			if(t_ManageSysDrive.IsChangeNetDrive())
			{
				WriteLogN("detect network drive changed.");
				if(t_ManagePoHostRun && t_ManagePoHostRun->IsApplyPolicyFromBootTime())
				{
					if (t_LogicApplyPolicy)		t_LogicApplyPolicy->ForceResetEPSDrv();
				}
				else
				{
					if (t_ThreadTimer)	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
				}
			}
			break;
		}
		case TIMER_ID_CHECK_CHANGE_FIX_DRIVE:
		{
			if(t_ManageSysDrive.IsChangeFixDrive())
			{
				WriteLogN("detect fix drive changed.");
				if(t_ManagePoHostRun && t_ManagePoHostRun->IsApplyPolicyFromBootTime())
				{
					if (t_LogicApplyPolicy)		t_LogicApplyPolicy->ForceResetEPSDrv();
				}
				else
				{
					if (t_ThreadTimer)		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
				}
			}
			break;
		}
		case TIMER_ID_CHECK_CHANGE_LOGIN_USER:
		{
			if (t_LogicMgrEnvSOrgLink)	t_LogicMgrEnvSOrgLink->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_APPLY_FA_CLEAR:
		{
			if (t_LogicMgrPoFaClear)	t_LogicMgrPoFaClear->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_APPLY_FA_OP_CLEAR:
		{
			if (t_LogicMgrPoFaOp)	t_LogicMgrPoFaOp->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_APPLY_POWER:
		{
			if (t_LogicMgrPoPower)	t_LogicMgrPoPower->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_APPLY_CHECK:
		{
			UINT32 nChkTime = GetCurrentDateTimeInt();
			if(!t_ManageLogicTimer)	break;

			TListID tIDList;
			t_ManageLogicTimer->GetItemIDList(tIDList);
			TListIDItor begin, end;
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				PMEM_LOGIC_TIMER pMLT = t_ManageLogicTimer->FindItem(*begin);
				if(!pMLT)	continue;
			
				if((nChkTime - pMLT->nLastTime) < pMLT->nPeriodSec)		continue;
				
				((CLogicBase*)pMLT->tLogicBase)->OnTimer();
				pMLT->nLastTime = nChkTime;
			}
			break;
		}
		case TIMER_ID_POLICY_APPLY_EPS:
		{
			if (t_LogicApplyPolicy)	t_LogicApplyPolicy->ResetEPSDrv();
			if (t_ThreadTimer)	t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_POLICY_APPLY_EPS);
			WriteLogN("apply eps policy end.. by timer");
			break;
		}
		case TIMER_ID_POLICY_HOST_NOTIFY:
		{
			if(t_LogicMgrPoHostNotify)	t_LogicMgrPoHostNotify->OnTimer_Logic();
			break;
		}
		case TIMER_ID_POLICY_APPLY_IN_PTN_OP:
		{
			if (t_LogicMgrPoInPtnOp)	t_LogicMgrPoInPtnOp->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_APPLY_IN_PTN_SP_RULE:
		{
			if (t_LogicMgrPoInPtnSPRule)	t_LogicMgrPoInPtnSPRule->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_APPLY_IN_RS_BK:
		{
			if (t_LogicMgrPoInRsBk)		t_LogicMgrPoInRsBk->OnTimer();
			break;
		}
		//---------------------------------------------------------------------------------------
		case TIMER_ID_POLICY_APPLY_NC_PTN_OP:
		{
			if (t_LogicMgrPoNcPtnOp)	t_LogicMgrPoNcPtnOp->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_APPLY_NC_PTN_SP_RULE:
		{
			if (t_LogicMgrPoNcPtnSPRule)	t_LogicMgrPoNcPtnSPRule->OnTimer();
			break;
		}
		//---------------------------------------------------------------------------------------

		case TIMER_ID_TS_PROTECT_MODE:
		{
			if (t_LogicApplyPolicy)	t_LogicApplyPolicy->ChkTSProtectMode();
			break;
		}
		case TIMER_ID_WINDOWS_EVENT_HANDLER:
		{
			if (t_LogicPWEvtMon)	t_LogicPWEvtMon->OnTimer();
			break;
		}
		case TIMER_ID_DP_FILE_EXCUTE_END:
		{
			INT32 nRtn = 0;
			if(t_LogicMgrPoDeployFile)	nRtn = t_LogicMgrPoDeployFile->ChkEndDpFile();

			if(nRtn)
				t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_DP_FILE_EXCUTE_END);

			break;
		}
		case TIMER_ID_POLICY_APPLY_PM_SCAN:
		{
			if(!t_LogicMgrPtnPatch)						break;			
			if(t_LogicMgrPtnPatch->IsInitLogic() != 1)	break;
// 			if(t_LogicMgrPtnPatch->GetPatchUtilStatus() != SS_PTN_PATCH_UTIL_STATUS_CONFIRM)
// 			{
// 				t_LogicMgrPtnPatch->ChkPtnPatchUtil();
// 				break;
// 			}
			if(t_LogicMgrPoPmScan->IsUsedOffLineScan())
			{
				if(t_LogicMgrPtnPatch->GetPatchWsusStatus() != SS_PTN_PATCH_WSUS_STATUS_CONFIRM)
				{
					t_LogicMgrPtnPatch->ChkPtnPatchWsus();
					break;
				}
			}

			if(t_LogicMgrPoPmScan)		t_LogicMgrPoPmScan->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_APPLY_PM_ROLLBACK:
		{
			if(!t_LogicMgrPtnPatch)						break;			
			if(t_LogicMgrPtnPatch->IsInitLogic() != 1)	break;
			
			if(t_LogicMgrPoPmEx)		t_LogicMgrPoPmEx->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_APPLY_PM_MON_RM_PATCH:
		{
			if(!t_LogicMgrPtnPatch)						break;			
			if(t_LogicMgrPtnPatch->IsInitLogic() != 1)	break;

			if(t_LogicMgrPoPmOp)		t_LogicMgrPoPmOp->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_APPLY_IN_VULN_SCAN:
		{
			if(!t_LogicMgrPtnVuln)						break;			
			if(t_LogicMgrPtnVuln->IsInitLogic() != 1)	break;

			INT32 nVulnOnLineScan = t_LogicMgrPtnVuln->IsVulnWsusLive();
			if(!nVulnOnLineScan)
			{
				if(t_LogicMgrPtnVuln->GetVulnWsusStatus() != SS_PTN_PATCH_WSUS_STATUS_CONFIRM)
				{
					t_LogicMgrPtnVuln->ChkPtnVulnWsus();
					break;
				}
			}

			if (t_LogicMgrPoInVulnScan)		t_LogicMgrPoInVulnScan->OnTimer();
			break;
		}
		case TIMER_ID_POLICY_READY_IN_DEPLOY:
		{
			if(!t_LogicMgrPtnVuln)						break;			
			if(t_LogicMgrPtnVuln->IsInitLogic() != 1)	break;
			if(t_LogicMgrPtnVuln->GetPtnDeployStatus() != SS_PTN_DEPLOY_STATUS_CONFIRM)
			{
				t_LogicMgrPtnVuln->ChkPtnDeploy();
				break;
			}
			else
			{
				t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_POLICY_READY_IN_DEPLOY);
				WriteLogN("check valid ptn deploy file end...");
			}
			break;
		}
		case TIMER_ID_INFO_PROC_REPUTATION:
		{
			if(t_LogicMgrInfoProcReputation)
				t_LogicMgrInfoProcReputation->OnTimer();
			break;
		}
		case TIMER_ID_SEND_KEEP_ALIVE:
		{
			if(t_LogicMgrEnvSocket)		t_LogicMgrEnvSocket->OnTimer_Logic();
			break;
		}
		case TIMER_ID_CONNECT_KEEP_MANAGE:
		{
			if(t_LogicMgrPoHostRun)		t_LogicMgrPoHostRun->OnTimer_Logic_ConnManage();
			break;
		}
		default:
		{
			WriteLogE("not define timer id : [0x%.8X][%u]", wParam, wParam);
			break;
		}
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}
//--------------------------------------------------------------------

INT32 CMainDlg::OnKernelFileLog(UINT32 nLogType, PPKT_DATA pPktData)
{
	INT32 nRetVal = 0;
	if(pPktData == NULL)
	{
		WriteLogE("invalid input data at onkernelfilelog");
		return -1;
	}
	
	switch(nLogType)
	{
		case ASI_EPS_APP_LOG_TYPE_DOC:
			t_LogicMgrLogDoc->AnalyzePkt_FromMgr(pPktData);
			break;
		case ASI_EPS_APP_LOG_TYPE_SECU:
			t_LogicMgrLogSecu->AnalyzePkt_FromMgr(pPktData);
			break;
		case ASI_EPS_APP_LOG_TYPE_SITE_FILE:
			t_LogicMgrSiteFile->AnalyzePkt_FromMgr(pPktData);
			break;
		case ASI_EPS_APP_LOG_TYPE_PO_IN_PTN_EX_TRUST:
			t_LogicMgrPoInPtnExTrust->AnalyzePkt_FromMgr(pPktData);
			break;
		case ASI_EPS_APP_LOG_TYPE_DEVICE:
        	t_LogicMgrLogDevice->AnalyzePkt_FromMgr(pPktData);
            break;
		case ASI_EPS_APP_LOG_TYPE_RS:
        	t_LogicMgrLogRs->AnalyzePkt_FromMgr(pPktData);
            break;
		case ASI_EPS_APP_LOG_TYPE_RS_BK:
        	t_LogicMgrLogRsBk->AnalyzePkt_FromMgr(pPktData);
            break;
		case ASI_EPS_APP_LOG_TYPE_PO_NC_PTN_EX_TRUST:
//			t_LogicMgrPoNcPtnExTrust->AnalyzePkt_FromMgr(pPktData);
			break;
		case ASI_EPS_APP_LOG_TYPE_SITE_FILE_ALIAS:
			t_LogicMgrSiteFileAlias->AnalyzePkt_FromMgr(pPktData);
			break;
		default:
			nRetVal = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
	}
	return nRetVal;
}

INT32 CMainDlg::OnNotifyFileLog(UINT32 nLogType, PNOTIFY_PATH pNotifyPath)
{
	INT32 nRetVal = 0;
	INT32 nFileFormat = 0;

	if(pNotifyPath == NULL)
	{
		WriteLogE("invalid input data at on notify file log");
		return -1;
	}

	if(nLogType == FILE_NOTIFY_TYPE_CREATE)
	{
		t_LogicMgrPoFaInotifyFile->InsertInotifyPath(pNotifyPath);
		nRetVal = 0;
	}
	else if(nLogType == FILE_NOTIFY_TYPE_DELETE)
	{
		t_LogicMgrPoFaInotifyFile->DeleteInotifyPath(pNotifyPath);
		nRetVal = 0;
	}
	else
	{
		WriteLogE("invalid log type at on notify file log (%d)", nLogType);
		nRetVal = -2;
	}
	return nRetVal;
}

//--------------------------------------------------------------------

DWORD CMainDlg::OnThreadLog(UINT32 nLogType, PVOID pFileInfo)
{
	if(pFileInfo == NULL)
	{
		WriteLogE("invalid input data at onthreadlog");
		return 1;
	}
	t_ManageEvtMon.AddEvtMon(3, nLogType, __FUNCTION__);
	switch(nLogType)
	{
		case ASI_EPS_APP_LOG_TYPE_SITE_FILE:	
			t_LogicMgrSiteFile->CheckSiteFile((PDB_SITE_FILE)pFileInfo);
			break;
		case ASI_EPS_APP_LOG_TYPE_SITE_FILE_ALIAS:
			t_LogicMgrSiteFileAlias->CheckSiteFileAlias((PDB_SITE_FILE_ALIAS)pFileInfo);
			break;
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}

//--------------------------------------------------------------------

DWORD CMainDlg::OnThreadEnd(UINT32 nEndType, PDB_PO_FE_PTN_LO pDPFPO)
{
	if(pDPFPO == NULL)
	{
		WriteLogE("invalid input data at onthreadend");
		return 1;
	}
	t_ManageEvtMon.AddEvtMon(4, nEndType, __FUNCTION__);

	switch(nEndType)
	{
		case G_TYPE_PO_FE_PTN_LO:
			t_LogicMgrPoFePtnLo->MakePtnEnd(pDPFPO);
			break;
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnThreadWorkEnd(UINT32 nEndWorkType, PMEM_FIND_ORDER_INFO pMFOI)
{
	if(pMFOI == NULL)
	{
		WriteLogE("invalid input data at on thread work end");
		return 1;
	}
	t_ManageEvtMon.AddEvtMon(5, nEndWorkType, __FUNCTION__);

	switch(nEndWorkType)
	{
		case G_TYPE_PO_FA_CLEAR_UNIT:	t_LogicMgrPoFaClear->EndFindOrder(pMFOI);		break;
		case G_TYPE_PO_FA_BK:			t_LogicMgrPoFaBk->EndFindOrder(pMFOI);			break;
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnThreadPoFaClearEnd(UINT32 nEndWorkType, PMEM_FIND_ORDER_INFO pMFOI)
{
	if(pMFOI == NULL)
	{
		WriteLogE("invalid input data at on thread fa clear end");
		return 1;
	}
	t_ManageEvtMon.AddEvtMon(5, nEndWorkType, __FUNCTION__);

	switch(nEndWorkType)
	{
		case G_TYPE_PO_FA_CLEAR_UNIT:	
		{
			t_LogicMgrPoFaClear->EndFindOrder(pMFOI);
			t_LogicMgrPoFaBk->EndFindOrder(pMFOI);			
			break;
		}
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}

DWORD CMainDlg::OnThreadPoFaClearProgress(UINT32 nEndWorkType, PMEM_FIND_ORDER_INFO pMFOI)
{
	if(pMFOI == NULL)
	{
		WriteLogE("invalid input data at on thread fa clear process");
		return 1;
	}
	
	t_ManageEvtMon.AddEvtMon(6, nEndWorkType, __FUNCTION__);

	switch(nEndWorkType)
	{
		case G_TYPE_PO_FA_CLEAR_UNIT:
		{
			t_LogicMgrPoFaClear->PgFindOrder(pMFOI);
			break;
		}
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnFileDown(WPARAM wParam, LPARAM lParam)
{
	t_ManageEvtMon.AddEvtMon(7, UINT32(wParam), __FUNCTION__);
	switch(wParam)
	{
		case ASIFDL_DL_RST_STATUS_TYPE_READY:
		case ASIFDL_DL_RST_STATUS_TYPE_START:
		case ASIFDL_DL_RST_STATUS_TYPE_FAIL_END:
		case ASIFDL_DL_RST_STATUS_TYPE_FAIL_HASH_END:
		case ASIFDL_DL_RST_STATUS_TYPE_CANCEL:
		case ASIFDL_DL_RST_STATUS_TYPE_CANCEL_REF_OVER:
		{
			PASI_FDL_INFO pafi = t_ManageFileDown->FindItem((UINT32)lParam);
			if(pafi)
			{
//				WriteLogN("file download status : [%d][%d]:[%d][%s]", pafi->nID, wParam, pafi->nItemType, pafi->szFileName);
				pafi->nFileStatus = wParam;
			}
			else
			{
				WriteLogN("not find file download info : [%d][%d]", lParam, wParam);
			}
			break;
		}
		case ASIFDL_DL_RST_STATUS_TYPE_SUCCESS_END:
		{
			PASI_FDL_INFO pafi = t_ManageFileDown->FindItem((UINT32)lParam);
			if(pafi)
			{
//				WriteLogN("file download status : [%d][%d]:[%d][%s]", pafi->nID, wParam, pafi->nItemType, pafi->szFileName);
			}
			else
			{
				WriteLogE("not find file download info : [%d][%d]", lParam, wParam);
				break;
			}

			if(pafi->nItemType == SS_DN_FILE_TYPE_PPTN_PATCH)
			{
				PDB_LOG_PATCH pdata = t_ManageLogPatch->FindKeyTypeID_Item(POS_KEY_ID_POSITION_LOG_PATCH_PID_MAP, pafi->nItemID);
				if(!pdata)	break;

				if(ISSYNCSTEP(pafi->nItemPkg))
				{
					pafi->nFileStatus = ASIFDL_DL_RST_STATUS_TYPE_SUCCESS_END;
					pdata->nPatchStatus = SS_LOG_PATCH_STATUS_TYPE_DOWN;
					pdata->nEvtTime = GetCurrentDateTimeInt();
					HISYNCSTEPUP(pdata->nSyncSvrStep);
					t_LogicMgrLogPatch->SetLogPatch(*pdata);
				}	
				else
				{
					pafi->nFileStatus = ASIFDL_DL_RST_STATUS_TYPE_SUCCESS_END;
//					WriteLogN("file download status : [%d][%d]:[%d][%s][%d/%d]", pafi->nID, wParam, pafi->nItemType, pafi->szFileName, LOSYNCSTEP(pafi->nItemPkg), HISYNCSTEP(pafi->nItemPkg));
				}
			}
			break;
		}
		case ASIFDL_DL_RST_STATUS_TYPE_ALL_END:
		{
			WriteLogN("all file download end : [%d]", lParam);
			if (t_ExecuteFileUtil)
				t_ExecuteFileUtil->ExecuteFile((void*)this);
			break;
		}
		case ASIFDL_DL_RST_STATUS_TYPE_LOGIC_END:
		{
			break;
		}
	}

	switch(wParam)
	{
		case ASIFDL_DL_RST_STATUS_TYPE_FAIL_END:
		case ASIFDL_DL_RST_STATUS_TYPE_FAIL_HASH_END:
		{
			PASI_FDL_INFO pafi = t_ManageFileDown->FindItem((UINT32)lParam);
			if(!pafi)	break;
			
			switch(pafi->nItemType)
			{
				case SS_DN_FILE_TYPE_DEPLOY:
				{
					DB_LOG_DEPLOY_FILE tDLDF;
					tDLDF.nPolicyID = pafi->nPolicyID;
					tDLDF.nUnitID = pafi->nItemID;
					PDB_LOG_DEPLOY_FILE pCurDLDF = t_ManageLogDeployFile->FindItemByDFInfo(tDLDF);
					if(pCurDLDF)	tDLDF = *pCurDLDF;

					tDLDF.nEndReason = 0;
					tDLDF.nEndTime = 0;
					tDLDF.nEvtErrCode = ERR_SYS_FILE_NOT_DOWNLOAD;			

					if(!pCurDLDF || pCurDLDF->nEndReason != tDLDF.nEndReason || pCurDLDF->nEndTime != tDLDF.nEndTime || 
						pCurDLDF->nEvtErrCode != tDLDF.nEvtErrCode ||
						!tDLDF.nEvtTime || (tDLDF.nEvtTime/TIMER_INTERVAL_TIME_DAY != GetCurrentDateTimeInt()/TIMER_INTERVAL_TIME_DAY))
					{
						tDLDF.nEvtTime	= t_ValidTimeUtil->GetValidTime();
						HISYNCSTEPUP(tDLDF.nSyncSvrStep);
						t_LogicMgrLogDeployFile->SetLogDeployFile(tDLDF);
					}
					break;
				}
			}
			break;
		}
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnMediaChange(WPARAM wParam, LPARAM lParam)
{
// 	if (t_ManageDeviceMedia->OnMediaChange(wParam, lParam) == 0)
// 		if (t_LogicApplyPolicy)		t_LogicApplyPolicy->ForceResetEPSDrv();

	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnPMSWorkNew(WPARAM wParam, LPARAM lParam)
{
/*
	if(wParam == ASI_PMS_WORK_NOTIFY_MSG_TYPE_END_SCAN)	
	{
		if(t_LogicMgrPoPmScan)
		{
			t_LogicMgrPoPmScan->SendPkt_NewPatch(UINT32(lParam));
		}
	}	
*/
	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnPMSWorkOld(WPARAM wParam, LPARAM lParam)
{
/*
	if(wParam == ASI_PMS_WORK_NOTIFY_MSG_TYPE_END_SCAN)	
	{
		if(t_LogicMgrHostPatch)
		{
			t_LogicMgrHostPatch->SendPkt_HostPatch(UINT32(lParam));
		}
	}	
*/
	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnVulnPMSWorkNew(WPARAM wParam, LPARAM lParam)
{
/*
	if(wParam == ASI_PMS_WORK_NOTIFY_MSG_TYPE_END_SCAN)	
	{
		if(t_LogicCtrlVuln)
		{
			t_LogicCtrlVuln->CheckWindowsPatch(UINT32(lParam));
		}
	}	
*/
	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnVulnPMSWorkPreNew(WPARAM wParam, LPARAM lParam)
{
/*
	if(wParam == ASI_PMS_WORK_NOTIFY_MSG_TYPE_END_SCAN)	
	{
		CSimpleVulnPreScanUtil tSimpleVulnPreScanUtil;
		{
			tSimpleVulnPreScanUtil.CheckWindowsPatch(UINT32(lParam));
		}
	}	
*/
	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnWebSockMsg(WPARAM wParam, LPARAM lParam)
{
/*
	PASI_WEBSOCK_MSG pdata = (PASI_WEBSOCK_MSG)wParam;
	if(!pdata)	return 0;

	if(t_LogicMgrPoHostWebSock)	
	{
		t_LogicMgrPoHostWebSock->ChkPolicy(pdata);
	}
*/
	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
/*
	PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
	if(!pHdr)	return 0;

	switch(wParam)
	{
	case DBT_DEVICEARRIVAL:
	case DBT_DEVICEREMOVECOMPLETE:
		{
			WriteLogN("detect usb drive changed. [dvtype:%d]", pHdr->dbch_devicetype);

			if(pHdr->dbch_devicetype != DBT_DEVTYP_VOLUME)
				break;

			if(t_ManagePoHostRun && t_ManagePoHostRun->IsApplyPolicyFromBootTime())
			{
				if (t_LogicApplyPolicy)		t_LogicApplyPolicy->ForceResetEPSDrv();
			}
			else
			{
				if (t_ThreadTimer)		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
			}

			break;
		}
	}
*/
	return 0;
}
//--------------------------------------------------------------------

DWORD CMainDlg::OnDebugMsgTest(WPARAM wParam, LPARAM lParam)
{
// 	t_ManageSysDrive.PrintDbgInfo();
// 	Sleep(1000);
// 
// 	PostMessage(WM_GLOBAL_DEBUG_MSG_TEST);
	return 0;
}
//--------------------------------------------------------------------

