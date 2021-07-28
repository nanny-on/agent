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

#include "stdafx.h"
#include "com_struct.h"
#include "FileDeleteUtil.h"
#include "DocBackupUtil.h"

CDocBackupUtil*		t_DocBackupUtil = NULL;

CDocBackupUtil::CDocBackupUtil(void)
{
	m_strUtilName = "util doc backup";

	{
		m_tSkipNameMap["document.doc"] = "";
		m_tSkipNameMap["hancom.hwp"] = "";
		m_tSkipNameMap["image.jpg"] = "";
		m_tSkipNameMap["text.txt"] = "";

		m_tSkipNameMap["hangeul.hwp"] = "";
		m_tSkipNameMap["memo.txt"] = "";
		m_tSkipNameMap["photo.jpg"] = "";
		m_tSkipNameMap["sheet.xls"] = "";
		m_tSkipNameMap["word.doc"] = "";
	}

	{
		// #20191212 ::Work Ahnshy - 아래 서비스로 체크하면 랜섬웨어 기능과 충돌이 재현될 수 있습니다. 2.0.2.35버전이상부터 적용
		// alyac.aye - 트레이 아이콘 프로세스 (부팅 시 지연되어 올라옴.)
		// ayagent.aye - 메인 대화상자 (트레이아이콘 더블클릭해야 생성되는 프로세스, 메인대화상자가 없으면 프로세스 없음)
		//m_tSkipProcMap["alyac.aye"] = "";
		//m_tSkipProcMap["ayagent.aye"] = "";

		m_tSkipProcMap["ayrtsrv.aye"] = "";	// 메인상주 서비스 프로세스
		m_tSkipProcMap["ayupdsrv.aye"] = "";// 업데이트 서비스 프로세스 (거의 상주)
		// End Ahnshy
	}
}
//---------------------------------------------------------------------------

CDocBackupUtil::~CDocBackupUtil(void)
{
}
//---------------------------------------------------------------------------

