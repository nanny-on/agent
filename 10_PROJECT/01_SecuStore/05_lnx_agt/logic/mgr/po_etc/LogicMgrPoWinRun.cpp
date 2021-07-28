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

//---------------------------------------------------------------------------


#include "stdafx.h"
#include "com_struct.h"
#include "LogicMgrPoWinRun.h"

//---------------------------------------------------------------------------

CLogicMgrPoWinRun*		t_LogicMgrPoWinRun = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoWinRun::CLogicMgrPoWinRun()
{
	t_ManagePoWinRun	= new CManagePoWinRun();
	t_ManagePoWinRun->LoadDBMS();
	t_ManagePoWinRun->InitHash();

	m_strLogicName		= "mgr po win run";
	
	m_nPolicyType		= SS_POLICY_TYPE_WIN_RUN;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_WIN_RUN;
}
//---------------------------------------------------------------------------

CLogicMgrPoWinRun::~CLogicMgrPoWinRun()
{
	SAFE_DELETE(t_ManagePoWinRun);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
INT32		CLogicMgrPoWinRun::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoWinRun::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_WIN_RUN pdpwr = NULL;
	DB_PO_WIN_RUN dpwr;
	m_tDPH = &(dpwr.tDPH);

	if( t_ManagePoWinRun->GetPkt(RecvToken, dpwr))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	pdpwr = (PDB_PO_WIN_RUN)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdpwr)
	{
		t_ManagePoWinRun->DelPoWinRun(pdpwr->tDPH.nID);
	}

	{
		if(SetER(t_ManagePoWinRun->ApplyPoWinRun(dpwr)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoWinRun::ApplyPolicy()
{
	return 0;
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoWinRun::DumpMBR()
{/*

	PDB_PO_WIN_RUN pdpwr = (PDB_PO_WIN_RUN)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if (!pdpwr)
	{
		WriteLogE("[%s] not find current policy.", m_strLogicName.c_str());
		return -1;
	}
	
	if (pdpwr->nMbrUsedMode == STATUS_USED_MODE_OFF)
	{
		WriteLogN("[%s] current policy used mode off.", m_strLogicName.c_str());
		return -2;
	}

	CHAR szPath[CHAR_MAX_SIZE] = { NULL, };
	sprintf_ext(CHAR_MAX_SIZE, szPath, "%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_BACKUP_MBR_FILE);

	CFileUtil tFileUtil;
	tFileUtil.ForceDirectory(szPath);

	SYSTEMTIME tCurrent;
	GetLocalTime(&tCurrent);

	CHAR szFile[CHAR_MAX_SIZE] = { NULL, };
	snprintf(szFile, CHAR_MAX_SIZE-1, "%s/%04d%02d%02d_%02d%02d%02d_%03d.md", szPath, tCurrent.wYear, tCurrent.wMonth, tCurrent.wDay, tCurrent.wHour, tCurrent.wMinute, tCurrent.wSecond, tCurrent.wMilliseconds);

	CHAR szError[BUFSIZ] = { NULL };
 	if(t_ASIMbrDLLUtil->ASIMBR_DumpMbrData(szFile, MODE_ACTIVE_DATA, szError) == FALSE)
 	{
 		WriteLogE("fail to dump mbr data : %s.", szError);
 		return -4;
 	}

	CHAR szHash[CHAR_MAX_SIZE] = { NULL, };
	if (SetER(SHAFile(ASIHASH_SHA_LEN_TYPE_256, szFile, szHash, CHAR_MAX_SIZE)))
	{
		WriteLogE("get md dump file hash fail : [%s]", szFile);
		return -5;
	}

	CHAR szCRC[CHAR_MAX_SIZE] = { NULL, };
	sprintf_ext(CHAR_MAX_SIZE, szCRC, "%s/%s/%s", t_EnvInfo->m_strRootPath, STR_BACKUP_MBR_FILE, STR_BACKUP_CRC_FILE);
	WritePrivateProfileString(PathFindFileName(szFile), "crc", szHash, szCRC);
*/
	return 0;
}

//---------------------------------------------------------------------------
INT32		CLogicMgrPoWinRun::RestoreMBR()
{
/*
	PDB_PO_WIN_RUN pdpwr = (PDB_PO_WIN_RUN)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if (!pdpwr)
	{
		WriteLogE("[%s] not find current policy.", m_strLogicName.c_str());
		return -1;
	}

	if (pdpwr->nMbrUsedMode == STATUS_USED_MODE_OFF)
	{
		WriteLogN("[%s] current policy used mode off.", m_strLogicName.c_str());
		return -2;
	}

	CHAR szPath[CHAR_MAX_SIZE]	= { NULL, };
	CHAR szError[CHAR_MAX_SIZE]	= { NULL, };
	CHAR szCRC[CHAR_MAX_SIZE]		= { NULL, };
	CHAR szHash[CHAR_MAX_SIZE]	= { NULL, };
	CHAR szHashed[CHAR_MAX_SIZE]	= { NULL, };
	sprintf_ext(CHAR_MAX_SIZE, szCRC, "%s/%s/%s", t_EnvInfo->m_strRootPath, STR_BACKUP_MBR_FILE, STR_BACKUP_CRC_FILE);

	DWORD dwFalsify = 1;
	BOOL bFind = FALSE;
	while(t_ASIMbrDLLUtil->ASIMBR_GetLastDumpPath(szPath, CHAR_MAX_SIZE-sizeof(CHAR), szError))
	{
		dwFalsify = 1;
 		if (!t_ASIMbrDLLUtil->ASIMBR_CompareMbrData(szPath, MODE_ACTIVE_DATA, &dwFalsify, szError))
 		{
 			DeleteFile(szPath);
 			WriteLogE("[%s] md compare fail.(%s)", m_strLogicName.c_str(), szError);
 			continue;
 		}

		if (dwFalsify == TRUE)
		{			
			WriteLogN("[%s] md is not falsification.", m_strLogicName.c_str());
			return 0;
		}

		if (szPath[0] != NULL && PathFileExists(szPath))
		{
			if (SetER(SHAFile(ASIHASH_SHA_LEN_TYPE_256, szPath, szHash, CHAR_MAX_SIZE)))
			{
				DeleteFile(szPath);
				WriteLogE("[%s] md compare fail.(%s)(%s)", m_strLogicName.c_str(), szPath, szError);
				continue;
			}

			GetPrivateProfileString(PathFindFileName(szPath), _T("crc"), NULL,szHashed,  CHAR_MAX_SIZE-sizeof(CHAR), szCRC);
			if (_tcsicmp(szHashed, szHash) == 0)
			{
				bFind = TRUE;
				break;
			}
			else
			{
				DeleteFile(szPath);
				WriteLogE("[%s] md different hashed.(%s)", m_strLogicName.c_str(), szPath);
				continue;
			}
		}
		else
		{
			DeleteFile(szPath);
			WriteLogE("[%s] md file not found.(%s)", m_strLogicName.c_str(), szPath);
		}
	}

	if (!bFind)
	{
		WriteLogE("[%s] not found adjust md file.", m_strLogicName.c_str());
		return -3;
	}


 	BOOL bRet = t_ASIMbrDLLUtil->ASIMBR_RestoreMbrData(szPath, MODE_ACTIVE_DATA, szError);
 	if (bRet == FALSE)
 	{
 		WriteLogE("[%s] md restore fail.(%s)(%s)", m_strLogicName.c_str(), szPath, szError);
 		return -4;
 	}

	WriteLogN("[%s] md restored success.(%s)", m_strLogicName.c_str(), szPath);
*/	
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoWinRun::OnTimer_Logic()
{
	return 0;	
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
