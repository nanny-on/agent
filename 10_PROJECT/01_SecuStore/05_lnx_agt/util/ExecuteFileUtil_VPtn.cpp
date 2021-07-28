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
#include "ExecuteFileUtil.h"

//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_VPtnASV(INT32& nChgWPtn)
{
	INT32 nRtn = 0;
	TListID tIDList;
	TListIDItor begin, end;
	t_ManageFileDown->GetIDListByType(SS_DN_FILE_TYPE_VPTN_ASV, tIDList);

	if(tIDList.size() == 0)		return nRtn;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{										
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi)	continue;

		String strDwName, strSvName;

		do 
		{
			PDB_PO_IN_VULN_OP pdata = (PDB_PO_IN_VULN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_OP);
			if(!pdata)
			{
				WriteLogE("[%s] not find in vuln asv info : [%d][%d]", m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID);
				break;
			}		

			String strDnFileName, strDnFileHash;
			strDnFileName = (pdata->tDPIVOPF.nPtnDnFileType ? pdata->tDPIVOPF.strPtnDnFileName : pdata->tDPIVOPF.strPtnFileSvName);
			strDnFileHash = (pdata->tDPIVOPF.nPtnDnFileType ? pdata->tDPIVOPF.strPtnDnFileHash : pdata->tDPIVOPF.strPtnFileHash);

			strDwName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, strDnFileName.c_str());
			strSvName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PTN_VULN_FILE, pdata->tDPIVOPF.strPtnFileName.c_str());

			{
				String strPtnSvPath;
				strPtnSvPath = SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PTN_VULN_FILE);

				CFileUtil tFileUtil;
				if (tFileUtil.DirectoryExists(strPtnSvPath.c_str()))
				{
					if (!strPtnSvPath.empty())		// #20181226 :: Work Ahnshy - Prevent Delete All Files From Root
					{
						DeletePrefixFiles(strPtnSvPath, "patch_ptn");
					}
				}
				else
					tFileUtil.ForceDirectory(strPtnSvPath.c_str());
			}

			if(ChkFileHash(strDwName, strDnFileHash))
			{
				unlink(strDwName.c_str());
				WriteLogE("[%s] The hash of the in vuln asv download file is invalid : [%s][%s]", m_strUtilName.c_str(), strSvName.c_str(), pdata->tDPIVOPF.strPtnFileHash.c_str());
				break;
			}

			if(CopyFile(strDwName.c_str(), strSvName.c_str(), TRUE) == FALSE)
			{
				unlink(strDwName.c_str());
				WriteLogE("[%s] change in vuln asv file name : [%s]", m_strUtilName.c_str(), pdata->tDPIVOPF.strPtnFileName.c_str());
				break;
			}
			if(t_LogicMgrPtnVuln->ReloadPtnVuln() == 0)
			{
				t_LogicMgrPtnVuln->InitPtnDeploy();
			}
			nChgWPtn = 1;
			//unlink(strDwName);
		} while (FALSE);		
	}
	t_ManageFileDown->ClearItemByType(SS_DN_FILE_TYPE_VPTN_ASV);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_VPtnWSUS(INT32& nChgWPtn)
{
	INT32 nRtn = 0;
	TListID tIDList;
	TListIDItor begin, end;
	t_ManageFileDown->GetIDListByType(SS_DN_FILE_TYPE_VPTN_WSUS, tIDList);

	if(tIDList.size() == 0)		return nRtn;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi)	continue;

		String strDwName, strSvName;

		do 
		{
//			PDB_PTN_PATCH_WSUS pdata = t_MMPPVuln->t_ManagePtnPatchWsus->FindItem(pafi->nPolicyID);
			PDB_PTN_PATCH_WSUS pdata = NULL;
			if(!pdata)
			{
				WriteLogE("[%s] not find ptn vuln wsus info : [%d][%d]", m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID);
				break;
			}		

			String strDnFileName, strDnFileHash;
			strDnFileName = pdata->strName;
			strDnFileHash = pdata->strHash;

			strDwName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, strDnFileName.c_str());
			strSvName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PTN_VULN_FILE_PATCH, strDnFileName.c_str());

			{
				String strPtnSvPath;
				strPtnSvPath = SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PTN_VULN_FILE_PATCH);

				CFileUtil tFileUtil;
				if (tFileUtil.DirectoryExists(strPtnSvPath.c_str()) == FALSE)
				{
					tFileUtil.ForceDirectory(strPtnSvPath.c_str());
				}					
			}

			if(ChkFileHash(strSvName, strDnFileHash))
			{
				if(ChkFileHash(strDwName, strDnFileHash))
				{
					unlink(strDwName.c_str());
					WriteLogE("[%s] The hash of the ptn vuln wsus download file is invalid : [%s][%s]", m_strUtilName.c_str(), strDwName.c_str(), strDnFileHash.c_str());
					break;
				}

				if(CopyFile(strDwName.c_str(), strSvName.c_str(), TRUE) == FALSE)
				{
					unlink(strDwName.c_str());
					WriteLogE("[%s] change ptn vuln wsus file name : [%s]", m_strUtilName.c_str(), strDnFileHash.c_str());
					break;
				}
			}			

			{
				t_LogicMgrPtnVuln->ApplyPtnVulnWsus(pdata->nID);
			}

		} while (FALSE);		
	}
	t_ManageFileDown->ClearItemByType(SS_DN_FILE_TYPE_VPTN_WSUS);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_VPtnPatch()
{
	CApplyPatchUtil tAPUtil;
	TMapMemKeyWorkExecute tKeyExeWorkMap;

	INT32 nRtn = 0;
	TListDBLogPatch tChgLogPatchList;
	TMapID64 tDnPatchIDMap;

// 	{
// 		TListID tIDList;
// 		TListIDItor begin, end;
// 		t_ManageFileDown->GetIDListByType(SS_DN_FILE_TYPE_VPTN_PATCH, tIDList);
// 
// 		if(tIDList.size() == 0)		return nRtn;
// 
// 		begin = tIDList.begin();	end = tIDList.end();
// 		for(begin; begin != end; begin++)
// 		{
// 			PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
// 			if(!pafi)	continue;
// 
// 			UINT64 nDnKey = MAKEUINT64(pafi->nItemID, pafi->nPolicyID);
// 			TMapID64Itor find = tDnPatchIDMap.find(nDnKey);
// 			if(find == tDnPatchIDMap.end())
// 			{
// 				tDnPatchIDMap[nDnKey] = 0;
// 				find = tDnPatchIDMap.find(nDnKey);	
// 			}
// 
// 			INT32 nPatchStatus = 0;
// 
// 			if(pafi->nFileStatus != ASIFDL_DL_RST_STATUS_TYPE_SUCCESS_END)
// 			{
// 				if(pafi->nFileStatus == ASIFDL_DL_RST_STATUS_TYPE_CANCEL_REF_OVER)
// 					nPatchStatus = SS_LOG_PATCH_STATUS_TYPE_DOWN_FAIL_REF_OVER;
// 				else
// 					nPatchStatus = SS_LOG_PATCH_STATUS_TYPE_DOWN_FAIL;
// 			}
// 
// 			if(find->second < nPatchStatus)		find->second = nPatchStatus;
// 		}
// 	}
// 
// 	TMapID64Itor begin, end;
// 	begin = tDnPatchIDMap.begin();	end = tDnPatchIDMap.end();
// 	for(begin; begin != end; begin++)
// 	{
// 		String strDwName, strSvName;
// 		UINT32 nPolicyID	= HIUINT64(begin->first);
// 		UINT32 nPatchID		= LOUINT64(begin->first);
// 		do 
// 		{
// 			PDB_LOG_PATCH pdata = t_ManageLogPatch->FindKeyTypeID_Item(POS_KEY_ID_POSITION_LOG_PATCH_PID_MAP, nPatchID);
// 			if(!pdata)
// 			{
// 				WriteLogE("[%s] not find log patch info : [%d]", m_strUtilName.c_str(), nPatchID);
// 				break;
// 			}
// 
// 			if(begin->second != 0)
// 			{				
// 				WriteLogE("[%s] patch file download fail : [%d][%d]", m_strUtilName.c_str(), nPolicyID, pdata->nPatchID);
// 				pdata->nPatchStatus = UINT32(begin->second);
// 				pdata->nEvtTime = GetCurrentDateTimeInt();
// 				HISYNCSTEPUP(pdata->nSyncSvrStep);
// 				tChgLogPatchList.push_back(*pdata);				
// 				break;
// 			}
// 
// 			PDB_PTN_PATCH pdata_patch = t_MMPPVuln->t_ManagePtnPatch->FindItem(pdata->nPatchID);
// 			if(!pdata_patch)
// 			{
// 				WriteLogE("[%s] not find ptn patch file info : [%d][%d]", m_strUtilName.c_str(), nPolicyID, pdata->nPatchID);
// 				break;
// 			}
// 
// 			PMEM_PATCH_FILE pdata_patch_file = t_ManagePatchFile->FindItem(pdata->nPatchID);
// 			if(!pdata_patch_file)
// 			{
// 				WriteLogE("[%s] not find mem patch file info : [%d][%d]", m_strUtilName.c_str(), nPolicyID, pdata->nPatchID);
// 				break;
// 			}
// 
// 			if(tAPUtil.IsApplyPatch(pdata_patch->nID, nPolicyID) == 0)
// 			{
// 				pdata->nPatchStatus = SS_LOG_PATCH_STATUS_TYPE_WAIT_USER;
// 				pdata->nEvtTime = GetCurrentDateTimeInt();
// 				HISYNCSTEPUP(pdata->nSyncSvrStep);
// 
// 				tChgLogPatchList.push_back(*pdata);
// 				break;
// 			}
// 
// 			TMapIDStrItor begin, end;
// 			begin = pdata_patch_file->tDnFileMap.begin();	end = pdata_patch_file->tDnFileMap.end();
// 			for(begin; begin != end; begin++)
// 			{
// 				CTokenString Token(begin->second, begin->second.GetLength(), '.');
// 
// 				String strDnFileName, strDnFileHash;
// 				strDnFileName = begin->second;
// 				strDnFileHash = Token.NextToken();		strDnFileHash.MakeUpper();
// 
// 				strDwName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, strDnFileName);
// 				strSvName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PTN_PATCH_FILE_PATCH, strDnFileName);
// 
// 				{
// 					String strPtnSvPath;
// 					strPtnSvPath = SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PTN_PATCH_FILE_PATCH);
// 
// 					CFileUtil tFileUtil;
// 					if (tFileUtil.DirectoryExists(strPtnSvPath) == FALSE)
// 					{
// 						tFileUtil.ForceDirectory(strPtnSvPath);
// 					}					
// 				}
// 
// 				if(ChkFileHash(strSvName, strDnFileHash))
// 				{
// 					if(ChkFileHash(strDwName, strDnFileHash))
// 					{
// 						unlink(strDwName);
// 						WriteLogE("[%s] The hash of the downloaded ptn patch file is invalid : [%s][%s]", m_strUtilName.c_str(), strSvName, strDnFileHash);
// 						break;
// 					}
// 
// 					unlink(strSvName);
// 					if(CopyFile(strDwName, strSvName, TRUE) == FALSE)
// 					{
// 						unlink(strDwName);
// 						WriteLogE("[%s] change ptn patch file name fail : [%s]", m_strUtilName.c_str(), strDnFileHash);
// 						break;
// 					}
// 				}
// 				
// 
// 				{	
// 					String strCmd = "";					
// 					{
// 						MEM_WORK_EXECUTE tMWE;
// 						tMWE.nType			= G_TYPE_PTN_PATCH;
// 						tMWE.nItemID		= pdata->nID;
// 						tMWE.nItemPkg		= MAKEUINT32(begin->first, pdata_patch_file->tDnFileMap.size());
// 
// 						{
// 							PDB_PTN_PATCH_SCAN pdata_ptn_scan = t_MMPPVuln->t_ManagePtnPatchScan->FindSKeyID_Item(strDnFileHash);
// 							if(pdata_ptn_scan)
// 							{
// 								tMWE.nExeSession = pdata_ptn_scan->nFileExeSession;
// 								strCmd = pdata_ptn_scan->strFileCmd;
// 							}
// 						}
// 
// 						tAPUtil.MakeExecuteWork(tMWE, strSvName, strCmd);
// 						tKeyExeWorkMap[MAKEUINT64(begin->first, pdata_patch->nID)] = tMWE;
// 					}
// 
// 					{
// 						pdata->nPatchStatus = SS_LOG_PATCH_STATUS_TYPE_WAIT_AUTO;
// 						pdata->nEvtTime = GetCurrentDateTimeInt();
// 						HISYNCSTEPUP(pdata->nSyncSvrStep);
// 
// 						t_LogicMgrLogPatch->SetLogPatch(*pdata);
// 					}
// 				}
// 			}
// 
// 		} while (FALSE);		
// 	}
// 
// 	if(tKeyExeWorkMap.size())
// 	{
// 		TMapMemKeyWorkExecuteItor begin, end;
// 		begin = tKeyExeWorkMap.begin();	end = tKeyExeWorkMap.end();
// 		for(begin; begin != end; begin++)
// 		{
// 			WriteLogN("[%s] add execute work : [%d][%d - %d]", m_strUtilName.c_str(), begin->second.nItemID, HIUINT64(begin->first), LOUINT64(begin->first));
// 			t_ThreadExecute->AddWork(&(begin->second));			
// 		}	
// 	}
// 
// 	if(tChgLogPatchList.size())
// 	{
// 		t_LogicMgrLogPatch->SetLogPatch(tChgLogPatchList);
// 	}

	t_ManageFileDown->ClearItemByType(SS_DN_FILE_TYPE_VPTN_PATCH);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_VPtnDeploy()
{
	INT32 nRtn = 0;
	TListID tIDList;
	TListIDItor begin, end;
	t_ManageFileDown->GetIDListByType(SS_DN_FILE_TYPE_VPTN_DEPLOY, tIDList);

	if(tIDList.size() == 0)		return nRtn;

	CFileUtil tFileUtil;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi)	continue;

		String strDwName, strSvName;

		do 
		{
//			PDB_PTN_DEPLOY pdata = t_MMPPVuln->t_ManagePtnDeploy->FindItem(pafi->nItemID);
			PDB_PTN_DEPLOY pdata = NULL;
			if(!pdata)
			{
				WriteLogE("[%s] not find ptn deploy info : [%d][%d]", m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID);
				break;
			}		

			String strDnFileName, strDnFileHash;
			strDnFileName = pdata->strDnFileName;
			strDnFileHash = pdata->strDnFileHash;

			strDwName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, strDnFileName.c_str());			
			strSvName = tFileUtil.GetEnvPathToLocalPathSingle(pdata->strSvFileName, 0);

			{
				String strPtnSvPath;
				CTokenString TokenSv(strSvName.c_str(), strSvName.length(), '/', 1);	TokenSv.NextToken();
				strPtnSvPath = TokenSv.RemainValue();

				CFileUtil tFileUtil;
				if (tFileUtil.DirectoryExists(strPtnSvPath.c_str()) == FALSE)
				{
					tFileUtil.ForceDirectory(strPtnSvPath.c_str());
				}					
			}

			if(ChkFileHash(strDwName, strDnFileHash))
			{
				unlink(strDwName.c_str());
				WriteLogE("[%s] The hash of the ptn vuln deploy download file is invalid : [%s][%s]", m_strUtilName.c_str(), strSvName.c_str(), strDnFileHash.c_str());
				break;
			}

			if(tFileUtil.FileExists(strSvName.c_str()) == FALSE)
			{
				if(CopyFile(strDwName.c_str(), strSvName.c_str(), TRUE) == FALSE)
				{
					unlink(strDwName.c_str());
					WriteLogE("[%s] copy deploy file fail : [%d][%s]", m_strUtilName.c_str(), errno, strDwName.c_str());
					break;
				}
			}
			else
			{
				unlink(strDwName.c_str());
				WriteLogE("[%s] delete deploy old file fail : [%d][%s]", m_strUtilName.c_str(), errno, strSvName.c_str());
				break;
			}

			{
				t_LogicMgrPtnVuln->ApplyPtnDeploy(pdata->nID);
			}

		} while (FALSE);		
	}
	t_ManageFileDown->ClearItemByType(SS_DN_FILE_TYPE_VPTN_DEPLOY);
	return nRtn;
}
//---------------------------------------------------------------------------
