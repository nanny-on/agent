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
#include "LogicMgrCtrlRemoteOrder.h"
//---------------------------------------------------------------------------

CLogicMgrCtrlRemoteOrder*		t_LogicMgrCtrlRemoteOrder = NULL;

//---------------------------------------------------------------------------

CLogicMgrCtrlRemoteOrder::CLogicMgrCtrlRemoteOrder()
{
	m_strLogicName		= "ctrl remote control";

	m_nControlType		= SS_CONTROL_TYPE_CONTROL_REMOTE_ORDER;
	m_nEvtSubType		= EVENT_SUBJECT_TYPE_ADMIN;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_HOST_CTRL_REMOTE_REQUEST;
}
//---------------------------------------------------------------------------

CLogicMgrCtrlRemoteOrder::~CLogicMgrCtrlRemoteOrder()
{
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_CTRL:		nRtn = AnalyzePkt_FromMgr_Ext_Ctrl();			break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::AnalyzePkt_FromMgr_Ext_Ctrl()
{
	MEM_CTRL_REMOTE_ORDER tMCRO;
	
	if(!RecvToken.TokenDel_32(tMCRO.nType))					return AZPKT_CB_RTN_PKT_INVALID;
	if(!RecvToken.TokenDel_32(tMCRO.nSubGroup))				return AZPKT_CB_RTN_PKT_INVALID;
	if(!RecvToken.TokenDel_32(tMCRO.nOption))				return AZPKT_CB_RTN_PKT_INVALID;
	if( RecvToken.TokenDel_String(tMCRO.strValue) < 0)		return AZPKT_CB_RTN_PKT_INVALID;

	if( RecvToken.TokenDel_IDMap(tMCRO.tValueMap) < 0)		return AZPKT_CB_RTN_PKT_INVALID;
	if( RecvToken.TokenDel_StrMap(tMCRO.tStrValueMap) < 0)	return AZPKT_CB_RTN_PKT_INVALID;

	RecvToken.TokenSkip_Block();

	ApplyRemotePolicy(&tMCRO); 

	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::ApplyRemotePolicy(PMEM_CTRL_REMOTE_ORDER pMCRO)
{
	INT32 nRtn = 0;
	m_nEvtOpType = EVENT_OPERATION_TYPE_CONTROL;

	if (!pMCRO)		return nRtn;

	switch(pMCRO->nType)
	{	
		case SS_COMMON_REMOTE_ORDER_TYPE_FE_PTN_GET_FILE_INFO:		nRtn = ApplyRemotePolicy_FEScan(pMCRO);			break;
		case SS_COMMON_REMOTE_ORDER_TYPE_NC_PTN_GET_FILE_INFO:		nRtn = ApplyRemotePolicy_NCScan(pMCRO);			break;
		case SS_COMMON_REMOTE_ORDER_TYPE_NC_PTN_RE_SEND_FILE_INFO:	nRtn = ApplyRemotePolicy_NCReSend(pMCRO);		break;
		case SS_COMMON_REMOTE_ORDER_TYPE_NC_PTN_MAKE_LOCAL_PTN:		nRtn = ApplyRemotePolicy_NCMakeLoPtn(pMCRO);	break;
		case SS_COMMON_REMOTE_ORDER_TYPE_NO_PTN_GET_FILE_INFO:		nRtn = ApplyRemotePolicy_NOScan(pMCRO);			break;
		case SS_COMMON_REMOTE_ORDER_TYPE_NO_PTN_RE_SEND_FILE_INFO:	nRtn = ApplyRemotePolicy_NOReSend(pMCRO);		break;
		case SS_COMMON_REMOTE_ORDER_TYPE_RE_GET_SYSTEMINFO:			nRtn = ApplyRemotePolicy_ReGetSysInfo();		break;
		case SS_COMMON_REMOTE_ORDER_TYPE_NP_SCAN_PATCH:				nRtn = ApplyRemotePolicy_NPScan(pMCRO);			break;
		case SS_COMMON_REMOTE_ORDER_TYPE_NO_VUIN_SCAN:				nRtn = ApplyRemotePolicy_NO_VUINScan(pMCRO);	break;
	}


	{
		InitDLE_OBJ(pMCRO->nType, pMCRO->strValue);
		t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
	}
	
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::ApplyRemotePolicy_FEScan(PMEM_CTRL_REMOTE_ORDER pMCRO)
{
	t_LogicMgrPoFePtnOp->ScanDefaultOption();
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::ApplyRemotePolicy_NCScan(PMEM_CTRL_REMOTE_ORDER pMCRO)
{
	{
		DB_PO_NC_PTN_OP_UNIT data;
		WriteLogN("start remote ctrl : po nc ptn scan : [%s]", pMCRO->strValue.c_str());		
		{			
			data.strScanPath = pMCRO->strValue;
			data.tDPH.strName = "admin demand control";
			data.nFileGatherType = SS_PO_NC_PTN_OP_NEW_FILE_SEND_TYPE_INFO;
			if(t_ThreadPoNcPtnScan)
				t_ThreadPoNcPtnScan->AddWork(&data);
		}
	}
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::ApplyRemotePolicy_NCReSend(PMEM_CTRL_REMOTE_ORDER pMCRO)
{
	{
		WriteLogN("start remote ctrl : po nc ptn resend : [%d]", pMCRO->nType);		
		{			
			t_LogicMgrSiteFile->SendPkt_ReSend();
			t_LogicMgrSiteFileAlias->SendPkt_ReSend();
		}
	}
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::ApplyRemotePolicy_NCMakeLoPtn(PMEM_CTRL_REMOTE_ORDER pMCRO)
{
	if(t_LogicMgrPoNcPtnLo)
	{
		WriteLogN("start remote ctrl : po nc ptn make local ptn : [%d]", pMCRO->nType);
		t_LogicMgrPoNcPtnLo->MakePtn();
	}
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::ApplyRemotePolicy_NOScan(PMEM_CTRL_REMOTE_ORDER pMCRO)
{
	{
		DB_PO_IN_PTN_OP_UNIT data;
		WriteLogN("start remote ctrl : po in ptn scan : [%s]", pMCRO->strValue.c_str());		
		{			
			data.strScanPath = pMCRO->strValue;
			data.tDPH.strName = "admin demand control";
			data.nFileGatherType = SS_PO_IN_PTN_OP_NEW_FILE_SEND_TYPE_INFO;

			t_ThreadPoInPtnScan->AddWork(&data);
		}
	}
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::ApplyRemotePolicy_NOReSend(PMEM_CTRL_REMOTE_ORDER pMCRO)
{
	{
		WriteLogN("start remote ctrl : po in ptn resend : [%d]", pMCRO->nType);		
		{			
			t_LogicMgrSiteFile->SendPkt_ReSend();
			t_LogicMgrSiteFileAlias->SendPkt_ReSend();
		}
	}
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------
INT32		CLogicMgrCtrlRemoteOrder::ApplyRemotePolicy_ReGetSysInfo()
{
	{
		WriteLogN("start remote ctrl : reload system info");		
		{			
			t_LogicMgrHostSys->SendPkt_SysInfo();
			t_LogicMgrHostHw->SendPkt_HostHw(FALSE);
			t_LogicMgrHostSw->SendPkt_HostSw_Cur();
		}

		{
			t_LogicMgrHostPatch->SendPkt_Sync();
		}
	}
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::ApplyRemotePolicy_NPScan(PMEM_CTRL_REMOTE_ORDER pMCRO)
{
	WriteLogN("start remote ctrl : po pm scan : [%d]", pMCRO->nType);		

	if(t_LogicMgrPtnPatch->IsInitLogic() == 0)
	{
		WriteLogE("[%s] is not ptn patch initialize.. by pass", m_strLogicName.c_str());
		return -1;
	}

	{
		t_LogicMgrHostPatch->ScanPatchList(G_TYPE_CTL_REMOTE_ORDER, pMCRO->nType);
	}

	{
		t_LogicMgrPoPmScan->StartScanDefault();
	}

	WriteLogN("[%s] pms scan started by admin", m_strLogicName.c_str());
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteOrder::ApplyRemotePolicy_NO_VUINScan(PMEM_CTRL_REMOTE_ORDER pMCRO)
{
	WriteLogN("start remote ctrl : po no vuin scan : [%d]", pMCRO->nOption);		
	SendToken.TokenAdd_32(pMCRO->nOption);
	SendData_Link(G_TYPE_PO_IN_VULN_REMOTE_SCAN, G_CODE_COMMON_SCAN, SendToken);
	SendToken.Clear();
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------