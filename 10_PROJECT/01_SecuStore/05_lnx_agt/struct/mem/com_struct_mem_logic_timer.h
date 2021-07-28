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

#ifndef MEM_LOGIC_TIMER_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_LOGIC_TIMER_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_logic_timer
{
	_mem_logic_timer()
	{
		nID					= 0;
		tLogicBase			= 0;
		nPeriodSec			= 0;
		nLastTime			= 0;
	}

	UINT32			nID;	
	PVOID			tLogicBase;
	UINT32			nPeriodSec;
	UINT32			nLastTime;
}MEM_LOGIC_TIMER, *PMEM_LOGIC_TIMER;

typedef list<MEM_LOGIC_TIMER>				TListMemLogicTimer;
typedef TListMemLogicTimer::iterator		TListMemLogicTimerItor;

typedef map<UINT32, MEM_LOGIC_TIMER>		TMapMemLogicTimer;
typedef TMapMemLogicTimer::iterator			TMapMemLogicTimerItor;

#endif //MEM_LOGIC_TIMER_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



