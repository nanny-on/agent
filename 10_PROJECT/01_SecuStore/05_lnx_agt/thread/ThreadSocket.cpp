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

// ThreadSocket.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadSocket

CThreadSocket*	t_ThreadSocket = NULL;

CThreadSocket::CThreadSocket()
{
	m_nCtlLgnStatus		= 0;
	m_nCtlMgrStatus		= 0;
	m_nCtlUdtStatus		= 0;
	m_nSvrLinkStatus	= 0;
}

CThreadSocket::~CThreadSocket()
{
}

BOOL CThreadSocket::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadSocket::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadSocket message handlers

int CThreadSocket::Run() 
{
	m_nRunFlag = 1;
	INT32		nOncePktNum = 0;
	INT32		nTotalPktOp = 0;	

	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);
	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{
			t_TimerUtil.ChkTimer();

			nOncePktNum = 1000;
			nTotalPktOp = 0;				

			if(CheckLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN))
				Logic_CltLgn(nTotalPktOp, nOncePktNum);
			if(CheckLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR))
				Logic_CltMgr(nTotalPktOp, nOncePktNum);
			if(CheckLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_UDT))
				Logic_CltUdt(nTotalPktOp, nOncePktNum);
			if(CheckLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_SVR_LINK))
				Logic_SvrLink(nTotalPktOp, nOncePktNum);

			if(nTotalPktOp == 0)
				Sleep(10);
		}	
		else
		{
			Sleep(10);
		}
	}

	if(!t_EnvInfoOp)
		WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())
		WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())
		WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadSocket::SetLogicStatus(UINT32 nType, UINT32 nStatus, UINT32 nWaitTime)
{
	LONG nCurStatus = 0;
	switch(nType)
	{
		case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN:
			nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nCtlLgnStatus, nStatus);
			break;
		case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR:
			nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nCtlMgrStatus, nStatus);
			break;
		case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_UDT:
			nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nCtlUdtStatus, nStatus);
			break;
		case SSA_SOCKET_THREAD_LOGIC_TYPE_SVR_LINK:
			nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nSvrLinkStatus, nStatus);
			break;
	}

	{
		if(m_SemExt.CreateEvent() == FALSE)
			return -1;
		INT32 nLWaitTime = nWaitTime * 10;

		while(m_SemExt.WaitForSingleObject(100) == WAIT_TIMEOUT)
		{
			switch(nType)
			{
				case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN:
					nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nCtlLgnStatus, 0);
					break;
				case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR:
					nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nCtlMgrStatus, 0);
					break;
				case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_UDT:
					nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nCtlUdtStatus, 0);
					break;
				case SSA_SOCKET_THREAD_LOGIC_TYPE_SVR_LINK:
					nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nSvrLinkStatus, 0);
					break;
			}

			if((nCurStatus & nStatus) == 0)	
				break;
			if(!nLWaitTime)
				break;
			--nLWaitTime;
		}
		m_SemExt.CloseHandle(); 
	}
	if(nStatus == SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE)
	{
		switch(nType)
		{
			case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN:		
				t_LgnCltUtil->CloseSocket_CLD(1);
				break;
			case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR:
				t_MgrCltUtil->CloseSocket_CLD(1);
				break;
			case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_UDT:
				t_UdtCltUtil->CloseSocket_CLD(1);
				break;
			case SSA_SOCKET_THREAD_LOGIC_TYPE_SVR_LINK:
				t_LinkSvrUtil->StopServer(1);
				t_LinkSvrUtil->CloseSocket(0, 0);				
				break;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

LONG		CThreadSocket::CheckLogicStatus(UINT32 nType)
{
	LONG nCurStatus = 0;
	switch(nType)
	{
		case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN:
			nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nCtlLgnStatus, 0);
			break;
		case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR:
			nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nCtlMgrStatus, 0);
			break;
		case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_UDT:
			nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nCtlUdtStatus, 0);
			break;
		case SSA_SOCKET_THREAD_LOGIC_TYPE_SVR_LINK:
			nCurStatus = m_StatusMutex.InterlockedExchangeAdd(&m_nSvrLinkStatus, 0);
			break;
	}

	if(nCurStatus & SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_ENABLE)
	{
		switch(nType)
		{
			case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN:
				m_StatusMutex.InterlockedExchange(&m_nCtlLgnStatus, 1);
				break;
			case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR:
				m_StatusMutex.InterlockedExchange(&m_nCtlMgrStatus, 1);
				break;
			case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_UDT:
				m_StatusMutex.InterlockedExchange(&m_nCtlUdtStatus, 1);
				break;
			case SSA_SOCKET_THREAD_LOGIC_TYPE_SVR_LINK:
				m_StatusMutex.InterlockedExchange(&m_nSvrLinkStatus, 1);
				break;
		}
		nCurStatus = 1;
	}
	else if(nCurStatus & SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_DISABLE)
	{
		switch(nType)
		{
			case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_LGN:
				m_StatusMutex.InterlockedExchange(&m_nCtlLgnStatus, 0);
				break;
			case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_MGR:
				m_StatusMutex.InterlockedExchange(&m_nCtlMgrStatus, 0);
				break;
			case SSA_SOCKET_THREAD_LOGIC_TYPE_CTL_UDT:
				m_StatusMutex.InterlockedExchange(&m_nCtlUdtStatus, 0);
				break;
			case SSA_SOCKET_THREAD_LOGIC_TYPE_SVR_LINK:
				m_StatusMutex.InterlockedExchange(&m_nSvrLinkStatus, 0);
				break;
		}
		nCurStatus = 0;
	}

	return nCurStatus;
}
//---------------------------------------------------------------------------
