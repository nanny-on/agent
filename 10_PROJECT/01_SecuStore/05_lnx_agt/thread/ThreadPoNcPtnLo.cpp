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
#include "ThreadPoNcPtnLo.h"
//#include "MakePtnProcUtil.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadPoNcPtnLo

CThreadPoNcPtnLo*	t_ThreadPoNcPtnLo = NULL;

CThreadPoNcPtnLo::CThreadPoNcPtnLo()
{
	m_nOrderID = FF_START_ORDER_ID_PO_NC_LOCAL;
}

CThreadPoNcPtnLo::~CThreadPoNcPtnLo()
{
}

BOOL CThreadPoNcPtnLo::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadPoNcPtnLo::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadPoNcPtnLo message handlers

void			CThreadPoNcPtnLo::AddWork(UINT64 nOrderID)
{
	m_tOrderMutex.Lock();
	
	if(m_tOrderIDMap.find(nOrderID) == m_tOrderIDMap.end())
	{
		m_tOrderIDList.push_back(nOrderID);
		m_tOrderIDMap[nOrderID] = m_nOrderID++;
	}
	m_tOrderMutex.UnLock();
	return;
}
//-------------------------------------------------------------------------------

UINT32			CThreadPoNcPtnLo::IsWork()
{
	UINT32 nRtn = 0;
	m_tOrderMutex.Lock();
	nRtn = m_tOrderIDList.size();
	m_tOrderMutex.UnLock();
	return nRtn;
}
//-------------------------------------------------------------------------------

