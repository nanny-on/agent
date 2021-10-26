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
	m_pString = NULL;
	m_nTestTime = 0;
	m_nTestCount = 0;
	m_fTotalDiffTime = 0;
}

CThreadPoInAccFile::~CThreadPoInAccFile()
{
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

INT32 CThreadPoInAccFile::InitShm()
{
	INT32 nRetVal = 0;
	size_t nSize = sizeof(ASI_CHK_FILE_PROC)+8;
	do{
		m_nShmId = shmget((key_t)SM_CHK_ACCESS_FILE_KEY, nSize, 0777 | IPC_CREAT);
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
		nRetVal = InitShm();
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
	INT32 i, nRetVal = 0;
	INT32 nState = 0;
	pthread_t tid = 0;
	PASI_CHK_PTN_FILE pChkPtnFile = NULL;

	Sleep(3000);
	tid = syscall(SYS_gettid);
	nRetVal = setpriority(PRIO_PROCESS, tid, -10);
	if(nRetVal < 0)
	{
		WriteLogE("fail to set priority: [chk_thread][%d]", errno);
		return -2;
	}

	pChkPtnFile = (PASI_CHK_PTN_FILE)malloc(sizeof(ASI_CHK_PTN_FILE));
	if(pChkPtnFile == NULL)
	{
		WriteLogE("fail to allocate memory : [%s][%d]", m_strThreadName.c_str(), errno);
		return -2;
	}

	nRetVal = InitNotifyEvent();
	if(nRetVal == 0)
	{
		m_nPause = 0;
		SendPolicy(AS_SEND_POLICY_ALL);
	}
	else
	{
		m_nPause = 1;
		WriteLogE("fail to init notify event : [%s][%d]", m_strThreadName.c_str(), nRetVal);
	}

	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{
			nRetVal = CheckShmEvent(pChkPtnFile);
			if(nRetVal < 0)
			{
				UninitShm();
				m_nPause = 1;
				Sleep(1000);
			}
		}
		else
		{
			nRetVal = InitNotifyEvent();
			if(nRetVal == 0)
			{
				m_nPause = 0;
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

	UninitShm();
	safe_free(pChkPtnFile);
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

INT32	CThreadPoInAccFile::SendWhiteFile(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol, INT32 &nSendCount)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSend = 0;
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
		if(!_strnicmp(begin->acFullPath, "/bin/", 5) || strstr(begin->acFullPath, "_file_") != NULL)
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
			nSend++;
		}
	}
	tFileHashList.clear();
	nSendCount = nSend++;
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


INT32	CThreadPoInAccFile::SendBlackFile(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol, INT32 &nSendCount)
{
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	INT32 nSend = 0;
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
		if(!_strnicmp(begin->acFullPath, "/bin/", 5) || strstr(begin->acFullPath, "_file_") != NULL)
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
			nSend++;
		}
	}
	tFileHashList.clear();
	nSendCount = nSend;
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
	INT32 nSendCnt = 0;
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
			else
				WriteLogN("success to send policy info");
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
			nSendCnt = 0;
			nRetVal = SendWhiteFile(nClientFd, pAccNotiPol, nSendCnt);
			if (nRetVal < 0)
			{
				WriteLogE("[SendPolicy] fail to send white file %d (%d)", nRetVal, errno);
				nRetVal -= 20;
				break;
			}
			else
				WriteLogN("success to send white file %d", nSendCnt);
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
			nSendCnt = 0;
			nRetVal = SendBlackFile(nClientFd, pAccNotiPol, nSendCnt);
			if (nRetVal < 0)
			{
				WriteLogE("[SendPolicy] fail to send black file %d (%d)", nRetVal, errno);
				nRetVal -= 30;
				break;
			}
			else
				WriteLogN("success to send black file %d", nSendCnt);
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
		else if(pChkPtnFile->stCHKFILE.nCmdId == CMD_PIPE_REQ_ACCESS_INFO)
		{
			SendPolicy(AS_SEND_POLICY_ALL);
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

