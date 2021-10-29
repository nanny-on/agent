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
#include "LogicMgrUser.h"

//---------------------------------------------------------------------------

CLogicMgrUser*		t_LogicMgrUser = NULL;

//---------------------------------------------------------------------------

CLogicMgrUser::CLogicMgrUser()
{
	t_ManageUser	= new CManageUser();
	t_LogicUser		= new CLogicUser();

	t_ManageUser->LoadDBMS();
	t_ManageUser->InitHash();


	m_strLogicName		= "mgr user";

	m_nControlType	= SS_CONTROL_TYPE_ORG_USER;	
	m_nEvtObjType	= EVENT_OBJECT_TYPE_USER;
	m_nEvtObjCode	= 0;
}
//---------------------------------------------------------------------------

CLogicMgrUser::~CLogicMgrUser()
{
	SAFE_DELETE(t_ManageUser);
	SAFE_DELETE(t_LogicUser);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrUser::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_END:			nRtn = AnalyzePkt_FromMgr_Ext_Logoff();		break;
		case G_CODE_COMMON_AUTH:		nRtn = AnalyzePkt_FromMgr_Ext_Auth();		break;
		case G_CODE_COMMON_CHANGE:		nRtn = AnalyzePkt_FromMgr_Ext_ChgPW();		break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrUser::AnalyzePkt_FromMgr_Add_Ext()
{
	PDB_USER pdu = NULL;
	DB_USER du;


	if( t_ManageUser->GetPkt(RecvToken, du))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	{
		if(SetER(t_ManageUser->ApplyUser(du)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply user information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}

		WriteLogE("[%s] add user info ", m_strLogicName.c_str());
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrUser::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_USER pdu = NULL;
	DB_USER du;
	
	if( t_ManageUser->GetPkt(RecvToken, du))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	{
		if(SetER(t_ManageUser->ApplyUser(du)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply user information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrUser::AnalyzePkt_FromMgr_Del_Ext()
{
	PDB_USER pdu = NULL;
	
	if(!RecvToken.TokenDel_32(m_nRecvID))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	if(t_ManageUser->FindItem(m_nRecvID) == NULL)
	{
		return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
	}
	
	{
		if(SetER(t_ManageUser->DelUser(m_nRecvID)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] delete user information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
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

INT32		CLogicMgrUser::AnalyzePkt_FromMgr_Ext_Auth()
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_CONNECT;

	DB_USER du;
	PDB_LOCAL_ENV_AUTH pdlea = NULL;

	if(CheckPktResult(RecvToken))				goto SEND_PKT;
	if(t_ManageUser->GetPkt(RecvToken, du))
	{
		m_nPktRst = ERR_SOCKET_SVR_TO_CLT_INVALID_PKT;
		goto SEND_PKT;
	}

	if((pdlea = t_ManageLocalEnvAuth->FirstItem()) == NULL)
	{
		m_nPktRst = ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		goto SEND_PKT;
	}
	
	{
		pdlea->strUniCode	= du.strUniCode;
		pdlea->strAccountID = du.strAccountID;
		pdlea->strAccountPW = du.strAccountPW;
		pdlea->nEvtTime		= GetCurrentDateTimeInt();

		if(SetER(t_ManageLocalEnvAuth->EditLocalEnvAuth(*pdlea)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] edit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			m_nPktRst = ERR_DBMS_UPDATE_FAIL;
		}

		t_LogicLocalEnvAuth->SendPkt_LocalEnvAuth_Edit();

		WriteLogN("[%s] edit local_env_auth info login", m_strLogicName.c_str());
	}

	{
		t_LogicMgrHost->SendPkt_Edit();
	}

	{
		PDB_HOST pdh = t_ManageHost->FirstItem();
		if(!pdh)
		{
			WriteLogE("[%s] not find host first item..", m_strLogicName.c_str());
			goto SEND_PKT;
		}

		InitDLEU(m_nEvtOpType, EVENT_OBJECT_TYPE_USER, 0, du.nID, du.strAccountID,  m_strEvtDesc);
		t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
	}

	goto SEND_PKT;

SEND_PKT:	
	t_LogicUser->SendPkt_Auth(m_nPktRst, du);
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrUser::AnalyzePkt_FromMgr_Ext_Logoff()
{
	if(CheckPktResult(RecvToken))				goto SEND_PKT;
	if(!RecvToken.TokenDel_32(m_nRecvID))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	
	if(!SetUserLogout(m_nRecvID))
		goto SEND_PKT;

SEND_PKT:
	t_LogicUser->SendPkt_Logout(m_nRecvID);
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrUser::AnalyzePkt_FromMgr_Ext_ChgPW()
{
	DB_USER du;
	PDB_LOCAL_ENV_AUTH pdlea = NULL;

	if(CheckPktResult(RecvToken))				goto SEND_PKT;
	if(t_ManageUser->GetPkt(RecvToken, du))
	{
		m_nPktRst = ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		goto SEND_PKT;
	}

	{
		if(SetER(t_ManageUser->ApplyUser(du)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply user information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			m_nPktRst = ERR_DBMS_UPDATE_FAIL;
		}

		WriteLogN("[%s] edit user info ", m_strLogicName.c_str());
	}

	if((pdlea = t_ManageLocalEnvAuth->FirstItem()) == NULL)
	{
		m_nPktRst = ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		goto SEND_PKT;
	}

	{
		pdlea->strAccountPW = du.strAccountPW;

		if(SetER(t_ManageLocalEnvAuth->EditLocalEnvAuth(*pdlea)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] edit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			m_nPktRst = ERR_DBMS_UPDATE_FAIL;
		}

		t_LogicLocalEnvAuth->SendPkt_LocalEnvAuth_Edit();

		WriteLogN("[%s] edit local_env_auth info login", m_strLogicName.c_str());
	}

SEND_PKT:
	t_LogicUser->SendPkt_UserChgPW(m_nPktRst, du);
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrUser::SendPkt_Auth(PDB_USER pdu)
{
	m_tMutex.Lock();
	SendToken.Clear();
	t_ManageUser->SetPkt(pdu, SendToken);
	SendData_Mgr(G_TYPE_USER, G_CODE_COMMON_AUTH, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrUser::SendPkt_ChgPW(String strAccountID, String strPassword)
{
	m_tMutex.Lock();
	SendToken.Clear();
	SendToken.TokenAdd_String(strAccountID);
	SendToken.TokenAdd_String(strPassword);
	SendData_Mgr(G_TYPE_USER, G_CODE_COMMON_CHANGE, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
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

INT32		CLogicMgrUser::ApplyPolicy()
{
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

INT32		CLogicMgrUser::OnTimer_Logic()
{
	return 0;	
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrUser::SetUserLogout(INT32 nExtOption)
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_DISCONNECT;

	String strAccountID;
	PDB_LOCAL_ENV_AUTH pdlea = NULL;
	DB_LOCAL_ENV_AUTH dlea;
	pdlea = t_ManageLocalEnvAuth->FirstItem();
	if(pdlea)	
	{
		dlea = *pdlea;
		strAccountID = pdlea->strAccountID;
	}

	{
		dlea.strAccountID	= "";
		dlea.strAccountPW	= "";
		dlea.strUniCode		= "";
		dlea.nEvtTime		= GetCurrentDateTimeInt();

		if(SetER(t_ManageLocalEnvAuth->EditLocalEnvAuth(dlea)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] edit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return 0;
		}

		t_LogicLocalEnvAuth->SendPkt_LocalEnvAuth_Edit();

		WriteLogN("[%s] edit local_env_auth info logout", m_strLogicName.c_str());
	}

	{
		t_LogicMgrHost->SendPkt_Edit();
	}

	{
		nExtOption == SS_HOST_OPTION_TYPE_LOGOFF_ADMIN ? m_nEvtSubType = EVENT_SUBJECT_TYPE_ADMIN : m_nEvtSubType = EVENT_SUBJECT_TYPE_USER;
		PDB_HOST pdh = t_ManageHost->FirstItem();
		if(!pdh)
		{
			WriteLogE("[%s] not find host first item..", m_strLogicName.c_str());
			return 0;
		}
		PDB_USER pdu = t_ManageUser->FindUserAccountID(strAccountID);
		if(!pdu)
		{
			WriteLogE("[%s] not find user item..", m_strLogicName.c_str());
			return 0;
		}

		InitDLEU(m_nEvtOpType, EVENT_OBJECT_TYPE_USER, 0, pdh->nUserID, pdu->strAccountID,  m_strEvtDesc);
		t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
