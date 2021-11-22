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

#if !defined(AFX_ASIDBFQDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_)
#define AFX_ASIDBFQDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_


//------------------------------------------------------
#include "asi_lnx_define_w32.h"
#include "ASIDBFQDLLMember.h"
//------------------------------------------------------

class CASIDBFQDLLUtil
{

private:
	HINSTANCE				m_hDLL;
	PVOID					m_lpObject;

private:
	TMapASIDBFQDLLMember	m_tDbfqMemberMap;

public:
	CMutexExt				m_tMutexMember;
	CASIDBFQDLLMember*		GetMember();
	void					ClearMember();
	LPTSTR					GetMemberTbName();

public:
	INT32					LoadLibraryExt(LPCTSTR lpDLLPath);
	INT32					FreeLibraryExt();

private:
	INT32					LoadFunction();

private:
	ASIDBFQ_NextType				m_fnASIDBFQ_Next;
	ASIDBFQ_GetFieldByIdxType		m_fnASIDBFQ_GetFieldByIdx;
	ASIDBFQ_GetFieldByNameType		m_fnASIDBFQ_GetFieldByName;
	ASIDBFQ_GetFieldFPType			m_fnASIDBFQ_GetFieldFP;

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
	INT32					ASIDBFQ_Init(PASIDBFQ_INIT_INFO pscdbfq_init_info);
	INT32					ASIDBFQ_Create();
	INT32					ASIDBFQ_Free();

	INT32					ASIDBFQ_CreateDatabase(LPCTSTR lpDBName, LPCTSTR lpUserID, LPCTSTR lpUserPw);
	INT32					ASIDBFQ_OpenDatabase(LPCTSTR lpDBName, LPCTSTR lpUserID, LPCTSTR lpUserPw, UINT32 nAccessMode = (GENERIC_READ|GENERIC_WRITE));
	INT32					ASIDBFQ_CloseDatabase(LPCTSTR lpDBName);

    INT32					ASIDBFQ_IsExistTable(CString strTBName);
	INT32					ASIDBFQ_GetTableVer(CString strTBName, PUINT32 pnVer);
	INT32					ASIDBFQ_SetTableVer(CString strTBName, UINT32 nVer);

	INT32					ASIDBFQ_QuerySelect(CString strQuery);
	INT32					ASIDBFQ_QueryExecute(CString strQuery);

	INT32					ASIDBFQ_First();
	INT32					ASIDBFQ_Last();
	INT32					ASIDBFQ_FirstKey(UINT64 nKey);
	INT32					ASIDBFQ_LastKey(UINT64 nKey);
	INT32					ASIDBFQ_Next();
	INT32					ASIDBFQ_NextStop();

	INT32					ASIDBFQ_GetFieldByIdx(INT32 nIdx, PDBF_ITEM_RTN pdfir);
	INT32					ASIDBFQ_GetFieldByName(LPCTSTR lpName, PDBF_ITEM_RTN pdfir);
	INT32					ASIDBFQ_GetFieldFP(PUINT64 pnFP);

	INT32					ASIDBFQ_GetLastID(PUINT64 pnLastID);
	INT32					ASIDBFQ_GetLastFp(PUINT64 pnLastFp);

	INT32					ASIDBFQ_GetTbLastID(LPCTSTR lpTableName, PUINT64 pnLastID);

public:
	PDBF_ITEM_RTN			GetField(INT32 nIdx);
	PDBF_ITEM_RTN			GetField(LPCTSTR szName);
	UINT64					GetFieldFP();
	UINT64					GetLastID();
	UINT64					GetLastFp();

private:
	BOOL					m_bUseMutex;
//	CMutexExt				m_tDBFQMutex;

private:
	INT32					LockMutex();
	INT32					UnLockMutex();

public:	
	void					SetUseMutex(BOOL bUse = TRUE)	{	m_bUseMutex	=	bUse;	};


public:
	void					CheckCurTableName(LPCTSTR lpTableName);
	void					SetCurTableName(LPCTSTR lpTableName);
    void					SetCurTableName(CString lpTableName);

public:
	INT32					ASIDBF_ReloadDBF_Table();
	INT32					ASIDBF_GetTbColumnIdx(LPCTSTR lpColName, PUINT32 nColIdx, LPCTSTR lpTableName = NULL);

	INT32					ASIDBF_PreSetDBF_TableItem(LPCTSTR lpColName, PVOID lpBuff, UINT32 nBufLen = 0, LPCTSTR lpTableName = NULL);
	INT32					ASIDBF_ExeSelect(UINT64 nFP, LPCTSTR lpTableName = NULL);
	INT32					ASIDBF_ExeInsert(UINT64& nNewFP, UINT64& nLastID, LPCTSTR lpTableName = NULL);
	INT32					ASIDBF_ExeUpdate(UINT64& nFP, LPCTSTR lpTableName = NULL);
	INT32					ASIDBF_ExeDelete(UINT64 nFP, LPCTSTR lpTableName = NULL);

