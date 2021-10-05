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

// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "fanotify_thread.h"
#include "fanotify_thread_event.h"
#include "fanotify_dlg.h"


typedef queue<MSG*> MonMsgQueue;

static MonMsgQueue g_MonMsgQueue;
static pthread_mutex_t g_mon_mutex;
static pthread_mutex_t g_log_mutex;
static pthread_cond_t g_mon_cond;

CFaNotifyDlg* t_FaNotifydDlg = NULL;

char g_acLogBuf[CHAR_MAX_SIZE];

BOOL CFaNotifyDlg::GetMessage(MSG* pMsg)
{
	MSG* pGetMsg = NULL;
	BOOL bRetVal = TRUE;

	do{
		if(pMsg == NULL)
		{
			bRetVal = FALSE;
			break;
		}
	
		pthread_mutex_lock(&g_mon_mutex);
		if(g_MonMsgQueue.empty())
		{
			pthread_cond_wait(&g_mon_cond, &g_mon_mutex);
		}
		pGetMsg = g_MonMsgQueue.front();
		g_MonMsgQueue.pop();
		pthread_mutex_unlock(&g_mon_mutex);
		if(pGetMsg == NULL)
		{
			bRetVal = FALSE;
			break;
		}
		memcpy(pMsg, pGetMsg, sizeof(MSG));
		delete pGetMsg;
		if(pMsg->message == WM_FANOTIFYD_CLOSE)
		{
			bRetVal = FALSE;
			break;
		}
		bRetVal = TRUE;
	}while(FALSE);

	return bRetVal;
}

BOOL CFaNotifyDlg::PostMessage(UINT dwMsgID, WPARAM wParam, LPARAM lParam)
{
	MSG* pMsg = new MSG;
	if(pMsg == NULL)
		return FALSE;
	pMsg->message = dwMsgID;
	pMsg->wParam = wParam;
	pMsg->lParam = lParam;
	pthread_mutex_lock(&g_mon_mutex);
	g_MonMsgQueue.push(pMsg);
	pthread_cond_signal(&g_mon_cond);
	pthread_mutex_unlock(&g_mon_mutex);
	return TRUE;
}

CFaNotifyDlg::CFaNotifyDlg()
{
    m_nCloseMode = 0;
	m_nFileLogRetention = 5;
	memset(m_acRootPath, 0, MAX_PATH);
	m_dwThreadID = (DWORD)getpid();
    pthread_mutex_init(&g_mon_mutex, NULL);
	pthread_mutex_init(&g_log_mutex, NULL);
    pthread_cond_init(&g_mon_cond, NULL);
	memset(g_acLogBuf, 0, CHAR_MAX_SIZE);
	this->PostMessage(WM_FANOTIFYD_INITIALIZE, 0, 0);    
}

CFaNotifyDlg::~CFaNotifyDlg()
{
	MSG* pMsg = NULL;
	while(!g_MonMsgQueue.empty())
	{
		pMsg = g_MonMsgQueue.front();
		if(pMsg)
			delete pMsg;
		g_MonMsgQueue.pop();
	}
	pthread_cond_destroy(&g_mon_cond);
	pthread_mutex_destroy(&g_mon_mutex);
	pthread_mutex_destroy(&g_log_mutex);
}

void CFaNotifyDlg::WriteLogN(char *fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };

	pthread_mutex_lock(&g_log_mutex);
	do{
		GetCurrentDateTime(0, acTimeBuf);

		if(m_acRootPath[0] == 0)
		{
			if(get_nanny_agent_root(m_acRootPath, MAX_PATH) != 0)
				break;
		}

		snprintf(acSaveFile, MAX_PATH-1, "%s/nanny/log/fanotifyd_log_%s.txt", m_acRootPath, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			snprintf(acLogBuf, CHAR_MAX_SIZE-1, "%s/nanny/log", m_acRootPath);
			ClearOldLogFile(acLogBuf, "fanotifyd_log_", m_nFileLogRetention);
			memset(acLogBuf, 0, CHAR_MAX_SIZE);
		}

		fp = fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}

		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);

		if(_stricmp(acLogBuf, g_acLogBuf))
		{
			strncpy(g_acLogBuf, acLogBuf, CHAR_MAX_SIZE-1);
			g_acLogBuf[CHAR_MAX_SIZE-1] = 0;
			fprintf(fp, "%s\t[Info]\t%s\n", acTimeBuf, acLogBuf);
		}
		fclose(fp);
	}while(FALSE);
	pthread_mutex_unlock(&g_log_mutex);
}

