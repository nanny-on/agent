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
#include "ApplyPatchUtil.h"


CApplyPatchUtil::CApplyPatchUtil(void)
{
}
//--------------------------------------------------------------------------

CApplyPatchUtil::~CApplyPatchUtil(void)
{
}
//--------------------------------------------------------------------------

INT32		CApplyPatchUtil::GetPatchFileType(String strFileName)
{
	UINT32 nRtn = SS_APPLY_PATCH_FILE_TYPE_NONE;

	CTokenString Token(strFileName.c_str(), strFileName.length(), '.', 1);
	String strExtension = Token.NextToken();

	if(strExtension == "exe")		nRtn = SS_APPLY_PATCH_FILE_TYPE_EXE;
	if(strExtension == "msu")		nRtn = SS_APPLY_PATCH_FILE_TYPE_MSU;
	if(strExtension == "msi")		nRtn = SS_APPLY_PATCH_FILE_TYPE_MSI;
	if(strExtension == "msp")		nRtn = SS_APPLY_PATCH_FILE_TYPE_MSP;
	if(strExtension == "cab")		nRtn = SS_APPLY_PATCH_FILE_TYPE_CAB;

	return nRtn;
}
//--------------------------------------------------------------------------

String		CApplyPatchUtil::GetCommandByExeType(String strFileName)
{
	String strCmd = "";
/*
	CHAR szFileDescr[CHAR_MAX_SIZE] = {0, };
	t_ASIFIDLLUtil->ASIFI_GetFileDescription(strFileName, szFileDescr);
	if(szFileDescr[0] == 0)
	{
		t_ASIFIDLLUtil->ASIFI_GetFileSpecialValue(strFileName, "ProductName", szFileDescr);
	}

	String strCmd = "";
	String strFileDescr = szFileDescr;
	strFileDescr.MakeLower();

	TMapIDStr tCompMap, tCmdMap;
	{
		INT32 nIdx = 1;
		tCompMap[nIdx] = "hotfix package";				tCmdMap[nIdx] = " /quiet /norestart";		nIdx += 1;
		tCompMap[nIdx] = "update package";				tCmdMap[nIdx] = " /quiet /norestart";		nIdx += 1;
		tCompMap[nIdx] = "security update";				tCmdMap[nIdx] = " /quiet /norestart";		nIdx += 1;
		tCompMap[nIdx] = "self-extracting cabinet";		tCmdMap[nIdx] = " /quiet /norestart";		nIdx += 1;
		tCompMap[nIdx] = "service pack package";		tCmdMap[nIdx] = " /quiet /norestart";		nIdx += 1;
		tCompMap[nIdx] = "* .net framework 4.*";		tCmdMap[nIdx] = " /quiet /norestart";		nIdx += 1;
		tCompMap[nIdx] = "* .net framework 3.*";		tCmdMap[nIdx] = " /quiet /norestart";		nIdx += 1;
		tCompMap[nIdx] = "* .net framework 2.*";		tCmdMap[nIdx] = " /quiet /norestart";		nIdx += 1;
		tCompMap[nIdx] = "* .net framework 1.*";		tCmdMap[nIdx] = " /q /norestart";			nIdx += 1;

		tCompMap[nIdx] = "win32 cabinet self-extractor";					tCmdMap[nIdx] = " /Q";				nIdx += 1;
		tCompMap[nIdx] = "*malicious software removal tool *";				tCmdMap[nIdx] = " /Q";				nIdx += 1;

		tCompMap[nIdx] = "microsoft visual c++ 2008 redistributable *";		tCmdMap[nIdx] = " /q";				nIdx += 1;
		tCompMap[nIdx] = "microsoft visual c++ 2010 redistributable *";		tCmdMap[nIdx] = " /q /norestart";	nIdx += 1;
		tCompMap[nIdx] = "*microsoft visual studio 2010*";					tCmdMap[nIdx] = " /q /norestart";	nIdx += 1;
		tCompMap[nIdx] = "*microsoft visual studio 2008 setup";				tCmdMap[nIdx] = " /q /norestart";	nIdx += 1;
	}

	{
		TMapIDStrItor begin, end;
		begin = tCompMap.begin();	end = tCompMap.end();
		for(begin; begin != end; begin++)
		{
			if(StrMatchSpec(strFileDescr, begin->second) == 0)		continue;

			TMapIDStrItor find = tCmdMap.find(begin->first);
			if(find != tCmdMap.end())
			{
				strCmd = find->second;
				break;
			}
		}

		if(strCmd.empty())
		{
			strCmd = " /quiet /norestart";
			WriteLogE("[apply patch util] can not match file description : [%s][%s]", strFileName, strFileDescr);
		}
	}
*/
	return strCmd;
}
//--------------------------------------------------------------------------

