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

// ThreadPoInPtnFile.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadPoInPtnFile.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInPtnFile

CThreadPoInPtnFile*	t_ThreadPoInPtnFile = NULL;

CThreadPoInPtnFile::CThreadPoInPtnFile()
{
	m_nOrderID = 0;
	m_nPtnRetCount = 0;
	m_nServerFd = -1;
	m_nClientFd = -1;
	m_nTestTime = 0;
	m_fTotalDiffTime = 0;
	m_nTestCount = 0;
	m_nCheckThread = ASI_DISCONNECT_STATE;
	pthread_mutex_init(&m_SockMutex, NULL);
	pthread_mutex_init(&m_PtnRetMutex, NULL);
}

CThreadPoInPtnFile::~CThreadPoInPtnFile()
{
	pthread_mutex_destroy(&m_SockMutex);
	pthread_mutex_destroy(&m_PtnRetMutex);
}

BOOL CThreadPoInPtnFile::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoInPtnFile::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInPtnFile message handlers

INT32 CThreadPoInPtnFile::InitUnixSock(INT32 &nSrvFd)
{
	INT32 nRetVal = 0;
	INT32 nOptVal = 1;
	struct sockaddr_un serveraddr;
	char acSockPath[MAX_FILE_NAME] = {0,};
	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_FILE);
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

VOID CThreadPoInPtnFile::UninitUnixSock()
{
	char acSockPath[MAX_FILE_NAME] = {0,};
	pthread_mutex_lock (&m_SockMutex);
	if(m_nServerFd != -1)
	{
		close(m_nServerFd);
		m_nServerFd = -1;
	}
	pthread_mutex_unlock (&m_SockMutex);

	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_FILE);
	acSockPath[MAX_FILE_NAME-1] = 0;
	if(is_file(acSockPath) == SOCK_FILE)
	{
		unlink(acSockPath);
	}
}

INT32	CThreadPoInPtnFile::IsInitLogic()
{
	INT32 i = 0;
	INT32 nRetVal = -1;
	for(i=0; i<30; i++)
	{
		if(t_LogicMgrPtnGBO->IsInitLogic() || t_LogicMgrPtnGWO->IsInitLogic())
		{
			nRetVal = 0;
			break;
		}
		Sleep(300);
	}
	return nRetVal;
}

INT32	CThreadPoInPtnFile::InitNotifyEvent(INT32 &nSrvFd)
{
	INT32 nRetVal = 0;
	INT32 nSFd = -1;
	do{
		nRetVal = CheckWhitePatternFile();
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}
		nRetVal = IsInitLogic();
		if(nRetVal < 0)
		{
			nRetVal -= 200;
			break;
		}
		nRetVal = InitUnixSock(nSFd);
		if(nRetVal < 0)
		{
			nRetVal -= 300;
			break;
		}
		nSrvFd = nSFd;
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

INT32	CThreadPoInPtnFile::GetCheckThreadState()
{
	INT32 nFlag = 0;
	pthread_mutex_lock (&m_SockMutex);
	nFlag = m_nCheckThread;
	pthread_mutex_unlock (&m_SockMutex);
	return nFlag;
}


VOID	CThreadPoInPtnFile::SetCheckThreadState(INT32 nState)
{
	pthread_mutex_lock (&m_SockMutex);
	m_nCheckThread = nState;
	pthread_mutex_unlock (&m_SockMutex);
}

INT32	CThreadPoInPtnFile::GetClientFd()
{
	INT32 nFd = 0;
	pthread_mutex_lock (&m_SockMutex);
	nFd = m_nClientFd;
	pthread_mutex_unlock (&m_SockMutex);
	return nFd;
}


VOID	CThreadPoInPtnFile::SetClientFd(INT32 nFd)
{
	pthread_mutex_lock (&m_SockMutex);
	m_nClientFd = nFd;
	pthread_mutex_unlock (&m_SockMutex);
}


INT32 CThreadPoInPtnFile::SockRecv(INT32 nFd, PVOID pRecvData, INT32 nReqSize)
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

INT32 CThreadPoInPtnFile::SockWrite(INT32 nFd, PVOID pWriteData, INT32 nReqSize)
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

INT32		CThreadPoInPtnFile::SendExitThreadCmd()
{
	INT32 nClientFd = -1;
	INT32 nRetVal = 0;
	INT32 nState = 0;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
	INT32 nClientLen = 0;
	struct sockaddr_un stClientAddr;
	char acSockPath[MAX_PATH] = {0,};

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
			WriteLogE("[po_in_ptn_file] fail to allocate memory (%d)", errno);
			nRetVal = -1;
			break;
		}
		memset(pChkFileProc, 0, nSize);

		nClientFd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (nClientFd == -1)
		{
			WriteLogE("[po_in_ptn_file] fail to create sock (%d)", errno);
			nRetVal = -2;
			break;
		}

		snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_FILE);
		acSockPath[MAX_FILE_NAME-1] = 0;
		nClientLen = sizeof(stClientAddr);
		memset(&stClientAddr, 0, nClientLen);
		stClientAddr.sun_family = AF_UNIX;
		strncpy(stClientAddr.sun_path, acSockPath, MAX_FILE_NAME-1);
		nRetVal = connect(nClientFd, (struct sockaddr *)&stClientAddr, nClientLen);
		if (nRetVal < 0)
		{
			WriteLogE("[po_in_ptn_file] fail to connect %s (%d)", acSockPath, errno);
			nRetVal = -3;
			break;
		}

		pChkFileProc->nCmdId = CMD_PIPE_EXIT_THREAD;
		nRetVal = SockWrite(nClientFd, (PVOID)pChkFileProc, nSize);
		if(nRetVal < 0)
		{
			WriteLogE("[po_in_ptn_file] fail to write data (%d) (%d)", nRetVal, errno);
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

INT32 CThreadPoInPtnFile::CheckThread(INT32 nClientFd)
{
	INT32 nRetVal = 0;
	PASI_CHK_PTN_FILE pChkPtnFile = NULL;
	if(nClientFd < 0)
	{
		WriteLogE("invalid input data : [%s][%d]", m_strThreadName.c_str(), nRetVal);
		return -1;
	}

	pChkPtnFile = (PASI_CHK_PTN_FILE)malloc(sizeof(ASI_CHK_PTN_FILE));
	if(pChkPtnFile == NULL)
	{
		WriteLogE("fail to allocate memory : [%s][%d]", m_strThreadName.c_str(), errno);
		close(nClientFd);
		SetClientFd(-1);
		return -2;
	}

	SetCheckThreadState(ASI_CONNECT_STATE);
	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		memset(pChkPtnFile, 0, sizeof(ASI_CHK_PTN_FILE));
		nRetVal = CheckSockEvent(nClientFd, pChkPtnFile);
		if(nRetVal != 0)
		{
			if(nRetVal != -13)
				WriteLogE("fail to check sock event : [%s][%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
			close(nClientFd);
			SetClientFd(-1);
			ClearPtnRet();
			safe_free(pChkPtnFile);
			SetCheckThreadState(ASI_DISCONNECT_STATE);
			return nRetVal;
		}
	}	
	SetCheckThreadState(ASI_DISCONNECT_STATE);
	close(nClientFd);
	SetClientFd(-1);
	ClearPtnRet();
	safe_free(pChkPtnFile);
	return 0;
}

INT32 CThreadPoInPtnFile::Run()
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 i, nRetVal = 0;
	INT32 nServerFd = -1;
	INT32 nClientFd = -1;
	INT32 nState = 0;
	pthread_t tid = 0;
	struct sockaddr_un stClientAddr;
	socklen_t nClientLen = sizeof(stClientAddr);

	tid = syscall(SYS_gettid);
	nRetVal = setpriority(PRIO_PROCESS, tid, -10);
	if(nRetVal < 0)
	{
		WriteLogE("fail to set priority: [chk_thread][%d]", errno);
		return -2;
	}

	nRetVal = LoadWhitePattern();
	if(nRetVal < 0)
	{
		WriteLogE("fail to load white pattern : [%s][%d]", m_strThreadName.c_str(), nRetVal);
		return -1;
	}

	nRetVal = InitNotifyEvent(nServerFd);
	if(nRetVal == 0)
	{
		m_nPause = 0;
	}
	else
	{
		m_nPause = 1;
		WriteLogE("fail to init notify event : [%s][%d]", m_strThreadName.c_str(), nRetVal);
	}

	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	SetCheckThreadState(ASI_DISCONNECT_STATE);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{
			SetCheckThreadState(ASI_ACCEPT_STATE);
			nClientFd = accept(nServerFd, (struct sockaddr *)&stClientAddr, &nClientLen);
			if (nClientFd < 0)
			{
				WriteLogE("fail to accept : [%s][%d]", m_strThreadName.c_str(), errno);
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
			nRetVal = InitNotifyEvent(nServerFd);
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

	if(!t_EnvInfoOp)							
		WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())
		WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())
		WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	UninitUnixSock();
	UnloadWhitePattern();
	m_tOrderIDMap.clear();
	return 0;
}


