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
#include "LogicMgrLogSecu.h"

//---------------------------------------------------------------------------

CLogicMgrLogSecu*		t_LogicMgrLogSecu = NULL;

//---------------------------------------------------------------------------

CLogicMgrLogSecu::CLogicMgrLogSecu()
{	
	t_ManageLogSecu	= new CManageLogSecu();
	t_LogicLogSecu	= new CLogicLogSecu();

	t_ManageLogSecu->LoadDBMS();

	m_strLogicName		= "logic mgr log secu";

	m_nControlType		= SS_CONTROL_TYPE_LOG_SECU;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode		= 0;
}
//---------------------------------------------------------------------------

CLogicMgrLogSecu::~CLogicMgrLogSecu()
{
	SAFE_DELETE(t_LogicLogSecu);
	SAFE_DELETE(t_ManageLogSecu);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrLogSecu::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrLogSecu::AnalyzePkt_FromMgr_Add_Ext()
{
	DB_LOG_SECU dls;

	if(t_ManageLogSecu->GetPkt(RecvToken, dls))		return AZPKT_CB_RTN_PKT_INVALID;

	dls.nEvtTime	= t_ValidTimeUtil->GetValidTime();
	HISYNCSTEPUP(dls.nSyncSvrStep);

	SetLogSecu(dls);

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogSecu::AnalyzePkt_FromMgr_Edit_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogSecu::AnalyzePkt_FromMgr_Del_Ext()
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

INT32		CLogicMgrLogSecu::AnalyzePkt_FromMgr_Ext_Sync()
{	
	TListDBLogSecu tDBLogSecuList;

	if(CheckPktResult(RecvToken))					return AZPKT_CB_RTN_RESULT_FAILED;
	if(!RecvToken.TokenDel_32(m_nRecvNum))			return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		DB_LOG_SECU tDLS;
		if(t_ManageLogSecu->GetPkt(RecvToken, tDLS))		return AZPKT_CB_RTN_PKT_INVALID;

		tDBLogSecuList.push_back(tDLS);
	}

	{
		TListDBLogSecuItor begin, end;
		begin = tDBLogSecuList.begin();	end = tDBLogSecuList.end();
		for(begin; begin != end; begin++)
		{
			t_ManageLogSecu->EditLogSecu(*begin);
		}
	}
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrLogSecu::SetLogSecu(DB_LOG_SECU& dls)
{
	{
		if(dls.strObjectPath.find("%") == string::npos && dls.strObjectName.find("%") == string::npos &&
			dls.strSubjectPath.find("%") == string::npos && dls.strSubjectName.find("%") == string::npos)
		{
			WriteLogN("[%s] remain evt log to file : pt[%d]:bt[%d]:ot:[%d]:sb[%s/%s]:ob[%s/%s]", 
							m_strLogicName.c_str(),
							dls.nPolicyType, dls.nBlockType, dls.nObjectType, 
							dls.strSubjectPath.c_str(), dls.strSubjectName.c_str(), 
							dls.strObjectPath.c_str(), dls.strObjectName.c_str());

		}
	}

	{
		dls.nUserID = t_ManageHost->GetUserID(t_ManageHost->FirstID());
	}

	{
		PDB_ENV_LOG_UNIT pDELEU = t_ManageEnvLogUnit->FindRecordLogSecuUnit(&dls);
		if(pDELEU && pDELEU->tDPH.nUsedMode == STATUS_USED_MODE_ON)
		{
			dls.nSkipTarget = pDELEU->tDPH.nExtOption;
		}
	}
	
	if(!(dls.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT))
	{
		t_ManageLogSecu->AddLogSecu(dls);
	}

	{
		m_tMutex.Lock();
		SendToken.Set(1024);
		SendToken.TokenAdd_32(1);
		t_ManageLogSecu->SetPkt(&dls, SendToken);
		if(!(dls.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER))
		{
			SendData_Mgr(G_TYPE_LOG_SECU, G_CODE_COMMON_SYNC, SendToken);
		}
		if(!(dls.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT))		
		{
			SendData_Link(G_TYPE_LOG_SECU, G_CODE_COMMON_SYNC, SendToken);
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

void		CLogicMgrLogSecu::SendPkt_Sync(INT32 nOnceMaxNum)
{
	TListPVOID tSendList;
	{
		t_ManageLogSecu->SetPktSync(tSendList);

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
			t_ManageLogSecu->SetPkt((PDB_LOG_SECU)(*begin), SendToken);

			nSendNum += 1;
			nOnceNum -= 1;
		}
		SendData_Mgr(G_TYPE_LOG_SECU, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	return;
}
//---------------------------------------------------------------------------
