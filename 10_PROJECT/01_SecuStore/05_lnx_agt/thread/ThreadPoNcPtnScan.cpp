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
 * License along with this program. If not, see <https://www.gnu.org/licenses/>.   
 *  
 * 
 */

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadPoNcPtnScan.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoNcPtnScan

CThreadPoNcPtnScan*	t_ThreadPoNcPtnScan = NULL;

CThreadPoNcPtnScan::CThreadPoNcPtnScan()
{
	m_nOrderID = 10000;
	m_nWorkID = 0;
}

CThreadPoNcPtnScan::~CThreadPoNcPtnScan()
{
}

BOOL CThreadPoNcPtnScan::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoNcPtnScan::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoNcPtnScan message handlers



INT32		CThreadPoNcPtnScan::AddWork(PDB_PO_NC_PTN_OP_UNIT punit)
{
	m_tWorkMutex.Lock();		
	m_tWorkInfoMap[GetNextOrderID()] = *punit;
	m_tWorkMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoNcPtnScan::DelWork(UINT32 nID)
{
	m_tWorkMutex.Lock();	
	m_tWorkInfoMap.erase(nID);
	m_tWorkMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

UINT32			CThreadPoNcPtnScan::GetNextWork()
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

PDB_PO_NC_PTN_OP_UNIT	CThreadPoNcPtnScan::GetCurWork()
{
	PDB_PO_NC_PTN_OP_UNIT pdata = NULL;
	m_tWorkMutex.Lock();	
	TMapDBPoNcPtnOpUnitItor find = m_tWorkInfoMap.find(m_nWorkID);
	if(find != m_tWorkInfoMap.end())
		pdata = &(find->second);
	m_tWorkMutex.UnLock();

	return pdata;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

int CThreadPoNcPtnScan::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nRunFlag = 1;

	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
/*
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
*/
		Sleep(10);
	}

	if(!t_EnvInfoOp)							WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())	WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())						WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	return CThreadBase::Run();
}
//---------------------------------------------------------------------------

