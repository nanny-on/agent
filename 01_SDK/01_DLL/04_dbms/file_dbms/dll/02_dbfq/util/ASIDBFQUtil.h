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

#pragma once


#include "ASIDBFQMember.h"

class CASIDBFQUtil
{

private:
	ASIDBFQ_INIT_INFO	m_tSQII;
	CASIDBFDLLUtil		m_tASIDBFDLLUtil;

private:
	TMapASIDBFQMember	m_tDbfqUtilMemberMap;

public:
	CMutexExt			m_tMutexMember;
	CASIDBFQMember*		GetMember();
	void				ClearMember();

public:
	INT32		Init(PASIDBFQ_INIT_INFO psqii);

	INT32		CreateDatabase(CString strDBName, CString strUserID, CString strUserPw);
	INT32		OpenDatabase(CString strDBName, CString strUserID, CString strUserPw, UINT32 nAccessMode);
	INT32		CloseDatabase(CString strDBName);

public:
	INT32		SetCurDatabase(CString strDBName);

public:
	INT32		QuerySelect(CString strQuery);
	INT32		QueryExecute(CString strQuery);
	INT32		QuerySelectResult(LPCTSTR* szTableName);
	INT32		QueryExecuteResult(PUINT32 pnCmdType, PUINT32 pnCmdCode, PUINT32 pnRtnCnt);

public:
	INT32		First();
	INT32		Last();
	INT32		FirstKey(UINT64 nKey);
	INT32		LastKey(UINT64 nKey);
	INT32		Next();
	INT32		NextStop();

	PDBF_ITEM_RTN		GetField(INT32 nIdx);
	PDBF_ITEM_RTN		GetField(LPCTSTR lpName);
	UINT64				GetFieldFP();

public:
	INT32		IsExistTable(CString strTName);
	INT32		GetTbVer(CString strTName, PUINT32 nVer);
	INT32		SetTbVer(CString strTName, UINT32 nVer);

private:
	INT32		QueryExecuteLine(CString strQuery, UINT32& nAQOT);
	INT32		IsValidContidion(INT32& nPrimaryKey);
	INT32		IsValidCompWhCdt(PDBFQ_CDT_INFO_WHERE pDCIW, UINT64 nValue);
	INT32		IsValidCompWhCdt(PDBFQ_CDT_INFO_WHERE pDCIW, CString strValue);
	INT32		IsPrimaryKeyCondition(TListID64& tChgItemFPList);

public:
	INT32		ASIDBF_RecoveryDB(UINT32 nRemainMode);
	INT32		ASIDBF_BackupDB(CString strBkFileName);
	INT32		ASIDBF_IsAutoRestore(INT32& nMode);

	INT32		ASIDBF_ChangeAuthInfoDB(LPCTSTR lpCurUserID, LPCTSTR lpCurUserPw, LPCTSTR lpNewUserID, LPCTSTR lpNewUserPw, UINT32 nChkNewType);
	INT32		ASIDBF_SetTItemEncTypeDB(UINT32 nDefEncType);
	INT32		ASIDBF_SetTItemCompTypeDB(UINT32 nDefCompType);
	INT32		ASIDBF_GetTItemEncTypeDB(PUINT32 pnDefEncType);
	INT32		ASIDBF_GetTItemCompTypeDB(PUINT32 pnDefCompType);


	INT32		ASIDBF_PreSetDBF_TableColumn(LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue = 0, UINT16 nInitStrLen = 0, UINT32 nColOption = 0, LPCTSTR lpTableName = NULL);
	INT32		ASIDBF_ForceCreateDBF_Table(LPCTSTR lpTableName = NULL);
	INT32		ASIDBF_ModifyDBF_TableAddColumn(LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue = 0, UINT16 nInitStrLen = 0, UINT32 nColOption = 0, LPCTSTR lpTableName = NULL);
	INT32		ASIDBF_ModifyDBF_TableDelColumn(LPCTSTR lpColName, LPCTSTR lpTableName = NULL);
	INT32		ASIDBF_ModifyDBF_TableRenameColumn(LPCTSTR lpColName, LPCTSTR lpNewColName, LPCTSTR lpTableName = NULL);
	INT32		ASIDBF_ModifyDBF_TableInitAutoIncrement(UINT64 nInitValue, LPCTSTR lpTableName = NULL);
	INT32		ASIDBF_DropDBF_Table(LPCTSTR lpTableName);
	INT32		ASIDBF_ReloadDBF_Table();

	INT32		ASIDBF_GetTbInfo(CString strTName, PDBF_TABLE_INFO_RTN pDTIR);
	INT32		ASIDBF_GetTbInfoList(PDBF_TABLE_INFO_RTN pInfoBuffList, UINT32 nBuffLen, PUINT32 pnTableNum);
	INT32		ASIDBF_GetTbColumnList(CString strTName, PDBF_TABLE_COLUMN_RTN pColBuffList, UINT32 nBuffLen, PUINT32 pnColNum);
	INT32		ASIDBF_GetTbColumnIdx(CString strTName, CString strTCName, PUINT32 pnColIdx);

	INT32		ASIDBF_First(CString strTableName);
	INT32		ASIDBF_Last(CString strTableName);
	INT32		ASIDBF_FirstKey(CString strTableName, UINT64 nKey);
	INT32		ASIDBF_LastKey(CString strTableName, UINT64 nKey);
	INT32		ASIDBF_Next(CString strTableName);
	INT32		ASIDBF_NextStop();

	INT32		ASIDBF_PreSetDBF_TableItem(CString strColName, PVOID lpBuff, UINT32 nBufLen, CString strTableName);
	INT32		ASIDBF_ExeSelect(UINT64 nFP, CString strTableName);
	INT32		ASIDBF_ExeInsert(PUINT64 pnNewFP, PUINT64 pnLastID, CString strTableName);
	INT32		ASIDBF_ExeUpdate(PUINT64 pnFP, CString strTableName);
	INT32		ASIDBF_ExeDelete(UINT64 nFP, CString strTableName);

	INT32		ASIDBF_ExeSelectByID(UINT32 nID, CString strTableName);
	INT32		ASIDBF_ExeUpdateByID(UINT32 nID, CString strTableName);
	INT32		ASIDBF_ExeDeleteByID(UINT32 nID, CString strTableName);

	INT32		ASIDBF_FastSetCurTable(LPCTSTR szTName);
	INT32		ASIDBF_FastPreSetDBF_TableItem(LPCTSTR szColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen);
	INT32		ASIDBF_FastExeInsertDBF(PUINT64 pnLastID, PUINT64 pnNewFP);
	INT32		ASIDBF_FastExeUpdateDBF(UINT32 nID);

	INT32		ASIDBF_GetFieldByIdx(INT32 nIdx, PDBF_ITEM_RTN pdfir);
	INT32		ASIDBF_GetFieldByName(CString strName, PDBF_ITEM_RTN pdfir);
	INT32		ASIDBF_GetTableNextID(LPCTSTR lpTName, PUINT64 pnNextID);
	INT32		ASIDBF_GetTableItemFPByPKey(LPCTSTR lpTName, UINT64 nKey, PUINT64 pnFP);



public:
	CASIDBFQUtil(void);
	~CASIDBFQUtil(void);
};
