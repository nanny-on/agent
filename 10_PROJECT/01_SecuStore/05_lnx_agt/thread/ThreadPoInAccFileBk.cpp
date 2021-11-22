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

// ThreadPoInAccFile.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadPoInAccFile.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInAccFile

CThreadPoInAccFile*	t_ThreadPoInAccFile = NULL;

CThreadPoInAccFile::CThreadPoInAccFile()
{
	m_nOrderID = 0;
	m_nServerFd = -1;
	m_nClientFd = -1;
	m_nCheckThread = ASI_DISCONNECT_STATE;
	pthread_mutex_init(&m_SockMutex, NULL);

}

CThreadPoInAccFile::~CThreadPoInAccFile()
{
	pthread_mutex_destroy(&m_SockMutex);
}

BOOL CThreadPoInAccFile::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoInAccFile::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInAccFile message handlers

INT32 CThreadPoInAccFile::InitUnixSock(INT32 &nSrvFd)
{
	INT32 nRetVal = 0;
	INT32 nOptVal = 1;
	struct sockaddr_un serveraddr;
	char acSockPath[MAX_FILE_NAME] = {0,};
	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_ACC_FILE);
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

VOID CThreadPoInAccFile::UninitUnixSock()
{
	char acSockPath[MAX_FILE_NAME] = {0,};
	pthread_mutex_lock (&m_SockMutex);
	if(m_nServerFd != -1)
	{
		close(m_nServerFd);
		m_nServerFd = -1;
	}
	pthread_mutex_unlock (&m_SockMutex);

	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_ACC_FILE);
	acSockPath[MAX_FILE_NAME-1] = 0;
	if(is_file(acSockPath) == SOCK_FILE)
	{
		unlink(acSockPath);
	}
}

INT32	CThreadPoInAccFile::IsInitLogic()
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

INT32	CThreadPoInAccFile::InitNotifyEvent(INT32 &nSrvFd)
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

INT32	CThreadPoInAccFile::GetCheckThreadState()
{
	INT32 nFlag = 0;
	pthread_mutex_lock (&m_SockMutex);
	nFlag = m_nCheckThread;
	pthread_mutex_unlock (&m_SockMutex);
	return nFlag;
}


VOID	CThreadPoInAccFile::SetCheckThreadState(INT32 nState)
{
	pthread_mutex_lock (&m_SockMutex);
	m_nCheckThread = nState;
	pthread_mutex_unlock (&m_SockMutex);
}

INT32	CThreadPoInAccFile::GetClientFd()
{
	INT32 nFd = 0;
	pthread_mutex_lock (&m_SockMutex);
	nFd = m_nClientFd;
	pthread_mutex_unlock (&m_SockMutex);
	return nFd;
}


VOID	CThreadPoInAccFile::SetClientFd(INT32 nFd)
{
	pthread_mutex_lock (&m_SockMutex);
	m_nClientFd = nFd;
	pthread_mutex_unlock (&m_SockMutex);
}


INT32 CThreadPoInAccFile::SockRecv(INT32 nFd, PVOID pRecvData, INT32 nReqSize)
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

INT32 CThreadPoInAccFile::SockWrite(INT32 nFd, PVOID pWriteData, INT32 nReqSize)
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

INT32		CThreadPoInAccFile::SendExitThreadCmd()
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

		snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_ACC_FILE);
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

INT32 CThreadPoInAccFile::CheckThread(INT32 nClientFd)
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
			if(nRetVal != 13)
				WriteLogE("fail to check sock event : [%s][%d][%d]", m_strThreadName.c_str(), nRetVal, errno);
			close(nClientFd);
			SetClientFd(-1);
			safe_free(pChkPtnFile);
			SetCheckThreadState(ASI_DISCONNECT_STATE);
			return nRetVal;
		}
	}	
	SetCheckThreadState(ASI_DISCONNECT_STATE);
	close(nClientFd);
	SetClientFd(-1);
	safe_free(pChkPtnFile);
	return 0;
}

INT32 CThreadPoInAccFile::Run()
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
	m_tOrderIDMap.clear();
	return 0;
}


INT32 CThreadPoInAccFile::CheckWhitePatternFile()
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

