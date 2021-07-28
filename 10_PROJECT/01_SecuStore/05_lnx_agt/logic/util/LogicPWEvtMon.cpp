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
#include "LogicPWEvtMon.h"

//---------------------------------------------------------------------------

CLogicPWEvtMon*	t_LogicPWEvtMon = NULL;

CLogicPWEvtMon::CLogicPWEvtMon()
{
	m_strLogicName		= "logic event handler";
}
//---------------------------------------------------------------------------

CLogicPWEvtMon::~CLogicPWEvtMon()
{
}
//---------------------------------------------------------------------------

INT32		CLogicPWEvtMon::ApplyPolicy()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicPWEvtMon::OnTimer_Logic()
{
	CheckFastBoot();
	ExecuteEventHandler();
	RecordTimeStamp();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicPWEvtMon::CheckFastBoot()
{
	t_EnvInfo->GetReg_LastOffInfo();

	//WriteLogN("check fastboot : [%d][%d]", t_EnvInfo->m_nLastOffType, t_EnvInfo->m_nLastOffTime);

	if(t_EnvInfo->m_nLastOffType != ASI_BOOT_TYPE_FASTSTARTUP || 
		abs(INT32(GetCurrentDateTimeInt() - t_EnvInfo->m_nLastOffTime)) < 60 ||
		(GetTickCount() / 1000) < TIMER_INTERVAL_TIME_SYS_BOOT)
	{
		return 0;
	}

	WriteLogN("check fastboot : [%d][%d]", t_EnvInfo->m_nLastOffType, t_EnvInfo->m_nLastOffTime);
	{
		t_EnvInfo->SetReg_LastOffInfo(ASI_BOOT_TYPE_NORMAL);
		RunBootTimeOp(ASI_BOOT_TYPE_FASTSTARTUP);		
	}

	{
		t_EnvInfoOp->SetSysBootChkMode(1, GetCurrentDateTimeInt());
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_SYS_BOOT_OPERATION_END);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicPWEvtMon::RunBootTimeOp(UINT32 nBootType)
{
	if(nBootType == ASI_BOOT_TYPE_NORMAL)
	{
		t_LogicMgrPoWinRun->DumpMBR();
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicPWEvtMon::InitEnv()
{
	INT32 nContinue = 0;
	CSystemInfo tSystemInfo;
	CProcUtil tProcUtil;
	switch(tSystemInfo.GetSystemID())
	{
		case ASI_SYSTEM_ID_VISTA:
		case ASI_SYSTEM_ID_7:
		case ASI_SYSTEM_ID_8:
		case ASI_SYSTEM_ID_81:
		case ASI_SYSTEM_ID_10:	nContinue = 1;	break;
	}

	if(!nContinue)	return 0;

	ExecuteEventHandler();
	return 0;
}
//---------------------------------------------------------------------------
INT32		CLogicPWEvtMon::ExecuteEventHandler()
{
	CProcUtil tProcUtil;
	if(tProcUtil.ProcessExist("pwevt_mon.exe") == true || tProcUtil.ProcessExist("pwevt_~1.exe") == true)
		return 0;

	{

		ASI_TS_INFO tATI;
		ASI_TS_MGR_PARAM tATMP;
		{
			strncpy(tATI.szTaskName, STR_TS_NAME_PWEVT_MON, CHAR_MAX_SIZE-1);			
			tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_USER_LOGIN;
			tATI.nStartTime	= GetCurrentDateTimeInt();

			tATMP.strTSChildPath = SPrintf("%s/util/pwevt_mon.exe", t_EnvInfo->m_strRootPath.c_str());
			tATMP.nTSSingleRun		= 1;
			tATMP.nTSChildHideMode	= 1;
		}		
		t_ExecuteFileUtil->ExecuteFileByUser(tATI, tATMP);
	}


	if(tProcUtil.ProcessExist("pwevt_mon.exe") == false && tProcUtil.ProcessExist("pwevt_~1.exe") == false)
	{
		String strChildPath;

		{
			CHAR szShortPath[CHAR_MAX_SIZE] = {0, };
			strChildPath = SPrintf("%s/util/pwevt_mon.exe", t_EnvInfo->m_strRootPath.c_str());
//			GetShortPathName(strChildPath, szShortPath, MAX_PATH);
			strChildPath = szShortPath;
		}

		ASI_TS_INFO tATI;
		ASI_TS_MGR_PARAM tATMP;
		{
			sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_PWEVT_MON_ONCE, GetTickCount()); 
			tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
			tATI.nStartTime	= GetCurrentDateTimeInt() + 5;
			tATI.nMultipleInst = ASI_TS_MULTIPLE_INST_TYPE_PARALLEL;

			tATMP.strTSChildPath	= strChildPath;
			tATMP.nTSSingleRun		= 1;
			tATMP.nTSAutoDel		= 1;
			tATMP.nTSChildHideMode	= 1;
		}
		t_ExecuteFileUtil->ExecuteFileByUser(tATI, tATMP);
	}

	return 0;
}
//---------------------------------------------------------------------------
INT32		CLogicPWEvtMon::RecordTimeStamp()
{
	DWORD dwTimeStamp = 0;
	PDB_PO_HOST_RUN pPolicy = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_RUN);
	if(pPolicy)
	{
		dwTimeStamp = (pPolicy->nLnxRunOption & SS_PO_HOST_RUN_OPTION_FLAG_SUPPORT_RESTART_SVC ? GetTickCount() : 0);
	}
	t_EnvInfo->SetReg_TimeStamp(dwTimeStamp);
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicPWEvtMon::IsNotExcutePwevt()
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------