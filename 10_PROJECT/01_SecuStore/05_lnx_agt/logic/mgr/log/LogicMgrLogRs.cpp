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
#include "LogicMgrLogRs.h"

//---------------------------------------------------------------------------

CLogicMgrLogRs*		t_LogicMgrLogRs = NULL;

//---------------------------------------------------------------------------

CLogicMgrLogRs::CLogicMgrLogRs()
{	
	t_ManageLogRs	= new CManageLogRs();
	t_LogicLogRs	= new CLogicLogRs();

	t_ManageLogRs->LoadDBMS();

	m_strLogicName		= "logic mgr log rs";

	m_nControlType		= SS_CONTROL_TYPE_LOG_RS;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode		= 0;
}
//---------------------------------------------------------------------------

CLogicMgrLogRs::~CLogicMgrLogRs()
{
	SAFE_DELETE(t_LogicLogRs);
	SAFE_DELETE(t_ManageLogRs);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrLogRs::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrLogRs::AnalyzePkt_FromMgr_Add_Ext()
{
	DB_LOG_RS data;

	if(t_ManageLogRs->GetPkt(RecvToken, data))		return AZPKT_CB_RTN_PKT_INVALID;

	data.nEvtTime	= t_ValidTimeUtil->GetValidTime();
	HISYNCSTEPUP(data.nSyncSvrStep);

	SetLogRs(data);

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogRs::AnalyzePkt_FromMgr_Edit_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogRs::AnalyzePkt_FromMgr_Del_Ext()
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrLogRs::AnalyzePkt_FromMgr_Ext_Sync()
{	
	TListDBLogRs tDBLogRsList;

	if(CheckPktResult(RecvToken))					return AZPKT_CB_RTN_RESULT_FAILED;
	if(!RecvToken.TokenDel_32(m_nRecvNum))			return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		DB_LOG_RS tDLS;
		if(t_ManageLogRs->GetPkt(RecvToken, tDLS))		return AZPKT_CB_RTN_PKT_INVALID;

		tDBLogRsList.push_back(tDLS);
	}

	{
		TListDBLogRsItor begin, end;
		begin = tDBLogRsList.begin();	end = tDBLogRsList.end();
		for(begin; begin != end; begin++)
		{
			t_ManageLogRs->EditLogRs(*begin);
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


void		CLogicMgrLogRs::SetLogRs(DB_LOG_RS& data)
{
	{
		if(data.strObjectPath.find("%") == string::npos && data.strObjectName.find("%") == string::npos &&
			data.strSubjectPath.find("%") == string::npos && data.strSubjectName.find("%") == string::npos)
		{
			WriteLogN("[%s] remain evt log to file : pt[%d]:bt[%d]:ot:[%d]:sb[%s/%s]:ob[%s/%s]", 
							m_strLogicName.c_str(),
							data.nPolicyType, data.nBlockType, data.nObjectType, 
							data.strSubjectPath.c_str(), data.strSubjectName.c_str(), 
							data.strObjectPath.c_str(), data.strObjectName.c_str());

		}
	}

	{
		data.nUserID = t_ManageHost->GetUserID(t_ManageHost->FirstID());
	}

	{
		PDB_ENV_LOG_UNIT pDELEU = t_ManageEnvLogUnit->FindRecordLogRsUnit(&data);
		if(pDELEU && pDELEU->tDPH.nUsedMode == STATUS_USED_MODE_ON)
		{
			data.nSkipTarget = pDELEU->tDPH.nExtOption;
		}
	}
	
	if(!(data.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT))
	{
		t_ManageLogRs->AddLogRs(data);
	}

	{
		m_tMutex.Lock();

		SendToken.Set(1024);
		SendToken.TokenAdd_32(1);
		t_ManageLogRs->SetPkt(&data, SendToken);
		if(!(data.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER))
		{
			SendData_Mgr(G_TYPE_LOG_RS, G_CODE_COMMON_SYNC, SendToken);
		}
		if(!(data.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT))		
		{
			SendData_Link(G_TYPE_LOG_RS, G_CODE_COMMON_SYNC, SendToken);
		}
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrLogRs::SendPkt_Sync(INT32 nOnceMaxNum)
{
	TListPVOID tSendList;
	{
		t_ManageLogRs->SetPktSync(tSendList);

		if(tSendList.empty())	return;
	}

	INT32 nOnceNum = nOnceMaxNum;
	INT32 nSendNum = 0;

	TListPVOIDItor begin, end;
	begin = tSendList.begin();	end = tSendList.end();

	while(nSendNum < tSendList.size())
	{
		nOnceNum = (((tSendList.size() - nSendNum) > nOnceMaxNum && nOnceMaxNum > 0) ? nOnceMaxNum : (tSendList.size() - nSendNum));
		m_tMutex.Lock();
		SendToken.Clear();
		SendToken.TokenAdd_32(nOnceNum);
		for(begin; begin != end && nOnceNum; begin++)
		{
			t_ManageLogRs->SetPkt((PDB_LOG_RS)(*begin), SendToken);

			nSendNum += 1;
			nOnceNum -= 1;
		}
		SendData_Mgr(G_TYPE_LOG_RS, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	return;
}
//---------------------------------------------------------------------------
