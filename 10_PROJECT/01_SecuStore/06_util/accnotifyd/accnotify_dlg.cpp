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
#include "accnotify_thread.h"
#include "accnotify_thread_event.h"
#include "accnotify_thread_policy.h"
#include "accnotify_dlg.h"


typedef queue<MSG*> AccNotiMsgQueue;

static AccNotiMsgQueue g_AccNotiMsgQueue;
static pthread_mutex_t g_acc_noti_mutex;
static pthread_mutex_t g_acc_log_mutex;
static pthread_cond_t g_acc_noti_cond;

CAccNotifyDlg* t_AccNotifydDlg = NULL;

char g_acAccLogBuf[CHAR_MAX_SIZE];

BOOL CAccNotifyDlg::GetMessage(MSG* pMsg)
{
	MSG* pGetMsg = NULL;
	BOOL bRetVal = TRUE;

	do{
		if(pMsg == NULL)
		{
			bRetVal = FALSE;
			break;
		}
	
		pthread_mutex_lock(&g_acc_noti_mutex);
		if(g_AccNotiMsgQueue.empty())
		{
			pthread_cond_wait(&g_acc_noti_cond, &g_acc_noti_mutex);
		}
		pGetMsg = g_AccNotiMsgQueue.front();
		g_AccNotiMsgQueue.pop();
		pthread_mutex_unlock(&g_acc_noti_mutex);
		if(pGetMsg == NULL)
		{
			bRetVal = FALSE;
			break;
		}
		memcpy(pMsg, pGetMsg, sizeof(MSG));
		delete pGetMsg;
		if(pMsg->message == WM_ACCNOTIFYD_CLOSE)
		{
			bRetVal = FALSE;
			break;
		}
		bRetVal = TRUE;
	}while(FALSE);

	return bRetVal;
}

BOOL CAccNotifyDlg::PostMessage(UINT dwMsgID, WPARAM wParam, LPARAM lParam)
{
	MSG* pMsg = new MSG;
	if(pMsg == NULL)
		return FALSE;
	pMsg->message = dwMsgID;
	pMsg->wParam = wParam;
	pMsg->lParam = lParam;
	pthread_mutex_lock(&g_acc_noti_mutex);
	g_AccNotiMsgQueue.push(pMsg);
	pthread_cond_signal(&g_acc_noti_cond);
	pthread_mutex_unlock(&g_acc_noti_mutex);
	return TRUE;
}

CAccNotifyDlg::CAccNotifyDlg()
{
    m_nCloseMode = 0;
	m_nFileLogRetention = 5;
	memset(m_acRootPath, 0, MAX_PATH);
	memset(g_acAccLogBuf, 0, CHAR_MAX_SIZE);
	m_dwThreadID = (DWORD)getpid();
    pthread_mutex_init(&g_acc_noti_mutex, NULL);
	pthread_mutex_init(&g_acc_log_mutex, NULL);
    pthread_cond_init(&g_acc_noti_cond, NULL);
	pthread_mutex_init(&m_PolMutex, NULL);
	m_nIsSetPolicy = 0;
	memset(&m_stPolicyInfo, 0, sizeof(ASI_POLICY_INFO));
	memset(g_acAccLogBuf, 0, CHAR_MAX_SIZE);
	this->PostMessage(WM_ACCNOTIFYD_INITIALIZE, 0, 0);    
}

CAccNotifyDlg::~CAccNotifyDlg()
{
	MSG* pMsg = NULL;
	while(!g_AccNotiMsgQueue.empty())
	{
		pMsg = g_AccNotiMsgQueue.front();
		if(pMsg)
			delete pMsg;
		g_AccNotiMsgQueue.pop();
	}
	pthread_cond_destroy(&g_acc_noti_cond);
	pthread_mutex_destroy(&g_acc_noti_mutex);
	pthread_mutex_destroy(&g_acc_log_mutex);
	pthread_mutex_destroy(&m_PolMutex);
}

void CAccNotifyDlg::SetPolicyInfo(PASI_POLICY_INFO pPolicyInfo)
{
	pthread_mutex_lock (&m_PolMutex);
	if(pPolicyInfo != NULL)
	{
		memcpy(&m_stPolicyInfo, pPolicyInfo, sizeof(ASI_POLICY_INFO));
	}
	pthread_mutex_unlock (&m_PolMutex);
	WriteLogN("success to set policy info.");
}

