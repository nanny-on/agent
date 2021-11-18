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
	m_nShmId = -1;
	m_pString = NULL;
	m_nTestTime = 0;
	m_fTotalDiffTime = 0;
	m_nTestCount = 0;
	m_nCheckThread = ASI_DISCONNECT_STATE;
	pthread_mutex_init(&m_ShmMutex, NULL);
}

CThreadPoInPtnFile::~CThreadPoInPtnFile()
{
	pthread_mutex_destroy(&m_ShmMutex);
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

INT32 CThreadPoInPtnFile::InitShm()
{
	INT32 nRetVal = 0;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;
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
		pChkFileProc = (PASI_CHK_FILE_PROC)m_pString;
		pChkFileProc->nState = SM_WRITE_CLIENT_FLAG;
		nRetVal = 0;
	}while(FALSE);

	if(nRetVal < 0)
		UninitShm();
	return nRetVal;
}


VOID CThreadPoInPtnFile::UninitShm()
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

INT32	CThreadPoInPtnFile::InitNotifyEvent()
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

INT32 CThreadPoInPtnFile::ShmRecv(PVOID pRecvData, INT32 nReqSize)
{
	INT32 nContinueCount = 0;
	PASI_CHK_FILE_PROC pChkFileProc = NULL;

	if(pRecvData == NULL || nReqSize < 1 || m_pString == NULL)
	{
		return -1;
	}
	pChkFileProc = (PASI_CHK_FILE_PROC)m_pString;
	while (nContinueCount < 50)
	{
		if(pChkFileProc->nState == SM_READ_SERVER_FLAG)
		{
			memcpy(pRecvData, m_pString, nReqSize);
			pChkFileProc->nState = SM_WRITE_CLIENT_FLAG;
			return 0;
		}
		nContinueCount++;
		Sleep(100);
	}
	return 1;
}

INT32 CThreadPoInPtnFile::Run()
{
	// TODO: Add your specialized code here and/or call the base class
	INT32 i, nRetVal = 0;
	INT32 nState = 0;
	PASI_CHK_PTN_FILE pChkPtnFile = NULL;
	nRetVal = LoadWhitePattern();
	if(nRetVal < 0)
	{
		WriteLogE("fail to load white pattern : [%s][%d]", m_strThreadName.c_str(), nRetVal);
		return -1;
	}

	pChkPtnFile = (PASI_CHK_PTN_FILE)malloc(sizeof(ASI_CHK_PTN_FILE));
	if(pChkPtnFile == NULL)
	{
		WriteLogE("fail to allocate memory : [%s][%d]", m_strThreadName.c_str(), errno);
		UnloadWhitePattern();
		return -2;
	}
	memset(pChkPtnFile, 0, sizeof(ASI_CHK_PTN_FILE));
	nRetVal = InitNotifyEvent();
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

	UnloadWhitePattern();
	UninitShm();
	safe_free(pChkPtnFile);
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
	if(pdata_sp == NULL)
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
	if(pChkPtnFile == NULL)
	{
		return -1;
	}
	DB_LOG_SECU stSecuLog;
	stSecuLog.strSubjectPath = pChkPtnFile->stCHKFILE.stProcInfo.acPath;
	stSecuLog.strSubjectName = pChkPtnFile->stCHKFILE.stProcInfo.acFile;
	stSecuLog.strObjectPath	= pChkPtnFile->stCHKFILE.stFileInfo.acPath;
	stSecuLog.strObjectName	= pChkPtnFile->stCHKFILE.stFileInfo.acFile;
	if(pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash[0] != 0)
		stSecuLog.strExtInfo = pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash;
		
	stSecuLog.nOpType = LOG_PROCESS_ACCESS_DENIED;
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
	return 0;
}

