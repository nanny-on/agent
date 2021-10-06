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
#include "accnotify_dlg.h"
#include "accnotify_timer.h"

CAccMonTimerUtil::CAccMonTimerUtil()
{
	m_nMaxID	= 1;
	m_nNotifyID	= 0;
	m_pNotifyWnd = NULL;
	pthread_mutex_init(&m_mutex, NULL);
}
//---------------------------------------------------------------------------

CAccMonTimerUtil::~CAccMonTimerUtil()
{
	pthread_mutex_destroy(&m_mutex);
}
//---------------------------------------------------------------------------

INT32	CAccMonTimerUtil::SetNotifyWnd(HWND pNotifyWnd, UINT32 dwNotifyID)
{
	m_pNotifyWnd = pNotifyWnd;
	m_nNotifyID	= dwNotifyID;
	return 0;
}
//---------------------------------------------------------------------------

UINT32	CAccMonTimerUtil::GetNextTimerID()
{
	UINT32 nRtn = 0;
	TListAccMonTimerInfoItor begin, end;
    begin = tAccMonTimerInfoList.begin();	end = tAccMonTimerInfoList.end();
    for(begin; begin != end; begin++)
    {
    	if( begin->nID < nRtn)
			continue;

        nRtn = begin->nID;
    }

    return ++nRtn;
}
//---------------------------------------------------------------------------

UINT32	CAccMonTimerUtil::IsExistTimerID(UINT32 nTimerID)
{
	TListAccMonTimerInfoItor begin, end;
    begin = tAccMonTimerInfoList.begin();	end = tAccMonTimerInfoList.end();
    for(begin; begin != end; begin++)
    {
    	if( begin->nID != nTimerID)
			continue;

	    return 1;
    }
	
	return 0;
}
//---------------------------------------------------------------------------

UINT32	CAccMonTimerUtil::IsEnabledTimer(UINT32 nTimerID)
{
	PACC_MON_TIMER_INFO pInfo = FindTimer(nTimerID);
	if(pInfo == NULL)
		return 0;

	return pInfo->nEnabled;
}
//---------------------------------------------------------------------------

PACC_MON_TIMER_INFO 	CAccMonTimerUtil::FindTimer(UINT32 nTimerID)
{
 	TListAccMonTimerInfoItor begin, end;
    begin = tAccMonTimerInfoList.begin();	end = tAccMonTimerInfoList.end();
    for(begin; begin != end; begin++)
    {
    	if( begin->nID != nTimerID)
			continue;

		return &(*begin);
    }

	return NULL;
}
//---------------------------------------------------------------------------

UINT32	CAccMonTimerUtil::AddTimer(UINT32 nTimerID, UINT32 nInterval, UINT32 nMsgType, UINT32 nEnabled, INT32 nLimited)
{
    ACC_MON_TIMER_INFO stAccTimerInfo;
	memset(&stAccTimerInfo, 0, sizeof(ACC_MON_TIMER_INFO));

	pthread_mutex_lock(&m_mutex);
	if(IsExistTimerID(nTimerID))
    	nTimerID = GetNextTimerID();
    stAccTimerInfo.nID			 = nTimerID;
    stAccTimerInfo.nInterval    = nInterval;
    stAccTimerInfo.nEnabled	 = nEnabled;
	stAccTimerInfo.nMessageType = nMsgType;
	stAccTimerInfo.nLimited	 = nLimited;
	stAccTimerInfo.nLastChkTime = GetCurrentDateTimeInt();
    tAccMonTimerInfoList.push_back(stAccTimerInfo);
	pthread_mutex_unlock(&m_mutex);

    return nTimerID;
}
//---------------------------------------------------------------------------

INT32	CAccMonTimerUtil::EditTimer(UINT32 nTimerID, UINT32 nInterval, UINT32 nMsgType, UINT32 nEnabled, INT32 nLimited)
{
	INT32 nReVal = 0;
	PACC_MON_TIMER_INFO pInfo = NULL;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	pthread_mutex_lock(&m_mutex);
	do{
		pInfo = FindTimer(nTimerID);
		if(pInfo == NULL)
		{
			if(t_AccNotifyDlg != NULL)
				t_AccNotifyDlg->WriteLogE("[EditTimer] fail to find timer (%d)", nTimerID);
			nReVal = -1;
			break;
		}

		pInfo->nEnabled	= nEnabled;
		pInfo->nMessageType	= nMsgType;
		pInfo->nLimited	= nLimited;
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

INT32	CAccMonTimerUtil::EnableTimer(UINT32 nTimerID, UINT32 nResetTime)
{
	PACC_MON_TIMER_INFO pInfo = NULL;
	INT32 nReVal = 0;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;
	pthread_mutex_lock(&m_mutex);	
	do{
		pInfo = FindTimer(nTimerID);
		if(pInfo == NULL)
		{
			if(t_AccNotifyDlg != NULL)
				t_AccNotifyDlg->WriteLogE("[EnableTimer] fail to find timer (%d)", nTimerID);
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

INT32	CAccMonTimerUtil::DisableTimer(UINT32 nTimerID, UINT32 nResetTime)
{
	PACC_MON_TIMER_INFO pInfo = NULL;
	INT32 nReVal = 0;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;

	do{
		pInfo = FindTimer(nTimerID);
		if(pInfo == NULL)
		{
			if(t_AccNotifyDlg != NULL)
				t_AccNotifyDlg->WriteLogE("[DisableTimer] fail to find timer (%d)", nTimerID);
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

INT32	CAccMonTimerUtil::DelTimer(UINT32 nTimerID)
{
	TListAccMonTimerInfoItor begin, end;
	pthread_mutex_lock(&m_mutex);
    begin = tAccMonTimerInfoList.begin();	end = tAccMonTimerInfoList.end();
    for(begin; begin != end; begin++)
    {
    	if( begin->nID != nTimerID)
			continue;

        tAccMonTimerInfoList.erase(begin);
        break;
    }
	pthread_mutex_unlock(&m_mutex);
    return 1;
}
//---------------------------------------------------------------------------

void	CAccMonTimerUtil::ChkTimer()
{
	UINT32 nChkTimerID = 0;
	UINT32 nNowTime = 0;
	UINT32 nInterval = 0;
	TListAccMonTimerInfoItor begin, end;
	CAccNotifyDlg* t_AccNotifyDlg = (CAccNotifyDlg *)m_pNotifyWnd;

	pthread_mutex_lock(&m_mutex);
	begin = tAccMonTimerInfoList.begin();	end = tAccMonTimerInfoList.end();
	for(begin; begin != end; begin++)
	{
		if( begin->nEnabled == 0)
			continue;

		nNowTime = GetCurrentDateTimeInt();

		nInterval = nNowTime - begin->nLastChkTime;
		if(nInterval <= begin->nInterval)
			continue;

		nChkTimerID = begin->nID;
		
		if(t_AccNotifyDlg != NULL)
			t_AccNotifyDlg->PostMessage(m_nNotifyID, nChkTimerID, 0);

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


