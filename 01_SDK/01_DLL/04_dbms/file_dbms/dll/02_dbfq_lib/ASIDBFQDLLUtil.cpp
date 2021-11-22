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
#include "ASIDBFQDLLUtil.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASIDBFQDLLUtil*	t_ASIDBFQDLLUtil;

CASIDBFQDLLUtil::CASIDBFQDLLUtil()
{
	m_hDLL		= NULL;
	m_lpObject	= NULL;
	m_bUseMutex	= FALSE;


	m_fnASIDBFQ_Next				= NULL;
	m_fnASIDBFQ_GetFieldByIdx		= NULL;
	m_fnASIDBFQ_GetFieldByName		= NULL;
	m_fnASIDBFQ_GetFieldFP			= NULL;


	m_fnASIDBF_ExeInsert			= NULL;
	m_fnASIDBF_ExeSelectByID		= NULL;
	m_fnASIDBF_ExeUpdateByID		= NULL;
	m_fnASIDBF_ExeDeleteByID		= NULL;
	m_fnASIDBF_PreSetDBF_TableItem	= NULL;


	m_fnASIDBF_FastSetCurTable			= NULL;
	m_fnASIDBF_FastPreSetDBF_TableItem	= NULL;
	m_fnASIDBF_FastExeInsertDBF			= NULL;
	m_fnASIDBF_FastExeUpdateDBF			= NULL;
}
//--------------------------------------------------------------------------

