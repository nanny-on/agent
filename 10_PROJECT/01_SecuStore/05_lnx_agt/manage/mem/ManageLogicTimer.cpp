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
#include "ManageLogicTimer.h"

//---------------------------------------------------------------------------

CManageLogicTimer*	t_ManageLogicTimer = NULL;

//---------------------------------------------------------------------------

CManageLogicTimer::CManageLogicTimer()
{
	m_nGlobalID	= 0;
}
//---------------------------------------------------------------------------

CManageLogicTimer::~CManageLogicTimer()
{
	
}
//---------------------------------------------------------------------------

INT32		CManageLogicTimer::AddLogicBase(PVOID tLogicBase, INT32 nPeriodSec)
{
	MEM_LOGIC_TIMER tMLT;
	tMLT.nID		= m_nGlobalID++;
	tMLT.tLogicBase	= tLogicBase;
	tMLT.nPeriodSec	= nPeriodSec;
	
	return AddItem(tMLT);
}
//---------------------------------------------------------------------------

INT32		CManageLogicTimer::EditLogicBase(PVOID tLogicBase, INT32 nPeriodSec)
{
	PMEM_LOGIC_TIMER pMLT = FindItemByLogic(tLogicBase);
	
	if(pMLT)
	{
		pMLT->nPeriodSec	= nPeriodSec;
	}
	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageLogicTimer::DelLogicBase(PVOID tLogicBase)
{
	PMEM_LOGIC_TIMER pMLT = FindItemByLogic(tLogicBase);

	if(pMLT)
	{
		DeleteItem(pMLT->nID);
	}
	return 0;
}
//---------------------------------------------------------------------------

PMEM_LOGIC_TIMER	CManageLogicTimer::FindItemByLogic(PVOID tLogicBase)
{
	TMapMemLogicTimerItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.tLogicBase != tLogicBase)		continue;

		return &(begin->second);
	}
	return NULL;
}
//---------------------------------------------------------------------------

