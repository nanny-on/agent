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
#include "ManageVulnPreScan.h"

//---------------------------------------------------------------------------

CManageVulnPreScan*	t_ManageVulnPreScan = NULL;

//---------------------------------------------------------------------------

CManageVulnPreScan::CManageVulnPreScan()
{
	m_nGID = 1;
}
//---------------------------------------------------------------------------

CManageVulnPreScan::~CManageVulnPreScan()
{
}
//---------------------------------------------------------------------------

INT32					CManageVulnPreScan::AddVulnPreScan(MEM_VULN_PRESCAN&	data)
{
	if(data.nID == 0)		data.nID = m_nGID++;
	AddItem(data.nID, data);  

	AddKeyTypeID(data.nVulnID, data.nVulnScanID, data.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageVulnPreScan::EditVulnPreScan(MEM_VULN_PRESCAN&	data)
{
	PMEM_VULN_PRESCAN pdata = FindItem(data.nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	{
		EditItem(data.nID, data);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageVulnPreScan::DelVulnPreScan(UINT32 nID)
{
	PMEM_VULN_PRESCAN pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	DelKeyTypeID(pdata->nVulnID, pdata->nVulnScanID);

	DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageVulnPreScan::ApplyVulnPreScan(MEM_VULN_PRESCAN&	data)
{
	if(FindItem(data.nID))
	{
		return EditVulnPreScan(data);
	}
	return AddVulnPreScan(data);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageVulnPreScan::GetPreScanResult(UINT32 nKey)
{
	INT32 nRtn = SS_PTN_VULN_SCAN_RESULT_SAFE;
	TListID tIDList;
	GetKeyTypeID_IDList(nKey, tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PMEM_VULN_PRESCAN pdata = FindItem(*begin);
		if(!pdata)	continue;

		if((pdata->nRegDate/TIMER_INTERVAL_TIME_DAY) != (GetCurrentDateTimeInt()/TIMER_INTERVAL_TIME_DAY))
		{
			nRtn = SS_PTN_VULN_SCAN_RESULT_DANGER;
			continue;
		}

		if(pdata->nResult != SS_PTN_VULN_SCAN_RESULT_SAFE)	
		{
			nRtn = pdata->nResult;
		}
	}
	return nRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------- 

void 		CManageVulnPreScan::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][vuln pre scan] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------



