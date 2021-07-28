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

// MainRun.cpp: implementation of the CMainRun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadExecute.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadExecute
CThreadExecute*	t_ThreadExecute = NULL;

CThreadExecute::CThreadExecute()
{
	m_nLastID = 1;
}

CThreadExecute::~CThreadExecute()
{
}

BOOL CThreadExecute::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadExecute::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadExecute message handlers

int CThreadExecute::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName, m_nRunFlag);
/*	
	{
		m_tASICabDLLUtil.LoadLibraryExt();
	}
*/

	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
/*
		INT32 nNextID = 0;
		if(!m_nPause && (nNextID = IsNextWork()))
		{
			MEM_WORK_EXECUTE data;
			if(GetWork(nNextID, data))	continue;

			if(data.nType == G_TYPE_PTN_PATCH)
			{

#ifndef _DEBUG
				if(t_EnvInfo->GetReg_DbgEnv_SkipPM() == 0)
				{
					ExecuteWork(&data);
					if(data.nCode == SS_APPLY_PATCH_FILE_TYPE_CAB)
					{
						//WriteLogN("[%s] check execut process result : [%d][%s]", m_strThreadName, data.nErrCode, data.strLogFile);
						
						INT32 nExeSucc = 0;
						if(m_tSysInfo.IsLowOsVer(6, 0))		nExeSucc = m_tAPUtil.IsValidExecutePkgMgr(data.strLogFile);
						else								nExeSucc = m_tAPUtil.IsValidExecuteDISM(data.strLogFile);

						WriteLogN("[%s] check execut process result [%d] : [%d][%s]", m_strThreadName, nExeSucc, data.nErrCode, data.strLogFile);

						if(!nExeSucc)
						{
							ExecuteWorkForCab(&data);
						}
						else
						{
							data.nExeType = SS_APPLY_PATCH_FILE_TYPE_CAB;
						}
					}
					else
					{
						data.nExeType = data.nCode;
					}
				}
#else
				Sleep(10000);
#endif
			}
			else
			{
				ExecuteWork(&data);
			}

			data.nEndWork = 1;
			EditWork(nNextID, data);

			if(data.hNotifyHwnd && data.nNotifyID)
			{
				::PostMessage((HWND)(data.hNotifyHwnd), data.nNotifyID, (WPARAM)data.nType, (LPARAM)nNextID);
			}
			else
			{
				::PostMessage(t_EnvInfoOp->GetMainHandle(), WM_GLOBAL_THREAD_WORK_EXECUTE, (WPARAM)data.nType, (LPARAM)nNextID);	
			}
		}		
		else
		{
			Sleep(10);
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

INT32		CThreadExecute::ExecuteWork(PMEM_WORK_EXECUTE pdata)
{
	INT32 nRtn = 0;
/*
	CProcUtil tProcUtil;
	UINT32 nProcID = 0;
	
	LPCTSTR lpExe = NULL;
	if(pdata->strExecute.GetLength())	lpExe = pdata->strExecute;
	
	
	INT32 nExeRst = 0;

	switch(pdata->nExeSession)
	{
		case SS_PTN_SCAN_FILE_EXE_SESSION_SYSTEM:	nExeRst = tProcUtil.ProcessStart(lpExe, pdata->strCommand, false, false, &nProcID);		break;
		case SS_PTN_SCAN_FILE_EXE_SESSION_DESKTOP:	nExeRst = ExecuteProcDesktop(pdata);		break;
	}

	if(!nExeRst)
	{		
		pdata->nErrCode = GetLastError();
		WriteLogE("[%s] execut process fail : [%d] -- [%d][%s][%s]", m_strThreadName, pdata->nErrCode, pdata->nExeSession, pdata->strExecute, pdata->strCommand);
	}
	else
	{
		WriteLogN("[%s] execut process succ : [%d][%s][%s]", m_strThreadName, pdata->nErrCode, pdata->strExecute, pdata->strCommand);
		UINT32 nWaitTime = GetCurrentDateTimeInt();
		UINT32 nCurTime = 0;
		
		Sleep(1000);
		HANDLE mhStop = ::CreateEvent(0, TRUE, FALSE, 0);
		while(::WaitForSingleObject(mhStop, 100) != WAIT_OBJECT_0  && t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue()) 
		{
			if(nProcID && tProcUtil.ProcessExist(nProcID))
			{				
				nCurTime = GetCurrentDateTimeInt();
				if(nCurTime - nWaitTime > 60)
				{
					WriteLogN("[%s] wait process .. [%d]", m_strThreadName, nProcID);
					nWaitTime = nCurTime;
				}
			}
			else
			{
				SetEvent(mhStop);
			}
		}
		CloseHandle(mhStop);	

		WriteLogN("[%s] execut process end and continue wait child : [%d][%s][%s][%s]", m_strThreadName, pdata->nErrCode, pdata->strExecute, pdata->strCommand, (pdata->nWaitChild ? "Wait" : "Skip"));

		if(pdata->nWaitChild && (nProcID = tProcUtil.GetChildProcessID(nProcID)))
		{
			mhStop = ::CreateEvent(0, TRUE, FALSE, 0);
			while(::WaitForSingleObject(mhStop, 100) != WAIT_OBJECT_0  && t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue()) 
			{
				if(nProcID && tProcUtil.ProcessExist(nProcID))
				{				
					nCurTime = GetCurrentDateTimeInt();
					if(nCurTime - nWaitTime > 60)
					{
						WriteLogN("[%s] wait child process .. [%d]", m_strThreadName, nProcID);
						nWaitTime = nCurTime;
					}
				}
				else
				{
					SetEvent(mhStop);
				}
			}
			CloseHandle(mhStop);	
		}
	}
*/
	return nRtn;	
}
//---------------------------------------------------------------------------