CASIDBFQDLLUtil::~CASIDBFQDLLUtil()
{
	if(m_hDLL)
	{
		FreeLibraryExt();
	}

	ClearMember();
	return;
}
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::LoadLibraryExt(LPCTSTR lpDLLPath)
{
	if(m_hDLL)		return 0;

	if( (m_hDLL = LoadLibrary(lpDLLPath)) == NULL)		return -1;

	if( LoadFunction() )								return -2;

	return 0;
}
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::FreeLibraryExt()
{
	if(m_hDLL)
	{
		if(m_lpObject)
		{
			ASIDBFQ_Free();
		}
		
		FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}
	return 0;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::LoadFunction()
{
	if(!m_hDLL)		return -100;

	{
		m_fnASIDBFQ_Next = (ASIDBFQ_NextType)GetProcAddress(m_hDLL, "ASIDBFQ_Next");
		if(!m_fnASIDBFQ_Next)		return -110;
		
		m_fnASIDBFQ_GetFieldByIdx = (ASIDBFQ_GetFieldByIdxType)GetProcAddress(m_hDLL, "ASIDBFQ_GetFieldByIdx");
		if(!m_fnASIDBFQ_GetFieldByIdx)	return -101;

		m_fnASIDBFQ_GetFieldByName = (ASIDBFQ_GetFieldByNameType)GetProcAddress(m_hDLL, "ASIDBFQ_GetFieldByName");
		if(!m_fnASIDBFQ_GetFieldByName)	return -102;

		m_fnASIDBFQ_GetFieldFP = (ASIDBFQ_GetFieldFPType)GetProcAddress(m_hDLL, "ASIDBFQ_GetFieldFP");
		if(!m_fnASIDBFQ_GetFieldFP)	return -103;


		m_fnASIDBF_GetTableNextID = (ASIDBF_GetTableNextIDType)GetProcAddress(m_hDLL, "ASIDBF_GetTableNextID");
		if(!m_fnASIDBF_GetTableNextID)	return -150;

		m_fnASIDBF_GetTableItemFPByPKey = (ASIDBF_GetTableItemFPByPKeyType)GetProcAddress(m_hDLL, "ASIDBF_GetTableItemFPByPKey");
		if(!m_fnASIDBF_GetTableItemFPByPKey)	return -151;


		m_fnASIDBF_ExeInsert = (ASIDBF_ExeInsertType)GetProcAddress(m_hDLL, "ASIDBF_ExeInsert");
		if(!m_fnASIDBF_ExeInsert)	return -200;

		m_fnASIDBF_ExeSelectByID = (ASIDBF_ExeSelectByIDType)GetProcAddress(m_hDLL, "ASIDBF_ExeSelectByID");
		if(!m_fnASIDBF_ExeSelectByID)	return -200;

		m_fnASIDBF_ExeUpdateByID = (ASIDBF_ExeUpdateByIDType)GetProcAddress(m_hDLL, "ASIDBF_ExeUpdateByID");
		if(!m_fnASIDBF_ExeUpdateByID)	return -201;

		m_fnASIDBF_ExeDeleteByID = (ASIDBF_ExeDeleteByIDType)GetProcAddress(m_hDLL, "ASIDBF_ExeDeleteByID");
		if(!m_fnASIDBF_ExeDeleteByID)	return -202;

		m_fnASIDBF_PreSetDBF_TableItem = (ASIDBF_PreSetDBF_TableItemType)GetProcAddress(m_hDLL, "ASIDBF_PreSetDBF_TableItem");
		if(!m_fnASIDBF_PreSetDBF_TableItem)	return -203;



		m_fnASIDBF_FastSetCurTable = (ASIDBF_FastSetCurTableType)GetProcAddress(m_hDLL, "ASIDBF_FastSetCurTable");
		if(!m_fnASIDBF_FastSetCurTable)	return -300;

		m_fnASIDBF_FastPreSetDBF_TableItem = (ASIDBF_FastPreSetDBF_TableItemType)GetProcAddress(m_hDLL, "ASIDBF_FastPreSetDBF_TableItem");
		if(!m_fnASIDBF_FastPreSetDBF_TableItem)	return -301;

		m_fnASIDBF_FastExeInsertDBF = (ASIDBF_FastExeInsertDBFType)GetProcAddress(m_hDLL, "ASIDBF_FastExeInsertDBF");
		if(!m_fnASIDBF_FastExeInsertDBF)	return -302;

		m_fnASIDBF_FastExeUpdateDBF = (ASIDBF_FastExeUpdateDBFType)GetProcAddress(m_hDLL, "ASIDBF_FastExeUpdateDBF");
		if(!m_fnASIDBF_FastExeUpdateDBF)	return -303;
	}
	return 0;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_Init(PASIDBFQ_INIT_INFO pscdbfq_init_info)
{
	if(!m_hDLL)		return -100;

	ASIDBFQ_InitType fnASIDBFQ_Init = (ASIDBFQ_InitType)GetProcAddress(m_hDLL, "ASIDBFQ_Init");
	if(!fnASIDBFQ_Init)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_Init(m_lpObject, pscdbfq_init_info);
		UnLockMutex();
	}
	
	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_Create()
{
	if(!m_hDLL)		return -100;
	if(m_lpObject)	return -101;

	ASIDBFQ_CreateType fnASIDBFQ_Create = (ASIDBFQ_CreateType)GetProcAddress(m_hDLL, "ASIDBFQ_Create");
	if(!fnASIDBFQ_Create)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_Create(&m_lpObject);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_Free()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_FreeType fnASIDBFQ_Free = (ASIDBFQ_FreeType)GetProcAddress(m_hDLL, "ASIDBFQ_Free");
	if(!fnASIDBFQ_Free)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_Free(&m_lpObject);
		UnLockMutex();
	}
	return nRet;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------



INT32	CASIDBFQDLLUtil::ASIDBFQ_CreateDatabase(LPCTSTR lpDBName, LPCTSTR lpUserID, LPCTSTR lpUserPw)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_CreateDatabaseType fnASIDBFQ_CreateDatabase = (ASIDBFQ_CreateDatabaseType)GetProcAddress(m_hDLL, "ASIDBFQ_CreateDatabase");
	if(!fnASIDBFQ_CreateDatabase)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_CreateDatabase(m_lpObject, lpDBName, lpUserID, lpUserPw);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_OpenDatabase(LPCTSTR lpDBName, LPCTSTR lpUserID, LPCTSTR lpUserPw, UINT32 nAccessMode)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_OpenDatabaseType fnASIDBFQ_OpenDatabase = (ASIDBFQ_OpenDatabaseType)GetProcAddress(m_hDLL, "ASIDBFQ_OpenDatabase");
	if(!fnASIDBFQ_OpenDatabase)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_OpenDatabase(m_lpObject, lpDBName, lpUserID, lpUserPw, nAccessMode);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_CloseDatabase(LPCTSTR lpDBName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_CloseDatabaseType fnASIDBFQ_CloseDatabase = (ASIDBFQ_CloseDatabaseType)GetProcAddress(m_hDLL, "ASIDBFQ_CloseDatabase");
	if(!fnASIDBFQ_CloseDatabase)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_CloseDatabase(m_lpObject, lpDBName);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_IsExistTable(CString strTBName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_IsExistTableType fnASIDBFQ_IsExistTable = (ASIDBFQ_IsExistTableType)GetProcAddress(m_hDLL, "ASIDBFQ_IsExistTable");
	if(!fnASIDBFQ_IsExistTable)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_IsExistTable(m_lpObject, strTBName);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_GetTableVer(CString strTBName, PUINT32 pnVer)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_GetTableVerType fnASIDBFQ_GetTableVer = (ASIDBFQ_GetTableVerType)GetProcAddress(m_hDLL, "ASIDBFQ_GetTableVer");
	if(!fnASIDBFQ_GetTableVer)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_GetTableVer(m_lpObject, strTBName, pnVer);
		UnLockMutex();
	}

	return nRet;

}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_SetTableVer(CString strTBName, UINT32 nVer)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_SetTableVerType fnASIDBFQ_SetTableVer = (ASIDBFQ_SetTableVerType)GetProcAddress(m_hDLL, "ASIDBFQ_SetTableVer");
	if(!fnASIDBFQ_SetTableVer)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_SetTableVer(m_lpObject, strTBName, nVer);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_QuerySelect(CString strQuery)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_QuerySelectType fnASIDBFQ_QuerySelect = (ASIDBFQ_QuerySelectType)GetProcAddress(m_hDLL, "ASIDBFQ_QuerySelect");
	if(!fnASIDBFQ_QuerySelect)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_QuerySelect(m_lpObject, strQuery);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_QueryExecute(CString strQuery)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_QueryExecuteType fnASIDBFQ_QueryExecute = (ASIDBFQ_QueryExecuteType)GetProcAddress(m_hDLL, "ASIDBFQ_QueryExecute");
	if(!fnASIDBFQ_QueryExecute)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_QueryExecute(m_lpObject, strQuery);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_First()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_FirstType fnASIDBFQ_First = (ASIDBFQ_FirstType)GetProcAddress(m_hDLL, "ASIDBFQ_First");
	if(!fnASIDBFQ_First)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_First(m_lpObject);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_Last()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_LastType fnASIDBFQ_Last = (ASIDBFQ_LastType)GetProcAddress(m_hDLL, "ASIDBFQ_Last");
	if(!fnASIDBFQ_Last)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_Last(m_lpObject);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_FirstKey(UINT64 nKey)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_FirstKeyType fnASIDBFQ_FirstKey = (ASIDBFQ_FirstKeyType)GetProcAddress(m_hDLL, "ASIDBFQ_FirstKey");
	if(!fnASIDBFQ_FirstKey)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_FirstKey(m_lpObject, nKey);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_LastKey(UINT64 nKey)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_LastKeyType fnASIDBFQ_LastKey = (ASIDBFQ_LastKeyType)GetProcAddress(m_hDLL, "ASIDBFQ_LastKey");
	if(!fnASIDBFQ_LastKey)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_LastKey(m_lpObject, nKey);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_Next()
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBFQ_Next)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBFQ_Next(m_lpObject);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_NextStop()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_NextStopType fnASIDBFQ_NextStop = (ASIDBFQ_NextStopType)GetProcAddress(m_hDLL, "ASIDBFQ_NextStop");
	if(!fnASIDBFQ_NextStop)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_NextStop(m_lpObject);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_GetFieldByIdx(INT32 nIdx, PDBF_ITEM_RTN pdfir)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBFQ_GetFieldByIdx)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBFQ_GetFieldByIdx(m_lpObject, nIdx, pdfir);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_GetFieldByName(LPCTSTR lpName, PDBF_ITEM_RTN pdfir)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBFQ_GetFieldByName)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBFQ_GetFieldByName(m_lpObject, lpName, pdfir);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_GetFieldFP(PUINT64 pnFP)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBFQ_GetFieldFP)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBFQ_GetFieldFP(m_lpObject, pnFP);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_GetLastID(PUINT64 pnLastID)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_GetLastIDType fnASIDBFQ_GetLastID = (ASIDBFQ_GetLastIDType)GetProcAddress(m_hDLL, "ASIDBFQ_GetLastID");
	if(!fnASIDBFQ_GetLastID)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_GetLastID(m_lpObject, pnLastID);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_GetLastFp(PUINT64 pnLastFp)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_GetLastFpType fnASIDBFQ_GetLastFp = (ASIDBFQ_GetLastFpType)GetProcAddress(m_hDLL, "ASIDBFQ_GetLastFp");
	if(!fnASIDBFQ_GetLastFp)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_GetLastFp(m_lpObject, pnLastFp);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBFQ_GetTbLastID(LPCTSTR lpTableName, PUINT64 pnLastID)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBFQ_GetTbLastIDType fnASIDBFQ_GetTbLastID = (ASIDBFQ_GetTbLastIDType)GetProcAddress(m_hDLL, "ASIDBFQ_GetTbLastID");
	if(!fnASIDBFQ_GetTbLastID)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBFQ_GetTbLastID(m_lpObject, lpTableName, pnLastID);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

