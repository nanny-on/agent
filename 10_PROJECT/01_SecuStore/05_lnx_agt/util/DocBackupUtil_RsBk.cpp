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
#include "DocBackupUtil.h"

//---------------------------------------------------------------------------

INT32		CDocBackupUtil::BackupFile(PDB_LOG_RS_BK pdata)
{
	CFileUtil tFileUtil;
	CProcUtil tProcUtil;

	String strBkDir = "";
	INT32 nRtn = 0;
	mode_t nMode = 0;
	struct stat fStat;
	INT32 nRemainLog = 1;

	if(pdata == NULL)
	{
		WriteLogE("[%s] invalid input data", m_strUtilName.c_str());
		return 1;
	}

	{
		if(pdata->strObjectName.find("%") != string::npos || pdata->strObjectPath.find("%") != string::npos || 
			pdata->strSubjectName.find("%") != string::npos || pdata->strSubjectPath.find("%") != string::npos)
		{
			nRemainLog = 0;
		}
	}

	m_tBkMutex.Lock();

	PDB_PO_IN_RS_BK pdata_bk = (PDB_PO_IN_RS_BK)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_RS_BK);
	if(!pdata_bk)	
	{
		WriteLogN("[%s] rs bk file backup pdata_bk is null.", m_strUtilName.c_str());
		m_tBkMutex.UnLock();
		return 11;
	}

	{
		strBkDir = SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_BACKUP_RS_BK);
		if(tFileUtil.DirectoryExists(strBkDir.c_str()) == FALSE)	
		{				
			tFileUtil.ForceDirectory(strBkDir.c_str());
		}
	}

	if(tFileUtil.FileExists(pdata->strObjectPath.c_str()) == FALSE)
	{
		if(nRemainLog)	WriteLogN("[%s] rs bk file backup not find file: [%d][%s/%s]:[%s]", m_strUtilName.c_str(), errno, pdata->strObjectPath.c_str(), pdata->strObjectName.c_str(), pdata->strBkFileName.c_str());
		m_tBkMutex.UnLock();
		return 0;
	}


	if (stat(pdata->strObjectPath.c_str(), &fStat) != -1)
	{
		nMode = fStat.st_mode;

		if(!(nMode & S_IWUSR))
			nMode = nMode | S_IWUSR;
		if(!(nMode & S_IWGRP))
			nMode = nMode | S_IWGRP;
		if(!(nMode & S_IWOTH))
			nMode = nMode | S_IWOTH;
		if (chmod(pdata->strObjectPath.c_str(), nMode) == -1)
		{
			WriteLogE("[%s] fail to chmod (%s) (%d)", m_strUtilName.c_str(), pdata->strObjectPath.c_str(), errno);
		}
	}
	else
	{
		WriteLogE("[%s] fail to stat (%s) (%d)", m_strUtilName.c_str(), pdata->strObjectPath.c_str(), errno);
	}

	if(!pdata_bk->nFileMaxSize || tFileUtil.GetFileSizeExt(pdata->strObjectPath.c_str()) < (pdata_bk->nFileMaxSize * ASI_GIGABYTE))
	{			
		if(pdata_bk->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(pdata->strBkFileName.empty())
				pdata->strBkFileName = GetGUID();

			UINT32 nCompTime = GetTickCount();
			WriteLogN("[%s] rs bk file backup start: [%d][%s/%s]:[%s]", m_strUtilName.c_str(), nRtn, pdata->strObjectPath.c_str(), pdata->strObjectName.c_str(), pdata->strBkFileName.c_str());

			//nRtn = t_ASICOMPDLLUtil->ASICOMP_CompFileW(pdata->strObjectPathW, W2A(strBkDir), pdata->strBkFileName);
			String strTarFile;
			{
				LPCTSTR lpBkFileName = pdata->strBkFileName.c_str();					
				String strShortPath = LPCTSTR(lpBkFileName + (pdata->strBkFileName.length() - 2));
					
				strTarFile = SPrintf("%s/%s", strBkDir.c_str(), strShortPath.c_str());
				if(tFileUtil.DirectoryExists(strTarFile.c_str()) == FALSE)
					tFileUtil.ForceDirectory(strTarFile.c_str());

				strTarFile = SPrintf("%s/%s/%s", strBkDir.c_str(), strShortPath.c_str(), pdata->strBkFileName.c_str());
				unlink(strTarFile.c_str());

				ASI_COMP_SECU tACS;
				strncpy(tACS.acSrcFile, pdata->strObjectPath.c_str(), MAX_PATH-1);
				strncpy(tACS.acTarFile, strTarFile.c_str(), MAX_PATH-1);
				SetBkSeedKey(tACS.pSeedKey, 0);

				nRtn = t_ASICOMPDLLUtil->ASICOMP_S_CompFile(&tACS, sizeof(tACS));
			}
			if(nRemainLog)	WriteLogN("[%s] rs bk file backup end: [%d]: opt[%u]:[%s/%s]:[%s]", m_strUtilName.c_str(), nRtn, GetTickCount() - nCompTime, pdata->strObjectPath.c_str(), pdata->strObjectName.c_str(), pdata->strBkFileName.c_str());

			pdata->nBackupSize	= tFileUtil.GetFileSizeExt(strTarFile.c_str());
			pdata->nBackupType	= SS_PO_IN_RS_BK_BACKUP_TYPE_COMP_SEED;
			pdata->nBackupTime = t_ValidTimeUtil->GetValidTime();
		}
	}

	m_tBkMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CDocBackupUtil::RecoveryFile(PDB_LOG_RS_BK pdata)
{
	String strBkFile = "";
	INT32 nRtn = 0;
	CFileUtil tFileUtil;

	if(pdata == NULL)
	{
		WriteLogE("[%s] invalid input data", m_strUtilName.c_str());
		return 1;
	}

	{
		LPCTSTR lpBkFileName = pdata->strBkFileName.c_str();

		if(strlen(lpBkFileName) < 2)
		{
			WriteLogE("[%s] rs bk file recovery empty backup file name : [%d][%s/%s]:[%s]", m_strUtilName.c_str(), errno, pdata->strObjectPath.c_str(), pdata->strObjectName.c_str(), pdata->strBkFileName.c_str());
			return 2;
		}

		String strShortPath = LPCTSTR(lpBkFileName + (strlen(lpBkFileName) - 2));			
		strBkFile = SPrintf("%s/%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_BACKUP_RS_BK, strShortPath.c_str(), pdata->strBkFileName.c_str());
	}

	m_tBkMutex.Lock();

	if(pdata->nBackupType == SS_PO_IN_RS_BK_BACKUP_TYPE_COMP_SEED)
	{
		{
			WriteLogN("[%s] rs bk file recovery(decomp) start: [%d:%d][%s]", m_strUtilName.c_str(), pdata->nID, pdata->nRegSvrID, pdata->strBkFileName.c_str());
			//nRtn = t_ASICOMPDLLUtil->ASICOMP_DecompFileW(W2A(strBkFile), NULL, NULL);
			String strNewFileName = "";				
			String strSvFilePath;
			{
				strNewFileName = tFileUtil.GetNewFileName(pdata->strObjectPath, pdata->strObjectName, 0, "recover_");
				strSvFilePath = pdata->strObjectPath;
										
				ASI_COMP_SECU tACS;
				strncpy(tACS.acSrcFile, strBkFile.c_str(), MAX_PATH-1);
				strncpy(tACS.acTarPath, strSvFilePath.c_str(), MAX_PATH-1);
				strncpy(tACS.acTarFile, strNewFileName.c_str(), MAX_PATH-1);
					
				SetBkSeedKey(tACS.pSeedKey, 1);

				nRtn = t_ASICOMPDLLUtil->ASICOMP_S_DecompFile(&tACS, sizeof(tACS));
			}				

			WriteLogN("[%s] rs bk file recovery(decomp) end: [%d][%d:%d][%s]->[%s/%s]", m_strUtilName.c_str(), nRtn, pdata->nID, pdata->nRegSvrID, pdata->strBkFileName.c_str(), 
																			strSvFilePath.c_str(), strNewFileName.c_str());
		}
	}

	m_tBkMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CDocBackupUtil::RemoveBackupFile(PDB_LOG_RS_BK pdata)
{
	CFileUtil tFileUtil;
	String strBkFileName = "";
	INT32 nRtn = 0;
	LPCTSTR lpBkFileName = NULL;

	if(pdata == NULL)
	{
		WriteLogE("[%s] invalid input data", m_strUtilName.c_str());
		return 1;
	}

	lpBkFileName = pdata->strBkFileName.c_str();
	String strShortPath = LPCTSTR(lpBkFileName + (pdata->strBkFileName.length() - 2));

	strBkFileName = SPrintf("%s/%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_BACKUP_RS_BK, strShortPath.c_str(), pdata->strBkFileName.c_str());

	m_tBkMutex.Lock();

	if(tFileUtil.FileExists(strBkFileName.c_str()) == FALSE)
	{
		WriteLogN("[%s] remove rs bk file backup not find file: [%d][%s/%s]:[%s]", m_strUtilName.c_str(), errno, pdata->strObjectPath.c_str(), pdata->strObjectName.c_str(), pdata->strBkFileName.c_str());
		m_tBkMutex.UnLock();
		return 0;
	}

	{
		if (unlink(strBkFileName.c_str()) == -1)
		{
			WriteLogN("[%s] remove rs bk file delete fail : [%d]:[%s/%s]", m_strUtilName.c_str(), GetLastError(), pdata->strObjectPath.c_str(), pdata->strObjectName.c_str());
			if(t_LogicMgrPoFaDelFileAfterBoot)
				t_LogicMgrPoFaDelFileAfterBoot->InsertDelFileInfo(strBkFileName.c_str());
		}
		WriteLogN("[%s] remove rs bk backup file: [%s/%s][%s]", m_strUtilName.c_str(), pdata->strObjectPath.c_str(), pdata->strObjectName.c_str(), pdata->strBkFileName.c_str());
	}
	m_tBkMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------
