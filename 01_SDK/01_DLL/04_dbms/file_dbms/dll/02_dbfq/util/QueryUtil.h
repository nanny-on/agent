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

#pragma once

#include "TokenQuery.h"

class CQueryUtil
{
private:
	CString		m_strQuery;
	
public:
	UINT32		m_nQueryCmdType;
	UINT32		m_nQueryCmdCode;
	UINT32		m_nQueryRtnCnt;
	CString		m_strTName;
	CString		m_strNewTName;

	TMapIDCStr			m_tSelValMap;	
	INT32				m_tSelValIDArray[ASI_DBF_MAX_COLUMN_NUMBER];
	TListDBFQCDTInfo	m_tCreateItemList;
	TListDBFQCDTInfo	m_tInsertItemList;
	TListDBFQCDTInfo	m_tUpdateItemList;
	TListDBFQCDTInfo	m_tAlterItemList;

	TListID64				m_tCdtWhereFpList;
	TListDBFQCDTInfoWhere	m_tCdtWhereList;
	UINT32					m_tCdtWhereFpUsedMode;

	INT32		m_nSelectItemNum;
	INT32		m_nSelectWhExist;

	CString		m_strSelDirectionKey;
	INT32		m_nSelDirection;


private:
	INT32		AnalysisCommand(CString strCmd);

	INT32		AnalysisCmd_Select(CString strCmd);
	INT32		AnalysisCmd_Insert(CString strCmd);
	INT32		AnalysisCmd_Update(CString strCmd);
	INT32		AnalysisCmd_Delete(CString strCmd);
	INT32		AnalysisCmd_Drop(CString strCmd);
	INT32		AnalysisCmd_Create(CString strCmd);
	INT32		AnalysisCmd_Alter(CString strCmd);
	INT32		AnalysisCmd_Rename(CString strCmd);

	INT32		AnalysisCmd_WhereClause(CString strCmd);

private:
	INT32		AnalysisCmd_CreateItem(CString strCmdItem, DBFQ_CDT_INFO& tDCI);	

private:
	INT32		ExportCmd_SelNameTableWhere(CString strCmd, CString& strName, CString& strTable, CString& strWhere);
	INT32		ExportCmd_InsNameValue(CString strCmd, CString& strName, CString& strValue);
	INT32		ExportCmd_WhereClause(CString strCmd, CString& strRemain, CString& strWhereClause);
	INT32		ExportCmd_WhereBetAndStrValue(CString strCmd, CString& strSValue, CString& strEValue);
	INT32		ExportCmd_WhereBetAndNumValue(CString strCmd, UINT64& nSValue, UINT64& nEValue);

private:
	UINT64		ConvertOnlyNumberFromStr(CString strValue);
	CString		GetOnlyNameStr(CString strValue);
	CString		RemoveFirstLastChar(CString strValue, CHAR szFRmChar, CHAR szLRmChar);
	CString		RemoveLastChar(CString strValue, CHAR szLRmChar);
	CString		ReverseString(LPCTSTR lpInput, INT32 nLen);

public:
	CString		ConvertSelectIdxToStr(INT32 nIdx);
	INT32		ConvertSelectIdxToIdx(INT32 nIdx);

public:
	INT32		AnalysisQuery(CString strQuery);

public:
	CQueryUtil(void);
	~CQueryUtil(void);
};