PDBF_ITEM_RTN	CASIDBFQDLLUtil::GetField(INT32 nIdx)
{
	CASIDBFQDLLMember* pMember = GetMember();
	
	INT32 nRtn = ASIDBFQ_GetFieldByIdx(nIdx, &(pMember->m_tDFIR));
	return &(pMember->m_tDFIR);
}
//--------------------------------------------------------------------------

PDBF_ITEM_RTN	CASIDBFQDLLUtil::GetField(LPCTSTR szName)
{
	CASIDBFQDLLMember* pMember = GetMember();

	INT32 nRtn = ASIDBFQ_GetFieldByName(szName, &(pMember->m_tDFIR));
	return &(pMember->m_tDFIR);
}
//--------------------------------------------------------------------------

UINT64			CASIDBFQDLLUtil::GetFieldFP()
{
	CASIDBFQDLLMember* pMember = GetMember();

	INT32 nRtn = ASIDBFQ_GetFieldFP(&(pMember->m_nCurFP));
	return pMember->m_nCurFP;
}
//--------------------------------------------------------------------------

UINT64			CASIDBFQDLLUtil::GetLastID()
{
	CASIDBFQDLLMember* pMember = GetMember();

	pMember->m_nLastID = 0;
	INT32 nRtn = ASIDBFQ_GetLastID(&(pMember->m_nLastID));
	return pMember->m_nLastID;
}
//--------------------------------------------------------------------------

