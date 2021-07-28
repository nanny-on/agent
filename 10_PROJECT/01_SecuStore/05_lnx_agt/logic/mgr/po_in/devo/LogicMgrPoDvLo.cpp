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
#include "LogicMgrPoDvLo.h"

//---------------------------------------------------------------------------

CLogicMgrPoDvLo*		t_LogicMgrPoDvLo = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoDvLo::CLogicMgrPoDvLo()
{
	m_strLogicName = "mgr agt po dv lo";
	
	m_nEvtObjType	= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode	= 0;

	t_ManagePoDvDefault			= new CManagePoDvDefault();
	t_ManagePoDvDefault->LoadDBMS();
}
//---------------------------------------------------------------------------

CLogicMgrPoDvLo::~CLogicMgrPoDvLo()
{
	SAFE_DELETE(t_ManagePoDvDefault);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoDvLo::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_DRTADD:			nRtn = AnalyzePkt_FromMgr_Ext_DirectAdd();	break;
		case G_CODE_COMMON_DRTDEL:			nRtn = AnalyzePkt_FromMgr_Ext_DirectDel();	break;
		case G_CODE_COMMON_LOAD:			nRtn = AnalyzePkt_FromMgr_Ext_GetDvLo();	break;
		default:
		{	
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoDvLo::AnalyzePkt_FromMgr_Ext_DirectAdd()
{
	SendToken.Clear();
	INT32 nConSessionID = 0, nCnt = 0, nRet = AZPKT_CB_RTN_SEND_ACK, nID = 0;
	String strDevName = "", strDvInstanceID = "", strDvPaInstanceID = "", strClassName = "";

	if(!RecvToken.TokenDel_32(nConSessionID))					return AZPKT_CB_RTN_PKT_INVALID;
	if(!RecvToken.TokenDel_32(m_nHostID))						return AZPKT_CB_RTN_PKT_INVALID;
	if( RecvToken.TokenDel_String(strDevName) < 0)				return AZPKT_CB_RTN_PKT_INVALID;
	if( RecvToken.TokenDel_String(strDvInstanceID) < 0)			return AZPKT_CB_RTN_PKT_INVALID;
	if( RecvToken.TokenDel_String(strDvPaInstanceID) < 0)		return AZPKT_CB_RTN_PKT_INVALID;
	if( RecvToken.TokenDel_String(strClassName) < 0)		return AZPKT_CB_RTN_PKT_INVALID;

	SendToken.TokenAdd_32(nConSessionID);
	SendToken.TokenAdd_32(m_nHostID);

	PDB_PO_IN_DEVO_OP pdata = t_ManagePoInDevOOp->LastItem();
	if(pdata)
	{
		if(pdata->nBlockMode != 1)	return nRet;
	}

	nID = t_ManagePoDvDefault->FindDvPolID(strDvInstanceID, strDvPaInstanceID, 0);
	if(nID)
	{
		SendToken.TokenAdd_32(AZPKT_CB_RTN_SUCCESS);
		PDB_PO_DV_DEFAULT pData = t_ManagePoDvDefault->FindItem(nID);
		if(pData && pData->tDPH.nUsedMode == 0)
		{
			pData->tDPH.nUsedMode = 1;
			if(!t_ManagePoDvDefault->EditPoDvDefault(*pData))
			{
				SendToken.TokenAdd_32(AZPKT_CB_RTN_SUCCESS);
				t_ManagePoDvDefault->SetPkt(nID, SendToken);
//				t_ASIEPSAPPDLLUtil->Stop();
				t_ManageDevOInfo->CheckDvPolicy();
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
			}
		}
		t_ManagePoDvDefault->SetPkt(nID, SendToken);
	}
	else
	{
		DB_PO_DV_DEFAULT ddd;
		ddd.tDPH.nUsedMode = 1;
		ddd.nPoPerm = RET_ALLOW;
		ddd.strDvName = strDevName;
		ddd.strDvInstanceID = strDvInstanceID;
		ddd.strDvPaInstanceID = strDvPaInstanceID;
		ddd.strClassName = strClassName;
		if(!t_ManagePoDvDefault->AddPoDvDefault(ddd))
		{
			SendToken.TokenAdd_32(AZPKT_CB_RTN_SUCCESS);
			t_ManagePoDvDefault->SetPkt(nID, SendToken);
//			t_ASIEPSAPPDLLUtil->Stop();
			t_ManageDevOInfo->CheckDvPolicy();
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
		}
		else
		{
			SendToken.TokenAdd_32(AZPKT_CB_RTN_RESULT_FAILED);
		}
	}
	return nRet;
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoDvLo::AnalyzePkt_FromMgr_Ext_DirectDel()
{
	SendToken.Clear();
	INT32 nConSessionID = 0, nCnt = 0, nID = 0, nRet = AZPKT_CB_RTN_SEND_ACK;

	if(!RecvToken.TokenDel_32(nConSessionID))	return AZPKT_CB_RTN_PKT_INVALID;
	if(!RecvToken.TokenDel_32(m_nHostID))		return AZPKT_CB_RTN_PKT_INVALID;
	if(!RecvToken.TokenDel_32(nID))				return AZPKT_CB_RTN_PKT_INVALID;

	if(nID)
	{
		SendToken.TokenAdd_32(nConSessionID);
		SendToken.TokenAdd_32(m_nHostID);
		SendToken.TokenAdd_32(nID);
		if(!t_ManagePoDvDefault->DeleteDvDefault(nID))
			SendToken.TokenAdd_32(AZPKT_CB_RTN_SUCCESS);
		else
			SendToken.TokenAdd_32(AZPKT_CB_RTN_RESULT_FAILED);
		SendData_Mgr(G_TYPE_PO_DV_LO, m_nPktCode, SendToken);
		SendToken.Clear();
		m_nHostID = 0;

//		t_ASIEPSAPPDLLUtil->Stop();
		t_ManageDevOInfo->CheckDvPolicy();
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
	}
	else
	{
		SendToken.TokenAdd_32(nConSessionID);
		SendToken.TokenAdd_32(m_nHostID);
		SendToken.TokenAdd_32(nID);
		SendToken.TokenAdd_32(AZPKT_CB_RTN_RESULT_FAILED);
		SendData_Mgr(G_TYPE_PO_DV_LO, m_nPktCode, SendToken);
		SendToken.Clear();
		m_nHostID = 0;
	}

	return nRet;
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
INT32		CLogicMgrPoDvLo::AnalyzePkt_FromMgr_Ext_GetDvLo()
{
	SendToken.Clear();

	INT32 nConSessionID = 0, nCnt = 0;

	if(!RecvToken.TokenDel_32(nConSessionID))		return AZPKT_CB_RTN_PKT_INVALID;
	if(!RecvToken.TokenDel_32(m_nHostID))			return AZPKT_CB_RTN_PKT_INVALID;
	
	SendToken.TokenAdd_32(nConSessionID);
	SendToken.TokenAdd_32(m_nHostID);

	t_ManagePoDvDefault->SetPkt(SendToken, TRUE);

	SendData_Mgr(G_TYPE_PO_DV_LO, m_nPktCode, SendToken);
	SendToken.Clear();
	m_nHostID = 0;

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------