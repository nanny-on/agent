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

#include "stdafx.h"
#include "com_struct.h"
#include "accnotify_dlg.h"
#include "accnotify_thread_server_socket.h"

static INT32				g_nServerFd = -1;
static INT32				g_nClientFd = -1;
static INT32				g_nOpMode = 0;
static INT32				m_nCheckThread = ASI_DISCONNECT_STATE;
static pthread_mutex_t 		g_SockMutex;
static UINT32				g_nSockContinue = 0;

static INT32				g_nPolicyUse = 0;
static ASI_POLICY_INFO		g_stPolicyInfo;
static TMapCStr				g_tAllowEXPathStrMap;
static TMapCStr				g_tDenyEXPathStrMap;
static TMapCStr				g_tAllowSPPathStrMap;
static TMapCStr				g_tDenySPPathStrMap;
static TMapCStr				g_tWLPathStrMap;
static TMapCStr				g_tBLPathStrMap;
static TMapCStr				g_tCRPathStrMap;
static pthread_mutex_t		g_StrMapMutex;

static HWND					g_pNotifyWnd = NULL;

/////////////////////////////////////////////////////////////////////////////
// CThreadAccServerSocket
CThreadAccServerSocket*	t_ThreadAccServerSocket = NULL;

CThreadAccServerSocket::CThreadAccServerSocket()
{
	pthread_mutex_init(&g_SockMutex, NULL);
	pthread_mutex_init(&g_StrMapMutex, NULL);
}

CThreadAccServerSocket::~CThreadAccServerSocket()
{
	pthread_mutex_destroy(&g_SockMutex);
	pthread_mutex_destroy(&g_StrMapMutex);
}

BOOL CThreadAccServerSocket::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadAccServerSocket::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

