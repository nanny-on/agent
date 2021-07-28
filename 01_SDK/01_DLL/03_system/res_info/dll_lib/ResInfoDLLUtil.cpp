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

#include "stdafx.h"
#include "com_struct.h"
#include "ResInfoDLLUtil.h"

CResInfoDLLUtil*		t_ResInfoDLLUtil;
//--------------------------------------------------------------------

CResInfoDLLUtil::CResInfoDLLUtil()
{
	m_hDLL = NULL;
}
//--------------------------------------------------------------------

CResInfoDLLUtil::~CResInfoDLLUtil()
{
	if(m_hDLL)
		FreeLibrary(m_hDLL);
	m_hDLL = NULL;
}
//--------------------------------------------------------------------

INT32	CResInfoDLLUtil::LoadLibraryExt(LPCTSTR lpDLLName)
{
	if(m_hDLL)
		return 0;

	m_hDLL = LoadLibrary(lpDLLName);
	if(m_hDLL == NULL)
		return -1;
	
	if((f_GetCpuUsageTotal = (GetCpuUsageTotalType)GetProcAddress(m_hDLL, "GetCpuUsageTotal")) == NULL)
		return -2;
	
	if((f_GetMemoryInfo = (GetMemoryInfoType)GetProcAddress(m_hDLL, "GetMemoryInfo")) == NULL)
		return -3;
	
	if((f_GetHddInfo = (GetHddInfoType)GetProcAddress(m_hDLL, "GetHddInfo")) == NULL)
		return -4;
	
	if((f_GetTrafficBandWidht = (GetTrafficBandWidhtType)GetProcAddress(m_hDLL, "GetTrafficBandWidht")) == NULL)
		return -5;
	if((f_GetTrafficValue = (GetTrafficValueType)GetProcAddress(m_hDLL, "GetTrafficValue")) == NULL)
		return -6;
	
	return 0;
}
//--------------------------------------------------------------------

INT32			CResInfoDLLUtil::GetCpuUsageTotal(UINT32* nUsage)
{
	if(m_hDLL)
		return f_GetCpuUsageTotal(nUsage);
	return -1000;
}
//--------------------------------------------------------------------

INT32			CResInfoDLLUtil::GetMemoryInfo(UINT64* nTotalPhySize, UINT64* nTotalPageSize, UINT64* nAvailPhySize, UINT64* nAvailPageSize)
{
	if(m_hDLL)
		return f_GetMemoryInfo(nTotalPhySize, nTotalPageSize, nAvailPhySize, nAvailPageSize);
	return -1000;
}

INT32			CResInfoDLLUtil::GetHddInfo(UINT64* nTotalHddSize, UINT64* nUsageHddSize, LPCTSTR lpDriveName)
{
	if(m_hDLL)
		return f_GetHddInfo(nTotalHddSize, nUsageHddSize, lpDriveName);
	return -1000;
}
//--------------------------------------------------------------------


INT32			CResInfoDLLUtil::GetTrafficBandWidht(INT32 nItemID, DOUBLE* dBandWidth)
{
	if(m_hDLL)
		return f_GetTrafficBandWidht(nItemID, dBandWidth);
	return -1000;
}
//--------------------------------------------------------------------

INT32			CResInfoDLLUtil::GetTrafficValue(INT32 nItemID, DOUBLE* dTrafficValue)
{
	if(m_hDLL)
		return f_GetTrafficValue(nItemID, dTrafficValue);
	return -1000;
}
//--------------------------------------------------------------------

