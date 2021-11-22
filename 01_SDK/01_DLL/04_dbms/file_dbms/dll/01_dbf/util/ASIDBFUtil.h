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

#if !defined(AFX_ASIDBFUTIL_H__7C6D3681_E856_4689_9CBA_BF541997D904__INCLUDED_)
#define AFX_ASIDBFUTIL_H__7C6D3681_E856_4689_9CBA_BF541997D904__INCLUDED_

#include "ASIDBFMember.h"
#include "SecurityUtil.h"
#include "asi_lnx_define_w32.h"

class CASIDBFUtil  
{

private:
	TMapPVOID		m_tFHMap;	
	CString			m_strDBFName;
	//UINT32		GetER();
	CMutexExt		m_tMutex;

private:
	TListDBFTableSearch		m_tDBFTableSearchList;
	UINT64					m_nDBFLastFP;
	//UINT64					m_nLastInsID;
	
private:
	DBF_DATABASE			m_tDB;
	//PDBF_TABLE_SEARCH		m_tCurSelTable;
	//INT32					m_nCurSelDirection;
	//PBYTE					m_pCurItemBuff;
	//UINT8					m_tCurDBFILArray[ASI_DBF_MAX_COLUMN_NUMBER * 2];	
	//DBF_TABLE_ITEM_HEADER	m_tCurDBFIH;
	//DBF_ITEM_RTN			m_tCurDBFIR;

private:
	//TMapTColumnSetValueList		m_tTColumSetValueListMap;
	//TMapTItemSetValueMap		m_tTItemSetValueMapMap;

private:
	UINT32		m_nChkOpTickCnt;
	UINT32		m_nChkOpTickID;

	UINT32		m_nChkOpTickCntIdx;
	UINT64		m_tChkOpTickCntMap[100];

public:
	INT32		SetFPKeyMap(UINT64 nFP);
	INT32		ClearFPKeyMap();

private:
	INT32		SetER(INT32 nRtn);
	INT32		GetER();

public:
	CMutexExt				m_tMutexMember;
	TMapASIDBFMember		m_tDbfUtilMemberMap;
	CASIDBFMember*			GetMember();
	void					ClearMember();

private:
	HANDLE			GetFileHandle(UINT32 nMode);
	INT32			SetFileHandle(HANDLE hFile, UINT32 nMode);

public:
	INT32		SetASyncMode(UINT32 nMode);

private:
	UINT64		GetFilePointerEx(UINT32 nMode);
	UINT64		SetFilePointerEx(UINT32 nMode, UINT64 nFileFP, INT32 nMoveMethod = FILE_BEGIN);
	UINT64		SetFileEOF(UINT64 nFP);

public:
	INT32		SetOpTickCntLog(UINT32 nChkOpTickCnt);
	INT32		InitOpTickCntLog();
	INT32		ChkOpTickCntLog(UINT32 nKey = 0, UINT32 nValue = 0);
	INT32		WriteLogOpTickCnt(CString strTName);

public:
	INT32		OpenDBF(CString strDBFileName, CString strUserID, CString strUserPw, UINT32 nAccessMode = (GENERIC_READ|GENERIC_WRITE), UINT32 nAutoLogin = 0);
	INT32		CreateDBF(CString strDBFileName, CString strUserID, CString strUserPw);
	INT32		CloseDBF();
	INT32		RemoveDBF(UINT32 nRenameMode = 0);
	INT32		RecoveryDBF(UINT32 nRenameMode = 0);
	INT32		BackupDBF(CString strBkFileName);
	INT32		IsAutoRestore(INT32& nMode);

private:
	INT32		WriteDBF_DI(PDBF_DATABASE pdd);
	INT32		ReadDBF_DI(PDBF_DATABASE pdd);
	INT32		UpdateDBF_DI(PDBF_DATABASE pdd = NULL);

public:
	INT32						ReadDBF_TableList();
	INT32						ReloadDBF_TableList();
	INT32						MakeDBF_TableColIdx(PDBF_TABLE_SEARCH pDTS);
	INT32						MakeDBF_TablePriFPList(PDBF_TABLE_SEARCH pDTS);
	PDBF_TABLE_SEARCH			FirstDBF_Table();
	PDBF_TABLE_SEARCH			LastDBF_Table();
	PDBF_TABLE_SEARCH			FindDBF_Table(CString strTName);
	

