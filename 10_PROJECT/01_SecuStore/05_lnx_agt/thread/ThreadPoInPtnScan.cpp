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

// ThreadPoInPtnScan.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadPoInPtnScan.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInPtnScan

CThreadPoInPtnScan*	t_ThreadPoInPtnScan = NULL;

CThreadPoInPtnScan::CThreadPoInPtnScan()
{
	m_nOrderID = 10000;
	m_nWorkID = 0;
}

CThreadPoInPtnScan::~CThreadPoInPtnScan()
{
}

BOOL CThreadPoInPtnScan::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoInPtnScan::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInPtnScan message handlers



INT32		CThreadPoInPtnScan::AddWork(PDB_PO_IN_PTN_OP_UNIT punit)
{
	m_tWorkMutex.Lock();		
	m_tWorkInfoMap[GetNextOrderID()] = *punit;
	m_tWorkMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoInPtnScan::DelWork(UINT32 nID)
{
	m_tWorkMutex.Lock();	
	m_tWorkInfoMap.erase(nID);
	m_tWorkMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

UINT32			CThreadPoInPtnScan::GetNextWork()
{
	UINT32 nRtn = 0;
	m_tWorkMutex.Lock();
	if(m_tWorkInfoMap.size())
	{
		nRtn = m_tWorkInfoMap.begin()->first;
	}
	m_tWorkMutex.UnLock();
	return nRtn;
}
//-------------------------------------------------------------------------------

PDB_PO_IN_PTN_OP_UNIT	CThreadPoInPtnScan::GetCurWork()
{
	PDB_PO_IN_PTN_OP_UNIT pdata = NULL;
	m_tWorkMutex.Lock();	
	TMapDBPoInPtnOpUnitItor find = m_tWorkInfoMap.find(m_nWorkID);
	if(find != m_tWorkInfoMap.end())
		pdata = &(find->second);
	m_tWorkMutex.UnLock();

	return pdata;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

int CThreadPoInPtnScan::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nRunFlag = 1;

	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		m_nWorkID = GetNextWork();
		if(m_nWorkID)
		{	
			do 
			{
				if(StartScan())
				{
					WriteLogE("init environment fail : [%s][oid:%u]", m_strThreadName.c_str(), m_nWorkID);
					break;
				}

				if(m_tWEngWLExMap.size() || m_tSiteFileAliasList.size())
				{
					ProcessScanResult();
				}
			} while (FALSE);						

			StopScan();
			DelWork(m_nWorkID);
			WriteLogN("stop scan : [%s][oid:%u]", m_strThreadName.c_str(), m_nWorkID);
		}
		Sleep(10);
	}

	if(!t_EnvInfoOp)							WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())	WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())						WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoInPtnScan::StartScan()
{
	{
		CHAR szDLLName[CHAR_MAX_SIZE] = {0, };
		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_ff.so", t_EnvInfo->m_strDLLPath.c_str());
		szDLLName[CHAR_MAX_SIZE-1] = 0;
		if(m_tFFDLLUtil.LoadLibraryExt(szDLLName))
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, errno);
			return -15;
		}
		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_weng.so", t_EnvInfo->m_strDLLPath.c_str());
		szDLLName[CHAR_MAX_SIZE-1] = 0;

		if(m_tWEDLLUtil.LoadLibraryExt(szDLLName))
		{
			WriteLogE("fail to load library  : [%s][%d]", szDLLName, errno);
			return -16;
		}

		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_fi.so", t_EnvInfo->m_strDLLPath.c_str());
		szDLLName[CHAR_MAX_SIZE-1] = 0;
		if(m_tFIDLLUtil.LoadLibraryExt(szDLLName))
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, errno);
			return -17;
		}
	}

	{
		ASI_FF_INIT_INFO tAFII;
		t_EnvInfoOp->GetFFPtnInitData(tAFII);
		m_tFFDLLUtil.ASIFF_Init(&tAFII);

		m_tWEDLLUtil.Init();
		m_tWEDLLUtil.InitDB();
	}

	m_tWEngWLExMap.clear();
	m_tSiteFileAliasList.clear();

	PDB_PO_IN_PTN_OP_UNIT pdata = GetCurWork();
	if(!pdata)
		return -18;

	WriteLogN("[%s] start scan path : [%s][%s]:[%d]", m_strThreadName.c_str(), pdata->tDPH.strName.c_str(), pdata->strScanPath.c_str(), pdata->nFileGatherType);

	CFileUtil tFileUtil;
	CHAR chTok = GetToken(pdata->strScanPath, ';', ',');
	CTokenString Token(pdata->strScanPath.c_str(), pdata->strScanPath.length(), chTok);
	while(Token.IsNextToken())
	{
		String strPath = Token.NextToken();

		strPath = tFileUtil.GetEnvPathToLocalPathSingle(strPath);

		m_tFFDLLUtil.ASIFF_AddSearchDirPath(m_nWorkID, strPath.c_str());
		WriteLogN("[%s] find file path : [%s]", m_strThreadName.c_str(), strPath.c_str());
	}

	m_tFFDLLUtil.ASIFF_SearchDirFileThread(m_nWorkID);
	UINT32 nSearchEnd = 0;

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue() && !nSearchEnd)
	{
		switch(ChkScanFile())
		{
		case -1:
			{
				m_tFFDLLUtil.ASIFF_ClearFileMask(m_nWorkID);
				m_tFFDLLUtil.ASIFF_ClearFileDateTime(m_nWorkID);
				m_tFFDLLUtil.ASIFF_ClearExceptPath(m_nWorkID);
				m_tFFDLLUtil.ASIFF_ClearSearchDirPath(m_nWorkID);
				m_tFFDLLUtil.ASIFF_ClearFileFindOption(m_nWorkID);
				WriteLogN("[%s] find file stop and clear operation : [%d]", m_strThreadName.c_str(), m_nWorkID);
				nSearchEnd = 1;
				break;
			}
		case 1:
			Sleep(1000);
			break;
		}
	}		

	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoInPtnScan::StopScan()
{
	m_tFFDLLUtil.FreeLibraryExt();
	m_tWEDLLUtil.FreeLibraryExt();
	m_tFIDLLUtil.FreeLibraryExt();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoInPtnScan::ChkScanFile()
{
	ASI_FF_FILE_ITEM tAFFI[1];
	ASI_FF_FILE_RESULT tAFFR;
	String strObjectName;
	UINT32 nElfType = 0;
	UINT32 nAFFINum = 1;
	ASI_WENG_WL_EX tAWWE;
	DWORD dwFileType = 0;
	memset(tAFFI, 0, sizeof(ASI_FF_FILE_ITEM) * nAFFINum);
	m_tFFDLLUtil.ASIFF_GetFindFileItem(m_nWorkID, tAFFI, &nAFFINum, &tAFFR);

	if(tAFFR.nContinue == 0 && tAFFR.nMoreFileItem == 0)  
	{
		WriteLogN("[%s] find file work end : [%d][%d:%d]", m_strThreadName.c_str(), m_nWorkID, tAFFR.nFileWorkedNum, tAFFR.nFileTotalNum);
		return -1;
	}

	if(!nAFFINum)
	{
		return 1; 
	}

	UINT32 nIdx = 0;
	while(nIdx < nAFFINum && GetContinue())
	{
		strObjectName = SPrintf("%s/%s", tAFFI[nIdx].szFilePath, tAFFI[nIdx].szFileName);

		nElfType = m_tFIDLLUtil.ASIFI_GetFileElfType((char *)strObjectName.c_str(), tAFFI[nIdx].szFileName);
		if(nElfType == ASI_FI_ELF_TYPE_EXE || nElfType == ASI_FI_ELF_TYPE_SCRIPTS)
		{
			memset(&tAWWE, 0, sizeof(ASI_WENG_WL_EX));
			if(m_tWEDLLUtil.GetWL(strObjectName.c_str(), (PVOID)&tAWWE, sizeof(tAWWE), &dwFileType) == ERROR_SUCCESS && dwFileType != AS_INVALID_FILE)
			{
				String strFeKey = tAWWE.acWhiteHash;
				if(m_tWEngWLExMap.find(strFeKey) == m_tWEngWLExMap.end())
				{
					strncpy(tAWWE.acFilePath, strObjectName.c_str(), MAX_PATH-1);
					tAWWE.acFilePath[MAX_PATH-1] = 0;
					m_tWEngWLExMap[strFeKey] = tAWWE;
					if((m_tWEngWLExMap.size() % 1000) == 0)
					{
						WriteLogN("[%s] add in ptn op local item : [%d]", m_strThreadName.c_str(), m_tWEngWLExMap.size());
					}	
				}
				else
				{
					CTokenString Token(strObjectName.c_str(), strObjectName.length(), '/', 1);

					DB_SITE_FILE_ALIAS tDSFA;
					tDSFA.strFeKey = strFeKey;
					tDSFA.strFileName = Token.NextToken();
					tDSFA.strFilePath = Token.RemainValue();

					m_tSiteFileAliasList.push_back(tDSFA);
					if((m_tSiteFileAliasList.size() % 1000) == 0)
					{
						WriteLogN("[%s] add in ptn op local item alias : [%d]", m_strThreadName.c_str(), m_tSiteFileAliasList.size());
					}
				}
			}
		}	

		nIdx += 1;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoInPtnScan::ProcessScanResult()
{
	CMainDlg* pMainDlg = (CMainDlg*)t_EnvInfoOp->GetMainHandle();	
	PDB_PO_IN_PTN_OP_UNIT pdata = GetCurWork(); 
	if(!pdata)		return 0;

	{
		TMapWEngWLExItor begin, end;
		begin = m_tWEngWLExMap.begin();	end = m_tWEngWLExMap.end();
		for(begin; begin != end; begin++)
		{
			if(pdata->nFileGatherType & SS_PO_IN_PTN_OP_NEW_FILE_SEND_TYPE_INFO)
			{
//				String strFeKey = BinToHexLen((PBYTE)(begin->second.acWhiteHash), SHA512_BLOCK_SIZE);
				String strFeKey = begin->second.acWhiteHash;

				PDB_SITE_FILE pdsf = t_ManageSiteFile->FindPosSKeyID_Item(strFeKey, SS_SITE_FILE_KEY_TYPE_FE);
				if(pdsf && (pdsf->nSyncSvrStep & SS_PO_IN_PTN_OP_NEW_FILE_SEND_TYPE_INFO))		continue;

				CTokenString Token(begin->second.acFilePath, strlen_ext(begin->second.acFilePath), '/', 1);

				DB_SITE_FILE tDSF;
				tDSF.strFeKey = strFeKey;				
				tDSF.strFileName = Token.NextToken();
				tDSF.strFilePath = Token.RemainValue();

				//DWORD_PTR dwError = -1;
				//SendMessageTimeout(t_EnvInfoOp->GetMainHandle(), WM_GLOBAL_THREAD_LOG_EVT, (WPARAM)&tDSF, (LPARAM)ASI_EPS_APP_LOG_TYPE_SITE_FILE, SMTO_NORMAL, SENDMESSAGE_DEFAULT_WAITING, &dwError);
				if(pMainDlg != NULL)
					pMainDlg->OnThreadLog(ASI_EPS_APP_LOG_TYPE_SITE_FILE, (PVOID)&tDSF);
			}
		}
	}

	{
		TListDBSiteFileAliasItor begin, end;
		begin = m_tSiteFileAliasList.begin();	end = m_tSiteFileAliasList.end();
		for(begin; begin != end; begin++)
		{
			if(pdata->nFileGatherType & SS_PO_IN_PTN_OP_NEW_FILE_SEND_TYPE_INFO)
			{
				PDB_SITE_FILE_ALIAS pdsf = t_ManageSiteFileAlias->FindSiteFileAliasKey((*begin));
				if(pdsf && (pdsf->nSyncSvrStep & SS_PO_IN_PTN_OP_NEW_FILE_SEND_TYPE_INFO))		continue;

				if(pMainDlg != NULL)
					pMainDlg->OnThreadLog(ASI_EPS_APP_LOG_TYPE_SITE_FILE_ALIAS, (PVOID)&(*begin));
			}
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
