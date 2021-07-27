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
 */


#ifndef _PROC_INFO_DLL_UTIL_H__

#define _PROC_INFO_DLL_UTIL_H__


//------------------------------------------------------
#include "com_header_ri_dll.h"
//------------------------------------------------------


class CPROCINFODLLUtil
{
private:
	HINSTANCE	m_hDLL;
	PVOID		m_lpObject;
	InitProcInfoType m_fInitProcInfo;
	FreeProcInfoType m_fFreeProcInfo;
	GetProcessResInfoType m_fGetProcessResInfo;
	GetNetWorkResInfoType m_fGetNetWorkResInfo;
	
public:
	INT32		LoadLibraryExt(LPCTSTR szDLLPath);
	INT32		FreeLibraryExt();

public:
	INT32		Init();
	INT32		Free();

	INT32		GetProcessInfo(TListResInfoProcInfo& tResInfoProcInfoList);
	INT32		GetNetWorkInfo(TListResInfoNetStat& tResInfoNetStatList);

public:
	static void CallBack_ProcInfo(PVOID lParam, ASIRES_INFO_PROCESS_INFO proc_info);
	static void CallBack_NetWorkInfo(PVOID lParam, ASIRES_INFO_NET_STAT tns);

public:
	TListResInfoProcInfo		m_tResInfoProcInfoList;
	TListResInfoNetStat			m_tResInfoNetStatList;

public:
	CPROCINFODLLUtil();
	virtual ~CPROCINFODLLUtil();

};

extern CPROCINFODLLUtil*	t_ProcInfoDLLUtil;

#endif // _PROC_INFO_DLL_UTIL_H__
