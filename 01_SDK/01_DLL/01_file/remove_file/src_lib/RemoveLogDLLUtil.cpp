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

// RemoveLogDLLUtil.cpp: implementation of the CRemoveLogDLLUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "RemoveLogDLLUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRemoveLogDLLUtil*	t_RemoveLogDLLUtil = NULL;
//----------------------------------------------------------------------------------

CRemoveLogDLLUtil::CRemoveLogDLLUtil()
{
	m_hDLL = NULL;

	f_RemoveFileByDT 		= NULL;
    f_RemoveFileByLastDay	= NULL;
}
//----------------------------------------------------------------------------------

CRemoveLogDLLUtil::~CRemoveLogDLLUtil()
{

}
//----------------------------------------------------------------------------------

INT32		CRemoveLogDLLUtil::LoadLibraryExt(LPCTSTR lpDLLPath)
{
	if((m_hDLL = LoadLibrary(lpDLLPath)) == NULL)	return -1;
	
	if((f_RemoveFileByDT = (RemoveFileByDTType)GetProcAddress(m_hDLL, "RemoveFileByDT")) == NULL)	return -11;
    if((f_RemoveFileByLastDay = (RemoveFileByLastDayType)GetProcAddress(m_hDLL, "RemoveFileByLastDay")) == NULL)	return -12;
	return 0;
}
//----------------------------------------------------------------------------------

INT32	CRemoveLogDLLUtil::RemoveFileByDT(LPCTSTR lpFindPath, UINT32 nChkType, UINT32 nDay)
{
	if(f_RemoveFileByDT == NULL)
		return -1;

	return f_RemoveFileByDT(lpFindPath, nChkType, nDay);
}
//----------------------------------------------------------------------------------   

void		CRemoveLogDLLUtil::RemoveFileByLastDay(LPCTSTR lpFindPath, UINT32 nChkType, UINT32 nDay)
{
	if(f_RemoveFileByLastDay == NULL)	return;

	f_RemoveFileByLastDay(lpFindPath, nChkType, nDay);
	return;
}
//----------------------------------------------------------------------------------
