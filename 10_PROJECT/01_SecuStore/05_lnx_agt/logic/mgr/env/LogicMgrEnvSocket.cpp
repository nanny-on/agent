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
#include "LogicMgrEnvSocket.h"

//---------------------------------------------------------------------------

CLogicMgrEnvSocket*		t_LogicMgrEnvSocket = NULL;

//---------------------------------------------------------------------------

CLogicMgrEnvSocket::CLogicMgrEnvSocket()
{
	t_ManageEnvSocket	= new CManageEnvSocket();
    t_ManageEnvSocket->LoadDBMS();
	t_ManageEnvSocket->InitHash();

	m_strLogicName		= "logic env socket";
	
	m_nControlType		= SS_CONTROL_TYPE_ENV_SOCKET;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_ENV;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_ENV_SOCKET;
}
//---------------------------------------------------------------------------

CLogicMgrEnvSocket::~CLogicMgrEnvSocket()
{
	SAFE_DELETE(t_ManageEnvSocket);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvSocket::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvSocket::AnalyzePkt_FromMgr_Add_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvSocket::AnalyzePkt_FromMgr_Edit_Ext()
{
	TListDBEnvSocket	tUnitList;
	
	if(!RecvToken.TokenDel_32(m_nRecvNum))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_ENV_SOCKET des;

		if( t_ManageEnvSocket->GetPkt(RecvToken, des))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tUnitList.push_back(des);		
	}

	{
		TListDBEnvSocketItor begin, end;
		begin = tUnitList.begin();	end = tUnitList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageEnvSocket->ApplyEnvSocket(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply env information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}
	
		t_ManageEnvSocket->InitHash();
	}

	{
		PDB_ENV_SOCKET pdata = t_ManageEnvSocket->GetDefaultItem();
		if(pdata)
		{
			if(pdata->tDPH.nExtOption & SS_ENV_SOCKET_OPTION_FLAGE_USE_KEEPALIVE)
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_SEND_KEEP_ALIVE);
			else
				t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_SEND_KEEP_ALIVE);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrEnvSocket::OnTimer_Logic()
{
	Send_KeepAlive();
	return 0;
}
//---------------------------------------------------------------------------
INT32		CLogicMgrEnvSocket::Send_KeepAlive()
{
	INT32 nMgrSvrAuthStatus = CLIENT_CON_STATUS_DISCONNECTED;
	t_EnvInfoOp->GetMgrSvrAuthStatus(nMgrSvrAuthStatus);
	if(nMgrSvrAuthStatus == CLIENT_CON_STATUS_CONNECTED)
	{
		m_tMutex.Lock();
		SendToken.Clear();
		SendToken.TokenAdd_32(1);
		SendData_Mgr(G_TYPE_ENV_SOCKET, G_CODE_COMMON_KEEPALIVE, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		/*WriteLogN("send keep alive packet");*/
	}

	return 0;
}
//---------------------------------------------------------------------------



