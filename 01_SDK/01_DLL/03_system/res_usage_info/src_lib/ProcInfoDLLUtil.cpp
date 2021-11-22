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

//
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "ProcInfoDLLUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CPROCINFODLLUtil*	t_ProcInfoDLLUtil		= NULL;

CPROCINFODLLUtil::CPROCINFODLLUtil()
{
	m_hDLL = NULL;
	m_lpObject = NULL;
	m_fInitProcInfo	= NULL;
	m_fFreeProcInfo	= NULL;
	m_fGetProcessResInfo = NULL;
	m_fGetNetWorkResInfo = NULL;
}
//--------------------------------------------------------------------

CPROCINFODLLUtil::~CPROCINFODLLUtil()
{
	FreeLibraryExt();
}
//--------------------------------------------------------------------


INT32 CPROCINFODLLUtil::LoadLibraryExt(LPCTSTR szDLLName)
{
	INT32 nRetVal = 0;


	do{
		if(m_hDLL)
		{
			nRetVal = 0;
			break;
		}

		if(szDLLName == NULL || szDLLName[0] == 0)
		{
			nRetVal = -1;
			break;
		}
		m_hDLL = LoadLibrary(szDLLName);
		if(m_hDLL == NULL)
		{	
			nRetVal = -2;
			break;
		}
		m_fInitProcInfo = (InitProcInfoType)GetProcAddress(m_hDLL, "InitProcInfo");
		if(m_fInitProcInfo == NULL)
		{	
			nRetVal = -3;
			break;
		}
		m_fFreeProcInfo = (FreeProcInfoType)GetProcAddress(m_hDLL, "FreeProcInfo");
		if(m_fFreeProcInfo == NULL)
		{	
			nRetVal = -4;
			break;
		}
		m_fGetProcessResInfo = (GetProcessResInfoType)GetProcAddress(m_hDLL, "GetProcessResInfo");
		if(m_fGetProcessResInfo == NULL)
		{	
			nRetVal = -5;
			break;
		}
		
		m_fGetNetWorkResInfo = (GetNetWorkResInfoType)GetProcAddress(m_hDLL, "GetNetWorkResInfo");
		if(m_fGetNetWorkResInfo == NULL)
		{	
			nRetVal = -6;
			break;
		}
		nRetVal = 0;
	}while(FALSE);

	if(nRetVal != 0)
		FreeLibraryExt();
	return nRetVal;
}
//--------------------------------------------------------------------

INT32		CPROCINFODLLUtil::FreeLibraryExt()
{
	if(m_hDLL)
	{
		FreeLibrary(m_hDLL);
		m_hDLL = NULL;
		m_fInitProcInfo	= NULL;
		m_fFreeProcInfo	= NULL;
		m_fGetProcessResInfo = NULL;
		m_fGetNetWorkResInfo = NULL;
	}
	return 0;
}
//--------------------------------------------------------------------
INT32		CPROCINFODLLUtil::Init()
{
	if(m_hDLL == NULL || m_fInitProcInfo == NULL)
		return -1;

	return m_fInitProcInfo(&m_lpObject);
}
//----------------------------------------------------------------------------------
INT32		CPROCINFODLLUtil::Free()
{
	INT32 nRetVal = 0;
	if(m_hDLL == NULL || m_fFreeProcInfo == NULL)
		return -1;

	nRetVal = m_fFreeProcInfo(m_lpObject);
	if(nRetVal == 0)
		m_lpObject = NULL;
	return nRetVal;
}
//--------------------------------------------------------------------
INT32		CPROCINFODLLUtil::GetProcessInfo(TListResInfoProcInfo& tResInfoProcInfoList)
{
	TListResInfoProcInfoItor begin, end;

	if(m_hDLL == NULL || m_lpObject == NULL || m_fGetProcessResInfo == NULL)
		return -1;

	m_fGetProcessResInfo(m_lpObject, this, (PVOID)CallBack_ProcInfo);
	
	begin = m_tResInfoProcInfoList.begin();	end = m_tResInfoProcInfoList.end();
	for(begin; begin != end; begin++)
	{
		tResInfoProcInfoList.push_back(*begin);
	}
	m_tResInfoProcInfoList.clear();
	return 0;
}
//----------------------------------------------------------------------------------

INT32		CPROCINFODLLUtil::GetNetWorkInfo(TListResInfoNetStat& tResInfoNetStatList)
{
	TListResInfoNetStatItor begin, end;

	if(m_hDLL == NULL || m_lpObject == NULL || m_fGetNetWorkResInfo == NULL)
		return -1;

	m_fGetNetWorkResInfo(m_lpObject, this, (PVOID)CallBack_NetWorkInfo);

	begin = m_tResInfoNetStatList.begin();	end = m_tResInfoNetStatList.end();
	for(begin; begin != end; begin++)
	{
		tResInfoNetStatList.push_back(*begin);
	}

	m_tResInfoNetStatList.clear();
	return 0;
}
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
void CPROCINFODLLUtil::CallBack_ProcInfo(PVOID lParam, ASIRES_INFO_PROCESS_INFO proc_info)
{
	CPROCINFODLLUtil* tResInfoDLLUtil = (CPROCINFODLLUtil*)lParam;
	if(tResInfoDLLUtil)
		tResInfoDLLUtil->m_tResInfoProcInfoList.push_back(proc_info);
}
//---------------------------------------------------------------------------

void CPROCINFODLLUtil::CallBack_NetWorkInfo(PVOID lParam, ASIRES_INFO_NET_STAT tns)
{
	CPROCINFODLLUtil* tResInfoDLLUtil = (CPROCINFODLLUtil*)lParam;
	if(tResInfoDLLUtil)
		tResInfoDLLUtil->m_tResInfoNetStatList.push_back(tns);
}
//---------------------------------------------------------------------------