INT32	CThreadExecute::ExecuteWorkForCab(PMEM_WORK_EXECUTE pdata)
{
/*
	WriteLogN("[%s] extract work for cab start : [%s]", m_strThreadName, pdata->strFileName); 

	CFileUtil tFileUtil;
	CHAR szTempDir[CHAR_MAX_SIZE] = {0, };
	GetTempPath(CHAR_MAX_SIZE, szTempDir);
	tFileUtil.DelLastChar(szTempDir, '\\');

	pdata->strWorkPath.Format("%s\\%u_%u", szTempDir, GetCurrentThreadId(), GetTickCount());
	tFileUtil.ForceDirectory(pdata->strWorkPath);

	if(m_tASICabDLLUtil.ASICAB_Extract(pdata->strFileName, pdata->strWorkPath))
	{
		WriteLogE("[%s] cabinet extract fail : [%s]", m_strThreadName, m_tASICabDLLUtil.GetLastError());
		return -1;
	}

	TListGatherFileInfo tFileNameList;
	tFileUtil.GetFileNameList(pdata->strWorkPath, "", "", tFileNameList, 0);

	{
		INT32 nIdx = 1;
		TListGatherFileInfoItor begin, end;
		begin = tFileNameList.begin();	end = tFileNameList.end();
		for(begin; begin != end; begin++)
		{
			MEM_WORK_EXECUTE data = *pdata;
			if(m_tAPUtil.MakeExecuteWork(data, begin->strFullPath, ""))		continue;
	
			WriteLogN("[%s] execute cabinet inner patch : [%d][%s]", m_strThreadName, nIdx++, begin->strFullPath);
			ExecuteWork(&data);
			if(data.nErrCode)
			{
				pdata->nErrCode = data.nErrCode;
				break;
			}

			pdata->strLogFile = data.strLogFile;
			pdata->nExeType = data.nCode;
		}
	}

	tFileUtil.RemoveDirectoryAll(pdata->strWorkPath);
*/
	return 0;
}
//---------------------------------------------------------------------------