INT32		CThreadPoNcPtnScan::StartScan()
{
/*
	{
		CHAR szDLLName[CHAR_MAX_SIZE] = {0, };

		if(m_tFFDLLUtil.LoadLibraryExt(FormatString(szDLLName, "%s/asi_ff.so", t_EnvInfo->m_strDLLPath)))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
			return -15;
		}

		if(m_tWEDLLUtil.LoadLibraryExt(FormatString(szDLLName, "%s/asi_weng.so", t_EnvInfo->m_strDLLPath)))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
			return -16;
		}

		if(m_tFIDLLUtil.LoadLibraryExt(FormatString(szDLLName, "%s/asi_fi.so", t_EnvInfo->m_strDLLPath)))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
			return -16;
		}

		if(m_tHashDLLUtil.LoadLibraryExt(FormatString(szDLLName, "%s/asi_hash.so", t_EnvInfo->m_strDLLPath)))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
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
	{
		PDB_PO_NC_PTN_OP_UNIT pdata = GetCurWork();
		if(!pdata)		return -18;
		
		WriteLogN("[%s] start scan path : [%s][%s]:[%d]", m_strThreadName.c_str(), pdata->tDPH.strName, pdata->strScanPath, pdata->nFileGatherType);

		{
			CFileUtil tFileUtil;
			CHAR chTok = GetToken(pdata->strScanPath, ';', ',');
			CTokenString Token(pdata->strScanPath, pdata->strScanPath.GetLength(), chTok);
			while(Token.IsNextToken())
			{
				String strPath = Token.NextToken();

				strPath = tFileUtil.GetEnvPathToLocalPathSingle(strPath);

				m_tFFDLLUtil.ASIFF_AddSearchDirPath(m_nWorkID, strPath);
				WriteLogN("[%s] find file path : [%s]", m_strThreadName.c_str(), strPath);
			}
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
				case 1:		Sleep(1000);		break;
			}
		}		
	}
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoNcPtnScan::StopScan()
{
/*
	m_tFFDLLUtil.FreeLibraryExt();
	m_tWEDLLUtil.FreeLibraryExt();
	m_tFIDLLUtil.FreeLibraryExt();
	m_tHashDLLUtil.FreeLibraryExt();
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoNcPtnScan::ChkScanFile()
{
/*
	ASI_FF_FILE_ITEM tAFFI[1];
	ASI_FF_FILE_RESULT tAFFR;

	UINT32 nAFFINum = 1;
	memset(tAFFI, 0, sizeof(ASI_FF_FILE_ITEM) * nAFFINum);
	m_tFFDLLUtil.ASIFF_GetFindFileItem(m_nWorkID, tAFFI, &nAFFINum, &tAFFR);

	if(tAFFR.nContinue == 0 && tAFFR.nMoreFileItem == 0)  
	{
		WriteLogN("[%s] find file work end : [%d][%d:%d]", m_strThreadName.c_str(), m_nWorkID, tAFFR.nFileWorkedNum, tAFFR.nFileTotalNum);
		return -1;
	}

	if(!nAFFINum)	return 1; 

	UINT32 nIdx = 0;
	while(nIdx < nAFFINum && GetContinue())
	{
		String strObjectName;
		strObjectName = SPrintf("%s%s", tAFFI[nIdx].szFilePath, tAFFI[nIdx].szFileName);

 		UINT32 nPeType = m_tFIDLLUtil.ASIFI_GetFilePEType(strObjectName);
		if(nPeType == ASI_FI_PE_TYPE_EXE)
		{
			ASI_WENG_WL_EX tAWWE;
			DWORD dwFileType = 0;
			if(m_tWEDLLUtil.GetWL(strObjectName, &tAWWE, sizeof(tAWWE), &dwFileType) == ERROR_SUCCESS &&
				dwFileType != AS_INVALID_FILE)
			{
				String strFeKey = BinToHexLen((PBYTE)(tAWWE.acWhiteHash), SHA512_BLOCK_SIZE);
				if(m_tWEngWLExMap.find(strFeKey) == m_tWEngWLExMap.end())
				{
					strcpy_ext(tAWWE.acFilePath, strObjectName, MAX_PATH);
					m_tWEngWLExMap[strFeKey] = tAWWE;
					if((m_tWEngWLExMap.size() % 1000) == 0)
					{
						WriteLogN("[%s] add nc ptn op local item : [%d]", m_strThreadName.c_str(), m_tWEngWLExMap.size());
					}	
				}	
				else
				{
					CTokenString Token(strObjectName, strObjectName.GetLength(), '\\', 1);

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
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoNcPtnScan::ProcessScanResult()
{
/*
	PDB_PO_NC_PTN_OP_UNIT pdata = GetCurWork(); 
	if(!pdata)		return 0;

	{
		TMapWEngWLExItor begin, end;
		begin = m_tWEngWLExMap.begin();	end = m_tWEngWLExMap.end();
		for(begin; begin != end; begin++)
		{
			if(pdata->nFileGatherType & SS_PO_NC_PTN_OP_NEW_FILE_SEND_TYPE_INFO)
			{
				String strFeKey = BinToHexLen((PBYTE)(begin->second.acWhiteHash), SHA512_BLOCK_SIZE);

				PDB_SITE_FILE pdsf = t_ManageSiteFile->FindPosSKeyID_Item(strFeKey, SS_SITE_FILE_KEY_TYPE_FE);
				if(pdsf && (pdsf->nSyncSvrStep & SS_PO_NC_PTN_OP_NEW_FILE_SEND_TYPE_INFO))		continue;
				
				CTokenString Token(begin->second.acFilePath, strlen_ext(begin->second.acFilePath), '\\', 1);

				DB_SITE_FILE tDSF;
				tDSF.strFeKey = strFeKey;				
				tDSF.strFileName = Token.NextToken();
				tDSF.strFilePath = Token.RemainValue();

				//DWORD_PTR dwError = -1;
				//SendMessageTimeout(t_EnvInfoOp->GetMainHandle(), WM_GLOBAL_THREAD_LOG_EVT, (WPARAM)&tDSF, (LPARAM)ASI_EPS_APP_LOG_TYPE_SITE_FILE, SMTO_NORMAL, SENDMESSAGE_DEFAULT_WAITING, &dwError);
				SendMessage(t_EnvInfoOp->GetMainHandle(), WM_GLOBAL_THREAD_LOG_EVT, (WPARAM)&tDSF, (LPARAM)ASI_EPS_APP_LOG_TYPE_SITE_FILE);
			}
		}
	}

	{
		TListDBSiteFileAliasItor begin, end;
		begin = m_tSiteFileAliasList.begin();	end = m_tSiteFileAliasList.end();
		for(begin; begin != end; begin++)
		{
			if(pdata->nFileGatherType & SS_PO_NC_PTN_OP_NEW_FILE_SEND_TYPE_INFO)
			{
				PDB_SITE_FILE_ALIAS pdsf = t_ManageSiteFileAlias->FindSiteFileAliasKey((*begin));
				if(pdsf && (pdsf->nSyncSvrStep & SS_PO_NC_PTN_OP_NEW_FILE_SEND_TYPE_INFO))		continue;

				SendMessage(t_EnvInfoOp->GetMainHandle(), WM_GLOBAL_THREAD_LOG_EVT, (WPARAM)&(*begin), (LPARAM)ASI_EPS_APP_LOG_TYPE_SITE_FILE_ALIAS);
			}
		}
	}
*/
	return 0;
}
//---------------------------------------------------------------------------
