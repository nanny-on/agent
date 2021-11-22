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
 * * 
 */

// ASIDVIDLLUtil.h: interface for the CDEVINFODLLUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASIRIDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_)
#define AFX_ASIRIDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_


//------------------------------------------------------
#include "com_header_dvi_dll.h"
//------------------------------------------------------


class CDEVINFODLLUtil
{
private:
	HINSTANCE	m_hDLL;
	PVOID		m_lpObject;

public:
	INT32		LoadLibraryExt(LPCTSTR szDLLPath);
	INT32		FreeLibraryExt();

public:
	INT32		Init();
	INT32		Free();

	INT32		GetDeviceInfo(TListDeviceInfo& tDeviceInfoList);


public:
	TListDeviceInfo				m_tDeviceInfoList;

public:
	CDEVINFODLLUtil();
	virtual ~CDEVINFODLLUtil();

};

extern CDEVINFODLLUtil*	t_DevInfoDLLUtil;

#endif // !defined(AFX_ASIRIDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_)
