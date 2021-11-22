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

#ifndef _WHITE_ENGINE_H__

#define _WHITE_ENGINE_H__

extern "C"{

INT32	ASIWENG_Init(PVOID* ppObject);
VOID	ASIWENG_Free(PVOID lpObject);
INT32	ASIWENG_InitDB(PVOID lpObject, char *acLogMsg = NULL);
INT32	ASIWENG_LoadDB(PVOID lpObject, char *acEnginePath, char *acLogMsg = NULL);
INT32	ASIWENG_CreateDB(PVOID lpObject, char *acWhitePath, char *acEnginePath, char *acDbName, char *acLogMsg = NULL);
INT32	ASIWENG_AddFile(PVOID lpObject, char *szPath, char *acLogMsg = NULL);
INT32	ASIWENG_DelFile(PVOID lpObject, char *szPath, char *acLogMsg = NULL);
INT32	ASIWENG_ClearFile(PVOID lpObject);
INT32	ASIWENG_MakeDB(PVOID lpObject, char *acEnginePath, char *acDbName, char *acLogMsg = NULL);
INT32	ASIWENG_UnInitDB(PVOID lpObject);
INT32	ASIWENG_AddWL(PVOID lpObject, char *szPath, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg = NULL);
INT32	ASIWENG_ModWL(PVOID lpObject, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg = NULL);
INT32	ASIWENG_DelWL(PVOID lpObject, char *szPath,  PVOID pWhiteList, DWORD dwListSize, char *acLogMsg = NULL);
INT32	ASIWENG_GetWL(PVOID lpObject, char *szPath, PVOID pWhiteList, DWORD dwListSize, PDWORD pdwWFileType, char *acLogMsg = NULL);
INT32	ASIWENG_GetWLCnt(PVOID lpObject, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg = NULL);
INT32	ASIWENG_GetAllWL(PVOID lpObject, PVOID pPasrseData, DWORD dwDataSize, INT32 nTotalCount, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg = NULL);
INT32	ASIWENG_GetWLDBCnt(PVOID lpObject, char *szEnginePath, INT32 *pnFileCount, char *acLogMsg = NULL);
INT32	ASIWENG_GetAllWLDBHdr(PVOID lpObject, char *szEnginePath, DWORD dwFileCount, PVOID pWhiteHdr, DWORD dwHdrSize, char *acLogMsg = NULL);
INT32	ASIWENG_GetWLDBHdr(PVOID lpObject, char *acEngineFile, PVOID pWhiteHdr, DWORD dwHdrSize, char *acLogMsg = NULL);
INT32	ASIWENG_MakeWLDB(PVOID lpObject, char *szEngineFile, DWORD dwCategory, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg = NULL);
INT32	ASIWENG_LoadWL(PVOID lpObject, char *szEngineFile, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg = NULL);

}

#endif /*_WHITE_ENGINE_H__*/