VOID	CThreadPoInAccFile::SetRetValValue(ASI_RET_INFO *pstRetInfo, INT32 nAcVal, INT32 nBlockMode, INT32 nIsWarning, INT32 nPolicyType)
{
	if(pstRetInfo != NULL)
	{
		pstRetInfo->nAcVal = nAcVal;
		pstRetInfo->nBlockMode = nBlockMode;
		pstRetInfo->nPolicyType = nPolicyType;
		pstRetInfo->nIsWarning = nIsWarning;
	}
}

INT32		CThreadPoInAccFile::ChkInPtn(PASI_CHK_PTN_FILE pChkPtnFile, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType)
{
	UINT32 nWLID = 0;
	UINT32 nBLID = 0;
	UINT32 nID = 0;
	UINT32 nBLCount = 0;
	UINT32 nWLCount = 0;
	UINT32 nOpUsedMode = STATUS_USED_MODE_OFF;
	UINT32 nBLUsedMode = STATUS_USED_MODE_OFF;
	UINT32 nWLUsedMode = STATUS_USED_MODE_OFF;
	UINT32 nPtnRisk = PTN_FB_PTN_RISK_UNKNOW;
	CString strFullPath;
	CString strHash;

	if(pChkPtnFile == NULL)
		return 0;
	if(t_MMPPGWO == NULL && t_MMPPGBO == NULL)
		return 0;

	PDB_PO_IN_PTN_OP pOpPolicy = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
	if(pOpPolicy != NULL)
	{
		nOpUsedMode = pOpPolicy->tDPH.nUsedMode;
	}
	if(nOpUsedMode == STATUS_USED_MODE_OFF)
		return 0;

	PDB_PO_IN_PTN_BL pBlPolicy = (PDB_PO_IN_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_BL);
	if(pBlPolicy)
	{
		nBLID = pBlPolicy->tDPH.nID;
		if(t_ManagePoInPtnBL->IsValidPtnFile(nBLID) && t_LogicMgrPtnGBO->IsInitLogic())
		{
			nBLUsedMode = pBlPolicy->tDPH.nUsedMode;
		}
		else
		{
			nBLUsedMode = STATUS_USED_MODE_OFF;
		}
	}

	PDB_PO_IN_PTN_WL pWlPolicy = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_WL);
	if(pWlPolicy)
	{
		nWLID = pWlPolicy->tDPH.nID;
		if(t_ManagePoInPtnWL->IsValidPtnFile(nWLID) && t_LogicMgrPtnGWO->IsInitLogic())
		{
			nWLUsedMode = pWlPolicy->tDPH.nUsedMode;
		}
		else
		{
			nWLUsedMode = STATUS_USED_MODE_OFF;
		}
	}

	if(nBLUsedMode == STATUS_USED_MODE_OFF && nWLUsedMode == STATUS_USED_MODE_OFF)
		return 0;

	if(nOpUsedMode == STATUS_USED_MODE_WARN)
		nIsWarning = 1;

	strFullPath.Format("%s", pChkPtnFile->stCHKFILE.stFileInfo.acFullPath);
	nPtnRisk = PTN_FB_PTN_RISK_UNKNOW;
	if(t_MMPPGWO && nWLUsedMode != STATUS_USED_MODE_OFF)
	{
		nWLCount = t_MMPPGWO->t_ManagePtnProcFile->Count();
		if(nWLCount != 0 && t_MMPPGWO->t_ManagePtnProcFile->FindWPathStr(strFullPath, strHash))
		{
			nPtnRisk = PTN_FB_PTN_RISK_WHITE;
			strncpy(pChkPtnFile->stAWWE.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
			pChkPtnFile->stAWWE.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		}
	}

	if(t_MMPPGBO && nPtnRisk == PTN_FB_PTN_RISK_UNKNOW && nBLUsedMode != STATUS_USED_MODE_OFF)
	{
		nBLCount = t_MMPPGBO->t_ManagePtnProcFile->Count();
		if(nBLCount != 0 && t_MMPPGBO->t_ManagePtnProcFile->FindWPathStr(strFullPath, strHash))
		{
			nPtnRisk = PTN_FB_PTN_RISK_BLACK;
			strncpy(pChkPtnFile->stAWWE.acWhiteHash, (char*)(LPCSTR)strHash, SHA512_BLOCK_SIZE+1);
			pChkPtnFile->stAWWE.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		}
	}

	nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_ALLOW;

	if(nPtnRisk == PTN_FB_PTN_RISK_WHITE)
	{
		nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_WL + nWLID;
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
	}
	else if(nPtnRisk == PTN_FB_PTN_RISK_BLACK)
	{
		nPolicyType = ASI_EPS_APP_POLICY_GROUP_ID_IN_PTN_BL + nBLID;
		nBlockMode = SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY;
	}
	return 1;
}


