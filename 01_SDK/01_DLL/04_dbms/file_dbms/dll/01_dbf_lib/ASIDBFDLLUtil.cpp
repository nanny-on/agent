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


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASIDBFDLLUtil*	t_ASIDBFDLLUtil;

CASIDBFDLLUtil::CASIDBFDLLUtil()
{
	m_hDLL		= NULL;
	m_lpObject	= NULL;

	m_fnASIDBF_Next				= NULL;
	m_fnASIDBF_GetFieldByIdx	= NULL;
	m_fnASIDBF_GetFieldByName	= NULL;
	m_fnASIDBF_GetFieldFP		= NULL;

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

CASIDBFDLLUtil::~CASIDBFDLLUtil()
{
	if(m_hDLL)
	{
		FreeLibraryExt();
	}
	return;
}
//--------------------------------------------------------------------------

INT32			CASIDBFDLLUtil::LoadLibraryExt(LPCTSTR lpDLLPath)
{
	if(m_hDLL)		return 0;

	if( (m_hDLL = LoadLibrary(lpDLLPath)) == NULL)		return -1;

	if( LoadFunction() )								return -2;


	return 0;
}
//--------------------------------------------------------------------------

INT32			CASIDBFDLLUtil::FreeLibraryExt()
{
	if(m_hDLL)
	{
		if(m_lpObject)
		{
			ASIDBF_Free();
		}
		
		FreeLibrary(m_hDLL);

        ClearMember();
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

INT32	CASIDBFDLLUtil::LoadFunction()
{
	if(!m_hDLL)		return -100;

	{
		m_fnASIDBF_Next = (ASIDBF_NextType)GetProcAddress(m_hDLL, "ASIDBF_Next");
		if(!m_fnASIDBF_Next)	return -110;
		
		m_fnASIDBF_GetFieldByIdx = (ASIDBF_GetFieldByIdxType)GetProcAddress(m_hDLL, "ASIDBF_GetFieldByIdx");
		if(!m_fnASIDBF_GetFieldByIdx)	return -101;

		m_fnASIDBF_GetFieldByName = (ASIDBF_GetFieldByNameType)GetProcAddress(m_hDLL, "ASIDBF_GetFieldByName");
		if(!m_fnASIDBF_GetFieldByName)	return -102;

		m_fnASIDBF_GetFieldFP = (ASIDBF_GetFieldFPType)GetProcAddress(m_hDLL, "ASIDBF_GetFieldFP");
		if(!m_fnASIDBF_GetFieldFP)	return -103;

		m_fnASIDBF_GetTableNextID = (ASIDBF_GetTableNextIDType)GetProcAddress(m_hDLL, "ASIDBF_GetTableNextID");
		if(!m_fnASIDBF_GetTableNextID)	return -104;

		m_fnASIDBF_GetTableItemFPByPKey = (ASIDBF_GetTableItemFPByPKeyType)GetProcAddress(m_hDLL, "ASIDBF_GetTableItemFPByPKey");
		if(!m_fnASIDBF_GetTableItemFPByPKey)	return -105;



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

INT32	CASIDBFDLLUtil::ASIDBF_Init(PASIDBF_INIT_INFO pasidbf_init_info)
{
	if(!m_hDLL)		return -100;

	ASIDBF_InitType fnASIDBF_Init = (ASIDBF_InitType)GetProcAddress(m_hDLL, "ASIDBF_Init");
	if(!fnASIDBF_Init)	return -101;

	return fnASIDBF_Init(pasidbf_init_info);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_Create()
{
	if(!m_hDLL)		return -100;
	if(m_lpObject)	return -101;

	ASIDBF_CreateType fnASIDBF_Create = (ASIDBF_CreateType)GetProcAddress(m_hDLL, "ASIDBF_Create");
	if(!fnASIDBF_Create)	return -101;

	return fnASIDBF_Create(&m_lpObject);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_Free()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_FreeType fnASIDBF_Free = (ASIDBF_FreeType)GetProcAddress(m_hDLL, "ASIDBF_Free");
	if(!fnASIDBF_Free)	return -101;

	return fnASIDBF_Free(&m_lpObject);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_SetOpTickCntLog(UINT32 nChkOpTickCnt)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_SetOpTickCntLogType fnASIDBF_SetOpTickCntLog = (ASIDBF_SetOpTickCntLogType)GetProcAddress(m_hDLL, "ASIDBF_SetOpTickCntLog");
	if(!fnASIDBF_SetOpTickCntLog)	return -101;

	return fnASIDBF_SetOpTickCntLog(m_lpObject, nChkOpTickCnt);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_SetASyncMode(UINT32 nMode)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_SetASyncModeType fnASIDBF_SetASyncMode = (ASIDBF_SetASyncModeType)GetProcAddress(m_hDLL, "ASIDBF_SetASyncMode");
	if(!fnASIDBF_SetASyncMode)	return -101;

	return fnASIDBF_SetASyncMode(m_lpObject, nMode);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_CreateDB(LPCTSTR lpFileName, LPCTSTR lpUserID, LPCTSTR lpUserPw)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_CreateDBType fnASIDBF_CreateDB = (ASIDBF_CreateDBType)GetProcAddress(m_hDLL, "ASIDBF_CreateDB");
	if(!fnASIDBF_CreateDB)	return -101;

	return fnASIDBF_CreateDB(m_lpObject, lpFileName, lpUserID, lpUserPw);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_OpenDB(LPCTSTR lpFileName, LPCTSTR lpUserID, LPCTSTR lpUserPw, UINT32 nAccessMode)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_OpenDBType fnASIDBF_OpenDB = (ASIDBF_OpenDBType)GetProcAddress(m_hDLL, "ASIDBF_OpenDB");
	if(!fnASIDBF_OpenDB)	return -101;

	return fnASIDBF_OpenDB(m_lpObject, lpFileName, lpUserID, lpUserPw, nAccessMode);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_CloseDB()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_CloseDBType fnASIDBF_CloseDB = (ASIDBF_CloseDBType)GetProcAddress(m_hDLL, "ASIDBF_CloseDB");
	if(!fnASIDBF_CloseDB)	return -101;

	return fnASIDBF_CloseDB(m_lpObject);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_RemoveDB()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_RemoveDBType fnASIDBF_RemoveDB = (ASIDBF_RemoveDBType)GetProcAddress(m_hDLL, "ASIDBF_RemoveDB");
	if(!fnASIDBF_RemoveDB)	return -101;

	return fnASIDBF_RemoveDB(m_lpObject);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_RecoveryDB(UINT32 nRemainMode)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_RecoveryDBType fnASIDBF_RecoveryDB = (ASIDBF_RecoveryDBType)GetProcAddress(m_hDLL, "ASIDBF_RecoveryDB");
	if(!fnASIDBF_RecoveryDB)	return -101;

	return fnASIDBF_RecoveryDB(m_lpObject, nRemainMode);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_BackupDB(LPCTSTR lpBkFileName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_BackupDBType fnASIDBF_BackupDB = (ASIDBF_BackupDBType)GetProcAddress(m_hDLL, "ASIDBF_BackupDB");
	if(!fnASIDBF_BackupDB)	return -101;

	return fnASIDBF_BackupDB(m_lpObject, lpBkFileName);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_IsAutoRestore(INT32& nMode)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_IsAutoRestoreType fnASIDBF_IsAutoRestore = (ASIDBF_IsAutoRestoreType)GetProcAddress(m_hDLL, "ASIDBF_IsAutoRestore");
	if(!fnASIDBF_IsAutoRestore)	return -101;

	return fnASIDBF_IsAutoRestore(m_lpObject, nMode);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ChangeAuthInfoDB(LPCTSTR lpCurUserID, LPCTSTR lpCurUserPw, LPCTSTR lpNewUserID, LPCTSTR lpNewUserPw, UINT32 nChkNewType)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_ChangeAuthInfoDBType fnASIDBF_ChangeAuthInfoDB = (ASIDBF_ChangeAuthInfoDBType)GetProcAddress(m_hDLL, "ASIDBF_ChangeAuthInfoDB");
	if(!fnASIDBF_ChangeAuthInfoDB)	return -101;

	return fnASIDBF_ChangeAuthInfoDB(m_lpObject, lpCurUserID, lpCurUserPw, lpNewUserID, lpNewUserPw, nChkNewType);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_SetTItemEncTypeDB(UINT32 nDefEncType)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_SetTItemEncTypeDBType fnASIDBF_SetTItemEncTypeDB = (ASIDBF_SetTItemEncTypeDBType)GetProcAddress(m_hDLL, "ASIDBF_SetTItemEncTypeDB");
	if(!fnASIDBF_SetTItemEncTypeDB)	return -101;

	return fnASIDBF_SetTItemEncTypeDB(m_lpObject, nDefEncType);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_SetTItemCompTypeDB(UINT32 nDefCompType)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_SetTItemCompTypeDBType fnASIDBF_SetTItemCompTypeDB = (ASIDBF_SetTItemCompTypeDBType)GetProcAddress(m_hDLL, "ASIDBF_SetTItemCompTypeDB");
	if(!fnASIDBF_SetTItemCompTypeDB)	return -101;

	return fnASIDBF_SetTItemCompTypeDB(m_lpObject, nDefCompType);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTItemEncTypeDB(PUINT32 pnDefEncType)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTItemEncTypeDBType fnASIDBF_GetTItemEncTypeDB = (ASIDBF_GetTItemEncTypeDBType)GetProcAddress(m_hDLL, "ASIDBF_GetTItemEncTypeDB");
	if(!fnASIDBF_GetTItemEncTypeDB)	return -101;

	return fnASIDBF_GetTItemEncTypeDB(m_lpObject, pnDefEncType);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTItemCompTypeDB(PUINT32 pnDefCompType)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTItemCompTypeDBType fnASIDBF_GetTItemCompTypeDB = (ASIDBF_GetTItemCompTypeDBType)GetProcAddress(m_hDLL, "ASIDBF_GetTItemCompTypeDB");
	if(!fnASIDBF_GetTItemCompTypeDB)	return -101;

	return fnASIDBF_GetTItemCompTypeDB(m_lpObject, pnDefCompType);
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

INT32	CASIDBFDLLUtil::ASIDBF_PreSetDBF_TableColumn(LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_PreSetDBF_TableColumnType fnASIDBF_PreSetDBF_TableColumn = (ASIDBF_PreSetDBF_TableColumnType)GetProcAddress(m_hDLL, "ASIDBF_PreSetDBF_TableColumn");
	if(!fnASIDBF_PreSetDBF_TableColumn)	return -101;

	return fnASIDBF_PreSetDBF_TableColumn(m_lpObject, GetCurTableName(), lpColName, nMemberType, nDefaultValue, nInitStrLen, nColOption);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ForceCreateDBF_Table(LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_ForceCreateDBF_TableType fnASIDBF_ForceCreateDBF_Table = (ASIDBF_ForceCreateDBF_TableType)GetProcAddress(m_hDLL, "ASIDBF_ForceCreateDBF_Table");
	if(!fnASIDBF_ForceCreateDBF_Table)	return -101;

	return fnASIDBF_ForceCreateDBF_Table(m_lpObject, GetCurTableName());
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ModifyDBF_TableAddColumn(LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_ModifyDBF_TableAddColumnType fnASIDBF_ModifyDBF_TableAddColumn = (ASIDBF_ModifyDBF_TableAddColumnType)GetProcAddress(m_hDLL, "ASIDBF_ModifyDBF_TableAddColumn");
	if(!fnASIDBF_ModifyDBF_TableAddColumn)	return -101;

	return fnASIDBF_ModifyDBF_TableAddColumn(m_lpObject, GetCurTableName(), lpColName, nMemberType, nDefaultValue, nInitStrLen, nColOption);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ModifyDBF_TableDelColumn(LPCTSTR lpColName, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_ModifyDBF_TableDelColumnType fnASIDBF_ModifyDBF_TableDelColumn = (ASIDBF_ModifyDBF_TableDelColumnType)GetProcAddress(m_hDLL, "ASIDBF_ModifyDBF_TableDelColumn");
	if(!fnASIDBF_ModifyDBF_TableDelColumn)	return -101;

	return fnASIDBF_ModifyDBF_TableDelColumn(m_lpObject, GetCurTableName(), lpColName);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ModifyDBF_TableRenameColumn(LPCTSTR lpColName, LPCTSTR lpNewColName, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_ModifyDBF_TableRenameColumnType fnASIDBF_ModifyDBF_TableRenameColumn = (ASIDBF_ModifyDBF_TableRenameColumnType)GetProcAddress(m_hDLL, "ASIDBF_ModifyDBF_TableRenameColumn");
	if(!fnASIDBF_ModifyDBF_TableRenameColumn)	return -101;

	return fnASIDBF_ModifyDBF_TableRenameColumn(m_lpObject, GetCurTableName(), lpColName, lpNewColName);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ModifyDBF_TableReLenColumn(LPCTSTR lpColName, UINT32 nDefLen, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_ModifyDBF_TableReLenColumnType fnASIDBF_ModifyDBF_TableReLenColumn = (ASIDBF_ModifyDBF_TableReLenColumnType)GetProcAddress(m_hDLL, "ASIDBF_ModifyDBF_TableReLenColumn");
	if(!fnASIDBF_ModifyDBF_TableReLenColumn)	return -101;

	return fnASIDBF_ModifyDBF_TableReLenColumn(m_lpObject, GetCurTableName(), lpColName, nDefLen);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ModifyDBF_TableInitAutoIncrement(UINT64 nInitValue, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_ModifyDBF_TableInitAutoIncrementType fnASIDBF_ModifyDBF_TableInitAutoIncrement = (ASIDBF_ModifyDBF_TableInitAutoIncrementType)GetProcAddress(m_hDLL, "ASIDBF_ModifyDBF_TableInitAutoIncrement");
	if(!fnASIDBF_ModifyDBF_TableInitAutoIncrement)	return -101;

	return fnASIDBF_ModifyDBF_TableInitAutoIncrement(m_lpObject, GetCurTableName(), nInitValue);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_DropDBF_Table(LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_DropDBF_TableType fnASIDBF_DropDBF_Table = (ASIDBF_DropDBF_TableType)GetProcAddress(m_hDLL, "ASIDBF_DropDBF_Table");
	if(!fnASIDBF_DropDBF_Table)	return -101;

	return fnASIDBF_DropDBF_Table(m_lpObject, lpTableName);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_RenameDBF_Table(LPCTSTR lpTableName, LPCTSTR lpNewTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_RenameDBF_TableType fnASIDBF_RenameDBF_Table = (ASIDBF_RenameDBF_TableType)GetProcAddress(m_hDLL, "ASIDBF_RenameDBF_Table");
	if(!fnASIDBF_RenameDBF_Table)	return -101;

	return fnASIDBF_RenameDBF_Table(m_lpObject, lpTableName, lpNewTableName);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ReloadDBF_Table()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_ReloadDBF_TableType fnASIDBF_ReloadDBF_Table = (ASIDBF_ReloadDBF_TableType)GetProcAddress(m_hDLL, "ASIDBF_ReloadDBF_Table");
	if(!fnASIDBF_ReloadDBF_Table)	return -101;

	return fnASIDBF_ReloadDBF_Table(m_lpObject);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTbSVer(PUINT32 nVer, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_GetTbSVerType fnASIDBF_GetTbSVer = (ASIDBF_GetTbSVerType)GetProcAddress(m_hDLL, "ASIDBF_GetTbSVer");
	if(!fnASIDBF_GetTbSVer)	return -101;

	return fnASIDBF_GetTbSVer(m_lpObject, GetCurTableName(), nVer);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_SetTbSVer(UINT32 nVer, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_SetTbSVerType fnASIDBF_SetTbSVer = (ASIDBF_SetTbSVerType)GetProcAddress(m_hDLL, "ASIDBF_SetTbSVer");
	if(!fnASIDBF_SetTbSVer)	return -101;

	return fnASIDBF_SetTbSVer(m_lpObject, GetCurTableName(), nVer);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTbInfo(PDBF_TABLE_INFO_RTN pDTIR, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101; 

	CheckCurTableName(lpTableName);

	ASIDBF_GetTbInfoType fnASIDBF_GetTbInfo = (ASIDBF_GetTbInfoType)GetProcAddress(m_hDLL, "ASIDBF_GetTbInfo");
	if(!fnASIDBF_GetTbInfo)	return -101;

	return fnASIDBF_GetTbInfo(m_lpObject, GetCurTableName(), pDTIR);
}
//--------------------------------------------------------------------------


INT32	CASIDBFDLLUtil::ASIDBF_GetTbInfoList(PDBF_TABLE_INFO_RTN pTableInfoList, UINT32 nBuffLen, PUINT32 pnTableNum)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTbInfoListType fnASIDBF_GetTbInfoList = (ASIDBF_GetTbInfoListType)GetProcAddress(m_hDLL, "ASIDBF_GetTbInfoList");
	if(!fnASIDBF_GetTbInfoList)	return -101;

	return fnASIDBF_GetTbInfoList(m_lpObject, pTableInfoList, nBuffLen, pnTableNum);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTbColumnList(LPCTSTR lpTableName, PDBF_TABLE_COLUMN_RTN pTableColumnList, UINT32 nBuffLen, PUINT32 pnColumnNum)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTbColumnListType fnASIDBF_GetTbColumnList = (ASIDBF_GetTbColumnListType)GetProcAddress(m_hDLL, "ASIDBF_GetTbColumnList");
	if(!fnASIDBF_GetTbColumnList)	return -101;

	return fnASIDBF_GetTbColumnList(m_lpObject, lpTableName, pTableColumnList, nBuffLen, pnColumnNum);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTbColumn(LPCTSTR lpTableName, LPCTSTR lpTableColName, PDBF_TABLE_COLUMN_RTN pTableColumn)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTbColumnType fnASIDBF_GetTbColumn = (ASIDBF_GetTbColumnType)GetProcAddress(m_hDLL, "ASIDBF_GetTbColumn");
	if(!fnASIDBF_GetTbColumn)	return -101;

	return fnASIDBF_GetTbColumn(m_lpObject, lpTableName, lpTableColName, pTableColumn);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTbColumnIdx(LPCTSTR lpTableName, LPCTSTR lpTableColName, PUINT32 pnTableColIdx)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTbColumnIdxType fnASIDBF_GetTbColumnIdx = (ASIDBF_GetTbColumnIdxType)GetProcAddress(m_hDLL, "ASIDBF_GetTbColumnIdx");
	if(!fnASIDBF_GetTbColumnIdx)	return -101;

	return fnASIDBF_GetTbColumnIdx(m_lpObject, lpTableName, lpTableColName, pnTableColIdx);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTbItemPriKeyFP(LPCTSTR lpTableName, LPCTSTR lpTableColName, UINT64 nPriKey, LPCTSTR lpPriKey, PUINT64 pnFP)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTbItemPriKeyFPType fnASIDBF_GetTbItemPriKeyFP = (ASIDBF_GetTbItemPriKeyFPType)GetProcAddress(m_hDLL, "ASIDBF_GetTbItemPriKeyFP");
	if(!fnASIDBF_GetTbItemPriKeyFP)	return -101;

	return fnASIDBF_GetTbItemPriKeyFP(m_lpObject, lpTableName, lpTableColName, nPriKey, lpPriKey, pnFP);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTbItemPriKeyFPList(LPCTSTR lpTableName, LPCTSTR lpTableColName, UINT64 nChkValueS, UINT64 nChkValueE, LPCTSTR lpChkValueS, LPCTSTR lpChkValueE, UINT32 nCompType, PUINT64* ppnFPList, PUINT32 pnFPListSize)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_GetTbItemPriKeyFPListType fnASIDBF_GetTbItemPriKeyFPList = (ASIDBF_GetTbItemPriKeyFPListType)GetProcAddress(m_hDLL, "ASIDBF_GetTbItemPriKeyFPList");
	if(!fnASIDBF_GetTbItemPriKeyFPList)	return -101;

	return fnASIDBF_GetTbItemPriKeyFPList(m_lpObject, lpTableName, lpTableColName,  nChkValueS, nChkValueE, lpChkValueS, lpChkValueE, nCompType, ppnFPList, pnFPListSize);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_FreeTbItemPriKeyFPList(PUINT64* ppnFPList)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_FreeTbItemPriKeyFPListType fnASIDBF_FreeTbItemPriKeyFPList = (ASIDBF_FreeTbItemPriKeyFPListType)GetProcAddress(m_hDLL, "ASIDBF_FreeTbItemPriKeyFPList");
	if(!fnASIDBF_FreeTbItemPriKeyFPList)	return -101;

	return fnASIDBF_FreeTbItemPriKeyFPList(m_lpObject, ppnFPList);
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

INT32	CASIDBFDLLUtil::ASIDBF_First(LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_FirstType fnASIDBF_First = (ASIDBF_FirstType)GetProcAddress(m_hDLL, "ASIDBF_First");
	if(!fnASIDBF_First)	return -101;

	return fnASIDBF_First(m_lpObject, GetCurTableName());
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_Last(LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_LastType fnASIDBF_Last = (ASIDBF_LastType)GetProcAddress(m_hDLL, "ASIDBF_Last");
	if(!fnASIDBF_Last)	return -101;

	return fnASIDBF_Last(m_lpObject, GetCurTableName());
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_FirstKey(LPCTSTR lpTableName, UINT64 nKey)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_FirstKeyType fnASIDBF_FirstKey = (ASIDBF_FirstKeyType)GetProcAddress(m_hDLL, "ASIDBF_FirstKey");
	if(!fnASIDBF_FirstKey)	return -101;

	return fnASIDBF_FirstKey(m_lpObject, GetCurTableName(), nKey);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_LastKey(LPCTSTR lpTableName, UINT64 nKey)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_LastKeyType fnASIDBF_LastKey = (ASIDBF_LastKeyType)GetProcAddress(m_hDLL, "ASIDBF_LastKey");
	if(!fnASIDBF_LastKey)	return -101;

	return fnASIDBF_LastKey(m_lpObject, GetCurTableName(), nKey);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_Next(PDBF_ITEM_RTN* pDIRArray, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	if(!m_fnASIDBF_Next)	return -101;

	return m_fnASIDBF_Next(m_lpObject, GetCurTableName(), pDIRArray);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_NextStop()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	ASIDBF_NextStopType fnASIDBF_NextStop = (ASIDBF_NextStopType)GetProcAddress(m_hDLL, "ASIDBF_NextStop");
	if(!fnASIDBF_NextStop)	return -101;

	return fnASIDBF_NextStop(m_lpObject);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_SetSelInfo(UINT32 nSelIDNum, PINT32 pSelIDArray, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_SetSelInfoType fnASIDBF_SetSelInfo = (ASIDBF_SetSelInfoType)GetProcAddress(m_hDLL, "ASIDBF_SetSelInfo");
	if(!fnASIDBF_SetSelInfo)	return -101;

	return fnASIDBF_SetSelInfo(m_lpObject, GetCurTableName(), nSelIDNum, pSelIDArray);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_PreSetDBF_TableItem(LPCTSTR lpColName, PVOID lpBuff, UINT32 nBufLen, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_PreSetDBF_TableItem)	return -101;

	CheckCurTableName(lpTableName);

	return m_fnASIDBF_PreSetDBF_TableItem(m_lpObject, GetCurTableName(), lpColName, lpBuff, nBufLen);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ExeSelectByID(UINT32 nID, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	if(!m_fnASIDBF_ExeSelectByID)	return -101;

	return m_fnASIDBF_ExeSelectByID(m_lpObject, GetCurTableName(), nID);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ExeUpdateByID(UINT32 nID, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	if(!m_fnASIDBF_ExeUpdateByID)	return -101;

	return m_fnASIDBF_ExeUpdateByID(m_lpObject, GetCurTableName(), nID);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ExeDeleteByID(UINT32 nID, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	if(!m_fnASIDBF_ExeDeleteByID)	return -101;

	return m_fnASIDBF_ExeDeleteByID(m_lpObject, GetCurTableName(), nID);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ExeInsert(PUINT64 pnNewFP, PUINT64 pnLastID, LPCTSTR lpTableName)
{
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	if(!m_fnASIDBF_ExeInsert)	return -101;

	return m_fnASIDBF_ExeInsert(m_lpObject, GetCurTableName(), pnNewFP, pnLastID);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ExeSelect(UINT64 nFP, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_ExeSelectType fnASIDBF_ExeSelect = (ASIDBF_ExeSelectType)GetProcAddress(m_hDLL, "ASIDBF_ExeSelect");
	if(!fnASIDBF_ExeSelect)	return -101;

	return fnASIDBF_ExeSelect(m_lpObject, GetCurTableName(), nFP);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ExeUpdate(PUINT64 pnFP, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_ExeUpdateType fnASIDBF_ExeUpdate = (ASIDBF_ExeUpdateType)GetProcAddress(m_hDLL, "ASIDBF_ExeUpdate");
	if(!fnASIDBF_ExeUpdate)	return -101;
	return fnASIDBF_ExeUpdate(m_lpObject, GetCurTableName(), pnFP);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_ExeDelete(UINT64 nFP, LPCTSTR lpTableName)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	CheckCurTableName(lpTableName);

	ASIDBF_ExeDeleteType fnASIDBF_ExeDelete = (ASIDBF_ExeDeleteType)GetProcAddress(m_hDLL, "ASIDBF_ExeDelete");
	if(!fnASIDBF_ExeDelete)	return -101;

	return fnASIDBF_ExeDelete(m_lpObject, GetCurTableName(), nFP);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


INT32	CASIDBFDLLUtil::ASIDBF_FastSetCurTable(LPCTSTR szTName)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_FastSetCurTable)	return -101;

	return m_fnASIDBF_FastSetCurTable(m_lpObject, szTName);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_FastPreSetDBF_TableItem(LPCTSTR szColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_FastPreSetDBF_TableItem)	return -101;

	return m_fnASIDBF_FastPreSetDBF_TableItem(m_lpObject, szColName, nMemberType, lpBuff, nBufLen);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_FastExeInsertDBF(PUINT64 pnLastID, PUINT64 pnNewFP)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_FastExeInsertDBF)	return -101;

	return m_fnASIDBF_FastExeInsertDBF(m_lpObject, pnLastID, pnNewFP);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_FastExeUpdateDBF(UINT32 nID)
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
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetFieldByIdx(INT32 nIdx, PDBF_ITEM_RTN pdfir)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_GetFieldByIdx)	return -101;

	return m_fnASIDBF_GetFieldByIdx(m_lpObject, nIdx, pdfir);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetFieldByName(LPCTSTR lpName, PDBF_ITEM_RTN pdfir)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_GetFieldByName)	return -101;

	return m_fnASIDBF_GetFieldByName(m_lpObject, lpName, pdfir);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetFieldFP(PUINT64 pnFP)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_GetFieldFP)	return -101;

	return m_fnASIDBF_GetFieldFP(m_lpObject, pnFP);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTableNextID(LPCTSTR lpTName, PUINT64 pnNextID)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_GetTableNextID)	return -101;

	return m_fnASIDBF_GetTableNextID(m_lpObject, lpTName, pnNextID);
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::ASIDBF_GetTableItemFPByPKey(LPCTSTR lpTName, UINT64 nKey, PUINT64 pnFP)
{
	if(!m_lpObject)	return -101;

	if(!m_fnASIDBF_GetTableItemFPByPKey)	return -101;

	return m_fnASIDBF_GetTableItemFPByPKey(m_lpObject, lpTName, nKey, pnFP);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

PDBF_ITEM_RTN	CASIDBFDLLUtil::GetField(INT32 nIdx)
{
	CASIDBFDLLMember* tMember = GetMember();	
	
	INT32 nRtn = ASIDBF_GetFieldByIdx(nIdx, &(tMember->m_tDFIR));
	return &(tMember->m_tDFIR);
}
//--------------------------------------------------------------------------

PDBF_ITEM_RTN	CASIDBFDLLUtil::GetField(LPCTSTR szName)
{
	CASIDBFDLLMember* tMember = GetMember();	

	INT32 nRtn = ASIDBF_GetFieldByName(szName, &(tMember->m_tDFIR));
	return &(tMember->m_tDFIR);
}
//--------------------------------------------------------------------------

PDBF_TABLE_COLUMN_RTN	CASIDBFDLLUtil::GetTableColumn(LPCTSTR szTCName, LPCTSTR lpTableName)
{
	CASIDBFDLLMember* tMember = GetMember();	

	CheckCurTableName(lpTableName);
	memset(&(tMember->m_tDFTCR), 0, sizeof((tMember->m_tDFTCR)));

	INT32 nRtn = ASIDBF_GetTbColumn(GetCurTableName(), szTCName, &(tMember->m_tDFTCR));
	return &(tMember->m_tDFTCR);
}
//--------------------------------------------------------------------------

UINT64			CASIDBFDLLUtil::GetFieldFP()
{
	CASIDBFDLLMember* tMember = GetMember();	

	INT32 nRtn = ASIDBF_GetFieldFP(&(tMember->m_nCurFP));
	return (tMember->m_nCurFP);
}
//--------------------------------------------------------------------------

void	CASIDBFDLLUtil::CheckCurTableName(LPCTSTR lpTableName)
{
	if(lpTableName)		SetCurTableName(lpTableName);	
	return;
}
//--------------------------------------------------------------------------

void	CASIDBFDLLUtil::SetCurTableName(LPCTSTR lpTableName)
{
	CASIDBFDLLMember* tMember = GetMember();	

	tMember->m_strTbName = lpTableName;
	return;
}
//--------------------------------------------------------------------------

CString	CASIDBFDLLUtil::GetCurTableName()
{
	CASIDBFDLLMember* tMember = GetMember();	
	return tMember->m_strTbName;
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

INT32	CASIDBFDLLUtil::CreateDBObject(LPCTSTR lpDBName)
{
 	if(!m_hDLL)		return -100;

    PVOID lpObject = NULL;

	ASIDBF_CreateType fnASIDBF_Create = (ASIDBF_CreateType)GetProcAddress(m_hDLL, "ASIDBF_Create");
	if(!fnASIDBF_Create)	return -101;

	if(fnASIDBF_Create(&lpObject))	return -102;

    m_tDBObjMap[lpDBName] = (UINT64)lpObject;
    m_lpObject = lpObject;
    return 0;
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::FreeDBObject(LPCTSTR lpDBName)
{
	INT32 nRtn = ASIDBF_Free();
    if(nRtn)	return nRtn;

    m_lpObject = NULL;
    m_tDBObjMap.erase(lpDBName);
    return 0;
}
//--------------------------------------------------------------------------

INT32	CASIDBFDLLUtil::SetCurDBObject(LPCTSTR lpDBName)
{
	TMapStrID64Itor find = m_tDBObjMap.find(lpDBName);
    if(find == m_tDBObjMap.end())	return -1;

    m_lpObject = (PVOID)(find->second);
    return 0;
}
//--------------------------------------------------------------------------

CASIDBFDLLMember*			CASIDBFDLLUtil::GetMember()
{
	CASIDBFDLLMember*	pRtn = NULL;
	m_tMutexMember.Lock();

	UINT64 nThreadID = (UINT64)gettid();
	TMapASIDBFDLLMemberItor find = m_tDbfMemberMap.find(nThreadID);
	if(find == m_tDbfMemberMap.end())
	{
		CASIDBFDLLMember* pMember = new CASIDBFDLLMember();
		m_tDbfMemberMap[nThreadID] = pMember;
		find = m_tDbfMemberMap.find(nThreadID);	
	}

	pRtn = (CASIDBFDLLMember*)(find->second);
	m_tMutexMember.UnLock();
	return pRtn;
}
//---------------------------------------------------------------------

void					CASIDBFDLLUtil::ClearMember()
{
	TMapASIDBFDLLMemberItor begin, end;
	begin = m_tDbfMemberMap.begin();	end = m_tDbfMemberMap.end();
	for(begin; begin != end; begin++)
	{
		CASIDBFDLLMember* pdata = (CASIDBFDLLMember*)(begin->second);
		if(!pdata)	continue;

		memset(&(pdata->m_tDFIR), 0, sizeof(pdata->m_tDFIR));
		delete pdata;
	}
	m_tDbfMemberMap.clear();
	return;
}
//---------------------------------------------------------------------



