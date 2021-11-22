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
#include "ManageChkExistInterval.h"

//---------------------------------------------------------------------------

CManageChkExistInterval	t_ManageChkExistInterval;

//---------------------------------------------------------------------------

CManageChkExistInterval::CManageChkExistInterval()
{
	m_nGlobalID	= 0;

	InitChkExistInterval();
}
//---------------------------------------------------------------------------

CManageChkExistInterval::~CManageChkExistInterval()
{
	
}
//---------------------------------------------------------------------------

INT32		CManageChkExistInterval::InitChkExistInterval()
{
	return 0;
}

//---------------------------------------------------------------------------

INT32		CManageChkExistInterval::SetChkExistStatus(String strChkName, UINT32 nStatus)
{
	m_tMutex.Lock();

	MEM_CHK_EXIST_INTERVAL tdata;
	PMEM_CHK_EXIST_INTERVAL pdata = FindSKeyID_Item(strChkName);
	if(!pdata)
	{
		{
			tdata.nID = ++m_nGlobalID;
			tdata.strChkName = strChkName;
		}
		AddSKeyID(tdata.strChkName, tdata.nID);
		AddItem(tdata);
	}
	else
		tdata = *pdata;

	tdata.nStatus = nStatus;
	EditItem(tdata);

	m_tMutex.UnLock();

	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageChkExistInterval::IsChkExistStatus(String strChkName)
{
	UINT32 nRtn = 0;

	m_tMutex.Lock();

	PMEM_CHK_EXIST_INTERVAL pdata = FindSKeyID_Item(strChkName);
	if(!pdata)
	{
		m_tMutex.UnLock();
		return -1;
	}

	nRtn = pdata->nStatus;
	
	m_tMutex.UnLock();

	return nRtn;
}
//---------------------------------------------------------------------------

