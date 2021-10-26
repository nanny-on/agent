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
#include "MainDlg.h"


typedef queue<MSG*> msgQueue;

static msgQueue g_appQueue;
static pthread_mutex_t g_mutex;
static pthread_cond_t g_cond;

CMainDlg* t_MainDlg;


BOOL CMainDlg::GetMessage(MSG* pMsg)
{
	MSG* pGetMsg = NULL;
	BOOL bRetVal = TRUE;

	do{
		if(pMsg == NULL)
		{
			bRetVal = FALSE;
			break;
		}
	
		pthread_mutex_lock(&g_mutex);
		if(g_appQueue.empty())
		{
			pthread_cond_wait(&g_cond, &g_mutex);
		}
		pGetMsg = g_appQueue.front();
		g_appQueue.pop();
		pthread_mutex_unlock(&g_mutex);
		if(pGetMsg == NULL)
		{
			bRetVal = FALSE;
			break;
		}
		memcpy(pMsg, pGetMsg, sizeof(MSG));
		delete pGetMsg;
		if(pMsg->message == WM_QUIT)
		{
			bRetVal = FALSE;
			break;
		}
		bRetVal = TRUE;
	}while(FALSE);

	return bRetVal;
}

BOOL CMainDlg::PostMessage(UINT dwMsgID, WPARAM wParam, LPARAM lParam)
{
	MSG* pMsg = new MSG;
	if(pMsg == NULL)
		return FALSE;
	pMsg->message = dwMsgID;
	pMsg->wParam = wParam;
	pMsg->lParam = lParam;
	pthread_mutex_lock(&g_mutex);
	g_appQueue.push(pMsg);
	pthread_cond_signal(&g_cond);
	pthread_mutex_unlock(&g_mutex);
	return TRUE;
}

CMainDlg::CMainDlg()
{
    m_nCloseMode = 0;
	m_dwThreadID = (DWORD)getpid();
    pthread_mutex_init(&g_mutex, NULL);
    pthread_cond_init(&g_cond, NULL);
	this->PostMessage(WM_AGENT_INITIALIZE, 0, 0);    
}

CMainDlg::~CMainDlg()
{
	MSG* pMsg = NULL;
	while(!g_appQueue.empty())
	{
		pMsg = g_appQueue.front();
		if(pMsg)
			delete pMsg;
		g_appQueue.pop();
	}
	pthread_cond_destroy(&g_cond);
	pthread_mutex_destroy(&g_mutex);
}


void CMainDlg::WM_AgentInitialize(WPARAM wParam, LPARAM lParam)
{
	INT32 nRetVal = 0;
	do{

		nRetVal = PreCreateSubClass();
	    if(nRetVal != 0)
	    {
	    	WriteLogE("fail to create pre sub class [%d]", nRetVal);
	        break;
	    }
		
		nRetVal = CreateLoadLibrary();
	    if(nRetVal != 0)
	    {
			WriteLogE("fail to load library [%d]", nRetVal);
	        break;
		}
		
		nRetVal = InitLoadLibrary();
	    if(nRetVal != 0)
		{
			WriteLogE("fail to init library [%d]", nRetVal);
	        break;
		}
		
		nRetVal = CreateSubClass();
	    if(nRetVal != 0)
		{
			WriteLogE("fail to create sub class [%d]", nRetVal);
	        break;
		}
		
		nRetVal = CheckIntegrity();
	    if(nRetVal != 0)
		{
			WriteLogE("invalid integrity [%d]", nRetVal);
		}
		
		nRetVal = InitSubClass();
	    if(nRetVal != 0)
		{
			WriteLogE("fail to init sub class [%d]", nRetVal);
	        break;
		}
		WriteLogN("success to init sub class.");
		
		nRetVal = InitRunEnv();
	    if(nRetVal != 0)
		{
			WriteLogE("fail to init run env [%d]", nRetVal);
	        break;
		}	
		WriteLogN("success to init run env.");
		
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
		WriteLogN("success to start sub class.");

		DelFileAfterBoot();
			
		nRetVal = PreStartOperation();
	    if(nRetVal != 0)
		{
			WriteLogE("fail to start pre operation [%d]", nRetVal);
	        break;
		}
		WriteLogN("success to start pre operation.");
		
		nRetVal = PreStartTimer();
		if(nRetVal != 0)
		{
			WriteLogE("fail to start pre timer [%d]", nRetVal);
	        break;
		}
		WriteLogN("success to start pre timer.");
		
		nRetVal = StartOperation();
		if(nRetVal != 0)
		{
			WriteLogE("fail to start operation [%d]", nRetVal);
	        break;
		}
		
		WriteLogN("success to start agent service. (tm_gmtoff:%ld)", GetCurrentTimeZone());
		t_LogicMgrLogEvent->InitDLEH(EVENT_OPERATION_TYPE_START, EVENT_OBJECT_TYPE_HOST, 0, 0, "", "");
		t_LogicMgrLogEvent->SetLogEvent();

		nRetVal = 0;
	}while(FALSE);

	if(nRetVal != 0)
	{
		CloseMain();
		PostMessage(WM_QUIT, 0, 0);
	}
}

//---------------------------------------------------------------------------

void CMainDlg::WM_MainThreadTerminated(WPARAM wParam, LPARAM lParam)
{
    if(m_nCloseMode)
		return;
    
	if(t_EnvInfo->m_nContinueOP & wParam)
    {
    	PostMessage(WM_QUIT, 0, 0);
    }
}
//---------------------------------------------------------------------------

void CMainDlg::WM_AgentAutoDestory(WPARAM wParam, LPARAM lParam)
{
	if(lParam != 0x00382364)
		return;
	if(t_EnvInfo == NULL)
		return;
    if(t_EnvInfo->m_nContinueOP)
    {
        this->PostMessage(WM_CLOSE, 0, 0);
        WriteLogN("agent terminating by service");
    }
}

void CMainDlg::CloseMain()
{
	if(m_nCloseMode)
		return;

	m_nCloseMode = 1;

	if(t_LogicLogEvent)
	{
		t_LogicMgrLogEvent->InitDLEH(EVENT_OPERATION_TYPE_STOP, EVENT_OBJECT_TYPE_HOST, 0, 0, "", "");
		t_LogicMgrLogEvent->SetLogEvent();
	}
	
	SetER(StopSubClass());
	WriteLogN("stop sub class result : [%d]", g_nErrRtn);

	SetER(DeleteSubClass());
	WriteLogN("delete sub class result : [%d]", g_nErrRtn); 
}
//---------------------------------------------------------------------------

int CMainDlg::Run()
{
	MSG Msg = {0,};

	while (GetMessage(&Msg))
	{
		Msg.hwnd = 0;		
		if(Msg.message == WM_QUIT)
		{
			return 1;
		}
		switch(Msg.message)
		{
			case WM_AGENT_INITIALIZE:
				WM_AgentInitialize(Msg.wParam, Msg.lParam);
				break;
			case WM_GLOBAL_TIMER_EVT:
				OnThreadTimer(Msg.wParam, Msg.lParam);
				break;
			case WM_GLOBAL_FDL_NOTIFY:
				OnFileDown(Msg.wParam, Msg.lParam);
				break;				
			case WM_AGENT_TERMINATED:
				WM_AgentAutoDestory(Msg.wParam, Msg.lParam);
				break;
			case WM_CLOSE:
				CloseMain();
				break;
		}
	}
	return 1;
}

