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
#include "LogicMgrPoPower.h"

//---------------------------------------------------------------------------

CLogicMgrPoPower*		t_LogicMgrPoPower = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoPower::CLogicMgrPoPower()
{
	t_ManagePoPower			= new CManagePoPower();
	t_ManagePoPowerPkg		= new CManagePoPowerPkg();
	t_ManagePoPowerUnit		= new CManagePoPowerUnit();

	t_ManagePoPower->LoadDBMS();
	t_ManagePoPowerPkg->LoadDBMS();
	t_ManagePoPowerUnit->LoadDBMS();

	t_ManagePoPower->InitHash();

	m_strLogicName		= "mgr po power";
	
	m_nPolicyType		= SS_POLICY_TYPE_POWER;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_POWER;

	m_dwWriteLogCount	= 0;
}
//---------------------------------------------------------------------------
CLogicMgrPoPower::~CLogicMgrPoPower()
{
	SAFE_DELETE(t_ManagePoPowerUnit);
	SAFE_DELETE(t_ManagePoPowerPkg);
	SAFE_DELETE(t_ManagePoPower);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
INT32		CLogicMgrPoPower::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoPower::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_POWER pdpp = NULL;
	DB_PO_POWER dpp;
	m_tDPH = &(dpp.tDPH);
	TListDBPoPowerPkg	tPkgList;
	TListDBPoPowerUnit	tUnitList;

	if( t_ManagePoPower->GetPkt(RecvToken, dpp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_POWER_PKG dppp;		
		DB_PO_POWER_UNIT dppu;

		if( t_ManagePoPowerPkg->GetPkt(RecvToken, dppp))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoPowerUnit->GetPkt(RecvToken, dppu))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(dppp);
		tUnitList.push_back(dppu);

		dpp.tDPH._add_id(dppp.tDPH.nID);
	}

	pdpp = (PDB_PO_POWER)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdpp)
	{
		t_ManagePoPowerPkg->ClearPkgUnitByPolicyID(pdpp->tDPH.nID);		
		t_ManagePoPower->DelPoPower(pdpp->tDPH.nID);
	}

	{
		{
			TListDBPoPowerUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoPowerUnit->ApplyPoPowerUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}
			}
		}

		{
			TListDBPoPowerPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoPowerPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				
				t_ManagePoPowerPkg->AddPoPowerPkg(*begin);
			}
		}

		if(SetER(t_ManagePoPower->ApplyPoPower(dpp)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPower::ApplyPolicy()
{
	return ApplyPowerPolicy();
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPower::OnTimer_Logic()
{
	ApplyPolicy();
	return 1;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPower::ApplyPowerPolicy()
{
	PDB_PO_POWER pPowerPolicy = (PDB_PO_POWER)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_POWER);
	if (!pPowerPolicy)
		return -1;

	if(pPowerPolicy->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
		return AZPKT_CB_RTN_SUCCESS;

	TListID tUnitIdList;
	t_ManagePoPowerPkg->GetKeyIDList(pPowerPolicy->tDPH.nID, tUnitIdList);
	if (tUnitIdList.size() <= 0)
		return 0;

	INT32 nRet = 0;
	TListIDItor begin, end;
	begin = tUnitIdList.begin();	end = tUnitIdList.end();
	PDB_PO_POWER_UNIT tPowerUnit = NULL;
	for(begin; begin != end; begin++)
	{
		tPowerUnit = t_ManagePoPowerUnit->FindItem(*begin);
		if (!tPowerUnit)
		{
			WriteLogE("::ApplyPowerPolicy tPowerUnit is null : [%d]", *begin);
			continue;
		}

		if(IsValidSchedule(tPowerUnit))
		{
			ApplyPowerUnit(tPowerUnit);
		}
	}

	tUnitIdList.clear();
	return nRet;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPower::IsValidSchedule(DB_PO_POWER_UNIT* pUnit)
{
	if(!pUnit)	return 0;

	if(pUnit->nRunWaitTime)
	{
		if(difftime(GetCurrentDateTimeInt(), pUnit->nRunWaitTime) > pUnit->nCtlWaitTime * 60)
		{
			return 1;
		}
		WriteLogN("[%s] wait apply policy : [%d][%.2d/%.2d min]", m_strLogicName.c_str(), pUnit->tDPH.nID, difftime(GetCurrentDateTimeInt(), pUnit->nRunWaitTime), pUnit->nCtlWaitTime);
		return 0;
	}
	
	INT32 nValidSchedule = CLogicBase::IsValidSchedule(pUnit->nScPeriodType, pUnit->nScStartTime, pUnit->nScEndTime);	
	if(!nValidSchedule)
	{
		return 0;
	}

	if(pUnit->nCtlWaitTime)
	{
		pUnit->nRunWaitTime = GetCurrentDateTimeInt();
		WriteLogN("[%s] wait apply policy : [%d][%.2d min]", m_strLogicName.c_str(), pUnit->tDPH.nID, pUnit->nCtlWaitTime);
		return 0;
	}
	return 1;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPower::ApplyPowerUnit(DB_PO_POWER_UNIT* pUnit)
{
	CTime tCurTime;
	int nDay		= tCurTime.GetDay();
	int nWeek		= tCurTime.GetDayOfWeek();
	int nHour		= tCurTime.GetHour();
	int nMin		= tCurTime.GetMinute();
	int nHourMin	= (nHour * 100) + nMin;
	WriteLogN("[%s] start apply powor policy: [%d][%.2d][%d][%.2d:%.2d][%.4d]", m_strLogicName.c_str(), pUnit->tDPH.nID, nDay, nWeek, nHour, nMin, nHourMin);

	INT32 nExitWindow = 0;
	String strCtrlType;
	CPowerUtil pu;
	switch(pUnit->nCtlMode)
	{
		case SS_PO_POWER_CONTROL_MODE_SYSTEM_TURN_OFF:
		{
			if(t_LogicMgrPoFaClear->IsExistSysOffSchByPoPwr() || t_LogicMgrPoFaOp->IsExistSysOffSchByPoPwr())
			{
				t_ThreadChkHkNoti->InitMemFile_ExitWin(4);
				strCtrlType = "reserved turn off system";
			}
			else
			{
				t_ThreadChkHkNoti->InitMemFile_ExitWin(2);
				pu.SystemShutDown();
				strCtrlType = "turn off system";
			}						
			break;
		}
		case SS_PO_POWER_CONTROL_MODE_SYSTEM_RESTART:
		{
			if(t_LogicMgrPoFaClear->IsExistSysOffSchByPoPwr() || t_LogicMgrPoFaOp->IsExistSysOffSchByPoPwr())
			{
				t_ThreadChkHkNoti->InitMemFile_ExitWin(5);
				strCtrlType = "reserved restart system";
			}
			else
			{
				t_ThreadChkHkNoti->InitMemFile_ExitWin(2);
				pu.RestartSystem();
				strCtrlType = "restart system";
			}
			break;
		}
		case SS_PO_POWER_CONTROL_MODE_MONITOR_TURN_ON:
		case SS_PO_POWER_CONTROL_MODE_MONITOR_TURN_OFF:
		{
			String strMode = "on";
			strCtrlType = "turn on monitor";
			if (pUnit->nCtlMode == SS_PO_POWER_CONTROL_MODE_MONITOR_TURN_OFF)
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
			WriteLogE("[%s] not define system power flag : [%d]", m_strLogicName.c_str(), pUnit->nCtlMode);
			break;
		}
	}

	if (nExitWindow != 0 && m_dwWriteLogCount > 0)
	{
		m_dwWriteLogCount++;
		WriteLogN("[%s] end power call count = %d : [%s]", m_strLogicName.c_str(), m_dwWriteLogCount, strCtrlType.c_str());
		return 0;
	}

	{
		if (nExitWindow != 0 && m_dwWriteLogCount <= 0)
			m_dwWriteLogCount = 1;	// Because ExitWindowsEx Executes Asynchronously

		InitDLEH_Sync(pUnit->tDPH.nID, pUnit->tDPH.strName, EVENT_OPERATION_TYPE_SCHEDULE);		
		AppendDLEDesc(SS_LOG_EVENT_HOST_DESC_KEY_START_TIME, GetCurrentDateTimeInt());		
		AppendDLEDesc(SS_LOG_EVENT_HOST_DESC_KEY_TYPE, strCtrlType);

		t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
	}

	WriteLogN("[%s] end apply power policy : [%s]", m_strLogicName.c_str(), strCtrlType.c_str());
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
