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

// ResInfo.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"
#include "com_struct.h"
#include "ResInfo.h"
#include "NetworkInfo.h"
#include "ResInfoUtil.h"


//-----------------------------------------------------------------------------
INT32	InitProcInfo(PVOID* lpObject)
{
	CResInfoUtil* tResInfoUtil = NULL;
	if(lpObject == NULL)
		return -1;
	
	tResInfoUtil = new CResInfoUtil();
	if(tResInfoUtil == NULL)
		return -2;
	*lpObject = (PVOID)tResInfoUtil;
	return 0;
}
//----------------------------------------------------------------------------
INT32	FreeProcInfo(PVOID lpObject)
{
	CResInfoUtil* tResInfoUtil = NULL; 
	if(lpObject == NULL)
		return -1;

	tResInfoUtil = (CResInfoUtil*)lpObject;
	delete tResInfoUtil;
	return 0;
}
//----------------------------------------------------------------------------
INT32 GetProcessResInfo(PVOID lpObject, LPVOID lParam, CallBack_ProcessInfo fnCallback)
{
	CResInfoUtil* tResInfoUtil = NULL; 
	if(lpObject == NULL)
		return -1;
	
	tResInfoUtil = (CResInfoUtil*)lpObject;
	tResInfoUtil->GetProcessResInfo(lParam, fnCallback);
	return 0;
}
//----------------------------------------------------------------------------- 
INT32	GetNetWorkResInfo(PVOID lpObject, LPVOID lParam, CallBack_NetWorkInfo fnCallback)
{	
	CResInfoUtil* tResInfoUtil = NULL; 
	if(lpObject == NULL)
		return -1;
	tResInfoUtil = (CResInfoUtil*)lpObject;
	tResInfoUtil->GetNetWorkResInfo(lParam, fnCallback);
	return 0;
}