INT32 CThreadPoInPtnFile::CheckWhitePatternFile()
{
	INT32 nRetVal = 0;
	DIR *pDir = NULL;
	struct dirent *pDrp = NULL;
	CHAR acPath[MAX_PATH] = {0, };
	snprintf(acPath, MAX_PATH-1, "%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE);
	acPath[MAX_PATH-1] = 0;

	nRetVal = is_file(acPath);
	if(nRetVal < 0)
	{
		return -1;
	}
	pDir = opendir(acPath);
	if (pDir == NULL)
		return -2;

	while ((pDrp = readdir(pDir)) != NULL)
	{
		if(!_stricmp(pDrp->d_name, ".") || !_stricmp(pDrp->d_name, ".."))
		{
			continue;
		}

		if(DT_REG == pDrp->d_type)
		{
			closedir(pDir);
			return 0;
		}
	}

	closedir(pDir);
	return -3;
}

INT32 CThreadPoInPtnFile::LoadWhitePattern()
{
	INT32 nRetVal = 0;
	CHAR acPath[MAX_PATH] = {0, };

	snprintf(acPath, MAX_PATH-1, "%s/asi_weng.so", t_EnvInfo->m_strDLLPath.c_str());
	acPath[MAX_PATH-1] = 0;

	nRetVal = m_tWEDLLUtil.LoadLibraryExt(acPath);
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to load library  : [%s][%d][%d]", m_strThreadName.c_str(), acPath, nRetVal, errno);
		return -2;
	}
	nRetVal = m_tWEDLLUtil.Init();
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to init we dll  : [%d]", m_strThreadName.c_str(), nRetVal);
		return -3;
	}
	nRetVal = m_tWEDLLUtil.InitDB();
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to init we db  : [%d]", m_strThreadName.c_str(), nRetVal);
		return -4;
	}
	return 0;
}


//---------------------------------------------------------------------------

VOID CThreadPoInPtnFile::UnloadWhitePattern()
{
	m_tWEDLLUtil.ClearFile();
	m_tWEDLLUtil.FreeLibraryExt();
}




BOOL CThreadPoInPtnFile::AddPtnRet(char *acPath, ASI_RET_INFO stRetInfo)
{
	String strPath;
	if(acPath == NULL || acPath[0] == 0)
		return FALSE;
	strPath = acPath;
	pthread_mutex_lock (&m_PtnRetMutex);
	m_tPtnRetMap[strPath] = stRetInfo;
	m_nPtnRetCount++;
	pthread_mutex_unlock (&m_PtnRetMutex);
	return TRUE;
}

BOOL CThreadPoInPtnFile::GetPtnRet(char *acPath, PASI_RET_INFO pRetInfo)
{
	BOOL bRetVal = FALSE;
	String strPath;
	TMapPtnRetExItor find;
	if(acPath == NULL || acPath[0] == 0 || pRetInfo == NULL)
		return FALSE;

	strPath = acPath;
	pthread_mutex_lock (&m_PtnRetMutex);
	find = m_tPtnRetMap.find(strPath);
	if(find != m_tPtnRetMap.end())
	{
		memcpy(pRetInfo, (PASI_RET_INFO)&(find->second), sizeof(ASI_RET_INFO));
		bRetVal = TRUE;
	}
	else
	{
		bRetVal = FALSE;
	}
	pthread_mutex_unlock (&m_PtnRetMutex);

	return bRetVal;
}


UINT32 CThreadPoInPtnFile::GetPtnRetCount()
{
	UINT32 nCount = 0;
	pthread_mutex_lock (&m_PtnRetMutex);
	nCount = m_nPtnRetCount;
	pthread_mutex_unlock (&m_PtnRetMutex);
	return nCount;
}