INT32		CDocBackupUtil::InitEnv()
{
	m_tDirSizeMap.clear();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CDocBackupUtil::RemoveBackupFile(PDB_LOG_DOC pdld)
{
	INT32 nRtn = 0;
	PDB_PO_FA_BK pDpfb = NULL;
	UINT64 ddwKeepSize = 0;
	UINT64 ddwMaxKeepSize = 0;
	UINT64 ddwFreeSize = 0;
	UINT64 ddwTotal = 0, ddwAvail = 0;
	CFileUtil tFileUtil;
	String strBkPath = "";
	String strBkFileName = "";

	if(pdld == NULL)
	{
		WriteLogE("[%s] invalid input data", m_strUtilName.c_str());
		return 1;
	}
	
	m_tBkMutex.Lock();
	pDpfb = (PDB_PO_FA_BK)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_BK);
	if(pDpfb == NULL)	
	{
		WriteLogN("[%s] doc file backup pdpfb is null.", m_strUtilName.c_str());
		m_tBkMutex.UnLock();
		return 11;
	}

	strBkPath = SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_BACKUP_DOC_FILE);

	do 
	{
		if(pDpfb->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
		{
			WriteLogN("[%s] policy used mode off..", m_strUtilName.c_str());
			break;
		}
		
		if(pDpfb->nKeepSize)
		{
			ddwKeepSize = tFileUtil.GetDirSizeExt(strBkPath.c_str(), &m_tDirSizeMap);
			ddwMaxKeepSize = ((UINT64)pDpfb->nKeepSize * (ASI_GIGABYTE));
			if(ddwKeepSize  > ddwMaxKeepSize)
			{
				WriteLogN("[%s] available size large then check keep size : [lk:%llu]>[rk:%u]", m_strUtilName.c_str(), ddwKeepSize, pDpfb->nKeepSize);
				break;
			}
		}

		if(pDpfb->nFreeSize)
		{
			t_ResInfoDLLUtil->GetHddInfo(&ddwTotal, &ddwAvail, strBkPath.c_str());
			ddwFreeSize = (ddwTotal / 100) * pDpfb->nFreeSize;
			if(ddwFreeSize > ddwAvail)
			{
				WriteLogN("[%s] available size small then check free size : [a:%llu]<[f:%llu][%d]", m_strUtilName.c_str(), ddwAvail, ddwFreeSize, pDpfb->nFreeSize);
				break;
			}
		}
		
		if(difftime(GetCurrentDateTimeInt(), pdld->nBackupTime) < (pDpfb->nKeepDay * TIMER_INTERVAL_TIME_DAY))
		{
			m_tBkMutex.UnLock();
			return 12;
		}
	} while (FALSE);		
	
	strBkFileName = SPrintf("%s/%s", strBkPath.c_str(), pdld->strBkFileName.c_str());
	if(tFileUtil.FileExists(strBkFileName.c_str()) == FALSE)
	{
		WriteLogN("[%s] remove doc file backup not find file: [%d][%s/%s]:[%s]", m_strUtilName.c_str(), errno, pdld->strObjectPath.c_str(), pdld->strObjectName.c_str(), pdld->strBkFileName.c_str());
		pdld->nRemoveTime = t_ValidTimeUtil->GetValidTime();
		m_tBkMutex.UnLock();
		return 0;
	}

	t_FileDeleteUtil->SecureDeleteFile(strBkFileName.c_str(), 0, 0);
	if (tFileUtil.FileExists(strBkFileName.c_str()) == TRUE)
	{
		if (unlink(strBkFileName.c_str()) == -1)
		{
			WriteLogN("[%s] remove doc file delete fail : [%d]:[%s]", m_strUtilName.c_str(), errno, strBkFileName.c_str());
			if(t_LogicMgrPoFaDelFileAfterBoot != NULL)
				t_LogicMgrPoFaDelFileAfterBoot->InsertDelFileInfo(strBkFileName.c_str());
		}
	}
	pdld->nRemoveTime = t_ValidTimeUtil->GetValidTime();
	WriteLogN("[%s] remove doc backup file: [%s/%s][%s]", m_strUtilName.c_str(), pdld->strObjectPath.c_str(), pdld->strObjectName.c_str(), pdld->strBkFileName.c_str());
	m_tBkMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CDocBackupUtil::BackupFile(PDB_LOG_DOC pdld, UINT32 nDelMethod, UINT32 nDelCnt, UINT32 nLimitSize, UINT32 nLimitDelCnt, UINT32 nPoFaUsedMode)
{
	INT32 nRtn = 0;
	INT32 nLen = 0;
	CFileUtil tFileUtil;
	struct stat fStat;
	PDB_PO_FA_BK pDpfb = NULL;
	String strBkDir = "";
	String strTarFile;
	String strFullPath;
	String strBkFileName, strBkFilePath;
	INT32 nRemainLog = 1;
	ASI_COMP_SECU tACS;
	mode_t nMode = 0;
	UINT32 nDelTime = 0;
	char acPath[MAX_PATH] = {0,};
	memset(&tACS, 0, sizeof(tACS));
	memset(&fStat, 0, sizeof(fStat));

	if(pdld == NULL)	
	{
		WriteLogN("[%s] invalid input data.", m_strUtilName.c_str());
		return -1;
	}

	if(pdld->strObjectName.find("%") != string::npos || pdld->strObjectPath.find("%") != string::npos || 
		pdld->strSubjectName.find("%") != string::npos || pdld->strSubjectPath.find("%") != string::npos)
	{
		nRemainLog = 0;
	}	
	m_tBkMutex.Lock();

	pDpfb = (PDB_PO_FA_BK)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_BK);
	if(pDpfb == NULL)	
	{
		WriteLogN("[%s] fail to get doc file backup policy.", m_strUtilName.c_str());
		m_tBkMutex.UnLock();
		return -2;
	}

	strncpy(acPath, t_EnvInfo->m_strRootPath.c_str(), MAX_PATH-2);
	acPath[MAX_PATH-2] = 0;
	nLen = strlen(acPath);
	if(nLen < 1)
	{
		WriteLogN("[%s] invalid root path.", m_strUtilName.c_str());
		m_tBkMutex.UnLock();
		return -3;
	}

	if(acPath[nLen-1] != '/')
	{
		acPath[nLen] = '/';
		acPath[nLen+1] = 0;
	}

	strBkDir = SPrintf("%s%s", acPath, STR_BACKUP_DOC_FILE);
	if (tFileUtil.FileExists(strBkDir.c_str()) == FALSE)
	{
		tFileUtil.ForceDirectory(strBkDir.c_str());
	}

	strncpy(acPath, pdld->strObjectPath.c_str(), MAX_PATH-2);
	acPath[MAX_PATH-2] = 0;
	nLen = strlen(acPath);
	if(nLen < 1)
	{
		WriteLogN("[%s] invalid object path.", m_strUtilName.c_str());
		m_tBkMutex.UnLock();
		return -4;
	}

	if(acPath[nLen-1] != '/')
	{
		acPath[nLen] = '/';
		acPath[nLen+1] = 0;
	}

	strFullPath = SPrintf("%s%s", acPath, pdld->strObjectName.c_str());

	if(tFileUtil.FileExists(strFullPath.c_str()) == FALSE)
	{
		if(nRemainLog)
			WriteLogN("[%s] doc file backup not find file: [%d][%s]:[%s]", m_strUtilName.c_str(), errno, strFullPath.c_str(), pdld->strBkFileName.c_str());
		pdld->nRemoveTime = t_ValidTimeUtil->GetValidTime();
		m_tBkMutex.UnLock();
		return 0;
	}

	if (stat(strFullPath.c_str(), &fStat) != -1)
	{
		nMode = fStat.st_mode;

		if(!(nMode & S_IWUSR))
			nMode = nMode | S_IWUSR;
		if(!(nMode & S_IWGRP))
			nMode = nMode | S_IWGRP;
		if(!(nMode & S_IWOTH))
			nMode = nMode | S_IWOTH;
		if (chmod(strFullPath.c_str(), nMode) == -1)
		{
			WriteLogE("[%s] fail to chmod (%s) (%d)", m_strUtilName.c_str(), strFullPath.c_str(), errno);
		}
	}
	else
	{
		WriteLogE("[%s] fail to stat (%s) (%d)", m_strUtilName.c_str(), strFullPath.c_str(), errno);
	}

	if(nPoFaUsedMode == STATUS_USED_MODE_ON)
	{
		if(!pDpfb->nFileMaxSize || tFileUtil.GetFileSizeExt(strFullPath.c_str()) < (pDpfb->nFileMaxSize * ASI_GIGABYTE))
		{			
			if(pDpfb->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
			{
				pdld->strBkFileName = GetGUID();

				UINT32 nCompTime = GetTickCount();
				WriteLogN("[%s] doc file backup start: [%d][%s]:[%s]", m_strUtilName.c_str(), nRtn, strFullPath.c_str(), pdld->strBkFileName.c_str());

				strTarFile = SPrintf("%s/%s", strBkDir.c_str(), pdld->strBkFileName.c_str());

				strncpy(tACS.acSrcFile, strFullPath.c_str(), MAX_PATH-1);
				strncpy(tACS.acTarFile, strTarFile.c_str(), MAX_PATH-1);
				SetBkSeedKey(tACS.pSeedKey, 0);
				nRtn = t_ASICOMPDLLUtil->ASICOMP_S_CompFile(&tACS, sizeof(tACS));
				if(nRemainLog)
					WriteLogN("[%s] doc file backup end: [%d]: opt[%u]:[%s]:[%s]", m_strUtilName.c_str(), nRtn, GetTickCount() - nCompTime, strFullPath.c_str(), pdld->strBkFileName.c_str());

				if(tFileUtil.FileExists(strTarFile.c_str()) == FALSE)
				{
					WriteLogE("[%s] fail to backup %s (%d)", m_strUtilName.c_str(), strTarFile.c_str(), errno);
				}
				else
				{
					nCompTime = tFileUtil.GetFileSizeExt(strTarFile.c_str());
					WriteLogN("[%s] success to backup %s (sz:%u)", m_strUtilName.c_str(), strTarFile.c_str(), nCompTime);
				}

				pdld->nBackupType	= SS_PO_FA_BK_BACKUP_TYPE_COMP_SEED;
				pdld->nBackupTime = t_ValidTimeUtil->GetValidTime();
			}
			else
			{
				pdld->nRemoveTime = t_ValidTimeUtil->GetValidTime();
			}

			t_FileDeleteUtil->SecureDeleteFile(strFullPath.c_str(), nDelMethod,  nDelCnt, TRUE, TRUE, nLimitSize, nLimitDelCnt);

			if(tFileUtil.FileExists(strFullPath.c_str()) == TRUE)
			{
				WriteLogE("[%s] fail to delete %s", m_strUtilName.c_str(), strFullPath.c_str());
				if(t_LogicMgrPoFaDelFileAfterBoot)
					t_LogicMgrPoFaDelFileAfterBoot->InsertDelFileInfo(strFullPath.c_str());
			}
			else
			{
				WriteLogN("[%s] success to delete %s : (used:%d)", m_strUtilName.c_str(), strFullPath.c_str(), nPoFaUsedMode);
			}
		}
		else
		{
			strBkFileName = SPrintf("%s_%u", pdld->strObjectName.c_str(), GetTickCount());
			strBkFilePath = SPrintf("%s/%s", strBkDir.c_str(), strBkFileName.c_str());

			pdld->nRemoveTime = t_ValidTimeUtil->GetValidTime();

			t_FileDeleteUtil->SecureDeleteFile(strFullPath.c_str(), nDelMethod, nDelCnt, TRUE, true, nLimitSize, nLimitDelCnt);
			if(tFileUtil.FileExists(strFullPath.c_str()) == TRUE)
			{
				WriteLogE("[%s] fail to delete %s", m_strUtilName.c_str(), strFullPath.c_str());
				if(t_LogicMgrPoFaDelFileAfterBoot)
					t_LogicMgrPoFaDelFileAfterBoot->InsertDelFileInfo(strFullPath.c_str());
			}
			else
				WriteLogN("[%s] success to delete %s : (used:%d)", m_strUtilName.c_str(), strFullPath.c_str(), nPoFaUsedMode);
			t_ManageDocDeleteInfo->DocDeleteCount(pdld->nOpType);
		}
	}
	else if(nPoFaUsedMode == STATUS_USED_MODE_WARN)
	{
		pdld->nRemoveTime = t_ValidTimeUtil->GetValidTime();
		WriteLogD("[%s] success to delete %s : (used:%d)", m_strUtilName.c_str(), strFullPath.c_str(), nPoFaUsedMode);
	}
	m_tBkMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CDocBackupUtil::RecoveryFile(PDB_LOG_DOC pdld)
{
	INT32 nRtn = 0;
	CFileUtil tFileUtil;
	ASI_COMP_SECU tACS;
	String strNewFileName = "";
	String strSvFilePath;
	char acLogMsg[MAX_LOGMSG+1] = {0,};

	String strBkFile = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_BACKUP_DOC_FILE, pdld->strBkFileName.c_str());

	m_tBkMutex.Lock();
	if(pdld->nBackupType == SS_PO_FA_BK_BACKUP_TYPE_COMP_SEED)
	{
		//nRtn = t_ASICOMPDLLUtil->ASICOMP_DecompFileW(W2A(strBkFile), NULL, NULL);
		strNewFileName = tFileUtil.GetNewFileName(pdld->strObjectPath, pdld->strObjectName, 0, "recover_");
		strSvFilePath = pdld->strObjectPath;
							
		strncpy(tACS.acSrcFile, strBkFile.c_str(), MAX_PATH-1);
		strncpy(tACS.acTarPath, strSvFilePath.c_str(), MAX_PATH-1); 
		strncpy(tACS.acTarFile, strNewFileName.c_str(), MAX_PATH-1);
		
		SetBkSeedKey(tACS.pSeedKey, 1);

		nRtn = t_ASICOMPDLLUtil->ASICOMP_S_DecompFile(&tACS, sizeof(tACS), acLogMsg);
		if(nRtn == 0)
		{
			pdld->strObjectName = strNewFileName;
			pdld->strObjectPath = SPrintf("%s/%s", strSvFilePath.c_str(), strNewFileName.c_str());
			WriteLogN("[%s] success to recovery file : [%d][%d:%d][%s]->[%s]", m_strUtilName.c_str(), nRtn, pdld->nID, pdld->nRegSvrID, 
				pdld->strBkFileName.c_str(), pdld->strObjectPath.c_str());
		}
		else
		{
			WriteLogE("[%s] fail to recovery file : [%d][%d:%d][%s]->[%s/%s]", m_strUtilName.c_str(), nRtn, pdld->nID, pdld->nRegSvrID, 
				pdld->strBkFileName.c_str(), pdld->strObjectPath.c_str(), pdld->strObjectName.c_str());
		}
	}
	else if(pdld->nBackupType == SS_PO_FA_BK_BACKUP_TYPE_COMP)
	{
		WriteLogN("[%s] doc file recovery(decomp) start: [%d:%d][%s]", m_strUtilName.c_str(), pdld->nID, pdld->nRegSvrID, pdld->strBkFileName.c_str());
		nRtn = t_ASICOMPDLLUtil->ASICOMP_DecompFile(strBkFile.c_str(), NULL, NULL);
		if(nRtn == 0)
		{
			WriteLogN("[%s] success to recovery file : [%d][%d:%d][%s]->[%s/%s]", m_strUtilName.c_str(), nRtn, pdld->nID, pdld->nRegSvrID, 
				pdld->strBkFileName.c_str(), pdld->strObjectPath.c_str(), pdld->strObjectName.c_str());
		}
		else
		{
			WriteLogE("[%s] fail to recovery file (%s) (%d)", m_strUtilName.c_str(), strBkFile.c_str(), nRtn);
		}
	}
	else
	{	
		//t_FileDeleteUtil->SecureDeleteFileW((LPWSTR)(LPCWSTR)pdld->strObjectPathW);
		strNewFileName = SPrintf("%s/%s", pdld->strObjectPath.c_str(), pdld->strObjectName.c_str());
		if(unlink(strNewFileName.c_str()) != -1)
		{
			CopyFile(strBkFile.c_str(), strNewFileName.c_str(), TRUE);	
			WriteLogN("[%s] doc file recovery(copy) end: [%d][%d:%d][%s]->[%s]", m_strUtilName.c_str(), nRtn, pdld->nID, pdld->nRegSvrID,
				strBkFile.c_str(), strNewFileName.c_str());
		}
		else
		{
			WriteLogE("[%s] fail to delete (%s) (%d)", m_strUtilName.c_str(), strNewFileName.c_str(), errno);
		}
	}

	pdld->nBackupType	= 0;
	pdld->nBackupTime	= 0;
	pdld->strBkFileName.empty();
	if(unlink(strBkFile.c_str()) == -1)
	{
		WriteLogE("[%s] fail to delete backup file (%s) (%d)", m_strUtilName.c_str(), strBkFile.c_str(), errno);
	}

	m_tBkMutex.UnLock();

	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CDocBackupUtil::SetBkSeedKey(PBYTE pSeedKey, UINT32 nMode)
{
	INT32 nIdx = 0;
	BYTE pFixKey[16] = {0xF3, 0x42, 0xC2, 0x38, 0x47, 0x66, 0x28, 0xC3, 0xBA, 0x3E, 0xA3, 0x8B, 0xCE, 0x0F, 0x32, 0x91};
	String strDyKey;
	if(!nMode)	
		strDyKey = t_ManageHostKey->GetDecBkKey();
	else
		strDyKey = t_ManageHostKey->GetDecRcKey();

	if(strDyKey.length() < ASI_COMP_S_SEED_KEY_LEN)
		strDyKey = "1317DC76-1F1B-4333-A129-29779D951298";
	while(nIdx < ASI_COMP_S_SEED_KEY_LEN && nIdx < t_EnvInfo->m_strHostKey.length())
	{
		pSeedKey[nIdx] = strDyKey[nIdx] + pFixKey[nIdx];
		nIdx += 1;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CDocBackupUtil::RemoveBackupFilesByNonExistLog(UINT32& nContinue)
{
	CFileUtil tFileUtil;
	String strDirectory;
	String strFileName, strFilePath;
	UINT32 nDelFlag = 0;
	TListStr tListFiles;
	TListStrItor begin, end;
	PDB_PO_FA_BK pDpfb = (PDB_PO_FA_BK)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_BK);
	if(pDpfb == NULL)	
	{
		WriteLogA("fail to get backup file policy");
		return -1;
	}

	strDirectory = SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_BACKUP_DOC_FILE);
	if (!strDirectory.empty() && strDirectory.length()>3)
	{
		tFileUtil.FindFileList(tListFiles, strDirectory, FALSE);
		if(!tListFiles.size())
			return 0;

		WriteLogN("start remove remain doc backup file by non exist log");

		begin = tListFiles.begin();  end = tListFiles.end();
		for(begin; begin != end && !nContinue; begin++)
		{
			strFilePath = *begin;
			if (strFilePath.empty())
				continue;

			strFileName = tFileUtil.FindFileName(strFilePath);
			if(pDpfb->tDPH.nUsedMode == STATUS_USED_MODE_OFF || !pDpfb->nKeepDay)
			{
				nDelFlag = 1;
			}
			else if(tFileUtil.IsExistFileDateTime(FILE_CHK_TYPE_MODIFY_DT, pDpfb->nKeepDay * TIMER_INTERVAL_TIME_DAY, strFilePath, "") 
				&& !t_ManageLogDoc->IsExistLogDocByBkName(strFileName, 1, 1))
			{
				nDelFlag = 2;
			}

			if(nDelFlag)
			{
				t_FileDeleteUtil->SecureDeleteFile(strFilePath.c_str(), 0, 0);
				if(tFileUtil.FileExists(strFilePath.c_str()) == TRUE)
				{
					if (unlink(strFilePath.c_str()) == -1)
					{
						WriteLogN("remove remain bk file delete fail and set del file reservation delay until reboot : [%d]:[%s]", errno, strFilePath.c_str());
						if(t_LogicMgrPoFaDelFileAfterBoot)
							t_LogicMgrPoFaDelFileAfterBoot->InsertDelFileInfo(strFilePath.c_str());
					}
				}
				WriteLogN("remove remain doc backup file : delflag[%d] bkpath[%s]", nDelFlag, strFilePath.c_str());
			}
		}

		WriteLogN("end remove remain doc backup file by non exist log");
	}
	return 0;
}
//---------------------------------------------------------------------------