String		CApplyPatchUtil::GetLogFileName(String strFileName)
{
	String strKBName = "";
	CTokenString Token(strFileName.c_str(), strFileName.length(), '-');
	while(Token.IsNextToken())
	{
		String strCell = Token.NextToken();
		if(strCell.find("kb") == string::npos)		continue;

		strKBName = strCell;
		break;
	}

	if(strKBName.empty())
	{
		strKBName = "NPM_Log";
	}

	String strLogFileName;	
	strLogFileName = SPrintf("%u_%s.npm.log", GetTickCount(), strKBName.c_str());
	return strLogFileName;
}
//--------------------------------------------------------------------------

String		CApplyPatchUtil::GetLogFileNameForRB(UINT32 nKBID)
{	
	String strFileName;	
	strFileName = SPrintf("%u_%u.npmrb.log", GetTickCount(), nKBID);
	return strFileName;
}
//--------------------------------------------------------------------------

INT32		CApplyPatchUtil::MakeExecuteWork(MEM_WORK_EXECUTE& tMWE, String strFileName, String strCmd)
{
/*
	UINT32 nFileType = GetPatchFileType(strFileName);
	if(nFileType == SS_APPLY_PATCH_FILE_TYPE_NONE)
	{
		WriteLogN("[apply patch util] can not support file extension : [%s]", strFileName);
		return -1;
	}


	{
		tMWE.nType			= G_TYPE_PTN_PATCH;
		tMWE.nCode			= nFileType;
		tMWE.strFileName	= strFileName;
	}


	CFileUtil tFileUtil;
	CHAR szSysDir[MAX_PATH] = {0, };
	GetSystemDirectory(szSysDir, MAX_PATH);
	tFileUtil.DelLastChar(szSysDir, '\\');
	
	{
		String strExe = "";
		switch(nFileType)
		{
			case SS_APPLY_PATCH_FILE_TYPE_EXE:	strExe= SPrintf("\"%s\"", strFileName);	break;
			case SS_APPLY_PATCH_FILE_TYPE_MSU:	strExe= SPrintf("%s\\wusa.exe", szSysDir);	break;
			case SS_APPLY_PATCH_FILE_TYPE_MSI:	strExe= SPrintf("%s\\msiexec.exe", szSysDir);	break;
			case SS_APPLY_PATCH_FILE_TYPE_MSP:	strExe= SPrintf("%s\\msiexec.exe", szSysDir);	break;
			case SS_APPLY_PATCH_FILE_TYPE_CAB:
			{
				if(m_tSysInfo.IsLowOsVer(6, 0))	strExe= SPrintf("%s\\pkgmgr.exe", szSysDir);
				else								strExe= SPrintf("%s\\dism.exe", szSysDir);
				break;
			}
		}

		if(strCmd.empty())
		{
			switch(nFileType)
			{
				case SS_APPLY_PATCH_FILE_TYPE_EXE:	tMWE.strCommand= SPrintf("%s %s", strExe, GetCommandByExeType(strFileName));	break;
				case SS_APPLY_PATCH_FILE_TYPE_MSU:	tMWE.strCommand= SPrintf("%s \"%s\" /quiet /norestart", strExe, strFileName);	break;
				case SS_APPLY_PATCH_FILE_TYPE_MSI:	tMWE.strCommand= SPrintf("%s /p \"%s\" /quiet /norestart", strExe, strFileName);				break;
				case SS_APPLY_PATCH_FILE_TYPE_MSP:	tMWE.strCommand= SPrintf("%s /p \"%s\" /quiet /norestart", strExe, strFileName);	break;
				case SS_APPLY_PATCH_FILE_TYPE_CAB:
				{
					if(m_tSysInfo.IsLowOsVer(6, 0))	tMWE.strCommand= SPrintf("%s /ip /m:c:\"%s\" /quiet /norestart", strExe, strFileName);
					else								tMWE.strCommand= SPrintf("%s /online /add-package /packagepath:\"%s\" /quiet /norestart", strExe, strFileName);
					break;
				}
			}
		}
		else
		{
			switch(nFileType)
			{
				case SS_APPLY_PATCH_FILE_TYPE_EXE:	tMWE.strCommand= SPrintf("%s %s", strExe, strCmd);	break;
				case SS_APPLY_PATCH_FILE_TYPE_MSU:	tMWE.strCommand= SPrintf("%s \"%s\" %s", strExe, strFileName, strCmd);	break;
				case SS_APPLY_PATCH_FILE_TYPE_MSI:	tMWE.strCommand= SPrintf("%s /p \"%s\" %s", strExe, strFileName, strCmd);				break;
				case SS_APPLY_PATCH_FILE_TYPE_MSP:	tMWE.strCommand= SPrintf("%s /p \"%s\" %s", strExe, strFileName, strCmd);	break;
				case SS_APPLY_PATCH_FILE_TYPE_CAB:	tMWE.strCommand= SPrintf("%s \"%s\" %s", strExe, strFileName, strCmd);	break;
			}
		}


	}
	
	switch(nFileType)
	{
		case SS_APPLY_PATCH_FILE_TYPE_MSU:
		{
			tMWE.strLogFile	= GetLogFileName(strFileName);
			String strLogCmd;
			strLogCmd= SPrintf(" /log:\"%s\"", tMWE.strLogFile);
			tMWE.strCommand += strLogCmd;

			break;
		}	
		case SS_APPLY_PATCH_FILE_TYPE_MSP:
		case SS_APPLY_PATCH_FILE_TYPE_MSI:
		{
			tMWE.strLogFile	= GetLogFileName(strFileName);
			String strLogCmd;
			strLogCmd= SPrintf(" /l*vx \"%s\"", tMWE.strLogFile);

			tMWE.strCommand += strLogCmd;
			break;
		}
		case SS_APPLY_PATCH_FILE_TYPE_CAB:
		{
			tMWE.strLogFile	= GetLogFileName(strFileName);
			String strLogCmd;

			if(m_tSysInfo.IsLowOsVer(6, 0))		strLogCmd= SPrintf(" /l:\"%s\"", tMWE.strLogFile);
			else								strLogCmd= SPrintf(" /logpath:\"%s\"", tMWE.strLogFile);

			tMWE.strCommand += strLogCmd;
			break;
		}
	}
*/
	return 0;
}
//--------------------------------------------------------------------------