void CAccNotifyDlg::GetPolicyInfo(PASI_POLICY_INFO pPolicyInfo)
{
	pthread_mutex_lock (&m_PolMutex);
	if(pPolicyInfo != NULL)
	{
		memcpy(pPolicyInfo, &m_stPolicyInfo, sizeof(ASI_POLICY_INFO));
	}
	pthread_mutex_unlock (&m_PolMutex);
}

void CAccNotifyDlg::ClearPolicyInfo()
{
	pthread_mutex_lock (&m_PolMutex);
	memset(&m_stPolicyInfo, 0, sizeof(ASI_POLICY_INFO));
	pthread_mutex_unlock (&m_PolMutex);
}

void CAccNotifyDlg::SetPolicyState(INT32 nIsSetPolicy)
{
	pthread_mutex_lock (&m_PolMutex);
	m_nIsSetPolicy = nIsSetPolicy;
	pthread_mutex_unlock (&m_PolMutex);
	WriteLogN("success to set policy state %d.", nIsSetPolicy);
}

INT32 CAccNotifyDlg::GetPolicyState()
{
	INT32 nIsSetPolicy = 0;
	pthread_mutex_lock (&m_PolMutex);
	nIsSetPolicy = m_nIsSetPolicy;
	pthread_mutex_unlock (&m_PolMutex);
	return nIsSetPolicy;
}

void		CAccNotifyDlg::AddWLPathStr(CString strPath, CString strHash)
{
	pthread_mutex_lock (&m_PolMutex);
	m_tWLPathStrMap[strPath] = strHash;
	pthread_mutex_unlock (&m_PolMutex);
	WriteLogN("success to add wl path %s : %s.", (char *)(LPCTSTR)strPath, (char *)(LPCTSTR)strHash);
}

UINT32		CAccNotifyDlg::FindWLPathStr(CString strPath, CString &strHash)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_PolMutex);
	TMapCStrItor find = m_tWLPathStrMap.find(strPath);
	if(find != m_tWLPathStrMap.end())
	{
		strHash = find->second;
		nRtn = 1;
	}
	pthread_mutex_unlock (&m_PolMutex);
	return nRtn;
}

void		CAccNotifyDlg::ClearWLPathStr()
{
	pthread_mutex_lock (&m_PolMutex);
	m_tWLPathStrMap.clear();
	pthread_mutex_unlock (&m_PolMutex);
	WriteLogN("success to clear wl path");
}

UINT32		CAccNotifyDlg::GetWLPathCnt()
{
	UINT32 nCount = 0;
	pthread_mutex_lock (&m_PolMutex);
	nCount = (UINT32)m_tWLPathStrMap.size();
	pthread_mutex_unlock (&m_PolMutex);
	return nCount;
}


void		CAccNotifyDlg::AddBLPathStr(CString strPath, CString strHash)
{
	pthread_mutex_lock (&m_PolMutex);
	m_tBLPathStrMap[strPath] = strHash;
	pthread_mutex_unlock (&m_PolMutex);
	WriteLogN("success to add bl path %s : %s.", (char *)(LPCTSTR)strPath, (char *)(LPCTSTR)strHash);
}

UINT32		CAccNotifyDlg::FindBLPathStr(CString strPath, CString &strHash)
{
	UINT32 nRtn = 0;
	pthread_mutex_lock (&m_PolMutex);
	TMapCStrItor find = m_tBLPathStrMap.find(strPath);
	if(find != m_tBLPathStrMap.end())
	{
		strHash = find->second;
		nRtn = 1;
	}
	pthread_mutex_unlock (&m_PolMutex);
	return nRtn;

}

void		CAccNotifyDlg::ClearBLPathStr()
{
	pthread_mutex_lock (&m_PolMutex);
	m_tBLPathStrMap.clear();
	pthread_mutex_unlock (&m_PolMutex);
	WriteLogN("success to clear bl path");
}

UINT32		CAccNotifyDlg::GetBLPathCnt()
{
	UINT32 nCount = 0;
	pthread_mutex_lock (&m_PolMutex);
	nCount = (UINT32)m_tBLPathStrMap.size();
	pthread_mutex_unlock (&m_PolMutex);
	return nCount;
}

void 		CAccNotifyDlg::ClearPolicy()
{
	ClearWLPathStr();
	ClearBLPathStr();
	ClearPolicyInfo();
	SetPolicyState(0);
}


