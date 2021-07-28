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
#include "LogicMgrPoHostWebSock.h"

//---------------------------------------------------------------------------

CLogicMgrPoHostWebSock*		t_LogicMgrPoHostWebSock = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoHostWebSock::CLogicMgrPoHostWebSock()
{
	t_ManagePoHostWebSock	= new CManagePoHostWebSock();
	t_ManagePoHostWebSock->LoadDBMS();
	t_ManagePoHostWebSock->InitHash();

	m_strLogicName		= "mgr po host web sock";
	
	m_nPolicyType		= SS_POLICY_TYPE_HOST_WEB_SOCK;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_HOST_WEB_SOCK;
}
//---------------------------------------------------------------------------

CLogicMgrPoHostWebSock::~CLogicMgrPoHostWebSock()
{
	SAFE_DELETE(t_ManagePoHostWebSock);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
INT32		CLogicMgrPoHostWebSock::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostWebSock::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_HOST_WEB_SOCK pdata = NULL;
	DB_PO_HOST_WEB_SOCK data;
	m_tDPH = &(data.tDPH);

	if( t_ManagePoHostWebSock->GetPkt(RecvToken, data))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	pdata = (PDB_PO_HOST_WEB_SOCK)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoHostWebSock->DelPoHostWebSock(pdata->tDPH.nID);
	}

	{
		if(SetER(t_ManagePoHostWebSock->ApplyPoHostWebSock(data)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostWebSock::ApplyPolicy()
{
/*
	CProcUtil tProcUtil;
	CSystemInfo tSysInfo;
	
	PDB_PO_HOST_WEB_SOCK pdata = (PDB_PO_HOST_WEB_SOCK)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		WriteLogE("[%s] not find current policy", m_strLogicName);
		return 0;
	}

	if(pdata->tDPH.nUsedMode == STATUS_MODE_OFF)
	{
//		t_ASIWebSockDLLUtil->ASIWEBSOCK_ClearSvr();		
	}
	else
	{
		UINT32 nRunMode = 0;
//		t_ASIWebSockDLLUtil->ASIWEBSOCK_IsRunSvr(pdata->nSvrPort, nRunMode);

		if(!nRunMode)
		{
			t_ASIWebSockDLLUtil->ASIWEBSOCK_StartSvr(pdata->nSvrPort);
		}		
	}

	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
	}
*/
	return 0;
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

INT32		CLogicMgrPoHostWebSock::ChkPolicy(PASI_WEBSOCK_MSG pdata)
{
/*
	WriteLogN("[%s] recv web sock msg : [%d] : [%d:%d] [%s]", m_strLogicName.c_str(), pdata->nMsgType, pdata->nSvrPort, pdata->nCliPort, pdata->lpMsgValue);
	PDB_PO_HOST_WEB_SOCK pdata_pol = (PDB_PO_HOST_WEB_SOCK)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);	

	switch(pdata->nMsgType)
	{
		case ASI_WEBSOCK_MSG_TYPE_CONNECT:
		{
			WriteLogN("[%s] set po host web sock con num success : connect [%d]", m_strLogicName.c_str(), t_ManagePoHostWebSock->CntConWeb());
			break;
		}
		case ASI_WEBSOCK_MSG_TYPE_DISCONNECT:
		{
			t_ManagePoHostWebSock->DelConWeb(pdata->nCliPort);
			if(SetER(t_ASIEPSAPPDLLUtil->SetPoHostWebSockConWebNum(t_ManagePoHostWebSock->CntConWeb(), 0)))
			{
				WriteLogE("[%s] set po host web sock con num fail : disconnect [%d]", m_strLogicName.c_str(), g_nErrRtn);
				break;
			}
			WriteLogN("[%s] set po host web sock con num success : disconnect [%d]", m_strLogicName.c_str(), t_ManagePoHostWebSock->CntConWeb());
			break;
		}
		case ASI_WEBSOCK_MSG_TYPE_RECV_MSG:
		{
			if(!pdata_pol)
			{
				SendMsg(pdata, "Wait");
				break;
			}			
			
			if(pdata_pol->strWebKey.CompareNoCase(pdata->lpMsgValue))
			{
				SendMsg(pdata, "invalid key");
				break;
			}

			t_ManagePoHostWebSock->AddConWeb(pdata->nCliPort);
			if(SetER(t_ASIEPSAPPDLLUtil->SetPoHostWebSockConWebNum(t_ManagePoHostWebSock->CntConWeb(), 1)))
			{
				WriteLogE("[%s] set po host web sock con num fail : connect [%d]", m_strLogicName.c_str(), g_nErrRtn);
				break;
			}

			String strSendMsg = "Unknown";

			do 
			{
				if(ChkPolicy_InVulnOp(pdata_pol, strSendMsg))
				{
					break;
				}

				if(ChkPolicy_InPtnOp(pdata_pol, strSendMsg))
				{
					break;
				}
			} while (FALSE);						

			SendMsg(pdata, strSendMsg);

			break;
		}
	}
*/	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostWebSock::ChkPolicy_InVulnOp(PDB_PO_HOST_WEB_SOCK pdata_pol, String& strMsg)
{
	INT32 nRtn = 0;
/*
	if(t_ManagePoHostWebSock->GetPolicyType(pdata_pol->tPolicyTypeMap, SS_POLICY_TYPE_IN_VULN_OP) == STATUS_MODE_ON)
	{
		String strDangerVulnName = "";
		UINT32 nDangerVulnID = 0;
		INT32 nRtn = t_LogicMgrPoInVulnOp->SimpleScanVuln(nDangerVulnID);
		switch(nRtn)
		{
			case SS_PTN_VULN_SCAN_RESULT_DANGER:
			{					
				{
					UINT8 nType = LOTYPEID32(nDangerVulnID);
					UINT32	nUID = HITYPEID32(nDangerVulnID);
					switch(nType)
					{
						case SS_COMMON_UNIT_TYPE_GLOBAL:
						{
							PDB_PTN_VULN pdata_vuln = t_MMPPVuln->t_ManagePtnVuln->FindItem(nUID);
							if(!pdata_vuln)		break;					

							strDangerVulnName = pdata_vuln->strName;
							break;
						}
					}
				}
				nRtn = -1;
				strMsg = "Danger";
				break;
			}
			case SS_PTN_VULN_SCAN_RESULT_SCANNING:	
			{
				strMsg = "Wait";
				nRtn = -1;
				break;
			}
			case SS_PTN_VULN_SCAN_RESULT_SAFE:		strMsg = "Safe";		break; 
		}

		{
			WriteLogN("[%s] request check unit is : [ss_policy_in_vuln_op][%s][%s]", m_strLogicName.c_str(), strMsg.c_str(), strDangerVulnName.c_str());
		}
	}
*/	
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostWebSock::ChkPolicy_InPtnOp(PDB_PO_HOST_WEB_SOCK pdata_pol, String& strMsg)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostWebSock::SendMsg(PASI_WEBSOCK_MSG pdata, String strMsg)
{
/*
	ASI_WEBSOCK_MSG tAWM;
	{
		tAWM.nSvrPort		= pdata->nSvrPort;
		tAWM.nCliPort		= pdata->nCliPort;
		tAWM.nMsgType		= ASI_WEBSOCK_MSG_TYPE_SEND_MSG;

		tAWM.lpMsgValue		= (LPCTSTR)strMsg;
		tAWM.nMsgLength		= strMsg.length();
	}

//	t_ASIWebSockDLLUtil->ASIWEBSOCK_SendMsg(&tAWM);
*/
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
