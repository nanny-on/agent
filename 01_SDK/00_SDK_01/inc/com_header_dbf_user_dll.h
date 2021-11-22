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

#ifndef COM_HEADER_DBF_USER_DLL_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define COM_HEADER_DBF_USER_DLL_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

#include "com_define_dbf_error.h"
#include "com_define_dbf_header.h"


typedef	INT32 (*ASIDBF_InitType)(PASIDBF_INIT_INFO pscdbf_init_info);
typedef	INT32 (*ASIDBF_CreateType)(PVOID* lpObject);
typedef	INT32 (*ASIDBF_FreeType)(PVOID* lpObject);

typedef	INT32 (*ASIDBF_CreateDBType)(PVOID lpObject, LPCTSTR lpFileName, LPCTSTR lpUserID, LPCTSTR lpUserPw);
typedef	INT32 (*ASIDBF_OpenDBType)(PVOID lpObject, LPCTSTR lpFileName, LPCTSTR lpUserID, LPCTSTR lpUserPw, UINT32 nAccessMode);
typedef	INT32 (*ASIDBF_CloseDBType)(PVOID lpObject);
typedef	INT32 (*ASIDBF_RemoveDBType)(PVOID lpObject);
typedef	INT32 (*ASIDBF_RecoveryDBType)(PVOID lpObject, UINT32 nRemainMode);
typedef	INT32 (*ASIDBF_BackupDBType)(PVOID lpObject, LPCTSTR lpBkFileName);
typedef	INT32 (*ASIDBF_IsAutoRestoreType)(PVOID lpObject, INT32& nMode);

typedef	INT32 (*ASIDBF_ChangeAuthInfoDBType)(PVOID lpObject, LPCTSTR lpCurUserID, LPCTSTR lpCurUserPw, LPCTSTR lpNewUserID, LPCTSTR lpNewUserPw, UINT32 nChkNewType);
typedef	INT32 (*ASIDBF_SetTItemEncTypeDBType)(PVOID lpObject, UINT32 nDefEncType);
typedef	INT32 (*ASIDBF_SetTItemCompTypeDBType)(PVOID lpObject, UINT32 nDefCompType);
typedef	INT32 (*ASIDBF_GetTItemEncTypeDBType)(PVOID lpObject, PUINT32 pnDefEncType);
typedef	INT32 (*ASIDBF_GetTItemCompTypeDBType)(PVOID lpObject, PUINT32 pnDefCompType);


typedef	INT32 (*ASIDBF_PreSetDBF_TableColumnType)(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption);
typedef	INT32 (*ASIDBF_ForceCreateDBF_TableType)(PVOID lpObject, LPCTSTR lpTableName);
typedef INT32 (*ASIDBF_ModifyDBF_TableAddColumnType)(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption);
typedef INT32 (*ASIDBF_ModifyDBF_TableDelColumnType)(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName);
typedef INT32 (*ASIDBF_ModifyDBF_TableRenameColumnType)(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, LPCTSTR lpNewColName);
typedef INT32 (*ASIDBF_ModifyDBF_TableReLenColumnType)(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, UINT32 nDefLen);
typedef INT32 (*ASIDBF_DropDBF_TableType)(PVOID lpObject, LPCTSTR lpTableName);
typedef INT32 (*ASIDBF_RenameDBF_TableType)(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpNewTableName);
typedef INT32 (*ASIDBF_ReloadDBF_TableType)(PVOID lpObject);