INT32	CThreadExecute::ExecuteProcDesktop(PMEM_WORK_EXECUTE pdata)
{
	INT32 nRtn = 0;
/*
	CSystemInfo tSysInfo;
	ASI_TS_INFO tATI;

	String strExe = pdata->strExecute;
	String strCmd = pdata->strCommand;
	
	LPCTSTR lpExe = NULL;
	if(strExe.length())
		lpExe = strExe.c_str();

	if(lpExe)
	{
		sprintf_ext(CHAR_MAX_SIZE, tATI.szProgram, "%s", lpExe);
		sprintf_ext(CHAR_MAX_SIZE, tATI.szProParam, " %s", strCmd.c_str());
	}
	else
	{
		sprintf_ext(CHAR_MAX_SIZE, tATI.szProParam, "%s", strCmd.c_str());
	}
	sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_PTN_PATCH, pdata->nItemID, GetTickCount());
	sprintf_ext(CHAR_MAX_SIZE, tATI.szAcntName, "INTERACTIVE");

	WriteLogN("[%s] execute process desktop : [%s][%s]", m_strThreadName.c_str(), tATI.szProgram, tATI.szProParam);

	if(tSysInfo.IsExistLoginSession())
	{
		ASI_TS_MGR_PARAM tATMP;
		{
			tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
			tATI.nStartTime	= GetCurrentDateTimeInt() + 5;

			tATMP.strTSChildPath		= tATI.szProgram;
			tATMP.strTSChildArgument	= tATI.szProParam;

			tATMP.nTSSingleRun			= 1;
			tATMP.nTSAutoDel			= 1;
			tATMP.nTSWaitMode			= 1;
			tATMP.nTSChildWaitMode		= 1;
		}		
		nRtn = t_ExecuteFileUtil->ExecuteFileByUser(tATI, tATMP);
		t_ASITSDLLUtil->DelTaskInfo(tATI.szTaskName);
	}
*/
	return !nRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CThreadExecute::AddWork(PMEM_WORK_EXECUTE pdata)
{
	m_tMutex.Lock();
	pdata->nID = m_nLastID;

	m_tWorkMap[m_nLastID++] = *pdata;
	m_tMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CThreadExecute::EditWork(UINT32 nID, MEM_WORK_EXECUTE& data)
{
	INT32 nRtn = -1;
	m_tMutex.Lock();
	do 
	{
		TMapMemWorkExecuteItor find = m_tWorkMap.find(nID);
		if(find == m_tWorkMap.end())		break;

		m_tWorkMap[data.nID] = data;
		nRtn = 0;

	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------

INT32					CThreadExecute::DelWork(UINT32 nID)
{
	m_tMutex.Lock();
	do 
	{
		TMapMemWorkExecuteItor find = m_tWorkMap.find(nID);
		if(find == m_tWorkMap.end())		break;

		m_tWorkMap.erase(find);

	} while (FALSE);
	
	m_tMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CThreadExecute::GetWork(UINT32 nID, MEM_WORK_EXECUTE& data)
{
	INT32 nRtn = -1;
	m_tMutex.Lock();
	do 
	{
		TMapMemWorkExecuteItor find = m_tWorkMap.find(nID);
		if(find == m_tWorkMap.end())		break;

		data = (find->second);
		nRtn = 0;

	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------

INT32					CThreadExecute::IsNextWork()
{
	INT32 nRtn = 0;
	m_tMutex.Lock();
	do 
	{
		TMapMemWorkExecuteItor begin, end;
		begin = m_tWorkMap.begin();	end = m_tWorkMap.end();
		for(begin; begin != end; begin++)
		{
			if(begin->second.nEndWork == 0)
			{
				nRtn = begin->first;
				break;
			}
		}
	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------

INT32					CThreadExecute::IsRemainWork(UINT32 nType, UINT32 nSkipID)
{
	INT32 nRtn = 0;
	m_tMutex.Lock();
	do 
	{
		TMapMemWorkExecuteItor begin, end;
		begin = m_tWorkMap.begin();	end = m_tWorkMap.end();
		for(begin; begin != end; begin++)
		{
			if(nType && begin->second.nType != nType)	continue;
			if(begin->first == nSkipID)					continue;

			{
				nRtn += 1;
				break;
			}
		}
	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------

INT32					CThreadExecute::IsExistWork(MEM_WORK_EXECUTE& data)
{
	INT32 nRtn = 0;
	m_tMutex.Lock();
	do 
	{
		TMapMemWorkExecuteItor begin, end;
		begin = m_tWorkMap.begin();	end = m_tWorkMap.end();
		for(begin; begin != end; begin++)
		{
			if(begin->second.nType != data.nType)		continue;
			if(begin->second.nCode != data.nCode)		continue;
			if(begin->second.nItemID != data.nItemID)	continue;

			{
				nRtn = begin->first;
				break;
			}
		}
	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------