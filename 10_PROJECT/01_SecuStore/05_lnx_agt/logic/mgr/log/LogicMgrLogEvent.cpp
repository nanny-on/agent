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
#include "LogicMgrLogEvent.h"

//---------------------------------------------------------------------------

CLogicMgrLogEvent*		t_LogicMgrLogEvent = NULL;

//---------------------------------------------------------------------------

CLogicMgrLogEvent::CLogicMgrLogEvent()
{		
	t_ManageLogEvent	= new CManageLogEvent();
	t_LogicLogEvent	= new CLogicLogEvent();

	t_ManageLogEvent->LoadDBMS();

	m_strLogicName		= "mgr log event";

	m_nControlType		= SS_CONTROL_TYPE_LOG_EVENT;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode		= 0;
}
//---------------------------------------------------------------------------

CLogicMgrLogEvent::~CLogicMgrLogEvent()
{
	SAFE_DELETE(t_LogicLogEvent);
	SAFE_DELETE(t_ManageLogEvent);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrLogEvent::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_SYNC:		nRtn = AnalyzePkt_FromMgr_Ext_Sync();				break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogEvent::AnalyzePkt_FromMgr_Add_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogEvent::AnalyzePkt_FromMgr_Edit_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogEvent::AnalyzePkt_FromMgr_Del_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
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

INT32		CLogicMgrLogEvent::AnalyzePkt_FromMgr_Ext_Sync()
{
	TListDBLogEvent tDBLogEventList;

	if(CheckPktResult(RecvToken))					return AZPKT_CB_RTN_RESULT_FAILED;
	if(!RecvToken.TokenDel_32(m_nRecvNum))			return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		DB_LOG_EVENT tDLE;
		if(t_ManageLogEvent->GetPkt(RecvToken, tDLE))		return AZPKT_CB_RTN_PKT_INVALID;

		tDBLogEventList.push_back(tDLE);
	}

	{
		TListDBLogEventItor begin, end;
		begin = tDBLogEventList.begin();	end = tDBLogEventList.end();
		for(begin; begin != end; begin++)
		{
			PDB_LOG_EVENT pdata = t_ManageLogEvent->FindItem(begin->nID);
			if(!pdata)	continue;

			begin->nSkipTarget = pdata->nSkipTarget;
			begin->nSyncSvrMode	= pdata->nSyncSvrMode;
			
			t_ManageLogEvent->EditLogEvent(*begin);
		}
	}

	return AZPKT_CB_RTN_SUCCESS_END;
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

void		CLogicMgrLogEvent::SetLogEvent()
{
	return SetLogEvent(m_tDLE);
}
//---------------------------------------------------------------------------

void		CLogicMgrLogEvent::SetLogEvent(TListDBLogEvent& tDELNList)
{
	TListDBLogEventItor begin, end;
	begin = tDELNList.begin();	end = tDELNList.end();
	for(begin; begin != end; begin++)
	{
		SetLogEvent(*begin);
	}
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrLogEvent::SetLogEvent(DB_LOG_EVENT& dle)
{
	{
		if(dle.tDescIDStrMap.size())
		{
			dle.strEventDescr = MapIDToStr_Str(dle.tDescIDStrMap);
		}

		dle.nUserID = t_ManageHost->GetUserID(t_ManageHost->FirstID());
	}

	{
		PDB_ENV_LOG_UNIT pDELEU = t_ManageEnvLogUnit->FindRecordLogEvtUnit(&dle);
		if(pDELEU && pDELEU->tDPH.nUsedMode == STATUS_USED_MODE_ON)
		{
			dle.nSkipTarget = pDELEU->tDPH.nExtOption;
		}
	}

	if(!(dle.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT) && (dle.nSyncSvrMode & SS_LOG_EVENT_HOST_SYNC_MODE_AGT))
	{
		t_ManageLogEvent->AddLogEvent(dle);
	}
	
	{	
		SendToken.Set(1024);
		SendToken.TokenAdd_32(1);
		t_ManageLogEvent->SetPkt(&dle, SendToken);
		if(!ISSYNCSTEP(dle.nSyncSvrStep) && !(dle.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER) && (dle.nSyncSvrMode & SS_LOG_EVENT_HOST_SYNC_MODE_SVR))
		{
			SendData_Mgr(G_TYPE_LOG_EVENT_HOST, G_CODE_COMMON_SYNC, SendToken);
		}	
		if(!(dle.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT) && (dle.nSyncSvrMode & SS_LOG_EVENT_HOST_SYNC_MODE_AGT))
		{
			SendData_Link(G_TYPE_LOG_EVENT, G_CODE_COMMON_ADD, SendToken);
		}
		SendToken.Clear();
	}
	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrLogEvent::SendPkt_Sync(INT32 nOnceMaxNum)
{
	TListPVOID tSendList;
	{
		t_ManageLogEvent->SetPktSync(tSendList);

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
			t_ManageLogEvent->SetPkt((PDB_LOG_EVENT)(*begin), SendToken);

			nSendNum += 1;
			nOnceNum -= 1;
		}
		SendData_Mgr(G_TYPE_LOG_EVENT_HOST, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();		
	}
	return;
}
//---------------------------------------------------------------------------

