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


#ifndef COM_HEADER_DBMS_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_DBMS_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


//////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#define		ASI_DBMS_TYPE_NOT_USED		0
#define     ASI_DBMS_TYPE_MSSQL			1
#define     ASI_DBMS_TYPE_MYSQL			100
#define     ASI_DBMS_TYPE_MYSQL_51		101
#define     ASI_DBMS_TYPE_MYSQL_52		102
#define		ASI_DBMS_TYPE_MDB			200
#define		ASI_DBMS_TYPE_EXCEL			300
#define		ASI_DBMS_TYPE_DSN			400

//---------------------------------------------------------------------------

typedef struct _asi_dbms_init
{
	_asi_dbms_init()
	{
		nDBMSType		= 0;
		nPort			= 0;
		ZeroMemoryExt(szAddress);
		ZeroMemoryExt(szDBName);
		ZeroMemoryExt(szAccountID);
		ZeroMemoryExt(szAccountPW);
		ZeroMemoryExt(szConnectString);
		ZeroMemoryExt(szLogPath);
		ZeroMemoryExt(szLogFile);
		nRemainLog		= 0;
	}
	UINT32	nDBMSType;
	CHAR	szAddress[CHAR_MAX_SIZE];
	UINT32	nPort;
	CHAR	szDBName[CHAR_MAX_SIZE];
	CHAR	szAccountID[CHAR_MAX_SIZE];
	CHAR	szAccountPW[CHAR_MAX_SIZE];
	CHAR	szConnectString[CHAR_MAX_SIZE];	
	CHAR	szLogPath[CHAR_MAX_SIZE];	
	CHAR	szLogFile[CHAR_MAX_SIZE];	
	UINT32	nRemainLog;
}ASI_DBMS_INIT, *PASI_DBMS_INIT;

//---------------------------------------------------------------------------

typedef INT32 (*ASIDBMS_InitType)(PVOID* lpObject, PASI_DBMS_INIT pADI, UINT32 nSize);
typedef INT32 (*ASIDBMS_FreeType)(PVOID* lpObject);
typedef INT32 (*ASIDBMS_OpenType)(PVOID lpObject);
typedef INT32 (*ASIDBMS_CloseType)(PVOID lpObject);

typedef INT32 (*ASIDBMS_GetErrMsgType)(PVOID lpObject, LPTSTR lpErrMsg, UINT32 nBufLen);
typedef INT32 (*ASIDBMS_GetLastInsertIDType)(PVOID lpObject, UINT32& nValue);

typedef INT32 (*ASIDBMS_SetColIdxNameType)(PVOID lpObject, LPCTSTR lpColNames);
typedef INT32 (*ASIDBMS_ExecuteQueryType)(PVOID lpObject, LPCTSTR lpQuery);
typedef INT32 (*ASIDBMS_IsExistNextType)(PVOID lpObject);
typedef INT32 (*ASIDBMS_NextType)(PVOID lpObject);

typedef INT32 (*ASIDBMS_GetField_UINT64Type)(PVOID lpObject, UINT32 nIdx, UINT64& nValue);
typedef INT32 (*ASIDBMS_GetField_INT64Type)(PVOID lpObject, UINT32 nIdx, INT64& nValue);
typedef INT32 (*ASIDBMS_GetField_UINT32Type)(PVOID lpObject, UINT32 nIdx, UINT32& nValue);
typedef INT32 (*ASIDBMS_GetField_INT32Type)(PVOID lpObject, UINT32 nIdx, INT32& nValue);
typedef INT32 (*ASIDBMS_GetField_StringType)(PVOID lpObject, UINT32 nIdx, LPCTSTR* lpValue);

//////////////////////////////////////////////////////////////////////////

#endif //COM_HEADER_DBMS_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
