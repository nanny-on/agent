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
#include "LogicUser.h"

//---------------------------------------------------------------------------

CLogicUser*		t_LogicUser = NULL;

//---------------------------------------------------------------------------

CLogicUser::CLogicUser()
{	
	m_strLogicName		= "link user";
		
	m_nEvtObjType		= 0;
	m_nEvtObjCode		= 0;
}
//---------------------------------------------------------------------------

CLogicUser::~CLogicUser()
{

}
//---------------------------------------------------------------------------

INT32	CLogicUser::AnalyzePkt_FromLink_User(PPKT_DATA pkt_data)
{
	InitBaseMember(pkt_data);
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_AUTH:		AnalyzePkt_FromLink_Ext_Auth();		break;
		case G_CODE_COMMON_END:			AnalyzePkt_FromLink_Ext_End();		break;
		case G_CODE_COMMON_CHANGE:		AnalyzePkt_FromLink_Ext_ChgPW();	break;;
		default:								break;		
	}
	InitBaseMember();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicUser::AnalyzePkt_FromLink_Ext_Auth()
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_LOGIN;

	DB_USER du;

	if(t_ManageUser->GetPkt(RecvToken, du))		goto INVALID_PKT;

	WriteLogN("[%s] receive login user auth info link data ", m_strLogicName.c_str());

	t_LogicMgrUser->SendPkt_Auth(&du);

	WriteLogN("[%s] send login user auth info mgr data ", m_strLogicName.c_str());

	goto OP_END;
INVALID_PKT:
	SendPkt_Auth(ERR_SOCKET_CLT_TO_SVR_INVALID_PKT, du);	

OP_END:
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicUser::AnalyzePkt_FromLink_Ext_End()
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_LOGOFF;

	String strAccountID;
	INT32 nMgrSvrAuthStatus = CLIENT_CON_STATUS_DISCONNECTED;
	PDB_LOCAL_ENV_AUTH pdlea = NULL;
	DB_LOCAL_ENV_AUTH dlea;
	pdlea = t_ManageLocalEnvAuth->FirstItem();
	m_nPktRst = ERR_SUCCESS;

	t_EnvInfoOp->GetMgrSvrAuthStatus(nMgrSvrAuthStatus);

	if(nMgrSvrAuthStatus == CLIENT_CON_STATUS_DISCONNECTED)
	{
		m_nPktRst = ERR_SOCKET_CONNECT_FAIL;
		goto INVALID_PKT;
	}

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
			goto SEND_PKT;
		}

		t_LogicLocalEnvAuth->SendPkt_LocalEnvAuth_Edit();

		WriteLogN("[%s] edit local_env_auth info logout", m_strLogicName.c_str());
	}

	{
		t_LogicMgrHost->SendPkt_Edit();

		SendToken.TokenAdd_32(m_nPktRst);
		SendToken.TokenAdd_32(SS_HOST_OPTION_TYPE_LOGOFF_USER);	
		WriteLogN("[%s] user logoff by user ", m_strLogicName.c_str());
	}

	{
		PDB_HOST pdh = t_ManageHost->FirstItem();
		if(!pdh)
		{
			WriteLogE("[%s] not find host first item..", m_strLogicName.c_str());
			goto SEND_PKT;
		}
		PDB_USER pdu = t_ManageUser->FindUserAccountID(strAccountID);
		if(!pdu)
		{
			WriteLogE("[%s] not find user item..", m_strLogicName.c_str());
			goto SEND_PKT;
		}

		InitDLEU(m_nEvtOpType, EVENT_OBJECT_TYPE_USER, 0, pdu->nID, pdu->strAccountID,  m_strEvtDesc);
		t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
	}

	goto SEND_PKT;

INVALID_PKT:
	SendToken.TokenAdd_32(m_nPktRst);
	SendToken.TokenAdd_32(SS_HOST_OPTION_TYPE_LOGOFF_NOT_USED);
	SendData_Link(m_tPktData, SendToken);
	return 0;

SEND_PKT:
	SendData_Link(m_tPktData, SendToken);
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicUser::AnalyzePkt_FromLink_Ext_ChgPW()
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_CHANGE;
	m_nPktRst	 = ERR_SOCKET_CLT_TO_SVR_INVALID_PKT;

	String	strAccount, strChangePw;
	INT32 nMgrSvrAuthStatus = CLIENT_CON_STATUS_DISCONNECTED;

	if( RecvToken.TokenDel_String(strAccount) < 0)			goto INVALID_PKT;
	if( RecvToken.TokenDel_String(strChangePw) < 0)			goto INVALID_PKT;

	t_EnvInfoOp->GetMgrSvrAuthStatus(nMgrSvrAuthStatus);
	if(nMgrSvrAuthStatus == CLIENT_CON_STATUS_DISCONNECTED)
	{
		m_nPktRst = ERR_SOCKET_CONNECT_FAIL;
		goto INVALID_PKT;
	}

	goto SEND_PKT;

INVALID_PKT:
	SendToken.TokenAdd_32(m_nPktRst);
	SendData_Link(m_tPktData, SendToken);
	return 0;

SEND_PKT:
	t_LogicMgrUser->SendPkt_ChgPW(strAccount, strChangePw);
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicUser::SendPkt_Auth(UINT32 nPktRtn, DB_USER& du)
{
	SendToken.TokenAdd_32(nPktRtn);
	t_ManageUser->SetPkt(&du, SendToken);
	SendData_Link(G_TYPE_USER, G_CODE_COMMON_AUTH, SendToken);
	SendToken.Clear();
	return;
}
//---------------------------------------------------------------------------

void		CLogicUser::SendPkt_Logout(UINT32 nPktRtn)
{
	SendToken.TokenAdd_32(m_nPktRst);
	SendToken.TokenAdd_32(nPktRtn);
	SendData_Link(G_TYPE_USER, G_CODE_COMMON_END, SendToken);
	SendToken.Clear();

	return;
}
//---------------------------------------------------------------------------

void		CLogicUser::SendPkt_UserChgPW(UINT32 nPktRtn, DB_USER& du)
{
	SendToken.TokenAdd_32(nPktRtn);
	t_ManageUser->SetPkt(&du, SendToken);
	SendData_Link(G_TYPE_USER, G_CODE_COMMON_CHANGE, SendToken);
	SendToken.Clear();
	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
