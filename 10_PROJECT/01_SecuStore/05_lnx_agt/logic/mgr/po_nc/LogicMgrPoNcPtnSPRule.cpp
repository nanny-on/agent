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
#include "LogicMgrPoNcPtnSPRule.h"

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnSPRule*		t_LogicMgrPoNcPtnSPRule = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnSPRule::CLogicMgrPoNcPtnSPRule()
{
	t_LogicPoNcPtnSPRule		= new CLogicPoNcPtnSPRule();
	t_ManagePoNcPtnSPRule		= new CManagePoNcPtnSPRule();
	
	t_ManagePoNcPtnSPRule->LoadDBMS();
	
	t_ManagePoNcPtnSPRule->InitHash();

	m_strLogicName		= "mgr agt po nc ptn sp rule";
	
	m_nPolicyUnitType	= SS_POLICY_UNIT_TYPE_NC_PTN_SP_RULE;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_NC_PTN_SP;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY_UNIT;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_UNIT_NC_PTN_SP_RULE;
}
//---------------------------------------------------------------------------

CLogicMgrPoNcPtnSPRule::~CLogicMgrPoNcPtnSPRule()
{
	SAFE_DELETE(t_ManagePoNcPtnSPRule);
	SAFE_DELETE(t_LogicPoNcPtnSPRule);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnSPRule::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_SYNC:		nRtn = AnalyzePkt_FromMgr_Ext_Sync();			break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnSPRule::AnalyzePkt_FromMgr_Edit_Ext()
{	
	TListDBPoNcPtnSPRule	tRuleList;

	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_NC_PTN_SP_RULE		data;	

		if( t_ManagePoNcPtnSPRule->GetPkt(RecvToken, data))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tRuleList.push_back(data);
	}

	{
		TListID tIDList;
		t_ManagePoNcPtnSPRule->GetItemIDList(tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManagePoNcPtnSPRule->IsExistID(*begin, tRuleList))		continue;

			t_ManagePoNcPtnSPRule->DelPoNcPtnSPRule(*begin);
		}
	}

	{
		TListDBPoNcPtnSPRuleItor begin, end;
		begin = tRuleList.begin();	end = tRuleList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManagePoNcPtnSPRule->ApplyPoNcPtnSPRule(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		t_ManagePoNcPtnSPRule->InitHash();
	}

	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnSPRule::AnalyzePkt_FromMgr_Ext_Sync()
{	
	TListDBSiteFile tDBSiteFileList;

	if(CheckPktResult(RecvToken))							return AZPKT_CB_RTN_RESULT_FAILED;
	if( RecvToken.TokenDel_IDList(m_tRecvIDList))			return AZPKT_CB_RTN_PKT_INVALID;
	
	{
		TListIDItor begin, end;
		begin = m_tRecvIDList.begin();	end = m_tRecvIDList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManagePoNcPtnSPRule->DelPoNcPtnSPRule(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] sync policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		{
			t_ManagePoNcPtnSPRule->SetPkt(SendToken);
			SendData_Link(G_TYPE_PO_NC_PTN_SP_RULE, G_CODE_COMMON_EDIT, SendToken);
			SendToken.Clear();
		}

		{
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
		}
	}

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnSPRule::OnTimer_Logic()
{
	UINT32 nCurTime = GetCurrentDateTimeInt();
	UINT32 nChgMode = 0;
	
	TListID tIDList;
	t_ManagePoNcPtnSPRule->GetItemIDList(tIDList);
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_NC_PTN_SP_RULE pdata = t_ManagePoNcPtnSPRule->FindItem(*begin);
		if(!pdata)		continue;

		if(pdata->tDPH.nID < SS_PO_NC_PTN_SP_RULE_LOCAL_START_ID)						continue;

		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_ON)								continue;
		if(pdata->nConfirmRst == SS_PO_NC_PTN_SP_RULE_CONFIRM_RST_TYPE_WAIT)			continue;
		if(pdata->nConfirmRst == SS_PO_NC_PTN_SP_RULE_CONFIRM_RST_TYPE_END)				continue;
		if(pdata->nConfirmRst == SS_PO_NC_PTN_SP_RULE_CONFIRM_RST_TYPE_END_FOREVER)		continue;
		if(pdata->nEndDate)																continue;
		if(pdata->nUseDay == 0)															continue;

		UINT32 nLimitTime = pdata->nEvtDate + (pdata->nUseDay*TIMER_INTERVAL_TIME_DAY);
		if(nLimitTime > nCurTime)		continue;

		pdata->nEndDate = nLimitTime;
		pdata->nConfirmRst = SS_PO_NC_PTN_SP_RULE_CONFIRM_RST_TYPE_END;

		if(SetER(t_ManagePoNcPtnSPRule->EditPoNcPtnSPRule(*pdata)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] edit policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_SEND_ACK);
		}

		nChgMode = 1;
	}

	if(nChgMode)
	{
		t_ManagePoNcPtnSPRule->SetPkt(SendToken);
		SendData_Link(G_TYPE_PO_NC_PTN_SP_RULE, G_CODE_COMMON_EDIT, SendToken);
		SendToken.Clear();
	}
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrPoNcPtnSPRule::SendPkt_Req(PDB_PO_NC_PTN_SP_RULE pdata)
{	
	SendToken.Clear();
	t_ManagePoNcPtnSPRule->SetPkt(pdata, SendToken);
	SendData_Mgr(G_TYPE_PO_NC_PTN_SP_RULE, G_CODE_COMMON_REQUEST, SendToken);
	SendToken.Clear();
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrPoNcPtnSPRule::SendPkt_Sync(INT32 nOnceMaxNum)
{
	TListPVOID tSendList;
	{
		t_ManagePoNcPtnSPRule->SetPktSync(tSendList);

		if(tSendList.empty())	return;
	}

	INT32 nOnceNum = nOnceMaxNum;
	INT32 nSendNum = 0;

	TListPVOIDItor begin, end;
	begin = tSendList.begin();	end = tSendList.end();

	while(nSendNum < tSendList.size())
	{
		nOnceNum = (((tSendList.size() - nSendNum) > nOnceMaxNum && nOnceMaxNum > 0) ? nOnceMaxNum : (tSendList.size() - nSendNum));

		SendToken.Clear();
		SendToken.TokenAdd_32(nOnceNum);
		for(begin; begin != end && nOnceNum; begin++)
		{
			t_ManagePoNcPtnSPRule->SetPkt((PDB_PO_NC_PTN_SP_RULE)(*begin), SendToken);

			nSendNum += 1;
			nOnceNum -= 1;
		}
		SendData_Mgr(G_TYPE_PO_NC_PTN_SP_RULE, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();		
	}
	return;
}
//---------------------------------------------------------------------------