INT32		CApplyPatchUtil::IsApplyPatch(UINT32 nPatchID, UINT32 nPoPmScanUnitID)
{
	INT32 nRtn = 0;
/*
	INT32 nContinue = 1;
	do 
	{
		if(t_ManageEnvLicense->IsInvalidRight(SS_PACKAGE_TYPE_NPMS, 0, 0))
		{
			nContinue = 0;
			break;
		}

		PDB_PO_PM_SCAN pdb_po = (PDB_PO_PM_SCAN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_PM_SCAN);
		if(!pdb_po)	
		{
			WriteLogE("[apply patch util] not find current policy : pm_scan");
			nContinue = 0;
			break;
		}

		if(pdb_po->tDPH.nUsedMode != STATUS_USED_MODE_ON)
		{
			WriteLogN("[apply patch util] current pm_scan policy not used : [patch_id:%d]", nPatchID);
			nContinue = 0;
			break;
		}

		PDB_PO_PM_SCAN_UNIT pdata_pu = t_ManagePoPmScanUnit->FindItem(nPoPmScanUnitID);
		if(!pdata_pu)
		{
			WriteLogE("[apply patch util] current pm_scan_unit policy is not find : [pu:%d][patch_id:%d]", nPoPmScanUnitID, nPatchID);
			nContinue = 0;
			break;
		}

		if(pdata_pu->tDPH.nExtOption & SS_PO_PM_SCAN_EXT_OPTION_TYPE_SCAN_ONLY)		
		{
			WriteLogN("[apply patch util] current pm_scan_unit policy is scan olny enabled : [pu:%d][patch_id:%d]", nPoPmScanUnitID, nPatchID);
			nContinue = 0;
			break;
		}

	} while (FALSE);
	if(!nContinue)	return nRtn;

	do
	{
		PDB_PO_PM_EX pdata = (PDB_PO_PM_EX)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_PM_EX);
		if(!pdata)		break;

		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_ON)	break;

		TListID tIDList;
		t_ManagePoPmExPkg->GetKeyIDList(pdata->tDPH.nID, tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_PM_EX_UNIT pdata_unit = t_ManagePoPmExUnit->FindItem(*begin);
			if(!pdata_unit)	continue;

			if(pdata_unit->tDPH.nUsedMode != STATUS_USED_MODE_ON)	continue;

			if(IsExistsValueFromMap(pdata_unit->tPatchIDMap, nPatchID) == 0)		break;

			nContinue = 0;
		}
	}while(FALSE);
	if(!nContinue)	return nRtn;
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	do 
	{
		PDB_PO_PM_OP pdata_po = (PDB_PO_PM_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_PM_OP);
		if(!pdata_po)	
		{
			WriteLogE("[apply patch util] not find current policy : pm_scan #2");
			nContinue = 0;
			break;
		}
		
		PDB_PTN_PATCH pdata = t_MMPPPatch->t_ManagePtnPatch->FindItem(nPatchID);
		if(!pdata)		
		{
			nContinue = 0;
			break;
		}

		if(pdata->nType & pdata_po->nAutoPatchType)
		{
			nRtn = 1;
			nContinue = 0;
			break;
		}
	} while (FALSE);
	if(!nContinue)	return nRtn;
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	do 
	{
		PDB_PO_PM_DM pdata = (PDB_PO_PM_DM)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_PM_DM);
		if(!pdata)		break;

		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_ON)	break;

		TListID tIDList;
		t_ManagePoPmDmPkg->GetKeyIDList(pdata->tDPH.nID, tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_PM_DM_UNIT pdata_unit = t_ManagePoPmDmUnit->FindItem(*begin);
			if(!pdata_unit)	continue;

			if(pdata_unit->tDPH.nUsedMode != STATUS_USED_MODE_ON)	continue;

			if(IsExistsValueFromMap(pdata_unit->tPatchIDMap, nPatchID) == 0)		break;

			if(pdata_unit->nApplyType == SS_PO_PM_DM_APPLY_TYPE_AUTO)
			{
				nRtn = 1;
				nContinue = 0;
				break;
			}
		}
	} while (FALSE);
	if(!nContinue)	return nRtn;
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////
*/
	return nRtn;
}
//--------------------------------------------------------------------------

