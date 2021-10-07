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

// ThreadChkHkNoti.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "ThreadChkHkNoti.h"


/////////////////////////////////////////////////////////////////////////////
// CThreadChkHkNoti

CThreadChkHkNoti*	t_ThreadChkHkNoti = NULL;

CThreadChkHkNoti::CThreadChkHkNoti()
{	
	//m_tMemFileExitWin.SetUsedMutex();
}

CThreadChkHkNoti::~CThreadChkHkNoti()
{
}

BOOL CThreadChkHkNoti::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadChkHkNoti::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadChkHkNoti message handlers

int CThreadChkHkNoti::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);
	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{	
			m_tMemMutex.Lock();
			INT32 nChkRtn = ChkMemFileList();
			m_tMemMutex.UnLock();

			switch(nChkRtn)
			{
				case 0:
					m_nPause = 1;
					break;
				case 1:
					Sleep(1000);
					break;
			}
		}	
		else
		{
			Sleep(10);
		}
	}

	if(!t_EnvInfoOp)
		WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())
		WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())
		WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadChkHkNoti::InitMemFile_ExitWin(INT32 nMode)
{
	m_tMemMutex.Lock();
	
	switch(nMode)
	{
		case 0:
		{
			WriteLogN("[%s] close mem file. : exit_win", m_strThreadName.c_str());	
			m_tMemFileExitWin.CloseMemFile();
			break;
		}
		case 1:
		{
			if(m_tMemFileExitWin.CreateMemFile(ASI_HK_NAME_EXITWIN, sizeof(ASI_HK_EXIT_WIN)) == 0)
			{
				PASI_HK_EXIT_WIN pAHEW = (PASI_HK_EXIT_WIN)m_tMemFileExitWin.MapViewFile();
				if(pAHEW)
				{
					memset(pAHEW, 0, sizeof(ASI_HK_EXIT_WIN));

					WriteLogN("[%s] create mem file succ. :exit_win", m_strThreadName.c_str());	
					m_tMemFileExitWin.UnmapViewFile();
				}
				else
				{
					WriteLogE("[%s] mem file fail [%d]", m_strThreadName.c_str(), errno);
				}
			}
			else
			{
				WriteLogE("[%s] create notify handle mem file fail [%d]", m_strThreadName.c_str(), errno);
			}
			break;
		}
		case 2:
		{
			PASI_HK_EXIT_WIN pAHEW = (PASI_HK_EXIT_WIN)m_tMemFileExitWin.MapViewFile();
			if(!pAHEW)
			{
				WriteLogN("[%s] skip mem file fail. :exit_win", m_strThreadName.c_str());	
				m_tMemFileExitWin.UnmapViewFile();
			}
			else
			{
				pAHEW->nChkFlag |= ASI_HK_EXIT_WIN_CHK_TYPE_SKIP;
				m_tMemFileExitWin.UnmapViewFile();
				WriteLogN("[%s] skip mem file succ. :exit_win", m_strThreadName.c_str());	
			}
			break;
		}
		case 3:
		{
			PASI_HK_EXIT_WIN pAHEW = (PASI_HK_EXIT_WIN)m_tMemFileExitWin.MapViewFile();
			if(!pAHEW)
			{
				WriteLogN("[%s] default mem file fail. :exit_win", m_strThreadName.c_str());	
				m_tMemFileExitWin.UnmapViewFile();
			}
			else
			{
				memset(pAHEW, 0, sizeof(ASI_HK_EXIT_WIN));
				m_tMemFileExitWin.UnmapViewFile();
				WriteLogN("[%s] default mem file succ. :exit_win", m_strThreadName.c_str());	
			}
			break;
		}
		case 4:
		{
			PASI_HK_EXIT_WIN pAHEW = (PASI_HK_EXIT_WIN)m_tMemFileExitWin.MapViewFile();
			if(!pAHEW)
			{
				WriteLogN("[%s] set wait mem file fail. :power_off by policy", m_strThreadName.c_str());	
				m_tMemFileExitWin.UnmapViewFile();
			}
			else
			{
				pAHEW->nChkFlag = ASI_HK_EXIT_WIN_CHK_TYPE_WAIT;
				pAHEW->nFlags	= EWX_POWEROFF;
				m_tMemFileExitWin.UnmapViewFile();
				WriteLogN("[%s] set wait mem file succ. :power_off by policy", m_strThreadName.c_str());	
			}
			break;
		}
		case 5:
		{
			PASI_HK_EXIT_WIN pAHEW = (PASI_HK_EXIT_WIN)m_tMemFileExitWin.MapViewFile();
			if(!pAHEW)
			{
				WriteLogN("[%s] set wait mem file fail. :reboot by policy", m_strThreadName.c_str());	
				m_tMemFileExitWin.UnmapViewFile();
			}
			else
			{
				pAHEW->nChkFlag = ASI_HK_EXIT_WIN_CHK_TYPE_WAIT;
				pAHEW->nFlags	= EWX_REBOOT;
				m_tMemFileExitWin.UnmapViewFile();
				WriteLogN("[%s] set wait mem file succ. :reboot by policy", m_strThreadName.c_str());	
			}
			break;
		}
	}	

	m_tMemMutex.UnLock();
	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadChkHkNoti::SetMemFile_ExitWin(String strFilePath, String strFileName, INT32 nFileOp)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadChkHkNoti::SetMemFile_ExitWinProgress(UINT32 nSchTotal, UINT32 nSchEnd, UINT32 nFindTotal, UINT32 nFindEnd)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadChkHkNoti::ChkMemFileList()
{
	do
	{
		PASI_HK_EXIT_WIN pAHEW = (PASI_HK_EXIT_WIN)m_tMemFileExitWin.MapViewFile();
		if(!pAHEW || pAHEW->nChkFlag != ASI_HK_EXIT_WIN_CHK_TYPE_WAIT)
		{
			m_tMemFileExitWin.UnmapViewFile();
			break;
		}

		pAHEW->nChkRtn	= ASI_HK_EXIT_WIN_RTN_TYPE_DENY;
		pAHEW->nChkFlag |= ASI_HK_EXIT_WIN_CHK_TYPE_DONE;
		if(t_EnvInfoOp->IsSysOffMode() == 0)
		{
			t_EnvInfoOp->SetSysOffMode(1, pAHEW->nFlags, pAHEW->dwReason);

			WriteLogN("start sys log off operation by exitwindow..[%x][%x]", pAHEW->nFlags, pAHEW->dwReason);

			t_EnvInfoOp->SetStopOpBySysOff(1);
			WriteLogN("set stop operation flag by exitwindow..[%x][%x]", pAHEW->nFlags, pAHEW->dwReason);

			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_SYS_OFF_OPERATION_END);

//			if(t_ThreadSubTimer)	t_ThreadSubTimer->t_TimerUtil.EnableTimer(TIMER_ID_SYS_OFF_OPERATION_CHK_WORK_TIME);

			t_EnvInfoOp->m_nApplyPolicyStatus = STATUS_USED_MODE_OFF;
		}
		m_tMemFileExitWin.UnmapViewFile();

	}while(FALSE);

	return 1;
}
//---------------------------------------------------------------------------

