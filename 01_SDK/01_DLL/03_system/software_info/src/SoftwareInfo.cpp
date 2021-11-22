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
#include "SoftwareInfo.h"
#include "SwInfoUtil.h"


INT32	ASISI_Init(PVOID* lpObject, PASISI_INIT pasisi_init, UINT32 nSize)
{
	INT32 nRtn = 0;
	try
	{

		CSwInfoUtil* tSwInfoUtil = new CSwInfoUtil();
		if(!tSwInfoUtil)
			return -1;

		*lpObject = (PVOID)tSwInfoUtil;
	}
	catch(...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//----------------------------------------------------------------------------

INT32	ASISI_Free(PVOID* lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!(*lpObject))
			return -100;

		CSwInfoUtil* tSwInfoUtil = (CSwInfoUtil*)(*lpObject);
		delete tSwInfoUtil;

		*lpObject = NULL;
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//----------------------------------------------------------------------------

INT32	ASISI_GetSwInfo(PVOID lpObject, INT32 nProcArchitect, GetSwInfoType cbGetSwInfo, PVOID lParam)
{	
	INT32 nRetVal = 0;
	PASISI_SW_INFO pSwInfo = NULL;
	CSwInfoUtil* tSwInfoUtil = NULL;
	TListSwInfoItor begin, end;
	TListSwInfo tSwInfoList;
	tSwInfoUtil = (CSwInfoUtil*)lpObject;
	if(tSwInfoUtil == NULL || cbGetSwInfo == NULL)
		return -100;
	
	nRetVal = tSwInfoUtil->GetSwList(nProcArchitect, tSwInfoList);
	if(nRetVal != 0)
	{
		nRetVal -= 200;
		return nRetVal;
	}

	begin = tSwInfoList.begin();	end = tSwInfoList.end();
	for(begin; begin != end; begin++)
	{
		pSwInfo = (PASISI_SW_INFO)&(*begin);
		if(pSwInfo != NULL)
			cbGetSwInfo(pSwInfo, sizeof(ASISI_SW_INFO), lParam);
	}
	return 0;
}
//----------------------------------------------------------------------------
