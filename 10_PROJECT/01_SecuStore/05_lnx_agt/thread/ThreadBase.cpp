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

// ThreadBase.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadBase.h"


/////////////////////////////////////////////////////////////////////////////
// CThreadBase

PVOID CThreadBase::ThreadWorker(LPVOID lParam)
{
	int nRetVal = 0;
	CThreadBase* pThread = (CThreadBase *)lParam;
	if(pThread == NULL)
	{
		return (PVOID)NULL;
	}
	while(pThread->m_nState != STATE_EXIT)
	{	
		switch(pThread->m_nState)
		{
			case STATE_SUSPEND:
				pThread->WaitToResume(3600*1000);
				break;
			case STATE_RESUME:
			case STATE_RUN:
				nRetVal = pThread->Run();
				if(nRetVal == 0)
				{
					pThread->m_nState = STATE_EXIT;
				}
				break;
			default:
				Sleep(10);
				break;
		}
	}
	pThread->Finalize();
	pThread->ExitInstance();
	return (PVOID)NULL;
}


CThreadBase::CThreadBase()
{
	m_bInit = FALSE;
	m_nPause = 0;
	m_nState = STATE_RESUME;
	m_nRunFlag = 1;
	m_nContinue = 1;
	m_pOwnerClass = NULL;
    pthread_mutex_init(&m_ExitMutex, NULL);
	pthread_cond_init(&m_ExitCond, NULL);
    pthread_mutex_init(&m_ResumeMutex, NULL);
	pthread_cond_init(&m_ResumeCond, NULL);
}

CThreadBase::~CThreadBase()
{
	if(m_hThread != NULL)
	{
		TerminateThread();
	}
	pthread_cond_destroy(&m_ExitCond);
	pthread_mutex_destroy(&m_ExitMutex);
	pthread_cond_destroy(&m_ResumeCond);
	pthread_mutex_destroy(&m_ResumeMutex);
}

int CThreadBase::ExitInstance()
{
	pthread_cond_signal(&m_ExitCond);
	return 0;
}


BOOL CThreadBase::CreateThreadExt(String strName, pthread_t *pnThreadID, BOOL bCreateSuspended)
{
	BOOL bRetVal = TRUE;
	pthread_attr_t attr = {0,};
	pthread_t tid = 0;

	m_strThreadName = strName;

	if(bCreateSuspended == TRUE)
	{
		m_nState = STATE_SUSPEND;
		m_nPause = 1;
	}
	else
	{
		m_nState = STATE_RESUME;
		m_nPause = 0;
	}

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(pthread_create(&tid, &attr, ThreadWorker, (void*)this) == 0)
	{
		m_hThread = (HANDLE)tid;
		if(pnThreadID != NULL)
			*pnThreadID = tid;
		m_dwThreadID = (DWORD)tid;
		bRetVal = TRUE;
	}
	else
	{
		m_hThread = NULL;
		m_dwThreadID = 0;
		bRetVal = FALSE;
	}
	pthread_attr_destroy(&attr);

	return bRetVal;
}

int CThreadBase::WaitToExit(DWORD dwMilliSecs)
{
	struct timespec ts;
	int nRetVal = 0;
	
	if(m_dwThreadID == 0)
		return (DWORD)(-1);

	if(m_nState == STATE_EXIT)
		return 0;

    if(m_nState == STATE_SUSPEND)
	{
        ResumeThread();
	}

	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += (dwMilliSecs)/1000;
	ts.tv_nsec += (dwMilliSecs%1000)*1000*1000;;
	m_nState = STATE_EXIT;
	pthread_mutex_lock(&m_ExitMutex);
	nRetVal = pthread_cond_timedwait(&m_ExitCond, &m_ExitMutex, &ts);
	pthread_mutex_unlock(&m_ExitMutex);
	return nRetVal;
}

int CThreadBase::WaitToResume(DWORD dwMilliSecs)
{
    struct timespec ts;
	int nRetVal = 0;
	if(m_dwThreadID == 0)
    	return -1;

	if(m_nState != STATE_SUSPEND)
		return 0;

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += (dwMilliSecs)/1000;
    ts.tv_nsec += (dwMilliSecs%1000)*1000*1000;;

    pthread_mutex_lock(&m_ResumeMutex);
    nRetVal = pthread_cond_timedwait(&m_ResumeCond, &m_ResumeMutex, &ts);
    pthread_mutex_unlock(&m_ResumeMutex);
    return nRetVal;
}


/////////////////////////////////////////////////////////////////////////////
// CThreadBase message handlers

//----------------------------------------------------------------------------

VOID CThreadBase::ResumeThread()
{
	if(m_bInit == FALSE)
	{
		InitInstance();
		m_bInit = TRUE;
	}
	m_nState = STATE_RESUME;
	pthread_cond_signal(&m_ResumeCond);
}

VOID CThreadBase::SuspendThread()
{
	m_nState = STATE_SUSPEND;
}

VOID CThreadBase::TerminateThread()
{
	if(m_dwThreadID)
	{
    	if(m_nState == STATE_SUSPEND)
		{
			ResumeThread();
		}
		WaitToExit(5000);
	}

	m_hThread = NULL;
	m_dwThreadID = 0;
}

//----------------------------------------------------------------------------
