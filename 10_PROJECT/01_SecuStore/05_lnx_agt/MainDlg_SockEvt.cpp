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

DWORD		CMainDlg::OnSockEvtCltLgn(PPKT_DATA pPktData)
{
	if(pPktData == NULL)
	{
		WriteLogE("invalid input clt lgn pkt data");
		return 1;
	}
	t_ManageEvtMon.AddEvtMon(1, pPktData->hdr.type, __FUNCTION__, pPktData->hdr.code);

	switch(pPktData->hdr.type)
	{
		case AL_TYPE_AUTH:
			t_LogicLgnAuth->AnalyzePkt_Auth(pPktData);
			break;
		default:
		{
			WriteLogE("invalid pkt type from clt lgn : [%d]", pPktData->hdr.type);
			pPktData->sock_evt.nErrorCode = ERR_SOCKET_SVR_TO_CLT_INVALID_PKT;
			break;
		}
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}
//--------------------------------------------------------------------


DWORD		CMainDlg::OnSockEvtCltMgr(PPKT_DATA pPktData)
{
	if(pPktData == NULL)
	{
		WriteLogE("invalid input clt mgr pkt data");
		return 1;
	}
	t_ManageEvtMon.AddEvtMon(1, pPktData->hdr.type, __FUNCTION__, pPktData->hdr.code);

	switch(pPktData->hdr.type)
	{
		case AM_TYPE_AUTH:					t_LogicMgrAuth->AnalyzePkt_Auth(pPktData);								break;
		case G_TYPE_HOST:					t_LogicMgrHost->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_HOST_HW:				t_LogicMgrHostHw->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_HOST_SW:				t_LogicMgrHostSw->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_HOST_SYS:				t_LogicMgrHostSys->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_HOST_STATUS_PO:			t_LogicMgrHostStatusPo->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_HOST_KEY:				t_LogicMgrHostKey->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_HOST_PATCH:				t_LogicMgrHostPatch->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_HOST_VULN_RST:			t_LogicMgrHostVulnRst->AnalyzePkt_FromMgr(pPktData);					break;

		case G_TYPE_USER:					t_LogicMgrUser->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_SITE_FILE:				t_LogicMgrSiteFile->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_SITE_FILE_ALIAS:		t_LogicMgrSiteFileAlias->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_SITE_VULN:				t_LogicMgrSiteVuln->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_SITE_VULN_SCAN:			t_LogicMgrSiteVulnScan->AnalyzePkt_FromMgr(pPktData);					break;
		
		case G_TYPE_PO_HOST_RM_INFO:		t_LogicMgrPoHostRmInfo->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_PO_HOST_RUN:			t_LogicMgrPoHostRun->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_SVR_INFO_LGN:		t_LogicMgrPoSvrInfoLgn->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_PO_SVR_INFO_UDT:		t_LogicMgrPoSvrInfoUdt->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_PO_HOST_NOTIFY:			t_LogicMgrPoHostNotify->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_PO_HOST_WEB_SOCK:
		{
			if(t_LogicMgrPoHostWebSock)
				t_LogicMgrPoHostWebSock->AnalyzePkt_FromMgr(pPktData);
			break;
		}

		case G_TYPE_PO_SCH:					t_LogicMgrPoSch->AnalyzePkt_FromMgr(pPktData);							break;

		case G_TYPE_PO_POWER:				t_LogicMgrPoPower->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_WIN_RUN:				t_LogicMgrPoWinRun->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_CTL_PANEL:			t_LogicMgrPoCtlPanel->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_DEPLOY_FILE:			t_LogicMgrPoDeployFile->AnalyzePkt_FromMgr(pPktData);					break;

		case G_TYPE_PO_CTL_PROC:			t_LogicMgrPoCtlProc->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_SELF_PROTECT:		t_LogicMgrPoSelfProtect->AnalyzePkt_FromMgr(pPktData);					break;

		case G_TYPE_PO_FA_OP:				t_LogicMgrPoFaOp->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_PO_FA_CLEAR:			t_LogicMgrPoFaClear->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_FA_ENV:				t_LogicMgrPoFaEnv->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_FA_PROC:				t_LogicMgrPoFaProc->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_FA_BK:				t_LogicMgrPoFaBk->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_PO_FA_NOTIFY:			t_LogicMgrPoFaNotify->AnalyzePkt_FromMgr(pPktData);						break;

		case G_TYPE_PO_FE_PTN_OP:			t_LogicMgrPoFePtnOp->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_FE_PTN_LO:			t_LogicMgrPoFePtnLo->AnalyzePkt_FromMgr(pPktData);						break;	
		case G_TYPE_PO_FE_PTN_BL:			t_LogicMgrPoFePtnBL->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_FE_PTN_WL:			t_LogicMgrPoFePtnWL->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_FE_EXCLUDE:			t_LogicMgrPoFeExclude->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_PO_FE_NOTIFY:			t_LogicMgrPoFeNotify->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_FE_SINGLE_PTN:		t_LogicMgrPoFeSinglePtn->AnalyzePkt_FromMgr(pPktData);					break;
			
		case G_TYPE_PO_IN_PTN_OP:			t_LogicMgrPoInPtnOp->AnalyzePkt_FromMgr(pPktData);						break;
 		case G_TYPE_PO_IN_PTN_BL:			t_LogicMgrPoInPtnBL->AnalyzePkt_FromMgr(pPktData);						break;
 		case G_TYPE_PO_IN_PTN_WL:			t_LogicMgrPoInPtnWL->AnalyzePkt_FromMgr(pPktData);						break;
 		case G_TYPE_PO_IN_PTN_EX:			t_LogicMgrPoInPtnEx->AnalyzePkt_FromMgr(pPktData);						break;
 		case G_TYPE_PO_IN_PTN_SP:			t_LogicMgrPoInPtnSP->AnalyzePkt_FromMgr(pPktData);						break;		
		case G_TYPE_PO_IN_PTN_SP_RULE:		t_LogicMgrPoInPtnSPRule->AnalyzePkt_FromMgr(pPktData);					break;		
		case G_TYPE_PO_IN_PTN_NO:			t_LogicMgrPoInPtnNo->AnalyzePkt_FromMgr(pPktData);						break;
		
		case G_TYPE_PO_IN_VULN_AX:			t_LogicMgrPoInVulnAx->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_IN_VULN_EDIT_APP:	t_LogicMgrPoInVulnEditApp->AnalyzePkt_FromMgr(pPktData);				break;
		case G_TYPE_PO_IN_VULN_OP:			t_LogicMgrPoInVulnOp->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_IN_VULN_QNA:			t_LogicMgrPoInVulnQna->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_PO_IN_VULN_SCAN:		t_LogicMgrPoInVulnScan->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_PO_IN_VULN_SECU_USB:	t_LogicMgrPoInVulnSecuUsb->AnalyzePkt_FromMgr(pPktData);				break;
		case G_TYPE_PO_IN_VULN_SW:			t_LogicMgrPoInVulnSw->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_IN_VULN_PATCH_EXCEPTION:	t_LogicMgrPoInVulnPatchException->AnalyzePkt_FromMgr(pPktData);		break;

		case G_TYPE_PO_IN_RS_OP:			t_LogicMgrPoInRsOp->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_IN_RS_BK:			t_LogicMgrPoInRsBk->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_IN_RS_NO:			t_LogicMgrPoInRsNo->AnalyzePkt_FromMgr(pPktData);						break;

		case G_TYPE_PO_PM_OP:				t_LogicMgrPoPmOp->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_PO_PM_DM:				t_LogicMgrPoPmDm->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_PO_PM_SCAN:				t_LogicMgrPoPmScan->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_PM_EX:				t_LogicMgrPoPmEx->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_PO_PM_NO:				t_LogicMgrPoPmNo->AnalyzePkt_FromMgr(pPktData);							break;

		case G_TYPE_PO_NC_PTN_OP:
		{
			if(t_LogicMgrPoNcPtnOp)
				t_LogicMgrPoNcPtnOp->AnalyzePkt_FromMgr(pPktData);
			break;
		}
		case G_TYPE_PO_NC_PTN_LO:
		{
			if(t_LogicMgrPoNcPtnLo)
				t_LogicMgrPoNcPtnLo->AnalyzePkt_FromMgr(pPktData);
			break;
		}
		case G_TYPE_PO_NC_PTN_BL:
		{
			if(t_LogicMgrPoNcPtnBL)
				t_LogicMgrPoNcPtnBL->AnalyzePkt_FromMgr(pPktData);
			break;
		}
		case G_TYPE_PO_NC_PTN_WL:
		{
			if(t_LogicMgrPoNcPtnWL)
				t_LogicMgrPoNcPtnWL->AnalyzePkt_FromMgr(pPktData);
			break;
		}
		case G_TYPE_PO_NC_PTN_EX:
		{
			if(t_LogicMgrPoNcPtnEx)
				t_LogicMgrPoNcPtnEx->AnalyzePkt_FromMgr(pPktData);
			break;
		}
		case G_TYPE_PO_NC_PTN_SP:
		{
			if(t_LogicMgrPoNcPtnSP)
				t_LogicMgrPoNcPtnSP->AnalyzePkt_FromMgr(pPktData);
			break;		
		}
		case G_TYPE_PO_NC_PTN_SP_RULE:
		{
			if(t_LogicMgrPoNcPtnSPRule)
				t_LogicMgrPoNcPtnSPRule->AnalyzePkt_FromMgr(pPktData);
			break;
		}
		case G_TYPE_PO_NC_PTN_NO:
		{
			if(t_LogicMgrPoNcPtnNo)
				t_LogicMgrPoNcPtnNo->AnalyzePkt_FromMgr(pPktData);
			break;
		}
		case G_TYPE_PO_DV_OP:				t_LogicMgrPoInDevOOp->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_DV_BL:				t_LogicMgrPoInDevOBL->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_DV_WL:				t_LogicMgrPoInDevOWL->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_DV_EX:				t_LogicMgrPoInDevOEx->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_PO_DV_INFO:				t_LogicMgrPoInDevOInfo->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_PO_DV_NOTIFY:			t_LogicMgrPoInDevONotify->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_PO_DV_LO:				t_LogicMgrPoDvLo->AnalyzePkt_FromMgr(pPktData);							break;
		
		case G_TYPE_ENV_LICENSE:			t_LogicMgrEnvLicense->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_ENV_LOG:				t_LogicMgrEnvLog->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_ENV_LOG_UNIT:			t_LogicMgrEnvLogUnit->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_ENV_NOTIFY_INFO:		t_LogicMgrEnvNotifyInfo->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_ENV_SELF_PROTECT_AGT:	t_LogicMgrEnvSelfProtectAgt->AnalyzePkt_FromMgr(pPktData);				break;
		case G_TYPE_ENV_SORG_LINK:			t_LogicMgrEnvSOrgLink->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_ENV_TRUST_SUBJECT:		t_LogicMgrEnvTrustSubject->AnalyzePkt_FromMgr(pPktData);				break;
		case G_TYPE_ENV_SOCKET:				t_LogicMgrEnvSocket->AnalyzePkt_FromMgr(pPktData);						break;

		case G_TYPE_LOG_DEPLOY_FILE:		t_LogicMgrLogDeployFile->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_LOG_DOC:				t_LogicMgrLogDoc->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_LOG_SECU:				t_LogicMgrLogSecu->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_LOG_EVENT_HOST:			t_LogicMgrLogEvent->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_LOG_PATCH:				t_LogicMgrLogPatch->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_LOG_DEVICE:				t_LogicMgrLogDevice->AnalyzePkt_FromMgr(pPktData);						break;
		case G_TYPE_LOG_RS:					t_LogicMgrLogRs->AnalyzePkt_FromMgr(pPktData);							break;
		case G_TYPE_LOG_RS_BK:				t_LogicMgrLogRsBk->AnalyzePkt_FromMgr(pPktData);						break;

		case G_TYPE_CTL_REMOTE:				t_LogicMgrCtrlRemoteCtrl->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_CTL_REMOTE_POWER:		t_LogicMgrCtrlRemotePower->AnalyzePkt_FromMgr(pPktData);				break;
		case G_TYPE_CTL_REMOTE_ORDER:		t_LogicMgrCtrlRemoteOrder->AnalyzePkt_FromMgr(pPktData);				break;
		case G_TYPE_CTL_INSTANT_MSG:		t_LogicMgrCtrlInstantMsg->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_CTL_RESINFO:			t_LogicMgrCtrlHostRes->AnalyzePkt_FromMgr(pPktData);					break;
		case G_TYPE_CTL_RESTORE:			t_LogicMgrCtrlRestore->AnalyzePkt_FromMgr(pPktData);					break;

		case G_TYPE_INFO_PROC_REPUTATION:
		{
			if(t_LogicMgrInfoProcReputation)
				t_LogicMgrInfoProcReputation->AnalyzePkt_FromMgr(pPktData);
			break;
		}
		default:
		{
			WriteLogE("invalid pkt type from clt mgr : [%x]", pPktData->hdr.type);
			pPktData->sock_evt.nErrorCode = ERR_SOCKET_SVR_TO_CLT_INVALID_PKT;
			break;
		}
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}
//--------------------------------------------------------------------

DWORD		CMainDlg::OnSockEvtCltUdt(PPKT_DATA pPktData)
{
	if(pPktData == NULL)
	{
		WriteLogE("invalid input clt udt pkt data");
		return 1;
	}

	t_ManageEvtMon.AddEvtMon(1, pPktData->hdr.type, __FUNCTION__, pPktData->hdr.code);

	switch(pPktData->hdr.type)
	{
		case AU_TYPE_AUTH:		t_LogicUdtAuth->AnalyzePkt_Auth(pPktData);		break;
		default:
		{
			WriteLogE("invalid pkt type from clt udt : [%d]", pPktData->hdr.type);
			pPktData->sock_evt.nErrorCode = ERR_SOCKET_SVR_TO_CLT_INVALID_PKT;
			break;
		}
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}
//--------------------------------------------------------------------

DWORD		CMainDlg::OnSockEvtSvrLink(PPKT_DATA pPktData)
{
	if(pPktData == NULL)
	{
		WriteLogE("invalid input svr link pkt data");
		return 1;
	}
	
	t_ManageEvtMon.AddEvtMon(1, pPktData->hdr.type, __FUNCTION__, pPktData->hdr.code);

	switch(pPktData->hdr.type)
	{
		case AK_TYPE_AUTH:				t_LogicAuth->AnalyzePkt_Auth(pPktData);					break;
		case G_TYPE_INIT:				t_LogicInitLink->AnalyzePkt_Init(pPktData);				break;

		case G_TYPE_LOG_EVENT:			t_LogicLogEvent->AnalyzePkt_LogEvent(pPktData);			break;
		case G_TYPE_PO_FA_CLEAR:		t_LogicPoFaClear->AnalyzePkt_FromLink(pPktData);		break;
		case G_TYPE_LOG_DOC_DSCAN:		t_LogicLogDocDScan->AnalyzePkt_FromLink(pPktData);		break;
		case G_TYPE_LOG_RS_BK:			t_LogicLogRsBk->AnalyzePkt_FromLink(pPktData);			break;

		case G_TYPE_USER:				t_LogicUser->AnalyzePkt_FromLink_User(pPktData);		break;
		case G_TYPE_PO_FE_SINGLE_PTN:	t_LogicMgrPoFeSinglePtn->AnalyzePkt_FromLink(pPktData);	break;
		case G_TYPE_ENV_NOTIFY_INFO:	t_LogicEnvNotifyInfo->AnalyzePkt_FromLink(pPktData);	break;
		
		case G_TYPE_PO_IN_PTN_SP_RULE:	t_LogicPoInPtnSPRule->AnalyzePkt_FromLink(pPktData);	break;
		case G_TYPE_PO_IN_PTN_EX_TRUST:	t_LogicPoInPtnExTrust->AnalyzePkt_FromLink(pPktData);	break;
		case G_TYPE_PO_NC_PTN_SP_RULE:	t_LogicPoNcPtnSPRule->AnalyzePkt_FromLink(pPktData);	break;
		case G_TYPE_PO_NC_PTN_LO_PTN:	t_LogicPoNcPtnLoPtn->AnalyzePkt_FromLink(pPktData);		break;
		case G_TYPE_PO_NC_PTN_EX_TRUST:	t_LogicPoNcPtnExTrust->AnalyzePkt_FromLink(pPktData);	break;
		case G_TYPE_HOST_VULN_RST:		t_LogicHostVulnRst->AnalyzePkt_FromLink(pPktData);		break;

		case G_TYPE_CTL_VULN:			t_LogicCtrlVuln->AnalyzePkt_FromLink(pPktData);			break;
		default:
		{
			WriteLogE("invalid pkt type from svr link : [%d]", pPktData->hdr.type);
			pPktData->sock_evt.nErrorCode = ERR_SOCKET_SVR_TO_CLT_INVALID_PKT;
			break;
		}
	}
	t_ManageEvtMon.DelEvtMon(__FUNCTION__);
	return 0;
}
//--------------------------------------------------------------------