INT32	CThreadAccServerSocket::SetNotifyWnd(HWND pNotifyWnd)
{
	if(pNotifyWnd == NULL)
	{
		return -1;
	}
	g_pNotifyWnd = pNotifyWnd;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CThreadAccServerSocket message handlers

INT32 CThreadAccServerSocket::InitUnixSock(INT32 &nSrvFd)
{
	INT32 nRetVal = 0;
	INT32 nOptVal = 1;
	struct sockaddr_un serveraddr;
	char acSockPath[MAX_FILE_NAME] = {0,};
	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_ACC_FILE);
	acSockPath[MAX_FILE_NAME-1] = 0;
	unlink(acSockPath);
	pthread_mutex_lock (&g_SockMutex);
	do{

		if ((g_nServerFd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		{
			g_nServerFd = -1;
			nRetVal = -1;
			break;
		}
		memset((char *)&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sun_family = AF_UNIX;
		strncpy(serveraddr.sun_path, acSockPath, MAX_FILE_NAME-1);
		setsockopt(g_nServerFd, SOL_SOCKET, SO_REUSEADDR, &nOptVal, sizeof(nOptVal));
		if (bind(g_nServerFd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		{
			nRetVal = -2;
			break;
		}
		if (listen(g_nServerFd, 1) < 0)
		{
			nRetVal = -3;
			break;
		}
		nRetVal = 0;
		nSrvFd = g_nServerFd;
	}while(FALSE);
	pthread_mutex_unlock (&g_SockMutex);
	if(nRetVal != 0)
		UninitUnixSock();
	return nRetVal;
}

VOID CThreadAccServerSocket::UninitUnixSock()
{
	char acSockPath[MAX_FILE_NAME] = {0,};
	pthread_mutex_lock (&g_SockMutex);
	if(g_nServerFd != -1)
	{
		close(g_nServerFd);
		g_nServerFd = -1;
	}
	pthread_mutex_unlock (&g_SockMutex);

	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_ACC_FILE);
	acSockPath[MAX_FILE_NAME-1] = 0;
	unlink(acSockPath);
}


VOID CThreadAccServerSocket::SetOpMode(INT32 nMode)
{
	pthread_mutex_lock (&g_SockMutex);
	g_nOpMode = nMode;
	pthread_mutex_unlock (&g_SockMutex);
}

VOID CThreadAccServerSocket::GetOpMode(INT32 &nMode)
{
	pthread_mutex_lock (&g_SockMutex);
	nMode = g_nOpMode;
	pthread_mutex_unlock (&g_SockMutex);
}

INT32	CThreadAccServerSocket::GetCheckThreadState()
{
	INT32 nFlag = 0;
	pthread_mutex_lock (&g_SockMutex);
	nFlag = m_nCheckThread;
	pthread_mutex_unlock (&g_SockMutex);
	return nFlag;
}


VOID CThreadAccServerSocket::SetCheckThreadState(INT32 nState)
{
	pthread_mutex_lock (&g_SockMutex);
	m_nCheckThread = nState;
	pthread_mutex_unlock (&g_SockMutex);
}

INT32 CThreadAccServerSocket::GetClientFd()
{
	INT32 nFd = 0;
	pthread_mutex_lock (&g_SockMutex);
	nFd = g_nClientFd;
	pthread_mutex_unlock (&g_SockMutex);
	return nFd;
}


VOID CThreadAccServerSocket::SetClientFd(INT32 nFd)
{
	pthread_mutex_lock (&g_SockMutex);
	g_nClientFd = nFd;
	pthread_mutex_unlock (&g_SockMutex);
}


VOID CThreadAccServerSocket::SetPolicyUse(INT32 nPolUse)
{
	pthread_mutex_lock (&g_StrMapMutex);
	g_nPolicyUse = nPolUse;
	pthread_mutex_unlock (&g_StrMapMutex);
}

VOID CThreadAccServerSocket::GetPolicyUse(INT32 &nPolUse)
{
	pthread_mutex_lock (&g_StrMapMutex);
	nPolUse = g_nPolicyUse;
	pthread_mutex_unlock (&g_StrMapMutex);
}

VOID CThreadAccServerSocket::SetPolicyInfo(PASI_POLICY_INFO pPolicyInfo)
{
	pthread_mutex_lock (&g_StrMapMutex);
	if(pPolicyInfo != NULL)
	{
		memcpy(&g_stPolicyInfo, pPolicyInfo, sizeof(ASI_POLICY_INFO));
	}
	pthread_mutex_unlock (&g_StrMapMutex);
}

VOID CThreadAccServerSocket::GetPolicyInfo(PASI_POLICY_INFO pPolicyInfo)
{
	pthread_mutex_lock (&g_StrMapMutex);
	if(pPolicyInfo != NULL)
	{
		memcpy(pPolicyInfo, &g_stPolicyInfo, sizeof(ASI_POLICY_INFO));
	}
	pthread_mutex_unlock (&g_StrMapMutex);
}

VOID CThreadAccServerSocket::ClearAllInfo()
{
	ClearPolicyInfo();
	ClearExPathStr();
	ClearSpPathStr();
	ClearWlPathStr();
	ClearBlPathStr();
	ClearCrPathStr();
}

VOID CThreadAccServerSocket::ClearPolicyInfo()
{
	pthread_mutex_lock (&g_StrMapMutex);
	memset(&g_stPolicyInfo, 0, sizeof(ASI_POLICY_INFO));
	pthread_mutex_unlock (&g_StrMapMutex);
}

INT32 CThreadAccServerSocket::GetAllowExPathCount()
{
	INT32 nCount = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	nCount = g_tAllowEXPathStrMap.size();
	pthread_mutex_unlock (&g_StrMapMutex);
	return nCount;
}

INT32 CThreadAccServerSocket::IsAllowExPathStr(CString strPath)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tAllowEXPathStrMap.find(strPath);
	if(find != g_tAllowEXPathStrMap.end())
	{
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::AddAllowExPathStr(CString strPath, CString strHash)
{
	if(IsAllowExPathStr(strPath) == 1)
		return;
	pthread_mutex_lock (&g_StrMapMutex);
	g_tAllowEXPathStrMap[strPath] = strHash;
	pthread_mutex_unlock (&g_StrMapMutex);
}

INT32 CThreadAccServerSocket::FindAllowExPathStr(CString strPath, CString &strHash)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tAllowEXPathStrMap.find(strPath);
	if(find != g_tAllowEXPathStrMap.end())
	{
		strHash = find->second;
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

INT32 CThreadAccServerSocket::GetDenyExPathCount()
{
	INT32 nCount = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	nCount = g_tDenyEXPathStrMap.size();
	pthread_mutex_unlock (&g_StrMapMutex);
	return nCount;
}

INT32 CThreadAccServerSocket::IsDenyExPathStr(CString strPath)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tDenyEXPathStrMap.find(strPath);
	if(find != g_tDenyEXPathStrMap.end())
	{
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::AddDenyExPathStr(CString strPath, CString strHash)
{
	if(IsDenyExPathStr(strPath) == 1)
		return;
	pthread_mutex_lock (&g_StrMapMutex);
	g_tDenyEXPathStrMap[strPath] = strHash;
	pthread_mutex_unlock (&g_StrMapMutex);
}

INT32 CThreadAccServerSocket::FindDenyExPathStr(CString strPath, CString &strHash)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tDenyEXPathStrMap.find(strPath);
	if(find != g_tDenyEXPathStrMap.end())
	{
		strHash = find->second;
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::ClearExPathStr()
{
	pthread_mutex_lock (&g_StrMapMutex);
	g_tAllowEXPathStrMap.clear();
	g_tDenyEXPathStrMap.clear();
	pthread_mutex_unlock (&g_StrMapMutex);
}


INT32 CThreadAccServerSocket::GetAllowSpPathCount()
{
	INT32 nCount = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	nCount = g_tAllowSPPathStrMap.size();
	pthread_mutex_unlock (&g_StrMapMutex);
	return nCount;
}

INT32 CThreadAccServerSocket::IsAllowSpPathStr(CString strPath)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tAllowSPPathStrMap.find(strPath);
	if(find != g_tAllowSPPathStrMap.end())
	{
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::AddAllowSpPathStr(CString strPath, CString strHash)
{
	if(IsAllowSpPathStr(strPath) == 1)
		return;

	pthread_mutex_lock (&g_StrMapMutex);
	g_tAllowSPPathStrMap[strPath] = strHash;
	pthread_mutex_unlock (&g_StrMapMutex);
}

INT32 CThreadAccServerSocket::FindAllowSpPathStr(CString strPath, CString &strHash)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tAllowSPPathStrMap.find(strPath);
	if(find != g_tAllowSPPathStrMap.end())
	{
		strHash = find->second;
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

INT32 CThreadAccServerSocket::GetDenySpPathCount()
{
	INT32 nCount = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	nCount = g_tDenySPPathStrMap.size();
	pthread_mutex_unlock (&g_StrMapMutex);
	return nCount;
}

INT32 CThreadAccServerSocket::IsDenySpPathStr(CString strPath)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tDenySPPathStrMap.find(strPath);
	if(find != g_tDenySPPathStrMap.end())
	{
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::AddDenySpPathStr(CString strPath, CString strHash)
{
	if(IsDenySpPathStr(strPath) == 1)
		return;

	pthread_mutex_lock (&g_StrMapMutex);
	g_tDenySPPathStrMap[strPath] = strHash;
	pthread_mutex_unlock (&g_StrMapMutex);
}

INT32 CThreadAccServerSocket::FindDenySpPathStr(CString strPath, CString &strHash)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tDenySPPathStrMap.find(strPath);
	if(find != g_tDenySPPathStrMap.end())
	{
		strHash = find->second;
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::ClearSpPathStr()
{
	pthread_mutex_lock (&g_StrMapMutex);
	g_tDenySPPathStrMap.clear();
	g_tAllowSPPathStrMap.clear();
	pthread_mutex_unlock (&g_StrMapMutex);
}

INT32 CThreadAccServerSocket::GetWlPathCount()
{
	INT32 nCount = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	nCount = g_tWLPathStrMap.size();
	pthread_mutex_unlock (&g_StrMapMutex);
	return nCount;
}

INT32 CThreadAccServerSocket::IsWlPathStr(CString strPath)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tWLPathStrMap.find(strPath);
	if(find != g_tWLPathStrMap.end())
	{
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::AddWlPathStr(CString strPath, CString strHash)
{
	if(IsWlPathStr(strPath) == 1)
		return;

	pthread_mutex_lock (&g_StrMapMutex);
	g_tWLPathStrMap[strPath] = strHash;
	pthread_mutex_unlock (&g_StrMapMutex);
}

INT32 CThreadAccServerSocket::FindWlPathStr(CString strPath, CString &strHash)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tWLPathStrMap.find(strPath);
	if(find != g_tWLPathStrMap.end())
	{
		strHash = find->second;
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::ClearWlPathStr()
{
	pthread_mutex_lock (&g_StrMapMutex);
	g_tWLPathStrMap.clear();
	pthread_mutex_unlock (&g_StrMapMutex);
}


INT32 CThreadAccServerSocket::GetBlPathCount()
{
	INT32 nCount = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	nCount = g_tBLPathStrMap.size();
	pthread_mutex_unlock (&g_StrMapMutex);
	return nCount;
}

INT32 CThreadAccServerSocket::IsBlPathStr(CString strPath)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tBLPathStrMap.find(strPath);
	if(find != g_tBLPathStrMap.end())
	{
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::AddBlPathStr(CString strPath, CString strHash)
{
	if(IsBlPathStr(strPath) == 1)
		return;
	pthread_mutex_lock (&g_StrMapMutex);
	g_tBLPathStrMap[strPath] = strHash;
	pthread_mutex_unlock (&g_StrMapMutex);
}

INT32 CThreadAccServerSocket::FindBlPathStr(CString strPath, CString &strHash)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tBLPathStrMap.find(strPath);
	if(find != g_tBLPathStrMap.end())
	{
		strHash = find->second;
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::ClearBlPathStr()
{
	pthread_mutex_lock (&g_StrMapMutex);
	g_tBLPathStrMap.clear();
	pthread_mutex_unlock (&g_StrMapMutex);
}

INT32 CThreadAccServerSocket::GetCrPathCount()
{
	INT32 nCount = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	nCount = g_tCRPathStrMap.size();
	pthread_mutex_unlock (&g_StrMapMutex);
	return nCount;
}

INT32 CThreadAccServerSocket::IsCrPathStr(CString strPath)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tCRPathStrMap.find(strPath);
	if(find != g_tCRPathStrMap.end())
	{
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::AddCrPathStr(CString strPath, CString strHash)
{
	if(IsCrPathStr(strPath) == 1)
		return;
	pthread_mutex_lock (&g_StrMapMutex);
	g_tCRPathStrMap[strPath] = strHash;
	pthread_mutex_unlock (&g_StrMapMutex);
}

INT32 CThreadAccServerSocket::FindCrPathStr(CString strPath, CString &strHash)
{
	INT32 nRtn = 0;
	pthread_mutex_lock (&g_StrMapMutex);
	TMapCStrItor find = g_tCRPathStrMap.find(strPath);
	if(find != g_tCRPathStrMap.end())
	{
		strHash = find->second;
		nRtn = 1;
	}
	pthread_mutex_unlock (&g_StrMapMutex);
	return nRtn;
}

VOID CThreadAccServerSocket::ClearCrPathStr()
{
	pthread_mutex_lock (&g_StrMapMutex);
	g_tCRPathStrMap.clear();
	pthread_mutex_unlock (&g_StrMapMutex);
}


INT32 CThreadAccServerSocket::SockRecv(INT32 nFd, PVOID pRecvData, INT32 nReqSize)
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

		nRecvSize = read(nFd, &pcRecv[nRecvedSize], nOnceRecvSize);
		if (nRecvSize == -1)
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
				return -5;
			}
		}
		else if (nRecvSize == 0)
		{
			return -6;
		}
		else
		{
			nRecvedSize += nRecvSize;
		}
	}
	return 0;
}

INT32 CThreadAccServerSocket::SockWrite(INT32 nFd, PVOID pWriteData, INT32 nReqSize)
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

INT32		CThreadAccServerSocket::SendExitThreadCmd()
{
	INT32 nClientFd = -1;
	INT32 nRetVal = 0;
	INT32 nState = 0;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	INT32 nClientLen = 0;
	struct sockaddr_un stClientAddr;
	char acSockPath[MAX_PATH] = {0,};
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)g_pNotifyWnd;

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
				t_AccNotifyDlg->WriteLogE("[acc_svr_sock] fail to allocate memory (%d)", errno);
			nRetVal = -1;
			break;
		}
		memset(pChkFileProc, 0, nSize);

		nClientFd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (nClientFd == -1)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogE("[acc_svr_sock] fail to create sock (%d)", errno);
			nRetVal = -2;
			break;
		}

		snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_ACC_FILE);
		acSockPath[MAX_FILE_NAME-1] = 0;
		nClientLen = sizeof(stClientAddr);
		memset(&stClientAddr, 0, nClientLen);
		stClientAddr.sun_family = AF_UNIX;
		strncpy(stClientAddr.sun_path, acSockPath, MAX_FILE_NAME-1);
		nRetVal = connect(nClientFd, (struct sockaddr *)&stClientAddr, nClientLen);
		if (nRetVal < 0)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogE("[acc_svr_sock] fail to connect %s (%d)", acSockPath, errno);
			nRetVal = -3;
			break;
		}

		pChkFileProc->nCmdId = CMD_PIPE_EXIT_THREAD;
		nRetVal = SockWrite(nClientFd, (PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogE("[acc_svr_sock] fail to write data (%d) (%d)", nRetVal, errno);
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

UINT32 CThreadAccServerSocket::GetSockContinue()
{
	UINT32 nContinue = 0;
	pthread_mutex_lock (&g_SockMutex);
	nContinue = g_nSockContinue;
	pthread_mutex_unlock (&g_SockMutex);
	return nContinue;
}


VOID CThreadAccServerSocket::SetSockContinue(UINT32 nContinue)
{
	pthread_mutex_lock (&g_SockMutex);
	g_nSockContinue = nContinue;	
	pthread_mutex_unlock (&g_SockMutex);
}


INT32 CThreadAccServerSocket::CheckThread(INT32 nClientFd)
{
	INT32 nRetVal = 0;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)g_pNotifyWnd;
	PASI_ACC_NOTIFY_POLICY pAccNotiPol = NULL;
	if(nClientFd < 0)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[acc_svr_sock] invalid input data : [%d]", nRetVal);
		return -1;
	}

	pAccNotiPol = (PASI_ACC_NOTIFY_POLICY)malloc(sizeof(ASI_ACC_NOTIFY_POLICY));
	if(pAccNotiPol == NULL)
	{
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[acc_svr_sock] fail to allocate memory : [%d]", errno);
		close(nClientFd);
		SetClientFd(-1);
		return -2;
	}

	SetCheckThreadState(ASI_CONNECT_STATE);
	SetSockContinue(1);

	while(GetSockContinue())
	{
		memset(pAccNotiPol, 0, sizeof(ASI_ACC_NOTIFY_POLICY));
		nRetVal = CheckSockEvent(nClientFd, pAccNotiPol);
		if(nRetVal != 0)
		{
			if(nRetVal != -13)
			{
				if(t_AccNotifyDlg)
					t_AccNotifyDlg->WriteLogE("[acc_svr_sock] fail to check sock event : [%d][%d]", nRetVal, errno);
			}
			close(nClientFd);
			SetClientFd(-1);
			safe_free(pAccNotiPol);
			SetCheckThreadState(ASI_DISCONNECT_STATE);
			SetSockContinue(0);
			return nRetVal;
		}
	}	
	SetCheckThreadState(ASI_DISCONNECT_STATE);
	close(nClientFd);
	SetClientFd(-1);
	safe_free(pAccNotiPol);
	return 0;
}

INT32 CThreadAccServerSocket::Run()
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 i, nRetVal = 0;
	INT32 nServerFd = -1;
	INT32 nClientFd = -1;
	INT32 nState = 0;
	pthread_t tid = 0;
	struct sockaddr_un stClientAddr;
	socklen_t nClientLen = sizeof(stClientAddr);
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)g_pNotifyWnd;

	nRetVal = InitUnixSock(nServerFd);
	if(nRetVal == 0)
	{
		m_nPause = 0;
		SetOpMode(1);
	}
	else
	{
		m_nPause = 1;
		if(t_AccNotifyDlg)
			t_AccNotifyDlg->WriteLogE("[acc_svr_sock] fail to init notify event : [%d]", nRetVal);
	}

	m_nRunFlag = 1;
	if(t_AccNotifyDlg)
		t_AccNotifyDlg->WriteLogN("start acc_svr_sock thread : [%d]", m_nRunFlag);

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
					t_AccNotifyDlg->WriteLogE("[acc_svr_sock] fail to accept : [%d]", errno);
				UninitUnixSock();
				m_nPause = 1;
				SetOpMode(0);
				Sleep(1000);
			}
			nRetVal = CheckThread(nClientFd);
			if(nRetVal == 1111)
			{
				SetOpMode(0);
				break;
			}
			else
			{
				SetOpMode(0);
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
				SetOpMode(1);
				
			}
			else
			{
				m_nPause = 1;
				Sleep(1000);
			}
		}
	}

	if(t_AccNotifyDlg)
		t_AccNotifyDlg->WriteLogN("[acc_svr_sock] stop thread by sub continue flag");
	ClearAllInfo();
	UninitUnixSock();
	return 0;
}



INT32		CThreadAccServerSocket::CheckSockEvent(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	CString strPath;
	CString strHash;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)g_pNotifyWnd;
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
		else if(pAccNotiPol->nCmdId == CMD_PIPE_ALIVE_MESSAGE)
		{
			nRetVal = 0;
			break;
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_START_POLICY_INFO)
		{
			SetPolicyUse(0);
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogN("[acc_svr_sock] start to set policy");
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_SET_POLICY_INFO)
		{
			SetPolicyInfo(&pAccNotiPol->stPolInfo);
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogN("[acc_svr_sock] set policy info");
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_ADD_WHITE_FILE)
		{
			if(pAccNotiPol->stFileHash.acFullPath[0] != 0 && pAccNotiPol->stFileHash.acWhiteHash[0] != 0)
			{
				strPath.Format("%s", pAccNotiPol->stFileHash.acFullPath);
				strHash.Format("%s", pAccNotiPol->stFileHash.acWhiteHash);
				AddWlPathStr(strPath, strHash);
				if(t_AccNotifyDlg)
					t_AccNotifyDlg->WriteLogN("[acc_svr_sock] add white file : [%s]", pAccNotiPol->stFileHash.acFullPath);
			}
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_CLEAR_WHITE_FILE)
		{
			ClearWlPathStr();
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogN("[acc_svr_sock] clear white file");
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_ADD_BLACK_FILE)
		{
			if(pAccNotiPol->stFileHash.acFullPath[0] != 0 && pAccNotiPol->stFileHash.acWhiteHash[0] != 0)
			{
				strPath.Format("%s", pAccNotiPol->stFileHash.acFullPath);
				strHash.Format("%s", pAccNotiPol->stFileHash.acWhiteHash);
				AddBlPathStr(strPath, strHash);
				if(t_AccNotifyDlg)
					t_AccNotifyDlg->WriteLogN("[acc_svr_sock] add black file : [%s]", pAccNotiPol->stFileHash.acFullPath);
			}
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_CLEAR_BLACK_FILE)
		{
			ClearBlPathStr();
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogN("[acc_svr_sock] clear black file");
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_ADD_CREATE_FILE)
		{
			if(pAccNotiPol->stFileHash.acFullPath[0] != 0 && pAccNotiPol->stFileHash.acWhiteHash[0] != 0)
			{
				strPath.Format("%s", pAccNotiPol->stFileHash.acFullPath);
				strHash.Format("%s", pAccNotiPol->stFileHash.acWhiteHash);
				AddCrPathStr(strPath, strHash);
				if(t_AccNotifyDlg)
					t_AccNotifyDlg->WriteLogN("[acc_svr_sock] add create file : [%s]", pAccNotiPol->stFileHash.acFullPath);
			}
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_CLEAR_CREATE_FILE)
		{
			ClearCrPathStr();
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogN("[acc_svr_sock] clear create file");

		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_ADD_ALLOW_EX_FILE)
		{
			if(pAccNotiPol->stFileHash.acFullPath[0] != 0 && pAccNotiPol->stFileHash.acWhiteHash[0] != 0)
			{
				strPath.Format("%s", pAccNotiPol->stFileHash.acFullPath);
				strHash.Format("%s", pAccNotiPol->stFileHash.acWhiteHash);
				AddAllowExPathStr(strPath, strHash);
				if(t_AccNotifyDlg)
					t_AccNotifyDlg->WriteLogN("[acc_svr_sock] add allow ex file : [%s]", pAccNotiPol->stFileHash.acFullPath);
			}
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_ADD_DENY_EX_FILE)
		{
			if(pAccNotiPol->stFileHash.acFullPath[0] != 0 && pAccNotiPol->stFileHash.acWhiteHash[0] != 0)
			{
				strPath.Format("%s", pAccNotiPol->stFileHash.acFullPath);
				strHash.Format("%s", pAccNotiPol->stFileHash.acWhiteHash);
				AddDenyExPathStr(strPath, strHash);
				if(t_AccNotifyDlg)
					t_AccNotifyDlg->WriteLogN("[acc_svr_sock] add deny ex file : [%s]", pAccNotiPol->stFileHash.acFullPath);
			}
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_CLEAR_EX_FILE)
		{
			ClearExPathStr();
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogN("[acc_svr_sock] clear ex file");
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_ADD_DENY_SP_FILE)
		{
			if(pAccNotiPol->stFileHash.acFullPath[0] != 0 && pAccNotiPol->stFileHash.acWhiteHash[0] != 0)
			{
				strPath.Format("%s", pAccNotiPol->stFileHash.acFullPath);
				strHash.Format("%s", pAccNotiPol->stFileHash.acWhiteHash);
				AddDenySpPathStr(strPath, strHash);
				if(t_AccNotifyDlg)
					t_AccNotifyDlg->WriteLogN("[acc_svr_sock] add deny sp file : [%s]", pAccNotiPol->stFileHash.acFullPath);
			}
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_ADD_ALLOW_SP_FILE)
		{
			if(pAccNotiPol->stFileHash.acFullPath[0] != 0 && pAccNotiPol->stFileHash.acWhiteHash[0] != 0)
			{
				strPath.Format("%s", pAccNotiPol->stFileHash.acFullPath);
				strHash.Format("%s", pAccNotiPol->stFileHash.acWhiteHash);
				AddAllowSpPathStr(strPath, strHash);
				if(t_AccNotifyDlg)
					t_AccNotifyDlg->WriteLogN("[acc_svr_sock] add allow sp file : [%s]", pAccNotiPol->stFileHash.acFullPath);
			}
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_CLEAR_SP_FILE)
		{
			ClearSpPathStr();
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogN("[acc_svr_sock] clear sp file");
		}
		else if(pAccNotiPol->nCmdId == CMD_PIPE_END_POLICY_INFO)
		{
			SetPolicyUse(1);
			if(t_AccNotifyDlg)
				t_AccNotifyDlg->WriteLogN("[acc_svr_sock] end to set policy");
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