INT32	CThreadPoInAccFile::SetLogAccEvent(PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nRetVal = 0;
	DB_LOG_SECU stSecuLog;
	if(pChkPtnFile == NULL)
	{
		return -1;
	}

	do{
		stSecuLog.strSubjectPath = SPrintf("%s", pChkPtnFile->stCHKFILE.stProcInfo.acPath);
		stSecuLog.strSubjectName = SPrintf("%s", pChkPtnFile->stCHKFILE.stProcInfo.acFile);
		stSecuLog.strObjectPath	= SPrintf("%s", pChkPtnFile->stCHKFILE.stFileInfo.acPath);
		stSecuLog.strObjectName	= SPrintf("%s", pChkPtnFile->stCHKFILE.stFileInfo.acFile);
		stSecuLog.strExtInfo = SPrintf("%s", pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash);

		stSecuLog.nOpType = LOG_FILE_WRITE_DENIED;
		stSecuLog.nObjectType = DETECT_MODE_ACL;
		if(pChkPtnFile->stCHKFILE.stRetInfo.nIsWarning == 1)
		{
			stSecuLog.nBlockType = ASI_EPS_LOG_TYPE_WARN;
		}
		else
		{
			stSecuLog.nBlockType = ASI_EPS_LOG_TYPE_DENY;
		}
		stSecuLog.nPolicyType = pChkPtnFile->stCHKFILE.stRetInfo.nPolicyType;
		stSecuLog.nEvtTime = GetCurrentDateTimeInt();
		stSecuLog.nRegDate = t_ValidTimeUtil->GetValidTime();
		HISYNCSTEPUP(stSecuLog.nSyncSvrStep);
		t_LogicMgrLogSecu->SetLogSecu(stSecuLog);
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

INT32	CThreadPoInAccFile::BypassObjectPath(PASI_CHK_PTN_FILE pChkPtnFile)
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


INT32	CThreadPoInAccFile::AnalyzeAccEvent(PASI_CHK_PTN_FILE pChkPtnFile)
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
			break;
		}

		nRetVal = ChkInPtn(pChkPtnFile, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal != 1)
		{
			nAcVal = RET_NONE;
			break;
		}
		if(nBlockMode == SS_PO_IN_PTN_OP_BLOCK_MODE_TYPE_DENY)
		{
			if(nIsWarning == 1)
				nAcVal = RET_WARN;
			else
				nAcVal = RET_DENY;
			SetRetValValue(&pChkPtnFile->stCHKFILE.stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
		}
		else
		{
			nAcVal = RET_NONE;
			SetRetValValue(&pChkPtnFile->stCHKFILE.stRetInfo, nAcVal, 0, 0, 0);
		}
	}while(FALSE);

	return nAcVal;
}

INT32	CThreadPoInAccFile::SendPolicyInfo(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	if(nClientFd < 0 || pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	PDB_PO_IN_PTN_OP pOpPolicy = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
	if(pOpPolicy != NULL)
	{
		pAccNotiPol->stPolInfo.nOpMode = pOpPolicy->tDPH.nUsedMode;
	}

	PDB_PO_IN_PTN_BL pBlPolicy = (PDB_PO_IN_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_BL);
	if(pBlPolicy)
	{
		pAccNotiPol->stPolInfo.nBLID = pBlPolicy->tDPH.nID;
		pAccNotiPol->stPolInfo.nBLUsedMode = pBlPolicy->tDPH.nUsedMode;
	}

	PDB_PO_IN_PTN_WL pWlPolicy = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_WL);
	if(pWlPolicy)
	{
		pAccNotiPol->stPolInfo.nWLID = pWlPolicy->tDPH.nID;
		pAccNotiPol->stPolInfo.nWLUsedMode = pWlPolicy->tDPH.nUsedMode;
	}

	pAccNotiPol->nCmdId = CMD_PIPE_SET_POLICY_INFO;
	nRetVal = SockWrite(nClientFd,(PVOID)pAccNotiPol, nSize);
	if(nRetVal < 0)
	{
		return -2;
	}
	return 0;
}