INT32		CThreadChkHkNoti::RunLockProcess() 
{
//	CSystemInfo tSysInfo;
	INT32 nChkTS = 1;
	UINT64 nSystemID = 0;
	if(t_SystemInfo != NULL)
		nSystemID = t_SystemInfo->GetSystemID();
	if(nSystemID == ASI_SYSTEM_ID_7)
	{
		nChkTS = 0;
		CProcUtil tProcUtil;
		String strProcName;
		strProcName = SPrintf("%s/util/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PROC_NAME_NANNY_DTLOCK);
		if(tProcUtil.ProcessStartAsUser(strProcName.c_str(), NULL) == FALSE)
		{
			nChkTS = 1;
		}
	}
	else
	{
		t_EnvInfo->SetReg_LastOffHookInfo();
		nChkTS = 0;
	}

	if(nChkTS)
	{
		ASI_TS_INFO tATI;
		tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_USER_LOGIN;
		tATI.nStartTime	= GetCurrentDateTimeInt();

		sprintf_ext(CHAR_MAX_SIZE, tATI.szProgram, "%s/util/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PROC_NAME_NANNY_DTLOCK);
		sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_DTLOCK_ONCE, GetTickCount());
		sprintf_ext(CHAR_MAX_SIZE, tATI.szAcntName, "INTERACTIVE");

		{
			ASI_TS_MGR_PARAM tATMP;
			{
				tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
				tATI.nStartTime	= GetCurrentDateTimeInt() + 2;

				tATMP.strTSChildPath		= tATI.szProgram;
				tATMP.strTSChildArgument	= tATI.szProParam;

				tATMP.nTSSingleRun			= 1;
				tATMP.nTSAutoDel			= 1;
				tATMP.nTSChildHideMode		= 0;
				tATMP.nTSChildWaitMode		= 0;
			}		
			t_ExecuteFileUtil->ExecuteFileByUser(tATI, tATMP);
		}
	}
	
	return 0;
}
//---------------------------------------------------------------------------
