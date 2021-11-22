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

#ifndef _PWEVT_TIMER_H__
#define _PWEVT_TIMER_H__
//---------------------------------------------------------------------------
typedef struct _mon_timer_info
{
	_mon_timer_info()
    {
    	nID				= 0;
    	nInterval		= 1;
        nLastChkTime    = 0;
        nEnabled		= 0;
		nMessageType	= 1;
		nChkDTType		= 0;
		nLimited		= -1;
    }
    UINT32			nID;
	UINT32			nInterval;
    UINT32			nLastChkTime;
    UINT32			nEnabled;
	UINT32			nMessageType;
	UINT32			nChkDTType;
	INT32			nLimited;
}MON_TIMER_INFO, *PMON_TIMER_INFO;

typedef list<MON_TIMER_INFO>			TListMonTimerInfo;
typedef TListMonTimerInfo::iterator		TListMonTimerInfoItor;

class CMonTimerUtil
{
private:
	TListMonTimerInfo	tMonTimerInfoList;
    UINT32				m_nMaxID;
	pthread_mutex_t		m_mutex;
	HWND				m_pNotifyWnd;
	UINT32				m_nNotifyID;

private:
	UINT32				GetNextTimerID();
    PMON_TIMER_INFO 	FindTimer(UINT32 nTimerID);

public:
	UINT32	AddTimer(UINT32 nTimerID, UINT32 nInterval, UINT32 nMsgType = 1, UINT32 nEnabled = 1, INT32 nLimited = -1);
    INT32	EditTimer(UINT32 nTimerID, UINT32 nInterval, UINT32 nMsgType, UINT32 nEnabled, INT32 nLimited = -1);
	INT32	EnableTimer(UINT32 nTimerID, UINT32 nResetTime = 1);
	INT32	DisableTimer(UINT32 nTimerID, UINT32 nResetTime = 1);
    INT32	DelTimer(UINT32 nTimerID);
	UINT32	IsExistTimerID(UINT32 nTimerID);
	UINT32	IsEnabledTimer(UINT32 nTimerID);

public:
	INT32	SetNotifyWnd(HWND tNotifyWnd, UINT32 nNotifyID);
	
public:
	void	ChkTimer();    

public:
	CMonTimerUtil();
    ~CMonTimerUtil();
};


#endif /*_PWEVT_TIMER_H__*/