INT32	CThreadPoInAccFile::SendWhiteClear(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	TListFileHashInfo tFileHashList;
	if(nClientFd < 0 || pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	pAccNotiPol->nCmdId = CMD_PIPE_CLEAR_WHITE_FILE;
	nRetVal = SockWrite(nClientFd,(PVOID)pAccNotiPol, nSize);
	if(nRetVal < 0)
	{
		return -2;
	}
	return 0;
}

INT32	CThreadPoInAccFile::SendWhiteFile(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	TListFileHashInfo tFileHashList;
	if(nClientFd < 0 || pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	pAccNotiPol->nCmdId = CMD_PIPE_ADD_WHITE_FILE;
	nCount = t_MMPPGWO->t_ManagePtnProcFile->Count();
	if(nCount == 0)
	{
		return 0;
	}
	nCount = t_MMPPGWO->t_ManagePtnProcFile->GetWPathStr_FileHashList(tFileHashList);
	if(nCount == 0)
	{
		return 0;
	}			

	TListFileHashInfoItor begin, end;
	begin = tFileHashList.begin();	end = tFileHashList.end();
	for(begin; begin != end; begin++)
	{
		if(!_strnicmp(begin->acFullPath, "/bin/", 5))
		{
			strncpy(pAccNotiPol->stFileHash.acFullPath, begin->acFullPath, MAX_PATH-1);
			pAccNotiPol->stFileHash.acFullPath[MAX_PATH-1] = 0;
			strncpy(pAccNotiPol->stFileHash.acWhiteHash, begin->acWhiteHash, SHA512_BLOCK_SIZE);
			pAccNotiPol->stFileHash.acFullPath[SHA512_BLOCK_SIZE] = 0;
			nRetVal = SockWrite(nClientFd,(PVOID)pAccNotiPol, nSize);
			if(nRetVal < 0)
			{
				tFileHashList.clear();
				return -2;
			}
		}
	}
	tFileHashList.clear();
	return 0;
}

INT32	CThreadPoInAccFile::SendBlackClear(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	TListFileHashInfo tFileHashList;
	if(nClientFd < 0 || pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	pAccNotiPol->nCmdId = CMD_PIPE_CLEAR_BLACK_FILE;
	nRetVal = SockWrite(nClientFd,(PVOID)pAccNotiPol, nSize);
	if(nRetVal < 0)
	{
		return -2;
	}
	return 0;
}


INT32	CThreadPoInAccFile::SendBlackFile(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	TListFileHashInfo tFileHashList;
	if(nClientFd < 0 || pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	pAccNotiPol->nCmdId = CMD_PIPE_ADD_BLACK_FILE;
	nCount = t_MMPPGBO->t_ManagePtnProcFile->Count();
	if(nCount == 0)
	{
		return 0;
	}
	nCount = t_MMPPGBO->t_ManagePtnProcFile->GetWPathStr_FileHashList(tFileHashList);
	if(nCount == 0)
	{
		return 0;
	}			

	TListFileHashInfoItor begin, end;
	begin = tFileHashList.begin();	end = tFileHashList.end();
	for(begin; begin != end; begin++)
	{
		if(!_strnicmp(begin->acFullPath, "/bin/", 5))
		{
			strncpy(pAccNotiPol->stFileHash.acFullPath, begin->acFullPath, MAX_PATH-1);
			pAccNotiPol->stFileHash.acFullPath[MAX_PATH-1] = 0;
			strncpy(pAccNotiPol->stFileHash.acWhiteHash, begin->acWhiteHash, SHA512_BLOCK_SIZE);
			pAccNotiPol->stFileHash.acFullPath[SHA512_BLOCK_SIZE] = 0;
			nRetVal = SockWrite(nClientFd,(PVOID)pAccNotiPol, nSize);
			if(nRetVal < 0)
			{
				tFileHashList.clear();
				return -2;
			}
		}
	}
	tFileHashList.clear();
	return 0;
}

INT32	CThreadPoInAccFile::SendEndPolicy(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	TListFileHashInfo tFileHashList;
	if(nClientFd < 0 || pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	pAccNotiPol->nCmdId = CMD_PIPE_END_POLICY_INFO;
	nRetVal = SockWrite(nClientFd,(PVOID)pAccNotiPol, nSize);
	if(nRetVal < 0)
	{
		return -2;
	}
	return 0;
}


INT32	CThreadPoInAccFile::SendPolicy(INT32 nSendFlag)
{
	INT32 nClientFd = -1;
	INT32 nRetVal = 0;
	INT32 nState = 0;
	PASI_ACC_NOTIFY_POLICY pAccNotiPol = NULL;
	INT32 nClientLen = 0;
	struct sockaddr_un stClientAddr;
	char acSockPath[MAX_FILE_NAME] = {0,};

	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_POL_FILE);
	acSockPath[MAX_FILE_NAME-1] = 0;

	if(is_file(acSockPath) != SOCK_FILE)
	{
		return 0;
	}

	do{
		pAccNotiPol = (PASI_ACC_NOTIFY_POLICY)malloc(sizeof(ASI_ACC_NOTIFY_POLICY));
		if(pAccNotiPol == NULL)
		{
			WriteLogE("[SendPolicy] fail to allocate memory (%d)", errno);
			nRetVal = -1;
			break;
		}
		nClientFd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (nClientFd == -1)
		{
			WriteLogE("[SendPolicy] fail to create sock (%d)", errno);
			nRetVal = -2;
			break;
		}


		nClientLen = sizeof(stClientAddr);
		memset(&stClientAddr, 0, nClientLen);
		stClientAddr.sun_family = AF_UNIX;
		strncpy(stClientAddr.sun_path, acSockPath, MAX_FILE_NAME-1);
		nRetVal = connect(nClientFd, (struct sockaddr *)&stClientAddr, nClientLen);
		if (nRetVal < 0)
		{
			if(errno != 111)
				WriteLogE("[SendPolicy] fail to connect %s (%d)", acSockPath, errno);
			nRetVal = -3;
			break;
		}
		if(nSendFlag & AS_SEND_POLICY_INFO)
		{
			nRetVal = SendPolicyInfo(nClientFd, pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[SendPolicy] fail to send policy info %d (%d)", nRetVal, errno);
				nRetVal -= 10;
				break;
			}
		}
		if(nSendFlag & AS_SEND_WHITE_FILE)
		{
			nRetVal = SendWhiteClear(nClientFd, pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[SendPolicy] fail to send white file %d (%d)", nRetVal, errno);
				nRetVal -= 20;
				break;
			}
			nRetVal = SendWhiteFile(nClientFd, pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[SendPolicy] fail to send white file %d (%d)", nRetVal, errno);
				nRetVal -= 20;
				break;
			}
		}
		if(nSendFlag & AS_SEND_BLACK_FILE)
		{
			nRetVal = SendBlackClear(nClientFd, pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[SendPolicy] fail to send white file %d (%d)", nRetVal, errno);
				nRetVal -= 20;
				break;
			}
			nRetVal = SendBlackFile(nClientFd, pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[SendPolicy] fail to send black file %d (%d)", nRetVal, errno);
				nRetVal -= 30;
				break;
			}
		}
		nRetVal = SendEndPolicy(nClientFd, pAccNotiPol);
		if (nRetVal < 0)
		{
			WriteLogE("[SendPolicy] fail to send end policy cmd %d (%d)", nRetVal, errno);
			nRetVal -= 40;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	if(nClientFd != -1)
	{
		close(nClientFd);
	}
	safe_free(pAccNotiPol);
	return nRetVal;
}


INT32		CThreadPoInAccFile::CheckSockEvent(INT32 nClientFd, PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nRetVal = 0;
	INT32 nAcVal = RET_NONE;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);

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
		else if(pChkPtnFile->stCHKFILE.nCmdId == CMD_PIPE_PO_IN_ACCESS_FILE)
		{
			nAcVal = pChkPtnFile->stCHKFILE.stRetInfo.nAcVal;
			if(nAcVal == RET_DENY || nAcVal == RET_WARN)
			{
				SetLogAccEvent(pChkPtnFile);
			}
		}
		else if(pChkPtnFile->stCHKFILE.nCmdId == CMD_PIPE_REQ_ACCESS_INFO)
		{
			SendPolicy(AS_SEND_POLICY_ALL);
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