BOOL CThreadPoInPtnFile::DelPtnRet(char *acPath)
{
	String strPath;
	if(acPath == NULL || acPath[0] == 0)
		return FALSE;
	strPath = acPath;
	pthread_mutex_lock (&m_PtnRetMutex);
	m_tPtnRetMap.erase(strPath);
	if(m_nPtnRetCount > 0)
		m_nPtnRetCount--;
	pthread_mutex_unlock (&m_PtnRetMutex);
	return TRUE;
}


void CThreadPoInPtnFile::ClearPtnRet()
{
	pthread_mutex_lock (&m_PtnRetMutex);
	if(m_nPtnRetCount != 0)
	{
		m_tPtnRetMap.clear();
		m_nPtnRetCount = 0;
	}
	pthread_mutex_unlock (&m_PtnRetMutex);
}



BOOL	CThreadPoInPtnFile::ParseFilePath(PASI_CHK_INFO pInfo)
{
	INT32 i, nLen = 0;
	BOOL bIsSep = FALSE;
	if (pInfo == NULL || pInfo->acFullPath[0] == 0 || pInfo->nLen < 1)
	{
		return FALSE;
	}
	nLen = pInfo->nLen;
	for(i=nLen-1; i>=0; i--)
	{
		if(pInfo->acFullPath[i] == '/')	
		{
			bIsSep = TRUE;
			break;
		}
	}
	if(bIsSep == FALSE || i<0)
		return FALSE;

	if(i==0)
	{
		pInfo->acPath[0] = '/';
		pInfo->acPath[1] = '\0';
		if(nLen < MAX_FILE_NAME)
		{
			strncpy(pInfo->acFile, &pInfo->acFullPath[1], nLen-1);
			pInfo->acFile[nLen-1] = '\0'; 
		}
		else
		{
			strncpy(pInfo->acFile, &pInfo->acFullPath[1], MAX_FILE_NAME-1);
			pInfo->acFile[MAX_FILE_NAME-1] = '\0'; 
		}
	}
	else
	{
		strncpy(pInfo->acFile, &pInfo->acFullPath[i+1], MAX_FILE_NAME-1);
		pInfo->acFile[MAX_FILE_NAME-1] = '\0'; 
		strncpy(pInfo->acPath, pInfo->acFullPath, i);
		pInfo->acPath[i] = '\0'; 
	}
	return TRUE;
}