typedef	INT32 (*ASIDBF_GetTbSVerType)(PVOID lpObject, LPCTSTR lpTableName, PUINT32 nVer);
typedef	INT32 (*ASIDBF_SetTbSVerType)(PVOID lpObject, LPCTSTR lpTableName, UINT32 nVer);
typedef	INT32 (*ASIDBF_GetTbInfoType)(PVOID lpObject, LPCTSTR lpTableName, PDBF_TABLE_INFO_RTN pDTIR);
typedef	INT32 (*ASIDBF_GetTbInfoListType)(PVOID lpObject, PDBF_TABLE_INFO_RTN pTableInfoList, UINT32 nBuffLen, PUINT32 pnTableNum);
typedef	INT32 (*ASIDBF_GetTbColumnListType)(PVOID lpObject, LPCTSTR lpTableName, PDBF_TABLE_COLUMN_RTN pTableColumnList, UINT32 nBuffLen, PUINT32 pnColumnNum);
typedef	INT32 (*ASIDBF_GetTbColumnType)(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpTableColName, PDBF_TABLE_COLUMN_RTN pTableColumn);
typedef INT32 (*ASIDBF_GetTbColumnIdxType)(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpTableColName, PUINT32 pnTableColIdx);

typedef	INT32 (*ASIDBF_FirstType)(PVOID lpObject, LPCTSTR lpTableName);
typedef	INT32 (*ASIDBF_LastType)(PVOID lpObject, LPCTSTR lpTableName);
typedef	INT32 (*ASIDBF_FirstKeyType)(PVOID lpObject, LPCTSTR lpTableName, UINT64 nKey);
typedef	INT32 (*ASIDBF_LastKeyType)(PVOID lpObject, LPCTSTR lpTableName, UINT64 nKey);
typedef	INT32 (*ASIDBF_NextType)(PVOID lpObject, LPCTSTR lpTableName, PDBF_ITEM_RTN* pDIRArray);
typedef	INT32 (*ASIDBF_NextStopType)(PVOID lpObject);
typedef INT32 (*ASIDBF_SetSelInfoType)(PVOID lpObject, LPCTSTR szTName, UINT32 nSelIDNum, PINT32 pSelIDArray);

typedef	INT32 (*ASIDBF_PreSetDBF_TableItemType)(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, PVOID lpBuff, UINT32 nBufLen);
typedef	INT32 (*ASIDBF_ExeSelectType)(PVOID lpObject, LPCTSTR lpTableName, UINT64 nFP);
typedef	INT32 (*ASIDBF_ExeInsertType)(PVOID lpObject, LPCTSTR lpTableName, PUINT64 pnNewFP, PUINT64 pnLastID);
typedef	INT32 (*ASIDBF_ExeUpdateType)(PVOID lpObject, LPCTSTR lpTableName, PUINT64 pnFP);
typedef	INT32 (*ASIDBF_ExeDeleteType)(PVOID lpObject, LPCTSTR lpTableName, UINT64 nFP);

typedef	INT32 (*ASIDBF_ExeSelectByIDType)(PVOID lpObject, LPCTSTR lpTableName, UINT32 nID);
typedef	INT32 (*ASIDBF_ExeUpdateByIDType)(PVOID lpObject, LPCTSTR lpTableName, UINT32 nID);
typedef	INT32 (*ASIDBF_ExeDeleteByIDType)(PVOID lpObject, LPCTSTR lpTableName, UINT32 nID);

typedef	INT32 (*ASIDBF_FastSetCurTableType)(PVOID lpObject, LPCTSTR szTName);
typedef	INT32 (*ASIDBF_FastPreSetDBF_TableItemType)(PVOID lpObject, LPCTSTR szColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen);
typedef	INT32 (*ASIDBF_FastExeInsertDBFType)(PVOID lpObject, PUINT64 pnLastID, PUINT64 pnNewFP);
typedef	INT32 (*ASIDBF_FastExeUpdateDBFType)(PVOID lpObject, UINT32 nID);

