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

// ASIDBFDLLUtil.h: interface for the CASIDBFDLLUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASIDBFDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_)
#define AFX_ASIDBFDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_


//------------------------------------------------------
#include "ASIDBFDLLMember.h"
#include "com_struct_dbf_header_user.h"
//------------------------------------------------------

class CASIDBFDLLUtil
{

private:
	HINSTANCE		m_hDLL;
	PVOID			m_lpObject;
	TMapStrID64		m_tDBObjMap;

public:
	INT32			LoadLibraryExt(LPCTSTR lpDLLPath);
	INT32			FreeLibraryExt();

private:
	INT32			LoadFunction();


private:
	TMapASIDBFDLLMember		m_tDbfMemberMap;

public:
	CMutexExt				m_tMutexMember;
	CASIDBFDLLMember*		GetMember();
	void					ClearMember();

private:
	ASIDBF_NextType					m_fnASIDBF_Next;
	ASIDBF_GetFieldByIdxType		m_fnASIDBF_GetFieldByIdx;
	ASIDBF_GetFieldByNameType		m_fnASIDBF_GetFieldByName;
	ASIDBF_GetFieldFPType			m_fnASIDBF_GetFieldFP;
	ASIDBF_GetTableNextIDType		m_fnASIDBF_GetTableNextID;
	ASIDBF_GetTableItemFPByPKeyType	m_fnASIDBF_GetTableItemFPByPKey;

	ASIDBF_ExeInsertType			m_fnASIDBF_ExeInsert;
	ASIDBF_ExeSelectByIDType		m_fnASIDBF_ExeSelectByID;
	ASIDBF_ExeUpdateByIDType		m_fnASIDBF_ExeUpdateByID;
	ASIDBF_ExeDeleteByIDType		m_fnASIDBF_ExeDeleteByID;
	ASIDBF_PreSetDBF_TableItemType	m_fnASIDBF_PreSetDBF_TableItem;

	ASIDBF_FastSetCurTableType			m_fnASIDBF_FastSetCurTable;
	ASIDBF_FastPreSetDBF_TableItemType	m_fnASIDBF_FastPreSetDBF_TableItem;
	ASIDBF_FastExeInsertDBFType			m_fnASIDBF_FastExeInsertDBF;
	ASIDBF_FastExeUpdateDBFType			m_fnASIDBF_FastExeUpdateDBF;

public:
	INT32	ASIDBF_Init(PASIDBF_INIT_INFO pasidbf_init_info);
	INT32	ASIDBF_Create();
	INT32	ASIDBF_Free();

	INT32	ASIDBF_SetOpTickCntLog(UINT32 nChkOpTickCnt);
	INT32	ASIDBF_SetASyncMode(UINT32 nMode);

	INT32	ASIDBF_CreateDB(LPCTSTR lpFileName, LPCTSTR lpUserID, LPCTSTR lpUserPw);
	INT32	ASIDBF_OpenDB(LPCTSTR lpFileName, LPCTSTR lpUserID, LPCTSTR lpUserPw, UINT32 nAccessMode);
	INT32	ASIDBF_CloseDB();
	INT32	ASIDBF_RemoveDB();
	INT32	ASIDBF_RecoveryDB(UINT32 nRemainMode = 0);
	INT32	ASIDBF_BackupDB(LPCTSTR lpBkFileName);
	INT32	ASIDBF_IsAutoRestore(INT32& nMode);

	INT32	ASIDBF_ChangeAuthInfoDB(LPCTSTR lpCurUserID, LPCTSTR lpCurUserPw, LPCTSTR lpNewUserID, LPCTSTR lpNewUserPw, UINT32 nChkNewType);
	INT32	ASIDBF_SetTItemEncTypeDB(UINT32 nDefEncType);
	INT32	ASIDBF_SetTItemCompTypeDB(UINT32 nDefCompType);
	INT32	ASIDBF_GetTItemEncTypeDB(PUINT32 pnDefEncType);
	INT32	ASIDBF_GetTItemCompTypeDB(PUINT32 pnDefCompType);

