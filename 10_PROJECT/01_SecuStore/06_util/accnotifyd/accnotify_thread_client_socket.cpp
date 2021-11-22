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
 * License along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 * * 
 */

#include "stdafx.h"
#include "com_struct.h"
#include "accnotify_dlg.h"
#include "accnotify_thread_client_socket.h"

static HWND					g_pNotifyWnd = NULL;
static pthread_mutex_t 		g_EventMutex;
static INT32				g_nOpMode = 0;
static BOOL					g_bIsConnected = FALSE;
static char *				g_pString = NULL;
static INT32				g_nShmId = -1;
static TListChkFileProc		g_tChkFileProcList;

/////////////////////////////////////////////////////////////////////////////
// CThreadAccClientSocket
CThreadAccClientSocket*	t_ThreadAccClientSocket = NULL;

CThreadAccClientSocket::CThreadAccClientSocket()
{
	g_pNotifyWnd = NULL;
}

CThreadAccClientSocket::~CThreadAccClientSocket()
{
}

BOOL CThreadAccClientSocket::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadAccClientSocket::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

INT32	CThreadAccClientSocket::SetNotifyWnd(HWND pNotifyWnd)
{
	if(pNotifyWnd == NULL)
	{
		return -1;
	}
	g_pNotifyWnd = pNotifyWnd;
	return 0;
}

VOID CThreadAccClientSocket::SetOpMode(INT32 nMode)
{
	pthread_mutex_lock (&g_EventMutex);
	g_nOpMode = nMode;
	pthread_mutex_unlock (&g_EventMutex);
}

VOID CThreadAccClientSocket::GetOpMode(INT32 &nMode)
{
	pthread_mutex_lock (&g_EventMutex);
	nMode = g_nOpMode;
	pthread_mutex_unlock (&g_EventMutex);
}

/////////////////////////////////////////////////////////////////////////////
// CThreadAccClientSocket message handlers

INT32	CThreadAccClientSocket::InitShmEnv()
{
	INT32 nRetVal = 0;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)g_pNotifyWnd;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	do{
		if(g_nShmId == -1)
		{
			g_nShmId = shmget((key_t)SM_CHK_ACCESS_FILE_KEY, 0, NULL);
			if(g_nShmId == -1)
			{
				if(t_AccNotifyDlg != NULL)
					t_AccNotifyDlg->WriteLogE("fail to get shared memory(%d)", errno);
				nRetVal = -1;
				break;
			}
		}

		nRetVal = check_proc_exist_by_name(NANNY_AGENT_IDENT, 0);
		if(nRetVal != ASI_PROC_EXIST)
		{
			nRetVal = -2;
			break;
		}

		g_pString = (char *)shmat(g_nShmId, NULL, 0);
		if(g_pString == (char *)-1)
		{
			if(t_AccNotifyDlg != NULL)
				t_AccNotifyDlg->WriteLogE("fail to attach shared memory(%d)", errno);
			g_pString = NULL;
			nRetVal = -3;
			break;
		}
		pChkFileProc = (PASI_CHK_FILE_PROC)g_pString;
		pChkFileProc->nState = SM_WRITE_CLIENT_FLAG;

		g_bIsConnected = TRUE;
		nRetVal = 0;
	}while(FALSE);

	if(nRetVal < 0)
	{
		UninitShmEnv();
	}
	return nRetVal;
}

VOID	CThreadAccClientSocket::UninitShmEnv()
{
	pthread_mutex_lock (&g_EventMutex);
	if(g_pString != NULL)
	{
		shmdt(g_pString);
		g_pString = NULL;
	}
	g_bIsConnected = FALSE;
	pthread_mutex_unlock (&g_EventMutex);
}

INT32 CThreadAccClientSocket::ShmWrite(PVOID pWriteData, INT32 nReqSize)
{
	INT32 nContinueCount = 0;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	if(pWriteData == NULL || nReqSize != sizeof(ASI_CHK_FILE_PROC) || g_pString == NULL)
	{
		return -1;
	}
	pChkFileProc = (PASI_CHK_FILE_PROC)g_pString;
	while (nContinueCount < 50)
	{
		if(pChkFileProc->nState == SM_WRITE_CLIENT_FLAG)
		{
			memcpy(g_pString, pWriteData, nReqSize);
			pChkFileProc->nState = SM_READ_SERVER_FLAG;
			return 0;
		}
		nContinueCount++;
		Sleep(100);
	}
	memcpy(g_pString, pWriteData, nReqSize);
	pChkFileProc->nState = SM_READ_SERVER_FLAG;
	return 1;
}


INT32 CThreadAccClientSocket::GetChkFileProcListCnt()
{
	INT32 nCount = 0;
	pthread_mutex_lock (&g_EventMutex);
	nCount = g_tChkFileProcList.size();
	pthread_mutex_unlock (&g_EventMutex);
	return nCount;
}

