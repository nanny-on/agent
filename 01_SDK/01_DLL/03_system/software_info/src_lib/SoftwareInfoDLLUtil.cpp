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
 */

#include "stdafx.h"
#include "com_struct.h"
#include "SoftwareInfoDLLUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASISIDLLUtil*	t_ASISIDLLUtil = NULL;

CASISIDLLUtil::CASISIDLLUtil()
{
	m_hDLL = NULL;
	m_lpObject	= NULL;
}
//--------------------------------------------------------------------

CASISIDLLUtil::~CASISIDLLUtil()
{
	FreeLibraryExt();
}
//--------------------------------------------------------------------

INT32 CASISIDLLUtil::LoadLibraryExt(LPCTSTR szDLLName)
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

INT32		CASISIDLLUtil::FreeLibraryExt()
{
	if(m_hDLL)
	{
		FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}
	return 0;
}
//--------------------------------------------------------------------

INT32		CASISIDLLUtil::Init(PASISI_INIT pas_init)
{
	if(!m_hDLL)
		return -1;

	ASISI_InitType fASISI_Init = (ASISI_InitType)GetProcAddress(m_hDLL, "ASISI_Init");
	if(!fASISI_Init)
		return -2;

	return fASISI_Init(&m_lpObject, pas_init, sizeof(ASISI_INIT));
}
//----------------------------------------------------------------------------------

INT32		CASISIDLLUtil::Free()
{
	if(!m_hDLL)
		return -1;

	ASISI_FreeType fASISI_Free = (ASISI_FreeType)GetProcAddress(m_hDLL, "ASISI_Free");
	if(!fASISI_Free)
		return -2;

	return fASISI_Free(&m_lpObject);
}
//----------------------------------------------------------------------------------

INT32		CASISIDLLUtil::GetSwInfo(INT32 nProcArchitect, GetSwInfoType cbGetSwInfo, PVOID lParam)
{
	if(!m_hDLL)
		return -1;

	ASISI_GetSwInfoType fASISI_GetSwInfo = (ASISI_GetSwInfoType)GetProcAddress(m_hDLL, "ASISI_GetSwInfo");
	if(!fASISI_GetSwInfo)
		return -2;

	return fASISI_GetSwInfo(m_lpObject, nProcArchitect, cbGetSwInfo, lParam);
}
