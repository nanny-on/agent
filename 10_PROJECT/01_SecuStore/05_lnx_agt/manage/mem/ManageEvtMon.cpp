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
#include "ManageEvtMon.h"

//---------------------------------------------------------------------------

CManageEvtMon	t_ManageEvtMon;

//---------------------------------------------------------------------------

CManageEvtMon::CManageEvtMon()
{
	m_nGID		= 1;
}
//---------------------------------------------------------------------------

CManageEvtMon::~CManageEvtMon()
{
	
}
//---------------------------------------------------------------------------

INT32		CManageEvtMon::AddEvtMon(UINT32 nClass, UINT32 nType, String strName, UINT32 nCode)
{
	m_tMutex.Lock();

	MEM_EVT_MON tMWS;
	tMWS.nID		= m_nGID++;
	tMWS.nThreadID	= gettid();
	tMWS.strName	= strName;
	tMWS.nClass		= nClass;
	tMWS.nType		= nType;
	tMWS.nCode		= nCode;
	tMWS.nTime		= GetCurrentDateTimeInt();
	AddItem(tMWS);

	m_tMutex.UnLock();

	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageEvtMon::DelEvtMon(String strName)
{
	m_tMutex.Lock();
	UINT64 nThreadID = gettid();
	TMapMemEvtMonItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; )
	{
		if(begin->second.nThreadID == nThreadID && begin->second.strName == strName)
		{
			m_tMap.erase(begin);
			break;
		}
		begin++;
	}
	m_tMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageEvtMon::GetOverTimeEvtMon(TListMemEvtMon& tEvtMonList)
{
	m_tMutex.Lock();
	UINT32 nCurTime = GetCurrentDateTimeInt();
	TMapMemEvtMonItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nLogTime)
		{
			if((nCurTime - begin->second.nLogTime > 60))
			{
				begin->second.nLogTime = nCurTime;
				tEvtMonList.push_back(begin->second);
			}
		}
		else if((nCurTime - begin->second.nTime > 5))
		{
			begin->second.nLogTime = nCurTime;
			tEvtMonList.push_back(begin->second);
		}
	}
	m_tMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------