	INT32		GetDBF_TableInfo(CString strTName, PDBF_TABLE_INFO_RTN pDTIR);
	INT32		GetDBF_TableInfoList(PDBF_TABLE_INFO_RTN pInfoBuffList, UINT32 nBuffLen, PUINT32 pnTableNum);
	INT32		GetDBF_TableColumnList(CString strTName, PDBF_TABLE_COLUMN_RTN pColBuffList, UINT32 nBuffLen, PUINT32 pnColNum);
	INT32		GetDBF_TableColumn(CString strTName, CString strColName, PDBF_TABLE_COLUMN_RTN pDTCR);
	INT32		GetDBF_TableColumnIdx(CString strTName, CString strColName, PUINT32 pnTableColIdx);
	INT32		GetTbItemPriKeyFP(CString strTName, CString strColName, UINT64 nPriKey, CString strPriKey, PUINT64 pnFP);
	INT32		GetTbItemPriKeyFPList(CString strTName, CString strColName, UINT64 nChkValueS, UINT64 nChkValueE, CString strChkValueS, CString strChkValueE, UINT32 nCompType, PUINT64* ppnFPList, PUINT32 pnFPListSize);

public:	
	INT32		CreateDBF_Table(PDBF_TABLE_SEARCH pdts);
	INT32		UpdateDBF_Table(PDBF_TABLE_SEARCH pdts);
	INT32		DropDBF_Table(CString strTName);
	INT32		RenameDBF_Table(CString strTName, CString strNewTName);

	INT32		PreSetDBF_TableColumn(CString strTName, CString strColName, UINT16 nMemberType, UINT64 nDefaultValue = 0, UINT16 nInitStrLen = 0, UINT32 nColOption = 0);
	INT32		ForceCreateDBF_Table(CString strTName);
	INT32		ModifyDBF_TableAddColumn(CString strTName, CString strColName, UINT16 nMemberType, UINT64 nDefaultValue = 0, UINT16 nInitStrLen = 0, UINT32 nColOption = 0);
	INT32		ModifyDBF_TableDelColumn(CString strTName, CString strColName);	
	INT32		ModifyDBF_TableRenameColumn(CString strTName, CString strColName, CString strNewColName);	
	INT32		ModifyDBF_TableReLenColumn(CString strTName, CString strColName, UINT32 nDefLen);	
	INT32		ModifyDBF_TableInitAutoIncrement(CString strTName, UINT64 nInitValue);

	INT32		FirstDBF_Item(CString strTName);
	INT32		LastDBF_Item(CString strTName);
	INT32		NextDBF_Item(CString strTName, PDBF_ITEM_RTN* pDIRArray = NULL);
	
	INT32		SetDBF_SelInfo(CString strTName = "", UINT32 nSelIDNum = 0, PINT32 pSelIDArray = NULL);
	INT32		StopDBF_Item();

	INT32		FirstDBF_Item(PDBF_TABLE_SEARCH pdts);
	INT32		LastDBF_Item(PDBF_TABLE_SEARCH pdts);
	INT32		NextDBF_Item(PDBF_TABLE_SEARCH pdts = NULL);

	INT32		NextDBF_ItemBuff(PDBF_TABLE_SEARCH pdts, CASIDBFMember* pMember);

public:
	INT32		FirstDBFKey_Item(CString strTName, UINT64 nKey);
	INT32		LastDBFKey_Item(CString strTName, UINT64 nKey);