void CFaNotifyDlg::WriteLogE(char *fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };

	pthread_mutex_lock(&g_log_mutex);
	do{
		GetCurrentDateTime(0, acTimeBuf);

		if(m_acRootPath[0] == 0)
		{
			if(get_nanny_agent_root(m_acRootPath, MAX_PATH) != 0)
				break;
		}

		snprintf(acSaveFile, MAX_PATH-1, "%s/nanny/log/fanotifyd_log_%s.txt", m_acRootPath, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			snprintf(acLogBuf, CHAR_MAX_SIZE-1, "%s/nanny/log", m_acRootPath);
			ClearOldLogFile(acLogBuf, "fanotifyd_log_", m_nFileLogRetention);
			memset(acLogBuf, 0, CHAR_MAX_SIZE);
		}

		fp = fopen(acSaveFile, "at");
		if(fp == NULL)
		{
			break;
		}

		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acLogBuf, CHAR_MAX_SIZE - 1, fmt, args);		
		va_end(args);
		if(_stricmp(acLogBuf, g_acLogBuf))
		{
			strncpy(g_acLogBuf, acLogBuf, CHAR_MAX_SIZE-1);
			g_acLogBuf[CHAR_MAX_SIZE-1] = 0;
			fprintf(fp, "%s\t[Error]\t%s\n", acTimeBuf, acLogBuf);
		}
		fclose(fp);
	}while(FALSE);
	pthread_mutex_unlock(&g_log_mutex);
}

INT32		CFaNotifyDlg::CreateSubClass()
{
	t_MonThreadTimer = new CMonThreadTimer();
	if(t_MonThreadTimer == NULL)
	{
		return -1;
	}

	t_ThreadFaNotifyEvent = new CThreadFaNotifyEvent();
	if(t_ThreadFaNotifyEvent == NULL)
	{
		SAFE_DELETE(t_MonThreadTimer);
		return -2;
	}
	t_ThreadFaNotifyEvent->SetNotifyWnd((HWND)this);
	return 0;
}


INT32		CFaNotifyDlg::InitTimerID()
{
	t_MonThreadTimer->t_MonTimerUtil.SetNotifyWnd((HWND)this, (UINT32)WM_FANOTIFYD_TIMER_EVT);

	t_MonThreadTimer->t_MonTimerUtil.AddTimer(TIMER_ID_FANOTIFY_ALIVE_MSG, TIMER_INTERVAL_TIME_SEC*5, NULL, 1, 0);

	return 0;
}



INT32		CFaNotifyDlg::StartSubClass()
{
	pthread_t tid = 0;
	BOOL bRetVal = FALSE;
	bRetVal = t_MonThreadTimer->CreateThreadExt("timer", &tid);
	if(bRetVal == FALSE)
	{
		WriteLogE("start [timer] thread result : fail [%d]", errno);
		return -1;
	}
	Sleep(10);
	tid = 0;
	bRetVal = t_ThreadFaNotifyEvent->CreateThreadExt("fanotify", &tid);
	if(bRetVal == FALSE)
	{
		WriteLogE("start [fanotify] thread result : fail [%d]", errno);
		return -2;

	}
	Sleep(10);
	return 0;
}


void CFaNotifyDlg::WM_AgentInitialize(WPARAM wParam, LPARAM lParam)
{
	INT32 nRetVal = 0;
	do{
		nRetVal = CreateSubClass();
	    if(nRetVal != 0)
		{
			WriteLogE("fail to create sub class [%d]", nRetVal);
	        break;
		}
		WriteLogN("success to create sub class.");
		
		nRetVal = InitTimerID();
	    if(nRetVal != 0)
		{
			WriteLogE("fail to init timer id [%d]", nRetVal);
	        break;
		}
		WriteLogN("success to init timer id.");
		
		nRetVal = StartSubClass(); 
	    if(nRetVal != 0)
		{
			WriteLogE("fail to start sub class [%d]", nRetVal);
	        break;
		}
		WriteLogN("success to start fanotifyd service.");
		nRetVal = 0;
	}while(FALSE);

	if(nRetVal != 0)
	{
		CloseMain();
		PostMessage(WM_FANOTIFYD_CLOSE, 0, 0);
	}
}

