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

#ifndef _WHITE_ENGINE_UTIL__
#define _WHITE_ENGINE_UTIL__

#include "WhitePatternUtil.h"

class CWhiteEngineUtil
{
private:
	CWhitePatternUtil	m_tWPtnUtil;

public:
	INT32	InitDB(char *acLogMsg = NULL);
	INT32	LoadDB(char *acEnginePath, char *acLogMsg = NULL);
	INT32	CreateDB(char *acWhitePath, char *acEnginePath, char *acDbName, char *acLogMsg = NULL);
	INT32	AddFile(char *szPath, char *acLogMsg = NULL);
	INT32	DelFile(char *szPath, char *acLogMsg = NULL);
	void	ClearFile();
	INT32	MakeDB(char *acEnginePath, char *acDbName, char *acLogMsg = NULL);
	void	UnInitDB();
	INT32	AddWL(char *szPath, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg = NULL);
	INT32	ModWL(PVOID pWhiteList, DWORD dwListSize, char *acLogMsg = NULL);
	INT32	DelWL(char *szPath,  PVOID pWhiteList, DWORD dwListSize, char *acLogMsg = NULL);
	INT32	GetWL(char *szPath, PVOID pWhiteList, DWORD dwListSize, DWORD *pdwWhiteType, char *acLogMsg = NULL);
	INT32	GetWLCnt(PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg = NULL);
	INT32	GetAllWL(PVOID pPasrseData, DWORD dwDataSize, INT32 nTotalCount, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg = NULL);
	INT32	GetWLDBCnt(char *szEnginePath, INT32 *pnFileCount, char *acLogMsg = NULL);
	INT32	GetAllWLDBHdr(char *szEnginePath, INT32 nFileCount, PVOID pWhiteHdr, DWORD dwHdrSize, char *acLogMsg = NULL);
	INT32	GetWLDBHdr(char *acEngineFile, PVOID pWhiteHdr, DWORD dwHdrSize, char *acLogMsg = NULL);
	INT32	MakeWLDB(char *szEngineFile, DWORD dwCategory, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg = NULL);
	INT32	LoadWL(char *szEngineFile, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg = NULL);

public:
	CWhiteEngineUtil();
	~CWhiteEngineUtil();
};

#endif