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
 * License along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 * * 
 */

// accnotify_thread_policy.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "accnotify_dlg.h"
#include "accnotify_thread_policy.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadAccNotifyPolicy

CThreadAccNotifyPolicy*	t_ThreadAccNotifyPolicy = NULL;

CThreadAccNotifyPolicy::CThreadAccNotifyPolicy()
{
	m_pNotifyWnd = NULL;
	m_nServerFd = -1;
	m_nClientFd = -1;
	m_nCheckThread = ASI_DISCONNECT_STATE;
	pthread_mutex_init(&m_SockMutex, NULL);
}

CThreadAccNotifyPolicy::~CThreadAccNotifyPolicy()
{
	pthread_mutex_destroy(&m_SockMutex);
}

INT32	CThreadAccNotifyPolicy::SetNotifyWnd(HWND pNotifyWnd)
{
	if(pNotifyWnd == NULL)
	{
		return -1;
	}
	m_pNotifyWnd = pNotifyWnd;
	return 0;
}


BOOL CThreadAccNotifyPolicy::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadAccNotifyPolicy::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadAccNotifyPolicy message handlers

INT32 CThreadAccNotifyPolicy::InitUnixSock(INT32 &nSrvFd)
{
	INT32 nRetVal = 0;
	INT32 nOptVal = 1;
	struct sockaddr_un serveraddr;
	char acSockPath[MAX_FILE_NAME] = {0,};
	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_POL_FILE);
	acSockPath[MAX_FILE_NAME-1] = 0;
	if(is_file(acSockPath) == SOCK_FILE)
	{
		unlink(acSockPath);
	}
	pthread_mutex_lock (&m_SockMutex);
	do{

		if ((m_nServerFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		{
			m_nServerFd = -1;
			nRetVal = -1;
			break;
		}
		memset((char *)&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sun_family = AF_UNIX;
		strncpy(serveraddr.sun_path, acSockPath, MAX_FILE_NAME-1);
		setsockopt(m_nServerFd, SOL_SOCKET, SO_REUSEADDR, &nOptVal, sizeof(nOptVal));
		if (bind(m_nServerFd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		{
			nRetVal = -2;
			break;
		}
		if (listen(m_nServerFd, 1) < 0)
		{
			nRetVal = -3;
			break;
		}
		nRetVal = 0;
		nSrvFd = m_nServerFd;
	}while(FALSE);
	pthread_mutex_unlock (&m_SockMutex);
	if(nRetVal != 0)
		UninitUnixSock();
	return nRetVal;
}

VOID CThreadAccNotifyPolicy::UninitUnixSock()
{
	char acSockPath[MAX_FILE_NAME] = {0,};
	pthread_mutex_lock (&m_SockMutex);
	if(m_nServerFd != -1)
	{
		close(m_nServerFd);
		m_nServerFd = -1;
	}
	pthread_mutex_unlock (&m_SockMutex);

	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_POL_FILE);
	acSockPath[MAX_FILE_NAME-1] = 0;
	if(is_file(acSockPath) == SOCK_FILE)
	{
		unlink(acSockPath);
	}
}

INT32	CThreadAccNotifyPolicy::GetCheckThreadState()
{
	INT32 nFlag = 0;
	pthread_mutex_lock (&m_SockMutex);
	nFlag = m_nCheckThread;
	pthread_mutex_unlock (&m_SockMutex);
	return nFlag;
}


VOID	CThreadAccNotifyPolicy::SetCheckThreadState(INT32 nState)
{
	pthread_mutex_lock (&m_SockMutex);
	m_nCheckThread = nState;
	pthread_mutex_unlock (&m_SockMutex);
}

INT32	CThreadAccNotifyPolicy::GetClientFd()
{
	INT32 nFd = 0;
	pthread_mutex_lock (&m_SockMutex);
	nFd = m_nClientFd;
	pthread_mutex_unlock (&m_SockMutex);
	return nFd;
}


VOID	CThreadAccNotifyPolicy::SetClientFd(INT32 nFd)
{
	pthread_mutex_lock (&m_SockMutex);
	m_nClientFd = nFd;
	pthread_mutex_unlock (&m_SockMutex);
}


INT32 CThreadAccNotifyPolicy::SockRecv(INT32 nFd, PVOID pRecvData, INT32 nReqSize)
{
	INT32 nOnceRecvSize = 0;
	INT32 nRecvedSize = 0;
	INT32 nRecvSize = 0;
	INT32 nContinueCount = 0;
	INT32 nRetVal = 0;
	char *pcRecv = NULL;

	if(pRecvData == NULL || nReqSize < 1 || nFd < -1)
	{
		return -1;
	}
	pcRecv = (char *)pRecvData;
	while (nRecvedSize < nReqSize)
	{
		nOnceRecvSize = nReqSize - nRecvedSize;

		nRecvSize = recv(nFd, &pcRecv[nRecvedSize], nOnceRecvSize, 0);
		if (nRecvSize == -1)
		{
			if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nContinueCount++;
				if(nContinueCount > 20)
				{
					return -4;
				}
				Sleep(20);
				continue;
			}
			else
			{
				return -2;
			}
		}
		else if (nRecvSize == 0)
		{
			return -3;
		}
		else
		{
			nRecvedSize += nRecvSize;
		}
	}
	return 0;
}

INT32 CThreadAccNotifyPolicy::SockWrite(INT32 nFd, PVOID pWriteData, INT32 nReqSize)
{
	INT32 nOnceWriteSize = 0;
	INT32 nWritedSize = 0;
	INT32 nWriteSize = 0;
	INT32 nContinueCount = 0;
	char *pcWrite = NULL;
	if(pWriteData == NULL || nReqSize < 1 || nFd == -1)
	{
		return -1;
	}
	pcWrite = (char *)pWriteData;
	while (nWritedSize < nReqSize)
	{
		nOnceWriteSize = nReqSize - nWritedSize;

		nWriteSize = send(nFd, &pcWrite[nWritedSize], nOnceWriteSize, 0);
		if (nWriteSize == -1)
		{
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nContinueCount++;
				if(nContinueCount > 20)
				{
					return -4;
				}
				Sleep(20);
				continue;
			}
			else
			{
				return -2;
			}
		}
		else if (nWriteSize == 0)	//	socket close
		{
			return -3;
		}
		else
		{
			nWritedSize += nWriteSize;
		}
	}
	return 0;
}

INT32		CThreadAccNotifyPolicy::SendExitThreadCmd()
{
	INT32 nClientFd = -1;
	INT32 nRetVal = 0;
	INT32 nState = 0;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	INT32 nClientLen = 0;
	struct sockaddr_un stClientAddr;
	char acSockPath[MAX_PATH] = {0,};
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	do{
		nRetVal = GetCheckThreadState();
		if(nRetVal != ASI_ACCEPT_STATE)
		{
			nRetVal = 0;
			break;
		}
		pChkFileProc = (PASI_CHK_FILE_PROC)malloc(nSize);
		if(pChkFileProc == NULL)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogE("[accnotify_policy] fail to allocate memory (%d)", errno);
			nRetVal = -1;
			break;
		}
		memset(pChkFileProc, 0, nSize);

		nClientFd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (nClientFd == -1)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogE("[accnotify_policy] fail to create sock (%d)", errno);
			nRetVal = -2;
			break;
		}

		snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_POL_FILE);
		acSockPath[MAX_FILE_NAME-1] = 0;
		nClientLen = sizeof(stClientAddr);
		memset(&stClientAddr, 0, nClientLen);
		stClientAddr.sun_family = AF_UNIX;
		strncpy(stClientAddr.sun_path, acSockPath, MAX_FILE_NAME-1);
		nRetVal = connect(nClientFd, (struct sockaddr *)&stClientAddr, nClientLen);
		if (nRetVal < 0)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogE("[accnotify_policy] fail to connect %s (%d)", acSockPath, errno);
			nRetVal = -3;
			break;
		}

		pChkFileProc->nCmdId = CMD_PIPE_EXIT_THREAD;
		nRetVal = SockWrite(nClientFd, (PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogE("[accnotify_policy] fail to write data (%d) (%d)", nRetVal, errno);
			nRetVal = -4;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	if(nClientFd != -1)
	{
		close(nClientFd);
	}
	safe_free(pChkFileProc);
	return nRetVal;
}

INT32 CThreadAccNotifyPolicy::CheckThread(INT32 nClientFd)
{
	INT32 nRetVal = 0;
	PASI_ACC_NOTIFY_POLICY pAccNotiPol = NULL;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	if(nClientFd < 0)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("invalid input data : [%s][%d]", m_strThreadName.c_str(), nRetVal);
		return -1;
	}

	pAccNotiPol = (PASI_ACC_NOTIFY_POLICY)malloc(sizeof(ASI_ACC_NOTIFY_POLICY));
	if(pAccNotiPol == NULL)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("fail to allocate memory : [%s][%d]", m_strThreadName.c_str(), errno);
		close(nClientFd);
		SetClientFd(-1);
		return -2;
	}

	SetCheckThreadState(ASI_CONNECT_STATE);
	while(GetContinue())
	{
		memset(pAccNotiPol, 0, sizeof(ASI_ACC_NOTIFY_POLICY));
		nRetVal = CheckSockEvent(nClientFd, pAccNotiPol);
		if(nRetVal != 0)
		{
			if(nRetVal != -13 && t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogE("fail to check sock event : [%s][%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
			close(nClientFd);
			SetClientFd(-1);
			safe_free(pAccNotiPol);
			SetCheckThreadState(ASI_DISCONNECT_STATE);
			return nRetVal;
		}
	}	
	SetCheckThreadState(ASI_DISCONNECT_STATE);
	close(nClientFd);
	SetClientFd(-1);
	safe_free(pAccNotiPol);
	return 0;
}

INT32 CThreadAccNotifyPolicy::Run()
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 i, nRetVal = 0;
	INT32 nServerFd = -1;
	INT32 nClientFd = -1;
	INT32 nState = 0;
	struct sockaddr_un stClientAddr;
	socklen_t nClientLen = sizeof(struct sockaddr_un);
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	nRetVal = InitUnixSock(nServerFd);
	if(nRetVal == 0)
	{
		m_nPause = 0;
	}
	else
	{
		m_nPause = 1;
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("fail to init notify event : [%s][%d]", m_strThreadName.c_str(), nRetVal);
	}

	m_nRunFlag = 1;
	if(t_AccNotifyDlg)
		t_AccNotifyDlg->WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	SetCheckThreadState(ASI_DISCONNECT_STATE);

	while(GetContinue())
	{
		if(!m_nPause)
		{
			SetCheckThreadState(ASI_ACCEPT_STATE);
			nClientFd = accept(nServerFd, (struct sockaddr *)&stClientAddr, &nClientLen);
			if (nClientFd < 0)
			{
				if(t_AccNotifyDlg)
					t_AccNotifyDlg->WriteLogE("fail to accept : [%s][%d]", m_strThreadName.c_str(), errno);
				UninitUnixSock();
				m_nPause = 1;
				Sleep(1000);
			}
			nRetVal = CheckThread(nClientFd);
			if(nRetVal == 1111)
			{
				break;
			}
			else
			{
				UninitUnixSock();
				m_nPause = 1;
				Sleep(1000);
			}
		}
		else
		{
			nServerFd = -1;
			nRetVal = InitUnixSock(nServerFd);
			if(nRetVal == 0)
			{
				m_nPause = 0;
			}
			else
			{
				m_nPause = 1;
				Sleep(1000);
			}
		}
	}

	if(t_AccNotifyDlg && !GetContinue())
		t_AccNotifyDlg->WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	UninitUnixSock();
	return 0;
}
INT32		CThreadAccNotifyPolicy::CheckSockEvent(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nAcVal = RET_NONE;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	CString strPath;
	CString strHash;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	if(pAccNotiPol == NULL || nClientFd == -1)
	{
		return -1;
	}
	do{
		nRetVal = SockRecv(nClientFd, (PVOID)pAccNotiPol, nSize);
		if(nRetVal != 0)
		{
			if(nRetVal < 0)
				nRetVal -= 10;
			break;
		}

		if(pAccNotiPol->nCmdId == CMD_PIPE_EXIT_THREAD)
		{
			nRetVal = 1111;
			break;
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_SET_POLICY_INFO)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->SetPolicyInfo(&pAccNotiPol->stPolInfo);
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_ADD_WHITE_FILE)
		{
			strPath.Format("%s", pAccNotiPol->stFileHash.acFullPath);
			strHash.Format("%s", pAccNotiPol->stFileHash.acWhiteHash);
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->AddWLPathStr(strPath, strHash);
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_CLEAR_WHITE_FILE)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->ClearWLPathStr();
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_ADD_BLACK_FILE)
		{
			strPath.Format("%s", pAccNotiPol->stFileHash.acFullPath);
			strHash.Format("%s", pAccNotiPol->stFileHash.acWhiteHash);
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->AddBLPathStr(strPath, strHash);
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_CLEAR_BLACK_FILE)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->ClearBLPathStr();
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_END_POLICY_INFO)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->SetPolicyState(TRUE);
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

