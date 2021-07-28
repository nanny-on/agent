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
#include "LogicMgrEnvNotifyInfo.h"

//---------------------------------------------------------------------------

CLogicMgrEnvNotifyInfo*		t_LogicMgrEnvNotifyInfo = NULL;

//---------------------------------------------------------------------------

CLogicMgrEnvNotifyInfo::CLogicMgrEnvNotifyInfo()
{
	t_ManageEnvNotifyInfo	= new CManageEnvNotifyInfo();
	t_ManageEnvNotifyInfo->LoadDBMS();
	t_ManageEnvNotifyInfo->InitHash();

	t_LogicEnvNotifyInfo    = new CLogicEnvNotifyInfo();

	m_strLogicName		= "mgr env notify info";

	m_nControlType		= SS_CONTROL_TYPE_ENV_NOTIFY_INFO;	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_ENV;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_ENV_NOTIFY_INFO;
}
//---------------------------------------------------------------------------

CLogicMgrEnvNotifyInfo::~CLogicMgrEnvNotifyInfo()
{
	SAFE_DELETE(t_LogicEnvNotifyInfo);
	SAFE_DELETE(t_ManageEnvNotifyInfo);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvNotifyInfo::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvNotifyInfo::AnalyzePkt_FromMgr_Edit_Ext()
{
	TListDBEnvNotifyInfo	tUnitList;

	if(!RecvToken.TokenDel_32(m_nRecvNum))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_ENV_NOTIFY_INFO deni;

		if( t_ManageEnvNotifyInfo->GetPkt(RecvToken, deni)) return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tUnitList.push_back(deni);		
	}
	
	{
		TListID tIDList;
		t_ManageEnvNotifyInfo->GetItemIDList(tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageEnvNotifyInfo->IsExistID(*begin, tUnitList))		continue;
			
			t_ManageEnvNotifyInfo->DelEnvNotifyInfo(*begin);
		}
	}

	{
		TListDBEnvNotifyInfoItor begin, end;
		begin = tUnitList.begin();	end = tUnitList.end();
		for(begin; begin != end; begin++)
		{
			if(begin->nMsgFmtType != 0)
				continue;
			if(t_ManageEnvNotifyInfo->ApplyEnvNotifyInfo(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		t_ManageEnvNotifyInfo->InitHash();
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvNotifyInfo::ApplyPolicy()
{	
	{
		t_LogicMgrPoFaEnv->ApplyPolicy();
	}
	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvNotifyInfo::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvNotifyInfo::IsValidSchduleNotify(UINT32 nLastChkTime, UINT64 nSchInfo, MEM_ENV_NOTIFY_INFO_SCH& meni)
{
	if(!t_ManageChkExistInterval.IsChkExistStatus(STR_PROC_NAME_NANNY_MGR) || !t_ManageChkExistInterval.IsChkExistStatus(STR_WIN_SESSION))
		return 2;

	TListID tIDList;
	t_ManageEnvNotifyInfo->GetIDLIstByNotifyPolType(meni.nNotifyType, tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();
	end   = tIDList.end();

	for(; begin != end; begin++)
	{
		PDB_ENV_NOTIFY_INFO pNotify = t_ManageEnvNotifyInfo->FindItem(*begin);
		if(!pNotify)
		{
			WriteLogE("check valid schdule notify fail : not find item is null");
			continue;
		}
		
		if(pNotify->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
			continue;

		UINT32 nCurTime	 = GetCurrentDateTimeInt();
		UINT32 nSchTime  = GetSchTimeByType(meni.nSchType, nSchInfo, nCurTime, nLastChkTime);
		UINT32 nStTime   =  nSchTime - (pNotify->nNotifyBeforeDay * TIMER_INTERVAL_TIME_DAY);
		UINT32 nNotifyTime = t_ManageEnvNotifyInfo->GetEnvNotifyInfoNotiTime(meni.nPolType, meni.tDPH.nID, pNotify->tDPH.nID);

		if (pNotify->nPolicyCode == SS_PO_FA_NOTIFY_MSG_TYPE_SCH_START && 
			!t_ManageEnvNotifyInfo->IsExcludeNotifySchdule(meni.nPolType, meni.tDPH.nID) &&
			(nStTime < nCurTime) && difftime(nCurTime, nNotifyTime) >= TIMER_INTERVAL_TIME_DAY)
		{
			meni.nNotifyInfoID = pNotify->tDPH.nID;
			meni.nSchTime = nSchTime;

			WriteLogN("sucuss check valid schdule notify [pol_type:%u][unitid:%u][notify_before_time:%u][stt:scht %u:%u][notify_time:%u]", meni.nPolType, meni.tDPH.nID, pNotify->nNotifyBeforeDay, nStTime, nSchTime, nNotifyTime);
			return 0;
		}
	}

	return 1;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvNotifyInfo::GetSchTimeByType(UINT32 nType, UINT64 nSchInfo, UINT32 nCurTime, UINT32 nLastChkTime)
{
	U64_SCHEDULE tIS;
	tIS.all = nSchInfo;

	UINT32 nCurYearMonDay   = (nCurTime / TIMER_INTERVAL_TIME_DAY) * TIMER_INTERVAL_TIME_DAY;
	UINT32 nCurYearMon      = nCurYearMonDay - (GetDayOfDay(nCurTime, 1) - 1) * TIMER_INTERVAL_TIME_DAY;
	UINT32 nCurYearMonDayHour = (nCurTime / TIMER_INTERVAL_TIME_HOUR) * TIMER_INTERVAL_TIME_HOUR;

	INT32 nRtn = 0;

	switch(nType)
	{
	case SCHEDULE_PERIOD_TYPE_MIN:
		{
			UINT32 nChkMin = ((nCurTime - nLastChkTime) / TIMER_INTERVAL_TIME_MIN);
			if(nChkMin >= tIS.U8.min)
				nRtn = nCurTime;
			else
				nRtn = nCurTime + (tIS.U8.hour - nChkMin) * TIMER_INTERVAL_TIME_HOUR;

			break;
		}
	case SCHEDULE_PERIOD_TYPE_HOUR:
		{
			UINT32 nChkHour = ((nCurTime - nLastChkTime) / TIMER_INTERVAL_TIME_HOUR);
			if(nChkHour >= tIS.U8.hour)
				nRtn = nCurTime;
			else
				nRtn = nCurTime + (tIS.U8.hour - nChkHour) * TIMER_INTERVAL_TIME_HOUR;

			break;
		}
	case SCHEDULE_PERIOD_TYPE_DAY:
		{
			UINT32 nCurSchTime = nCurYearMonDay + tIS.U8.hour * TIMER_INTERVAL_TIME_HOUR + tIS.U8.min * TIMER_INTERVAL_TIME_MIN;

			if(nCurSchTime >= nCurTime && (nCurSchTime - nLastChkTime) >= (TIMER_INTERVAL_TIME_DAY))
				nRtn = nCurSchTime;
			else
				nRtn = nCurSchTime + TIMER_INTERVAL_TIME_DAY;

			break;
		}
	case SCHEDULE_PERIOD_TYPE_WEEK:
		{
			UINT32 nDiffDay = GetDayDiffWeek(GetDayOfWeek(nCurTime, 1), tIS.U8.value);
			UINT32 nCurSchTime = nCurYearMonDay + tIS.U8.hour * TIMER_INTERVAL_TIME_HOUR + tIS.U8.min * TIMER_INTERVAL_TIME_MIN + nDiffDay * TIMER_INTERVAL_TIME_DAY;

			if(nCurSchTime >= nCurTime && (nCurSchTime - nLastChkTime) >= (TIMER_INTERVAL_TIME_DAY))
				nRtn = nCurSchTime;
			else
				nRtn = nCurSchTime + TIMER_INTERVAL_TIME_DAY * 7;
			break;
		}
	case SCHEDULE_PERIOD_TYPE_MONTH:
		{
			UINT32 nCurSchTime = nCurYearMon + tIS.U8.value * TIMER_INTERVAL_TIME_DAY + tIS.U8.hour * TIMER_INTERVAL_TIME_HOUR + tIS.U8.min * TIMER_INTERVAL_TIME_MIN;

			if(nCurSchTime >= nCurTime && (nCurSchTime - nLastChkTime) >= (TIMER_INTERVAL_TIME_DAY))
				nRtn = nCurSchTime;
			else
				nRtn = GetTimeIntAddMonth(nCurSchTime, 1);

			break;
		}
	case SCHEDULE_PERIOD_TYPE_FIX_HOUR:
		{
			UINT32 nLastFixTime = (nLastChkTime / 3600) * 3600;
			UINT32 nLastFixSchTime = nLastFixTime + tIS.U8.hour * TIMER_INTERVAL_TIME_HOUR;
			UINT32 nCurSchTime = nCurYearMonDayHour;

			if(nLastFixSchTime >= nCurTime && (nCurTime - nLastFixTime) >= TIMER_INTERVAL_TIME_HOUR)
				nRtn = nLastFixSchTime;
			else
				nRtn = nCurSchTime;

			break;
		}
	case SCHEDULE_PERIOD_TYPE_FIX_DAY:
		{
			UINT32 nCurSchTime = nCurYearMonDay + tIS.U8.hour * TIMER_INTERVAL_TIME_HOUR + tIS.U8.min * TIMER_INTERVAL_TIME_MIN;

			if(nCurSchTime >= nCurTime && nCurSchTime > nLastChkTime)
				nRtn = nCurSchTime;
			else
				nRtn = nCurSchTime + TIMER_INTERVAL_TIME_DAY;

			break;
		}
	}

	return nRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
