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
 * * 
 */

#ifndef _RES_USAGE_INFO_DLL_UTIL_H__
#define _RES_USAGE_INFO_DLL_UTIL_H__

//--------------------------------------------------------------------------------------------------
typedef INT32	(*GetCpuUsageTotalType)(UINT32* nUsage);
typedef INT32	(*GetMemoryInfoType)(UINT64* nTotalPhySize, UINT64* nTotalPageSize, UINT64* nAvailPhySize, UINT64* nAvailPageSize);
typedef INT32	(*GetHddInfoType)(UINT64* nTotalHddSize, UINT64* nUsageHddSize, LPCTSTR lpDriveName);
typedef INT32	(*GetTrafficBandWidhtType)(INT32 nItemID, DOUBLE* dBandWidth);
typedef INT32	(*GetTrafficValueType)(INT32 nItemID, DOUBLE* dTrafficValue);
//--------------------------------------------------------------------------------------------------

class CResUsageInfoDLLUtil * 
{
private:
	HINSTANCE		m_hDLL;

private:
	GetCpuUsageTotalType		f_GetCpuUsageTotal;
	GetMemoryInfoType			f_GetMemoryInfo;
	GetHddInfoType				f_GetHddInfo;
	GetTrafficBandWidhtType		f_GetTrafficBandWidht;
	GetTrafficValueType			f_GetTrafficValue;

public:
	INT32			LoadLibraryExt(LPCTSTR lpDLLName);

public:
	INT32			GetCpuUsageTotal(UINT32* nUsage);
	INT32			GetMemoryInfo(UINT64* nTotalPhySize, UINT64* nTotalPageSize, UINT64* nAvailPhySize, UINT64* nAvailPageSize);
	INT32			GetHddInfo(UINT64* nTotalHddSize, UINT64* nUsageHddSize, LPCTSTR lpDriveName);
	INT32			GetTrafficBandWidht(INT32 nItemID, DOUBLE* dBandWidth);
	INT32			GetTrafficValue(INT32 nItemID, DOUBLE* dTrafficValue);

public:
	CResUsageInfoDLLUtil();
	virtual ~CResUsageInfoDLLUtil();

};

extern CResUsageInfoDLLUtil*		t_ResUsageInfoDLLUtil;

#endif * /*_RES_USAGE_INFO_DLL_UTIL_H__*/
