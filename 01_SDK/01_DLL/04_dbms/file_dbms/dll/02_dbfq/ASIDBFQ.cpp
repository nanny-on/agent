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

#include "ASIDBFQ.h"
#include "ASIDBFQUtil.h"

INT32	ASIDBFQ_Init(PVOID lpObject, PASIDBFQ_INIT_INFO psqii)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->Init(psqii);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_Create(PVOID* lpObject)
{
	if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

	*lpObject	= new CASIDBFQUtil();
	return 0;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_Free(PVOID* lpObject)
{
	if(*lpObject == NULL)	return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

	CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)(*lpObject);
	delete tASIDBFQUtil;
	*lpObject = NULL;
	return 0;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_CreateDatabase(PVOID lpObject, LPCTSTR szDBName, LPCTSTR lpUserID, LPCTSTR lpUserPw)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;
		if(!szDBName || !lpUserID || !lpUserPw)		return ASI_DBFQ_ERROR_TYPE_INVALID_ARGUMENT;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->CreateDatabase(szDBName, lpUserID, lpUserPw);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_OpenDatabase(PVOID lpObject, LPCTSTR szDBName, LPCTSTR lpUserID, LPCTSTR lpUserPw, UINT32 nAccessMode)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;
		if(!szDBName || !lpUserID || !lpUserPw)		return ASI_DBFQ_ERROR_TYPE_INVALID_ARGUMENT;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->OpenDatabase(szDBName, lpUserID, lpUserPw, nAccessMode);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_CloseDatabase(PVOID lpObject, LPCTSTR szDBName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->CloseDatabase(szDBName);
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

INT32	ASIDBFQ_IsExistTable(PVOID lpObject, LPCTSTR szTBName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->IsExistTable(szTBName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_GetTableVer(PVOID lpObject, LPCTSTR szTBName, PUINT32 nVer)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->GetTbVer(szTBName, nVer);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_SetTableVer(PVOID lpObject, LPCTSTR szTBName, UINT32 nVer)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->SetTbVer(szTBName, nVer);
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

INT32	ASIDBFQ_QuerySelect(PVOID lpObject, LPCTSTR szQuery)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->QuerySelect(szQuery);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_QuerySelectResult(PVOID lpObject, LPCTSTR* szTableName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->QuerySelectResult(szTableName);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_QueryExecute(PVOID lpObject, LPCTSTR szQuery)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->QueryExecute(szQuery);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_QueryExecuteResult(PVOID lpObject, PUINT32 pnCmdType, PUINT32 pnCmdCode, PUINT32 pnRtnCnt)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->QueryExecuteResult(pnCmdType, pnCmdCode, pnRtnCnt);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_First(PVOID lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->First();
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_Last(PVOID lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->Last();
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_FirstKey(PVOID lpObject, UINT64 nKey)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->FirstKey(nKey);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_LastKey(PVOID lpObject, UINT64 nKey)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->LastKey(nKey);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_Next(PVOID lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->Next();
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_NextStop(PVOID lpObject)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->NextStop();
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

INT32	ASIDBFQ_GetFieldByIdx(PVOID lpObject, INT32 nIdx, PDBF_ITEM_RTN pdfir)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		*pdfir = *(tASIDBFQUtil->GetField(nIdx));
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

INT32	ASIDBFQ_GetFieldByName(PVOID lpObject, LPCTSTR lpName, PDBF_ITEM_RTN pdfir)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		*pdfir = *(tASIDBFQUtil->GetField(lpName));
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

INT32	ASIDBFQ_GetFieldFP(PVOID lpObject, PUINT64 pnFP)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		*pnFP = tASIDBFQUtil->GetFieldFP();
		nRtn = 0;
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_GetLastID(PVOID lpObject, PUINT64 pnLastID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;
		CASIDBFQMember* pMember = tASIDBFQUtil->GetMember();

		*pnLastID = pMember->m_nLastID;
		nRtn = 0;
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_GetLastFp(PVOID lpObject, PUINT64 pnLastFp)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;
		CASIDBFQMember* pMember = tASIDBFQUtil->GetMember();

		*pnLastFp = pMember->m_nLastFp;
		nRtn = 0;
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBFQ_GetTbLastID(PVOID lpObject, LPCTSTR lpTableName, PUINT64 pnLastID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		DBF_TABLE_INFO_RTN tDTIR;
		tASIDBFQUtil->ASIDBF_GetTbInfo(lpTableName, &tDTIR);
		*pnLastID = tDTIR.nLastID;
		nRtn = 0;
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
//-------------------------------------------------------------------------

INT32	ASIDBF_RecoveryDB(PVOID lpObject, UINT32 nRemainMode)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_RecoveryDB(nRemainMode);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_BackupDB(lpBkFileName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_IsAutoRestore(nMode);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ChangeAuthInfoDB(lpCurUserID, lpCurUserPw, lpNewUserID, lpNewUserPw, nChkNewType);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_SetTItemEncTypeDB(nDefEncType);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_SetTItemCompTypeDB(PVOID lpObject, UINT32 nDefCompType)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_SetTItemCompTypeDB(nDefCompType);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_GetTItemEncTypeDB(pnDefEncType);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTItemCompTypeDB(PVOID lpObject, PUINT32 pnDefCompType)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_GetTItemCompTypeDB(pnDefCompType);
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


INT32	ASIDBF_PreSetDBF_TableColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_PreSetDBF_TableColumn(lpColName, nMemberType, nDefaultValue, nInitStrLen, nColOption, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ForceCreateDBF_Table(lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ModifyDBF_TableAddColumn(lpColName, nMemberType, nDefaultValue, nInitStrLen, nColOption, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ModifyDBF_TableDelColumn(lpColName, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ModifyDBF_TableRenameColumn(lpColName, lpNewColName, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ModifyDBF_TableInitAutoIncrement(nInitValue, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_DropDBF_Table(lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ReloadDBF_Table();
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

INT32	ASIDBF_GetTbInfo(PVOID lpObject, LPCTSTR lpTableName, PDBF_TABLE_INFO_RTN pDTIR)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_GetTbInfo(lpTableName, pDTIR);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_GetTbInfoList(pTableInfoList, nBuffLen, pnTableNum);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_GetTbColumnList(lpTableName, pTableColumnList, nBuffLen, pnColumnNum);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_GetTbColumnIdx(lpTableName, lpTableColName, pnTableColIdx);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_First(lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_Last(lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_FirstKey(lpTableName, nKey);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_LastKey(lpTableName, nKey);
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_Next(PVOID lpObject, LPCTSTR lpTableName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_Next(lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_NextStop();
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_PreSetDBF_TableItem(lpColName, lpBuff, nBufLen, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ExeSelectByID(nID, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ExeUpdateByID(nID, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ExeDeleteByID(nID, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ExeSelect(nFP, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ExeInsert(pnNewFP, pnLastID, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ExeUpdate(pnFP, lpTableName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_ExeDelete(nFP, lpTableName);
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
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

INT32	ASIDBF_FastSetCurTable(PVOID lpObject, LPCTSTR szTName)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_FastSetCurTable(szTName);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_FastPreSetDBF_TableItem(szColName, nMemberType, lpBuff, nBufLen);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_FastExeInsertDBF(pnLastID, pnNewFP);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		nRtn = tASIDBFQUtil->ASIDBF_FastExeUpdateDBF(nID);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		return tASIDBFQUtil->ASIDBF_GetFieldByIdx(nIdx, pdfir);
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

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		return tASIDBFQUtil->ASIDBF_GetFieldByName(lpName, pdfir);		
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTableNextID(PVOID lpObject, LPCTSTR lpName, PUINT64 pnNextID)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		return tASIDBFQUtil->ASIDBF_GetTableNextID(lpName, pnNextID);		
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

INT32	ASIDBF_GetTableItemFPByPKey(PVOID lpObject, LPCTSTR lpName, UINT64 nKey, PUINT64 pnFP)
{
	INT32 nRtn = 0;
	try
	{
		if(!lpObject)		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

		CASIDBFQUtil* tASIDBFQUtil = (CASIDBFQUtil*)lpObject;

		return tASIDBFQUtil->ASIDBF_GetTableItemFPByPKey(lpName, nKey, pnFP);		
	}
	catch (...)
	{
		nRtn = ASI_DBF_ERROR_TYPE_NOT_DEFINE_ERROR;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

