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

CExecuteFileUtil*		t_ExecuteFileUtil = NULL;

CExecuteFileUtil::CExecuteFileUtil(void)
{
	m_strUtilName = "exec_file_util";
}
//---------------------------------------------------------------------------

CExecuteFileUtil::~CExecuteFileUtil(void)
{
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile(LPVOID lParam)
{
	if(ExecuteFile_Udt())
	{
		t_ManageFileDown->ClearItem();
		t_ManageFileDown->ClearKeyIDList();
		if(lParam != NULL)
		{
			CMainDlg* pMainDlg = (CMainDlg*)lParam;
			WriteLogN("[%s] start to update. close main service.", m_strUtilName.c_str());
			pMainDlg->CloseMain();
			Sleep(100);
			pMainDlg->PostMessage(WM_QUIT, 0, 0);
		}

		return 0;
	}

	if(ExecuteFile_DpFile())
	{
		WriteLogN("[%s] exeucte file..", m_strUtilName.c_str());
	}

	{
		INT32 nChgWPtn = 0;
		if(ExecuteFile_WPtnGBL(nChgWPtn))
		{
			WriteLogN("[%s] wptn gbl file..", m_strUtilName.c_str());
		}

		if(ExecuteFile_WPtnGWL(nChgWPtn))
		{
			WriteLogN("[%s] wptn gwl file..", m_strUtilName.c_str());
		}
		if(nChgWPtn)
		{
			WriteLogN("[%s] change wptn file success [%d]..", m_strUtilName.c_str(), nChgWPtn);
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
		}
	}

	{
		INT32 nChgWPtn = 0;
		if(ExecuteFile_WPtnGBO(nChgWPtn))
		{
			WriteLogN("[%s] wptn gbo file..", m_strUtilName.c_str());
		}

		if(ExecuteFile_WPtnGWO(nChgWPtn))
		{
			WriteLogN("[%s] wptn gwo file..", m_strUtilName.c_str());
		}
		if(nChgWPtn)
		{
			WriteLogN("[%s] change wptn on file success [%d]..", m_strUtilName.c_str(), nChgWPtn);
			t_LogicMgrPoInPtnExTrust->CheckPtnLifeCycle();
			if(t_ThreadPoInPtnFile != NULL)
				t_ThreadPoInPtnFile->ClearPtnRet();
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
		}
	}
	
	{
		INT32 nChgWPtn = 0;
		if(ExecuteFile_PPtnASP(nChgWPtn))
		{
			WriteLogN("[%s] pptn patch..", m_strUtilName.c_str());
		}
		
		nChgWPtn = 0;
		if(ExecuteFile_PPtnWSUS(nChgWPtn))
		{
			WriteLogN("[%s] pptn wsus..", m_strUtilName.c_str());
		}

		if(ExecuteFile_PPtnPatchUtil())
		{
			WriteLogN("[%s] pptn patch util..", m_strUtilName.c_str());
		}	

		if(ExecuteFile_PPtnPatch())
		{
			WriteLogN("[%s] pptn patch..", m_strUtilName.c_str());
		}

		if(ExecuteFile_PPtnDeploy())
		{
			WriteLogN("[%s] pptn deploy..", m_strUtilName.c_str());
		}
	}

	{
		INT32 nChgWPtn = 0;
		if(ExecuteFile_VPtnASV(nChgWPtn))
		{
			WriteLogN("[%s] pptn vuln..", m_strUtilName.c_str());
		}
		
		nChgWPtn = 0;
		if(ExecuteFile_VPtnWSUS(nChgWPtn))
		{
			WriteLogN("[%s] pptn vuln wsus..", m_strUtilName.c_str());
		}	

		if(ExecuteFile_VPtnPatch())
		{
			WriteLogN("[%s] pptn vuln patch..", m_strUtilName.c_str());
		}

		if(ExecuteFile_VPtnDeploy())
		{
			WriteLogN("[%s] pptn vuln deploy..", m_strUtilName.c_str());
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_Udt()
{
	INT32 nRtn = 0;
	TListID tIDList;
	TListIDItor begin, end;
	char acZipFile[MAX_PATH] = {0,};
	char acUdtExePath[MAX_PATH] = {0,};
	char acUpdatePath[MAX_PATH] = {0,};
	char acCommand[MAX_PATH] = {0,};
	t_ManageFileDown->GetIDListByType(ASIFDL_DL_FILE_TYPE_UPDATE, tIDList);

	if(tIDList.size() == 0)
		return 0;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi)
			continue;
		
		snprintf(acUdtExePath, MAX_PATH-1, "%s/%s", (char *)t_EnvInfo->m_strUtilPath.c_str(), NANNY_UPDATE_IDENT);
		snprintf(acZipFile, MAX_PATH-1, "%s/%s/%s", (char *)t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, pafi->szFileName);
		snprintf(acUpdatePath, MAX_PATH-1, "%s/%s", (char *)t_EnvInfo->m_strRootPath.c_str(), STR_UNZIP_PATH_FOR_UPDATE);
		snprintf(acCommand, MAX_PATH-1, "-u -f %s -d %s", acZipFile, acUpdatePath);
		if(is_file(acZipFile) != 0)
		{
			char acTempFile[MAX_PATH] = {0,};
			snprintf(acTempFile, MAX_PATH-1, "%s/%s/%s", (char *)t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_TEMP, pafi->szFileName);
			MoveFileEx(acTempFile, acZipFile);
			if(is_file(acZipFile) != 0)
			{
				WriteLogE("[%s] fail to find zip file : [%s]", (char *)m_strUtilName.c_str(), acZipFile);
				continue;
			}
		}

		if(ChkFileHash(acZipFile, pafi->szFileHash))
		{
			continue;
		}
	
		if(DirectoryExists(acUpdatePath) == FALSE)
		{
			if(ForceDirectories(acUpdatePath) == FALSE)
			{
				WriteLogE("[%s] fail to create directory (%s) (%d)", (char *)m_strUtilName.c_str(), acUpdatePath, errno);
				continue;
			}
		}

		if(m_tProcUtil.UpdateProcStart(acUdtExePath, NANNY_UPDATE_IDENT, acZipFile, acUpdatePath))
		{
//			WriteLogN("[%s] start agent update file : (%s %s)", (char *)m_strUtilName.c_str(), acUdtExePath, acCommand);
			nRtn = 1;
			break;
		}

		t_EnvInfoOp->m_nModuleUpdateMode = STATUS_USED_MODE_ON;
		t_LogicApplyPolicy->SetEPSSvcDisable();
	}
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_DpFile()
{
	INT32 nRtn = 0;
	TListID	  tIDNonDelList;
	TListID64 tIDList;
	TListID64Itor begin, end;
	String strExeName, strSvName;

	t_ManageFileDown->GetDFIDList(tIDList);

	if(tIDList.size() == 0)		return nRtn;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		UINT32 nAFIID = LOUINT64(*begin);
		UINT32 nSchPkgID = HIUINT64(*begin);
		
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(nAFIID);
		if(!pafi)	continue;

		DB_LOG_DEPLOY_FILE tDLDF;
		PDB_LOG_DEPLOY_FILE pCurDLDF = NULL;
		tDLDF.nPolicyID	= pafi->nPolicyID;
		tDLDF.nUnitID	= pafi->nItemID;
		pCurDLDF = t_ManageLogDeployFile->FindItemByDFInfo(tDLDF);
		if(pCurDLDF)	tDLDF = *pCurDLDF;

		
		do 
		{
			PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfiu = t_ManagePoDeployFileInfoUnit->FindItemByPoUnitID(pafi->nItemID);
			if(!pdpdfiu)
			{
				WriteLogE("[%s] not find dp info : [%d][%d]", (char *)m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID);
				tDLDF.nEvtErrCode = ERR_SUCCESS;
				tDLDF.nEndTime = 0;
				tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_CANCEL_ADMIN;
				break;
			}		
			
			strExeName = SPrintf("%s/%s/%s", (char *)t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, (char *)pdpdfiu->strFileOrName.c_str());
			strSvName = SPrintf("%s/%s/%s", (char *)t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, (char *)pdpdfiu->strFileSaveName.c_str());

			unlink((char *)strExeName.c_str());
			if(CopyFile(strSvName.c_str(), strExeName.c_str(), TRUE) == FALSE)
			{
				WriteLogE("[%s] change execute file name : [%s]", (char *)m_strUtilName.c_str(), (char *)pdpdfiu->strFileOrName.c_str());
				tDLDF.nEvtErrCode = ERR_SYS_FILE_NOT_COPY;
				tDLDF.nEndTime = 0;
				tDLDF.nEndReason = ERR_SUCCESS;
				break;
			}

			if(ChkFileHash(strExeName, pdpdfiu->strFileHash))
			{
				WriteLogE("[%s] The hash of the download file is invalid : [%s][%s]", (char *)m_strUtilName.c_str(), (char *)strExeName.c_str(), (char *)pdpdfiu->strFileHash.c_str());
				tDLDF.nEvtErrCode = ERR_SYS_FILE_INVALID_HASH;
				tDLDF.nEndTime = 0;
				tDLDF.nEndReason = ERR_SUCCESS;
				break;
			}

			if(ChkFileCertInfo(strExeName))
			{
				WriteLogE("[%s] The CodeSign of the download file is invalid : [%s]", (char *)m_strUtilName.c_str(), (char *)strExeName.c_str());
				tDLDF.nEvtErrCode = ERR_SYS_FILE_NOT_FIND_CODESIGN;
				tDLDF.nEndTime = 0;
				tDLDF.nEndReason = ERR_SUCCESS;
				break;
			}

			UINT32 nProID = 0;

			switch(pdpdfiu->nFileType)
			{
				case SS_PO_DEPLOY_FILE_FILE_TYPE_SETUP:
				{
					if(pdpdfiu->nExeUserType == SS_PO_DEPLOY_FILE_EXE_USER_TYPE_SYSTEM)
					{
						if(m_tProcUtil.ProcessStart(strExeName.c_str(), pdpdfiu->strFileCommand.c_str(), FALSE, TRUE, &nProID))
						{
							tIDNonDelList.push_back(nAFIID);

							t_ManageFileDown->AddEndDpFile(nAFIID, nProID);

							if(!t_ThreadTimer->t_TimerUtil.IsEnabledTimer(TIMER_ID_DP_FILE_EXCUTE_END))
								t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_DP_FILE_EXCUTE_END);

							tDLDF.nEvtErrCode = ERR_SYS_FILE_INSTALL_ING;

							WriteLogN("[%s] execute process end : [%s] ", m_strUtilName.c_str(), pdpdfiu->strFileOrName.c_str());
						}
						else
						{
							tDLDF.nEvtErrCode = ERR_SYS_FILE_NOT_EXECUTE;
						}
					}
					else if(pdpdfiu->nExeUserType == SS_PO_DEPLOY_FILE_EXE_USER_TYPE_USER)
					{
						ASI_TS_INFO tATI;
						ASI_TS_MGR_PARAM tATMP;
						{
							sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_DEPLOY_FILE, GetTickCount()); 
							tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
							tATI.nStartTime	= GetCurrentDateTimeInt() + 5;

							tATMP.nTSSingleRun		= 1;
							tATMP.nTSAutoDel		= 1;
							tATMP.nTSWaitMode		= TS_EXCUTE_WAIT_MODE_EXCUTE_PROC_CHILD;
							tATMP.nTSChildWaitMode	= 1;

							tATMP.strTSChildPath = strExeName;
							tATMP.strTSChildArgument = pdpdfiu->strFileCommand;	

							tATMP.strTSResultPath	= STR_REG_DEFAULT_SVC_LOCAL_PATH;
						}

						switch(ExecuteFileByUser(tATI, tATMP, TS_EXCUTE_LIMIT_WAIT_TIME))
						{
							case -1:	tDLDF.nEvtErrCode = ERR_ENV_STATUS_IS_NOT_LOG_ON;	break;
							case 0:
							{
								tIDNonDelList.push_back(nAFIID);

//								t_ASITSDLLUtil->GetTaskExecuteProcID(tATI.szTaskName, &nProID);
								nProID = m_tProcUtil.GetChildProcessID(nProID, pdpdfiu->strFileOrName);

								t_ManageFileDown->AddEndDpFile(nAFIID, nProID);

								if(!t_ThreadTimer->t_TimerUtil.IsEnabledTimer(TIMER_ID_DP_FILE_EXCUTE_END))
									t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_DP_FILE_EXCUTE_END);

								tDLDF.nEvtErrCode = ERR_SYS_FILE_INSTALL_ING;

								break;
							}
							default:
							{
								tDLDF.nEvtErrCode = ERR_SYS_FILE_NOT_EXECUTE;
								break;
							}
						}
					}
					break;
				}
				case SS_PO_DEPLOY_FILE_FILE_TYPE_NORMAL:
				{
					CFileUtil tFileUtil;
					String strTarPath = tFileUtil.GetEnvPathToLocalPathSingle(pdpdfiu->strTargetPath);
					{
						CTokenString Token(strTarPath.c_str(), strTarPath.length(), '/', 1);
						Token.NextToken();
						String strOnlyDir = Token.RemainValue();
						tFileUtil.ForceDirectory(strOnlyDir.c_str());
					}

					if(pdpdfiu->nExeUserType == SS_PO_DEPLOY_FILE_EXE_USER_TYPE_SYSTEM)
					{
						if(tFileUtil.FileExists(strTarPath.c_str()) == FALSE || unlink(strTarPath.c_str()) == 0)
						{
							CopyFile(strExeName.c_str(), strTarPath.c_str(), TRUE);
						}
						else
						{
							String strTarPathTemp;
							strTarPathTemp= SPrintf("%s_ssa_%u", (char *)strTarPath.c_str(), GetTickCount());
							unlink(strTarPathTemp.c_str());
							CopyFile(strExeName.c_str(), strTarPathTemp.c_str(), FALSE);
//							MoveFileEx(strTarPathTemp, strTarPath, MOVEFILE_DELAY_UNTIL_REBOOT);
							MoveFileEx((char *)strTarPathTemp.c_str(), (char *)strTarPath.c_str());

						}

						WriteLogN("[%s] file move end : [%s] ", (char *)m_strUtilName.c_str(), (char *)pdpdfiu->strFileOrName.c_str());
						tDLDF.nEvtErrCode = ERR_SUCCESS;
						tDLDF.nEndTime = GetCurrentDateTimeInt();
						tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_COPY_END;
					}
					else if(pdpdfiu->nExeUserType == SS_PO_DEPLOY_FILE_EXE_USER_TYPE_USER)
					{
						ASI_TS_INFO tATI;
						ASI_TS_MGR_PARAM tATMP;
						{
							sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_DEPLOY_FILE, GetTickCount()); 
							tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
							tATI.nStartTime	= GetCurrentDateTimeInt() + 5;

							tATMP.nTSSingleRun			= 1;
							tATMP.nTSAutoDel			= 1;
							tATMP.nTSShow				= 1;
							tATMP.nTSShowMode			= SHOW_MODE_TYPE_FILE_COPY;
							tATMP.nTSWaitMode			= TS_EXCUTE_WAIT_MODE_EXIST_TASK;

							tATMP.strTSChildPath		= strExeName;
							tATMP.strTSChildArgument	= pdpdfiu->strFileCommand;
							tATMP.nTSChildWaitMode		= 1;	

							tATMP.strTSResultPath	= STR_REG_DEFAULT_SVC_LOCAL_PATH;						
							
							{
								String strChildArg, strCAFName;

								strChildArg = SPrintf("%s\f%s\f", (char *)strExeName.c_str(), (char *)strTarPath.c_str());
								strCAFName = SPrintf("%s/inven/cmd", (char *)t_EnvInfo->m_strRootPath.c_str());
								tFileUtil.ForceDirectory(strCAFName.c_str());
								strCAFName= SPrintf("%s/inven/cmd/%s", (char *)t_EnvInfo->m_strRootPath.c_str(), GetGUID().c_str());
								{
									tFileUtil.WriteFileBuffer(strCAFName, strChildArg);
									tATMP.strTSArgument = strCAFName;
								}
							}
						}

						switch(ExecuteFileByUser(tATI, tATMP))
						{
							case -1:	tDLDF.nEvtErrCode = ERR_ENV_STATUS_IS_NOT_LOG_ON;	break;
							case 0:
							{
								tDLDF.nEvtErrCode = ERR_SUCCESS;
								tDLDF.nEndTime = GetCurrentDateTimeInt();
								tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_COPY_END;
								break;
							}
							default:
							{
								tDLDF.nEvtErrCode = ERR_SYS_FILE_NOT_EXECUTE;
								break;
							}
						}
					}
					break;
				}
				case SS_PO_DEPLOY_FILE_FILE_TYPE_BATCH:
				{
					CFileUtil tFileUtil;
					CTokenString Token(strExeName.c_str(), strExeName.length(), '.', 1);
					String strExtension = Token.NextToken();
					String strBatchExeName, strCommand;

					if(!_stricmp(strExtension.c_str(), "bat"))
					{
						strBatchExeName = STR_PROC_NAME_CMD;
						strCommand = SPrintf(" /C \"%s\"", strExeName.c_str());
					}
					else if(!_stricmp(strExtension.c_str(), "reg"))
					{
						strBatchExeName = STR_PROC_NAME_REGEDIT;
						strCommand = SPrintf(" /S \"%s\"", strExeName.c_str());
					}
					else
					{
						strBatchExeName = strExeName;
					}
					
					if(pdpdfiu->nExeUserType == SS_PO_DEPLOY_FILE_EXE_USER_TYPE_SYSTEM)
					{
						if(m_tProcUtil.ProcessStart(strBatchExeName.c_str(), strCommand.c_str(), FALSE, TRUE, &nProID))
						{
							WriteLogN("[%s] execute batch end : [%s] ", m_strUtilName.c_str(), pdpdfiu->strFileOrName.c_str());
							tDLDF.nEvtErrCode = ERR_SUCCESS;
							tDLDF.nEndTime = GetCurrentDateTimeInt();
							tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_EXECUTE_BAT;
						}
						else
						{
							tDLDF.nEvtErrCode = ERR_SYS_FILE_NOT_EXECUTE;
						}
					}
					else if(pdpdfiu->nExeUserType == SS_PO_DEPLOY_FILE_EXE_USER_TYPE_USER)
					{
						ASI_TS_INFO tATI;
						ASI_TS_MGR_PARAM tATMP;
						{
							sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_DEPLOY_FILE, GetTickCount()); 
							tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
							tATI.nStartTime	= GetCurrentDateTimeInt() + 5;

							tATMP.nTSSingleRun			= 1;
							tATMP.nTSAutoDel			= 1;
							tATMP.nTSWaitMode			= TS_EXCUTE_WAIT_MODE_EXIST_TASK;
							tATMP.nTSChildWaitMode		= 1;

							tATMP.strTSChildPath		= strBatchExeName;
							tATMP.strTSChildArgument	= strCommand;

							tATMP.strTSResultPath	= STR_REG_DEFAULT_SVC_LOCAL_PATH;
						}

						switch(ExecuteFileByUser(tATI, tATMP))
						{
							case -1:	tDLDF.nEvtErrCode = ERR_ENV_STATUS_IS_NOT_LOG_ON;	break;
							case 0:
							{
								tDLDF.nEvtErrCode = ERR_SUCCESS;
								tDLDF.nEndTime = GetCurrentDateTimeInt();
								tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_EXECUTE_BAT;
								break;
							}
							default:
							{
								tDLDF.nEvtErrCode = ERR_SYS_FILE_NOT_EXECUTE;
								break;
							}
						}
					}
					break;
				}
				case SS_PO_DEPLOY_FILE_FILE_TYPE_NORMAL_EXE:
				{
					CFileUtil tFileUtil;
					String strTarPath = tFileUtil.GetEnvPathToLocalPathSingle(pdpdfiu->strTargetPath);
					{
						CTokenString Token(strTarPath.c_str(), strTarPath.length(), '/', 1);
						Token.NextToken();
						String strOnlyDir = Token.RemainValue();
						tFileUtil.ForceDirectory(strOnlyDir.c_str());
					}

					{
						if(tFileUtil.FileExists(strTarPath.c_str()) == FALSE || unlink(strTarPath.c_str()) == 0)
						{
							CopyFile(strExeName.c_str(), strTarPath.c_str(), TRUE);
						}
						else
						{
							String strTarPathTemp;
							strTarPathTemp= SPrintf("%s_ssa_%u", (char *)strTarPath.c_str(), GetTickCount());
							unlink(strTarPathTemp.c_str());
							CopyFile(strExeName.c_str(), strTarPathTemp.c_str(), FALSE);
							MoveFileEx((char *)strTarPathTemp.c_str(), (char *)strTarPath.c_str());

							strTarPath = strTarPathTemp;
						}

						WriteLogN("[%s] file move end : [%s] ", (char *)m_strUtilName.c_str(), (char *)pdpdfiu->strFileOrName.c_str());
					}
					
					if(pdpdfiu->nExeUserType == SS_PO_DEPLOY_FILE_EXE_USER_TYPE_SYSTEM)
					{
						if(m_tProcUtil.ProcessStart(strTarPath.c_str(), pdpdfiu->strFileCommand.c_str(), FALSE, FALSE, &nProID))
						{
							WriteLogN("[%s] execute process end : [%s] ", (char *)m_strUtilName.c_str(), (char *)pdpdfiu->strFileOrName.c_str());
							tDLDF.nEvtErrCode = ERR_SUCCESS;
							tDLDF.nEndTime = GetCurrentDateTimeInt();
							tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_EXECUTE;
						}
						else
						{
							tDLDF.nEvtErrCode = ERR_SYS_FILE_NOT_EXECUTE;
						}
					}
					else if(pdpdfiu->nExeUserType == SS_PO_DEPLOY_FILE_EXE_USER_TYPE_USER)
					{
						ASI_TS_INFO tATI;
						ASI_TS_MGR_PARAM tATMP;
						{
							sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_DEPLOY_FILE, GetTickCount()); 
							tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
							tATI.nStartTime	= GetCurrentDateTimeInt() + 5;

							tATMP.nTSSingleRun			= 1;
							tATMP.nTSAutoDel			= 1;
							tATMP.nTSWaitMode			= TS_EXCUTE_WAIT_MODE_EXIST_TASK;
							tATMP.nTSChildWaitMode		= 0;

							tATMP.strTSChildPath		= strTarPath;
							tATMP.strTSChildArgument	= pdpdfiu->strFileCommand;

							tATMP.strTSResultPath	= STR_REG_DEFAULT_SVC_LOCAL_PATH;
						}

						switch(ExecuteFileByUser(tATI, tATMP))
						{
							case -1:	tDLDF.nEvtErrCode = ERR_ENV_STATUS_IS_NOT_LOG_ON;	break;
							case 0:
							{
								tDLDF.nEvtErrCode = ERR_SUCCESS;
								tDLDF.nEndTime = GetCurrentDateTimeInt();
								tDLDF.nEndReason = SS_LOG_DEPLOY_FILE_END_REASON_TYPE_EXECUTE;
								break;
							}
							default:
							{
								tDLDF.nEvtErrCode = ERR_SYS_FILE_NOT_EXECUTE;
								break;
							}
						}
					}
					break;
				}
			}
		} while (FALSE);

		unlink(strExeName.c_str());

		PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp = t_ManagePoDeployFileUnitSchPkg->FindItem(nSchPkgID);
		if(pdpdfusp)
		{
			pdpdfusp->nScanTime = GetCurrentDateTimeInt();
			t_ManagePoDeployFileUnitSchPkg->EditPoDeployFileUnitSchPkg(*pdpdfusp);
			WriteLogN("[%s] execute deploy file success and record scan_time : [pid:%d][uid:%d][sid:%d]", (char *)m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID, nSchPkgID);
		}

		if(!pCurDLDF || pCurDLDF->nEndReason != tDLDF.nEndReason || pCurDLDF->nEndTime != tDLDF.nEndTime || 
			pCurDLDF->nEvtErrCode != tDLDF.nEvtErrCode ||
			!tDLDF.nEvtTime || (tDLDF.nEvtTime/TIMER_INTERVAL_TIME_DAY != GetCurrentDateTimeInt()/TIMER_INTERVAL_TIME_DAY))
		{
			tDLDF.nEvtTime	= t_ValidTimeUtil->GetValidTime();
//			HISYNCSTEPUP(tDLDF.nSyncSvrStep);
			t_LogicMgrLogDeployFile->SetLogDeployFile(tDLDF);
		}
	}

	t_ManageFileDown->ClearItemByEndDp(ASIFDL_DL_FILE_TYPE_DEPLOY, tIDNonDelList);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_WPtnGBL(INT32& nChgWPtn)
{
	INT32 nRtn = 0;
	TListID tIDList;
	TListIDItor begin, end;
	t_ManageFileDown->GetIDListByType(ASIFDL_DL_FILE_TYPE_WPTN_GBL, tIDList);

	if(tIDList.size() == 0)		return nRtn;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi)	continue;
		
		String strDwName, strSvName;
		
		do 
		{
			PDB_PO_FE_PTN_BL pdpfpb = (PDB_PO_FE_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_BL);
			if(!pdpfpb)
			{
				WriteLogE("[%s] not find fe ptn bl info : [%d][%d]", (char *)m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID);
				break;
			}		
			
			strDwName = SPrintf("%s/%s/%s", (char *)t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, (char *)pdpfpb->tDPFI.strSaveName.c_str());
			strSvName = SPrintf("%s/%s/%s", (char *)t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, (char *)pdpfpb->tDPFI.strSaveName.c_str());

			{
				String strPtnSvPath;
				strPtnSvPath= SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE);

				CFileUtil tFileUtil;
				if (tFileUtil.DirectoryExists(strPtnSvPath.c_str()))
				{
					if (!strPtnSvPath.empty())
					{
						DeletePrefixFiles(strPtnSvPath, "gb");
						//tFileUtil.RemoveFileAll(strPtnSvPath, _T("*.*"));
					}
				}
				else
					tFileUtil.ForceDirectory(strPtnSvPath.c_str());
			}

			if(CopyFile(strDwName.c_str(), strSvName.c_str(), TRUE) == FALSE)
			{
				unlink((char *)strDwName.c_str());
				WriteLogE("[%s] change fe ptn bl file name : [%s]", m_strUtilName.c_str(), pdpfpb->tDPFI.strSaveName.c_str());
				break;
			}

			if(ChkFileHash(strSvName, pdpfpb->tDPFI.strFileHash))
			{
				unlink((char *)strDwName.c_str());
				WriteLogE("[%s] The hash of the fe ptn bl download file is invalid : [%s][%s]", m_strUtilName.c_str(), strSvName.c_str(), pdpfpb->tDPFI.strFileHash.c_str());
				break;
			}
			nChgWPtn = 1;
			unlink((char *)strDwName.c_str());
		} while (FALSE);		
	}
	t_ManageFileDown->ClearItemByType(ASIFDL_DL_FILE_TYPE_WPTN_GBL);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_WPtnGWL(INT32& nChgWPtn)
{
	INT32 nRtn = 0;
	TListID tIDList;
	TListIDItor begin, end;
	t_ManageFileDown->GetIDListByType(ASIFDL_DL_FILE_TYPE_WPTN_GWL, tIDList);

	if(tIDList.size() == 0)		return nRtn;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi)	continue;

		String strDwName, strSvName;

		do 
		{
			PDB_PO_FE_PTN_WL pdpfpw = (PDB_PO_FE_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_PTN_WL);
			if(!pdpfpw)
			{
				WriteLogE("[%s] not find fe ptn wl info : [%d][%d]", m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID);
				break;
			}		

			strDwName= SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, pdpfpw->tDPFI.strSaveName.c_str());
			strSvName= SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdpfpw->tDPFI.strSaveName.c_str());

			{
				String strPtnSvPath;
				strPtnSvPath= SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE);

				CFileUtil tFileUtil;
				if (tFileUtil.DirectoryExists(strPtnSvPath.c_str()))
				{
					if (!strPtnSvPath.empty())
					{
						DeletePrefixFiles(strPtnSvPath, "gw");
						//tFileUtil.RemoveFileAll(strPtnSvPath, _T("*.*"));
					}
				}
				else
					tFileUtil.ForceDirectory(strPtnSvPath.c_str());
			}

			if(CopyFile(strDwName.c_str(), strSvName.c_str(), TRUE) == FALSE)
			{
				unlink((char *)strDwName.c_str());
				WriteLogE("[%s] change fe ptn wl file name : [%s]", m_strUtilName.c_str(), pdpfpw->tDPFI.strSaveName.c_str());
				break;
			}

			if(ChkFileHash(strSvName, pdpfpw->tDPFI.strFileHash))
			{
				unlink((char *)strDwName.c_str());
				WriteLogE("[%s] The hash of the fe ptn wl download file is invalid : [%s][%s]", m_strUtilName.c_str(), strSvName.c_str(), pdpfpw->tDPFI.strFileHash.c_str());
				break;
			}
			nChgWPtn = 1;
			unlink((char *)strDwName.c_str());
		} while (FALSE);		
	}
	t_ManageFileDown->ClearItemByType(ASIFDL_DL_FILE_TYPE_WPTN_GWL);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_WPtnGBO(INT32& nChgWPtn)
{
	INT32 nRtn = 0;
	TListID tIDList;
	TListIDItor begin, end;
	t_ManageFileDown->GetIDListByType(ASIFDL_DL_FILE_TYPE_WPTN_GBO, tIDList);

	if(tIDList.size() == 0)		return nRtn;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi)	continue;

		String strDwName, strSvName;

		do 
		{
			PDB_PO_IN_PTN_BL pdata = (PDB_PO_IN_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_BL);
			if(!pdata)
			{
				WriteLogE("[%s] not find in ptn bl info : [%d][%d]", m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID);
				break;
			}		

			strDwName= SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, pdata->tDPFI.strSaveName.c_str());
			strSvName= SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdata->tDPFI.strSaveName.c_str());

			{
				String strPtnSvPath;
				strPtnSvPath= SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE);

				CFileUtil tFileUtil;
				if (tFileUtil.DirectoryExists(strPtnSvPath.c_str()))
				{
					if (!strPtnSvPath.empty())
					{
						DeletePrefixFiles(strPtnSvPath, "gbo");
						//tFileUtil.RemoveFileAll(strPtnSvPath, _T("*.*"));
					}
				}
				else
					tFileUtil.ForceDirectory(strPtnSvPath.c_str());
			}

			if(CopyFile(strDwName.c_str(), strSvName.c_str(), TRUE) == FALSE)
			{
				unlink((char *)strDwName.c_str());
				WriteLogE("[%s] change in ptn bl file name : [%s]", m_strUtilName.c_str(), pdata->tDPFI.strSaveName.c_str());
				break;
			}

			if(ChkFileHash(strSvName, pdata->tDPFI.strFileHash))
			{
				unlink((char *)strDwName.c_str());
				WriteLogE("[%s] The hash of the in ptn bl download file is invalid : [%s][%s]", m_strUtilName.c_str(), strSvName.c_str(), pdata->tDPFI.strFileHash.c_str());
				break;
			}

			if(t_LogicMgrPtnGBO->ReloadPtnGBO())
			{
				WriteLogE("[%s] can not reload gbo ptn file  : [%s]", m_strUtilName.c_str(), strSvName.c_str());
				break;
			}

			nChgWPtn = 1;
			unlink((char *)strDwName.c_str());
		} while (FALSE);		
	}
	t_ManageFileDown->ClearItemByType(ASIFDL_DL_FILE_TYPE_WPTN_GBO);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_WPtnGWO(INT32& nChgWPtn)
{
	INT32 nRtn = 0;
	TListID tIDList;
	TListIDItor begin, end;
	t_ManageFileDown->GetIDListByType(ASIFDL_DL_FILE_TYPE_WPTN_GWO, tIDList);

	if(tIDList.size() == 0)		return nRtn;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi)	continue;

		String strDwName, strSvName;

		do 
		{
			PDB_PO_IN_PTN_WL pdata = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_WL);
			if(!pdata)
			{
				WriteLogE("[%s] not find in ptn wl info : [%d][%d]", m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID);
				break;
			}		

			strDwName= SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, pdata->tDPFI.strSaveName.c_str());
			strSvName= SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdata->tDPFI.strSaveName.c_str());

			{
				String strPtnSvPath;
				strPtnSvPath= SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE);

				CFileUtil tFileUtil;
				if (tFileUtil.DirectoryExists(strPtnSvPath.c_str()))
				{
					if (!strPtnSvPath.empty())
					{
						DeletePrefixFiles(strPtnSvPath, "gwo");
						//tFileUtil.RemoveFileAll(strPtnSvPath, _T("*.*"));
					}
				}
				else
					tFileUtil.ForceDirectory(strPtnSvPath.c_str());
			}

			if(CopyFile(strDwName.c_str(), strSvName.c_str(), TRUE) == FALSE)
			{
				unlink((char *)strDwName.c_str());
				WriteLogE("[%s] change in ptn wl file name : [%s]", (char *)m_strUtilName.c_str(), (char *)pdata->tDPFI.strSaveName.c_str());
				break;
			}

			if(ChkFileHash(strSvName, pdata->tDPFI.strFileHash))
			{
				unlink((char *)strDwName.c_str());
				WriteLogE("[%s] The hash of the in ptn wl download file is invalid : [%s][%s]", (char *)m_strUtilName.c_str(), (char *)strSvName.c_str(), (char *)pdata->tDPFI.strFileHash.c_str());
				break;
			}
			if(t_LogicMgrPtnGWO->ReloadPtnGWO())
			{
				WriteLogE("[%s] can not reload gwo ptn file  : [%s]", m_strUtilName.c_str(), strSvName.c_str());
				break;
			}

			nChgWPtn = 1;
			unlink((char *)strDwName.c_str());
		} while (FALSE);		
	}
	t_ManageFileDown->ClearItemByType(ASIFDL_DL_FILE_TYPE_WPTN_GWO);
	return nRtn;
}