typedef	INT32 (*ASIDBF_GetFieldByIdxType)(PVOID lpObject, INT32 nIdx, PDBF_ITEM_RTN pdfir);
typedef	INT32 (*ASIDBF_GetFieldByNameType)(PVOID lpObject, LPCTSTR lpName, PDBF_ITEM_RTN pdfir);
typedef	INT32 (*ASIDBF_GetFieldFPType)(PVOID lpObject, PUINT64 pnFP);
typedef	INT32 (*ASIDBF_GetTableNextIDType)(PVOID lpObject, LPCTSTR lpTName, PUINT64 pnNextID);
typedef	INT32 (*ASIDBF_GetTableItemFPByPKeyType)(PVOID lpObject, LPCTSTR lpTName, UINT64 nKey, PUINT64 pnFP);
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

typedef	INT32 (*ASIDBFQ_InitType)(PVOID lpObject, PASIDBFQ_INIT_INFO psqii);
typedef	INT32 (*ASIDBFQ_CreateType)(PVOID* lpObject);
typedef	INT32 (*ASIDBFQ_FreeType)(PVOID* lpObject);

typedef	INT32 (*ASIDBFQ_CreateDatabaseType)(PVOID lpObject, LPCTSTR szDBName, LPCTSTR lpUserID, LPCTSTR lpUserPw);
typedef	INT32 (*ASIDBFQ_OpenDatabaseType)(PVOID lpObject, LPCTSTR szDBName, LPCTSTR lpUserID, LPCTSTR lpUserPw, UINT32 nAccessMode);
typedef	INT32 (*ASIDBFQ_CloseDatabaseType)(PVOID lpObject, LPCTSTR szDBName);

typedef	INT32 (*ASIDBFQ_IsExistTableType)(PVOID lpObject, LPCTSTR szTBName);			// 0 : Exist, other value : Not Exist
typedef	INT32 (*ASIDBFQ_GetTableVerType)(PVOID lpObject, LPCTSTR szTBName, PUINT32 nVer);
typedef INT32 (*ASIDBFQ_SetTableVerType)(PVOID lpObject, LPCTSTR szTBName, UINT32 nVer);

typedef	INT32 (*ASIDBFQ_QuerySelectType)(PVOID lpObject, LPCTSTR szQuery);
typedef	INT32 (*ASIDBFQ_QueryExecuteType)(PVOID lpObject, LPCTSTR szQuery);
typedef	INT32 (*ASIDBFQ_QuerySelectResultType)(PVOID lpObject, LPCTSTR* szTableName);
typedef	INT32 (*ASIDBFQ_QueryExecuteResultType)(PVOID lpObject, PUINT32 pnCmdType, PUINT32 pnCmdCode, PUINT32 pnRtnCnt);

typedef	INT32 (*ASIDBFQ_FirstType)(PVOID lpObject);
typedef	INT32 (*ASIDBFQ_LastType)(PVOID lpObject);
typedef	INT32 (*ASIDBFQ_FirstKeyType)(PVOID lpObject, UINT64 nKey);
typedef	INT32 (*ASIDBFQ_LastKeyType)(PVOID lpObject, UINT64 nKey);
typedef	INT32 (*ASIDBFQ_NextType)(PVOID lpObject);
typedef	INT32 (*ASIDBFQ_NextStopType)(PVOID lpObject);

typedef	INT32 (*ASIDBFQ_GetFieldByIdxType)(PVOID lpObject, INT32 nIdx, PDBF_ITEM_RTN pdfir);
typedef	INT32 (*ASIDBFQ_GetFieldByNameType)(PVOID lpObject, LPCTSTR lpName, PDBF_ITEM_RTN pdfir);
typedef	INT32 (*ASIDBFQ_GetFieldFPType)(PVOID lpObject, PUINT64 pnFP);

typedef	INT32 (*ASIDBFQ_GetLastIDType)(PVOID lpObject, PUINT64 pnLastID);
typedef	INT32 (*ASIDBFQ_GetLastFpType)(PVOID lpObject, PUINT64 pnLastFp);
typedef	INT32 (*ASIDBFQ_GetTbLastIDType)(PVOID lpObject, LPCTSTR lpTableName, PUINT64 pnLastID);
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#pragma pack()

#endif //COM_HEADER_DBF_USER_DLL_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