	INT32					ASIDBF_ExeSelectByID(UINT32 nID, LPCTSTR lpTableName = NULL);
	INT32					ASIDBF_ExeUpdateByID(UINT32 nID, LPCTSTR lpTableName = NULL);
	INT32					ASIDBF_ExeDeleteByID(UINT32 nID, LPCTSTR lpTableName = NULL);

	INT32					ASIDBF_First(LPCTSTR lpTableName = NULL);
	INT32					ASIDBF_Last(LPCTSTR lpTableName = NULL);
	INT32					ASIDBF_FirstKey(LPCTSTR lpTableName = NULL, UINT64 nKey = 0);
	INT32					ASIDBF_LastKey(LPCTSTR lpTableName = NULL, UINT64 nKey = 0);
	INT32					ASIDBF_Next(LPCTSTR lpTableName = NULL);
	INT32					ASIDBF_NextStop();

    INT32					ASIDBF_GetFieldByIdx(INT32 nIdx, PDBF_ITEM_RTN pdfir);
	INT32					ASIDBF_GetFieldByName(LPCTSTR lpName, PDBF_ITEM_RTN pdfir);
	INT32					ASIDBF_GetTableNextID(LPCTSTR lpTName, PUINT64 pnNextID);
	INT32					ASIDBF_GetTableItemFPByPKey(LPCTSTR lpTName, UINT64 nKey, PUINT64 pnFP);

	INT32					ASIDBF_BackupDB(LPCTSTR lpBkFileName);
	INT32					ASIDBF_RecoveryDB(INT32 nRemainMode);
	INT32					ASIDBF_IsAutoRestore(INT32& nMode);
	INT32					ASIDBF_ChangeAuthInfoDB(LPCTSTR lpCurUserID, LPCTSTR lpCurUserPw, LPCTSTR lpNewUserID, LPCTSTR lpNewUserPw, UINT32 nChkNewType);
	INT32					ASIDBF_SetTItemEncTypeDB(UINT32 nDefEncType);
	INT32					ASIDBF_GetTItemEncTypeDB(PUINT32 pnDefEncType);

public:
	INT32					ASIDBF_FastSetCurTable(LPCTSTR szTName);
	INT32					ASIDBF_FastPreSetDBF_TableItem(LPCTSTR szColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen = 0);
	INT32					ASIDBF_FastExeInsertDBF(PUINT64 pnLastID, PUINT64 pnNewFP = NULL);
	INT32					ASIDBF_FastExeUpdateDBF(UINT32 nID);

public:
    PDBF_ITEM_RTN			ASIDBF_GetField(INT32 nIdx);
	PDBF_ITEM_RTN			ASIDBF_GetField(LPCTSTR szName);
	INT32					ASIDBF_GetFieldCIdx(LPCTSTR szCName);

    INT32					ASIDBF_PreSetValue16(LPCTSTR lpColName, UINT16& nValue);
    INT32					ASIDBF_PreSetValue32(LPCTSTR lpColName, UINT32& nValue);
    INT32					ASIDBF_PreSetValue64(LPCTSTR lpColName, UINT64& nValue);
    INT32					ASIDBF_PreSetValue(LPCTSTR lpColName, CString& strValue);

public:
	INT32					ASIDBF_FastPreSetValue16(LPCTSTR lpColName, UINT16& nValue);
	INT32					ASIDBF_FastPreSetValue32(LPCTSTR lpColName, UINT32& nValue);
	INT32					ASIDBF_FastPreSetValue64(LPCTSTR lpColName, UINT64& nValue);
	INT32					ASIDBF_FastPreSetValue(LPCTSTR lpColName, CString& strValue);

public:
	UINT16					GetField_UINT16(INT32 nIdx);
	UINT32					GetField_UINT32(INT32 nIdx);
	UINT64					GetField_UINT64(INT32 nIdx);
	CString					GetField_String(INT32 nIdx);

public:
	INT32					ASIDBF_GetTbInfo(PDBF_TABLE_INFO_RTN pDTIR, LPCTSTR lpTableName);
	INT32					ASIDBF_GetTbColumnList(LPCTSTR lpTableName, PDBF_TABLE_COLUMN_RTN pTableColumnList, UINT32 nBuffLen, PUINT32 pnColumnNum);


public:
	CASIDBFQDLLUtil();
	virtual ~CASIDBFQDLLUtil();

};

extern CASIDBFQDLLUtil*		t_ASIDBFQDLLUtil;

#endif // !defined(AFX_ASIDBFQDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_)