INT32		CApplyPatchUtil::GetMSUPatchExecuteRst(String strLogFile)
{
	INT32 nRtn = SS_LOG_PATCH_STATUS_TYPE_END;
/*
	CFileUtil tFileUtil;

	PBYTE pFileBuf = NULL;
	UINT32 nFileLen = 0;
	tFileUtil.ReadFileBuffer(strLogFile, &pFileBuf, nFileLen);
	if(!nFileLen)	return SS_LOG_PATCH_STATUS_TYPE_END;

	do
	{
		{
			BYTE lpMsuRstSuccess[] =		{	0x49, 0x00, 0x73, 0x00, 0x52, 0x00, 0x65, 0x00, 0x62, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x74, 0x00, 
												0x52, 0x00, 0x65, 0x00, 0x71, 0x00, 0x75, 0x00, 0x69, 0x00, 0x72, 0x00, 0x65, 0x00, 0x64, 0x00, 
												0x3A, 0x00, 0x20, 0x00, 
												0x30, 0x00};

			if(FindMemory(pFileBuf, nFileLen, lpMsuRstSuccess, 38))
			{
				nRtn = SS_LOG_PATCH_STATUS_TYPE_END;
				break;
			}
		}
		{
			BYTE lpMsuRstSuccRestart[] =	{	0x49, 0x00, 0x73, 0x00, 0x52, 0x00, 0x65, 0x00, 0x62, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x74, 0x00, 
												0x52, 0x00, 0x65, 0x00, 0x71, 0x00, 0x75, 0x00, 0x69, 0x00, 0x72, 0x00, 0x65, 0x00, 0x64, 0x00, 
												0x3A, 0x00, 0x20, 0x00, 
												0x31, 0x00};

			if(FindMemory(pFileBuf, nFileLen, lpMsuRstSuccRestart, 38))
			{
				nRtn = SS_LOG_PATCH_STATUS_TYPE_END_RESTART;
				break;
			}
		}
	}while(FALSE);
*/	
	return nRtn;
}
//--------------------------------------------------------------------------

