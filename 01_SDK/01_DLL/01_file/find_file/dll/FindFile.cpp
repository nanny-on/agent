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

// FindFile.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "com_struct.h"
#include "com_struct_find_work.h"
#include "FindFileUtil.h"
#include "FindFile.h"

// The one and only application object

INT32 ASIFF_Init(PVOID* ppObject, PASI_FF_INIT_INFO pAFII, UINT32 nSize)
{
	UINT32 dwLen = 0;
	CFindFileUtil* tFFUtil = NULL;
	ASI_FF_INIT_INFO afii;

	if(ppObject == NULL || pAFII == NULL)
		return -1;

	memset(&afii, 0, sizeof(afii));

	if(nSize > sizeof(afii))
		dwLen = sizeof(afii);
	else
		dwLen = nSize;
	memcpy(&afii, pAFII, dwLen);
	
	tFFUtil = new CFindFileUtil();
	if(tFFUtil == NULL)
	{
		return -2;
	}
	tFFUtil->Init(&afii);
	*ppObject = (PVOID)tFFUtil;
	return 0;
}
//-------------------------------------------------------------

INT32 ASIFF_Free(PVOID lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;
		tFFUtil->Release();
		delete tFFUtil;
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_DeleteFindFileWork(PVOID lpObject, UINT32 nOrderID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->DeleteFindFileWork(nOrderID);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_AddSearchDirPath(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpSearchPath)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->AddSearchDirPath(nOrderID, lpSearchPath);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_ClearSearchDirPath(PVOID lpObject, UINT32 nOrderID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->ClearSearchDirPath(nOrderID);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

INT32 ASIFF_AddFileMask(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpFileMask)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->AddFileMask(nOrderID, lpFileMask);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_DelFileMask(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpFileMask)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->DelFileMask(nOrderID, lpFileMask);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_ClearFileMask(PVOID lpObject, UINT32 nOrderID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->ClearFileMask(nOrderID);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------


INT32 ASIFF_AddExceptPath(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpExceptPath)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->AddExceptDir(nOrderID, lpExceptPath);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_DelExceptPath(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpExceptPath)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->DelExceptDir(nOrderID, lpExceptPath);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_ClearExceptPath(PVOID lpObject, UINT32 nOrderID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->ClearExceptDir(nOrderID);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

INT32	ASIFF_AddExceptPathPtn(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpExceptPathPtn)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->AddExceptDirFileMask(nOrderID, lpExceptPathPtn);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}

INT32	ASIFF_DelExceptPathPtn(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpExceptPathPtn)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->DelExceptDirFileMask(nOrderID, lpExceptPathPtn);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}

INT32	ASIFF_ClearExceptPathPtn(PVOID lpObject, UINT32 nOrderID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->ClearExceptDirFileMask(nOrderID);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

INT32	ASIFF_AddFileDateTime(PVOID lpObject, UINT32 nOrderID, UINT32 nType, UINT32 nChkTime)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->AddFileDateTime(nOrderID, nType, nChkTime);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32	ASIFF_DelFileDateTime(PVOID lpObject, UINT32 nOrderID, UINT32 nType)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->DelFileDateTime(nOrderID, nType);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32	ASIFF_ClearFileDateTime(PVOID lpObject, UINT32 nOrderID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->ClearFileDateTime(nOrderID);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

INT32	ASIFF_AddFileFindOption(PVOID lpObject, UINT32 nOrderID, UINT32 nFindOption)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->AddFileFindOption(nOrderID, nFindOption);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32	ASIFF_ClearFileFindOption(PVOID lpObject, UINT32 nOrderID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->ClearFileFindOption(nOrderID);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

INT32 ASIFF_SearchDirFile(PVOID lpObject, UINT32 nOrderID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->SearchDirFile(nOrderID);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_SearchDirFileThread(PVOID lpObject, UINT32 nOrderID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->SearchDirFileThread(nOrderID);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_GetFindFileItem(PVOID lpObject, UINT32 nOrderID, PASI_FF_FILE_ITEM pAFFIList, PUINT32 nListSize, PASI_FF_FILE_RESULT pAFFR)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->GetFindFileItem(nOrderID, pAFFIList, nListSize, pAFFR);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_IsDocFileFormat(PVOID lpObject, LPCTSTR pFilePath, INT32 *pnFileType)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->IsDocFileFormat(pFilePath, pnFileType);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------


INT32 ASIFF_StopFindFileWork(PVOID lpObject, UINT32 nOrderID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->Stop(nOrderID);
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}
//-------------------------------------------------------------

INT32 ASIFF_StopFindFileWorkAll(PVOID lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)	return -100;

		CFindFileUtil* tFFUtil = (CFindFileUtil*)lpObject;

		return tFFUtil->Stop();
	}
	catch (...)
	{
		nRtn = -1001;
	}
	return nRtn;
}



