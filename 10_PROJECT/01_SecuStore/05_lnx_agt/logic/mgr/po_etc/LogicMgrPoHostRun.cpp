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
#include "LogicMgrPoHostRun.h"

//---------------------------------------------------------------------------

CLogicMgrPoHostRun*		t_LogicMgrPoHostRun = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoHostRun::CLogicMgrPoHostRun()
{
	t_ManagePoHostRun	= new CManagePoHostRun();
	t_ManagePoHostRun->LoadDBMS();
	t_ManagePoHostRun->InitHash();

	m_strLogicName		= "mgr po host run";
	
	m_nPolicyType		= SS_POLICY_TYPE_HOST_RUN;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_HOST_RUN;
}
//---------------------------------------------------------------------------

CLogicMgrPoHostRun::~CLogicMgrPoHostRun()
{
	SAFE_DELETE(t_ManagePoHostRun);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
INT32		CLogicMgrPoHostRun::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostRun::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_HOST_RUN pdphr = NULL;
	DB_PO_HOST_RUN dphr;
	m_tDPH = &(dphr.tDPH);

	if( t_ManagePoHostRun->GetPkt(RecvToken, dphr))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	pdphr = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdphr)
	{
		t_ManagePoHostRun->DelPoHostRun(pdphr->tDPH.nID);
	}
