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

#ifndef MEM_EVT_MON_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_EVT_MON_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_evt_mon
{
	_mem_evt_mon()
	{
		nID				= 0;
		nThreadID		= 0;
		
		nClass			= 0;
		nType			= 0;
		nCode			= 0;
		strName			= "";
		nTime			= 0;
		nLogTime		= 0;
	}

	UINT32			nID;	
	UINT64			nThreadID;
	UINT32			nClass;
	UINT32			nType;
	UINT32			nCode;
	String			strName;
	UINT32			nTime;
	UINT32			nLogTime;
}MEM_EVT_MON, *PMEM_EVT_MON;

typedef list<MEM_EVT_MON>				TListMemEvtMon;
typedef TListMemEvtMon::iterator		TListMemEvtMonItor;

typedef map<UINT32, MEM_EVT_MON>		TMapMemEvtMon;
typedef TMapMemEvtMon::iterator			TMapMemEvtMonItor;

#endif //MEM_EVT_MON_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



