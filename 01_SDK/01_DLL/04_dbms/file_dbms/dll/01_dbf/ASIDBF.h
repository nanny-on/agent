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

extern "C"	INT32 ASIDBF_Init(PASIDBF_INIT_INFO pscdbf_init_info);
extern "C"	INT32 ASIDBF_Create(PVOID* lpObject);
extern "C"	INT32 ASIDBF_Free(PVOID* lpObject);

extern "C"	INT32 ASIDBF_SetOpTickCntLog(PVOID lpObject, UINT32 nChkOpTickCnt);
extern "C"	INT32 ASIDBF_SetASyncMode(PVOID lpObject, UINT32 nMode);

extern "C"	INT32 ASIDBF_CreateDB(PVOID lpObject, LPCTSTR lpFileName, LPCTSTR lpUserID, LPCTSTR lpUserPw);
extern "C"	INT32 ASIDBF_OpenDB(PVOID lpObject, LPCTSTR lpFileName, LPCTSTR lpUserID, LPCTSTR lpUserPw, UINT32 nAccessMode);
extern "C"	INT32 ASIDBF_CloseDB(PVOID lpObject);
extern "C"	INT32 ASIDBF_RemoveDB(PVOID lpObject);
extern "C"	INT32 ASIDBF_RecoveryDB(PVOID lpObject, UINT32 nRemainMode);
extern "C"	INT32 ASIDBF_BackupDB(PVOID lpObject, LPCTSTR lpBkFileName);
extern "C"	INT32 ASIDBF_IsAutoRestore(PVOID lpObject, INT32& nMode);

extern "C"	INT32 ASIDBF_ChangeAuthInfoDB(PVOID lpObject, LPCTSTR lpCurUserID, LPCTSTR lpCurUserPw, LPCTSTR lpNewUserID, LPCTSTR lpNewUserPw, UINT32 nChkNewType);
extern "C"	INT32 ASIDBF_SetTItemEncTypeDB(PVOID lpObject, UINT32 nDefEncType);
extern "C"	INT32 ASIDBF_SetTItemCompTypeDB(PVOID lpObject, UINT32 nDefCompType);
extern "C"	INT32 ASIDBF_GetTItemEncTypeDB(PVOID lpObject, PUINT32 pnDefEncType);
extern "C"	INT32 ASIDBF_GetTItemCompTypeDB(PVOID lpObject, PUINT32 pnDefCompType);

extern "C"	INT32 ASIDBF_PreSetDBF_TableColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption);
extern "C"	INT32 ASIDBF_ForceCreateDBF_Table(PVOID lpObject, LPCTSTR lpTableName);
extern "C"	INT32 ASIDBF_ModifyDBF_TableAddColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption);
extern "C"	INT32 ASIDBF_ModifyDBF_TableDelColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName);
extern "C"	INT32 ASIDBF_ModifyDBF_TableRenameColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, LPCTSTR lpNewColName);
extern "C"	INT32 ASIDBF_ModifyDBF_TableInitAutoIncrement(PVOID lpObject, LPCTSTR lpTableName, UINT64 nInitValue);
extern "C"	INT32 ASIDBF_ModifyDBF_TableReLenColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, UINT32 nDefLen);
extern "C"	INT32 ASIDBF_DropDBF_Table(PVOID lpObject, LPCTSTR lpTableName);
extern "C"	INT32 ASIDBF_RenameDBF_Table(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpNewTableName);
extern "C"	INT32 ASIDBF_ReloadDBF_Table(PVOID lpObject);

extern "C"	INT32 ASIDBF_GetTbSVer(PVOID lpObject, LPCTSTR lpTableName, PUINT32 nVer);
extern "C"	INT32 ASIDBF_SetTbSVer(PVOID lpObject, LPCTSTR lpTableName, UINT32 nVer);