	INT32		FirstDBFKey_Item(PDBF_TABLE_SEARCH pdts, UINT64 nKey);
	INT32		LastDBFKey_Item(PDBF_TABLE_SEARCH pdts, UINT64 nKey);


public:
	INT32		MakeField(PDBF_TABLE_SEARCH pdts, CASIDBFMember* pMember);

public:	
	INT32		CheckDBF_Table(PDBF_TABLE_SEARCH pdts_new, INT32 nModify = 1);
	INT32		CompareDBF_TableHeader(PDBF_TABLE_SEARCH pdts_src, PDBF_TABLE_SEARCH pdts_tar);
	INT32		CompareDBF_TableHeaderCol(PDBF_TABLE_COLUMN pdfhc_src, PDBF_TABLE_COLUMN pdfhc_tar);

public:
	INT32				GetTbSVer(LPCTSTR szTName, PUINT32 nVer);
	INT32				SetTbSVer(LPCTSTR szTName, UINT32 nVer);

public:
	INT32				IsExistField(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName);
	PDBF_TABLE_COLUMN	FindFieldColum(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName);
	UINT32				GetTableItemHeaderSize(PDBF_TABLE_SEARCH pdts);
	UINT32				SetTableItemHeaderSize(PDBF_TABLE_SEARCH pdts);
	UINT32				SetTableAutoItem(PDBF_TABLE_SEARCH pdts);
	INT32				SetAutoIncrementID(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, PBYTE lpBuff);

public:
	UINT32				GetFieldType(PDBF_TABLE_SEARCH pdts, INT32 nIdx);
	UINT32				GetFieldType(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName);
	LPTSTR				GetFieldName(PDBF_TABLE_SEARCH pdts, INT32 nIdx);
	UINT32				GetFieldLength(PDBF_TABLE_SEARCH pdts, INT32 nIdx);
	UINT32				GetFieldLength(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName);
	UINT32				GetFieldInitLength(PDBF_TABLE_SEARCH pdts, INT32 nIdx);
	PDBF_ITEM_RTN		GetField(INT32 nIdx);
	PDBF_ITEM_RTN		GetField(LPCTSTR lpName);
	PDBF_ITEM_RTN		GetField(PDBF_TABLE_SEARCH pdts, INT32 nIdx);
	PDBF_ITEM_RTN		GetField(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName);
	DBF_TABLE_ITEM		GetFieldDefValue(PDBF_TABLE_SEARCH pdts, INT32 nIdx);
	DBF_TABLE_ITEM		GetFieldDefValue(PDBF_TABLE_SEARCH pdts, LPCTSTR lpName);
	UINT64				GetFieldFP();	
	UINT32				GetTableNextID(LPCTSTR lpTName, PUINT64 pNextID);
	UINT32				GetTableItemFPByPKey(LPCTSTR lpTName, UINT64 nKey, PUINT64 pnFP);
	INT32				CopyField(PDBF_ITEM_RTN pDIR, PBYTE pOrBuffer, INT32 nCpSPos);

public:
	INT32				InsertDBF_TItem(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, PBYTE lpBuff);
	INT32				UpdateDBF_TItem(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, PBYTE lpBuff);
	INT32				DeleteDBF_TItem(PDBF_TABLE_SEARCH pdts, UINT64 nFP);

public:
	INT32						PreSetDBF_TableItem(CString strTName, CString strColName, PVOID lpBuff, UINT32 nBufLen = 0);
	PDBF_TABLE_ITEM_SET_VALUE	FindPreSetTItemValue(CString strTName, CString strColName);

	INT32				ExeSelectDBF(CString strTName, UINT64 nFP);
	INT32				ExeInsertDBF(CString strTName, PUINT64 pnNewFP, PUINT64 pnLastID);
	INT32				ExeUpdateDBF(CString strTName, PUINT64 pnFP);
	INT32				ExeDeleteDBF(CString strTName, UINT64 nFP);

