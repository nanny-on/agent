/*
 * Copyright (C) 2021-2026 ASHINi Corp. 
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; either 
 * version 3 of the License, or any later version, with the additional
 * special exception to link portions of this program with the OpenSSL
 * library. See LICENSE for more details. 
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

// ThreadEvtMon.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadTimer.h"
#include "ThreadEvtMon.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadEvtMon

CThreadEvtMon*	t_ThreadEvtMon = NULL;

CThreadEvtMon::CThreadEvtMon()
{
	m_nTimerThreadId = 0;
	m_nSocketThreadId = 0;
}

CThreadEvtMon::~CThreadEvtMon()
{
}

BOOL CThreadEvtMon::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadEvtMon::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

INT32	CThreadEvtMon::SetThreadID(pthread_t nTimerThreadId, pthread_t nSocketThreadId)
{
	if(nTimerThreadId == 0)
		return -1;

	if(nSocketThreadId == 0)
		return -2;

	m_nTimerThreadId = nTimerThreadId;
	m_nSocketThreadId = nSocketThreadId;
	return 0;
}

VOID	CThreadEvtMon::CheckThreadID()
{
	int nStatus = 0;
	BOOL bRetVal = FALSE;
	pthread_t nThreadId = 0;
	if(m_nTimerThreadId != 0)
	{
		nStatus = pthread_kill(m_nTimerThreadId, 0);
		if ( nStatus == ESRCH )
		{
			m_nTimerThreadId = 0;
			WriteLogE("Timer Thread ID[%d] not exist.. : [%s]", m_nTimerThreadId, m_strThreadName.c_str());
			if(t_ThreadTimer != NULL)
			{
				nThreadId = 0;
				bRetVal = t_ThreadTimer->CreateThreadExt("timer", &nThreadId);
				if(bRetVal == FALSE || nThreadId == 0)
				{
					WriteLogE("start [timer] thread result : fail [%d]", errno);
				}
				else
				{
					m_nTimerThreadId = nThreadId;
					WriteLogN("restart [timer] thread result : success [%ld]", nThreadId);
				}
			}
		}
	}
	if(m_nSocketThreadId != 0)
	{
		nStatus = pthread_kill(m_nSocketThreadId, 0);
		if ( nStatus == ESRCH )
		{
			m_nSocketThreadId = 0;
			WriteLogE("Socket Thread ID[%d] not exist.. : [%s]", m_nSocketThreadId, m_strThreadName.c_str());
			if(t_ThreadSocket != NULL)
			{
				nThreadId = 0;
				bRetVal = t_ThreadSocket->CreateThreadExt("socket", &nThreadId);
				if(bRetVal == FALSE || nThreadId == 0)
				{
					WriteLogE("start [socket] thread result : fail [%d]", errno);
				}
				else
				{
					m_nSocketThreadId = nThreadId;
					WriteLogN("restart [socket] thread result : success [%ld]", nThreadId);
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CThreadEvtMon message handlers

int CThreadEvtMon::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);
	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{
			ChkEvtTime();
		}
//		CheckThreadID();
		t_TimerUtil.MonitorTimer();
		Sleep(1000);
	}

	if(!t_EnvInfoOp)
		WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())
		WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())
		WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	return 0;
}
//------------------------------------------------------------------------------

INT32	CThreadEvtMon::ChkEvtTime()
{
	TListMemEvtMon tEvtMonList;
	t_ManageEvtMon.GetOverTimeEvtMon(tEvtMonList);
	TListMemEvtMonItor begin, end;
	begin = tEvtMonList.begin();	end = tEvtMonList.end();
	for(begin; begin != end; begin++)
	{
		WriteLogN("[%s] detect over time evt : [%d]:[%d][%s][%d][%d] : %llu", m_strThreadName.c_str(), begin->nID, begin->nClass, begin->strName.c_str(), begin->nType, begin->nCode, begin->nThreadID);
	}
	return 0;
}
//------------------------------------------------------------------------------