INT32		CFaNotifyDlg::StopThread_Common(CThreadBase* tThreadObject, UINT32 nWaitTime)
{
	INT32 nLWaitTime = 0;
	if(tThreadObject == NULL)
		return 0;

	if(m_SemExt.CreateEvent() == FALSE)
		return -1;

	nLWaitTime = nWaitTime * 10;

	while(m_SemExt.WaitForSingleObject(100) == WAIT_TIMEOUT)
	{
		if(tThreadObject->GetContinue() == 0)
			break;
		if(!nLWaitTime)
			--nLWaitTime;
	}
	m_SemExt.CloseHandle(); 
	if(!nLWaitTime)
	{
		WriteLogN("thread terminate fail : over wait time [%d]", nWaitTime);
		return -10;
	}
	return 0;
}



VOID CFaNotifyDlg::StopSubClass()
{
	t_MonThreadTimer->SetContinue(0);
	SetER(StopThread_Common(t_MonThreadTimer));
	WriteLogN("stop [timer] thread result : [%d]", g_nErrRtn);

	t_ThreadFaNotifyEvent->SetContinue(0);
	SetER(StopThread_Common(t_ThreadFaNotifyEvent));
	WriteLogN("stop [fanotify] thread result : [%d]", g_nErrRtn);
}
//--------------------------------------------------------------------

VOID		CFaNotifyDlg::DeleteSubClass()
{
	SAFE_DELETE(t_MonThreadTimer);
	SAFE_DELETE(t_ThreadFaNotifyEvent);
}


//---------------------------------------------------------------------------

void CFaNotifyDlg::WM_MainThreadTerminated(WPARAM wParam, LPARAM lParam)
{
    if(m_nCloseMode)
		return;
    
	if(wParam)
    {
    	PostMessage(WM_FANOTIFYD_CLOSE, 0, 0);
    }
}
//---------------------------------------------------------------------------

void CFaNotifyDlg::WM_AgentAutoDestory(WPARAM wParam, LPARAM lParam)
{
	if(lParam != 0x00382364)
		return;
    this->PostMessage(WM_FANOTIFYD_CLOSE, 0, 0);
    WriteLogN("agent terminating by service");
}

void CFaNotifyDlg::CloseMain()
{
	if(m_nCloseMode)
		return;

	m_nCloseMode = 1;

	StopSubClass();
	WriteLogN("stop sub class");

	DeleteSubClass();
	WriteLogN("delete sub class"); 
}
//---------------------------------------------------------------------------

INT32 CFaNotifyDlg::SendAliveMessage()
{
	UINT32 dwGap = 0;
	UINT32 dwCurrentTime = 0;
	UINT32 dwSendTime = 0;
	INT32 nRetVal = 0;
	if(t_ThreadFaNotifyEvent == NULL)
	{
		WriteLogE("invalid notify thread addr");
		return -1;
	}
	dwSendTime = t_ThreadFaNotifyEvent->GetSendTime();
	dwCurrentTime = GetCurrentDateTimeInt();
	dwGap = dwCurrentTime - dwSendTime;
	if(dwGap < 3)
		return 0;

	nRetVal = t_ThreadFaNotifyEvent->SendAliveMessage();
	if(nRetVal < 0)
		WriteLogE("fail to send alive message (%d)", nRetVal);
	return nRetVal;
}

DWORD		CFaNotifyDlg::OnThreadTimer(WPARAM wParam, LPARAM lParam)
{	
	switch(wParam)
	{
		case TIMER_ID_FANOTIFY_ALIVE_MSG:
		{			
			t_MonThreadTimer->t_MonTimerUtil.DisableTimer(TIMER_ID_FANOTIFY_ALIVE_MSG);

			SendAliveMessage();

			t_MonThreadTimer->t_MonTimerUtil.EnableTimer(TIMER_ID_FANOTIFY_ALIVE_MSG);
			break;
		}
		default:
		{
			WriteLogE("not define timer id : [0x%.8X][%u]", wParam, wParam);
			break;
		}
	}
	return 0;
}



int CFaNotifyDlg::Run()
{
	MSG Msg = {0,};

	while (GetMessage(&Msg))
	{
		Msg.hwnd = 0;		
		switch(Msg.message)
		{
			case WM_FANOTIFYD_INITIALIZE:
				WM_AgentInitialize(Msg.wParam, Msg.lParam);
				break;
			case WM_FANOTIFYD_TIMER_EVT:
				OnThreadTimer(Msg.wParam, Msg.lParam);
				break;
			case WM_FANOTIFYD_TERMINATED:
				WM_AgentAutoDestory(Msg.wParam, Msg.lParam);
				break;
			case WM_FANOTIFYD_CLOSE:
				CloseMain();
				break;
		}
	}
	return 1;
}