INT32		CExecuteFileUtil::ExecuteFile_WPtnGBC(INT32& nChgWPtn)
{
	INT32 nRtn = 0;
/*
	TListID tIDList;
	TListIDItor begin, end;
	t_ManageFileDown->GetIDListByType(SS_DN_FILE_TYPE_WPTN_GBC, tIDList);

	if(tIDList.size() == 0)		return nRtn;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi)	continue;

		String strDwName, strSvName;

		do 
		{
			PDB_PO_NC_PTN_BL pdata = (PDB_PO_NC_PTN_BL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_BL);
			if(!pdata)
			{
				WriteLogE("[%s] not find nc ptn bl info : [%d][%d]", m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID);
				break;
			}		

			strDwName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, pdata->tDPFI.strSaveName.c_str());
			strSvName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdata->tDPFI.strSaveName.c_str());

			{
				String strPtnSvPath;
				strPtnSvPath = SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE);

				CFileUtil tFileUtil;
				if (tFileUtil.DirectoryExists(strPtnSvPath.c_str()))
				{
					if (!strPtnSvPath.empty() && !PathIsRoot(strPtnSvPath))		// #20181226 :: Work Ahnshy - Prevent Delete All Files From Root
					{
						DeletePrefixFiles(strPtnSvPath, "gbc");
					}
				}
				else
					tFileUtil.ForceDirectory(strPtnSvPath.c_str());
			}

			if(CopyFile(strDwName.c_str(), strSvName.c_str(), TRUE) == FALSE)
			{
				unlink(strDwName.c_str());
				WriteLogE("[%s] change nc ptn bl file name : [%s]", m_strUtilName.c_str(), pdata->tDPFI.strSaveName.c_str());
				break;
			}

			if(ChkFileHash(strSvName.c_str(), pdata->tDPFI.strFileHash.c_str()))
			{
				unlink(strDwName.c_str());
				WriteLogE("[%s] The hash of the nc ptn bl download file is invalid : [%s][%s]", m_strUtilName.c_str(), strSvName.c_str(), pdata->tDPFI.strFileHash.c_str());
				break;
			}

			if(t_LogicMgrPtnGBC->ReloadPtnGBC())
			{
				WriteLogE("[%s] can not reload gbc ptn file  : [%s]", m_strUtilName.c_str(), strSvName.c_str());
				break;
			}

			nChgWPtn = 1;
			unlink(strDwName.c_str());
		} while (FALSE);		
	}
	t_ManageFileDown->ClearItemByType(SS_DN_FILE_TYPE_WPTN_GBC);
*/
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFile_WPtnGWC(INT32& nChgWPtn)
{
	INT32 nRtn = 0;
/*
	TListID tIDList;
	TListIDItor begin, end;
	t_ManageFileDown->GetIDListByType(SS_DN_FILE_TYPE_WPTN_GWC, tIDList);

	if(tIDList.size() == 0)		return nRtn;

	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PASI_FDL_INFO pafi = t_ManageFileDown->FindItem(*begin);
		if(!pafi)	continue;

		String strDwName, strSvName;

		do 
		{
			PDB_PO_NC_PTN_WL pdata = (PDB_PO_NC_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_WL);
			if(!pdata)
			{
				WriteLogE("[%s] not find nc ptn wl info : [%d][%d]", m_strUtilName.c_str(), pafi->nPolicyID, pafi->nItemID);
				break;
			}		

			strDwName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_DEPLOY_FILE_LOCAL_DOWN, pdata->tDPFI.strSaveName.c_str());
			strSvName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdata->tDPFI.strSaveName.c_str());

			{
				String strPtnSvPath;
				strPtnSvPath = SPrintf("%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE);

				CFileUtil tFileUtil;
				if (tFileUtil.DirectoryExists(strPtnSvPath.c_str()))
				{
					if (!strPtnSvPath.empty() && !PathIsRoot(strPtnSvPath))		// #20181226 :: Work Ahnshy - Prevent Delete All Files From Root
					{
						DeletePrefixFiles(strPtnSvPath, "gwc");
					}
				}
				else
					tFileUtil.ForceDirectory(strPtnSvPath.c_str());
			}

			if(CopyFile(strDwName.c_str(), strSvName.c_str(), TRUE) == FALSE)
			{
				unlink(strDwName.c_str());
				WriteLogE("[%s] change nc ptn wl file name : [%s]", m_strUtilName.c_str(), pdata->tDPFI.strSaveName.c_str());
				break;
			}

			if(ChkFileHash(strSvName.c_str(), pdata->tDPFI.strFileHash.c_str()))
			{
				unlink(strDwName.c_str());
				WriteLogE("[%s] The hash of the nc ptn wl download file is invalid : [%s][%s]", m_strUtilName.c_str(), strSvName.c_str(), pdata->tDPFI.strFileHash.c_str());
				break;
			}

			if(t_LogicMgrPtnGWC->ReloadPtnGWC())
			{
				WriteLogE("[%s] can not reload gwc ptn file  : [%s]", m_strUtilName.c_str(), strSvName.c_str());
				break;
			}

			nChgWPtn = 1;
			unlink(strDwName.c_str());
		} while (FALSE);		
	}
	t_ManageFileDown->ClearItemByType(SS_DN_FILE_TYPE_WPTN_GWC);
*/
	return nRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ChkFileCertInfo(String strFileName)
{
/*	
	ASI_FILE_CERT_INFO tAFCI;
	strcpy_ext(tAFCI.szCheckFileName, strFileName.c_str(), MAX_PATH);
//	t_ASIFCIDLLUtil->ASIFCI_GetFileCertInfo(&tAFCI);

	if(tAFCI.szPubName[0] == 0 || tAFCI.szPubSN[0] == 0)
	{
		WriteLogE("[%s] not fine file cert information : [%s]", m_strUtilName.c_str(), strFileName.c_str());
		return -1;
	}

	WriteLogN("[%s] check file cert info : [%s][%s]:[%s]", m_strUtilName.c_str(), tAFCI.szPubName, tAFCI.szPubSN, strFileName.c_str());
*/
	return 0;
}

INT32		CExecuteFileUtil::ChkFileHash(String strFileName, String strFileHash)
{
	INT nHashLen = 0;
	char pszHashValue[CHAR_MAX_SIZE] = {0, };
	
	nHashLen = strFileHash.length();
	switch(nHashLen)
	{
		case ASIHASH_SHA_TYPE_128_LEN:
			SHAFile(ASIHASH_SHA_LEN_TYPE_128, strFileName.c_str(), pszHashValue, CHAR_MAX_SIZE-1);
			break;
		case ASIHASH_SHA_TYPE_256_LEN:
			SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFileName.c_str(), pszHashValue, CHAR_MAX_SIZE-1);
			break;
	}

	if(_stricmp(strFileHash.c_str(), pszHashValue))
	{
		WriteLogE("[%s] invalid hash (%s) (%d) [%s : %s]", (char *)m_strUtilName.c_str(), strFileName.c_str(), nHashLen, strFileHash.c_str(), pszHashValue);
		return -1;
	}

	return 0;
}

//---------------------------------------------------------------------------

INT32		CExecuteFileUtil::ExecuteFileByUser(ASI_TS_INFO& tATI, ASI_TS_MGR_PARAM& tATMP, UINT32 nWaitTime)
{
	INT32 nRtn = 0;
/*
	String strParam;
	String strTSExe, strTSDLL, strChildPath;
	CSystemInfo tSysInfo;


	if(tATI.nChkPeriod == ASI_TS_CHECK_PREIOD_TYPE_ONCE && tSysInfo.IsExistLoginSession() == 0)
	{
//		WriteLogN("[%s] invaild local current status : once and logoff : [%s]", m_strUtilName.c_str(), tATMP.strTSChildPath.c_str());
		return -1;
	}


	{
		strTSExe= SPrintf("%s/util/ts_mgr.exe", (char *)t_EnvInfo->m_strRootPath.c_str());
	}
	{
		strTSDLL= SPrintf("%s/asi_ts.so", (char *)t_EnvInfo->m_strDLLPath.c_str());
	}

	strChildPath = tATMP.strTSChildPath;
	
	{
		if(tATMP.nTSChildWaitMode && tATMP.strTSResultPath.empty() == FALSE)
		{
			tATMP.strTSResultValue = tATI.szTaskName;
		}
	}

	{
		strParam= SPrintf("|t_nm=1:%s|t_ad=0:%d|t_dl=1:%s"
			"|t_sr=0:%d|t_sh=0:%d|t_sm=0:%d|t_ar=1:%s"
			"|t_rp=1:%s|t_rv=1:%s"
			"|t_cf=1:%s"
			"|c_pt=1:%s"
			"|c_ag=1:%s"
			"|c_wc=0:%d|c_hd=0:%d", 
			tATI.szTaskName, tATMP.nTSAutoDel, strTSDLL.c_str(),
			tATMP.nTSSingleRun, tATMP.nTSShow, tATMP.nTSShowMode, tATMP.strTSArgument.c_str(),
			tATMP.strTSResultPath.c_str(), tATMP.strTSResultValue.c_str(),
			tATMP.strTSConfirmMsg.c_str(),
			strChildPath.c_str(), 
			tATMP.strTSChildArgument.c_str(),
			tATMP.nTSChildWaitMode, tATMP.nTSChildHideMode);

	}
	if(tATMP.nTSWaitMode)
	{
		UINT32 nExistMode = 0, nProID = 0;
		String strExeName;
		char szExeName[MAX_HBUFF] = {0,};

		if(tATMP.nTSWaitMode == TS_EXCUTE_WAIT_MODE_EXCUTE_PROC || tATMP.nTSWaitMode == TS_EXCUTE_WAIT_MODE_EXCUTE_PROC_CHILD)
		{
			do
			{
				nProID = 0;
				if(tATMP.nTSWaitMode == 3 && nProID)
				{

					if(get_basename((char *)tATMP.strTSChildPath.c_str(), szExeName, MAX_HBUFF-1) != NULL)
					{
						szExeName[MAX_HBUFF-1] = 0;
						strExeName = szExeName;
						nProID = m_tProcUtil.GetChildProcessID(nProID, strExeName);
					}
				}

				Sleep(1000);
				nWaitTime -= 1;
			}while(!nProID && t_EnvInfoOp->GetMainContinue() && nWaitTime);
		}
		else
		{
			do
			{
				nExistMode = 0;

				Sleep(1000);
				nWaitTime -= 1;
			}while(nExistMode && t_EnvInfoOp->GetMainContinue() && nWaitTime);
		}

		if(!nWaitTime)
		{
			return -2;
		}

		if(tATMP.strTSResultPath.empty() == FALSE)
		{
			CRegUtil tRegUtil;

			CHAR szRstValue[CHAR_MAX_SIZE] = {0, };
			if(tRegUtil.RegReadString((HKEY)HKEY_LOCAL_MACHINE, tATMP.strTSResultPath.c_str(), tATMP.strTSResultValue.c_str(), szRstValue) == ERR_SUCCESS)
			{
				CTokenString Token(szRstValue, strlen_ext(szRstValue), '\f');
				String strUserName = Token.NextToken();
				nRtn = Token.NextToken_Int();
				WriteLogN("[%s] execute process by user token [%s][%d] .. ", (char *)m_strUtilName.c_str(), (char *)strUserName.c_str(), nRtn);

				tRegUtil.RegDeleteValueExt((HKEY)HKEY_LOCAL_MACHINE, (char *)tATMP.strTSResultPath.c_str(), (char *)tATMP.strTSResultValue.c_str());
			}
		}
	}
*/
	return nRtn;
}
//---------------------------------------------------------------------------
