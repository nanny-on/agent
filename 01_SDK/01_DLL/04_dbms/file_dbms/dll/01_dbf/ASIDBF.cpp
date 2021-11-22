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
#include "ASIDBFUtil.h"
#include "ASIDBF.h"


// This is an example of an exported variable

INT32 ASIDBF_Init(PASIDBF_INIT_INFO pscdbf_init_info)
{
	SetLogFileInfo(pscdbf_init_info->szLogFilePath, pscdbf_init_info->szLogFileName, pscdbf_init_info->nRemainLog);
	WriteLogN("asi_dbf_init success");
	return 0;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_Create(PVOID* lpObject)
{
	if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

	*lpObject	= new CASIDBFUtil();
	return 0;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_Free(PVOID* lpObject)
{
	if(*lpObject == NULL)	return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

	delete *lpObject;
	*lpObject = NULL;
	return 0;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_SetOpTickCntLog(PVOID lpObject, UINT32 nChkOpTickCnt)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;		

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		nRtn = tASIDBFUtil->SetOpTickCntLog(nChkOpTickCnt);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_SetASyncMode(PVOID lpObject, UINT32 nMode)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;		

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		nRtn = tASIDBFUtil->SetASyncMode(nMode);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_CreateDB(PVOID lpObject, LPCTSTR lpFileName, LPCTSTR lpUserID, LPCTSTR lpUserPw)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;
		if(!lpFileName || !lpUserID || !lpUserPw)	return ASI_DBF_ERROR_TYPE_INVALID_ARGUMENT;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		nRtn = tASIDBFUtil->CreateDBF(lpFileName, lpUserID, lpUserPw);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_OpenDB(PVOID lpObject, LPCTSTR lpFileName, LPCTSTR lpUserID, LPCTSTR lpUserPw, UINT32 nAccessMode)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;
		if(!lpFileName || !lpUserID || !lpUserPw)	return ASI_DBF_ERROR_TYPE_INVALID_ARGUMENT;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		nRtn = tASIDBFUtil->OpenDBF(lpFileName, lpUserID, lpUserPw, nAccessMode);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_CloseDB(PVOID lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		nRtn = tASIDBFUtil->CloseDBF();
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_RemoveDB(PVOID lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		nRtn = tASIDBFUtil->RemoveDBF();
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_RecoveryDB(PVOID lpObject, UINT32 nRemainMode)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->RecoveryDBF(nRemainMode);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_BackupDB(PVOID lpObject, LPCTSTR lpBkFileName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->BackupDBF(lpBkFileName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_IsAutoRestore(PVOID lpObject, INT32& nMode)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->IsAutoRestore(nMode);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ChangeAuthInfoDB(PVOID lpObject, LPCTSTR lpCurUserID, LPCTSTR lpCurUserPw, LPCTSTR lpNewUserID, LPCTSTR lpNewUserPw, UINT32 nChkNewType)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ChangeAuthInfoDBF(lpCurUserID, lpCurUserPw, lpNewUserID, lpNewUserPw, nChkNewType);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_SetTItemEncTypeDB(PVOID lpObject, UINT32 nDefEncType)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->SetTItemEncTypeDBF(nDefEncType);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_SetTItemCompTypeDB(PVOID lpObject, PUINT32 pnDefCompType)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetTItemCompTypeDBF(pnDefCompType);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTItemEncTypeDB(PVOID lpObject, PUINT32 pnDefEncType)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetTItemEncTypeDBF(pnDefEncType);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTItemCompTypeDB(PVOID lpObject, UINT32 nDefCompType)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->SetTItemCompTypeDBF(nDefCompType);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


INT32	ASIDBF_PreSetDBF_TableColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->PreSetDBF_TableColumn(lpTableName, lpColName, nMemberType, nDefaultValue, nInitStrLen, nColOption);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ForceCreateDBF_Table(PVOID lpObject, LPCTSTR lpTableName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ForceCreateDBF_Table(lpTableName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ModifyDBF_TableAddColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ModifyDBF_TableAddColumn(lpTableName, lpColName, nMemberType, nDefaultValue, nInitStrLen, nColOption);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ModifyDBF_TableDelColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ModifyDBF_TableDelColumn(lpTableName, lpColName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ModifyDBF_TableRenameColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, LPCTSTR lpNewColName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ModifyDBF_TableRenameColumn(lpTableName, lpColName, lpNewColName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ModifyDBF_TableReLenColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, UINT32 nDefLen)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ModifyDBF_TableReLenColumn(lpTableName, lpColName, nDefLen);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ModifyDBF_TableInitAutoIncrement(PVOID lpObject, LPCTSTR lpTableName, UINT64 nInitValue)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ModifyDBF_TableInitAutoIncrement(lpTableName, nInitValue);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_DropDBF_Table(PVOID lpObject, LPCTSTR lpTableName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->DropDBF_Table(lpTableName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_RenameDBF_Table(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpNewTableName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->RenameDBF_Table(lpTableName, lpNewTableName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ReloadDBF_Table(PVOID lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ReloadDBF_TableList();
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTbSVer(PVOID lpObject, LPCTSTR lpTableName, PUINT32 nVer)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetTbSVer(lpTableName, nVer);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_SetTbSVer(PVOID lpObject, LPCTSTR lpTableName, UINT32 nVer)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->SetTbSVer(lpTableName, nVer);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTbInfo(PVOID lpObject, LPCTSTR lpTableName, PDBF_TABLE_INFO_RTN pDTIR)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetDBF_TableInfo(lpTableName, pDTIR);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTbInfoList(PVOID lpObject, PDBF_TABLE_INFO_RTN pTableInfoList, UINT32 nBuffLen, PUINT32 pnTableNum)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetDBF_TableInfoList(pTableInfoList, nBuffLen, pnTableNum);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTbColumnList(PVOID lpObject, LPCTSTR lpTableName, PDBF_TABLE_COLUMN_RTN pTableColumnList, UINT32 nBuffLen, PUINT32 pnColumnNum)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetDBF_TableColumnList(lpTableName, pTableColumnList, nBuffLen, pnColumnNum);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTbColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpTableColName, PDBF_TABLE_COLUMN_RTN pTableColumn)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetDBF_TableColumn(lpTableName, lpTableColName, pTableColumn);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTbColumnIdx(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpTableColName, PUINT32 pnTableColIdx)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetDBF_TableColumnIdx(lpTableName, lpTableColName, pnTableColIdx);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTbItemPriKeyFP(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpTableColName, UINT64 nPriKey, LPCTSTR lpPriKey, PUINT64 pnFP)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetTbItemPriKeyFP(lpTableName, lpTableColName, nPriKey, lpPriKey, pnFP);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32 ASIDBF_GetTbItemPriKeyFPList(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpTableColName, UINT64 nChkValueS, UINT64 nChkValueE, LPCTSTR lpChkValueS, LPCTSTR lpChkValueE, UINT32 nCompType, PUINT64* ppnFPList, PUINT32 pnFPListSize)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetTbItemPriKeyFPList(lpTableName, lpTableColName, nChkValueS, nChkValueE, lpChkValueS, lpChkValueE, nCompType, ppnFPList, pnFPListSize);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32 ASIDBF_FreeTbItemPriKeyFPList(PVOID lpObject, PUINT64* ppnFPList)
{
	if(*ppnFPList)
	{
		free(*ppnFPList);
		*ppnFPList = NULL;
	}
	return 0;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

INT32	ASIDBF_SetSelInfo(PVOID lpObject, LPCTSTR szTName, UINT32 nSelIDNum, PINT32 pSelIDArray)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->SetDBF_SelInfo(szTName, nSelIDNum, pSelIDArray);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_First(PVOID lpObject, LPCTSTR lpTableName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->FirstDBF_Item(lpTableName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_Last(PVOID lpObject, LPCTSTR lpTableName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->LastDBF_Item(lpTableName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_FirstKey(PVOID lpObject, LPCTSTR lpTableName, UINT64 nKey)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->FirstDBFKey_Item(lpTableName, nKey);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_LastKey(PVOID lpObject, LPCTSTR lpTableName, UINT64 nKey)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->LastDBFKey_Item(lpTableName, nKey);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_Next(PVOID lpObject, LPCTSTR lpTableName, PDBF_ITEM_RTN* pDIRArray)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->NextDBF_Item(lpTableName, pDIRArray);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_NextStop(PVOID lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->StopDBF_Item();
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_PreSetDBF_TableItem(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, PVOID lpBuff, UINT32 nBufLen)
{
	INT32 nRtn = 0; 
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->PreSetDBF_TableItem(lpTableName, lpColName, lpBuff, nBufLen);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ExeSelect(PVOID lpObject, LPCTSTR lpTableName, UINT64 nFP)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ExeSelectDBF(lpTableName, nFP);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ExeSelectByID(PVOID lpObject, LPCTSTR lpTableName, UINT32 nID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ExeSelectDBFByID(lpTableName, nID);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ExeUpdateByID(PVOID lpObject, LPCTSTR lpTableName, UINT32 nID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ExeUpdateDBFByID(lpTableName, nID);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ExeDeleteByID(PVOID lpObject, LPCTSTR lpTableName, UINT32 nID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ExeDeleteDBFByID(lpTableName, nID);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ExeInsert(PVOID lpObject, LPCTSTR lpTableName, PUINT64 pnNewFP, PUINT64 pnLastID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ExeInsertDBF(lpTableName, pnNewFP, pnLastID);
	}
	catch (...)
	{
		WriteLogE("occur exception : [ASIDBF_ExeInsert]");
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ExeUpdate(PVOID lpObject, LPCTSTR lpTableName, PUINT64 pnFP)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ExeUpdateDBF(lpTableName, pnFP);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_ExeDelete(PVOID lpObject, LPCTSTR lpTableName, UINT64 nFP)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->ExeDeleteDBF(lpTableName, nFP);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

INT32	ASIDBF_FastSetCurTable(PVOID lpObject, LPCTSTR szTName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->FastSetCurTable(szTName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_FastPreSetDBF_TableItem(PVOID lpObject, LPCTSTR szColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->FastPreSetDBF_TableItem(szColName, nMemberType, lpBuff, nBufLen);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_FastExeInsertDBF(PVOID lpObject, PUINT64 pnLastID, PUINT64 pnNewFP)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->FastExeInsertDBF(pnLastID, pnNewFP);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_FastExeUpdateDBF(PVOID lpObject, UINT32 nID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->FastExeUpdateDBF(nID);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

INT32	ASIDBF_GetFieldByIdx(PVOID lpObject, INT32 nIdx, PDBF_ITEM_RTN pdfir)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		*pdfir = *(tASIDBFUtil->GetField(nIdx));
		if(!pdfir)			return ASI_DBF_ERROR_TYPE_INVALID_COLUMN_IDX;
		nRtn = 0;
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetFieldByName(PVOID lpObject, LPCTSTR lpName, PDBF_ITEM_RTN pdfir)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		*pdfir = *(tASIDBFUtil->GetField(lpName));
		if(!pdfir)			return ASI_DBF_ERROR_TYPE_INVALID_COLUMN_NAME;
		nRtn = 0;
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetFieldFP(PVOID lpObject, PUINT64 pnFP)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		*pnFP = tASIDBFUtil->GetFieldFP();
		nRtn = 0;
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTableNextID(PVOID lpObject, LPCTSTR lpTName, PUINT64 pnID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetTableNextID(lpTName, pnID);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTableItemFPByPKey(PVOID lpObject, LPCTSTR lpTName, UINT64 nKey, PUINT64 pnFP)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFUtil* tASIDBFUtil = (CASIDBFUtil*)lpObject;

		if(tASIDBFUtil->IsInvalidAuthRight())	return ASI_DBF_ERROR_TYPE_V2_AUTH_INVALID_RIGHT;

		nRtn = tASIDBFUtil->GetTableItemFPByPKey(lpTName, nKey, pnFP);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