void CAccNotifyDlg::WriteLogN(char *fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };

	pthread_mutex_lock(&g_acc_log_mutex);
	do{
		GetCurrentDateTime(0, acTimeBuf);

		if(m_acRootPath[0] == 0)
		{
			if(get_nanny_agent_root(m_acRootPath, MAX_PATH) != 0)
				break;
		}

		snprintf(acSaveFile, MAX_PATH-1, "%s/nanny/log/acc_notifyd_log_%s.txt", m_acRootPath, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			snprintf(acLogBuf, CHAR_MAX_SIZE-1, "%s/nanny/log", m_acRootPath);
			ClearOldLogFile(acLogBuf, "accnotifyd_log_", m_nFileLogRetention);
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

		if(_stricmp(acLogBuf, g_acAccLogBuf))
		{
			strncpy(g_acAccLogBuf, acLogBuf, CHAR_MAX_SIZE-1);
			g_acAccLogBuf[CHAR_MAX_SIZE-1] = 0;
			fprintf(fp, "%s\t[Info]\t%s\n", acTimeBuf, acLogBuf);
		}
		fclose(fp);
	}while(FALSE);
	pthread_mutex_unlock(&g_acc_log_mutex);
}

void CAccNotifyDlg::WriteLogE(char *fmt,...)
{
	FILE *fp = NULL;
	va_list args;
	char acSaveFile[MAX_PATH] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };
	char acLogBuf[CHAR_MAX_SIZE] = {0, };

	pthread_mutex_lock(&g_acc_log_mutex);
	do{
		GetCurrentDateTime(0, acTimeBuf);

		if(m_acRootPath[0] == 0)
		{
			if(get_nanny_agent_root(m_acRootPath, MAX_PATH) != 0)
				break;
		}

		snprintf(acSaveFile, MAX_PATH-1, "%s/nanny/log/acc_notifyd_log_%s.txt", m_acRootPath, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			snprintf(acLogBuf, CHAR_MAX_SIZE-1, "%s/nanny/log", m_acRootPath);
			ClearOldLogFile(acLogBuf, "accnotifyd_log_", m_nFileLogRetention);
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
		if(_stricmp(acLogBuf, g_acAccLogBuf))
		{
			strncpy(g_acAccLogBuf, acLogBuf, CHAR_MAX_SIZE-1);
			g_acAccLogBuf[CHAR_MAX_SIZE-1] = 0;
			fprintf(fp, "%s\t[Error]\t%s\n", acTimeBuf, acLogBuf);
		}
		fclose(fp);
	}while(FALSE);
	pthread_mutex_unlock(&g_acc_log_mutex);
}

INT32		CAccNotifyDlg::CreateSubClass()
{
/*
	t_AccMonThreadTimer = new CAccMonThreadTimer();
	if(t_AccMonThreadTimer == NULL)
	{
		return -1;
	}
*/
	t_ThreadAccNotifyPolicy = new CThreadAccNotifyPolicy();
	if(t_ThreadAccNotifyPolicy == NULL)
	{
		return -1;
	}
	t_ThreadAccNotifyPolicy->SetNotifyWnd((HWND)this);

	t_ThreadAccNotifyEvent = new CThreadAccNotifyEvent();
	if(t_ThreadAccNotifyEvent == NULL)
	{
		SAFE_DELETE(t_ThreadAccNotifyPolicy);
		return -2;
	}
	t_ThreadAccNotifyEvent->SetNotifyWnd((HWND)this);
	return 0;
}


INT32		CAccNotifyDlg::InitTimerID()
{
//	t_AccMonThreadTimer->t_AccMonTimerUtil.SetNotifyWnd((HWND)this, (UINT32)WM_ACCNOTIFYD_TIMER_EVT);
//	t_AccMonThreadTimer->t_AccMonTimerUtil.AddTimer(TIMER_ID_ACCNOTIFY_ALIVE_MSG, TIMER_INTERVAL_TIME_SEC*5, NULL, 1, 0);
	return 0;
}



INT32		CAccNotifyDlg::StartSubClass()
{
	pthread_t tid = 0;
	BOOL bRetVal = FALSE;
/*
	bRetVal = t_AccMonThreadTimer->CreateThreadExt("timer", &tid);
	if(bRetVal == FALSE)
	{
		WriteLogE("start [timer] thread result : fail [%d]", errno);
		return -1;
	}
	Sleep(10);
*/
	tid = 0;
	bRetVal = t_ThreadAccNotifyPolicy->CreateThreadExt("accpolicy", &tid);
	if(bRetVal == FALSE)
	{
		WriteLogE("start [accpolicy] thread result : fail [%d]", errno);
		return -2;

	}
	Sleep(10);

	tid = 0;
	bRetVal = t_ThreadAccNotifyEvent->CreateThreadExt("accnotify", &tid);
	if(bRetVal == FALSE)
	{
		WriteLogE("start [accnotify] thread result : fail [%d]", errno);
		return -2;

	}
	Sleep(10);
	return 0;
}