BOOL	CThreadPoInPtnFile::GetFilePathFromFd(INT32 nFd, PASI_CHK_INFO pFileInfo)
{
	ssize_t nLen = 0;
	char acPath[MAX_TYPE_LEN] = {0,};

	if (nFd < 1 || pFileInfo == NULL)
	{
		return FALSE;
	}

	snprintf (acPath, MAX_TYPE_LEN-1, "/proc/self/fd/%d", nFd);
	acPath[MAX_TYPE_LEN-1] = 0;
	nLen = readlink (acPath, pFileInfo->acFullPath, MAX_PATH-1);
	if (nLen < 2 || nLen > MAX_PATH-1)
	{
		return FALSE;
	}
	pFileInfo->acFullPath[nLen] = '\0';
	pFileInfo->nLen = (INT32)nLen;

	if(nLen > 3 && !strcmp(&pFileInfo->acFullPath[nLen-3], ".so"))
	{
		return FALSE;
	}
	if(ParseFilePath(pFileInfo) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL	CThreadPoInPtnFile::GetProcPathFromPid(INT32 nPid, PASI_CHK_INFO pProcInfo)
{
	ssize_t nLen = 0;
	char acPath[MAX_TYPE_LEN] = {0,};

	if (nPid < 1 || pProcInfo == NULL)
	{
		return FALSE;
	}

	snprintf (acPath, MAX_TYPE_LEN-1, "/proc/%d/exe", nPid);
	acPath[MAX_TYPE_LEN-1] = 0;
	nLen = readlink (acPath, pProcInfo->acFullPath, MAX_PATH-1);
	if (nLen < 2 || nLen > MAX_PATH-1)
	{
		return FALSE;
	}
	pProcInfo->acFullPath[nLen] = '\0';
	pProcInfo->nLen = (INT32)nLen;
	if(ParseFilePath(pProcInfo) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL	CThreadPoInPtnFile::IsMatchFile(char *pFilePath, char *pcFeKey, DB_FILE_INFO& tSpecFI, UINT32 nChkType, CHAR chTok)
{
	BOOL bIsHashMatch = FALSE;
	BOOL bIsPathMatch = FALSE;
	if(pFilePath == NULL || pcFeKey == NULL)
		return FALSE;

	if(tSpecFI.strHash.empty() == FALSE)
	{
		if(!_stricmp(pcFeKey, tSpecFI.strHash.c_str()))
			bIsHashMatch = TRUE;
	}

	if(tSpecFI.strPath.empty() == FALSE)
	{
		if(!_stricmp(pFilePath, tSpecFI.strPath.c_str()))
			bIsPathMatch = TRUE;
	}

	if(nChkType == SS_MATCH_FILTER_CHK_TYPE_OR)
	{
		if(bIsHashMatch == TRUE || bIsPathMatch == TRUE)
			return TRUE;
	}
	else
	{
		if(bIsHashMatch == TRUE && bIsPathMatch == TRUE)
			return TRUE;
	}	

	return FALSE;
}

INT32		CThreadPoInPtnFile::ChkInPtnEx(char *pFilePath, char *pcFeKey, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType, UINT32& nExtOption)
{
	INT32 nRtn = 0;

	if(pcFeKey == NULL || pFilePath == NULL || t_ManagePoInPtnExPkg == NULL)
		return 0;

	PDB_PO_IN_PTN_OP pOpPolicy = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
	if(pOpPolicy == NULL)
		return 0;

	UINT32 nOpUsedMode = pOpPolicy->tDPH.nUsedMode;
	if(nOpUsedMode == STATUS_USED_MODE_OFF)
	{
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		nIsWarning = 1;
		return 1;
	}
	if(nOpUsedMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	PDB_PO_IN_PTN_EX pPtnExPolicy = (PDB_PO_IN_PTN_EX)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_EX);
	if(pPtnExPolicy == NULL)
		return 0;

	UINT32 nPoUsedMode = pPtnExPolicy->tDPH.nUsedMode;
	if(nPoUsedMode == STATUS_USED_MODE_OFF)
		return 0;
	if(nPoUsedMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	TListID tIDList;
	TListIDItor begin, end;
	t_ManagePoInPtnExPkg->GetUnitIDListByChkOrder(tIDList);

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_PTN_EX_UNIT pdata_unit = t_ManagePoInPtnExUnit->FindItem(*begin);
		if(!pdata_unit)
			continue;

		if(pdata_unit->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
			continue;		

		if(IsMatchFile(pFilePath, pcFeKey, pdata_unit->tDFI, pdata_unit->nFltChkType, '\f') == FALSE)
			continue;

		if(pdata_unit->tDPH.nUsedMode == STATUS_USED_MODE_WARN)
			nIsWarning = 1;

		nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_EX + pdata_unit->tDPH.nID;
		nBlockMode = pdata_unit->nBlockMode;
		nExtOption = pdata_unit->tDPH.nExtOption;

		nRtn = 1;
		break;
	}
	return nRtn;
}


INT32		CThreadPoInPtnFile::ChkInPtnSP(char *pPath, char *pFile, char *pcFeKey, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType)
{
	INT32 nRtn = 0;
	PDB_PO_IN_PTN_SP_RULE pdata_rule = NULL;

	if(pPath == NULL || pFile == NULL || pcFeKey == NULL)
		return 0;
	PDB_PO_IN_PTN_OP pOpPolicy = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
	if(pOpPolicy == NULL)
		return 0;

	UINT32 nOpUsedMode = pOpPolicy->tDPH.nUsedMode;
	if(nOpUsedMode == STATUS_USED_MODE_OFF)
	{
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		nIsWarning = 1;
		return 1;
	}
	if(nOpUsedMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	PDB_PO_IN_PTN_SP pdata_sp = (PDB_PO_IN_PTN_SP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_SP);
	if(pOpPolicy == NULL)
		return 0;
	UINT32 nPoUsedMode = pdata_sp->tDPH.nUsedMode;
	UINT32 nExtOption = pdata_sp->tDPH.nExtOption;

	if(nPoUsedMode == STATUS_USED_MODE_OFF)
		return 0;
	if(nPoUsedMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	TListDBPoInPtnSPRule tRuleList;
	TListDBPoInPtnSPRuleItor begin, end;

	t_ManagePoInPtnSPRule->GetItemCopy(tRuleList);

	nRtn = 0;
	begin = tRuleList.begin();	end = tRuleList.end();
	for(begin; begin != end; begin++)
	{
		pdata_rule = &(*begin);
		if(!pdata_rule)
			continue;

		if(pdata_rule->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
			continue;	
		if(pdata_rule->nConfirmRst == SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_WAIT)
			continue;
		if(pdata_rule->nConfirmRst == SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_END)
			continue;
		if(pdata_rule->nConfirmRst == SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_END_FOREVER)
			continue;
		if(pdata_rule->nEndDate)
			continue;
		if(_stricmp(pdata_rule->strFileKey.c_str(), pcFeKey))
			continue;

		if(nExtOption & SS_PO_IN_PTN_SP_EXT_OPTION_PATH_BASED_INSPECTION)
		{
			if(_stricmp(pdata_rule->strFilePath.c_str(), pPath))
				continue;
			if(_stricmp(pdata_rule->strFileName.c_str(), pFile))
				continue;
		}

		if(pdata_rule->tDPH.nUsedMode == STATUS_USED_MODE_WARN)
			nIsWarning = 1;

		nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_SP + pdata_rule->tDPH.nID;
		nBlockMode = pdata_rule->nConfirmRst;

		nRtn = 1;
		break;
	}
	return nRtn;
}

INT32		CThreadPoInPtnFile::ChkInPtn(char *pcFeKey, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType)
{
	if(pcFeKey == NULL)
		return -1;
	UINT32 nOpUsedMode = STATUS_USED_MODE_OFF;
	UINT32 nOpID = 0;
	UINT32 nWLID = 0;
	UINT32 nBLID = 0;
	UINT32 nOpBlockMode = 0;
	UINT32 nBLCount = 0;
	UINT32 nWLCount = 0;
	UINT32 nBLUsedMode = STATUS_USED_MODE_OFF;
	UINT32 nWLUsedMode = STATUS_USED_MODE_OFF;
	UINT32 nPtnRisk = PTN_FB_PTN_RISK_UNKNOW;
	INT32 i;
	if(t_MMPPGWO == NULL && t_MMPPGBO == NULL)
		return 0;

	PDB_PO_IN_PTN_OP pOpPolicy = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
	if(pOpPolicy != NULL)
	{
		nOpUsedMode = pOpPolicy->tDPH.nUsedMode;
		nOpID = pOpPolicy->tDPH.nID;
		nOpBlockMode = pOpPolicy->nBlockMode;
	}
	if(nOpUsedMode == STATUS_USED_MODE_OFF)
		return 0;
	PDB_PO_IN_PTN_WL pWlPolicy = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_WL);
	if(pWlPolicy)
	{
		nWLID = pWlPolicy->tDPH.nID;
//		if(t_ManagePoInPtnWL->IsValidPtnFile(nWLID) && t_LogicMgrPtnGWO->IsInitLogic())
		if(t_LogicMgrPtnGWO->IsInitLogic())
		{
			nWLUsedMode = pWlPolicy->tDPH.nUsedMode;
		}
		else
		{
			nWLUsedMode = STATUS_USED_MODE_OFF;
		}
	}

	PDB_PO_IN_PTN_BL pBlPolicy = (PDB_PO_IN_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_BL);
	if(pBlPolicy)
	{
		nBLID = pBlPolicy->tDPH.nID;
//		if(t_ManagePoInPtnBL->IsValidPtnFile(nBLID) && t_LogicMgrPtnGBO->IsInitLogic())
		if(t_LogicMgrPtnGBO->IsInitLogic())
		{
			nBLUsedMode = pBlPolicy->tDPH.nUsedMode;
		}
		else
		{
			nBLUsedMode = STATUS_USED_MODE_OFF;
		}
	}


	if(nBLUsedMode == STATUS_USED_MODE_OFF && nWLUsedMode == STATUS_USED_MODE_OFF)
		return 0;

	if(nOpUsedMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	if(nOpBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW)
	{
		if(nBLUsedMode == STATUS_USED_MODE_OFF)
			return 0;
		nPtnRisk = PTN_FB_PTN_RISK_UNKNOW;


		nBLCount = t_MMPPGBO->t_ManagePtnProcFile->Count();
		if(nBLCount != 0 && t_MMPPGBO->t_ManagePtnProcFile->FindSKeyID(pcFeKey))
		{
			nPtnRisk = PTN_FB_PTN_RISK_BLACK;
		}

		if(t_MMPPGBO && nPtnRisk == PTN_FB_PTN_RISK_UNKNOW)
		{
			if(t_MMPPGWO && nWLUsedMode != STATUS_USED_MODE_OFF)
			{
				nWLCount = t_MMPPGWO->t_ManagePtnProcFile->Count();
				if(nWLCount != 0 && t_MMPPGWO->t_ManagePtnProcFile->FindSKeyID(pcFeKey))
				{
					nPtnRisk = PTN_FB_PTN_RISK_WHITE;
				}
			}
		}

		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		if(nPtnRisk == PTN_FB_PTN_RISK_BLACK)
		{
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_BL + nBLID;
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
		}
		else if(nPtnRisk == PTN_FB_PTN_RISK_WHITE)
		{
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_WL + nWLID;
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		}
		else
		{
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_OP + nOpID;
		}
	}
	else
	{
		if(nWLUsedMode == STATUS_USED_MODE_OFF)
			return 0;

		nPtnRisk = PTN_FB_PTN_RISK_UNKNOW;

		if(t_MMPPGBO && nBLUsedMode != STATUS_USED_MODE_OFF)
		{
			nBLCount = t_MMPPGBO->t_ManagePtnProcFile->Count();
			if(nBLCount != 0 && t_MMPPGBO->t_ManagePtnProcFile->FindSKeyID(pcFeKey))
			{
				nPtnRisk = PTN_FB_PTN_RISK_BLACK;
			}
		}

		if(t_MMPPGWO && nPtnRisk == PTN_FB_PTN_RISK_UNKNOW)
		{
			nWLCount = t_MMPPGWO->t_ManagePtnProcFile->Count();
			if(nWLCount != 0 && t_MMPPGWO->t_ManagePtnProcFile->FindSKeyID(pcFeKey))
			{
				nPtnRisk = PTN_FB_PTN_RISK_WHITE;
			}
		}

		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;

		if(nPtnRisk == PTN_FB_PTN_RISK_BLACK)
		{
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_BL + nBLID;
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
		}
		else if(nPtnRisk == PTN_FB_PTN_RISK_WHITE)
		{
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_WL + nWLID;
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;
		}
		else
		{
			nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
			nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_OP + nOpID;
		}
	}
	return 1;
}


INT32	CThreadPoInPtnFile::SetLogExecEvent(PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nRetVal = 0;
	DB_LOG_SECU *pSecuLog = NULL;
	if(pChkPtnFile == NULL)
	{
		return -1;
	}

	do{
		pSecuLog = (DB_LOG_SECU *)malloc(sizeof(DB_LOG_SECU));
		if(pSecuLog == NULL)
		{
			nRetVal = -2;
			break;
		}
		memset(pSecuLog, 0, sizeof(DB_LOG_SECU));

		pSecuLog->strSubjectPath = pChkPtnFile->stCHKFILE.stProcInfo.acPath;
		pSecuLog->strSubjectName = pChkPtnFile->stCHKFILE.stProcInfo.acFile;
		pSecuLog->strObjectPath	= pChkPtnFile->stCHKFILE.stFileInfo.acPath;
		pSecuLog->strObjectName	= pChkPtnFile->stCHKFILE.stFileInfo.acFile;
		pSecuLog->strExtInfo = pChkPtnFile->stAWWE.acWhiteHash;
		
		pSecuLog->nOpType = LOG_PROCESS_ACCESS_DENIED;
		pSecuLog->nObjectType = DETECT_MODE_ACL;
		if(pChkPtnFile->stCHKFILE.stRetInfo.nIsWarning == 1)
		{
			pSecuLog->nBlockType = ASI_EPS_LOG_TYPE_WARN;
		}
		else
		{
			pSecuLog->nBlockType = ASI_EPS_LOG_TYPE_DENY;
		}
		pSecuLog->nPolicyType = pChkPtnFile->stCHKFILE.stRetInfo.nPolicyType;
		pSecuLog->nEvtTime = GetCurrentDateTimeInt();
		pSecuLog->nRegDate = t_ValidTimeUtil->GetValidTime();
		HISYNCSTEPUP(pSecuLog->nSyncSvrStep);
		t_LogicMgrLogSecu->SetLogSecu(*pSecuLog);
		nRetVal = 0;
	}while(FALSE);
	safe_free(pSecuLog);
	return nRetVal;
}

VOID	CThreadPoInPtnFile::SetLogCreateEvent(PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 i, nRetVal = 0;
	DWORD dwFileType = AS_INVALID_FILE;
#ifdef _PERP_TEST_LOG
	BOOL bIsTestPgm = FALSE;
	double fDiffTime = 0;
	struct timeval stStartTime;
#endif
	if(pChkPtnFile == NULL)
	{
		return;
	}
	if(t_LogicMgrSiteFile != NULL)
	{
#ifdef _PERP_TEST_LOG
		if(!_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFile, "tc5_file_", 9))
		{
			bIsTestPgm = TRUE;
		}
#endif
		for(i=0; i<3; i++)
		{
#ifdef _PERP_TEST_LOG
			if(bIsTestPgm == TRUE)
			{
				gettimeofday(&stStartTime, NULL);
			}
#endif
			nRetVal = m_tWEDLLUtil.GetWL(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, (PVOID)&pChkPtnFile->stAWWE, sizeof(ASI_WENG_WL_EX), &dwFileType);
			if(nRetVal == 0 || dwFileType != AS_INVALID_FILE)
			{
				break;
			}
			Sleep(100);
		}
		if(pChkPtnFile->stAWWE.acWhiteHash[0] == 0)
		{
			return;
		}

		strncpy(pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash, pChkPtnFile->stAWWE.acWhiteHash, SHA512_BLOCK_SIZE+1);
		pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		t_LogicMgrSiteFile->CheckSiteCreateFile(&pChkPtnFile->stCHKFILE);

#ifdef _PERP_TEST_LOG
		if(bIsTestPgm == TRUE)
		{
			fDiffTime = diff_time(stStartTime);
			UINT32 nTime = (UINT32)time(NULL);
			if(nTime - m_nTestTime > 30)
			{
				m_nTestCount = 0;
				m_fTotalDiffTime = 0;
			}
			else
				m_nTestCount++;
			m_nTestTime = nTime;
			m_fTotalDiffTime += fDiffTime;
			WritePerfTest5Log("[%03d]\ttest the interval time for detection of created file [%s] [detection time : %.02f ms]", m_nTestCount, pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, fDiffTime/1000);
			if(m_nTestCount == 19)
			{
				fDiffTime = m_fTotalDiffTime/20;
				WritePerfTest5Log("[total]\ttest the interval time for detection of created file [total : %d files] [average time : %.02f ms]", m_nTestCount+1, fDiffTime/1000);
			}
		}
#endif


	}
}


INT32	CThreadPoInPtnFile::AnalyzeCreateEvent2(PASI_CHK_PTN_FILE pChkPtnFile, INT32 nCount)
{
	struct fanotify_response stAccess;
	INT32 nRetVal = 0;
	INT32 nUsedMode = STATUS_USED_MODE_OFF;
	INT32 nBlockMode = SS_PO_CTL_PROC_BLOCK_MODE_ALLOW;
	INT32 nIsWarning = 0;
	INT32 nPolicyType = 0;
	UINT32 nExtOption = 0;
	DWORD dwFileType = AS_INVALID_FILE;
	String strPath;
	TMapStrIDItor find;


	do{
		strncpy(pChkPtnFile->stCHKFILE.stProcInfo.acFullPath, "/usr/local/test", MAX_PATH-1);
		pChkPtnFile->stCHKFILE.stProcInfo.acFullPath[MAX_PATH-1] = 0;
		pChkPtnFile->stCHKFILE.stProcInfo.nLen = (INT32)strlen(pChkPtnFile->stCHKFILE.stProcInfo.acFullPath);

		if(ParseFilePath(&pChkPtnFile->stCHKFILE.stProcInfo) == FALSE)
		{
			break;
		}
		if(nCount == 0)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/home/castle/share/test/Client", MAX_PATH-1);
		else if(nCount == 1)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/home/castle/share/test/Server", MAX_PATH-1);
		else if(nCount == 2)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/home/castle/share/test/EchoClient", MAX_PATH-1);
		else if(nCount == 3)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/home/castle/share/test/EchoServer", MAX_PATH-1);
		else if(nCount == 4)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/home/castle/share/test/test2", MAX_PATH-1);
		else if(nCount == 5)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/home/castle/share/test/test3", MAX_PATH-1);
		else
		{
			break;
		}

		pChkPtnFile->stCHKFILE.stFileInfo.acFullPath[MAX_PATH-1] = 0;
		pChkPtnFile->stCHKFILE.stFileInfo.nLen = (INT32)strlen(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath);
		if(ParseFilePath(&pChkPtnFile->stCHKFILE.stFileInfo) == FALSE)
		{
			break;
		}

		AnalyzeCreateEvent(pChkPtnFile);
	}while(FALSE);
	Sleep(2000);
	return 0;
}


INT32	CThreadPoInPtnFile::AnalyzeExecEvent2(PASI_CHK_PTN_FILE pChkPtnFile, INT32 nCount)
{
	struct fanotify_response stAccess;
	INT32 nRetVal = 0;
	INT32 nUsedMode = STATUS_USED_MODE_OFF;
	INT32 nBlockMode = SS_PO_CTL_PROC_BLOCK_MODE_ALLOW;
	INT32 nIsWarning = 0;
	INT32 nPolicyType = 0;
	UINT32 nExtOption = 0;
	DWORD dwFileType = AS_INVALID_FILE;
	INT32 i, nAcVal = RET_NONE;
	String strPath;
	TMapStrIDItor find;
	do{
		strncpy(pChkPtnFile->stCHKFILE.stProcInfo.acFullPath, "/usr/local/test", MAX_PATH-1);
		pChkPtnFile->stCHKFILE.stProcInfo.acFullPath[MAX_PATH-1] = 0;
		pChkPtnFile->stCHKFILE.stProcInfo.nLen = (INT32)strlen(pChkPtnFile->stCHKFILE.stProcInfo.acFullPath);

		if(ParseFilePath(&pChkPtnFile->stCHKFILE.stProcInfo) == FALSE)
		{
			nAcVal = RET_NONE;
			break;
		}
		if(nCount == 0)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/bin/ping", MAX_PATH-1);
		else if(nCount == 1)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/bin/kmod", MAX_PATH-1);
		else if(nCount == 2)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/bin/true", MAX_PATH-1);
		else if(nCount == 3)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/usr/bin/tail", MAX_PATH-1);
		else if(nCount == 4)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/usr/bin/vim.tiny", MAX_PATH-1);
		else if(nCount == 5)
			strncpy(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/usr/share/autojump/autojump", MAX_PATH-1);
		else
		{
			nAcVal = RET_NONE;
			break;
		}

		pChkPtnFile->stCHKFILE.stFileInfo.acFullPath[MAX_PATH-1] = 0;
		pChkPtnFile->stCHKFILE.stFileInfo.nLen = (INT32)strlen(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath);
		if(ParseFilePath(&pChkPtnFile->stCHKFILE.stFileInfo) == FALSE)
		{
			nAcVal = RET_NONE;
			break;
		}

		if(!_strnicmp(pChkPtnFile->stCHKFILE.stProcInfo.acFullPath, NANNY_INSTALL_DIR, NANNY_INSTALL_DIR_LEN) || 
			!_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, NANNY_INSTALL_DIR, NANNY_INSTALL_DIR_LEN))
		{
			nAcVal = RET_NONE;
			break;
		}
		dwFileType = 0;
		nRetVal = m_tWEDLLUtil.GetWL(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, (PVOID)&pChkPtnFile->stAWWE, sizeof(ASI_WENG_WL_EX), &dwFileType);
		if(nRetVal < 0 || dwFileType == AS_INVALID_FILE)
		{
			dwFileType = AS_INVALID_FILE;
			nAcVal = RET_NONE;
			break;
		}
		nRetVal = ChkInPtnEx(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, pChkPtnFile->stAWWE.acWhiteHash, nBlockMode, nIsWarning, nPolicyType, nExtOption);
		if(nRetVal == 1)
		{
			nAcVal = RET_ALLOW;
			if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
			{
				if(nIsWarning == 1)
					nAcVal = RET_WARN;
				else
					nAcVal = RET_DENY;
			}
			break;
		}

		nRetVal = ChkInPtnSP(pChkPtnFile->stCHKFILE.stFileInfo.acPath, pChkPtnFile->stCHKFILE.stFileInfo.acFile, pChkPtnFile->stAWWE.acWhiteHash, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal == 1)
		{
			nAcVal = RET_ALLOW;
			if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
			{
				if(nIsWarning == 1)
					nAcVal = RET_WARN;
				else
					nAcVal = RET_DENY;
			}
			break;
		}

		nRetVal = ChkInPtn(pChkPtnFile->stAWWE.acWhiteHash, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal != 1)
		{
			nAcVal = RET_NONE;
			break;
		}
		nAcVal = RET_ALLOW;
		if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
		{
			if(nIsWarning == 1)
				nAcVal = RET_WARN;
			else
				nAcVal = RET_DENY;
		}
	}while(FALSE);

	if(nAcVal == RET_DENY || nAcVal == RET_WARN)
	{
		//log
		SetLogExecEvent(pChkPtnFile);
	}
	Sleep(2000);
	return 0;
}


INT32	CThreadPoInPtnFile::GetRetMapData(PASI_CHK_PTN_FILE pChkPtnFile)
{
	if(pChkPtnFile == NULL || pChkPtnFile->stCHKFILE.stProcInfo.acFullPath[0] == 0)
	{
		return -1;
	}
	if(GetPtnRet(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, &pChkPtnFile->stCHKFILE.stRetInfo) == FALSE)
	{
		return -2;
	}

	if(pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash[0] != 0)
	{
		strncpy(pChkPtnFile->stAWWE.acWhiteHash, pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash, SHA512_BLOCK_SIZE+1);
		pChkPtnFile->stAWWE.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
	}
	return 0;
}


INT32	CThreadPoInPtnFile::SetRetMapData(PASI_CHK_PTN_FILE pChkPtnFile)
{
	if(pChkPtnFile == NULL || pChkPtnFile->stCHKFILE.stProcInfo.acFullPath[0] == 0)
	{
		WriteLogE("[SetRetMapData] invalid input event data");
		return -1;
	}
	if(pChkPtnFile->stAWWE.acWhiteHash[0] != 0)
	{
		strncpy(pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash, pChkPtnFile->stAWWE.acWhiteHash, SHA512_BLOCK_SIZE+1);
		pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
	}
	else
		pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash[0] = 0;
	if(AddPtnRet(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, pChkPtnFile->stCHKFILE.stRetInfo) == FALSE)
	{
		WriteLogE("[SetRetMapData] fail to set ret map data");
		return -2;
	}
	return 0;
}

INT32	CThreadPoInPtnFile::BypassObjectPath(PASI_CHK_PTN_FILE pChkPtnFile)
{
	if(!_strnicmp(pChkPtnFile->stCHKFILE.stProcInfo.acFullPath, NANNY_INSTALL_DIR, NANNY_INSTALL_DIR_LEN))
	{
		return 0;
	}
	else if(!_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, NANNY_INSTALL_DIR, NANNY_INSTALL_DIR_LEN))
	{
		return 0;
	}
	else if(!_stricmp(pChkPtnFile->stCHKFILE.stProcInfo.acFullPath, REQ_WHITE_PATH))
	{
		return 0;
	}
	else if(!_stricmp(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, REQ_WHITE_PATH))
	{
		return 0;
	}
	else if(!_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/sys", 4))
	{
		return 0;
	}
	else if(!_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/lib", 4))
	{
		return 0;
	}
	else if(!_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/proc", 5))
	{
		return 0;
	}
	else if(!_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/usr/src", 8))
	{
		return 0;
	}
	else if(!_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/var/lib", 8))
	{
		return 0;
	}
	else if(!_strnicmp(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, "/usr/include", 12))
	{
		return 0;
	}
	return -1;
}


VOID	CThreadPoInPtnFile::SetRetValValue(ASI_RET_INFO *pstRetInfo, INT32 nAcVal, INT32 nBlockMode, INT32 nIsWarning, INT32 nPolicyType)
{
	if(pstRetInfo != NULL)
	{
		pstRetInfo->nAcVal = nAcVal;
		pstRetInfo->nBlockMode = nBlockMode;
		pstRetInfo->nPolicyType = nPolicyType;
		pstRetInfo->nIsWarning = nIsWarning;
	}
}

INT32	CThreadPoInPtnFile::AnalyzeExecEvent(PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nRetVal = 0;
	DWORD dwFileType = AS_INVALID_FILE;
	INT32 nBlockMode = SS_PO_CTL_PROC_BLOCK_MODE_ALLOW;
	INT32 nAcVal = RET_NONE;
	INT32 nIsWarning = 0;
	INT32 nPolicyType = 0;
	UINT32 nExtOption = 0;

	if(pChkPtnFile == NULL)
	{
		return RET_NONE;
	}

	do{
		if(BypassObjectPath(pChkPtnFile) == 0)
		{
			nAcVal = RET_NONE;
			SetRetValValue(&pChkPtnFile->stCHKFILE.stRetInfo, nAcVal, 0, 0, 0);
			break;
		}
/*
		if(GetRetMapData(pChkPtnFile) == 0)
		{
			nAcVal = pChkPtnFile->stCHKFILE.stRetInfo.nAcVal;
			break;
		}
*/
		dwFileType = 0;
		nRetVal = m_tWEDLLUtil.GetWL(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, (PVOID)&pChkPtnFile->stAWWE, sizeof(ASI_WENG_WL_EX), &dwFileType);
		if(nRetVal < 0 || dwFileType == AS_INVALID_FILE)
		{
			nAcVal = RET_NONE;
			SetRetValValue(&pChkPtnFile->stCHKFILE.stRetInfo, nAcVal, 0, 0, 0);
//			SetRetMapData(pChkPtnFile);
			break;
		}

		nRetVal = ChkInPtnEx(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, pChkPtnFile->stAWWE.acWhiteHash, nBlockMode, nIsWarning, nPolicyType, nExtOption);
		if(nRetVal == 1)
		{
			nAcVal = RET_ALLOW;
			if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
			{
				if(nIsWarning == 1)
					nAcVal = RET_WARN;
				else
					nAcVal = RET_DENY;
			}
			SetRetValValue(&pChkPtnFile->stCHKFILE.stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
//			SetRetMapData(pChkPtnFile);
			break;
		}

		nRetVal = ChkInPtnSP(pChkPtnFile->stCHKFILE.stFileInfo.acPath, pChkPtnFile->stCHKFILE.stFileInfo.acFile, pChkPtnFile->stAWWE.acWhiteHash, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal == 1)
		{
			nAcVal = RET_ALLOW;
			if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
			{
				if(nIsWarning == 1)
					nAcVal = RET_WARN;
				else
					nAcVal = RET_DENY;
			}
			SetRetValValue(&pChkPtnFile->stCHKFILE.stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
//			SetRetMapData(pChkPtnFile);
			break;
		}
		nRetVal = ChkInPtn(pChkPtnFile->stAWWE.acWhiteHash, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal != 1)
		{
			nAcVal = RET_NONE;
			SetRetValValue(&pChkPtnFile->stCHKFILE.stRetInfo, RET_NONE, 0, 0, 0);
//			SetRetMapData(pChkPtnFile);
			break;
		}
		nAcVal = RET_NONE;
		if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
		{
			if(nIsWarning == 1)
				nAcVal = RET_WARN;
			else
			{
				nAcVal = RET_DENY;
			}
		}
		SetRetValValue(&pChkPtnFile->stCHKFILE.stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
//		SetRetMapData(pChkPtnFile);
	}while(FALSE);

	return nAcVal;
}

DWORD	CThreadPoInPtnFile::AnalyzeCreateEvent(PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nRetVal = 0;
	DWORD dwFileType = AS_INVALID_FILE;

	if(pChkPtnFile == NULL)
	{
		return AS_INVALID_FILE;
	}
	pChkPtnFile->stCHKFILE.stRetInfo.nAcVal = RET_NONE;
	nRetVal = m_tWEDLLUtil.GetWL(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, (PVOID)&pChkPtnFile->stAWWE, sizeof(ASI_WENG_WL_EX), &dwFileType);
	if(nRetVal != 0)
	{
		return AS_INVALID_FILE;
	}
	return dwFileType;
}

INT32		CThreadPoInPtnFile::CheckSockEvent(INT32 nClientFd, PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nRetVal = 0;
	INT32 nAcVal = RET_NONE;
	DWORD dwFileType = AS_INVALID_FILE;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
#ifdef _PERP_TEST_LOG
	BOOL bIsTestPgm = FALSE;
	double fDiffTime = 0;
	struct timeval stStartTime;
#endif
	if(pChkPtnFile == NULL || nClientFd == -1)
	{
		return -1;
	}
	do{
		nRetVal = SockRecv(nClientFd, (PVOID)&pChkPtnFile->stCHKFILE, nSize);
		if(nRetVal != 0)
		{
			if(nRetVal < 0)
				nRetVal -= 10;
			break;
		}

		if(pChkPtnFile->stCHKFILE.nCmdId == CMD_PIPE_EXIT_THREAD)
		{
			nRetVal = 1111;
			break;
		}
		else if(pChkPtnFile->stCHKFILE.nCmdId == CMD_PIPE_ALIVE_MESSAGE)
		{
			nRetVal = 0;
			break;
		}
		else if(pChkPtnFile->stCHKFILE.nCmdId == CMD_PIPE_PO_IN_CHK_WHITE)
		{
#ifdef _PERP_TEST_LOG
			if(!_stricmp(pChkPtnFile->stCHKFILE.stProcInfo.acFile, "exe_test_pgm"))
			{
				bIsTestPgm = TRUE;
				gettimeofday(&stStartTime, NULL);
			}
#endif
			nAcVal = AnalyzeExecEvent(pChkPtnFile);
#ifdef _PERP_TEST_LOG
			if(bIsTestPgm == TRUE)
			{
				fDiffTime = diff_time(stStartTime);
			}
#endif
			nRetVal = SockWrite(nClientFd, (PVOID)&pChkPtnFile->stCHKFILE, nSize);
			if(nRetVal != 0)
			{
				if(nRetVal < 0)
					nRetVal -= 20;
				break;
			}
			if(nAcVal == RET_DENY || nAcVal == RET_WARN)
			{
				SetLogExecEvent(pChkPtnFile);
#ifdef _PERP_TEST_LOG
				if(bIsTestPgm == TRUE)
				{
					UINT32 nTime = (UINT32)time(NULL);
					if(nTime - m_nTestTime > 30)
					{
						m_nTestCount = 0;
						m_fTotalDiffTime = 0;
					}
					else
						m_nTestCount++;
					m_nTestTime = nTime;
					WritePerfTest1Log("[%03d]\ttest for detection of unauthorized file [%s]", m_nTestCount, pChkPtnFile->stCHKFILE.stFileInfo.acFullPath);
					m_fTotalDiffTime += fDiffTime;
					WritePerfTest3Log("[%03d]\ttest the interval time for detection of unauthorized file [%s] [detection time : %.02f ms]", m_nTestCount, pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, fDiffTime/1000);
					if(m_nTestCount == 19)
					{
						fDiffTime = m_fTotalDiffTime/20;
						WritePerfTest3Log("[total]\ttest the interval time for detection of unauthorized file [total : %d files] [average time : %.02f ms]", m_nTestCount+1, fDiffTime/1000);
					}
				}
#endif /*_PERP_TEST_LOG*/
			}
			else
			{
#ifdef _PERP_TEST_LOG
				if(bIsTestPgm == TRUE)
				{
					UINT32 nTime = (UINT32)time(NULL);
					if(nTime - m_nTestTime > 30)
						m_nTestCount = 0;
					else
						m_nTestCount++;
					m_nTestTime = nTime;
					WritePerfTest2Log("[%03d]\ttest for detection of authorized file [%s]", m_nTestCount, pChkPtnFile->stCHKFILE.stFileInfo.acFullPath);
				}
#endif /*_PERP_TEST_LOG*/
			}
		}
		else if(pChkPtnFile->stCHKFILE.nCmdId == CMD_PIPE_PO_IN_CREATE_FILE)
		{
			dwFileType = AnalyzeCreateEvent(pChkPtnFile);
			nRetVal = SockWrite(nClientFd, (PVOID)&pChkPtnFile->stCHKFILE, nSize);
			if(nRetVal != 0)
			{
				if(nRetVal < 0)
					nRetVal -= 30;
				break;
			}
			if(dwFileType != AS_INVALID_FILE)
			{
				SetLogCreateEvent(pChkPtnFile);
			}
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