INT32		CApplyPatchUtil::MakeRollBackWork(MEM_WORK_EXECUTE& tMWE, PDB_HOST_PATCH pdata)
{
/*
	tMWE.nType			= G_TYPE_PO_PM_EX;
	tMWE.nItemID		= pdata->nID;
	CFileUtil tFileUtil;
	CHAR szSysDir[MAX_PATH] = {0, };
	GetSystemDirectory(szSysDir, MAX_PATH);
	tFileUtil.DelLastChar(szSysDir, '\\');

	switch(pdata->nFindType)
	{
		case ASI_PMS_WORK_FIND_TYPE_OLD_OS_VULN_QFE:
		{	
			tMWE.nCode = SS_APPLY_PATCH_FILE_TYPE_MSU;
			tMWE.strCommand= SPrintf("%s\\wusa.exe /uninstall /kb:%u /quiet /norestart", szSysDir, pdata->nKBID);	

			tMWE.strLogFile	= GetLogFileNameForRB(pdata->nKBID);
			String strLogCmd;
			strLogCmd= SPrintf(" /log:\"%s\"", tMWE.strLogFile);
			tMWE.strCommand += strLogCmd;			
			break;
		}
		default:
		{
			tMWE.strCommand = pdata->strUninstall;
			break;
		}
	}

	if(tMWE.strCommand.empty())
	{
		WriteLogE("[%s] not find execute information : [%d][%d]", __FUNCTION__, pdata->nID, pdata->nKBID);
		return -1;
	}
*/
	return 0;
}
//--------------------------------------------------------------------------

INT32		CApplyPatchUtil::IsValidExecuteDISM(String strLogFile)
{
/*
	String strFileBuff, strFindBuff;
	CFileUtil tFileUtil;
	tFileUtil.ReadFileBuffer(strLogFile, strFileBuff);

	if(strFileBuff.empty())		return 0;

	LPCTSTR szFileBuff = strFileBuff;
	while(szFileBuff[0])
	{
		if(CHAR(*szFileBuff) > 0)	break;
		szFileBuff += 1;
	}
	strFindBuff = szFileBuff;

	strFindBuff.MakeLower();
	if(strFindBuff.Find("package manager processed the command line but failed.") == -1)	return 1;
*/
	return 0;
}
//--------------------------------------------------------------------------