void CAccNotifyDlg::WM_AgentInitialize(WPARAM wParam, LPARAM lParam)
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
		WriteLogN("success to start accnotifyd service.");
		nRetVal = 0;
	}while(FALSE);

	if(nRetVal != 0)
	{
		CloseMain();
		PostMessage(WM_ACCNOTIFYD_CLOSE, 0, 0);
	}
}

INT32		CAccNotifyDlg::StopThread_Common(CThreadBase* tThreadObject, UINT32 nWaitTime)
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



VOID CAccNotifyDlg::StopSubClass()
{
/*
	t_AccMonThreadTimer->SetContinue(0);
	SetER(StopThread_Common(t_AccMonThreadTimer));
	WriteLogN("stop [timer] thread result : [%d]", g_nErrRtn);
*/

	t_ThreadAccNotifyPolicy->SetContinue(0);
	SetER(StopThread_Common(t_ThreadAccNotifyPolicy));
	WriteLogN("stop [accpolicy] thread result : [%d]", g_nErrRtn);

	t_ThreadAccNotifyEvent->SetContinue(0);
	SetER(StopThread_Common(t_ThreadAccNotifyEvent));
	WriteLogN("stop [accnotify] thread result : [%d]", g_nErrRtn);
}
//--------------------------------------------------------------------

VOID		CAccNotifyDlg::DeleteSubClass()
{
//	SAFE_DELETE(t_AccMonThreadTimer);
	SAFE_DELETE(t_ThreadAccNotifyPolicy);
	SAFE_DELETE(t_ThreadAccNotifyEvent);
}


//---------------------------------------------------------------------------

void CAccNotifyDlg::WM_MainThreadTerminated(WPARAM wParam, LPARAM lParam)
{
    if(m_nCloseMode)
		return;
    
	if(wParam)
    {
    	PostMessage(WM_ACCNOTIFYD_CLOSE, 0, 0);
    }
}
//---------------------------------------------------------------------------

void CAccNotifyDlg::WM_AgentAutoDestory(WPARAM wParam, LPARAM lParam)
{
	if(lParam != 0x00382364)
		return;
    this->PostMessage(WM_ACCNOTIFYD_CLOSE, 0, 0);
    WriteLogN("agent terminating by service");
}

void CAccNotifyDlg::CloseMain()
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

INT32 CAccNotifyDlg::SendAliveMessage()
{
	return 0;
/*
	UINT32 dwGap = 0;
	UINT32 dwCurrentTime = 0;
	UINT32 dwSendTime = 0;
	INT32 nRetVal = 0;
	if(t_ThreadAccNotifyEvent == NULL)
	{
		WriteLogE("invalid notify thread addr");
		return -1;
	}
	dwSendTime = t_ThreadAccNotifyEvent->GetSendTime();
	dwCurrentTime = GetCurrentDateTimeInt();
	dwGap = dwCurrentTime - dwSendTime;
	if(dwGap < 3)
		return 0;

	nRetVal = t_ThreadAccNotifyEvent->SendAliveMessage();
	if(nRetVal < 0)
		WriteLogE("fail to send alive message (%d)", nRetVal);
	return nRetVal;
*/
}

DWORD		CAccNotifyDlg::OnThreadTimer(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case TIMER_ID_ACCNOTIFY_ALIVE_MSG:
		{
			if(t_AccMonThreadTimer)
			{
				t_AccMonThreadTimer->t_AccMonTimerUtil.DisableTimer(TIMER_ID_ACCNOTIFY_ALIVE_MSG);
				SendAliveMessage();
				t_AccMonThreadTimer->t_AccMonTimerUtil.EnableTimer(TIMER_ID_ACCNOTIFY_ALIVE_MSG);
			}
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



int CAccNotifyDlg::Run()
{
	MSG Msg = {0,};

	while (GetMessage(&Msg))
	{
		Msg.hwnd = 0;		
		switch(Msg.message)
		{
			case WM_ACCNOTIFYD_INITIALIZE:
				WM_AgentInitialize(Msg.wParam, Msg.lParam);
				break;
			case WM_ACCNOTIFYD_TIMER_EVT:
				OnThreadTimer(Msg.wParam, Msg.lParam);
				break;
			case WM_ACCNOTIFYD_TERMINATED:
				WM_AgentAutoDestory(Msg.wParam, Msg.lParam);
				break;
			case WM_ACCNOTIFYD_CLOSE:
				CloseMain();
				break;
		}
	}
	return 1;
}

