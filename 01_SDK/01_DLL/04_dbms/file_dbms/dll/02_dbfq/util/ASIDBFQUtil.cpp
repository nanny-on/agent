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
#include "ASIDBFQUtil.h"

CASIDBFQUtil::CASIDBFQUtil(void)
{
}
//------------------------------------------------------------------

CASIDBFQUtil::~CASIDBFQUtil(void)
{
	m_tASIDBFDLLUtil.FreeLibraryExt();
	ClearMember();
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::Init(PASIDBFQ_INIT_INFO psqii)
{
	CHAR szDLLFilePath[TCHAR_MAX_SIZE] = {0, };
	SetLogFileInfo(psqii->szLogFilePath, psqii->szLogFileName, psqii->nRemainLog);
	snprintf(szDLLFilePath, TCHAR_MAX_SIZE-1, "%s/asi_dbf.so", psqii->szASIDBFDLLPath);

	if(SetER(m_tASIDBFDLLUtil.LoadLibraryExt(szDLLFilePath)))
	{
		WriteLogE("load library fail : [%d][%s]", g_nErrRtn, szDLLFilePath);
		return ASI_DBFQ_ERROR_TYPE_NOT_FIND_DEFAULT_LIBRARY;
	}

	m_tSQII = *psqii;

	return 0;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::CreateDatabase(CString strDBName, CString strUserID, CString strUserPw)
{
	if(SetER(m_tASIDBFDLLUtil.SetCurDBObject(strDBName)) == 0)
	{
		m_tASIDBFDLLUtil.ASIDBF_CloseDB();
		m_tASIDBFDLLUtil.FreeDBObject(strDBName);
	}
	
	{		
		ASIDBF_INIT_INFO tSII;
		strncpy(tSII.szLogFilePath, m_tSQII.szLogFilePath, TCHAR_MAX_SIZE-1);
		tSII.szLogFilePath[TCHAR_MAX_SIZE-1] = 0;
		snprintf(tSII.szLogFileName, TCHAR_MAX_SIZE-1, "/nanny_dbf_%s_engine_sys_", (char *)(LPCSTR)strDBName);
		tSII.szLogFileName[TCHAR_MAX_SIZE-1] = 0;
		m_tASIDBFDLLUtil.ASIDBF_Init(&tSII);
	}	
	
	m_tASIDBFDLLUtil.CreateDBObject(strDBName);	

	CString strDBFilePath;
	if(m_tSQII.szASIDBFDataPath[0])
		strDBFilePath.Format("%s/%s.dbf", m_tSQII.szASIDBFDataPath, (char *)(LPCSTR)strDBName);
	else
		strDBFilePath = strDBName;

	if(SetER(m_tASIDBFDLLUtil.ASIDBF_CreateDB(strDBFilePath, strUserID, strUserPw)))
	{
		WriteLogE("create database fail : [%d][%s]", g_nErrRtn, (char *)(LPCTSTR)strDBFilePath);
		m_tASIDBFDLLUtil.FreeDBObject(strDBName);
		return ASI_DBFQ_ERROR_TYPE_CREATE_DBF_FAIL;
	}

	if(SetER(m_tASIDBFDLLUtil.ASIDBF_CloseDB()))
	{
		WriteLogE("close database fail : [ge:%d]", g_nErrRtn);
		return ASI_DBFQ_ERROR_TYPE_SYSTEM_ERROR;
	}

	if(SetER(m_tASIDBFDLLUtil.FreeDBObject(strDBName)))
	{
		WriteLogE("free database fail : [ge:%d]", g_nErrRtn);
		return ASI_DBFQ_ERROR_TYPE_SYSTEM_ERROR;
	}
	return 0;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::OpenDatabase(CString strDBName, CString strUserID, CString strUserPw, UINT32 nAccessMode)
{	
	if(SetER(m_tASIDBFDLLUtil.SetCurDBObject(strDBName)) == 0)
	{
		m_tASIDBFDLLUtil.ASIDBF_CloseDB();
		m_tASIDBFDLLUtil.FreeDBObject(strDBName);
	}

	{		
		ASIDBF_INIT_INFO tSII;
		strncpy(tSII.szLogFilePath, m_tSQII.szLogFilePath, TCHAR_MAX_SIZE-1);
		tSII.szLogFilePath[TCHAR_MAX_SIZE-1] = 0;

		if(m_tSQII.szASIDBFDataPath[0])
			snprintf(tSII.szLogFileName, TCHAR_MAX_SIZE-1, "/nanny_dbf_%s_engine_sys_", (char *)(LPCSTR)strDBName);
		else
		{
			CTokenCString TokenName(strDBName, strDBName.GetLength(), '/', 1);
			CString strOnlyFName = TokenName.NextToken();

			CTokenCString TokenFile(strOnlyFName, strOnlyFName.GetLength(), '.');
			CString strFileName = TokenFile.NextToken();

			snprintf(tSII.szLogFileName, TCHAR_MAX_SIZE-1, "/nanny_dbf_%s_engine_sys_", (char *)(LPCSTR)strFileName);
		}
		m_tASIDBFDLLUtil.ASIDBF_Init(&tSII);
	}

	m_tASIDBFDLLUtil.CreateDBObject(strDBName);	

	CString strDBFilePath;
	if(m_tSQII.szASIDBFDataPath[0])
		strDBFilePath.Format("%s/%s.dbf", m_tSQII.szASIDBFDataPath, (char *)(LPCSTR)strDBName);
	else
		strDBFilePath = strDBName;

	m_tASIDBFDLLUtil.ASIDBF_SetASyncMode(m_tSQII.nASyncMode);
	SetER(m_tASIDBFDLLUtil.ASIDBF_OpenDB(strDBFilePath, strUserID, strUserPw, nAccessMode));	
	if(g_nErrRtn)
	{		
		WriteLogE("open database fail : [%d][%s][tc:%d]", g_nErrRtn, (char *)(LPCTSTR)strDBFilePath, m_tSQII.nOpenWaitCnt);
		INT32 nLoofCnt = m_tSQII.nOpenWaitCnt;
		
		while(g_nErrRtn == ASI_DBF_ERROR_TYPE_OPEN_DB_FAIL_USED_AN_PROC && nLoofCnt)
		{
			Sleep(1000);
			nLoofCnt--;

			SetER(m_tASIDBFDLLUtil.ASIDBF_OpenDB(strDBFilePath, strUserID, strUserPw, nAccessMode));
		}

		if(g_nErrRtn)
		{
			if(g_nErrRtn == ASI_DBF_ERROR_TYPE_OPEN_DB_FAIL_USED_AN_PROC)	
				return ASI_DBFQ_ERROR_TYPE_OPEN_DBF_FAIL_USED_AN_PROC;

			return ASI_DBFQ_ERROR_TYPE_OPEN_DBF_FAIL;
		}
	}

	m_tASIDBFDLLUtil.ASIDBF_SetOpTickCntLog(m_tSQII.nRemainQOpTime);
	WriteLogN("open dbf success. [%s]", (char *)(LPCSTR)strDBName);
	return 0;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::CloseDatabase(CString strDBName)
{
	if(SetER(m_tASIDBFDLLUtil.SetCurDBObject(strDBName)))
	{
		WriteLogE("not find database [%s]", (char *)(LPCTSTR)strDBName);
		return ASI_DBFQ_ERROR_TYPE_NOT_FIND_DATABASE;
	}

	if(SetER(m_tASIDBFDLLUtil.ASIDBF_CloseDB()))
	{
		WriteLogE("close database fail : [ge:%d]", g_nErrRtn);
		return ASI_DBFQ_ERROR_TYPE_SYSTEM_ERROR;
	}

	if(SetER(m_tASIDBFDLLUtil.FreeDBObject(strDBName)))
	{
		WriteLogE("free database fail : [ge:%d]", g_nErrRtn);
		return ASI_DBFQ_ERROR_TYPE_SYSTEM_ERROR;
	}
	return 0;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::SetCurDatabase(CString strDBName)
{
	if(SetER(m_tASIDBFDLLUtil.SetCurDBObject(strDBName)))
	{
		WriteLogE("not find database [%s]", (char *)(LPCSTR)strDBName);
		return ASI_DBFQ_ERROR_TYPE_NOT_FIND_DATABASE;
	}
	return 0;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::QuerySelectResult(LPCTSTR* szTableName)
{
	CASIDBFQMember* pMember = GetMember();

	if(szTableName)	*szTableName = pMember->m_tQueryUtil.m_strTName;
	return 0;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::QuerySelect(CString strQuery)
{
	if(strQuery.IsEmpty())
	{
		return ASI_DBFQ_ERROR_TYPE_INVALID_QUERY;
	}

	LPCTSTR lpQuery = strQuery;
	INT32 nIdx = 0;
	CString strLine = "";
	while(nIdx < strQuery.GetLength())
	{
		CHAR chChk = lpQuery[nIdx++];

		if(chChk == ';')
		{
			break;
		}
		else
		{
			strLine += chChk;
		}
	}

	if(strLine.IsEmpty())
	{
		return ASI_DBFQ_ERROR_TYPE_INVALID_QUERY;
	}

	CASIDBFQMember* pMember = GetMember();

	if(SetER(pMember->m_tQueryUtil.AnalysisQuery(strLine)))
	{
		return ASI_DBFQ_ERROR_TYPE_INVALID_QUERY;
	}

	if(IsExistTable(pMember->m_tQueryUtil.m_strTName))
	{
		return ASI_DBFQ_ERROR_TYPE_INVALID_QUERY;	
	}

	m_tASIDBFDLLUtil.SetCurTableName(pMember->m_tQueryUtil.m_strTName);

	if(pMember->m_tQueryUtil.m_nSelectItemNum)
	{
		TMapIDCStrItor begin, end;
		begin = pMember->m_tQueryUtil.m_tSelValMap.begin();	end = pMember->m_tQueryUtil.m_tSelValMap.end();
		for(begin; begin != end; begin++)
		{
			PDBF_TABLE_COLUMN_RTN pDTCR = m_tASIDBFDLLUtil.GetTableColumn(begin->second); 
			if(pDTCR == NULL)
			{
				return ASI_DBFQ_ERROR_TYPE_INVALID_QUERY;
			}
			pMember->m_tQueryUtil.m_tSelValIDArray[begin->first] = pDTCR->nColIdx;
		}
	}

	if(IsPrimaryKeyCondition(pMember->m_tQueryUtil.m_tCdtWhereFpList) == 0)
	{
		pMember->m_tQueryUtil.m_tCdtWhereList.clear();
		pMember->m_tQueryUtil.m_tCdtWhereFpUsedMode = 1;
	}
	return 0;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::First()
{	
	CASIDBFQMember* pMember = GetMember();

	if(pMember->m_tQueryUtil.m_nSelectWhExist && pMember->m_tQueryUtil.m_tCdtWhereFpUsedMode)
	{		
		m_tASIDBFDLLUtil.ASIDBF_SetSelInfo(0, NULL, "");
	}
	else
	{
		m_tASIDBFDLLUtil.ASIDBF_SetSelInfo(pMember->m_tQueryUtil.m_nSelectItemNum, pMember->m_tQueryUtil.m_tSelValIDArray);
	}
		
	return m_tASIDBFDLLUtil.ASIDBF_First();
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::Last()
{
	CASIDBFQMember* pMember = GetMember();

	INT32 nRtn = 0;
	if(pMember->m_tQueryUtil.m_nSelectWhExist && pMember->m_tQueryUtil.m_tCdtWhereFpUsedMode)
	{		
		m_tASIDBFDLLUtil.ASIDBF_SetSelInfo(0, NULL, "");
	}
	else
	{
		m_tASIDBFDLLUtil.ASIDBF_SetSelInfo(pMember->m_tQueryUtil.m_nSelectItemNum, pMember->m_tQueryUtil.m_tSelValIDArray);
	}
	return m_tASIDBFDLLUtil.ASIDBF_Last();
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::FirstKey(UINT64 nKey)
{	
	CASIDBFQMember* pMember = GetMember();

	if(pMember->m_tQueryUtil.m_nSelectWhExist && pMember->m_tQueryUtil.m_tCdtWhereFpUsedMode)
	{		
		m_tASIDBFDLLUtil.ASIDBF_SetSelInfo(0, NULL, "");
	}
	else
	{
		m_tASIDBFDLLUtil.ASIDBF_SetSelInfo(pMember->m_tQueryUtil.m_nSelectItemNum, pMember->m_tQueryUtil.m_tSelValIDArray);
	}

	return m_tASIDBFDLLUtil.ASIDBF_FirstKey(NULL, nKey);
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::LastKey(UINT64 nKey)
{
	CASIDBFQMember* pMember = GetMember();

	INT32 nRtn = 0;
	if(pMember->m_tQueryUtil.m_nSelectWhExist && pMember->m_tQueryUtil.m_tCdtWhereFpUsedMode)
	{		
		m_tASIDBFDLLUtil.ASIDBF_SetSelInfo(0, NULL, "");
	}
	else
	{
		m_tASIDBFDLLUtil.ASIDBF_SetSelInfo(pMember->m_tQueryUtil.m_nSelectItemNum, pMember->m_tQueryUtil.m_tSelValIDArray);
	}
	return m_tASIDBFDLLUtil.ASIDBF_LastKey(NULL, nKey);
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::Next()
{
	INT32 nRtn = ASI_DBF_ERROR_TYPE_V2_NO_MORE_DATA;
	CASIDBFQMember* pMember = GetMember();

	if(pMember->m_tQueryUtil.m_nSelectWhExist)
	{		
		if(pMember->m_tQueryUtil.m_tCdtWhereFpUsedMode)
		{
			TListID64Itor begin, end;
			begin = pMember->m_tQueryUtil.m_tCdtWhereFpList.begin();		end = pMember->m_tQueryUtil.m_tCdtWhereFpList.end();

			if(begin != end)
			{
				nRtn = m_tASIDBFDLLUtil.ASIDBF_ExeSelect(*begin);
				pMember->m_tQueryUtil.m_tCdtWhereFpList.erase(begin);
			}			
			else
			{
				m_tASIDBFDLLUtil.ASIDBF_NextStop();
			}
		}
		else if(pMember->m_tQueryUtil.m_tCdtWhereList.empty() == FALSE)
		{
			INT32 nPriKeyChk = 0;
			while(m_tASIDBFDLLUtil.ASIDBF_Next(&(pMember->m_pDIRArray)) == 0 && !nPriKeyChk)
			{
				if(IsValidContidion(nPriKeyChk))
				{
					nRtn = 0;
					break;
				}
			}
			if(nPriKeyChk)	
			{
				m_tASIDBFDLLUtil.ASIDBF_NextStop();
				pMember->m_tQueryUtil.m_tCdtWhereList.clear();
			}

			//WriteLogN("next : [%d][%d]:[%d][%d]", m_tQueryUtil.m_tCdtWhereFpList.size(), m_tQueryUtil.m_tCdtWhereList.size(), nRtn, nPriKeyChk);
		}		
	}
	else 
	{
		nRtn = m_tASIDBFDLLUtil.ASIDBF_Next(&(pMember->m_pDIRArray));
	}
	
	return nRtn;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::NextStop()
{
	return m_tASIDBFDLLUtil.ASIDBF_NextStop();
}
//------------------------------------------------------------------

PDBF_ITEM_RTN		CASIDBFQUtil::GetField(INT32 nIdx)
{
	CASIDBFQMember* pMember = GetMember();

	if(pMember == NULL || pMember->m_pDIRArray == NULL)
		NULL;

	return &(pMember->m_pDIRArray[nIdx]);


// 	if(pMember->m_tQueryUtil.m_nSelectItemNum == 0)
// 	{
// 		return m_tASIDBFDLLUtil.GetField(nIdx);
// 	}
// 
// 	INT32 nOrIdx = pMember->m_tQueryUtil.ConvertSelectIdxToIdx(nIdx);
// 	if(nOrIdx < 0)
// 	{
// 		WriteLogE("get field name is empty : [%d]", nIdx);
// 		return NULL;
// 	}
// 	
// 	return m_tASIDBFDLLUtil.GetField(nOrIdx);
}
//------------------------------------------------------------------

PDBF_ITEM_RTN		CASIDBFQUtil::GetField(LPCTSTR lpName)
{
	return m_tASIDBFDLLUtil.GetField(lpName);
}
//------------------------------------------------------------------

UINT64				CASIDBFQUtil::GetFieldFP()
{
	return m_tASIDBFDLLUtil.GetFieldFP();
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------

INT32		CASIDBFQUtil::QueryExecuteResult(PUINT32 pnCmdType, PUINT32 pnCmdCode, PUINT32 pnRtnCnt)
{
	CASIDBFQMember* pMember = GetMember();	
	
	if(pnCmdType)		*pnCmdType = pMember->m_tQueryUtil.m_nQueryCmdType;
	if(pnCmdCode)		*pnCmdCode = pMember->m_tQueryUtil.m_nQueryCmdCode;
	if(pnRtnCnt)		*pnRtnCnt = pMember->m_tQueryUtil.m_nQueryRtnCnt;

	return 0;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::QueryExecute(CString strQuery)
{
	if(strQuery.IsEmpty())
	{
		return ASI_DBFQ_ERROR_TYPE_INVALID_QUERY;
	}

	UINT32	nSOpTickCnt = 0;
	if(m_tSQII.nRemainQOpTime)	nSOpTickCnt = GetTickCount();
	
	
	TListCStr tQueryList;
	LPCTSTR lpQuery = strQuery;
	INT32 nIdx = 0;
	INT32 nWaitNewLine = 0, nNonChkFlag = 0;
	CString strLine = "";
	while(nIdx < strQuery.GetLength())
	{
		CHAR chChk = lpQuery[nIdx++];
		
		if(chChk == '\'')
		{
			nNonChkFlag ^= 1;
			strLine += chChk;
			continue;
		}
		else if(nNonChkFlag)
		{
			strLine += chChk;
			continue;
		}
		else if(nWaitNewLine)
		{
			if(chChk == '\n')				nWaitNewLine ^= 1;
			continue;
		}
		else if(chChk == ';')
		{
			tQueryList.push_back(strLine);
			strLine = "";
			nWaitNewLine ^= 1;
			continue;
		}
		else
		{
			strLine += chChk;
		}
	}

	if(tQueryList.empty() && strLine.IsEmpty() == FALSE)	tQueryList.push_back(strLine);

	{
		TListCStrItor begin, end;
		begin = tQueryList.begin();	end = tQueryList.end();
		for(begin; begin != end; begin++)
		{
			UINT32 nAQOpTickCnt = 0;
			if(SetER(QueryExecuteLine(*begin, nAQOpTickCnt)))
			{
				WriteLogE("execute query fail : [%d][%s]", g_nErrRtn, (char *)(LPCTSTR)*begin);
				return -1;
			}

			if(m_tSQII.nRemainQOpTime)
			{
				UINT32 nEOpTickCnt = GetTickCount();
				if(nEOpTickCnt - nSOpTickCnt > (UINT32)m_tSQII.nRemainQOpTime)
				{
					WriteLogN("dbg_print : [opt:%u(%u) ms][%s]", nEOpTickCnt - nSOpTickCnt, nAQOpTickCnt - nSOpTickCnt, (char *)(LPCTSTR)*begin);
				}
			}
		}
	}
	return 0;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::QueryExecuteLine(CString strQuery, UINT32& nAQOT)
{
	CASIDBFQMember* pMember = GetMember();
	
	if(SetER(pMember->m_tQueryUtil.AnalysisQuery(strQuery)))
	{
		return ASI_DBFQ_ERROR_TYPE_INVALID_QUERY;
	}

	INT32 nRtn = 0;
	m_tASIDBFDLLUtil.SetCurTableName(pMember->m_tQueryUtil.m_strTName);

	switch(pMember->m_tQueryUtil.m_nQueryCmdType)
	{
		case ASI_DBFQ_QUERY_COMMAND_TYPE_CREATE:
		{	
			TListDBFQCDTInfoItor begin, end;
			begin = pMember->m_tQueryUtil.m_tCreateItemList.begin();		end = pMember->m_tQueryUtil.m_tCreateItemList.end();
			for(begin; begin != end; begin++)
			{
				m_tASIDBFDLLUtil.ASIDBF_PreSetDBF_TableColumn(begin->strName, begin->nType, begin->nDefault, 0, begin->nOption);
			}
			nRtn = m_tASIDBFDLLUtil.ASIDBF_ForceCreateDBF_Table();
			pMember->m_tQueryUtil.m_nQueryRtnCnt += 1;
			break;
		}
		case ASI_DBFQ_QUERY_COMMAND_TYPE_SELECT:
		{
			TListID64	tChgItemFPList;
			m_tASIDBFDLLUtil.ASIDBF_NextStop();

			if(pMember->m_tQueryUtil.m_tCdtWhereFpList.empty() == FALSE)
			{
				TListID64Itor begin, end;
				begin = pMember->m_tQueryUtil.m_tCdtWhereFpList.begin();	end = pMember->m_tQueryUtil.m_tCdtWhereFpList.end();
				for(begin; begin != end; begin++)
				{
					tChgItemFPList.push_back(*begin);
				}
			}
			else
			{
				if(IsPrimaryKeyCondition(tChgItemFPList))
				{
					INT32 nPriKeyChk = 0;
					m_tASIDBFDLLUtil.ASIDBF_SetSelInfo(pMember->m_tQueryUtil.m_nSelectItemNum, pMember->m_tQueryUtil.m_tSelValIDArray);
					m_tASIDBFDLLUtil.ASIDBF_First();
					while(m_tASIDBFDLLUtil.ASIDBF_Next() == 0 && !nPriKeyChk)
					{
						if(IsValidContidion(nPriKeyChk))		tChgItemFPList.push_back(m_tASIDBFDLLUtil.GetFieldFP());
					}

					m_tASIDBFDLLUtil.ASIDBF_NextStop();
				}
			}

			if(pMember->m_tQueryUtil.m_nSelectItemNum)
			{
				TMapIDCStrItor begin, end;
				begin = pMember->m_tQueryUtil.m_tSelValMap.begin();	end = pMember->m_tQueryUtil.m_tSelValMap.end();
				for(begin; begin != end; begin++)
				{
					PDBF_TABLE_COLUMN_RTN pDTCR = m_tASIDBFDLLUtil.GetTableColumn(begin->second); 
					if(pDTCR == NULL)
					{
						return ASI_DBFQ_ERROR_TYPE_INVALID_QUERY;
					}
					pMember->m_tQueryUtil.m_tSelValIDArray[begin->first] = pDTCR->nColIdx;
				}
			}

			{
				TListID64Itor begin, end;
				begin = tChgItemFPList.begin();		end = tChgItemFPList.end();
				if(begin != end)
				{
					nRtn = m_tASIDBFDLLUtil.ASIDBF_ExeSelect(*begin);
					if(!nRtn)	pMember->m_tQueryUtil.m_nQueryRtnCnt += 1;
				}
			}

			break;
		}
		case ASI_DBFQ_QUERY_COMMAND_TYPE_INSERT:
		{
			TListDBFQCDTInfoItor begin, end;
			begin = pMember->m_tQueryUtil.m_tInsertItemList.begin();		end = pMember->m_tQueryUtil.m_tInsertItemList.end();
			for(begin; begin != end; begin++)
			{
				switch(begin->nType)
				{
					case ASI_DBF_MEMBER_TYPE_LSTRING:
					{
						SetER(m_tASIDBFDLLUtil.ASIDBF_PreSetDBF_TableItem(begin->strName, (PVOID)(LPCTSTR)(begin->strDefault), begin->strDefault.GetLength()));
						break;
					}
					case ASI_DBF_MEMBER_TYPE_OBJECT:
					{
						SetER(m_tASIDBFDLLUtil.ASIDBF_PreSetDBF_TableItem(begin->strName, begin->lpDefault, begin->nDefLen));
						break;
					}
					case ASI_DBF_MEMBER_TYPE_UINT64:
					{
						SetER(m_tASIDBFDLLUtil.ASIDBF_PreSetDBF_TableItem(begin->strName, &(begin->nDefault)));
						break;
					}
				}

				if(g_nErrRtn)
				{
					WriteLogE("ASIDBF_PreSetDBF_TableItem fail : [%d]", g_nErrRtn);
					return ASI_DBFQ_ERROR_TYPE_INVALID_QUERY;
				}
			}

			if(m_tSQII.nRemainQOpTime)		nAQOT = GetTickCount();

			nRtn = m_tASIDBFDLLUtil.ASIDBF_ExeInsert(&(pMember->m_nLastFp), &(pMember->m_nLastID));
			pMember->m_tQueryUtil.m_nQueryRtnCnt += 1;
			break;
		}
		case ASI_DBFQ_QUERY_COMMAND_TYPE_UPDATE:
		{
			TListID64	tChgItemFPList;
			
			if(pMember->m_tQueryUtil.m_tCdtWhereFpList.empty() == FALSE)
			{
				TListID64Itor begin, end;
				begin = pMember->m_tQueryUtil.m_tCdtWhereFpList.begin();	end = pMember->m_tQueryUtil.m_tCdtWhereFpList.end();
				for(begin; begin != end; begin++)
				{
					tChgItemFPList.push_back(*begin);
				}
			}
			else
			{
				if(IsPrimaryKeyCondition(tChgItemFPList))
				{
					INT32 nPriKeyChk = 0;
					m_tASIDBFDLLUtil.ASIDBF_SetSelInfo();
					m_tASIDBFDLLUtil.ASIDBF_First();
					while(m_tASIDBFDLLUtil.ASIDBF_Next() == 0 && !nPriKeyChk)
					{
						if(IsValidContidion(nPriKeyChk))		tChgItemFPList.push_back(m_tASIDBFDLLUtil.GetFieldFP());
					}
					m_tASIDBFDLLUtil.ASIDBF_NextStop();
				}
			}

			{
				TListID64Itor begin, end;
				begin = tChgItemFPList.begin();		end = tChgItemFPList.end();
				for(begin; begin != end; begin++)
				{
					TListDBFQCDTInfoItor begin_item, end_item;
					begin_item = pMember->m_tQueryUtil.m_tUpdateItemList.begin();	end_item = pMember->m_tQueryUtil.m_tUpdateItemList.end();
					for(begin_item; begin_item != end_item; begin_item++)
					{
						switch(begin_item->nType)
						{
							case ASI_DBF_MEMBER_TYPE_LSTRING:
							{
								m_tASIDBFDLLUtil.ASIDBF_PreSetDBF_TableItem(begin_item->strName, (PVOID)(LPCTSTR)begin_item->strDefault, begin_item->strDefault.GetLength());
								break;
							}
							case ASI_DBF_MEMBER_TYPE_OBJECT:
							{
								m_tASIDBFDLLUtil.ASIDBF_PreSetDBF_TableItem(begin_item->strName, begin_item->lpDefault, begin_item->nDefLen);
								break;
							}
							case ASI_DBF_MEMBER_TYPE_UINT64:
							{
								m_tASIDBFDLLUtil.ASIDBF_PreSetDBF_TableItem(begin_item->strName, &(begin_item->nDefault));	
								break;
							}
						}
					}
					pMember->m_nLastFp = *begin;
					
					if(m_tSQII.nRemainQOpTime)		nAQOT = GetTickCount();

					nRtn = m_tASIDBFDLLUtil.ASIDBF_ExeUpdate(&(pMember->m_nLastFp));
					if(!nRtn)	pMember->m_tQueryUtil.m_nQueryRtnCnt += 1;
				}
			}			
			break;
		}
		case ASI_DBFQ_QUERY_COMMAND_TYPE_DELETE:
		{
			TListID64	tChgItemFPList;

			if(pMember->m_tQueryUtil.m_tCdtWhereFpList.empty() == FALSE)
			{
				TListID64Itor begin, end;
				begin = pMember->m_tQueryUtil.m_tCdtWhereFpList.begin();	end = pMember->m_tQueryUtil.m_tCdtWhereFpList.end();
				for(begin; begin != end; begin++)
				{
					tChgItemFPList.push_back(*begin);
				}
			}
			else
			{
				if(IsPrimaryKeyCondition(tChgItemFPList))
				{
					INT32 nPriKeyChk = 0;
					m_tASIDBFDLLUtil.ASIDBF_SetSelInfo();
					m_tASIDBFDLLUtil.ASIDBF_First();
					while(m_tASIDBFDLLUtil.ASIDBF_Next() == 0 && !nPriKeyChk)
					{
						if(IsValidContidion(nPriKeyChk))		tChgItemFPList.push_back(m_tASIDBFDLLUtil.GetFieldFP());
					}
					m_tASIDBFDLLUtil.ASIDBF_NextStop();
				}
			}

			{
				TListID64Itor begin, end;
				begin = tChgItemFPList.begin();		end = tChgItemFPList.end();
				for(begin; begin != end; begin++)
				{
					if(m_tSQII.nRemainQOpTime)		nAQOT = GetTickCount();

					UINT64 nCurFP = *begin;
					if(m_tASIDBFDLLUtil.ASIDBF_ExeDelete(nCurFP))	continue;

					pMember->m_tQueryUtil.m_nQueryRtnCnt += 1;
				}
			}			
			break;
		}
		case ASI_DBFQ_QUERY_COMMAND_TYPE_DROP:
		{
			nRtn = m_tASIDBFDLLUtil.ASIDBF_DropDBF_Table(pMember->m_tQueryUtil.m_strTName);
			break;
		}
		case ASI_DBFQ_QUERY_COMMAND_TYPE_RENAME:
		{
			nRtn = m_tASIDBFDLLUtil.ASIDBF_RenameDBF_Table(pMember->m_tQueryUtil.m_strTName, pMember->m_tQueryUtil.m_strNewTName);
			break;
		}
		case ASI_DBFQ_QUERY_COMMAND_TYPE_ALTER:
		{
			TListDBFQCDTInfoItor begin, end;
			begin = pMember->m_tQueryUtil.m_tAlterItemList.begin();	end = pMember->m_tQueryUtil.m_tAlterItemList.end();
			for(begin; begin != end && !nRtn; begin++)
			{
				switch(pMember->m_tQueryUtil.m_nQueryCmdCode)
				{
					case ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_ADD:
					{
						nRtn = m_tASIDBFDLLUtil.ASIDBF_ModifyDBF_TableAddColumn(begin->strName, begin->nType, begin->nDefault, 0, begin->nOption);
						break;
					}
					case ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_DEL:
					{
						nRtn = m_tASIDBFDLLUtil.ASIDBF_ModifyDBF_TableDelColumn(begin->strName);
						break;
					}
					case ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_RENAME:
					{
						nRtn = m_tASIDBFDLLUtil.ASIDBF_ModifyDBF_TableRenameColumn(begin->strName, begin->strNewName);
						break;
					}
					case ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_RELEN:
					{
						nRtn = m_tASIDBFDLLUtil.ASIDBF_ModifyDBF_TableReLenColumn(begin->strName, begin->nDefLen);
						break;
					}
					case ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_AUTOINCREMENT:
					{
						nRtn = m_tASIDBFDLLUtil.ASIDBF_ModifyDBF_TableInitAutoIncrement(begin->nDefault);
						break;
					}
					case ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_DB_RECOVERY:
					{
						nRtn = m_tASIDBFDLLUtil.ASIDBF_RecoveryDB();
						break;
					}
				}

				if(!nRtn)	pMember->m_tQueryUtil.m_nQueryRtnCnt += 1;
			}			
			break;
		}
	}
	return nRtn;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::IsValidContidion(INT32& nPrimaryKey)
{
	CASIDBFQMember* pMember = GetMember();

	TListDBFQCDTInfoWhereItor begin, end;
	begin = pMember->m_tQueryUtil.m_tCdtWhereList.begin();	end = pMember->m_tQueryUtil.m_tCdtWhereList.end();
	for(begin; begin != end; begin++)
	{
		PDBF_ITEM_RTN pDIR = m_tASIDBFDLLUtil.GetField(begin->strName);
		PDBF_TABLE_COLUMN_RTN pDTCR = m_tASIDBFDLLUtil.GetTableColumn(begin->strName);
		if(!pDIR || !pDTCR)
		{
			WriteLogE("invalid where clause key name [%s]", (char *)(LPCSTR)begin->strName);
			return 0;
		}
		
		switch(pDIR->nRtnType)
		{
			case ASI_DBF_MEMBER_TYPE_UINT16:
			{
				if(begin->nChkType != ASI_DBF_MEMBER_TYPE_UINT64)
				{
					WriteLogE("invalid where clause key type : [%d][%d]", pDIR->nRtnType, begin->nChkType);
					return 0;
				}

				if(IsValidCompWhCdt(&(*begin), *(pDIR->pRtnUINT16)) == 0)
				{
					return 0;
				}
				else if((pDTCR->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY) && begin->nCompType == ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL)
				{
					nPrimaryKey |= 1;
				}
				break;
			}
			case ASI_DBF_MEMBER_TYPE_UINT32:
			{
				if(begin->nChkType != ASI_DBF_MEMBER_TYPE_UINT64)
				{
					WriteLogE("invalid where clause key type : [%d][%d]", pDIR->nRtnType, begin->nChkType);
					return 0;
				}

				if(IsValidCompWhCdt(&(*begin), *(pDIR->pRtnUINT32)) == 0)
				{
					return 0;
				}
				else if((pDTCR->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY) && begin->nCompType == ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL)
				{
					nPrimaryKey |= 1;
				}
				break;
			}
			case ASI_DBF_MEMBER_TYPE_UINT64:
			{
				if(begin->nChkType != ASI_DBF_MEMBER_TYPE_UINT64)
				{
					WriteLogE("invalid where clause key type : [%d][%d]", pDIR->nRtnType, begin->nChkType);
					return 0;
				}

				if(IsValidCompWhCdt(&(*begin), *(pDIR->pRtnUINT64)) == 0)
				{
					return 0;
				}
				else if((pDTCR->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY) && begin->nCompType == ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL)
				{
					nPrimaryKey |= 1;
				}
				break;
			}
			case ASI_DBF_MEMBER_TYPE_STRING:
			{
				if(begin->nChkType != ASI_DBF_MEMBER_TYPE_STRING)
				{
					WriteLogE("invalid where clause key type : [%d][%d]", pDIR->nRtnType, begin->nChkType);
					return 0;
				}

				if(IsValidCompWhCdt(&(*begin), CString(pDIR->szRtnString, pDIR->nRtnLen)) == 0)
				{
					return 0;
				}
				else if((pDTCR->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY) && begin->nCompType == ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL)
				{
					nPrimaryKey |= 1;
				}
				break;
			}
			case ASI_DBF_MEMBER_TYPE_LSTRING:
			{
				if(begin->nChkType != ASI_DBF_MEMBER_TYPE_LSTRING)
				{
					WriteLogE("invalid where clause key type : [%d][%d]", pDIR->nRtnType, begin->nChkType);
					return 0;
				}

				if(IsValidCompWhCdt(&(*begin), CString(pDIR->szRtnString, pDIR->nRtnLen)) == 0)
				{
					return 0;
				}
				else if((pDTCR->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY) && begin->nCompType == ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL)
				{
					nPrimaryKey |= 1;
				}
				break;
			}
			case ASI_DBF_MEMBER_TYPE_OBJECT:
			{
				if(begin->nChkType != ASI_DBF_MEMBER_TYPE_OBJECT)
				{
					WriteLogE("invalid where clause key type : [%d][%d]", pDIR->nRtnType, begin->nChkType);
					return 0;
				}
				break;
			}
		}
	}

	if(pMember->m_tQueryUtil.m_tCdtWhereList.size() > 1)		nPrimaryKey = 0;

	return 1;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::IsValidCompWhCdt(PDBFQ_CDT_INFO_WHERE pDCIW, UINT64 nValue)
{
	INT32 nRtn = 0;
	
	switch(pDCIW->nCompType)
	{
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL:			nRtn = (nValue == pDCIW->nChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_SMALL:			nRtn = (nValue < pDCIW->nChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_LARGE:			nRtn = (nValue > pDCIW->nChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_NOT_EQUAL:		nRtn = (nValue != pDCIW->nChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_SMALL:	nRtn = (nValue <= pDCIW->nChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_LARGE:	nRtn = (nValue >= pDCIW->nChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_BETWEEN:	
		{
			if(pDCIW->nChkValueS <= nValue && nValue <= pDCIW->nChkValueE)	nRtn = 1;
			break;
		}
	}

	//WriteLogN("is valid condition wh cdt : [%d][%u][%u]:[%d]", pDCIW->nCompType, nValue, pDCIW->nChkValueS, nRtn);
	return nRtn;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::IsValidCompWhCdt(PDBFQ_CDT_INFO_WHERE pDCIW, CString strValue)
{
	INT32 nRtn = 0;

	switch(pDCIW->nCompType)
	{
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL:			nRtn = (strValue == pDCIW->strChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_SMALL:			nRtn = (strValue < pDCIW->strChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_LARGE:			nRtn = (strValue > pDCIW->strChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_NOT_EQUAL:		nRtn = (strValue != pDCIW->strChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_SMALL:	nRtn = (strValue <= pDCIW->strChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_LARGE:	nRtn = (strValue >= pDCIW->strChkValueS ? 1 : 0);		break;
		case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_BETWEEN:	
		{
			if(pDCIW->strChkValueS <= strValue && strValue <= pDCIW->strChkValueE)	nRtn = 1;
			break;
		}
	}
	return nRtn;
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::IsPrimaryKeyCondition(TListID64& tChgItemFPList)
{	
	CASIDBFQMember* pMember = GetMember();

	if(pMember->m_tQueryUtil.m_tCdtWhereList.size() > 1)	return -1000;
	
	INT32 nRtn = 0;
	TListDBFQCDTInfoWhereItor begin, end;
	begin = pMember->m_tQueryUtil.m_tCdtWhereList.begin();	end = pMember->m_tQueryUtil.m_tCdtWhereList.end();
	for(begin; begin != end; begin++)
	{
		PDBF_TABLE_COLUMN_RTN pDTCR = m_tASIDBFDLLUtil.GetTableColumn(begin->strName);
		if(!pDTCR)
		{
			WriteLogE("invalid where clause key name [%s]", (char *)(LPCSTR)begin->strName);
			return -100;
		}

		if((pDTCR->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY) == 0)
		{
			return -1;
		}

		switch(pDTCR->nMemberType)
		{
			case ASI_DBF_MEMBER_TYPE_UINT16:
			case ASI_DBF_MEMBER_TYPE_UINT32:
			case ASI_DBF_MEMBER_TYPE_UINT64:
			{
				if(begin->nChkType != ASI_DBF_MEMBER_TYPE_UINT64)
				{
					WriteLogE("invalid where clause key type : [%d][%d]", pDTCR->nMemberType, begin->nChkType);
					return -10;
				}

				PUINT64 pnFPList = NULL;
				UINT32 nFPListSize = 0;

				if(m_tASIDBFDLLUtil.ASIDBF_GetTbItemPriKeyFPList(pMember->m_tQueryUtil.m_strTName, begin->strName, begin->nChkValueS, begin->nChkValueE, "", "", begin->nCompType, &pnFPList, &nFPListSize))
				{
					break;
				}
				
				if(nFPListSize)
				{
					INT32 nCpIdx = 0;
					while(nCpIdx < nFPListSize)
					{
						tChgItemFPList.push_back(pnFPList[nCpIdx]);
						nCpIdx += 1;
					}

					m_tASIDBFDLLUtil.ASIDBF_FreeTbItemPriKeyFPList(&pnFPList);
					nRtn =nFPListSize;
				}
				break;
			}
			case ASI_DBF_MEMBER_TYPE_STRING:
			{
				if(begin->nChkType != ASI_DBF_MEMBER_TYPE_STRING)
				{
					WriteLogE("invalid where clause key type : [%d][%d]", pDTCR->nMemberType, begin->nChkType);
					return -10;
				}
				
				PUINT64 pnFPList = NULL;
				UINT32 nFPListSize = 0;

				if(m_tASIDBFDLLUtil.ASIDBF_GetTbItemPriKeyFPList(pMember->m_tQueryUtil.m_strTName, begin->strName, begin->nChkValueS, begin->nChkValueE, begin->strChkValueS, begin->strChkValueE, begin->nCompType, &pnFPList, &nFPListSize))
				{
					break;
				}

				if(nFPListSize)
				{
					INT32 nCpIdx = 0;
					while(nCpIdx < nFPListSize)
					{
						tChgItemFPList.push_back(pnFPList[nCpIdx]);
						nCpIdx += 1;
					}

					m_tASIDBFDLLUtil.ASIDBF_FreeTbItemPriKeyFPList(&pnFPList);
					nRtn = nFPListSize;
				}
				break;
			}
			case ASI_DBF_MEMBER_TYPE_LSTRING:
			{
				if(begin->nChkType != ASI_DBF_MEMBER_TYPE_LSTRING)
				{
					WriteLogE("invalid where clause key type : [%d][%d]", pDTCR->nMemberType, begin->nChkType);
					return -10;
				}

				PUINT64 pnFPList = NULL;
				UINT32 nFPListSize = 0;

				if(m_tASIDBFDLLUtil.ASIDBF_GetTbItemPriKeyFPList(pMember->m_tQueryUtil.m_strTName, begin->strName, begin->nChkValueS, begin->nChkValueE, begin->strChkValueS, begin->strChkValueE, begin->nCompType, &pnFPList, &nFPListSize))
				{
					break;
				}

				if(nFPListSize)
				{
					INT32 nCpIdx = 0;
					while(nCpIdx < nFPListSize)
					{
						tChgItemFPList.push_back(pnFPList[nCpIdx]);
						nCpIdx += 1;
					}

					m_tASIDBFDLLUtil.ASIDBF_FreeTbItemPriKeyFPList(&pnFPList);
					nRtn = nFPListSize;
				}
				break;
			}
			
			case ASI_DBF_MEMBER_TYPE_OBJECT:
			{
				if(begin->nChkType != ASI_DBF_MEMBER_TYPE_OBJECT)
				{
					WriteLogE("invalid where clause key type : [%d][%d]", pDTCR->nMemberType, begin->nChkType);
					return -10;
				}				
				break;
			}
		}
	}

	return (nRtn ? 0 : -101);
}
//------------------------------------------------------------------

CASIDBFQMember*			CASIDBFQUtil::GetMember()
{
	CASIDBFQMember*	pRtn = NULL;
	m_tMutexMember.Lock();

	UINT64 nThreadID = (UINT64)gettid();
	TMapASIDBFQMemberItor find = m_tDbfqUtilMemberMap.find(nThreadID);
	if(find == m_tDbfqUtilMemberMap.end())
	{
		CASIDBFQMember* pMember = new CASIDBFQMember();
		m_tDbfqUtilMemberMap[nThreadID] = pMember;
		find = m_tDbfqUtilMemberMap.find(nThreadID);	
	}

	pRtn = (CASIDBFQMember*)(find->second);
	m_tMutexMember.UnLock();
	return pRtn;
}
//---------------------------------------------------------------------

void					CASIDBFQUtil::ClearMember()
{
	TMapASIDBFQMemberItor begin, end;
	begin = m_tDbfqUtilMemberMap.begin();	end = m_tDbfqUtilMemberMap.end();
	for(begin; begin != end; begin++)
	{
		CASIDBFQMember* pdata = (CASIDBFQMember*)(begin->second);
		if(!pdata)	continue;

		delete pdata;
	}
	m_tDbfqUtilMemberMap.clear();
	return;
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::IsExistTable(CString strTName)
{
	DBF_TABLE_INFO_RTN tDTIR;
	return m_tASIDBFDLLUtil.ASIDBF_GetTbInfo(&tDTIR, strTName);
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::GetTbVer(CString strTName, PUINT32 nVer)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetTbSVer(nVer, strTName);
}
//------------------------------------------------------------------

INT32		CASIDBFQUtil::SetTbVer(CString strTName, UINT32 nVer)
{
	return m_tASIDBFDLLUtil.ASIDBF_SetTbSVer(nVer, strTName);
}
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_RecoveryDB(UINT32 nRemainMode)
{
	return m_tASIDBFDLLUtil.ASIDBF_RecoveryDB(nRemainMode);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_BackupDB(CString strBkFileName)
{
	return m_tASIDBFDLLUtil.ASIDBF_BackupDB(strBkFileName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_IsAutoRestore(INT32& nMode)
{
	return m_tASIDBFDLLUtil.ASIDBF_IsAutoRestore(nMode);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ChangeAuthInfoDB(LPCTSTR lpCurUserID, LPCTSTR lpCurUserPw, LPCTSTR lpNewUserID, LPCTSTR lpNewUserPw, UINT32 nChkNewType)
{
	return m_tASIDBFDLLUtil.ASIDBF_ChangeAuthInfoDB(lpCurUserID, lpCurUserPw, lpNewUserID, lpNewUserPw, nChkNewType);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_SetTItemEncTypeDB(UINT32 nDefEncType)
{
	return m_tASIDBFDLLUtil.ASIDBF_SetTItemEncTypeDB(nDefEncType);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_SetTItemCompTypeDB(UINT32 nDefCompType)
{
	return m_tASIDBFDLLUtil.ASIDBF_SetTItemCompTypeDB(nDefCompType);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_GetTItemEncTypeDB(PUINT32 pnDefEncType)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetTItemEncTypeDB(pnDefEncType);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_GetTItemCompTypeDB(PUINT32 pnDefCompType)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetTItemCompTypeDB(pnDefCompType);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------


INT32		CASIDBFQUtil::ASIDBF_PreSetDBF_TableColumn(LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption, LPCTSTR lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_PreSetDBF_TableColumn(lpColName, nMemberType, nDefaultValue, nInitStrLen, nColOption, lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ForceCreateDBF_Table(LPCTSTR lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ForceCreateDBF_Table(lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ModifyDBF_TableAddColumn(LPCTSTR lpColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption, LPCTSTR lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ModifyDBF_TableAddColumn(lpColName, nMemberType, nDefaultValue, nInitStrLen, nColOption, lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ModifyDBF_TableDelColumn(LPCTSTR lpColName, LPCTSTR lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ModifyDBF_TableDelColumn(lpColName, lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ModifyDBF_TableRenameColumn(LPCTSTR lpColName, LPCTSTR lpNewColName, LPCTSTR lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ModifyDBF_TableRenameColumn(lpColName, lpNewColName, lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ModifyDBF_TableInitAutoIncrement(UINT64 nInitValue, LPCTSTR lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ModifyDBF_TableInitAutoIncrement(nInitValue, lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_DropDBF_Table(LPCTSTR lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_DropDBF_Table(lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ReloadDBF_Table()
{
	return m_tASIDBFDLLUtil.ASIDBF_ReloadDBF_Table();
}
//---------------------------------------------------------------------


INT32		CASIDBFQUtil::ASIDBF_GetTbInfo(CString strTName, PDBF_TABLE_INFO_RTN pDTIR)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetTbInfo(pDTIR, strTName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_GetTbInfoList(PDBF_TABLE_INFO_RTN pInfoBuffList, UINT32 nBuffLen, PUINT32 pnTableNum)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetTbInfoList(pInfoBuffList, nBuffLen, pnTableNum);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_GetTbColumnList(CString strTName, PDBF_TABLE_COLUMN_RTN pColBuffList, UINT32 nBuffLen, PUINT32 pnColNum)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetTbColumnList(strTName, pColBuffList, nBuffLen, pnColNum);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_GetTbColumnIdx(CString strTName, CString strTCName, PUINT32 pnColIdx)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetTbColumnIdx(strTName, strTCName, pnColIdx);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_First(CString strTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_First(strTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_Last(CString strTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_Last(strTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_FirstKey(CString strTableName, UINT64 nKey)
{
	return m_tASIDBFDLLUtil.ASIDBF_FirstKey(strTableName, nKey);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_LastKey(CString strTableName, UINT64 nKey)
{
	return m_tASIDBFDLLUtil.ASIDBF_LastKey(strTableName, nKey);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_Next(CString strTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_Next(NULL, strTableName);
}
//---------------------------------------------------------------------
INT32		CASIDBFQUtil::ASIDBF_NextStop()
{
	return m_tASIDBFDLLUtil.ASIDBF_NextStop();
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_PreSetDBF_TableItem(CString strColName, PVOID lpBuff, UINT32 nBufLen, CString strTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_PreSetDBF_TableItem(strColName, lpBuff, nBufLen, strTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ExeSelect(UINT64 nFP, CString lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ExeSelect(nFP, lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ExeInsert(PUINT64 pnNewFP, PUINT64 pnLastID, CString strTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ExeInsert(pnNewFP, pnLastID, strTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ExeUpdate(PUINT64 pnFP, CString strTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ExeUpdate(pnFP, strTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ExeDelete(UINT64 nFP, CString strTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ExeDelete(nFP, strTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ExeSelectByID(UINT32 nID, CString lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ExeSelectByID(nID, lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ExeUpdateByID(UINT32 nID, CString lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ExeUpdateByID(nID, lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_ExeDeleteByID(UINT32 nID, CString lpTableName)
{
	return m_tASIDBFDLLUtil.ASIDBF_ExeDeleteByID(nID, lpTableName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_FastSetCurTable(LPCTSTR szTName)
{
	return m_tASIDBFDLLUtil.ASIDBF_FastSetCurTable(szTName);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_FastPreSetDBF_TableItem(LPCTSTR szColName, UINT32 nMemberType, PVOID lpBuff, UINT32 nBufLen)
{
	return m_tASIDBFDLLUtil.ASIDBF_FastPreSetDBF_TableItem(szColName, nMemberType, lpBuff, nBufLen);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_FastExeInsertDBF(PUINT64 pnLastID, PUINT64 pnNewFP)
{
	return m_tASIDBFDLLUtil.ASIDBF_FastExeInsertDBF(pnLastID, pnNewFP);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_FastExeUpdateDBF(UINT32 nID)
{
	return m_tASIDBFDLLUtil.ASIDBF_FastExeUpdateDBF(nID);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_GetFieldByIdx(INT32 nIdx, PDBF_ITEM_RTN pdfir)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetFieldByIdx(nIdx, pdfir);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_GetFieldByName(CString strName, PDBF_ITEM_RTN pdfir)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetFieldByName(strName, pdfir);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_GetTableNextID(LPCTSTR lpTName, PUINT64 pnNextID)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetTableNextID(lpTName, pnNextID);
}
//---------------------------------------------------------------------

INT32		CASIDBFQUtil::ASIDBF_GetTableItemFPByPKey(LPCTSTR lpTName, UINT64 nKey, PUINT64 pnFP)
{
	return m_tASIDBFDLLUtil.ASIDBF_GetTableItemFPByPKey(lpTName, nKey, pnFP);
}
//---------------------------------------------------------------------