INT32		CApplyPatchUtil::GetCabDISMRst(String strLogFile)
{
/*
	String strFileBuff, strFindBuff;
	CFileUtil tFileUtil;
	tFileUtil.ReadFileBuffer(strLogFile, strFileBuff);

	if(strFileBuff.empty())		return 0;

	LPCTSTR szFileBuff = strFileBuff;
	while(szFileBuff[0])
	{
		if(CHAR(*szFileBuff) > 0)	break;
		szFileBuff += 1;
	}
	strFindBuff = szFileBuff;

	strFindBuff.MakeLower();

	if(strFindBuff.Find("package manager processed the command line but failed.") != -1)	return SS_LOG_PATCH_STATUS_TYPE_END;
	if(strFindBuff.Find("image session has been closed. reboot required=no.") != -1)		return SS_LOG_PATCH_STATUS_TYPE_END;
*/	
	return SS_LOG_PATCH_STATUS_TYPE_END_RESTART;
}
//--------------------------------------------------------------------------

INT32		CApplyPatchUtil::IsValidExecutePkgMgr(String strLogFile)
{
/*
	String strFileBuff, strFindBuff;
	CFileUtil tFileUtil;

	strLogFile += ".txt";
	tFileUtil.ReadFileBuffer(strLogFile, strFileBuff);

	if(strFileBuff.empty())		return 0;

	LPCTSTR szFileBuff = strFileBuff;
	while(szFileBuff[0])
	{
		if(CHAR(*szFileBuff) > 0)	break;
		szFileBuff += 1;
	}
	strFindBuff = szFileBuff;

	strFindBuff.MakeLower();
	if(strFindBuff.Find("package manager processed the command line but failed.") == -1)	return 1;
*/
	return 0;
}
//--------------------------------------------------------------------------

INT32		CApplyPatchUtil::GetCabPkgMgrRst(String strLogFile)
{
/*
	String strFileBuff, strFindBuff;
	CFileUtil tFileUtil;

	strLogFile += ".txt";
	tFileUtil.ReadFileBuffer(strLogFile, strFileBuff);

	if(strFileBuff.empty())		return 0;

	LPCTSTR szFileBuff = strFileBuff;
	while(szFileBuff[0])
	{
		if(CHAR(*szFileBuff) > 0)	break;
		szFileBuff += 1;
	}
	strFindBuff = szFileBuff;

	strFindBuff.MakeLower();
	if(strFindBuff.Find("package manager processed the command line but failed.") != -1)	return SS_LOG_PATCH_STATUS_TYPE_END;
	if(strFindBuff.Find("image session has been closed. reboot required=no.") != -1)		return SS_LOG_PATCH_STATUS_TYPE_END;
*/
	return SS_LOG_PATCH_STATUS_TYPE_END_RESTART;
}
//--------------------------------------------------------------------------

INT32		CApplyPatchUtil::GetCabMSIRst(String strLogFile)
{
/*
	CFileUtil tFileUtil;
	PBYTE pFileBuff = NULL;
	StringW strFileBuff;

	do
	{
		UINT32 nFileSize = tFileUtil.GetFileSizeExt(strLogFile);
		pFileBuff = (PBYTE)MallocExt(nFileSize + 2);

		tFileUtil.ReadFileBuffer(strLogFile, pFileBuff, nFileSize);
		if(!nFileSize)		break;

		strFileBuff = (LPWSTR)pFileBuff;
	}while(FALSE);

	SAFE_FREE(pFileBuff);
	if(strFileBuff.empty())	return SS_LOG_PATCH_STATUS_TYPE_END;
	
	if(strFileBuff.Find(L"Value of RebootAction property is ") == -1)	
		return SS_LOG_PATCH_STATUS_TYPE_END;
*/
	return SS_LOG_PATCH_STATUS_TYPE_END_RESTART;	
}
//--------------------------------------------------------------------------



