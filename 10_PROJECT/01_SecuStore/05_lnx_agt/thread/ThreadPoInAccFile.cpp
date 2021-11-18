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
	m_nShmId = -1;
	m_nClientFd = -1;
	m_pString = NULL;
	m_nTestTime = 0;
	m_nTestCount = 0;
	m_fTotalDiffTime = 0;
	m_nSendTime = 0;
	m_nSendingMode = 0;
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

INT32 CThreadPoInAccFile::LoadWhitePattern()
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

VOID CThreadPoInAccFile::UnloadWhitePattern()
{
	m_tWEDLLUtil.ClearFile();
	m_tWEDLLUtil.FreeLibraryExt();
}


/////////////////////////////////////////////////////////////////////////////
// CThreadPoInAccFile message handlers

INT32 CThreadPoInAccFile::InitShm()
{
	INT32 nRetVal = 0;
	size_t nSize = sizeof(ASI_CHK_FILE_PROC)+8;
	do{
		m_nShmId = shmget((key_t)SM_OLD_ACCESS_FILE_KEY, nSize, 0777 | IPC_CREAT);
		if (m_nShmId < 0)
		{
			nRetVal = -1;
			m_nShmId = -1;
			break;
		}
		m_pString = (char *)shmat(m_nShmId, NULL, 0);
		if(m_pString == (char *)-1)
		{
			nRetVal = -2;
			m_pString = NULL;
			break;
		}
		nRetVal = 0;
	}while(FALSE);

	if(nRetVal < 0)
		UninitShm();
	return nRetVal;
}

