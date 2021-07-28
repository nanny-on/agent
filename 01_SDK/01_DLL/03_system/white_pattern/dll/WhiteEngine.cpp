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
 */
#include "stdafx.h"
#include "com_struct.h"
#include "WhiteEngine.h"
#include "WhiteEngineUtil.h"

INT32	ASIWENG_Init(PVOID* ppObject)
{
	if(ppObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = new CWhiteEngineUtil();
	if(tWEUtil == NULL)
		return -2;

	*ppObject = (PVOID)tWEUtil;
	return 0;
}
//---------------------------------------------------------------------------------

VOID	ASIWENG_Free(PVOID lpObject)
{
	CWhiteEngineUtil* tWEUtil = NULL;
	if(lpObject != NULL)
	{
		tWEUtil = (CWhiteEngineUtil*)lpObject;
		delete tWEUtil;
	}
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_InitDB(PVOID lpObject, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->InitDB(acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_LoadDB(PVOID lpObject, char *acEnginePath, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->LoadDB(acEnginePath, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_CreateDB(PVOID lpObject, char *acWhitePath, char *acEnginePath, char *acDbName, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->CreateDB(acWhitePath, acEnginePath, acDbName, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_AddFile(PVOID lpObject, char *szPath, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->AddFile(szPath, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_DelFile(PVOID lpObject, char *szPath, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->DelFile(szPath, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_ClearFile(PVOID lpObject)
{
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	tWEUtil->ClearFile();
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_MakeDB(PVOID lpObject, char *acEnginePath, char *acDbName, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->MakeDB(acEnginePath, acDbName, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_UnInitDB(PVOID lpObject)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	tWEUtil->UnInitDB();
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_AddWL(PVOID lpObject, char *szPath, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->AddWL(szPath, pWhiteList, dwListSize, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_ModWL(PVOID lpObject, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->ModWL(pWhiteList, dwListSize, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_DelWL(PVOID lpObject, char *szPath,  PVOID pWhiteList, DWORD dwListSize, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->DelWL(szPath, pWhiteList, dwListSize, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_GetWL(PVOID lpObject, char *szPath, PVOID pWhiteList, DWORD dwListSize, PDWORD pdwWFileType, char *acLogMsg)
{
	INT32 nRetVal = 0;
	DWORD dwWhiteType = AS_INVALID_FILE;
	if(lpObject == NULL || pdwWFileType == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->GetWL(szPath, pWhiteList, dwListSize, &dwWhiteType, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	*pdwWFileType = dwWhiteType;
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_GetWLCnt(PVOID lpObject, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->GetWLCnt(pPasrseData, dwDataSize, pnCount, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_GetAllWL(PVOID lpObject, PVOID pPasrseData, DWORD dwDataSize, INT32 nTotalCount, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->GetAllWL(pPasrseData, dwDataSize, nTotalCount, pWhiteList, dwListSize, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_GetWLDBCnt(PVOID lpObject, char *szEnginePath, INT32 *pnFileCount, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->GetWLDBCnt(szEnginePath, pnFileCount, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_GetAllWLDBHdr(PVOID lpObject, char *szEnginePath, DWORD dwFileCount, PVOID pWhiteHdr, DWORD dwHdrSize, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->GetAllWLDBHdr(szEnginePath, dwFileCount, pWhiteHdr, dwHdrSize, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_GetWLDBHdr(PVOID lpObject, char *acEngineFile, PVOID pWhiteHdr, DWORD dwHdrSize, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->GetWLDBHdr(acEngineFile, pWhiteHdr, dwHdrSize, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_MakeWLDB(PVOID lpObject, char *szEngineFile, DWORD dwCategory, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->MakeWLDB(szEngineFile, dwCategory, pPasrseData, dwDataSize, pnCount, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIWENG_LoadWL(PVOID lpObject, char *szEngineFile, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg)
{
	INT32 nRetVal = 0;
	if(lpObject == NULL)
		return -1;

	CWhiteEngineUtil* tWEUtil = (CWhiteEngineUtil*)lpObject;

	nRetVal = tWEUtil->LoadWL(szEngineFile, pPasrseData, dwDataSize, pnCount, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------------