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
#include "QueryUtil.h"

CQueryUtil::CQueryUtil(void)
{
}
//------------------------------------------------------------

CQueryUtil::~CQueryUtil(void)
{
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisQuery(CString strQuery)
{
	m_nQueryCmdType = 0;
	m_nQueryCmdCode	= 0;
	m_nQueryRtnCnt	= 0; 
	m_strQuery = strQuery;
	m_strQuery.Trim();
	m_nSelectItemNum = 0;

	//Init
	{
		m_tSelValMap.clear();		
		m_tCreateItemList.clear();
		m_tInsertItemList.clear();
		m_tUpdateItemList.clear();
		m_tAlterItemList.clear();

		memset(m_tSelValIDArray, -1, sizeof(UINT32)*ASI_DBF_MAX_COLUMN_NUMBER);

		m_tCdtWhereFpList.clear();
		m_tCdtWhereList.clear();
		m_tCdtWhereFpUsedMode = 0;
	}

	{
		INT32 nIdx = 0;
		INT32 nContinue = 1;
		INT32 nLen = m_strQuery.GetLength();
		char *lpQueryBuff = (char *)(LPCTSTR)m_strQuery;

		CString strRemain;
		char acCmd[MAX_TYPE_LEN] = {0,};
		while(nIdx < nLen && nContinue)
		{
			if(lpQueryBuff[nIdx] == ' ')
			{
				acCmd[nIdx] = 0;
				nIdx++;
				break;
			}
//			strCmd += (char)lpQueryBuff[nIdx];
			acCmd[nIdx] = lpQueryBuff[nIdx];
			nIdx++;
			if(nIdx > MAX_TYPE_LEN-1)
				break;
		}
		strRemain = &(lpQueryBuff[nIdx]);

		switch(AnalysisCommand(acCmd))
		{
			case ASI_DBFQ_QUERY_COMMAND_TYPE_SELECT:		SetER(AnalysisCmd_Select(strRemain));			break;
			case ASI_DBFQ_QUERY_COMMAND_TYPE_INSERT:		SetER(AnalysisCmd_Insert(strRemain));			break;
			case ASI_DBFQ_QUERY_COMMAND_TYPE_UPDATE:		SetER(AnalysisCmd_Update(strRemain));			break;
			case ASI_DBFQ_QUERY_COMMAND_TYPE_DELETE:		SetER(AnalysisCmd_Delete(strRemain));			break;
			case ASI_DBFQ_QUERY_COMMAND_TYPE_DROP:			SetER(AnalysisCmd_Drop(strRemain));				break;
			case ASI_DBFQ_QUERY_COMMAND_TYPE_CREATE:		SetER(AnalysisCmd_Create(strRemain));			break;
			case ASI_DBFQ_QUERY_COMMAND_TYPE_ALTER:			SetER(AnalysisCmd_Alter(strRemain));			break;
			case ASI_DBFQ_QUERY_COMMAND_TYPE_RENAME:		SetER(AnalysisCmd_Rename(strRemain));			break;
			default:
			{
				WriteLogE("invalid query command : [%s]", acCmd);
				return -10;
			}
		}
		if(g_nErrRtn)
		{
			WriteLogE("invalid query [%d][e:%d]", m_nQueryCmdType, g_nErrRtn);
			return -20;
		}
	}
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCommand(CString strCmd)
{
	if(strCmd.CompareNoCase("select") == 0)		m_nQueryCmdType = ASI_DBFQ_QUERY_COMMAND_TYPE_SELECT;
	if(strCmd.CompareNoCase("insert") == 0)		m_nQueryCmdType = ASI_DBFQ_QUERY_COMMAND_TYPE_INSERT;
	if(strCmd.CompareNoCase("update") == 0)		m_nQueryCmdType = ASI_DBFQ_QUERY_COMMAND_TYPE_UPDATE;
	if(strCmd.CompareNoCase("delete") == 0)		m_nQueryCmdType = ASI_DBFQ_QUERY_COMMAND_TYPE_DELETE;
	if(strCmd.CompareNoCase("drop") == 0)		m_nQueryCmdType = ASI_DBFQ_QUERY_COMMAND_TYPE_DROP;
	if(strCmd.CompareNoCase("create") == 0)		m_nQueryCmdType = ASI_DBFQ_QUERY_COMMAND_TYPE_CREATE;
	if(strCmd.CompareNoCase("alter") == 0)		m_nQueryCmdType = ASI_DBFQ_QUERY_COMMAND_TYPE_ALTER;
	if(strCmd.CompareNoCase("rename") == 0)		m_nQueryCmdType = ASI_DBFQ_QUERY_COMMAND_TYPE_RENAME;

	return m_nQueryCmdType;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCmd_Select(CString strCmd)
{
	CString strName, strWClause;
	ExportCmd_SelNameTableWhere(strCmd, strName, m_strTName, strWClause);

	m_nSelectItemNum = 0;
	m_nSelectWhExist = 0;
	m_tSelValMap.clear();

	if(strName.IsEmpty() || m_strTName.IsEmpty())
	{
		WriteLogE("[select] invalid query [%s][%s]", (char *)(LPCSTR)m_strTName, (char *)(LPCSTR)strName);
		return -1;
	}

	//WriteLogN("analysis cmd : select : [%s][%s][%s]", m_strTName, strName, strWClause);

	INT32 nIdx = 0;
	CTokenQuery Token(strName, strName.GetLength(), ',');
	while(Token.IsNextToken())
	{
		CString strSelValue = Token.NextToken();
		strSelValue.Trim();
		if(strSelValue.Compare("*") == 0)
		{
			break;
		}
		else
		{
			//WriteLogN("select item info : [%d][%s]", nIdx, strSelValue);
			m_tSelValMap[nIdx++] = strSelValue;			
		}		
	}

	m_nSelectItemNum = m_tSelValMap.size();

	if(!strWClause.IsEmpty())
	{
		AnalysisCmd_WhereClause(strWClause);
		m_nSelectWhExist = 1;
	}

	return 0;	
}
//------------------------------------------------------------

CString		CQueryUtil::ConvertSelectIdxToStr(INT32 nIdx)
{
	TMapIDCStrItor find = m_tSelValMap.find(nIdx);

	if(find == m_tSelValMap.end())
	{
		WriteLogE("request invalid idx : [%d]", nIdx);
		return "";
	}
	return find->second;
}
//------------------------------------------------------------

INT32		CQueryUtil::ConvertSelectIdxToIdx(INT32 nIdx)
{
	INT32 nRtn = m_tSelValIDArray[nIdx];
	if(nRtn == -1)
	{
		WriteLogE("request invalid idx : [%d]", nIdx);
	}
	return nRtn;

// 	TMapIDItor find = m_tSelValIDMap.find(nIdx);
// 	if(find == m_tSelValIDMap.end())
// 	{
// 		WriteLogE("request invalid idx : [%d]", nIdx);
// 		return -1;
// 	}
// 	return find->second;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCmd_Insert(CString strCmd)
{	
	CTokenQuery Token(strCmd, strCmd.GetLength());
	Token.NextToken();	//into

	m_strTName = Token.NextTokenTableName();

	{
		CString strRmCmd = Token.RemainValue();				
		//strRmCmd = RemoveFirstLastChar(strRmCmd, '(', ')');

		CString strName, strValue;
		ExportCmd_InsNameValue(strRmCmd, strName, strValue);

		CTokenQuery TokenName(strName, strName.GetLength(), ',');
		CTokenQuery TokenValue(strValue, strValue.GetLength(), ',');

		while(TokenName.IsNextToken() && TokenValue.IsNextToken())
		{
			CString strCellName = TokenName.NextToken();
			CString strCellValue = TokenValue.NextTokenValue();

			DBFQ_CDT_INFO tDCI;
			tDCI.strName	= strCellName;		tDCI.strName.Trim();

			if(strCellValue.Find("\'") != -1)
			{
				tDCI.nType	= ASI_DBF_MEMBER_TYPE_LSTRING;
				tDCI.strDefault = RemoveFirstLastChar(strCellValue, '\'', '\'');
			}
			else if(strCellValue.Find(":") != -1)
			{
				INT32 nTrimPos = strCellValue.Find(":");

				tDCI.nType = ASI_DBF_MEMBER_TYPE_OBJECT;
				tDCI.lpDefault = (PVOID)StrToInt64(strCellValue.Left(nTrimPos));
				tDCI.nDefLen = (UINT32)StrToInt64(strCellValue.Right(strCellValue.GetLength() - (nTrimPos + 1)));
			}
			else
			{
				tDCI.nType = ASI_DBF_MEMBER_TYPE_UINT64;
				tDCI.nDefault = StrToInt64(strCellValue);
			}

			m_tInsertItemList.push_back(tDCI);
		}

		if(TokenName.IsNextToken() || TokenValue.IsNextToken())
		{
			WriteLogE("[insert]\tinvalid count key and value : [%s][%s][%s]", (TokenName.IsNextToken() ? "name" : "value"), (char *)(LPCSTR)strName, (char *)(LPCSTR)strValue);
			return -1;
		}
	}	
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCmd_Update(CString strCmd)
{
	CTokenQuery Token(strCmd, strCmd.GetLength());

	m_strTName = Token.NextTokenTableName();

	Token.NextToken();

	{
		CString strRmCmd = Token.RemainValue();						

		CString strValue, strWClause;
		if(SetER(ExportCmd_WhereClause(strRmCmd, strValue, strWClause)))
		{
			WriteLogE("analysiscmd_update fail : %d", g_nErrRtn);
			return -10;
		}

		CTokenQuery TokenNameValue(strValue, strValue.GetLength(), ',');
		while(TokenNameValue.IsNextToken())
		{
			CString strCellNameValue = TokenNameValue.NextTokenValue();

			CTokenCString TokenCNV(strCellNameValue, strCellNameValue.GetLength(), '=');
			DBFQ_CDT_INFO tDCI;

			tDCI.strName	= TokenCNV.NextToken();		tDCI.strName.Trim();

			CString strCellValue = TokenCNV.RemainValue();

			if(strCellValue.Find("\'") != -1)
			{
				tDCI.nType	= ASI_DBF_MEMBER_TYPE_LSTRING;
				tDCI.strDefault = RemoveFirstLastChar(strCellValue, '\'', '\'');
			}
			else if(strCellValue.Find(":") != -1)
			{
				INT32 nTrimPos = strCellValue.Find(":");

				tDCI.nType = ASI_DBF_MEMBER_TYPE_OBJECT;
				tDCI.lpDefault = (PVOID)StrToInt64(strCellValue.Left(nTrimPos));
				tDCI.nDefLen = (UINT32)StrToInt64(strCellValue.Right(strCellValue.GetLength() - (nTrimPos + 1)));
			}
			else
			{
				tDCI.nType = ASI_DBF_MEMBER_TYPE_UINT64;
				tDCI.nDefault = ConvertOnlyNumberFromStr(strCellValue);
			}

			m_tUpdateItemList.push_back(tDCI);
		}

		if(!strWClause.IsEmpty())
		{
			AnalysisCmd_WhereClause(strWClause);
		}
	}	
	
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCmd_Delete(CString strCmd)
{
	CTokenQuery Token(strCmd, strCmd.GetLength());
	Token.NextToken();	//from

	CString strRmCmd = Token.RemainValue();
	CString strTName, strWClause;
	
	if(SetER(ExportCmd_WhereClause(strRmCmd, strTName, strWClause)))
	{
		WriteLogE("analysiscmd_delete fail : %d", g_nErrRtn);
		return -10;
	}

	Token.SetBuffer(strTName, strTName.GetLength());	
	m_strTName = Token.NextTokenTableName();	
	
	if(!strWClause.IsEmpty())
	{
		AnalysisCmd_WhereClause(strWClause);
	}	
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCmd_Drop(CString strCmd)
{
	CTokenQuery Token(strCmd, strCmd.GetLength());
	Token.NextToken();	//table

	m_strTName = Token.NextToken();
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCmd_Rename(CString strCmd)
{
	CTokenQuery Token(strCmd, strCmd.GetLength());
	Token.NextToken();	//table

	m_strTName		= Token.NextToken();
	m_strNewTName	= Token.NextToken();
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCmd_Create(CString strCmd)
{
	CTokenQuery Token(strCmd, strCmd.GetLength());
	Token.NextToken();
	
	m_strTName = Token.NextTokenTableName();

	{
		CString strRmCmd = Token.RemainValue();	
		
		strRmCmd = RemoveFirstLastChar(strRmCmd, '(', ')');
		
		
		CTokenQuery TokenItem(strRmCmd, strRmCmd.GetLength(), ',');
		while(TokenItem.IsNextToken())
		{
			CString strItem = TokenItem.NextToken();
			DBFQ_CDT_INFO tDCI;

			if(AnalysisCmd_CreateItem(strItem, tDCI))		continue;

			m_tCreateItemList.push_back(tDCI);
		}
	}	
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCmd_CreateItem(CString strCmdItem, DBFQ_CDT_INFO& tDCI)
{
	INT32 nChkIdx = 0;	
	CTokenQuery Token(strCmdItem, strCmdItem.GetLength(), ' ');
	
	CString strColName = Token.NextToken();		strColName.Trim();
	CString strColOption = Token.RemainValue();
	strColOption.MakeLower();
	LPCTSTR lpColOption = strColOption;

	tDCI.nOption = 0;
	tDCI.strName = strColName;

	if(strColOption.Find("uint16") != -1)
	{
		tDCI.nType = ASI_DBF_MEMBER_TYPE_UINT16;

		if(strColOption.Find("auto_increment") != -1)	tDCI.nOption |= ASI_DBF_COL_OPTION_FLAG_AUTO_INC;
		if(strColOption.Find("primary") != -1)			tDCI.nOption |= ASI_DBF_COL_OPTION_FLAG_PRI_KEY;

		INT32 nDefaultRtn = strColOption.Find("default");
		if(nDefaultRtn != -1)
		{
			nDefaultRtn += strlen_ext("default ");
			CTokenQuery TokenDef(&(lpColOption[nDefaultRtn]), strColOption.GetLength() - nDefaultRtn, ' ');			
			tDCI.nDefault = ConvertOnlyNumberFromStr(TokenDef.NextToken());
		}
	}
	else if(strColOption.Find("integer(11)") != -1 || strColOption.Find("uint32") != -1)
	{
		tDCI.nType = ASI_DBF_MEMBER_TYPE_UINT32;

		if(strColOption.Find("auto_increment") != -1)	tDCI.nOption |= ASI_DBF_COL_OPTION_FLAG_AUTO_INC;
		if(strColOption.Find("primary") != -1)			tDCI.nOption |= ASI_DBF_COL_OPTION_FLAG_PRI_KEY;

		INT32 nDefaultRtn = strColOption.Find("default");
		if(nDefaultRtn != -1)
		{
			nDefaultRtn += strlen_ext("default ");
			CTokenQuery TokenDef(&(lpColOption[nDefaultRtn]), strColOption.GetLength() - nDefaultRtn, ' ');			
			tDCI.nDefault = ConvertOnlyNumberFromStr(TokenDef.NextToken());
		}
	}
	else if(strColOption.Find("integer(22)") != -1 || strColOption.Find("uint64") != -1)
	{
		tDCI.nType = ASI_DBF_MEMBER_TYPE_UINT64;

		if(strColOption.Find("auto_increment") != -1)	tDCI.nOption |= ASI_DBF_COL_OPTION_FLAG_AUTO_INC;
		if(strColOption.Find("primary") != -1)			tDCI.nOption |= ASI_DBF_COL_OPTION_FLAG_PRI_KEY;

		INT32 nDefaultRtn = strColOption.Find("default");
		if(nDefaultRtn != -1)
		{
			nDefaultRtn += strlen_ext("default ");
			CTokenQuery TokenDef(&(lpColOption[nDefaultRtn]), strColOption.GetLength() - nDefaultRtn, ' ');			
			tDCI.nDefault = ConvertOnlyNumberFromStr(TokenDef.NextToken());
		}
	}
	else if(strColOption.Find("bigint") != -1)
	{
		tDCI.nType = ASI_DBF_MEMBER_TYPE_UINT32;

		if(strColOption.Find("auto_increment") != -1)	tDCI.nOption |= ASI_DBF_COL_OPTION_FLAG_AUTO_INC;
		if(strColOption.Find("primary") != -1)			tDCI.nOption |= ASI_DBF_COL_OPTION_FLAG_PRI_KEY;

		INT32 nDefaultRtn = strColOption.Find("default");
		if(nDefaultRtn != -1)
		{
			nDefaultRtn += strlen_ext("default ");
			CTokenQuery TokenDef(&(lpColOption[nDefaultRtn]), strColOption.GetLength() - nDefaultRtn, ' ');			
			tDCI.nDefault = ConvertOnlyNumberFromStr(TokenDef.NextToken());
		}
	}
	else if(strColOption.Find("ltext") != -1 || strColOption.Find("lstring") != -1)
	{
		tDCI.nType = ASI_DBF_MEMBER_TYPE_LSTRING;	
		INT32 nDefaultRtn = strColOption.Find("default");
		if(nDefaultRtn != -1)
		{
			nDefaultRtn += strlen_ext("default \'");
			CTokenQuery TokenDef(&(lpColOption[nDefaultRtn]), strColOption.GetLength() - nDefaultRtn, '\'');

			tDCI.strDefault = TokenDef.NextToken();
		}
	}
	else if(strColOption.Find("text") != -1 || strColOption.Find("string") != -1)
	{
		tDCI.nType = ASI_DBF_MEMBER_TYPE_STRING;	
		INT32 nDefaultRtn = strColOption.Find("default");
		if(nDefaultRtn != -1)
		{
			nDefaultRtn += strlen_ext("default \'");
			CTokenQuery TokenDef(&(lpColOption[nDefaultRtn]), strColOption.GetLength() - nDefaultRtn, '\'');

			tDCI.strDefault = TokenDef.NextToken();
		}
	}
	else if(strColOption.Find("varchar") != -1)
	{
		tDCI.nType = ASI_DBF_MEMBER_TYPE_STRING;	
		INT32 nDefaultRtn = strColOption.Find("default");
		if(nDefaultRtn != -1)
		{
			nDefaultRtn += strlen_ext("default \'");
			CTokenQuery TokenDef(&(lpColOption[nDefaultRtn]), strColOption.GetLength() - nDefaultRtn, '\'');

			tDCI.strDefault = TokenDef.NextToken();
		}
	}
	else if(strColOption.Find("object") != -1 || strColOption.Find("blob") != -1)
	{
		tDCI.nType = ASI_DBF_MEMBER_TYPE_OBJECT;			
	}
	else
	{
		WriteLogE("not find create table option : [%s]", (char *)(LPCSTR)strColOption);
		return -1;
	}
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCmd_Alter(CString strCmd)
{
	CTokenQuery Token(strCmd, strCmd.GetLength());
	CString strAreaType = Token.NextToken();

	if(strAreaType.CompareNoCase("database") == 0)
	{
		CString strOpType = Token.NextToken();
		if(strOpType.CompareNoCase("recovery") == 0)
		{
			m_nQueryCmdCode = ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_DB_RECOVERY;
			DBFQ_CDT_INFO tDCI;
			m_tAlterItemList.push_back(tDCI);
		}
	}
	else
	{
		m_strTName = Token.NextTokenTableName();

		CString strOpType = Token.NextToken('=');

		if(strOpType.CompareNoCase("add") == 0)
		{
			Token.NextToken();
			m_nQueryCmdCode = ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_ADD;		
			CString strRmCmd = Token.RemainValue();	

			DBFQ_CDT_INFO tDCI;

			if(AnalysisCmd_CreateItem(strRmCmd, tDCI))
			{
				return -1;
			}
			m_tAlterItemList.push_back(tDCI);
		}		
		else if(strOpType.CompareNoCase("del") == 0)
		{
			Token.NextToken();
			m_nQueryCmdCode = ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_DEL;		
			CString strRmCmd = Token.RemainValue();	
			//strRmCmd = RemoveLastChar(strRmCmd, ';');

			DBFQ_CDT_INFO tDCI;
			tDCI.strName = strRmCmd.Trim();
			m_tAlterItemList.push_back(tDCI);
		}		
		else if(strOpType.CompareNoCase("rename") == 0)
		{
			m_nQueryCmdCode = ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_RENAME;		
			CString strOldCN = Token.NextToken();	
			if(strOldCN.CompareNoCase("column") == 0 || strOldCN.CompareNoCase("columns") == 0)
			{
				strOldCN = Token.NextToken();
			}
			CString strNewCN = Token.NextToken();	
			//strRmCmd = RemoveLastChar(strRmCmd, ';');

			WriteLogN("rename column : [%s][%s][%s]", (char *)(LPCSTR)m_strTName, (char *)(LPCSTR)strOldCN, (char *)(LPCSTR)strNewCN);

			DBFQ_CDT_INFO tDCI;
			tDCI.strName	= strOldCN.Trim();
			tDCI.strNewName	= strNewCN.Trim();
			m_tAlterItemList.push_back(tDCI);
		}		
		else if(strOpType.CompareNoCase("relen") == 0)
		{
			m_nQueryCmdCode = ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_RELEN;		
			CString strOldCN = Token.NextToken();	
			if(strOldCN.CompareNoCase("column") == 0 || strOldCN.CompareNoCase("columns") == 0)
			{
				strOldCN = Token.NextToken();
			}
			UINT32 nDefLen = Token.NextToken_UInt();	
			//strRmCmd = RemoveLastChar(strRmCmd, ';');

			WriteLogN("relen column : [%s][%s][%u]", m_strTName, strOldCN, nDefLen);

			DBFQ_CDT_INFO tDCI;
			tDCI.strName	= strOldCN.Trim();
			tDCI.nDefLen	= nDefLen;
			m_tAlterItemList.push_back(tDCI);
		}
		else if(strOpType.CompareNoCase("auto_increment") == 0)
		{
			m_nQueryCmdCode = ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_AUTOINCREMENT;		
			UINT32 nInitValue = Token.NextToken_Int();
			
			WriteLogN("auto increment column : [%s][%u]", m_strTName, nInitValue);

			DBFQ_CDT_INFO tDCI;
			tDCI.nDefault	= nInitValue;
			m_tAlterItemList.push_back(tDCI);
		}
		else
		{
			WriteLogE("invalid alter command op type : [%s]", (char *)(LPCSTR)strOpType);
			return -1;
		}
	}
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::AnalysisCmd_WhereClause(CString strCmd)
{
	CTokenQuery Token;
	CString strCmdOr = strCmd;

	while(Token.IsNextToken_WhereCdt(strCmdOr))
	{
		CString strKeyValue, strName, strValue, strNameLow, strValueLow;
		INT32 nType = 0;
		strKeyValue = Token.NextToken_WhereCdt(strCmdOr, strName, strValue, nType);
		strNameLow = strName;		strNameLow.MakeLower();		strNameLow.Trim();
		strValueLow = strValue;		strValueLow.MakeLower();	

		//WriteLogN("next token where cdt : [%s][%s][%d]", strName, strValue, nType);

		if(strNameLow.CompareNoCase(ASI_DBF_WHERE_FP_KEY_NAME) == 0)
		{
			UINT64 nDBFFp = ConvertOnlyNumberFromStr(strValue);
			m_tCdtWhereFpList.push_back(nDBFFp);
			m_tCdtWhereFpUsedMode = 1;
		}
		else
		{
			DBFQ_CDT_INFO_WHERE tDCIW;
			tDCIW.strName = strName;	tDCIW.strName.Trim();
			tDCIW.nCompType = nType;
			
			switch(nType)
			{
				case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL:
				case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_SMALL:
				case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_LARGE:
				case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_NOT_EQUAL:
				case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_SMALL:
				case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_LARGE:
				{
					if(strValueLow.Find("\'") != -1)
					{
						tDCIW.nChkType = ASI_DBF_MEMBER_TYPE_LSTRING;
						tDCIW.strChkValueS = RemoveFirstLastChar(strValue, '\'', '\'');;
					}
					else
					{
						tDCIW.nChkType = ASI_DBF_MEMBER_TYPE_UINT64;
						tDCIW.nChkValueS = ConvertOnlyNumberFromStr(strValue);
					}
					break;
				}
				case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_BETWEEN:
				{
					CString strSValue, strEValue;
					CTokenQuery TokenValue(strValue, strValue.GetLength(), ' ');
					TokenValue.NextToken();
					strSValue = TokenValue.NextTokenValue();
					TokenValue.NextToken();
					strEValue = TokenValue.NextTokenValue();
					
					if(strSValue.Find("\'") != -1)
					{
						tDCIW.nChkType = ASI_DBF_MEMBER_TYPE_LSTRING;
						tDCIW.strChkValueS = RemoveFirstLastChar(strSValue, '\'', '\'');
						tDCIW.strChkValueE = RemoveFirstLastChar(strEValue, '\'', '\'');
					}
					else
					{
						tDCIW.nChkType = ASI_DBF_MEMBER_TYPE_UINT64;
						tDCIW.nChkValueS = ConvertOnlyNumberFromStr(strSValue);
						tDCIW.nChkValueE = ConvertOnlyNumberFromStr(strEValue);
					}
					break;
				}
				default:		continue;
			}

			m_tCdtWhereList.push_back(tDCIW);
		}
	}
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::ExportCmd_SelNameTableWhere(CString strCmd, CString& strName, CString& strTable, CString& strWhere)
{
	CString strCmdLow = strCmd;
	strCmdLow.MakeLower();	

	CString strChk;
	INT32	nChkIdx = 0;
	INT32	nValueFind = 0;
	INT32	nFind = -1;
	LPCTSTR lpCmdLow = strCmdLow;

	INT32 nIdx = 0;
	while(nIdx < strCmdLow.GetLength())
	{
		CHAR chChk = lpCmdLow[nIdx++];

		if(nChkIdx != 5 && (chChk == ' ' || chChk == '\t'))
		{
			nFind = (nIdx - 1);
			nChkIdx = 1;
		}
		else if(nChkIdx == 1 && chChk == 'f')	nChkIdx += 1;
		else if(nChkIdx == 2 && chChk == 'r')	nChkIdx += 1;
		else if(nChkIdx == 3 && chChk == 'o')	nChkIdx += 1;
		else if(nChkIdx == 4 && chChk == 'm')	nChkIdx += 1;
		else if(nChkIdx == 5 && chChk == ' ')	break;
		else if(nChkIdx == 5 && chChk == '\t')	break;
		else
		{
			nFind = -1;
			nChkIdx = 0;
		}
	}

	if(nFind == -1)
	{
		strName = strCmd;
		return 0;
	}	

	strName = strCmd.Left(nFind);
	CString strRemain = strCmd.Right(strCmd.GetLength() - (nFind + 6));

	ExportCmd_WhereClause(strRemain, strTable, strWhere);
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::ExportCmd_InsNameValue(CString strCmd, CString& strName, CString& strValue)
{
	CString strCmdLow = strCmd;
	strCmdLow.MakeLower();	

	CString strChk;
	INT32	nChkIdx = 0;
	INT32	nValueFind = 0;
	INT32	nFind = -1;
	LPCTSTR lpCmdLow = strCmdLow;

	INT32 nIdx = 0;
	while(nIdx < strCmdLow.GetLength())
	{
		CHAR chChk = lpCmdLow[nIdx++];

		if(chChk == '\'')
		{
			nValueFind ^= 1;
			continue;
		}

		if(chChk == '(' || chChk == ')')
		{
			nValueFind ^= 2;
			//continue;
		}

		if(!nChkIdx && nValueFind)		continue;

		if(nChkIdx != 7 && (chChk == ' ' || chChk == '\t'|| chChk == ')'))
		{
			nFind = nIdx;
			nChkIdx = 1;
		}
		else if(nChkIdx == 1 && chChk == 'v')	nChkIdx += 1;
		else if(nChkIdx == 2 && chChk == 'a')	nChkIdx += 1;
		else if(nChkIdx == 3 && chChk == 'l')	nChkIdx += 1;
		else if(nChkIdx == 4 && chChk == 'u')	nChkIdx += 1;
		else if(nChkIdx == 5 && chChk == 'e')	nChkIdx += 1;
		else if(nChkIdx == 6 && chChk == 's')	nChkIdx += 1;
		else if(nChkIdx == 7 && chChk == ' ')	break;
		else if(nChkIdx == 7 && chChk == '\t')	break;
		else if(nChkIdx == 7 && chChk == '(')	break;
		else
		{
			nFind = -1;
			nChkIdx = 0;
		}
	}

	if(nFind == -1)
	{
		strName = strCmd;
		return 0;
	}	

	strName = strCmd.Left(nFind);
	strValue = strCmd.Right(strCmd.GetLength() - (nFind + 6));

	strName = RemoveFirstLastChar(strName, '(', ')');
	strValue = RemoveFirstLastChar(strValue, '(', ')');

	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::ExportCmd_WhereClause(CString strCmd, CString& strRemain, CString& strWhereClause)
{
	CString strCmdLow = strCmd;
	strCmdLow.MakeLower();	
	
	CString strChk;
	INT32	nChkIdx = 0;
	INT32	nValueFind = 0;
	INT32	nFind = -1;
	LPCTSTR lpCmdLow = strCmdLow;

	INT32 nIdx = 0;
	while(nIdx < strCmdLow.GetLength())
	{
		CHAR chChk = lpCmdLow[nIdx++];

		if(chChk == '\'')
		{
			nValueFind ^= 1;
			continue;
		}
		if(nValueFind)		continue;
		
		if(nChkIdx != 6 && (chChk == ' ' || chChk == '\t'))
		{
			nFind = (nIdx - 1);
			nChkIdx = 1;
		}
		else if(nChkIdx == 1 && chChk == 'w')	nChkIdx += 1;
		else if(nChkIdx == 2 && chChk == 'h')	nChkIdx += 1;
		else if(nChkIdx == 3 && chChk == 'e')	nChkIdx += 1;
		else if(nChkIdx == 4 && chChk == 'r')	nChkIdx += 1;
		else if(nChkIdx == 5 && chChk == 'e')	nChkIdx += 1;
		else if(nChkIdx == 6 && chChk == ' ')	break;
		else if(nChkIdx == 6 && chChk == '\t')	break;
		else
		{
			nFind = -1;
			nChkIdx = 0;
		}
	}

	if(nValueFind)
	{
		WriteLogE("invalid query : [%s]", (char *)(LPCSTR)strCmd);
		return -10;
	}

	if(nFind == -1)
	{
		strRemain = strCmd;
		return 0;
	}

	//nFind = strCmdLow.GetLength() - (nFind + 7);

	strRemain = strCmd.Left(nFind);
	strWhereClause = strCmd.Right(strCmd.GetLength() - (nFind + 7));

	return 0;	
}
//------------------------------------------------------------

INT32		CQueryUtil::ExportCmd_WhereBetAndStrValue(CString strCmd, CString& strSValue, CString& strEValue)
{
	INT32	nChkIdx = 0;
	INT32	nValueFind = 0;
	LPCTSTR lpCmd = strCmd;
	INT32	nValuePos = 0;

	INT32 nIdx = 0;
	while(nIdx < strCmd.GetLength())
	{
		CHAR chChk = lpCmd[nIdx++];
		
		if(chChk == '\'')
		{
			nValueFind ^= 1;
			nValuePos += 1;
			continue;
		}		
		if(nValueFind)
		{
			if(nValuePos == 1)		strSValue += chChk;
			if(nValuePos == 3)		strEValue += chChk;
		}
	}
	return 0;
}
//------------------------------------------------------------

INT32		CQueryUtil::ExportCmd_WhereBetAndNumValue(CString strCmd, UINT64& nSValue, UINT64& nEValue)
{
	CString strSValue, strEValue;
	CString strCmdLow = strCmd;		strCmdLow.MakeLower();

	INT32	nChkIdx = 0;
	INT32	nValueFind = 0;
	INT32	nValuePos = 0;
	LPCTSTR lpCmd = strCmd, lpCmdLow = strCmdLow;
	INT32	nIgnoreSpace = 0;

	INT32 nIdx = 0;
	while(nIdx < strCmd.GetLength())
	{
		CHAR chChk = lpCmd[nIdx++];

		if((chChk == ' ' || chChk == '\t'))
		{			
			if(!nIgnoreSpace)
			{
				nIgnoreSpace = 1;
			}
			continue;
		}
		else
		{
			if(nIgnoreSpace)	nValuePos += 1;

			switch(nValuePos)
			{
				case 2:		strSValue += chChk;		break;
				case 4:		strEValue += chChk;		break;
				default:	break;						
			}
			nIgnoreSpace = 0;
		}
	}

	if(strSValue.IsEmpty() || strEValue.IsEmpty())		return -1;

	nSValue = ConvertOnlyNumberFromStr(strSValue);
	nEValue = ConvertOnlyNumberFromStr(strEValue);

	return 0;
}
//------------------------------------------------------------

UINT64		CQueryUtil::ConvertOnlyNumberFromStr(CString strValue)
{
	CString strNumber;
	LPCTSTR lpValue = strValue;
	UINT64	nRtn = 0;

	INT32 nIdx = 0;
	while(nIdx < strValue.GetLength())
	{
		if(48 <= lpValue[nIdx] && lpValue[nIdx] <= 57)
		{
			strNumber += lpValue[nIdx];
		}

		nIdx++;
	}
	if(strNumber.IsEmpty() == FALSE)	nRtn = StrToInt64(strNumber);
	return nRtn;
}
//------------------------------------------------------------

CString		CQueryUtil::GetOnlyNameStr(CString strValue)
{
	CString strName;
	LPCTSTR lpValue = strValue;
	UINT64	nRtn = 0;

	INT32 nIdx = 0;
	while(nIdx < strValue.GetLength())
	{
		if(45 <= lpValue[nIdx] && lpValue[nIdx] <= 126)
		{
			strName += lpValue[nIdx];
		}

		nIdx++;
	}
	return strName;
}
//------------------------------------------------------------

CString		CQueryUtil::RemoveFirstLastChar(CString strValue, CHAR szFRmChar, CHAR szLRmChar)
{
	CString strRtn, strFRmRtn, strLRmRtn;
	LPCTSTR lpValue = strValue;
	INT32 nChecked = 0;

	{
		INT32 nIdx = 0;
		while(nIdx < strValue.GetLength())
		{
			CHAR chChk = lpValue[nIdx++];

			if(!nChecked && chChk == szFRmChar)
			{
				nChecked = 1;
				continue;
			}

			if(nChecked)	strFRmRtn += chChk;
		}
	}

	lpValue = strFRmRtn;
	nChecked = 0;

	{
		INT32 nIdx = strFRmRtn.GetLength();
		while(nIdx > 0)
		{
			nIdx--;

			if(!nChecked && lpValue[nIdx] == szLRmChar)
			{
				nChecked = 1;
				continue;
			}

			if(nChecked)	strLRmRtn += lpValue[nIdx];
		}	
	}

	lpValue = strLRmRtn;
	{
		INT32 nIdx = strLRmRtn.GetLength();
		while(nIdx > 0)
		{
			nIdx--;
			strRtn += lpValue[nIdx];
		}
	}
	return strRtn;
}
//------------------------------------------------------------

CString		CQueryUtil::RemoveLastChar(CString strValue, CHAR szLRmChar)
{
	CString strRtn, strFRmRtn, strLRmRtn;
	LPCTSTR lpValue = strValue;
	INT32 nChecked = 0;

	lpValue = strFRmRtn;
	nChecked = 0;

	{
		INT32 nIdx = strFRmRtn.GetLength();
		while(nIdx > 0)
		{
			nIdx--;

			if(!nChecked && lpValue[nIdx] == szLRmChar)
			{
				nChecked = 1;
				continue;
			}

			if(nChecked)	strLRmRtn += lpValue[nIdx];
		}	
	}

	lpValue = strLRmRtn;
	{
		INT32 nIdx = strLRmRtn.GetLength();
		while(nIdx > 0)
		{
			nIdx--;
			strRtn += lpValue[nIdx];
		}
	}
	return strRtn;
}
//------------------------------------------------------------

CString	CQueryUtil::ReverseString(LPCTSTR lpInput, INT32 nLen)
{
	if(!nLen)	nLen = strlen_ext(lpInput);

	INT32 nSIdx = 0;
	INT32 nEIdx = (nLen - 1);

	while(nSIdx < nEIdx && nSIdx != nEIdx)
	{
		char chTemp = lpInput[nSIdx];
		((LPTSTR)lpInput)[nSIdx] = lpInput[nEIdx];
		((LPTSTR)lpInput)[nEIdx] = chTemp;

		nSIdx++;	nEIdx--;
	}
	return lpInput;
}
//------------------------------------------------------------

