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

//---------------------------------------------------------------------------


#include "stdafx.h"
#include "com_struct.h"
#include "LogicMgrPtnPatch.h"

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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// INT32	CLogicMgrPtnPatch::ChkPtnPatchUtil()
// {
// 	if(m_nInitPatchUtil != SS_PTN_PATCH_UTIL_STATUS_NONE)	return -1;
// 	
// 	TListID tIDList;
// 	INT32 nRtn = 0;
// 	t_ManagePtnPatchUtil->GetItemIDList(tIDList);
// 	{
// 		TListIDItor begin, end;
// 		begin = tIDList.begin();	end = tIDList.end();
// 		for(begin; begin != end; begin++)
// 		{
// 			PDB_PTN_PATCH_UTIL pdata = t_ManagePtnPatchUtil->FindItem(*begin);
// 			if(!pdata)		continue;
// 
// 			if(pdata->nSysID != m_tSysInfo.GetSystemID() || pdata->nProcArch != m_tSysInfo.GetSysProcArchitecture())		continue;
// 			if(pdata->nSysSPID != -1 && pdata->nSysSPID != m_tSysInfo.GetSPID())		continue;
// 			
// 			if(ChkPtnPatchUtilUnit(pdata))
// 			{
// 				nRtn = -1;
// 				break;
// 			}
// 		}
// 	}
// 
// 	if(m_nInitPatchUtil == SS_PTN_PATCH_UTIL_STATUS_NONE)	m_nInitPatchUtil = SS_PTN_PATCH_UTIL_STATUS_CONFIRM;
// 	return nRtn;
// }
// //---------------------------------------------------------------------------
// 
// INT32	CLogicMgrPtnPatch::ChkPtnPatchUtilUnit(PDB_PTN_PATCH_UTIL pdata, UINT32 nChkOnly)
// {
// 	if(pdata->strName.Find("KB") == -1)
// 	{
// 		CFileUtil tFileUtil;
// 		String strTarFile = tFileUtil.GetEnvPathToLocalPathSingle(pdata->strTarFPath, 0);
// 		if(tFileUtil.FileExists(strTarFile))	
// 		{
// 			String strCurVer = tFileUtil.GetFileVersion(strTarFile);
// 			if(CompareVersion(strCurVer, pdata->strTarFVer) <= 0)
// 			{
// 				WriteLogN("[%s] check ptn patch util confirm : [%s]-[%s]/[%s]", m_strLogicName.c_str(), pdata->strName.c_str(), strCurVer, pdata->strTarFVer.c_str());
// 				return 0;
// 			}
// 		}
// 	}	
// 	else
// 	{
// 		do 
// 		{
// 			CHAR szTempDir[TCHAR_MAX_SIZE] = {0, };
// 			GetTempPath(TCHAR_MAX_SIZE, szTempDir);
// 			CFileUtil tFileUtil;
// 
// 			tFileUtil.AddLastChar(szTempDir, '\\');
// 
// 			String strPatchLogListFile;
// 			strPatchLogListFile.Format("%spatch_list_%u.txt", szTempDir, GetTickCount());
// 			String strCommand;
// 			strCommand.Format("cmd.exe /C wmic.exe qfe get hotfixid > \"%s\"", strPatchLogListFile);
// 
// 			CProcUtil tProcUtil;
// 			tProcUtil.ProcessStart(NULL, strCommand, TRUE);
// 
// 			if(tFileUtil.FileExists(strPatchLogListFile) == FALSE)
// 			{
// 				WriteLogE("[%s] make qfe file list fail : [%s]", m_strLogicName.c_str(), strPatchLogListFile.c_str());
// 				break;
// 			}
// 			
// 			{
// 				USES_CONVERSION;
// 				UINT32 nFileSize = tFileUtil.GetFileSizeExt(strPatchLogListFile);
// 				PBYTE lpBuffer = NULL;
// 				UINT32 nFieSize = 0;
// 
// 				tFileUtil.ReadFileBuffer(strPatchLogListFile, &lpBuffer, nFileSize);
// 				DeleteFile(strPatchLogListFile);
// 				if(!lpBuffer)
// 				{
// 					WriteLogE("[%s] read qfe file list fail : [%s]", m_strLogicName.c_str(), strPatchLogListFile.c_str());
// 					break;
// 				}
// 
// 				String strFileBuff = (LPCTSTR)W2A((LPCWSTR)lpBuffer);
// 				free(lpBuffer);
// 
// 				if(strFileBuff.Find(pdata->strName) == -1)
// 				{
// 					break;
// 				}
// 				WriteLogN("[%s] check ptn patch util confirm : [%s]", m_strLogicName.c_str(), pdata->strName.c_str());
// 			}
// 
// 			{
// 				CFileUtil tFileUtil;
// 				String strTarFile = tFileUtil.GetEnvPathToLocalPathSingle(pdata->strTarFPath, 0);
// 				if(tFileUtil.FileExists(strTarFile))	
// 				{
// 					String strCurVer = tFileUtil.GetFileVersion(strTarFile);
// 					if(CompareVersion(strCurVer, pdata->strTarFVer) <= 0)
// 					{
// 						WriteLogN("[%s] check ptn patch util confirm : [%s]-[%s]/[%s]", m_strLogicName.c_str(), pdata->strName.c_str(), strCurVer.c_str(), pdata->strTarFVer.c_str());
// 						return 0;
// 					}
// 				}
// 			}
// 
// 		} while (FALSE);
// 	}
// 
// 
// 	if(nChkOnly)	return -1;
// 
// 	AddDpDownInfoUtil(pdata);
// 	
// 	return -1;
// }
// //---------------------------------------------------------------------------
// 
// INT32	CLogicMgrPtnPatch::ApplyPtnPatchUtil(UINT32 nID)
// {	
// 	PDB_PTN_PATCH_UTIL pdata = t_ManagePtnPatchUtil->FindItem(nID);
// 	if(!pdata)	
// 	{
// 		WriteLogE("[%s] not find apply ptn patch util : [%d]", m_strLogicName.c_str(), nID);
// 		return 0;
// 	}
// 
// 	if(ChkPtnPatchUtilUnit(pdata, 1))
// 	{
// 		WriteLogE("[%s] check ptn patch unit....apply after system restart.. : [%d][%s]", m_strLogicName.c_str(), nID, pdata->strName);
// 
// 		m_nInitPatchUtil = SS_PTN_PATCH_UTIL_STATUS_WAIT_REBOOT;		
// 		t_ManageEnvLocal->SetEnvLocalNumber(G_TYPE_PTN_PATCH_UTIL, 0, 0, GetCurrentDateTimeInt());
// 
// 		InitDLEH(EVENT_OPERATION_TYPE_RESTART, EVENT_OBJECT_TYPE_DEPLOY, EVENT_OBJECT_CODE_DEPLOY_PATCH, 0, "");
// 		t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
// 
// 		return 0;
// 	}
// 
// 	m_nInitPatchUtil = SS_PTN_PATCH_UTIL_STATUS_NONE;
// 	return 0;
// }
// //---------------------------------------------------------------------------
// 
// INT32	CLogicMgrPtnPatch::IsWaitBootByPatchUtil()
// {
// 	UINT32 nEnvLocalTime = t_ManageEnvLocal->GetEnvLocalNumber(G_TYPE_PTN_PATCH_UTIL, 0, 0);
// 	if(!nEnvLocalTime)		return 0;
// 
// 	if(t_EnvInfo->m_nBootChkTime < nEnvLocalTime)	return 1;
// 
// 	t_ManageEnvLocal->SetEnvLocalNumber(G_TYPE_PTN_PATCH_UTIL, 0, 0, 0);
// 	return 0;
// }
// //---------------------------------------------------------------------------
// 
// INT32		CLogicMgrPtnPatch::AddDpDownInfoUtil(PDB_PTN_PATCH_UTIL pdata)
// {
// 	PDB_PO_SVR_INFO_UDT pdpsiu = (PDB_PO_SVR_INFO_UDT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_UDT);
// 	if(!pdpsiu)
// 	{
// 		WriteLogE("[%s] not find policy svr info udt", m_strLogicName);
// 		m_nInitPatchUtil = SS_PTN_PATCH_UTIL_STATUS_WAIT;
// 		return -1;
// 	}
// 	else if(pdpsiu->strDataSvrInfoList.empty())
// 	{
// 		WriteLogE("[%s] not exists udt chk svr info list", m_strLogicName);
// 		m_nInitPatchUtil = SS_PTN_PATCH_UTIL_STATUS_WAIT;
// 		return -2;
// 	}
// 
// 	String strDnFileName, strDnFileHash;
// 	strDnFileName = pdata->strPatchFName;
// 	strDnFileHash = pdata->strPatchFHash;
// 
// 	if(strDnFileName.empty() || strDnFileHash.empty())
// 	{
// 		WriteLogE("[%s] invalid patch util file info [%s][%s]", m_strLogicName.c_str(), strDnFileName, strDnFileHash);
// 		m_nInitPatchUtil = SS_PTN_PATCH_UTIL_STATUS_FAIL;
// 		return -4;
// 	}
// 
// 	{
// 		m_nInitPatchUtil = SS_PTN_PATCH_UTIL_STATUS_CHANGE;
// 
// 		ASI_FDL_INFO tAFI;
// 
// 		tAFI.nID = t_EnvInfoOp->GetGlobalID();
// 		tAFI.nItemType = SS_DN_FILE_TYPE_PPTN_PATCH_UTIL;
// 		tAFI.nItemID = 0;	
// 		tAFI.nPolicyID = pdata->nID;
// 		tAFI.nDLSvrUsedFlag	= (ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE);
// 
// 		strcpy_ext(tAFI.szDLPath, STR_DEPLOY_FILE_HOME_PPTN_UTIL, MAX_PATH);
// 
// 		{	
// 			strcpy_ext(tAFI.szFileName, strDnFileName, MAX_PATH);
// 			strcpy_ext(tAFI.szFileHash, strDnFileHash, MAX_PATH);
// 		}	
// 
// 		if(t_ManageFileDown->IsExistDnInfo(tAFI.nItemType, tAFI.nPolicyID, tAFI.nItemID))
// 		{
// 			WriteLogN("already exists pm ptn op download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList);
// 			return 0;
// 		}
// 
// 		WriteLogN("start ptn patch util download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList);
// 		t_ASIFDLDLLUtil->SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList);
// 		t_ASIFDLDLLUtil->AddDLInfo(&tAFI);
// 		t_ManageFileDown->AddItem(tAFI);
// 	}
// 	return 0;
// }
// //---------------------------------------------------------------------------
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