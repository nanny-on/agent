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

//---------------------------------------------------------------------------
#include "stdafx.h"
#include "com_struct.h"
#include "LogicMgrCtrlRemotePower.h"
//---------------------------------------------------------------------------

CLogicMgrCtrlRemotePower*		t_LogicMgrCtrlRemotePower = NULL;

//---------------------------------------------------------------------------

CLogicMgrCtrlRemotePower::CLogicMgrCtrlRemotePower()
{
	m_strLogicName		= "ctrl remote power";

	m_nControlType		= SS_CONTROL_TYPE_CONTROL_REMOTE_POWER;
	m_nEvtSubType		= EVENT_SUBJECT_TYPE_ADMIN;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_HOST_CTRL_REMOTE_POWER;
}
//---------------------------------------------------------------------------

CLogicMgrCtrlRemotePower::~CLogicMgrCtrlRemotePower()
{
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemotePower::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_CTRL:		nRtn = AnalyzePkt_FromMgr_Ext_Ctrl();			break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemotePower::AnalyzePkt_FromMgr_Ext_Ctrl()
{
	MEM_CTRL_REMOTE_POWER		tMCRP;

	if (!RecvToken.TokenDel_32(tMCRP.nControlType))		return AZPKT_CB_RTN_PKT_INVALID;
	RecvToken.TokenSkip_Block();

	ApplyPolicy(&tMCRP);

	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------   

INT32		CLogicMgrCtrlRemotePower::ApplyPolicy(PMEM_CTRL_REMOTE_POWER pInfo)
{
	INT32 nRet = ERR_SUCCESS;
	if (!pInfo)		return ERR_UNKNOWN;
	
 	m_nEvtOpType = EVENT_OPERATION_TYPE_CONTROL;
	
	String strCtrlType;
	CPowerUtil pu;
	switch(pInfo->nControlType)
	{
		case SS_COMMON_REMOTE_POWER_MODE_SYSTEM_TURN_OFF:
		{
			if(t_LogicMgrPoFaClear->IsExistSysOffSchByPoPwr() || t_LogicMgrPoFaOp->IsExistSysOffSchByPoPwr())
			{
				t_ThreadChkHkNoti->InitMemFile_ExitWin(4);
				strCtrlType = "reserved turn off system";
			}
			else
			{
				t_ThreadChkHkNoti->InitMemFile_ExitWin(2);
				if(pu.SystemShutDown() == FALSE)
				{
					WriteLogE("power off fail : [%d]", errno);
				}
				strCtrlType = "turn off system";
			}
			break;
		}
		case SS_COMMON_REMOTE_POWER_MODE_SYSTEM_RESTART:
		{
			if(t_LogicMgrPoFaClear->IsExistSysOffSchByPoPwr() || t_LogicMgrPoFaOp->IsExistSysOffSchByPoPwr())
			{
				t_ThreadChkHkNoti->InitMemFile_ExitWin(5);
				strCtrlType = "reserved restart system";
			}
			else
			{
				t_ThreadChkHkNoti->InitMemFile_ExitWin(2);
				if(pu.RestartSystem() == FALSE)
				{
					WriteLogE("power restart fail : [%d]", errno);
				}
				strCtrlType = "restart system";
			}
			break;
		}
		case SS_COMMON_REMOTE_POWER_MODE_MONITOR_TURN_ON:
		case SS_COMMON_REMOTE_POWER_MODE_MONITOR_TURN_OFF:
		{
			String strMode = "on";
			strCtrlType = "turn on monitor";
			if (pInfo->nControlType == SS_COMMON_REMOTE_POWER_MODE_MONITOR_TURN_OFF)
			{
				strMode = "off";
				strCtrlType = "turn off monitor";
			}

			ASI_TS_INFO tATI;
			ASI_TS_MGR_PARAM tATMP;
			{
				sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_MONITOR_PW, GetTickCount()); 
				{
					tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
					tATI.nStartTime	= GetCurrentDateTimeInt() + 5;
				}

				tATMP.nTSAutoDel		= 1;
				tATMP.nTSSingleRun		= 1;
				tATMP.nTSShow			= 1;
				tATMP.nTSShowMode		= SHOW_MODE_TYPE_MONITOR_PW;
				tATMP.nTSWaitMode		= 0;

				{
					tATMP.strTSArgument = strMode;		
				}

				tATMP.nTSChildWaitMode = 0;
				tATMP.nTSChildHideMode = 0;
			}		
			t_ExecuteFileUtil->ExecuteFileByUser(tATI, tATMP);
			break;
		}
		default:
		{
			WriteLogE("[%s] not define system power flag : [%d]", m_strLogicName.c_str(), pInfo->nControlType);
			break;
		}
	}

	{
		InitDLE_OBJ(0, strCtrlType);
		t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
	}
	
	return nRet;
}