	INT32				ExeSelectDBFByID(CString strTName, UINT32 nID);
	INT32				ExeUpdateDBFByID(CString strTName, UINT32 nID);
	INT32				ExeDeleteDBFByID(CString strTName, UINT32 nID);

private:
	INT32						FastMakeBuffer(PDBF_FAST_BUFF_INFO pDFBI, INT32 nMakeSize = ASI_DBF_FAST_BUFFER_SIZE);
	INT32						FastCopyBuffer(PDBF_FAST_BUFF_INFO pDFBI, PVOID pCpBuff, INT32 nCpBufLen, INT32 nDefLen = 0);
	PDBF_TABLE_ITEM_SET_VALUE	FastFindPreSetTItemValue(CASIDBFMember* pMember, CString strColName);
	INT32						FastSetAutoIncrementID(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, PBYTE lpBuff);
	
private:
	INT32						FastSetFilePointerEx(CASIDBFMember* pMember, UINT32 nMode, UINT64 nFileFP, INT32 nMoveMethod = FILE_BEGIN);
	INT32						FastMakeFileBuff(CASIDBFMember* pMember, UINT32 nMode, UINT64 nFileFP, INT32 nBuffSize = ASI_DBF_FAST_FILE_BUFF_MAX_SIZE, INT32 nMoveMethod = FILE_BEGIN);
	INT32						FastReadDBF(CASIDBFMember* pMember, PVOID lpBuff, UINT32 nSize, UINT32 nMode = ASI_DBF_FILE_HANDLE_READ);
	INT32						FastWriteFileBuff(PVOID lpBuff, INT32 nBuffLen, INT32 nMode = ASI_DBF_FILE_HANDLE_WRITE);

public:
	INT32						FastSetCurTable(CString strTName);
	INT32						FastPreSetDBF_TableItem(CString strColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen = 0);
	INT32						FastExeSelectDBF(CString strTName, UINT32 nID);
	INT32						FastExeInsertDBF(PUINT64 pnLastID, PUINT64 pnNewFP);
	INT32						FastExeUpdateDBF(UINT32 nID);

	INT32						FastNextDBF_ItemBuff(PDBF_TABLE_SEARCH pdts, CASIDBFMember* pMember);

	INT32						FastInsertDBF_TItem(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, DBF_FAST_BUFF_INFO tDFBI);
	INT32						FastUpdateDBF_TItem(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, DBF_FAST_BUFF_INFO tDFBI);


private:
	INT32		ReadDBF(PVOID lpBuff, UINT32 nSize, UINT32 nMode = ASI_DBF_FILE_HANDLE_READ);
	INT32		WriteDBF(PVOID lpBuff, UINT32 nSize);
	INT32		MoveDBF(UINT32 nMode, UINT64 nMoveSize);

public:
	PBYTE		MakeCommonBuff(UINT32 nSize);
	void		FreeCommonBuff(PBYTE* lpBuff);	
	void		SetBuffLength(TMapID64& tHLenMap, UINT32 nLen, INT32 nIdx);
	UINT32		GetBuffLength(TMapID64& tHLenMap, INT32 nIdx);
	UINT64		GetBuffLengthAll(TMapID64& tLenMap, INT32 nIdxNum);
	INT32		CopyHLenBuffToMap(PBYTE lpHLenBuff, TMapID64& tHLenMap, PDBF_TABLE_SEARCH pdts);
	void		CopyHLenMapToBuff(TMapID64& tHLenMap, PBYTE lpHLenBuff, PDBF_TABLE_SEARCH pdts);

private:
	CSecurityUtil	m_tSecurityUtil;
	UINT32			m_nAuthResult;

private:
	UINT32			GetSessionKey();
	INT32			ChgTableItemEnc(PDBF_TABLE_ITEM_HEADER pdtih, PBYTE lpBuff, PBYTE* lpEncBuff);
	INT32			ChgTableItemDec(PDBF_TABLE_ITEM_HEADER pdtih, PBYTE* lpBuff);

private:
	INT32			ApplyKeyToFPInfo(PDBF_TABLE_SEARCH pdts);
	INT32			DeleteKeyToFPInfo();

public:
	UINT32			IsInvalidAuthRight();
	UINT32			CopyAuthInfoDBF(PDBF_DATABASE_AUTH pDDA);


	UINT32			LoginDBF(CString strID, CString strPw);
	UINT32			ChangeAuthInfoDBF(CString strOldID, CString strOldPw, CString strNewID, CString strNewPw, UINT32 nChkNewType);
	UINT32			SetTItemEncTypeDBF(UINT32 nDefEncType);
	UINT32			SetTItemCompTypeDBF(UINT32 nDefCompType);
	UINT32			GetTItemEncTypeDBF(PUINT32 pnDefEncType);
	UINT32			GetTItemCompTypeDBF(PUINT32 pnDefCompType);

public:
	CASIDBFUtil();
	virtual ~CASIDBFUtil();

};

#endif // !defined(AFX_ASIDBFUTIL_H__7C6D3681_E856_4689_9CBA_BF541997D904__INCLUDED_)
