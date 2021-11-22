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
#include "asi_lnx_define_w32.h"
#include "asi_glb_w32_handle.h"

//------------------------------------------------------------------------------------------
TMapID64 glb_tHandleMap;
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

BOOL	CloseHandle(HANDLE hObject)
{
	DWORD dwFlag = 0;
	if(GetHandleInformation(hObject, &dwFlag) == FALSE)	return FALSE;

	UINT32 nHandle32 = (UINT32)(UINT64)(hObject);

	switch(dwFlag)
	{
		case HANDLE_TYPE_FILE:	close(nHandle32);	break;
	}

	glb_tHandleMap.erase(UINT64(hObject));
	return TRUE;
}
//------------------------------------------------------------------------------------------

BOOL	GetHandleInformation(HANDLE hObject, LPDWORD lpdwFlags)
{
	UINT64 nHandle = (UINT64)(hObject);
	TMapID64Itor find = glb_tHandleMap.find(nHandle);
	if(find == glb_tHandleMap.end())	return FALSE;

	if(!lpdwFlags)	return FALSE;

	*lpdwFlags = HIUINT64(find->second);
	return TRUE;
}
//------------------------------------------------------------------------------------------

BOOL	SetHandleInformation(HANDLE hObject, DWORD dwMask, DWORD dwFlags)
{
	UINT64 nHandle = (UINT64)(hObject);
	TMapID64Itor find = glb_tHandleMap.find(nHandle);
	if(find != glb_tHandleMap.end())	return FALSE;

	glb_tHandleMap[nHandle] = MAKEUINT64(dwMask, dwFlags);
	return TRUE;
}
//------------------------------------------------------------------------------------------