UINT32			CThreadPoNcPtnLo::NextWork()
{
	UINT32 nRtn = 0;
	m_tOrderMutex.Lock();

	if(m_tOrderIDList.size())
	{
		UINT64 nOrderID = *(m_tOrderIDList.begin());		m_tOrderIDList.pop_front();

		TMapID64Itor find = m_tOrderIDMap.find(nOrderID);
		if(find != m_tOrderIDMap.end())
		{
			nRtn = UINT32(find->second);
			m_tOrderIDMap.erase(find);
		}

		if(!nRtn)
		{
			m_tOrderIDList.clear();
			m_tOrderIDMap.clear();
		}
	}
	
	m_tOrderMutex.UnLock();
	return nRtn;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

int CThreadPoNcPtnLo::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nRunFlag = 1;

	WriteLogN("start %s thread : [%d]", m_strThreadName, m_nRunFlag);

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
/*
		if(IsWork())
		{	
			m_nWorkOrderID = NextWork();
			if(!m_nWorkOrderID)		continue;
			
			if(InitEnv())
			{
				WriteLogE("init environment fail : [%s]", m_strThreadName);
				continue;
			}

			if(m_tWEngWLExMap.size())
			{
				SavePtnFile();

				LRESULT hRst = SendMessage(t_EnvInfoOp->GetMainHandle(), WM_GLOBAL_THREAD_END, (WPARAM)&m_tCurPolicy, (LPARAM)G_TYPE_PO_NC_PTN_LO);
			}

			FreeEnv();
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

INT32		CThreadPoNcPtnLo::InitEnv()
{
/*
	{
		CHAR szDLLName[CHAR_MAX_SIZE] = {0, };

		if(m_tFFDLLUtil.LoadLibraryExt(FormatString(szDLLName, "%s\\asi_ff.so", t_EnvInfo->m_strDLLPath)))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
			return -15;
		}

		if(m_tWEDLLUtil.LoadLibraryExt(FormatString(szDLLName, "%s\\asi_weng.so", t_EnvInfo->m_strDLLPath)))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
			return -16;
		}

		if(m_tFIDLLUtil.LoadLibraryExt(FormatString(szDLLName, "%s\\asi_fi.so", t_EnvInfo->m_strDLLPath)))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
			return -16;
		}

		if(m_tHashDLLUtil.LoadLibraryExt(FormatString(szDLLName, "%s\\asi_hash.so", t_EnvInfo->m_strDLLPath)))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
			return -17;
		}

		if(m_tFCIDLLUtil.LoadLibraryExt(FormatString(szDLLName, "%s\\asi_fci.so", t_EnvInfo->m_strDLLPath)))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
			return -18;
		}


	}

	{
		ASI_FF_INIT_INFO tAFII;
		t_EnvInfoOp->GetFFPtnInitData(tAFII);
		m_tFFDLLUtil.ASIFF_Init(&tAFII);

		m_tWEDLLUtil.Init();
		m_tWEDLLUtil.InitDB();
	}

	PDB_PO_NC_PTN_LO pCurPolicy = NULL;
	{
		pCurPolicy = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_LO);
		if(!pCurPolicy)	
		{
			WriteLogE("[%s] not find po_fe_ptn_lo current policy", m_strThreadName);
			return -3;
		}

		m_tCurPolicy = *pCurPolicy;
	}
	
	m_tWEngWLExMap.clear();
	if(	m_tCurPolicy.nPtnRangeType == SS_PO_NC_PTN_LO_PTN_RANGE_TYPE_ALL_FILE ||
		m_tCurPolicy.nPtnRangeType == SS_PO_NC_PTN_LO_PTN_RANGE_TYPE_SYS_DRIVE)
	{
		String strScanPath = "";
		switch(m_tCurPolicy.nPtnRangeType)
		{
			case SS_PO_NC_PTN_LO_PTN_RANGE_TYPE_ALL_FILE:	
			{
				{
					ASIDM_INIT tADI;
					t_EnvInfoOp->GetDMInitData(tADI);
					t_ASIDMDLLUtil->Init(&tADI);
				}
				strScanPath = t_ASIDMDLLUtil->GetFixDrvList(m_strThreadName);	
				t_ASIDMDLLUtil->Free();				
				break;
			}
			case SS_PO_NC_PTN_LO_PTN_RANGE_TYPE_SYS_DRIVE:
			{
				CHAR szRtn[CHAR_MAX_SIZE] = {0, };
				GetSystemDirectory(szRtn, CHAR_MAX_SIZE);		

				strScanPath.Format("%C:\\*;", szRtn[0]);
				break;
			}
		}
		
		{
			CHAR chTok = GetToken(strScanPath, ';', ',');
			CTokenString Token(strScanPath, strScanPath.GetLength(), chTok);
			while(Token.IsNextToken())
			{
				String strPath = Token.NextToken();
				m_tFFDLLUtil.ASIFF_AddSearchDirPath(m_nWorkOrderID, strPath);
				WriteLogN("[%s] find file path : [%s]", m_strThreadName, strPath);
			}
		}
		m_tFFDLLUtil.ASIFF_SearchDirFileThread(m_nWorkOrderID);
		UINT32 nSearchEnd = 0;

		while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue() && !nSearchEnd)
		{
			switch(ChkScanFile())
			{
				case -1:
				{
					m_tFFDLLUtil.ASIFF_ClearFileMask(m_nWorkOrderID);
					m_tFFDLLUtil.ASIFF_ClearFileDateTime(m_nWorkOrderID);
					m_tFFDLLUtil.ASIFF_ClearExceptPath(m_nWorkOrderID);
					m_tFFDLLUtil.ASIFF_ClearSearchDirPath(m_nWorkOrderID);
					m_tFFDLLUtil.ASIFF_ClearFileFindOption(m_nWorkOrderID);
					WriteLogN("[%s] find file stop and clear operation : [%d]", m_strThreadName, m_nWorkOrderID);
					nSearchEnd = 1;
					break;
				}
				case 1:		Sleep(1000);		break;
			}
		}		
	}
	else
	{
		CProcUtil tProcUtil;
		TMapStrID tFMNameMap;
		tProcUtil.GetModuleFileNames(tFMNameMap, (m_tCurPolicy.nPtnObjType & SS_PO_NC_PTN_LO_PTN_OBJECT_TYPE_DLL));

		TMapStrIDItor begin, end;
		begin = tFMNameMap.begin();		end = tFMNameMap.end();
		for(begin; begin != end; begin++)
		{
			ASI_WENG_WL_EX tAWWE;
			DWORD dwFileType = 0;
			if(m_tWEDLLUtil.GetWL(begin->first, &tAWWE, sizeof(tAWWE), &dwFileType) == ERROR_SUCCESS &&
				dwFileType != AS_INVALID_FILE)
			{
				strcpy_ext(tAWWE.acFilePath, begin->first, MAX_PATH);
				String strFeKey = BinToHexLen((PBYTE)(tAWWE.acWhiteHash), SHA512_BLOCK_SIZE);
				if(m_tWEngWLExMap.find(strFeKey) == m_tWEngWLExMap.end())
				{
					WriteLogN("[%s] add exe ptn info : [%s]", m_strThreadName, tAWWE.acFilePath);
					m_tWEngWLExMap[strFeKey] = tAWWE;
				}
			}
		}
	}
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoNcPtnLo::FreeEnv()
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

INT32		CThreadPoNcPtnLo::ChkScanFile()
{
/*
	ASI_FF_FILE_ITEM tAFFI[1];
	ASI_FF_FILE_RESULT tAFFR;

	UINT32 nAFFINum = 1;
	memset(tAFFI, 0, sizeof(ASI_FF_FILE_ITEM) * nAFFINum);
	m_tFFDLLUtil.ASIFF_GetFindFileItem(m_nWorkOrderID, tAFFI, &nAFFINum, &tAFFR);

	if(tAFFR.nContinue == 0 && tAFFR.nMoreFileItem == 0)  
	{
		WriteLogN("[%s] find file work end : [%d][%d:%d]", m_strThreadName, m_nWorkOrderID, tAFFR.nFileWorkedNum, tAFFR.nFileTotalNum);
		return -1;
	}

	if(!nAFFINum)	return 1; 

	UINT32 nIdx = 0;
	while(nIdx < nAFFINum && GetContinue())
	{
		String strObjectName;
		strObjectName.Format("%s%s", tAFFI[nIdx].szFilePath, tAFFI[nIdx].szFileName);

		UINT32 nPeType = m_tFIDLLUtil.ASIFI_GetFilePEType(strObjectName);
		if(m_tCurPolicy.nPtnObjType & nPeType)	
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
						WriteLogN("[%s] add nc ptn op local item : [%d]", m_strThreadName, m_tWEngWLExMap.size());
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

INT32		CThreadPoNcPtnLo::SavePtnFile()
{
/*
	PDB_PO_NC_PTN_LO pCurPolicy = NULL;
	{
		pCurPolicy = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_LO);
		if(!pCurPolicy)	
		{
			WriteLogE("[%s] not find po_nc_ptn_lo current policy", m_strThreadName);
			return -1;
		}

		if(pCurPolicy->tDPH.nSeqNo != m_tCurPolicy.tDPH.nSeqNo)
		{
			WriteLogE("[%s] invalid sequence id po_nc_ptn_lo : [%d][%d]", m_strThreadName, pCurPolicy->tDPH.nSeqNo, m_tCurPolicy.tDPH.nSeqNo);
			return -2;
		}
	}

	CMakePtnProcUtil tMPPUtil;
	TListDBPtnProcFile tPtnFileList;
	String strSvPath, strSvName, strSvPathFile;
	String strPtnVersion;

	{
		CTime tCurTime = CTime::GetCurrentTime();
		strPtnVersion = tCurTime.Format("%Y-%m-%d %H%M");
	}


	{
		CHAR szLongPathName[CHAR_MAX_SIZE] = {0, };
		CHAR szShortPathName[CHAR_MAX_SIZE] = {0, };

		UINT32 nSaveTime = GetCurrentDateTimeInt();
		TMapWEngWLExItor begin, end;
		begin = m_tWEngWLExMap.begin();	end = m_tWEngWLExMap.end();
		for(begin; begin != end; begin++)
		{			
			ZeroMemoryExt(szLongPathName);
			ZeroMemoryExt(szShortPathName);

			if(GetShortPathName(begin->second.acFilePath, szShortPathName, CHAR_MAX_SIZE) == 0)
			{
				strcpy_ext(szShortPathName, begin->second.acFilePath);
			}
			if(GetLongPathName(szShortPathName, szLongPathName, CHAR_MAX_SIZE) == 0)
			{
				strcpy_ext(szLongPathName, szShortPathName);
			}
			
			if(szLongPathName[0])
			{
				szLongPathName[0] = TOUPPER(szLongPathName[0]);
			}

			
			DB_PTN_PROC_FILE tDPPF;
			CTokenString TokenFile(szLongPathName, strlen_ext(szLongPathName), '\\', 1);

			tDPPF.strFeKey		= begin->first;
			tDPPF.strFileName	= TokenFile.NextToken();
			tDPPF.strFilePath	= TokenFile.RemainValue();
			tDPPF.nRegDate		= nSaveTime;
			tDPPF.nPtnRisk		= PTN_FB_PTN_RISK_LOCAL;

			GatherFileInfo(&tDPPF);

			tPtnFileList.push_back(tDPPF);
		}
	}

	{
		{
			strSvPath.Format("\\%s", STR_WENG_PTN_FILE);
			strSvName.Format("LWC_%s", GetGUID());

			strSvPathFile.Format("%s\\%s", t_EnvInfo->m_strRootPath, strSvPath);
			{
				CFileUtil tFileUtil;
				tFileUtil.ForceDirectory(strSvPathFile);

				strSvPathFile += "\\";
				strSvPathFile += strSvName;
			}

			tMPPUtil.SetEnv("", strPtnVersion);
			tMPPUtil.InitDBF();
			tMPPUtil.OpenDBF(strSvPathFile);
			tMPPUtil.CreateTable();
			tMPPUtil.InsertTable();
		}


		TListDBPtnProcFileItor begin, end;
		begin = tPtnFileList.begin();	end = tPtnFileList.end();
		for(begin; begin != end; begin++)
		{
			if(SetER(tMPPUtil.InsertTable_PtnFileInfo(&(*begin))))
			{
				WriteLogE("nc ptn lo add db fail : nc ptn local white [%d]", g_nErrRtn);
				return -3;
			}
		}

		{
			tMPPUtil.CloseDBF();
		}
	}

	{		
		m_tCurPolicy.strSavePath = strSvPath;
		m_tCurPolicy.strSaveName = strSvName;

		CHAR szHashValue[CHAR_MAX_SIZE] = {0, };
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, strSvPathFile, szHashValue, CHAR_MAX_SIZE);

		m_tCurPolicy.strFileHash	= szHashValue;
		{
			m_tCurPolicy.strPtnVersion	= strPtnVersion;
			m_tCurPolicy.nPtnUnitNum = tPtnFileList.size();
		}
		WriteLogN("nc ptn lo save lo ptn wl item : [%d]", tPtnFileList.size());
	}
*/
// 	{
// 		if(SetER(t_ManagePoNcPtnLo->EditPoNcPtnLo(pCurPolicy)))
// 		{
// 			WriteLogE("edit nc ptn lo file fail : lo ptn white : [%d]", g_nErrRtn);
// 			return g_nErrRtn;
// 		}
// 	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadPoNcPtnLo::GatherFileInfo(PDB_PTN_PROC_FILE pdata)
{
/*
	CHAR szFileInfo[CHAR_MAX_SIZE] = {0, };

	if(pdata)
	{
		String strFullFileName;
		strFullFileName.Format("%s\\%s", pdata->strFilePath, pdata->strFileName);

		pdata->nRegDate		= GetCurrentDateTimeInt();
		pdata->nUsedMode	= STATUS_USED_MODE_ON;
		
		{
			ASI_FILE_CERT_INFO tAFCI;
			strcpy_ext(tAFCI.szCheckFileName, strFullFileName, MAX_PATH);
			m_tFCIDLLUtil.ASIFCI_GetFileCertInfo(&tAFCI);

			pdata->strPubName		= tAFCI.szPubName;			pdata->strPubName.Remove('\'');			
			pdata->strPubSN			= tAFCI.szPubSN;			//dsf.strPubName.Remove('\'');			
			pdata->strCntName		= tAFCI.szCntPubName;		pdata->strCntName.Remove('\'');			
			pdata->strCntSN			= tAFCI.szCntPubSN;			//dsf.strPubName.Remove('\'');			
		}
		{
			ZeroMemoryExt(szFileInfo);
			m_tFIDLLUtil.ASIFI_GetFileDescription(strFullFileName, szFileInfo);
			pdata->strFileDescr = szFileInfo;
			pdata->strFileDescr.Remove('\'');
		}
		{
			ZeroMemoryExt(szFileInfo);
			m_tFIDLLUtil.ASIFI_GetFileVersion(strFullFileName, szFileInfo);
			pdata->strFileVersion = szFileInfo;
			pdata->strFileVersion.Remove('\'');
		}
		{
			ZeroMemoryExt(szFileInfo);
			m_tFIDLLUtil.ASIFI_GetFileSpecialValue(strFullFileName, "CompanyName", szFileInfo);
			pdata->strPublisherName = szFileInfo;
			pdata->strPublisherName.Remove('\'');
		}
		{
			ZeroMemoryExt(szFileInfo);
			m_tFIDLLUtil.ASIFI_GetFileSpecialValue(strFullFileName, "ProductName", szFileInfo);
			pdata->strProductName = szFileInfo;
			pdata->strProductName.Remove('\'');
		}
		{
			ZeroMemoryExt(szFileInfo);
			SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullFileName, szFileInfo, CHAR_MAX_SIZE);
			pdata->strFileHash = szFileInfo;
		}
		{
			CFileUtil tFileUtil;
			pdata->nFileSize = tFileUtil.GetFileSizeExt(strFullFileName);
		}
		{
			pdata->nPeBit = m_tFIDLLUtil.ASIFI_GetFilePEMagic(strFullFileName);
			pdata->nPeType = m_tFIDLLUtil.ASIFI_GetFilePEType(strFullFileName);
		}		
	}
*/
	return 0;
}
//---------------------------------------------------------------------------