	INT32	ASIDBF_PreSetDBF_TableColumn(LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue = 0, UINT16 nInitStrLen = 0, UINT32 nColOption = 0, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_ForceCreateDBF_Table(LPCTSTR lpTableName = NULL);
    INT32	ASIDBF_ModifyDBF_TableAddColumn(LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue = 0, UINT16 nInitStrLen = 0, UINT32 nColOption = 0, LPCTSTR lpTableName = NULL);
    INT32	ASIDBF_ModifyDBF_TableDelColumn(LPCTSTR lpColName, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_ModifyDBF_TableRenameColumn(LPCTSTR lpColName, LPCTSTR lpNewColName, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_ModifyDBF_TableReLenColumn(LPCTSTR lpColName, UINT32 nDefLen, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_ModifyDBF_TableInitAutoIncrement(UINT64 nInitValue, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_DropDBF_Table(LPCTSTR lpTableName);
	INT32	ASIDBF_RenameDBF_Table(LPCTSTR lpTableName, LPCTSTR lpNewTableName);
	INT32	ASIDBF_ReloadDBF_Table();

	INT32	ASIDBF_GetTbSVer(PUINT32 nVer, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_SetTbSVer(UINT32 nVer, LPCTSTR lpTableName = NULL);

    INT32	ASIDBF_GetTbInfo(PDBF_TABLE_INFO_RTN pDTIR, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_GetTbInfoList(PDBF_TABLE_INFO_RTN pTableInfoList, UINT32 nBuffLen, PUINT32 pnTableNum);
	INT32	ASIDBF_GetTbColumnList(LPCTSTR lpTableName, PDBF_TABLE_COLUMN_RTN pTableColumnList, UINT32 nBuffLen, PUINT32 pnColumnNum);
	INT32	ASIDBF_GetTbColumn(LPCTSTR lpTableName, LPCTSTR lpTableColName, PDBF_TABLE_COLUMN_RTN pTableColumn);
	INT32	ASIDBF_GetTbColumnIdx(LPCTSTR lpTableName, LPCTSTR lpTableColName, PUINT32 pnTableColIdx);
	INT32	ASIDBF_GetTbItemPriKeyFP(LPCTSTR lpTableName, LPCTSTR lpTableColName, UINT64 nPriKey, LPCTSTR lpPriKey, PUINT64 pnFP);
	INT32	ASIDBF_GetTbItemPriKeyFPList(LPCTSTR lpTableName, LPCTSTR lpTableColName, UINT64 nChkValueS, UINT64 nChkValueE, LPCTSTR lpChkValueS, LPCTSTR lpChkValueE, UINT32 nCompType, PUINT64* ppnFPList, PUINT32 pnFPListSize);
	INT32	ASIDBF_FreeTbItemPriKeyFPList(PUINT64* ppnFPList);

	INT32	ASIDBF_First(LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_Last(LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_FirstKey(LPCTSTR lpTableName = NULL, UINT64 nKey = 0);
	INT32	ASIDBF_LastKey(LPCTSTR lpTableName = NULL, UINT64 nKey = 0);
	INT32	ASIDBF_Next(PDBF_ITEM_RTN* pDIRArray = NULL, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_NextStop();
	INT32	ASIDBF_SetSelInfo(UINT32 nSelIDNum = 0, PINT32 pSelIDArray = NULL, LPCTSTR lpTableName = NULL);
	
	INT32	ASIDBF_PreSetDBF_TableItem(LPCTSTR lpColName, PVOID lpBuff, UINT32 nBufLen = 0, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_ExeSelect(UINT64 nFP, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_ExeInsert(PUINT64 pnNewFP, PUINT64 pnLastID, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_ExeUpdate(PUINT64 pnFP, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_ExeDelete(UINT64 nFP, LPCTSTR lpTableName = NULL);

	INT32	ASIDBF_ExeSelectByID(UINT32 nID, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_ExeUpdateByID(UINT32 nID, LPCTSTR lpTableName = NULL);
	INT32	ASIDBF_ExeDeleteByID(UINT32 nID, LPCTSTR lpTableName = NULL);

	INT32	ASIDBF_FastSetCurTable(LPCTSTR szTName);
	INT32	ASIDBF_FastPreSetDBF_TableItem(LPCTSTR szColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen);
	INT32	ASIDBF_FastExeInsertDBF(PUINT64 pnLastID, PUINT64 pnNewFP);
	INT32	ASIDBF_FastExeUpdateDBF(UINT32 nID);

	INT32	ASIDBF_GetFieldByIdx(INT32 nIdx, PDBF_ITEM_RTN pdfir);
	INT32	ASIDBF_GetFieldByName(LPCTSTR lpName, PDBF_ITEM_RTN pdfir);
	INT32	ASIDBF_GetFieldFP(PUINT64 pnFP);
	INT32	ASIDBF_GetTableNextID(LPCTSTR lpTName, PUINT64 pnNextID);
	INT32	ASIDBF_GetTableItemFPByPKey(LPCTSTR lpTName, UINT64 nKey, PUINT64 pnFP);

public:
	PDBF_ITEM_RTN			GetField(INT32 nIdx);
	PDBF_ITEM_RTN			GetField(LPCTSTR szName);
	PDBF_TABLE_COLUMN_RTN	GetTableColumn(LPCTSTR szTCName, LPCTSTR lpTableName = NULL);	
	UINT64					GetFieldFP();

public:
	void	CheckCurTableName(LPCTSTR lpTableName);
	void	SetCurTableName(LPCTSTR lpTableName);
	CString	GetCurTableName();

public:
	INT32	CreateDBObject(LPCTSTR lpDBName);
	INT32	FreeDBObject(LPCTSTR lpDBName);
	INT32	SetCurDBObject(LPCTSTR lpDBName);

public:
	CASIDBFDLLUtil();
	virtual ~CASIDBFDLLUtil();

};

extern CASIDBFDLLUtil*	t_ASIDBFDLLUtil;

#endif // !defined(AFX_ASIDBFDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_)
