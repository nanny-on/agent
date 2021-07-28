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

//
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "DeviceInfoDLLUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDEVINFODLLUtil*	t_DevInfoDLLUtil	= NULL;

CDEVINFODLLUtil::CDEVINFODLLUtil()
{
	m_hDLL		= NULL;
	m_lpObject	= NULL;
}
//--------------------------------------------------------------------

CDEVINFODLLUtil::~CDEVINFODLLUtil()
{
	FreeLibraryExt();
}
//--------------------------------------------------------------------

INT32 CDEVINFODLLUtil::LoadLibraryExt(LPCTSTR szDLLName)
{
	if(m_hDLL)
		return 0;

	if( (m_hDLL = LoadLibrary(szDLLName)) == NULL)
	{	
		return -1;
	}
	return 0;
}
//--------------------------------------------------------------------

INT32		CDEVINFODLLUtil::FreeLibraryExt()
{
	if(m_hDLL)
	{
		FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}
	return 0;
}
//--------------------------------------------------------------------
INT32		CDEVINFODLLUtil::Init()
{
	if(!m_hDLL)
		return -1;

	ASIDVI_InitType fASIDVI_Init = (ASIDVI_InitType)GetProcAddress(m_hDLL, "InitDevInfo");
	if(!fASIDVI_Init)
		return -2;

	return fASIDVI_Init(&m_lpObject);
}
//----------------------------------------------------------------------------------
INT32		CDEVINFODLLUtil::Free()
{
	if(!m_hDLL)
		return -1;

	ASIDVI_FreeType fASIDVI_Free = (ASIDVI_FreeType)GetProcAddress(m_hDLL, "FreeDevInfo");
	if(!fASIDVI_Free)
		return -2;

	return fASIDVI_Free(m_lpObject);
}
//--------------------------------------------------------------------

INT32		CDEVINFODLLUtil::GetDeviceInfo(TListDeviceInfo& tDeviceInfoList)
{
	if(!m_hDLL)		return -1;

	ASIDVI_EnumDeviceInfoType fASIDVI_DeviceInfo = (ASIDVI_EnumDeviceInfoType)GetProcAddress(m_hDLL, "EnumDeviceInfoList");
	if(!fASIDVI_DeviceInfo)		return -2;

//	fASIDVI_DeviceInfo(m_lpObject, this, CallBack_DeviceInfo);

	TListDeviceInfoItor begin, end;
	begin = m_tDeviceInfoList.begin();	end = m_tDeviceInfoList.end();
	for(begin; begin != end; begin++)
	{
		tDeviceInfoList.push_back(*begin);
	}

	m_tDeviceInfoList.clear();
	return 0;
}
//----------------------------------------------------------------------------------

