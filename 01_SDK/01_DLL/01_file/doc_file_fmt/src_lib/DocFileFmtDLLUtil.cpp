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

//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "com_struct.h"
#include "DocFileFmtDLLUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASIDFFDLLUtil*	t_ASIDFFDLLUtil;

//--------------------------------------------------------------------------------------------

CASIDFFDLLUtil::CASIDFFDLLUtil()
{
	m_hDll = NULL;
}
//--------------------------------------------------------------------------------------------

CASIDFFDLLUtil::~CASIDFFDLLUtil()
{
	FreeLibraryExt();
}
//--------------------------------------------------------------------------------------------

INT32			CASIDFFDLLUtil::FreeLibraryExt()
{
	if(m_hDll)
	{
		FreeLibrary(m_hDll);
		m_hDll					= NULL;
	}

	return 0;
}
//--------------------------------------------------------------------------------------------

INT32			CASIDFFDLLUtil::LoadLibraryExt(LPCTSTR lpDLLPath)
{
	m_hDll = LoadLibrary(lpDLLPath);
	if(!m_hDll)	return -1;
	
	return 0;
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

INT32			CASIDFFDLLUtil::ASIDFF_SetDFFmtInit(PASI_DFILE_FMT_INIT pADFFI)
{
	if(!m_hDll)		return -1;

	ASIDFF_SetDFFmtInitType f_ASIDFF_SetDFFmtInit = (ASIDFF_SetDFFmtInitType)GetProcAddress(m_hDll, "ASIDF_SetDFFmtInit");
	if(!f_ASIDFF_SetDFFmtInit)
		return -1;

	return f_ASIDFF_SetDFFmtInit(pADFFI);
}


//--------------------------------------------------------------------------------------------

INT32			CASIDFFDLLUtil::ASIDFF_GetDFFmtInfo(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg)
{
	if(!m_hDll)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid library address");
		return -1;
	}

	ASIDFF_GetDFFmtInfoType f_ASIDFF_GetDFFmtInfo = (ASIDFF_GetDFFmtInfoType)GetProcAddress(m_hDll, "ASIDF_GetDFFmtInfo");
	if(!f_ASIDFF_GetDFFmtInfo)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to get ASIDF_GetDFFmtInfo address (%d)", errno);
		return -2;
	}

	return f_ASIDFF_GetDFFmtInfo(pADFFI, acLogMsg);
}


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------