/*
	{
		t_ManagePoHostRun->ClearPoHostRun();
	}
*/

	{
		if(SetER(t_ManagePoHostRun->ApplyPoHostRun(dphr)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
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

INT32		CLogicMgrPoHostRun::ApplyPolicy()
{
	CProcUtil tProcUtil;
//	CSystemInfo tSysInfo;
	INT32 nRetVal = 0;
	
	PDB_PO_HOST_RUN pdphr = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdphr)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		t_ThreadTimer->t_TimerUtil.EditTimer(TIMER_ID_LOGIN_SVR_AUTH, TIMER_INTERVAL_TIME_SEC*pdphr->nLnxLgnConPeriod, NULL, 1, 0, 0);
	}
/*
	{
		switch(pdphr->nMgrShowType)
		{
			case SS_PO_HOST_RUN_MGR_SHOW_TYPE_NOT_USED:
			{
				t_ASITSDLLUtil->DelTaskInfo(STR_TS_NAME_NANNY_MGR);
				tProcUtil.ProcessStop(STR_PROC_NAME_NANNY_MGR);
				break;
			}
			case SS_PO_HOST_RUN_MGR_SHOW_TYPE_NORMAL:
			{
				ASI_TS_INFO tATI;
				tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_USER_LOGIN;
				tATI.nStartTime	= GetCurrentDateTimeInt();

				sprintf_ext(CHAR_MAX_SIZE, tATI.szProgram, "%s/%s", t_EnvInfo->m_strRootPath, STR_PROC_NAME_NANNY_MGR);
				sprintf_ext(CHAR_MAX_SIZE, tATI.szProParam, " /show_type=0:1/", t_EnvInfo->m_strRootPath);
				sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, "%s", STR_TS_NAME_NANNY_MGR);
				sprintf_ext(CHAR_MAX_SIZE, tATI.szAcntName, "INTERACTIVE");
				
				t_ASITSDLLUtil->DelTaskInfo(STR_TS_NAME_NANNY_MGR);
				t_ASITSDLLUtil->AddTaskInfo(&tATI);
				
				if(tSysInfo.IsExistLoginSession() && tProcUtil.ProcessExist(STR_PROC_NAME_NANNY_MGR) == 0)
				{
					ASI_TS_MGR_PARAM tATMP;
					{
						sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_NANNY_MGR_ONCE, GetTickCount()); 
						tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
						tATI.nStartTime	= GetCurrentDateTimeInt() + 5;

						tATMP.strTSChildPath		= tATI.szProgram;
						tATMP.strTSChildArgument	= tATI.szProParam;

						tATMP.nTSSingleRun			= 1;
						tATMP.nTSAutoDel			= 1;
						tATMP.nTSChildHideMode		= 0;
						tATMP.nTSChildWaitMode		= 0;
					}		
					t_ExecuteFileUtil->ExecuteFileByUser(tATI, tATMP);
				}
				break;
			}
			case SS_PO_HOST_RUN_MGR_SHOW_TYPE_HIDE_START:
			{
				ASI_TS_INFO tATI;
				tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_USER_LOGIN;
				tATI.nStartTime	= GetCurrentDateTimeInt();

				sprintf_ext(CHAR_MAX_SIZE, tATI.szProgram, "%s/%s", t_EnvInfo->m_strRootPath, STR_PROC_NAME_NANNY_MGR);
				sprintf_ext(CHAR_MAX_SIZE, tATI.szProParam, " /show_type=0:0/", t_EnvInfo->m_strRootPath);
				sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, "%s", STR_TS_NAME_NANNY_MGR);
				sprintf_ext(CHAR_MAX_SIZE, tATI.szAcntName, "INTERACTIVE");

				t_ASITSDLLUtil->DelTaskInfo(STR_TS_NAME_NANNY_MGR);
				t_ASITSDLLUtil->AddTaskInfo(&tATI);		
				{
					ASI_TS_MGR_PARAM tATMP;
					{
						sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_NANNY_MGR_ONCE, GetTickCount()); 
						tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
						tATI.nStartTime	= GetCurrentDateTimeInt() + 5;

						tATMP.strTSChildPath		= tATI.szProgram;
						tATMP.strTSChildArgument	= tATI.szProParam;

						tATMP.nTSSingleRun			= 1;
						tATMP.nTSAutoDel			= 1;
						tATMP.nTSChildHideMode		= 0;
						tATMP.nTSChildWaitMode		= 0;
					}		
					t_ExecuteFileUtil->ExecuteFileByUser(tATI, tATMP);
				}
				break;
			}
		}
		WriteLogN("[%s] set show mgr mode : [%d]", m_strLogicName.c_str(), pdphr->nMgrShowType);
	}
*/

	{
		nRetVal = t_RemoveLogDLLUtil->RemoveFileByDT(t_EnvInfo->m_strLogPath.c_str(), FILE_DT_CHK_TYPE_CREATE, pdphr->nLnxRmLogDay);
		WriteLogN("[%s] cmd set remove log day : [%d], ret : [%d]", m_strLogicName.c_str(), pdphr->nLnxRmLogDay, nRetVal);
	}

	{
		if(pdphr->nLnxRunOption & SS_PO_HOST_RUN_OPTION_FLAG_SUPPORT_RESTART_SVC)
		{
			t_EnvInfo->SetReg_WatchDocMode(TRUE);
		}
		else
		{
			t_EnvInfo->SetReg_WatchDocMode(FALSE);
		}
		WriteLogN("[%s] set watchdog mode : [%x]", m_strLogicName.c_str(), (pdphr->nLnxRunOption & SS_PO_HOST_RUN_OPTION_FLAG_SUPPORT_RESTART_SVC));
	}

	{
		UINT32 nDLMethod = ASIFDL_DL_SVR_TYPE_MINIMUN;
		if(pdphr->nLnxRunOption & SS_PO_HOST_RUN_OPTION_FLAG_SUPPORT_PTOP)
		{
			nDLMethod += ASIFDL_DL_SVR_TYPE_PTOP;
		}
		if(pdphr->nLnxRunOption & SS_PO_HOST_RUN_OPTION_FLAG_SUPPORT_ASI_DEPLOY_SVR)
		{
			nDLMethod += ASIFDL_DL_SVR_TYPE_PUBLIC;
		}
/*
		if(t_ASIFDLDLLUtil)
			t_ASIFDLDLLUtil->SetDLSvrType(nDLMethod);
*/
		SetDLSvrType(nDLMethod);
		WriteLogN("[%s] set file download mode : [%x]", m_strLogicName.c_str(), nDLMethod);
	}

	{
		WriteLogN("[%s] apply policy time is [%s]", m_strLogicName.c_str(), (pdphr->nLnxRunOption & SS_PO_HOST_RUN_OPTION_FLAG_SUPPORT_BOOT_APPLY ? "system boot" : "after auth"));
	}

	return 0;
}
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

INT32		CLogicMgrPoHostRun::OnTimer_Logic()
{
	PDB_PO_HOST_RUN pdphr = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdphr)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	OnTimer_Logic_RmLog(pdphr);
	OnTimer_Logic_Setting(pdphr);
	return 0;	
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostRun::OnTimer_Logic_RmLog(PDB_PO_HOST_RUN pdphr)
{
	UINT32 nLastChkTime = 0;
	INT32 nRemoveCnt = 0;
	INT32 nRetVal = 0;
	U64_SCHEDULE tIS;
	tIS.U8.type	= SCHEDULE_PERIOD_TYPE_DAY;
	tIS.U8.hour	= 0;
	tIS.U8.min	= 0;
	

	if(IsValidSchedule(tIS.all, nLastChkTime) == 0)
	{
		return -1;
	}

	nRetVal = t_RemoveLogDLLUtil->RemoveFileByDT(t_EnvInfo->m_strLogPath.c_str(), FILE_DT_CHK_TYPE_CREATE, pdphr->nLnxRmLogDay);
	WriteLogN("[%s] timer set remove log day : [%d], ret : [%d]", m_strLogicName.c_str(), pdphr->nLnxRmLogDay, nRetVal);

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostRun::OnTimer_Logic_Setting(PDB_PO_HOST_RUN pdphr)
{
/*
	INT32 nReStartMgr = 0;
	INT32 nShowType   = 0;
	{

		switch(pdphr->nMgrShowType)
		{
			case SS_PO_HOST_RUN_MGR_SHOW_TYPE_NOT_USED:
			{
				break;
			}
			case SS_PO_HOST_RUN_MGR_SHOW_TYPE_NORMAL:
				nShowType = 1;
			case SS_PO_HOST_RUN_MGR_SHOW_TYPE_HIDE_START:
			{
				CProcUtil tProcUtil;
				if(tProcUtil.ProcessExist(STR_PROC_NAME_NANNY_MGR) == 0)
				{
					nReStartMgr = 1;
				}
				break;
			}
		}		

		if(nReStartMgr)
		{
			WriteLogN("[%s] re start nanny mgr process : [%d]", m_strLogicName.c_str(), pdphr->nMgrShowType);
			
			CSystemInfo tSysInfo;
			CProcUtil tProcUtil;

			ASI_TS_INFO tATI;
			tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_USER_LOGIN;
			tATI.nStartTime	= GetCurrentDateTimeInt();

			sprintf_ext(CHAR_MAX_SIZE, tATI.szProgram, "%s/%s", t_EnvInfo->m_strRootPath, STR_PROC_NAME_NANNY_MGR);
			sprintf_ext(CHAR_MAX_SIZE, tATI.szProParam, " /show_type=0:%d/", nShowType);
			sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, "%s", STR_TS_NAME_NANNY_MGR);
			sprintf_ext(CHAR_MAX_SIZE, tATI.szAcntName, "INTERACTIVE");

			t_ASITSDLLUtil->DelTaskInfo(STR_TS_NAME_NANNY_MGR);
			t_ASITSDLLUtil->AddTaskInfo(&tATI);

			if(tSysInfo.IsExistLoginSession() && tProcUtil.ProcessExist(STR_PROC_NAME_NANNY_MGR) == 0)
			{
				ASI_TS_MGR_PARAM tATMP;
				{
					sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_NANNY_MGR_ONCE, GetTickCount()); 
					tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
					tATI.nStartTime	= GetCurrentDateTimeInt() + 5;

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
	}
*/
	return 0;
}

INT32		CLogicMgrPoHostRun::OnTimer_Logic_ConnManage()
{
	t_EnvInfoOp->m_bSkipReConnectLgnSvr = FALSE;
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
	return 0;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostRun::ApplyMgrPortChg(DB_PO_HOST_RUN& dphr)
{
	if(t_LinkSvrUtil->IsRunSvr())
		return -1;

	SOCK_INIT_DATA tSID;
	INT32 nIFPort = t_EnvInfo->m_nIFPort;

	{
		t_LinkSvrUtil->StopServer(1);
		t_LinkSvrUtil->CloseSocket(0, 0);
		t_LinkSvrUtil->FreeServer();
	}

	if(dphr.nLnxRunOption & SS_PO_HOST_RUN_OPTION_FLAG_SUPPORT_MGR_IF_PORT_CHG_AUTO)
	{
		nIFPort = t_EnvInfo->GetReg_IFPortApply();
		nIFPort += SS_PO_HOST_RUN_MGR_IF_PORT_CHG_INTERVAL;

		if(nIFPort > 65535 || nIFPort < 1)
			nIFPort = t_EnvInfo->m_nIFPort;
	}

	tSID = t_EnvInfoOp->GetSvrLinkSID(0, nIFPort);
	if((SetER(t_LinkSvrUtil->StartServer(&tSID, NULL))))
	{
		WriteLogE("socket open fail for agent link connect by po host run! :[port:%d][errcode:%d]", nIFPort, g_nErrRtn);
	}

	t_EnvInfo->SetReg_IFPortApply(nIFPort);

	{
		CProcUtil tProcUtil;
		tProcUtil.ProcessStop(STR_PROC_NAME_NANNY_MGR);
	}

	WriteLogN("[%s] set if port apply chg auto : [port:%d][%x]", m_strLogicName.c_str(), nIFPort, dphr.nLnxRunOption & SS_PO_HOST_RUN_OPTION_FLAG_SUPPORT_MGR_IF_PORT_CHG_AUTO);

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
