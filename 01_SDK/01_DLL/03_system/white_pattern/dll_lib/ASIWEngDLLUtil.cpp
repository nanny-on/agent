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
#include "ASIWEngDLLUtil.h"

CASIWEngDLLUtil*	t_ASIWENGDLLUtil		= NULL;

CASIWEngDLLUtil::CASIWEngDLLUtil()
{
	m_hDLL		= NULL;
	m_lpObject	= NULL;
}
//--------------------------------------------------------------------

CASIWEngDLLUtil::~CASIWEngDLLUtil()
{
	FreeLibraryExt();
}
//--------------------------------------------------------------------

INT32 CASIWEngDLLUtil::LoadLibraryExt(LPCTSTR szDLLName)
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

INT32		CASIWEngDLLUtil::FreeLibraryExt()
{
	if(m_lpObject)
	{
		Free();
	}
	
	if(m_hDLL)
	{
		FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}
	return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

INT32		CASIWEngDLLUtil::Init()
{
	if(!m_hDLL)		return -1;

	ASIWENG_InitType fASIWENG_Init = (ASIWENG_InitType)GetProcAddress(m_hDLL, "ASIWENG_Init");
	if(!fASIWENG_Init)		return -2;

	return fASIWENG_Init(&m_lpObject);
}
//----------------------------------------------------------------------------------

INT32		CASIWEngDLLUtil::Free()
{
	if(!m_hDLL)		return -1;

	ASIWENG_FreeType fASIWENG_Free = (ASIWENG_FreeType)GetProcAddress(m_hDLL, "ASIWENG_Free");
	if(!fASIWENG_Free)		return -2;

	fASIWENG_Free(m_lpObject);
	m_lpObject = NULL;
	return 0;
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::InitDB()
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_InitDBType fASIWENG_InitDB = (ASIWENG_InitDBType)GetProcAddress(m_hDLL, "ASIWENG_InitDB");
	if(!fASIWENG_InitDB)		return -2;

	return fASIWENG_InitDB(m_lpObject, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::LoadDB(LPCTSTR acEnginePath)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_LoadDBType fASIWENG_LoadDB = (ASIWENG_LoadDBType)GetProcAddress(m_hDLL, "ASIWENG_LoadDB");
	if(!fASIWENG_LoadDB)		return -2;

	return fASIWENG_LoadDB(m_lpObject, (LPTSTR)acEnginePath, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::CreateDB(LPCTSTR acWhitePath, LPCTSTR acEnginePath, LPCTSTR acDbName)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_CreateDBType fASIWENG_CreateDB = (ASIWENG_CreateDBType)GetProcAddress(m_hDLL, "ASIWENG_CreateDB");
	if(!fASIWENG_CreateDB)		return -2;

	return fASIWENG_CreateDB(m_lpObject, (LPTSTR)acWhitePath, (LPTSTR)acEnginePath, (LPTSTR)acDbName, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::AddFile(LPCTSTR szPath)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_AddFileType fASIWENG_AddFile = (ASIWENG_AddFileType)GetProcAddress(m_hDLL, "ASIWENG_AddFile");
	if(!fASIWENG_AddFile)		return -2;

	return fASIWENG_AddFile(m_lpObject, (LPTSTR)szPath, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::DelFile(LPCTSTR szPath)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_DelFileType fASIWENG_DelFile = (ASIWENG_DelFileType)GetProcAddress(m_hDLL, "ASIWENG_DelFile");
	if(!fASIWENG_DelFile)		return -2;

	return fASIWENG_DelFile(m_lpObject, (LPTSTR)szPath, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::ClearFile()
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_ClearFileType fASIWENG_ClearFile = (ASIWENG_ClearFileType)GetProcAddress(m_hDLL, "ASIWENG_ClearFile");
	if(!fASIWENG_ClearFile)		return -2;

	fASIWENG_ClearFile(m_lpObject);
	return 0;
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::MakeDB(LPCTSTR acEnginePath, LPCTSTR acDbName)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_MakeDBType fASIWENG_MakeDB = (ASIWENG_MakeDBType)GetProcAddress(m_hDLL, "ASIWENG_MakeDB");
	if(!fASIWENG_MakeDB)		return -2;

	return fASIWENG_MakeDB(m_lpObject, (LPTSTR)acEnginePath, (LPTSTR)acDbName, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::UnInitDB()
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_UnInitDBType fASIWENG_UnInitDB = (ASIWENG_UnInitDBType)GetProcAddress(m_hDLL, "ASIWENG_UnInitDB");
	if(!fASIWENG_UnInitDB)		return -2;

	fASIWENG_UnInitDB(m_lpObject);
	return 0;
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::AddWL(LPCTSTR szPath, PVOID pWhiteList, DWORD dwListSize)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_AddWLType fASIWENG_AddWL = (ASIWENG_AddWLType)GetProcAddress(m_hDLL, "ASIWENG_AddWL");
	if(!fASIWENG_AddWL)		return -2;

	return fASIWENG_AddWL(m_lpObject, (LPTSTR)szPath, pWhiteList, dwListSize, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::ModWL(PVOID pWhiteList, DWORD dwListSize)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_ModWLType fASIWENG_ModWL = (ASIWENG_ModWLType)GetProcAddress(m_hDLL, "ASIWENG_ModWL");
	if(!fASIWENG_ModWL)		return -2;

	return fASIWENG_ModWL(m_lpObject, pWhiteList, dwListSize, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::DelWL(LPCTSTR szPath,  PVOID pWhiteList, DWORD dwListSize)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_DelWLType fASIWENG_DelWL = (ASIWENG_DelWLType)GetProcAddress(m_hDLL, "ASIWENG_DelWL");
	if(!fASIWENG_DelWL)		return -2;

	return fASIWENG_DelWL(m_lpObject, (LPTSTR)szPath, pWhiteList, dwListSize, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::GetWL(LPCTSTR szPath, PVOID pWhiteList, DWORD dwListSize, PDWORD pdwWFileType)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_GetWLType fASIWENG_GetWL = (ASIWENG_GetWLType)GetProcAddress(m_hDLL, "ASIWENG_GetWL");
	if(!fASIWENG_GetWL)		return -2;
	
	return fASIWENG_GetWL(m_lpObject, (LPTSTR)szPath, pWhiteList, dwListSize, pdwWFileType, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::GetWLCnt(PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_GetWLCntType fASIWENG_GetWLCnt = (ASIWENG_GetWLCntType)GetProcAddress(m_hDLL, "ASIWENG_GetWLCnt");
	if(!fASIWENG_GetWLCnt)		return -2;

	return fASIWENG_GetWLCnt(m_lpObject, pPasrseData, dwDataSize, pnCount, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::GetAllWL(PVOID pPasrseData, DWORD dwDataSize, INT32 nTotalCount, PVOID pWhiteList, DWORD dwListSize)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_GetAllWLType fASIWENG_GetAllWL = (ASIWENG_GetAllWLType)GetProcAddress(m_hDLL, "ASIWENG_GetAllWL");
	if(!fASIWENG_GetAllWL)		return -2;

	return fASIWENG_GetAllWL(m_lpObject, pPasrseData, dwDataSize, nTotalCount, pWhiteList, dwListSize, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::GetWLDBCnt(LPCTSTR szEnginePath, INT32 *pnFileCount)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_GetWLDBCntType fASIWENG_GetWLDBCnt = (ASIWENG_GetWLDBCntType)GetProcAddress(m_hDLL, "ASIWENG_GetWLDBCnt");
	if(!fASIWENG_GetWLDBCnt)		return -2;

	return fASIWENG_GetWLDBCnt(m_lpObject, (LPTSTR)szEnginePath, pnFileCount, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::GetAllWLDBHdr(LPCTSTR szEnginePath, INT32 nFileCount, PVOID pWhiteHdr, DWORD dwHdrSize)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_GetAllWLDBHdrType fASIWENG_GetAllWLDBHdr = (ASIWENG_GetAllWLDBHdrType)GetProcAddress(m_hDLL, "ASIWENG_GetAllWLDBHdr");
	if(!fASIWENG_GetAllWLDBHdr)		return -2;

	return fASIWENG_GetAllWLDBHdr(m_lpObject, (LPTSTR)szEnginePath, nFileCount, pWhiteHdr, dwHdrSize, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::GetWLDBHdr(LPCTSTR acEngineFile, PVOID pWhiteHdr, DWORD dwHdrSize)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_GetWLDBHdrType fASIWENG_GetWLDBHdr = (ASIWENG_GetWLDBHdrType)GetProcAddress(m_hDLL, "ASIWENG_GetWLDBHdr");
	if(!fASIWENG_GetWLDBHdr)		return -2;

	return fASIWENG_GetWLDBHdr(m_lpObject, (LPTSTR)acEngineFile, pWhiteHdr, dwHdrSize, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::MakeWLDB(LPCTSTR szEngineFile, DWORD dwCategory, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_MakeWLDBType fASIWENG_MakeWLDB = (ASIWENG_MakeWLDBType)GetProcAddress(m_hDLL, "ASIWENG_MakeWLDB");
	if(!fASIWENG_MakeWLDB)		return -2;

	return fASIWENG_MakeWLDB(m_lpObject, (LPTSTR)szEngineFile, dwCategory, pPasrseData, dwDataSize, pnCount, m_szErrMsg);
}
//----------------------------------------------------------------------------------

INT32	CASIWEngDLLUtil::LoadWL(LPCTSTR szEngineFile, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount)
{
	memset(m_szErrMsg, 0, CHAR_MAX_SIZE);

	if(!m_hDLL)		return -1;

	ASIWENG_LoadWLType fASIWENG_LoadWL = (ASIWENG_LoadWLType)GetProcAddress(m_hDLL, "ASIWENG_LoadWL");
	if(!fASIWENG_LoadWL)		return -2;

	return fASIWENG_LoadWL(m_lpObject, (LPTSTR)szEngineFile, pPasrseData, dwDataSize, pnCount, m_szErrMsg);
}
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

