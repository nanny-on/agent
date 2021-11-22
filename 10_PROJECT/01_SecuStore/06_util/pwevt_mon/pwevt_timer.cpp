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
#include "pwevt_dlg.h"
#include "pwevt_timer.h"

CMonTimerUtil::CMonTimerUtil()
{
	m_nMaxID	= 1;
	m_nNotifyID	= 0;
	m_pNotifyWnd = NULL;
	pthread_mutex_init(&m_mutex, NULL);
}
//---------------------------------------------------------------------------

CMonTimerUtil::~CMonTimerUtil()
{
	pthread_mutex_destroy(&m_mutex);
}
//---------------------------------------------------------------------------

INT32	CMonTimerUtil::SetNotifyWnd(HWND pNotifyWnd, UINT32 dwNotifyID)
{
	m_pNotifyWnd = pNotifyWnd;
	m_nNotifyID	= dwNotifyID;
	return 0;
}
//---------------------------------------------------------------------------

UINT32	CMonTimerUtil::GetNextTimerID()
{
	UINT32 nRtn = 0;

	TListMonTimerInfoItor begin, end;
    begin = tMonTimerInfoList.begin();	end = tMonTimerInfoList.end();
    for(begin; begin != end; begin++)
    {
    	if( begin->nID < nRtn)
			continue;

        nRtn = begin->nID;
    }

    return ++nRtn;
}
//---------------------------------------------------------------------------

UINT32	CMonTimerUtil::IsExistTimerID(UINT32 nTimerID)
{
	TListMonTimerInfoItor begin, end;
    begin = tMonTimerInfoList.begin();	end = tMonTimerInfoList.end();
    for(begin; begin != end; begin++)
    {
    	if( begin->nID != nTimerID)
			continue;

	    return 1;
    }
	
	return 0;
}
//---------------------------------------------------------------------------

UINT32	CMonTimerUtil::IsEnabledTimer(UINT32 nTimerID)
{
	PMON_TIMER_INFO pInfo = FindTimer(nTimerID);
	if(pInfo == NULL)
		return 0;

	return pInfo->nEnabled;
}
//---------------------------------------------------------------------------

PMON_TIMER_INFO 	CMonTimerUtil::FindTimer(UINT32 nTimerID)
{
 	TListMonTimerInfoItor begin, end;
    begin = tMonTimerInfoList.begin();	end = tMonTimerInfoList.end();
    for(begin; begin != end; begin++)
    {
    	if( begin->nID != nTimerID)
			continue;

		return &(*begin);
    }

	return NULL;
}
//---------------------------------------------------------------------------

UINT32	CMonTimerUtil::AddTimer(UINT32 nTimerID, UINT32 nInterval, UINT32 nMsgType, UINT32 nEnabled, INT32 nLimited)
{
    MON_TIMER_INFO stTimerInfo;
	memset(&stTimerInfo, 0, sizeof(MON_TIMER_INFO));

	pthread_mutex_lock(&m_mutex);
	if(IsExistTimerID(nTimerID))
    	nTimerID = GetNextTimerID();
    stTimerInfo.nID			 = nTimerID;
    stTimerInfo.nInterval    = nInterval;
    stTimerInfo.nEnabled	 = nEnabled;
	stTimerInfo.nMessageType = nMsgType;
	stTimerInfo.nLimited	 = nLimited;
	stTimerInfo.nLastChkTime = GetCurrentDateTimeInt();
    tMonTimerInfoList.push_back(stTimerInfo);
	pthread_mutex_unlock(&m_mutex);

    return nTimerID;
}
//---------------------------------------------------------------------------