INT32	CThreadPoInPtnFile::AnalyzeAccEvent(PASI_CHK_PTN_FILE pChkPtnFile)
{
	INT32 nAcVal = 0;
	if(pChkPtnFile == NULL)
	{
		return -1;
	}
	nAcVal = pChkPtnFile->stCHKFILE.stRetInfo.nAcVal;
	if(nAcVal == RET_DENY || nAcVal == RET_WARN)
	{
		DB_LOG_SECU stSecuLog;
		stSecuLog.strSubjectPath = pChkPtnFile->stCHKFILE.stProcInfo.acPath;
		stSecuLog.strSubjectName = pChkPtnFile->stCHKFILE.stProcInfo.acFile;
		stSecuLog.strObjectPath	= pChkPtnFile->stCHKFILE.stFileInfo.acPath;
		stSecuLog.strObjectName	= pChkPtnFile->stCHKFILE.stFileInfo.acFile;
		if(pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash[0] != 0)
			stSecuLog.strExtInfo = pChkPtnFile->stCHKFILE.stRetInfo.acWhiteHash;
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
	}
	return 0;
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
		dwFileType = 0;
		nRetVal = m_tWEDLLUtil.GetWL(pChkPtnFile->stCHKFILE.stFileInfo.acFullPath, (PVOID)&pChkPtnFile->stAWWE, sizeof(ASI_WENG_WL_EX), &dwFileType);
		if(nRetVal < 0 || dwFileType == AS_INVALID_FILE)
		{
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
			SetRetValValue(&pChkPtnFile->stCHKFILE.stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
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
			break;
		}
		nRetVal = ChkInPtn(pChkPtnFile->stAWWE.acWhiteHash, nBlockMode, nIsWarning, nPolicyType);
		if(nRetVal != 1)
		{
			nAcVal = RET_NONE;
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
		if(pChkPtnFile->stCHKFILE.stRetInfo.nPolicyType == 0)
			SetRetValValue(&pChkPtnFile->stCHKFILE.stRetInfo, nAcVal, nBlockMode, nIsWarning, nPolicyType);
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

INT32		CThreadPoInPtnFile::CheckShmEvent(PASI_CHK_PTN_FILE pChkPtnFile)
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
		else if(nRetVal == 1)
		{
			nRetVal = check_proc_exist_by_name(ACCNOTIFYD_IDENT, 0);
			if(nRetVal == ASI_PROC_EXIST)
			{
				nRetVal = 0;
				break;
			}
			else
			{
				if(nRetVal < 0)
					nRetVal -= 20;
				break;
			}
		}

		if(pChkPtnFile->stCHKFILE.nCmdId == CMD_PIPE_PO_IN_EXECUTE_FILE)
		{
#ifdef _PERP_TEST_LOG
			if(!_stricmp(pChkPtnFile->stCHKFILE.stProcInfo.acFile, "exe_test_pgm"))
			{
				bIsTestPgm = TRUE;
				gettimeofday(&stStartTime, NULL);
			}
#endif
			nAcVal = pChkPtnFile->stCHKFILE.stRetInfo.nAcVal;
			AnalyzeExecEvent(pChkPtnFile);
#ifdef _PERP_TEST_LOG
			if(bIsTestPgm == TRUE)
			{
				fDiffTime = diff_time(stStartTime);
			}
			if(nAcVal == RET_DENY || nAcVal == RET_WARN)
			{
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
			}
			else
			{
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
			}
#endif /*_PERP_TEST_LOG*/

			if(nAcVal == RET_DENY || nAcVal == RET_WARN)
			{
				SetLogExecEvent(pChkPtnFile);
			}
		}
		else if(pChkPtnFile->stCHKFILE.nCmdId == CMD_PIPE_PO_IN_ACCESS_FILE)
		{
#ifdef _PERP_TEST_LOG
			if(!_stricmp(pChkPtnFile->stCHKFILE.stProcInfo.acFile, "cp_test"))
			{
				bIsTestPgm = TRUE;
				gettimeofday(&stStartTime, NULL);
			}
#endif
			nAcVal = pChkPtnFile->stCHKFILE.stRetInfo.nAcVal;
			AnalyzeAccEvent(pChkPtnFile);
			if(nAcVal == RET_DENY || nAcVal == RET_WARN)
			{
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