extern "C"	INT32 ASIDBF_GetTbInfo(PVOID lpObject, LPCTSTR lpTableName, PDBF_TABLE_INFO_RTN pDTIR);
extern "C"	INT32 ASIDBF_GetTbInfoList(PVOID lpObject, PDBF_TABLE_INFO_RTN pTableInfoList, UINT32 nBuffLen, PUINT32 pnTableNum);
extern "C"	INT32 ASIDBF_GetTbColumnList(PVOID lpObject, LPCTSTR lpTableName, PDBF_TABLE_COLUMN_RTN pTableColumnList, UINT32 nBuffLen, PUINT32 pnColumnNum);
extern "C"	INT32 ASIDBF_GetTbColumn(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpTableColName, PDBF_TABLE_COLUMN_RTN pTableColumn);
extern "C"	INT32 ASIDBF_GetTbColumnIdx(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpTableColName, PUINT32 pnTableColIdx);
extern "C"	INT32 ASIDBF_GetTbItemPriKeyFP(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpTableColName, UINT64 nPriKey, LPCTSTR lpPriKey, PUINT64 pnFP);
extern "C"	INT32 ASIDBF_GetTbItemPriKeyFPList(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpTableColName, UINT64 nChkValueS, UINT64 nChkValueE, LPCTSTR lpChkValueS, LPCTSTR lpChkValueE, UINT32 nCompType, PUINT64* ppnFPList, PUINT32 pnFPListSize);
extern "C"	INT32 ASIDBF_FreeTbItemPriKeyFPList(PVOID lpObject, PUINT64* ppnFPList);

extern "C"	INT32 ASIDBF_First(PVOID lpObject, LPCTSTR lpTableName);
extern "C"	INT32 ASIDBF_Last(PVOID lpObject, LPCTSTR lpTableName);
extern "C"	INT32 ASIDBF_FirstKey(PVOID lpObject, LPCTSTR lpTableName, UINT64 nKey);
extern "C"	INT32 ASIDBF_LastKey(PVOID lpObject, LPCTSTR lpTableName, UINT64 nKey);
extern "C"	INT32 ASIDBF_Next(PVOID lpObject, LPCTSTR lpTableName, PDBF_ITEM_RTN* pDIRArray);
extern "C"	INT32 ASIDBF_NextStop(PVOID lpObject);
extern "C"	INT32 ASIDBF_SetSelInfo(PVOID lpObject, LPCTSTR szTName, UINT32 nSelIDNum, PINT32 pSelIDArray);

extern "C"	INT32 ASIDBF_PreSetDBF_TableItem(PVOID lpObject, LPCTSTR lpTableName, LPCTSTR lpColName, PVOID lpBuff, UINT32 nBufLen);
extern "C"	INT32 ASIDBF_ExeSelect(PVOID lpObject, LPCTSTR lpTableName, UINT64 nFP);
extern "C"	INT32 ASIDBF_ExeInsert(PVOID lpObject, LPCTSTR lpTableName, PUINT64 pnNewFP, PUINT64 pnLastID);
extern "C"	INT32 ASIDBF_ExeUpdate(PVOID lpObject, LPCTSTR lpTableName, PUINT64 pnFP);
extern "C"	INT32 ASIDBF_ExeDelete(PVOID lpObject, LPCTSTR lpTableName, UINT64 nFP);

extern "C"	INT32 ASIDBF_ExeSelectByID(PVOID lpObject, LPCTSTR lpTableName, UINT32 nID);
extern "C"	INT32 ASIDBF_ExeUpdateByID(PVOID lpObject, LPCTSTR lpTableName, UINT32 nID);
extern "C"	INT32 ASIDBF_ExeDeleteByID(PVOID lpObject, LPCTSTR lpTableName, UINT32 nID);

extern "C"	INT32 ASIDBF_FastSetCurTable(PVOID lpObject, LPCTSTR szTName);
extern "C"	INT32 ASIDBF_FastPreSetDBF_TableItem(PVOID lpObject, LPCTSTR szColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen);
extern "C"	INT32 ASIDBF_FastExeInsertDBF(PVOID lpObject, PUINT64 pnLastID, PUINT64 pnNewFP);
extern "C"	INT32 ASIDBF_FastExeUpdateDBF(PVOID lpObject, UINT32 nID);

extern "C"	INT32 ASIDBF_GetFieldByIdx(PVOID lpObject, INT32 nIdx, PDBF_ITEM_RTN pdfir);
extern "C"	INT32 ASIDBF_GetFieldByName(PVOID lpObject, LPCTSTR lpName, PDBF_ITEM_RTN pdfir);
extern "C"	INT32 ASIDBF_GetFieldFP(PVOID lpObject, PUINT64 pnFP);
extern "C"	INT32 ASIDBF_GetTableNextID(PVOID lpObject, LPCTSTR lpTName, PUINT64 pnID);
extern "C"	INT32 ASIDBF_GetTableItemFPByPKey(PVOID lpObject, LPCTSTR lpTName, UINT64 nKey, PUINT64 pnFP);