UINT64			CASIDBFQDLLUtil::GetLastFp()
{
	CASIDBFQDLLMember* pMember = GetMember();

	pMember->m_nLastFp = 0;
	INT32 nRtn = ASIDBFQ_GetLastFp(&(pMember->m_nLastFp));
	return pMember->m_nLastFp;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_ReloadDBF_Table()
{  
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;
    
	ASIDBF_ReloadDBF_TableType fnASIDBF_ReloadDBF_Table = (ASIDBF_ReloadDBF_TableType)GetProcAddress(m_hDLL, "ASIDBF_ReloadDBF_Table");
	if(!fnASIDBF_ReloadDBF_Table)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_ReloadDBF_Table(m_lpObject);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_GetTbColumnIdx(LPCTSTR lpColName, PUINT32 nColIdx, LPCTSTR lpTableName)
{  
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

    CheckCurTableName(lpTableName);

	ASIDBF_GetTbColumnIdxType fnASIDBF_GetTbColumnIdx = (ASIDBF_GetTbColumnIdxType)GetProcAddress(m_hDLL, "ASIDBF_GetTbColumnIdx");
	if(!fnASIDBF_GetTbColumnIdx)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_GetTbColumnIdx(m_lpObject, GetMemberTbName(), lpColName, nColIdx);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_PreSetDBF_TableItem(LPCTSTR lpColName, PVOID lpBuff, UINT32 nBufLen, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;

    CheckCurTableName(lpTableName);

	if(!m_fnASIDBF_PreSetDBF_TableItem)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBF_PreSetDBF_TableItem(m_lpObject, GetMemberTbName(), lpColName, lpBuff, nBufLen);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_ExeSelectByID(UINT32 nID, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;     

	CheckCurTableName(lpTableName);

	if(!m_fnASIDBF_ExeSelectByID)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBF_ExeSelectByID(m_lpObject, GetMemberTbName(), nID);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_ExeUpdateByID(UINT32 nID, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;     

	CheckCurTableName(lpTableName);

	if(!m_fnASIDBF_ExeUpdateByID)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBF_ExeUpdateByID(m_lpObject, GetMemberTbName(), nID);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_ExeDeleteByID(UINT32 nID, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;     

	CheckCurTableName(lpTableName);

	if(!m_fnASIDBF_ExeDeleteByID)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBF_ExeDeleteByID(m_lpObject, GetMemberTbName(), nID);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_ExeInsert(UINT64& nNewFP, UINT64& nLastID, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;      

    CheckCurTableName(lpTableName);

	if(!m_fnASIDBF_ExeInsert)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBF_ExeInsert(m_lpObject, GetMemberTbName(), &nNewFP, &nLastID);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_ExeSelect(UINT64 nFP, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;     

    CheckCurTableName(lpTableName);

	ASIDBF_ExeSelectType fnASIDBF_ExeSelect = (ASIDBF_ExeSelectType)GetProcAddress(m_hDLL, "ASIDBF_ExeSelect");
	if(!fnASIDBF_ExeSelect)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_ExeSelect(m_lpObject, GetMemberTbName(), nFP);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_ExeUpdate(UINT64& nFP, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;    

    CheckCurTableName(lpTableName);

	ASIDBF_ExeUpdateType fnASIDBF_ExeUpdate = (ASIDBF_ExeUpdateType)GetProcAddress(m_hDLL, "ASIDBF_ExeUpdate");
	if(!fnASIDBF_ExeUpdate)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_ExeUpdate(m_lpObject, GetMemberTbName(), &nFP);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_ExeDelete(UINT64 nFP, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;    

    CheckCurTableName(lpTableName);

	ASIDBF_ExeDeleteType fnASIDBF_ExeDelete = (ASIDBF_ExeDeleteType)GetProcAddress(m_hDLL, "ASIDBF_ExeDelete");
	if(!fnASIDBF_ExeDelete)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_ExeDelete(m_lpObject, GetMemberTbName(), nFP);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_First(LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

    CheckCurTableName(lpTableName);

	ASIDBF_FirstType fnASIDBF_First = (ASIDBF_FirstType)GetProcAddress(m_hDLL, "ASIDBF_First");
	if(!fnASIDBF_First)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_First(m_lpObject, GetMemberTbName());
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_Last(LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;      

    CheckCurTableName(lpTableName);

	ASIDBF_LastType fnASIDBF_Last = (ASIDBF_LastType)GetProcAddress(m_hDLL, "ASIDBF_Last");
	if(!fnASIDBF_Last)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_Last(m_lpObject, GetMemberTbName());
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_FirstKey(LPCTSTR lpTableName, UINT64 nKey)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_FirstKeyType fnASIDBF_FirstKey = (ASIDBF_FirstKeyType)GetProcAddress(m_hDLL, "ASIDBF_FirstKey");
	if(!fnASIDBF_FirstKey)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_FirstKey(m_lpObject, GetMemberTbName(), nKey);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_LastKey(LPCTSTR lpTableName, UINT64 nKey)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;      

	CheckCurTableName(lpTableName);

	ASIDBF_LastKeyType fnASIDBF_LastKey = (ASIDBF_LastKeyType)GetProcAddress(m_hDLL, "ASIDBF_LastKey");
	if(!fnASIDBF_LastKey)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_LastKey(m_lpObject, GetMemberTbName(), nKey);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_Next(LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;    

    CheckCurTableName(lpTableName);

	ASIDBF_NextType fnASIDBF_Next = (ASIDBF_NextType)GetProcAddress(m_hDLL, "ASIDBF_Next");
	if(!fnASIDBF_Next)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_Next(m_lpObject, GetMemberTbName(), NULL);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_NextStop()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_NextStopType fnASIDBF_NextStop = (ASIDBF_NextStopType)GetProcAddress(m_hDLL, "ASIDBF_NextStop");
	if(!fnASIDBF_NextStop)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_NextStop(m_lpObject);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_GetFieldByIdx(INT32 nIdx, PDBF_ITEM_RTN pdfir)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetFieldByIdxType fnASIDBF_GetFieldByIdx = (ASIDBF_GetFieldByIdxType)GetProcAddress(m_hDLL, "ASIDBF_GetFieldByIdx");
	if(!fnASIDBF_GetFieldByIdx)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_GetFieldByIdx(m_lpObject, nIdx, pdfir);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_GetFieldByName(LPCTSTR lpName, PDBF_ITEM_RTN pdfir)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetFieldByNameType fnASIDBF_GetFieldByName = (ASIDBF_GetFieldByNameType)GetProcAddress(m_hDLL, "ASIDBF_GetFieldByName");
	if(!fnASIDBF_GetFieldByName)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_GetFieldByName(m_lpObject, lpName, pdfir);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_GetTableNextID(LPCTSTR lpName, PUINT64 pnNextID)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_GetTableNextID)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBF_GetTableNextID(m_lpObject, lpName, pnNextID);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_GetTableItemFPByPKey(LPCTSTR lpName, UINT64 nKey, PUINT64 pnFP)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_GetTableItemFPByPKey)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = m_fnASIDBF_GetTableItemFPByPKey(m_lpObject, lpName, nKey, pnFP);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_BackupDB(LPCTSTR lpBkFileName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_BackupDBType fnASIDBF_BackupDB = (ASIDBF_BackupDBType)GetProcAddress(m_hDLL, "ASIDBF_BackupDB");
	if(!fnASIDBF_BackupDB)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_BackupDB(m_lpObject, lpBkFileName);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_RecoveryDB(INT32 nRemainMode)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_RecoveryDBType fnASIDBF_RecoveryDB = (ASIDBF_RecoveryDBType)GetProcAddress(m_hDLL, "ASIDBF_RecoveryDB");
	if(!fnASIDBF_RecoveryDB)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_RecoveryDB(m_lpObject, nRemainMode);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------
INT32	CASIDBFQDLLUtil::ASIDBF_ChangeAuthInfoDB(LPCTSTR lpCurUserID, LPCTSTR lpCurUserPw, LPCTSTR lpNewUserID, LPCTSTR lpNewUserPw, UINT32 nChkNewType)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_ChangeAuthInfoDBType fnASIDBF_ChangeAuthInfoDB = (ASIDBF_ChangeAuthInfoDBType)GetProcAddress(m_hDLL, "ASIDBF_ChangeAuthInfoDB");
	if(!fnASIDBF_ChangeAuthInfoDB)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_ChangeAuthInfoDB(m_lpObject, lpCurUserID, lpCurUserPw, lpNewUserID, lpNewUserPw, nChkNewType);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------
INT32	CASIDBFQDLLUtil::ASIDBF_IsAutoRestore(INT32& nMode)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_IsAutoRestoreType fnASIDBF_IsAutoRestore = (ASIDBF_IsAutoRestoreType)GetProcAddress(m_hDLL, "ASIDBF_IsAutoRestore");
	if(!fnASIDBF_IsAutoRestore)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_IsAutoRestore(m_lpObject, nMode);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::ASIDBF_SetTItemEncTypeDB(UINT32 nDefEncType)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_SetTItemEncTypeDBType fnASIDBF_SetTItemEncTypeDB = (ASIDBF_SetTItemEncTypeDBType)GetProcAddress(m_hDLL, "ASIDBF_SetTItemEncTypeDB");
	if(!fnASIDBF_SetTItemEncTypeDB)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_SetTItemEncTypeDB(m_lpObject, nDefEncType);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::ASIDBF_GetTItemEncTypeDB(PUINT32 pnDefEncType)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTItemEncTypeDBType fnASIDBF_GetTItemEncTypeDB = (ASIDBF_GetTItemEncTypeDBType)GetProcAddress(m_hDLL, "ASIDBF_GetTItemEncTypeDB");
	if(!fnASIDBF_GetTItemEncTypeDB)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_GetTItemEncTypeDB(m_lpObject, pnDefEncType);
		UnLockMutex();
	}

	return nRet;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_FastSetCurTable(LPCTSTR szTName)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_FastSetCurTable)	return -101;

	return m_fnASIDBF_FastSetCurTable(m_lpObject, szTName);
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_FastPreSetDBF_TableItem(LPCTSTR szColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_FastPreSetDBF_TableItem)	return -101;

	return m_fnASIDBF_FastPreSetDBF_TableItem(m_lpObject, szColName, nMemberType, lpBuff, nBufLen);
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_FastExeInsertDBF(PUINT64 pnLastID, PUINT64 pnNewFP)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_FastExeInsertDBF)	return -101;

	return m_fnASIDBF_FastExeInsertDBF(m_lpObject, pnLastID, pnNewFP);
}
//--------------------------------------------------------------------------

INT32	CASIDBFQDLLUtil::ASIDBF_FastExeUpdateDBF(UINT32 nID)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_FastExeUpdateDBF)	return -101;

	return m_fnASIDBF_FastExeUpdateDBF(m_lpObject, nID);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


INT32			CASIDBFQDLLUtil::ASIDBF_GetFieldCIdx(LPCTSTR szCName)
{
	UINT32 nColIdx = 0;
    if(ASIDBF_GetTbColumnIdx(szCName, &nColIdx))		return -1;
    return (INT32)nColIdx;
}
//--------------------------------------------------------------------------

PDBF_ITEM_RTN	CASIDBFQDLLUtil::ASIDBF_GetField(INT32 nIdx)
{
	CASIDBFQDLLMember* pMember = GetMember();

	INT32 nRtn = ASIDBF_GetFieldByIdx(nIdx, &(pMember->m_tDFIR));
	return &(pMember->m_tDFIR);
}
//--------------------------------------------------------------------------

PDBF_ITEM_RTN	CASIDBFQDLLUtil::ASIDBF_GetField(LPCTSTR szName)
{
	CASIDBFQDLLMember* pMember = GetMember();

	INT32 nRtn = ASIDBF_GetFieldByName(szName, &(pMember->m_tDFIR));
	return &(pMember->m_tDFIR);
}
//--------------------------------------------------------------------------

void	CASIDBFQDLLUtil::CheckCurTableName(LPCTSTR lpTableName)
{
	CASIDBFQDLLMember* pMember = GetMember();

	if(lpTableName)		pMember->m_strCurTableName = lpTableName;
	pMember->m_lpTableName = (LPTSTR)(LPCTSTR)(pMember->m_strCurTableName);
	return;
}
//--------------------------------------------------------------------------

void	CASIDBFQDLLUtil::SetCurTableName(LPCTSTR lpTableName)
{
	CASIDBFQDLLMember* pMember = GetMember();

	pMember->m_strCurTableName = lpTableName;
	return;
}
//--------------------------------------------------------------------------

void	CASIDBFQDLLUtil::SetCurTableName(CString lpTableName)
{
	SetCurTableName((LPCTSTR)lpTableName);
    return;
}  
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::ASIDBF_PreSetValue16(LPCTSTR lpColName, UINT16& nValue)
{
	return ASIDBF_PreSetDBF_TableItem(lpColName, &nValue);
}
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::ASIDBF_PreSetValue32(LPCTSTR lpColName, UINT32& nValue)
{
	return ASIDBF_PreSetDBF_TableItem(lpColName, &nValue);
}
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::ASIDBF_PreSetValue64(LPCTSTR lpColName, UINT64& nValue)
{                                               
	return ASIDBF_PreSetDBF_TableItem(lpColName, &nValue);
}
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::ASIDBF_PreSetValue(LPCTSTR lpColName, CString& strValue)
{                                                
	return ASIDBF_PreSetDBF_TableItem(lpColName, (LPTSTR)(LPCTSTR)strValue, strValue.GetLength());
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::ASIDBF_FastPreSetValue16(LPCTSTR lpColName, UINT16& nValue)
{
	return ASIDBF_FastPreSetDBF_TableItem(lpColName, ASI_DBF_MEMBER_TYPE_UINT16, &nValue);
}
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::ASIDBF_FastPreSetValue32(LPCTSTR lpColName, UINT32& nValue)
{
	return ASIDBF_FastPreSetDBF_TableItem(lpColName, ASI_DBF_MEMBER_TYPE_UINT32, &nValue);
}
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::ASIDBF_FastPreSetValue64(LPCTSTR lpColName, UINT64& nValue)
{                                               
	return ASIDBF_FastPreSetDBF_TableItem(lpColName, ASI_DBF_MEMBER_TYPE_UINT64, &nValue);
}
//--------------------------------------------------------------------------

INT32			CASIDBFQDLLUtil::ASIDBF_FastPreSetValue(LPCTSTR lpColName, CString& strValue)
{                                                
	return ASIDBF_FastPreSetDBF_TableItem(lpColName, ASI_DBF_MEMBER_TYPE_STRING, (LPTSTR)(LPCTSTR)strValue, strValue.GetLength());
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

UINT16			CASIDBFQDLLUtil::GetField_UINT16(INT32 nIdx)
{	
	DBF_ITEM_RTN dir;
	if(ASIDBFQ_GetFieldByIdx(nIdx, &dir))	return 0;
	
	if(dir.pRtnUINT16 == NULL)	return 0;

	return *(dir.pRtnUINT16);
}
//--------------------------------------------------------------------------

UINT32			CASIDBFQDLLUtil::GetField_UINT32(INT32 nIdx)
{
	DBF_ITEM_RTN dir;
	if(ASIDBFQ_GetFieldByIdx(nIdx, &dir))	return 0;

	if(dir.pRtnUINT32 == NULL)	return 0;

	return *(dir.pRtnUINT32);
}
//--------------------------------------------------------------------------

UINT64			CASIDBFQDLLUtil::GetField_UINT64(INT32 nIdx)
{
	DBF_ITEM_RTN dir;
	if(ASIDBFQ_GetFieldByIdx(nIdx, &dir))	return 0;

	if(dir.pRtnUINT64 == NULL)	return 0;

	return *(dir.pRtnUINT64);
}
//--------------------------------------------------------------------------

CString			CASIDBFQDLLUtil::GetField_String(INT32 nIdx)
{
	DBF_ITEM_RTN dir;
	if(ASIDBFQ_GetFieldByIdx(nIdx, &dir))
		return "";

	if(dir.szRtnString == NULL)	return "";

	return CString(dir.szRtnString, dir.nRtnLen);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32					CASIDBFQDLLUtil::LockMutex()
{
	if(!m_bUseMutex)	return 0;

	//m_tDBFQMutex.Lock();
	return 0;
}
//--------------------------------------------------------------------------

INT32					CASIDBFQDLLUtil::UnLockMutex()
{
	if(!m_bUseMutex)	return 0;

	//m_tDBFQMutex.UnLock();
	return 0;
}
//--------------------------------------------------------------------------

CASIDBFQDLLMember*			CASIDBFQDLLUtil::GetMember()
{
	CASIDBFQDLLMember*	pRtn = NULL;
	m_tMutexMember.Lock();

	UINT64 nThreadID = (UINT64)gettid();
	TMapASIDBFQDLLMemberItor find = m_tDbfqMemberMap.find(nThreadID);
	if(find == m_tDbfqMemberMap.end())
	{
		CASIDBFQDLLMember* pMember = new CASIDBFQDLLMember();
		m_tDbfqMemberMap[nThreadID] = pMember;
		find = m_tDbfqMemberMap.find(nThreadID);	
	}

	pRtn = (CASIDBFQDLLMember*)(find->second);
	m_tMutexMember.UnLock();
	return pRtn;
}
//---------------------------------------------------------------------

void					CASIDBFQDLLUtil::ClearMember()
{
	TMapASIDBFQDLLMemberItor begin, end;
	begin = m_tDbfqMemberMap.begin();	end = m_tDbfqMemberMap.end();
	for(begin; begin != end; begin++)
	{
		CASIDBFQDLLMember* pdata = (CASIDBFQDLLMember*)(begin->second);
		if(!pdata)	continue;

		delete pdata;
	}
	m_tDbfqMemberMap.clear();
	return;
}
//---------------------------------------------------------------------

LPTSTR			CASIDBFQDLLUtil::GetMemberTbName()
{
	CASIDBFQDLLMember* pMember = GetMember();

	return pMember->m_lpTableName;
}
//---------------------------------------------------------------------
INT32	CASIDBFQDLLUtil::ASIDBF_GetTbInfo(PDBF_TABLE_INFO_RTN pDTIR, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTbInfoType fnASIDBF_GetTbInfo = (ASIDBF_GetTbInfoType)GetProcAddress(m_hDLL, "ASIDBF_GetTbInfo");
	if(!fnASIDBF_GetTbInfo)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_GetTbInfo(m_lpObject, lpTableName, pDTIR);
		UnLockMutex();
	}
	return nRet;
}
//---------------------------------------------------------------------
INT32	CASIDBFQDLLUtil::ASIDBF_GetTbColumnList(LPCTSTR lpTableName, PDBF_TABLE_COLUMN_RTN pTableColumnList, UINT32 nBuffLen, PUINT32 pnColumnNum)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTbColumnListType fnASIDBF_GetTbColumnList = (ASIDBF_GetTbColumnListType)GetProcAddress(m_hDLL, "ASIDBF_GetTbColumnList");
	if(!fnASIDBF_GetTbColumnList)	return -101;

	INT32 nRet = -1;
	{
		LockMutex();
		nRet = fnASIDBF_GetTbColumnList(m_lpObject, lpTableName, pTableColumnList, nBuffLen, pnColumnNum);
		UnLockMutex();
	}
	return nRet;
}
//---------------------------------------------------------------------









