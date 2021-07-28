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
#include "ManageDelLastNotify.h"

//---------------------------------------------------------------------------

CManageDelLastNotify*	t_ManageDelLastNotify = NULL;

//---------------------------------------------------------------------------

CManageDelLastNotify::CManageDelLastNotify()
{
	m_nGID		= 1;
}
//---------------------------------------------------------------------------

CManageDelLastNotify::~CManageDelLastNotify()
{
	
}
//---------------------------------------------------------------------------

INT32		CManageDelLastNotify::AddDelLastNotify(MEM_FIND_ORDER_INFO& tMFOI)
{
	m_tMutex.Lock();

	AddDelLastNotify(tMFOI.nPoID, tMFOI.nOpType, tMFOI.nNotiTotalFind);

	m_tMutex.UnLock();

	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageDelLastNotify::AddDelLastNotify(UINT32 nPolicyType, UINT32 nOpType, UINT32 nTotalDelCnt)
{
	m_tMutex.Lock();

	MEM_FIND_ORDER_INFO tMFOI;
	tMFOI.nID		= m_nGID++;
	tMFOI.nPoID		= nPolicyType;
	tMFOI.nOpType	= nOpType;
	tMFOI.nNotiTotalFind = nTotalDelCnt;

	AddItem(tMFOI);

	m_tMutex.UnLock();

	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageDelLastNotify::DelDelLastNotify(UINT32 nID)
{
	m_tMutex.Lock();
	
	DeleteItem(nID);

	m_tMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CManageDelLastNotify::IsRemainNotify()
{
	INT32 nRtn = 0;
	m_tMutex.Lock();

	nRtn = Count();
	
	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CManageDelLastNotify::GetRemainNotifyList(TListMemFindOrderInfo& tMemFindOrderInfoList)
{
	m_tMutex.Lock();

	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(; begin != end; begin++)
	{
		PMEM_FIND_ORDER_INFO pmfoi = NULL;
		if(t_ManageDelLastNotify)
			pmfoi = t_ManageDelLastNotify->FindItem(*begin);
		if(!pmfoi)	continue;

		tMemFindOrderInfoList.push_back(*pmfoi);
	}
	
	m_tMutex.UnLock();
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