VOID CThreadPoInAccFile::UninitShm()
{
	if(m_pString != NULL)
	{
		shmdt(m_pString);
		m_pString = NULL;
	}
	if(m_nShmId != -1)
	{
		shmctl(m_nShmId,IPC_RMID,NULL);
		m_nShmId = -1;
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

VOID CThreadPoInAccFile::GetSockId(INT32 &nClientFd)
{
	pthread_mutex_lock (&m_SockMutex);
	nClientFd = m_nClientFd;
	pthread_mutex_unlock (&m_SockMutex);
}

VOID CThreadPoInAccFile::SetSockId(INT32 nClientFd)
{
	pthread_mutex_lock (&m_SockMutex);
	m_nClientFd = nClientFd;
	pthread_mutex_unlock (&m_SockMutex);
}


INT32 CThreadPoInAccFile::InitSockEnv()
{
	INT32 nClientFd = -1;
	INT32 nRetVal = 0;
	INT32 nClientLen = 0;
	struct sockaddr_un stClientAddr;
	char acSockPath[MAX_FILE_NAME] = {0,};

	snprintf(acSockPath, MAX_FILE_NAME-1, "%s/%s/pem/%s", NANNY_INSTALL_DIR, NANNY_AGENT_DIR, UNIX_SOCK_ACC_FILE);
	acSockPath[MAX_FILE_NAME-1] = 0;

	nRetVal = check_proc_exist_by_name(ACCNOTIFYD_IDENT, 0);
	if(nRetVal != ASI_PROC_EXIST || is_file(acSockPath) != SOCK_FILE)
	{
		return -1;
	}

	do{
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
			UninitSockEnv();
			nRetVal = -3;
			break;
		}
		SetSockId(nClientFd);
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

VOID CThreadPoInAccFile::UninitSockEnv()
{
	pthread_mutex_lock (&m_SockMutex);
	if(m_nClientFd != -1)
	{
		close(m_nClientFd);
		m_nClientFd = -1;
	}
	pthread_mutex_unlock (&m_SockMutex);
}


INT32	CThreadPoInAccFile::InitNotifyEvent()
{
	INT32 nRetVal = 0;
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

		nRetVal = InitSockEnv();
		if(nRetVal < 0)
		{
			nRetVal -= 300;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

INT32 CThreadPoInAccFile::ShmRecv(PVOID pRecvData, INT32 nReqSize)
{
	INT32 nContinueCount = 0;
	INT32 nRetVal = 0;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;

	if(pRecvData == NULL || nReqSize < 1 || m_pString == NULL)
	{
		return -1;
	}
	pChkFileProc = (PASI_CHK_FILE_PROC)m_pString;
	while (nContinueCount < 200)
	{
		if(pChkFileProc->nState == SM_READ_SERVER_FLAG)
		{
			memcpy(pRecvData, m_pString, sizeof(ASI_CHK_FILE_PROC));
			pChkFileProc->nState = SM_WRITE_SERVER_FLAG;
			return 0;
		}
		nContinueCount++;
		Sleep(10);
	}
	return 1;
}

INT32 CThreadPoInAccFile::ShmWrite(PVOID pWriteData, INT32 nReqSize)
{
	INT32 nContinueCount = 0;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
	if(pWriteData == NULL || nReqSize < 1 || m_pString == NULL)
	{
		return -1;
	}
	pChkFileProc = (PASI_CHK_FILE_PROC)m_pString;
	while (nContinueCount < 10)
	{
		if(pChkFileProc->nState == SM_WRITE_SERVER_FLAG)
		{
			memcpy(m_pString, pWriteData, sizeof(ASI_CHK_FILE_PROC));
			pChkFileProc->nState = SM_READ_CLIENT_FLAG;
			return 0;
		}
		nContinueCount++;
		Sleep(50);
	}
	return 1;
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

INT32 CThreadPoInAccFile::Run()
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 nRetVal = 0;
	nRetVal = LoadWhitePattern();
	if(nRetVal < 0)
	{
		WriteLogE("[%s] fail to load white pattern : [%d]", m_strThreadName.c_str(), nRetVal);
		return -1;
	}

	nRetVal = InitNotifyEvent();
	if(nRetVal == 0)
	{
		m_nPause = 0;
		Sleep(500);
		SendPolicy(AS_SEND_POLICY_ALL);
	}
	else
	{
		m_nPause = 1;
		if(nRetVal != 301)
			WriteLogE("fail to init notify event : [%s][%d]", m_strThreadName.c_str(), nRetVal);
	}

	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{
			nRetVal = check_proc_exist_by_name(ACCNOTIFYD_IDENT, 0);
			if(nRetVal != ASI_PROC_EXIST)
			{
				m_nPause = 1;
			}
			Sleep(1000);
		}
		else
		{
			nRetVal = InitNotifyEvent();
			if(nRetVal == 0)
			{
				m_nPause = 0;
				Sleep(500);
				SendPolicy(AS_SEND_POLICY_ALL);
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

	UninitSockEnv();
	UnloadWhitePattern();
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

VOID	CThreadPoInAccFile::GetSendTime(UINT32 &nSendTime)
{
	pthread_mutex_lock (&m_SockMutex);
	nSendTime = m_nSendTime;
	pthread_mutex_unlock (&m_SockMutex);
}

INT32	CThreadPoInAccFile::SendCmdInfoWithLock(PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	if(pAccNotiPol == NULL)
		return -1;

	nRetVal = check_proc_exist_by_name(ACCNOTIFYD_IDENT, 0);
	if(nRetVal != ASI_PROC_EXIST)
	{
		return -3;
	}
	pthread_mutex_lock (&m_SockMutex);
	do{
		if(m_nClientFd == -1)
		{
			nRetVal = -4;
			break;
		}
		nRetVal = SockWrite(m_nClientFd,(PVOID)pAccNotiPol, nSize);
		if(nRetVal < 0)
		{
			nRetVal = -2;
			break;
		}
		m_nSendTime = GetCurrentDateTimeInt();
		nRetVal = 0;
	}while(FALSE);
	pthread_mutex_unlock (&m_SockMutex);
	return 0;
}

INT32 CThreadPoInAccFile::SendAliveMessage()
{
	INT32 nRetVal = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	UINT32 nSendTime = GetCurrentDateTimeInt();
	UINT32 nOldSendTime = 0;
	PASI_ACC_NOTIFY_POLICY pAccNotiPol = NULL;
	return 0;
	pAccNotiPol = (PASI_ACC_NOTIFY_POLICY)malloc(sizeof(ASI_ACC_NOTIFY_POLICY));
	if(pAccNotiPol == NULL)
	{
		return -1;
	}
	memset(pAccNotiPol, 0, nSize);

	GetSendTime(nOldSendTime);
	pAccNotiPol->nCmdId = CMD_PIPE_ALIVE_MESSAGE;
	do{
		if(nSendTime - nOldSendTime < 10)
		{
			nRetVal = 0;
			break;
		}
		nRetVal = SendCmdInfoWithLock(pAccNotiPol);
		if(nRetVal < 0)
		{
			nRetVal = -10;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	safe_free(pAccNotiPol);
	return nRetVal;
}

INT32	CThreadPoInAccFile::GetSendingMode()
{
	INT32 nSendingMode = 0;
	pthread_mutex_lock (&m_SockMutex);
	nSendingMode = m_nSendingMode;
	pthread_mutex_unlock (&m_SockMutex);
	return nSendingMode;
}

VOID	CThreadPoInAccFile::SetSendingMode(INT32 nSendingMode)
{
	pthread_mutex_lock (&m_SockMutex);
	m_nSendingMode = nSendingMode;
	pthread_mutex_unlock (&m_SockMutex);
}

INT32	CThreadPoInAccFile::SendPolicyInfo(PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	if(pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	PDB_PO_IN_PTN_OP pOpPolicy = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_OP);
	if(pOpPolicy != NULL)
	{
		pAccNotiPol->stPolInfo.nOpMode = pOpPolicy->tDPH.nUsedMode;
		pAccNotiPol->stPolInfo.nOpID = pOpPolicy->tDPH.nID; 
		pAccNotiPol->stPolInfo.nOpBlockMode = pOpPolicy->nBlockMode;
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

	PDB_PO_IN_PTN_SP pdata_sp = (PDB_PO_IN_PTN_SP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_SP);
	if(pdata_sp)
	{
		pAccNotiPol->stPolInfo.nSpUsedMode = pdata_sp->tDPH.nUsedMode;
	}

	PDB_PO_IN_PTN_EX pPtnExPolicy = (PDB_PO_IN_PTN_EX)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_EX);
	if(pPtnExPolicy)
	{
		pAccNotiPol->stPolInfo.nExUsedMode = pPtnExPolicy->tDPH.nUsedMode;
	}

	pAccNotiPol->nCmdId = CMD_PIPE_SET_POLICY_INFO;
	nRetVal = SendCmdInfoWithLock(pAccNotiPol);
	if(nRetVal < 0)
	{
		nRetVal = -10;
		return nRetVal;
	}
	return 0;
}

INT32	CThreadPoInAccFile::SendWhiteClear(PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	if(pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	pAccNotiPol->nCmdId = CMD_PIPE_CLEAR_WHITE_FILE;
	nRetVal = SendCmdInfoWithLock(pAccNotiPol);
	if(nRetVal < 0)
	{
		nRetVal = -10;
		return nRetVal;
	}
	return 0;
}

INT32	CThreadPoInAccFile::SendWhiteFile(PASI_ACC_NOTIFY_POLICY pAccNotiPol, INT32 &nSendCount)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSend = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	TListFileHashInfo tFileHashList;
	if(pAccNotiPol == NULL)
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
		strncpy(pAccNotiPol->stFileHash.acFullPath, begin->acFullPath, MAX_PATH-1);
		pAccNotiPol->stFileHash.acFullPath[MAX_PATH-1] = 0;
		strncpy(pAccNotiPol->stFileHash.acWhiteHash, begin->acWhiteHash, SHA512_BLOCK_SIZE);
		pAccNotiPol->stFileHash.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		nRetVal = SendCmdInfoWithLock(pAccNotiPol);
		if(nRetVal < 0)
		{
			tFileHashList.clear();
			nRetVal = -10;
			return nRetVal;
		}
		nSend++;
	}
	tFileHashList.clear();
	nSendCount = nSend++;
	return 0;
}

INT32	CThreadPoInAccFile::SendBlackClear(PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	if(pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	pAccNotiPol->nCmdId = CMD_PIPE_CLEAR_BLACK_FILE;
	nRetVal = SendCmdInfoWithLock(pAccNotiPol);
	if(nRetVal < 0)
	{
		nRetVal = -10;
		return nRetVal;
	}
	return 0;
}

INT32	CThreadPoInAccFile::SendBlackFile(PASI_ACC_NOTIFY_POLICY pAccNotiPol, INT32 &nSendCount)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSend = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	TListFileHashInfo tFileHashList;
	if(pAccNotiPol == NULL)
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
		strncpy(pAccNotiPol->stFileHash.acFullPath, begin->acFullPath, MAX_PATH-1);
		pAccNotiPol->stFileHash.acFullPath[MAX_PATH-1] = 0;
		strncpy(pAccNotiPol->stFileHash.acWhiteHash, begin->acWhiteHash, SHA512_BLOCK_SIZE);
		pAccNotiPol->stFileHash.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		nRetVal = SendCmdInfoWithLock(pAccNotiPol);
		if(nRetVal < 0)
		{
			tFileHashList.clear();
			nRetVal -= 10;
			return nRetVal;
		}
		nSend++;
	}
	tFileHashList.clear();
	nSendCount = nSend;
	return 0;
}

INT32	CThreadPoInAccFile::SendSpClear(PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	if(pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	pAccNotiPol->nCmdId = CMD_PIPE_CLEAR_SP_FILE;
	nRetVal = SendCmdInfoWithLock(pAccNotiPol);
	if(nRetVal < 0)
	{
		nRetVal = -10;
		return nRetVal;
	}
	return 0;
}

INT32	CThreadPoInAccFile::SendSpFile(PASI_ACC_NOTIFY_POLICY pAccNotiPol, INT32 &nSendCount)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSend = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	ASI_WENG_WL_EX tAWWE;
	DWORD dwFileType = 0;
	if(pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	TListDBPoInPtnSPRule tRuleList;
	TListDBPoInPtnSPRuleItor begin, end;
	PDB_PO_IN_PTN_SP_RULE pdata_rule = NULL;
	t_ManagePoInPtnSPRule->GetItemCopy(tRuleList);

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
		if(pdata_rule->strFilePath.empty() || pdata_rule->strFileName.empty())
			continue;
		snprintf(pAccNotiPol->stFileHash.acFullPath, MAX_PATH-1, "%s/%s", pdata_rule->strFilePath.c_str(), pdata_rule->strFileName.c_str());
		pAccNotiPol->stFileHash.acFullPath[MAX_PATH-1] = 0;
		dwFileType = 0;
		memset(&tAWWE, 0, sizeof(ASI_WENG_WL_EX));
		if(m_tWEDLLUtil.GetWL(pAccNotiPol->stFileHash.acFullPath, (PVOID)&tAWWE, sizeof(tAWWE), &dwFileType) == ERROR_SUCCESS && dwFileType != AS_INVALID_FILE)
		{
			strncpy(pAccNotiPol->stFileHash.acWhiteHash, tAWWE.acWhiteHash, SHA512_BLOCK_SIZE);
			pAccNotiPol->stFileHash.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
			if(pdata_rule->nConfirmRst == SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_DENY)
			{
				pAccNotiPol->nCmdId = CMD_PIPE_ADD_DENY_SP_FILE;
			}
			else
			{
				pAccNotiPol->nCmdId = CMD_PIPE_ADD_ALLOW_SP_FILE;
			}
			nRetVal = SendCmdInfoWithLock(pAccNotiPol);
			if(nRetVal < 0)
			{
				nRetVal -= 10;
				return nRetVal;
			}
			nSendCount++;
		}
	}
	nSendCount = nSend;
	return 0;
}

INT32	CThreadPoInAccFile::SendExClear(PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	if(pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

	pAccNotiPol->nCmdId = CMD_PIPE_CLEAR_EX_FILE;
	nRetVal = SendCmdInfoWithLock(pAccNotiPol);
	if(nRetVal < 0)
	{
		nRetVal = -10;
		return nRetVal;
	}
	return 0;
}

INT32	CThreadPoInAccFile::SendExFile(PASI_ACC_NOTIFY_POLICY pAccNotiPol, INT32 &nSendCount)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSend = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	ASI_WENG_WL_EX tAWWE;
	DWORD dwFileType = 0;
	if(pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);

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

		memset(&tAWWE, 0, sizeof(ASI_WENG_WL_EX));
		snprintf(pAccNotiPol->stFileHash.acFullPath, MAX_PATH-1, "%s", pdata_unit->tDFI.strPath.c_str(), MAX_PATH-1);
		pAccNotiPol->stFileHash.acFullPath[MAX_PATH-1] = 0;
		dwFileType = 0;
		if(m_tWEDLLUtil.GetWL(pAccNotiPol->stFileHash.acFullPath, (PVOID)&tAWWE, sizeof(tAWWE), &dwFileType) == ERROR_SUCCESS && dwFileType != AS_INVALID_FILE)
		{
			strncpy(pAccNotiPol->stFileHash.acWhiteHash, tAWWE.acWhiteHash, SHA512_BLOCK_SIZE);
			pAccNotiPol->stFileHash.acWhiteHash[SHA512_BLOCK_SIZE] = 0;
			if(pdata_unit->nBlockMode == SS_PO_IN_PTN_EX_BLOCK_TYPE_DENY)
			{
				pAccNotiPol->nCmdId = CMD_PIPE_ADD_DENY_EX_FILE;
			}
			else
			{
				pAccNotiPol->nCmdId = CMD_PIPE_ADD_ALLOW_EX_FILE;
			}
			nRetVal = SendCmdInfoWithLock(pAccNotiPol);
			if(nRetVal < 0)
			{
				nRetVal -= 10;
				return nRetVal;
			}
			nSendCount++;
		
		}
	}
	nSendCount = nSend;
	return 0;
}

INT32	CThreadPoInAccFile::SendAddCreateFile(char *acFullPath, char *acWhiteHash)
{
	INT32 nRetVal = 0;
	INT32 i, nSendingMode = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	PASI_ACC_NOTIFY_POLICY pAccNotiPol = NULL;

	if(acFullPath == NULL || acWhiteHash == NULL)
	{
		return -1;
	}

	if(acFullPath[0] == 0 || acWhiteHash[0] == 0)
	{
		return -2;
	}

	pAccNotiPol = (PASI_ACC_NOTIFY_POLICY)malloc(sizeof(ASI_ACC_NOTIFY_POLICY));
	if(pAccNotiPol == NULL)
	{
		return -3;
	}
	memset(pAccNotiPol, 0, sizeof(ASI_ACC_NOTIFY_POLICY));

	for(i=0; i<300; i++)
	{
		nSendingMode = GetSendingMode();
		if(nSendingMode == 0)
			break;
		Sleep(100);
	}
	if(nSendingMode == 1)
	{
		WriteLogE("[%s] fail to send policy : [%d]", m_strThreadName.c_str(), errno);
		safe_free(pAccNotiPol);
		return -4;
	}
	SetSendingMode(1);

	strncpy(pAccNotiPol->stFileHash.acFullPath, acFullPath, MAX_PATH-1);
	pAccNotiPol->stFileHash.acFullPath[MAX_PATH-1] = 0;
	
	strncpy(pAccNotiPol->stFileHash.acWhiteHash, acWhiteHash, SHA512_BLOCK_SIZE);
	pAccNotiPol->stFileHash.acWhiteHash[SHA512_BLOCK_SIZE] = 0;

	pAccNotiPol->nCmdId = CMD_PIPE_ADD_CREATE_FILE;

	nRetVal = SendCmdInfoWithLock(pAccNotiPol);
	if(nRetVal < 0)
	{
		nRetVal = -10;
		SetSendingMode(0);
		safe_free(pAccNotiPol);
		return nRetVal;
	}
	SetSendingMode(0);
	safe_free(pAccNotiPol);
	return 0;
}

INT32	CThreadPoInAccFile::SendClearCreateFile()
{
	INT32 nRetVal = 0;
	INT32 i, nSendingMode = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	PASI_ACC_NOTIFY_POLICY pAccNotiPol = NULL;
	pAccNotiPol = (PASI_ACC_NOTIFY_POLICY)malloc(sizeof(ASI_ACC_NOTIFY_POLICY));
	if(pAccNotiPol == NULL)
	{
		return -1;
	}
	memset(pAccNotiPol, 0, sizeof(ASI_ACC_NOTIFY_POLICY));

	for(i=0; i<300; i++)
	{
		nSendingMode = GetSendingMode();
		if(nSendingMode == 0)
			break;
		Sleep(100);
	}
	if(nSendingMode == 1)
	{
		WriteLogE("[%s] fail to send policy : [%d]", m_strThreadName.c_str(), errno);
		safe_free(pAccNotiPol);
		return -2;
	}
	SetSendingMode(1);

	nRetVal = SendCmdInfoWithLock(pAccNotiPol);
	if(nRetVal < 0)
	{
		SetSendingMode(0);
		safe_free(pAccNotiPol);
		nRetVal = -10;
		return nRetVal;
	}
	SetSendingMode(0);
	safe_free(pAccNotiPol);
	return 0;
}


INT32	CThreadPoInAccFile::SendStartPolicy(PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	if(pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);
	do{
		pAccNotiPol->nCmdId = CMD_PIPE_START_POLICY_INFO;
		nRetVal = SendCmdInfoWithLock(pAccNotiPol);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	return 0;
}

INT32	CThreadPoInAccFile::SendEndPolicy(PASI_ACC_NOTIFY_POLICY pAccNotiPol)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSize = sizeof(ASI_ACC_NOTIFY_POLICY);
	if(pAccNotiPol == NULL)
		return -1;

	memset(pAccNotiPol, 0, nSize);
	do{
		pAccNotiPol->nCmdId = CMD_PIPE_END_POLICY_INFO;
		nRetVal = SendCmdInfoWithLock(pAccNotiPol);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	return 0;
}


INT32	CThreadPoInAccFile::SendPolicy(INT32 nSendFlag)
{
	INT32 nRetVal = 0;
	INT32 nSendCnt = 0;
	INT32 i, nSendingMode = 0;
	PASI_ACC_NOTIFY_POLICY pAccNotiPol = NULL;
	pAccNotiPol = (PASI_ACC_NOTIFY_POLICY)malloc(sizeof(ASI_ACC_NOTIFY_POLICY));
	if(pAccNotiPol == NULL)
	{
		WriteLogE("[%s] fail to allocate memory : [%d]", m_strThreadName.c_str(), errno);
		return -1;
	}
	memset(pAccNotiPol, 0, sizeof(ASI_ACC_NOTIFY_POLICY));
	for(i=0; i<300; i++)
	{
		nSendingMode = GetSendingMode();
		if(nSendingMode == 0)
			break;
		Sleep(100);
	}
	if(nSendingMode == 1)
	{
		WriteLogE("[%s] fail to send policy : [%d]", m_strThreadName.c_str(), errno);
		safe_free(pAccNotiPol);
		return -2;
	}
	SetSendingMode(1);
	do{
		if(nSendFlag & AS_SEND_START_POLICY)
		{
			nRetVal = SendStartPolicy(pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[SendPolicy] fail to send start policy cmd %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 10;
				break;
			}
		}

		if(nSendFlag & AS_SEND_POLICY_INFO)
		{
			nRetVal = SendPolicyInfo(pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[%s] fail to send policy info %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 20;
				break;
			}
			WriteLogN("[%s] success to send policy info", m_strThreadName.c_str());
		}
		if(nSendFlag & AS_SEND_WHITE_FILE)
		{
			nRetVal = SendWhiteClear(pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[%s] fail to send white file clear cmd %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 70;
				break;
			}
			nSendCnt = 0;
			nRetVal = SendWhiteFile(pAccNotiPol, nSendCnt);
			if (nRetVal < 0)
			{
				WriteLogE("[%s] fail to send white file %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 80;
				break;
			}
			WriteLogN("[%s] success to send white file %d", m_strThreadName.c_str(), nSendCnt);
		}
		if(nSendFlag & AS_SEND_BLACK_FILE)
		{
			nRetVal = SendBlackClear(pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[%s] fail to send black file clear cmd %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 90;
				break;
			}
			nSendCnt = 0;
			nRetVal = SendBlackFile(pAccNotiPol, nSendCnt);
			if (nRetVal < 0)
			{
				WriteLogE("[%s] fail to send black file %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 100;
				break;
			}
			else
				WriteLogN("[%s] success to send black file %d", m_strThreadName.c_str(), nSendCnt);
		}
		if(nSendFlag & AS_SEND_SP_FILE)
		{
			nRetVal = SendSpClear(pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[%s] fail to send sp file clear cmd %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 30;
				break;
			}
			nSendCnt = 0;
			nRetVal = SendSpFile(pAccNotiPol, nSendCnt);
			if (nRetVal < 0)
			{
				WriteLogE("[%s] fail to send sp file %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 40;
				break;
			}
			WriteLogN("[%s] success to send sp file %d", m_strThreadName.c_str(), nSendCnt);
		}
		if(nSendFlag & AS_SEND_EX_FILE)
		{
			nRetVal = SendExClear(pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[%s] fail to send ex file clear cmd %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 50;
				break;
			}
			nSendCnt = 0;
			nRetVal = SendExFile(pAccNotiPol, nSendCnt);
			if (nRetVal < 0)
			{
				WriteLogE("[%s] fail to send ex file %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 60;
				break;
			}
			WriteLogN("[%s] success to send ex file %d", m_strThreadName.c_str(), nSendCnt);
		}
		if(nSendFlag & AS_SEND_END_POLICY)
		{
			nRetVal = SendEndPolicy(pAccNotiPol);
			if (nRetVal < 0)
			{
				WriteLogE("[%s] fail to send end policy cmd %d (%d)", m_strThreadName.c_str(), nRetVal, errno);
				nRetVal -= 110;
				break;
			}
		}
		nRetVal = 0;
	}while(FALSE);
	SetSendingMode(0);
	safe_free(pAccNotiPol);
	return nRetVal;
}

INT32	CThreadPoInAccFile::SetLogAccEvent(PASI_CHK_PTN_FILE pChkPtnFile)
{
	DB_LOG_SECU stSecuLog;
	if(pChkPtnFile == NULL)
	{
		return -1;
	}

	stSecuLog.strSubjectPath = pChkPtnFile->stCHKFILE.stProcInfo.acPath;
	stSecuLog.strSubjectName = pChkPtnFile->stCHKFILE.stProcInfo.acFile;
	stSecuLog.strObjectPath	= pChkPtnFile->stCHKFILE.stFileInfo.acPath;
	stSecuLog.strObjectName	= pChkPtnFile->stCHKFILE.stFileInfo.acFile;
	stSecuLog.strExtInfo = pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash;
	stSecuLog.nOpType = LOG_FILE_WRITE_DENIED;
	stSecuLog.nObjectType = DETECT_MODE_ACL;
	stSecuLog.nPolicyType = pChkPtnFile->stCHKFILE.stRetInfo.nPolicyType;
	stSecuLog.nEvtTime = GetCurrentDateTimeInt();
	stSecuLog.nRegDate = t_ValidTimeUtil->GetValidTime();

	if(pChkPtnFile->stCHKFILE.stRetInfo.nIsWarning == 1)
	{
		stSecuLog.nBlockType = ASI_EPS_LOG_TYPE_WARN;
	}
	else
	{
		stSecuLog.nBlockType = ASI_EPS_LOG_TYPE_DENY;
	}
	HISYNCSTEPUP(stSecuLog.nSyncSvrStep);
	t_LogicMgrLogSecu->SetLogSecu(stSecuLog);
	return 0;
}


INT32		CThreadPoInAccFile::CheckShmEvent(PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nRetVal = 0;
	INT32 nAcVal = RET_NONE;
	INT32 nSize = sizeof(ASI_CHK_FILE_PROC);
#ifdef _PERP_TEST_LOG
	BOOL bIsTestPgm = FALSE;
	double fDiffTime = 0;
	struct timeval stStartTime;
#endif
	if(pChkPtnFile == NULL)
	{
		return -1;
	}
	do{
		nRetVal = ShmRecv((PVOID)&pChkPtnFile->stCHKFILE, nSize);
		if(nRetVal != 0)
		{
			if(nRetVal < 0)
				nRetVal -= 10;
			break;
		}

		if(pChkPtnFile->stCHKFILE.nCmdId == CMD_PIPE_PO_IN_ACCESS_FILE)
		{
			nAcVal = pChkPtnFile->stCHKFILE.stRetInfo.nAcVal;
			if(nAcVal == RET_DENY || nAcVal == RET_WARN)
			{
#ifdef _PERP_TEST_LOG
				if(!_stricmp(pChkPtnFile->stCHKFILE.stProcInfo.acFile, "cp_test"))
				{
					bIsTestPgm = TRUE;
					gettimeofday(&stStartTime, NULL);
				}
#endif
				SetLogAccEvent(pChkPtnFile);
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
					WritePerfTest4Log("[%03d]\ttest the interval time for detection of authorized file [%s] [detection time : %.02f ms]", m_nTestCount, pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, fDiffTime/1000);
					if(m_nTestCount == 19)
					{
						fDiffTime = m_fTotalDiffTime/20;
						WritePerfTest4Log("[total]\ttest the interval time for detection of authorized file [total : %d files] [average time : %.02f ms]", m_nTestCount+1, fDiffTime/1000);
					}
				}
#endif /*_PERP_TEST_LOG*/
			}
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