INT32 CThreadAccClientSocket::AddChkFileProcList(PASI_CHK_FILE_PROC pChkFileProc)
{
	INT32 nOpMode = 0;
	if(pChkFileProc == NULL)
	{
		return -1;
	}
	GetOpMode(nOpMode);
	if(nOpMode == 0)
	{
		return -2;
	}
	pthread_mutex_lock (&g_EventMutex);
	g_tChkFileProcList.push_back(*pChkFileProc);
	pthread_mutex_unlock (&g_EventMutex);
	return 0;
}

INT32 CThreadAccClientSocket::GetChkFileProcList(TListChkFileProc &tListChkFileProc) 
{
	INT32 nCount = 0;
	TListChkFileProcItor begin, end;
	pthread_mutex_lock (&g_EventMutex);
	begin = g_tChkFileProcList.begin();	end = g_tChkFileProcList.end();
	for(begin; begin != end; begin++)
	{
		tListChkFileProc.push_back(*begin);
	}
	g_tChkFileProcList.clear();
	pthread_mutex_unlock (&g_EventMutex);
	nCount = tListChkFileProc.size();
	return nCount;
}

VOID CThreadAccClientSocket::ClearChkFileProcList()
{
	pthread_mutex_lock (&g_EventMutex);
	g_tChkFileProcList.clear();
	pthread_mutex_unlock (&g_EventMutex);
}


INT32	CThreadAccClientSocket::SendEventToServer(TListChkFileProc tListChkFileProc)
{
	INT32 nRetVal = 0;
	char acSockPath[MAX_PATH] = {0,};
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	TListChkFileProcItor begin, end;

	begin = tListChkFileProc.begin();	end = tListChkFileProc.end();
	for(begin; begin != end; begin++)
	{
		pChkFileProc = (PASI_CHK_FILE_PROC)&(*begin);
		nRetVal = ShmWrite((PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			return nRetVal;
		}
		else if(nRetVal == 1)
		{
			nRetVal = check_proc_exist_by_name(NANNY_AGENT_IDENT, 0);
			if(nRetVal != ASI_PROC_EXIST)
			{
				nRetVal -= 20;
				return nRetVal;
			}
		}
	}

	return 0;
}

INT32 CThreadAccClientSocket::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	TListChkFileProc tListChkFileProc;
	CAccNotifyDlg* t_NotifyDlg = (CAccNotifyDlg *)g_pNotifyWnd;
	g_nShmId = -1;
	g_bIsConnected = FALSE;
	pthread_mutex_init(&g_EventMutex, NULL);
	SetOpMode(0);
	m_nPause = 1;
	nRetVal = InitShmEnv();
	if(nRetVal == 0)
	{
		m_nPause = 0;
		SetOpMode(1);
	}
	else
	{
		m_nPause = 1;
		if(t_NotifyDlg != NULL && nRetVal != -2)
			t_NotifyDlg->WriteLogE("[%s] fail to init shm env : [%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
		SetOpMode(0);
	}

	m_nRunFlag = 1;
	if(t_NotifyDlg != NULL)
		t_NotifyDlg->WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(GetContinue())
	{
		if(!m_nPause)
		{
			nCount = GetChkFileProcList(tListChkFileProc);
			if(nCount > 0)
			{
				nRetVal = SendEventToServer(tListChkFileProc);
				if(nRetVal != 0)
				{
					if(nRetVal == -1)
					{
						if(t_NotifyDlg != NULL)
							t_NotifyDlg->WriteLogE("[%s] invalid input data.exit thread", m_strThreadName.c_str());
						SetOpMode(0);
						break;
					}
					else
					{
						if(nRetVal == -2 && t_NotifyDlg != NULL)
							t_NotifyDlg->WriteLogN("[%s] reconnect nannysvc : [%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
						UninitShmEnv();
						SetOpMode(0);
						m_nPause = 1;
						Sleep(1000);
					}
				}
				else
				{
					tListChkFileProc.clear();
				}
			}
			else
			{
				Sleep(1000);
			}
		}
		else
		{
			nRetVal = InitShmEnv();
			if(nRetVal == 0)
			{
				m_nPause = 0;
				SetOpMode(1);
			}
			else
			{
				m_nPause = 1;
				if(t_NotifyDlg != NULL && nRetVal != -2)
					t_NotifyDlg->WriteLogE("[%s] fail to init shm env : [%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
				Sleep(1000);
			}
		}
	}
	if(t_NotifyDlg != NULL)
	{
		t_NotifyDlg->WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());
	}
	UninitShmEnv();
	ClearChkFileProcList();
	pthread_mutex_destroy(&g_EventMutex);
	return 0;
}