INT32	CMonTimerUtil::EditTimer(UINT32 nTimerID, UINT32 nInterval, UINT32 nMsgType, UINT32 nEnabled, INT32 nLimited)
{
	INT32 nReVal = 0;
	PMON_TIMER_INFO pInfo = NULL;
	CPwevtDlg* t_MonDlg = (CPwevtDlg *)m_pNotifyWnd;
	pthread_mutex_lock(&m_mutex);
	do{
		pInfo = FindTimer(nTimerID);
		if(pInfo == NULL)
		{
			if(t_MonDlg != NULL)
				t_MonDlg->WriteLogE("[EditTimer] fail to find timer (%d)", nTimerID);
			nReVal = -1;
			break;
		}

		pInfo->nEnabled		= nEnabled;
		pInfo->nMessageType	= nMsgType;
		pInfo->nLimited		= nLimited;
		if(nInterval)
			pInfo->nInterval = nInterval;
		if(nEnabled)
			pInfo->nLastChkTime	= GetCurrentDateTimeInt();

		nReVal = 0;
	}while(FALSE);

	pthread_mutex_unlock(&m_mutex);
    return nReVal;
}
//---------------------------------------------------------------------------

INT32	CMonTimerUtil::EnableTimer(UINT32 nTimerID, UINT32 nResetTime)
{
	PMON_TIMER_INFO pInfo = NULL;
	INT32 nReVal = 0;
	CPwevtDlg* t_MonDlg = (CPwevtDlg *)m_pNotifyWnd;
	pthread_mutex_lock(&m_mutex);	
	do{
		pInfo = FindTimer(nTimerID);
		if(pInfo == NULL)
		{
			if(t_MonDlg != NULL)
				t_MonDlg->WriteLogE("[EnableTimer] fail to find timer (%d)", nTimerID);
			nReVal = -1;
			break;
		}

		if(nResetTime)
		{
			pInfo->nLastChkTime = GetCurrentDateTimeInt();
		}

		pInfo->nEnabled = 1;
		nReVal = 0;
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);
	return nReVal;
}
//---------------------------------------------------------------------------

INT32	CMonTimerUtil::DisableTimer(UINT32 nTimerID, UINT32 nResetTime)
{
	PMON_TIMER_INFO pInfo = NULL;
	INT32 nReVal = 0;
	CPwevtDlg* t_MonDlg = (CPwevtDlg *)m_pNotifyWnd;

	do{
		pInfo = FindTimer(nTimerID);
		if(pInfo == NULL)
		{
			if(t_MonDlg != NULL)
				t_MonDlg->WriteLogE("[DisableTimer] fail to find timer (%d)", nTimerID);
			nReVal = -1;
			break;
		}

		if(nResetTime)
		{
			pInfo->nLastChkTime	= GetCurrentDateTimeInt();
		}

		pInfo->nEnabled	= 0;
		nReVal = 0;
	}while(FALSE);
	return nReVal;
}
//---------------------------------------------------------------------------

INT32	CMonTimerUtil::DelTimer(UINT32 nTimerID)
{
	TListMonTimerInfoItor begin, end;
	pthread_mutex_lock(&m_mutex);
    begin = tMonTimerInfoList.begin();	end = tMonTimerInfoList.end();
    for(begin; begin != end; begin++)
    {
    	if( begin->nID != nTimerID)
			continue;

        tMonTimerInfoList.erase(begin);
        break;
    }
	pthread_mutex_unlock(&m_mutex);
    return 1;
}
//---------------------------------------------------------------------------

void	CMonTimerUtil::ChkTimer()
{
	UINT32 nChkTimerID = 0;
	UINT32 nNowTime = 0;
	UINT32 nInterval = 0;
	TListMonTimerInfoItor begin, end;
	CPwevtDlg* t_MonDlg = (CPwevtDlg *)m_pNotifyWnd;

	pthread_mutex_lock(&m_mutex);
	begin = tMonTimerInfoList.begin();	end = tMonTimerInfoList.end();
	for(begin; begin != end; begin++)
	{
		if( begin->nEnabled == 0)
			continue;

		nNowTime = GetCurrentDateTimeInt();

		nInterval = nNowTime - begin->nLastChkTime;
		if(nInterval <= begin->nInterval)
			continue;

		nChkTimerID = begin->nID;
		
		if(t_MonDlg != NULL)
			t_MonDlg->PostMessage(m_nNotifyID, nChkTimerID, 0);

		if(begin->nLimited > 0)
		{
			begin->nLimited--;				
		}
		if(begin->nLimited == 0)
			begin->nEnabled = 0;

		begin->nLastChkTime = nNowTime;
	}
	pthread_mutex_unlock(&m_mutex);
}


