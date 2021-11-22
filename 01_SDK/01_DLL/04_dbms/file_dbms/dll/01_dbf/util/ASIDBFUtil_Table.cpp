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
#include "asi_glb_w32_handle.h"
#include "asi_glb_w32_file.h"
#include "ASIDBFUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ReadDBF_TableList()
{
	m_tDBFTableSearchList.clear();

	{
		DWORD dwHigh, dwLow;
		dwLow = GetFileSize(GetFileHandle(ASI_DBF_FILE_HANDLE_READ), &dwHigh);
		if(MAKEUINT64(dwLow, dwHigh) == sizeof(DBF_DATABASE))
		{
			return 0;
		}
	}

	SetFilePointerEx(ASI_DBF_FILE_HANDLE_READ, sizeof(DBF_DATABASE));
	DBF_TABLE_SEARCH tDTS;
	do 
	{
		if(SetER(ReadDBF(&tDTS.tDT, sizeof(DBF_TABLE))))
		{
			WriteLogN("read table header fail : [%.8x]", GetER());
			return GetER();
		}

		tDTS.m_tColNameIdxMap.clear();
		tDTS.tDT.tHInfo.nCheckItemFP = 0;
		
		MakeDBF_TableColIdx(&tDTS);
		SetTableItemHeaderSize(&tDTS);
		SetTableAutoItem(&tDTS);

		m_tDBFTableSearchList.push_back(tDTS);

		if(tDTS.tDT.tHInfo.nTailTableFP == 0)	break;
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_READ, tDTS.tDT.tHInfo.nTailTableFP);
	} while (TRUE);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ReloadDBF_TableList()
{
	ReadDBF_TableList();
	
	if(SetER(ReadDBF_TableList()))
	{
		WriteLogE("ReadDBF Table List Fail [%.8x]", GetER());
		return GetER();
	}

	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0, FILE_END);
	m_nDBFLastFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::MakeDBF_TableColIdx(PDBF_TABLE_SEARCH pDTS)
{
	UINT32 nColIdx = 0;
	while(pDTS->tDT.tHInfo.nColNumber > nColIdx)
	{
		PDBF_TABLE_COLUMN pdtc = &(pDTS->tDT.tColInfo[nColIdx]);

		pDTS->_add_idx(pdtc->szColName, nColIdx);
		nColIdx++;
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::MakeDBF_TablePriFPList(PDBF_TABLE_SEARCH pDTS)
{
	INT32 nPriKeyIdx = -1;
	UINT32 nColIdx = 0;
	if(pDTS == NULL)
		return -1;
	while(pDTS->tDT.tHInfo.nColNumber > nColIdx)
	{
		PDBF_TABLE_COLUMN pdtc = &(pDTS->tDT.tColInfo[nColIdx]);

		if(pdtc->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY)
		{
			nPriKeyIdx = nColIdx;
			break;
		}		
		nColIdx++;
	}

	if(nPriKeyIdx < 0)	return 0;

	CASIDBFMember* pMember = GetMember();

	{
		FirstDBF_Item(pDTS);
		while(NextDBF_Item() == 0)
		{
			PDBF_ITEM_RTN pdir = GetField(nPriKeyIdx);
			if(!pdir)	continue;

			switch(pdir->nRtnType)
			{
				case ASI_DBF_MEMBER_TYPE_UINT16:
					if(pdir->pRtnUINT16 != NULL)
						pDTS->_apply_fp(*(pdir->pRtnUINT16), GetFieldFP());
					break;
				case ASI_DBF_MEMBER_TYPE_UINT32:
					if(pdir->pRtnUINT32 != NULL)
						pDTS->_apply_fp(*(pdir->pRtnUINT32), GetFieldFP());
					break;
				case ASI_DBF_MEMBER_TYPE_UINT64:
					if(pdir->pRtnUINT64 != NULL)
						pDTS->_apply_fp(*(pdir->pRtnUINT64), GetFieldFP());
					break;
				case ASI_DBF_MEMBER_TYPE_STRING:
					if(pdir->szRtnString != NULL)
						pDTS->_apply_fp(CString(pdir->szRtnString, pdir->nRtnLen), GetFieldFP());
					break;
				case ASI_DBF_MEMBER_TYPE_LSTRING:
					if(pdir->szRtnString != NULL)
						pDTS->_apply_fp(CString(pdir->szRtnString, pdir->nRtnLen), GetFieldFP());
					break;
				case ASI_DBF_MEMBER_TYPE_OBJECT:
					break;
			}
		}
		FreeCommonBuff(&(pMember->m_pCurItemBuff));
	}

	return 0;
}
//---------------------------------------------------------------------

PDBF_TABLE_SEARCH	CASIDBFUtil::FirstDBF_Table()
{
	TListDBFTableSearchItor begin = m_tDBFTableSearchList.begin();
	if(begin != m_tDBFTableSearchList.end())	return &(*begin);
	return NULL;
}
//---------------------------------------------------------------------

PDBF_TABLE_SEARCH	CASIDBFUtil::LastDBF_Table()
{
	TListDBFTableSearchRItor begin = m_tDBFTableSearchList.rbegin();
	if(begin != m_tDBFTableSearchList.rend())	return &(*begin);
	return NULL;
}
//---------------------------------------------------------------------

PDBF_TABLE_SEARCH	CASIDBFUtil::FindDBF_Table(CString strTName)
{
	TListDBFTableSearchItor begin, end;
	begin = m_tDBFTableSearchList.begin();	end = m_tDBFTableSearchList.end();
	for(begin; begin != end; begin++)
	{
		if(begin->tDT.tHInfo.nDelFlag)	continue;
		if(strTName.CompareNoCase(begin->tDT.tHInfo.szName) == 0)	return &(*begin);
	}
	return NULL;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::GetDBF_TableInfo(CString strTName, PDBF_TABLE_INFO_RTN pDTIR)
{
	TListDBFTableSearchItor begin, end;
	begin = m_tDBFTableSearchList.begin();	end = m_tDBFTableSearchList.end();
	for(begin; begin != end; begin++)
	{
		if(begin->tDT.tHInfo.nDelFlag)	continue;

		if(strTName.CompareNoCase(begin->tDT.tHInfo.szName))	continue;
				
		pDTIR->nTID			= begin->tDT.tHInfo.nTID;
		pDTIR->nTType		= begin->tDT.tHInfo.nTType;
		pDTIR->nColNumber	= begin->tDT.tHInfo.nColNumber;
		pDTIR->nItemCnt		= begin->tDT.tHInfo.nItemCnt;
		pDTIR->nItemTCnt	= begin->tDT.tHInfo.nItemTCnt;
		pDTIR->nLastID		= begin->tDT.tHInfo.nLastID;
		pDTIR->nModifyDate	= begin->tDT.tHInfo.nModifyDate;
		pDTIR->nSchemaVer	= begin->tDT.tHInfo.nSchemaVer;
		pDTIR->szName		= begin->tDT.tHInfo.szName;
		
		return 0;
	}
	return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::GetDBF_TableInfoList(PDBF_TABLE_INFO_RTN pInfoBuffList, UINT32 nBuffLen, PUINT32 pnTableNum)
{
	*pnTableNum = 0;
	INT32 nTbIdx = 0;

	TListDBFTableSearchItor begin, end;
	begin = m_tDBFTableSearchList.begin();	end = m_tDBFTableSearchList.end();
	for(begin; begin != end; begin++)
	{
		if(begin->tDT.tHInfo.nDelFlag)	continue;
		
		if(!pInfoBuffList)
		{
			*pnTableNum += 1;
			continue;
		}

		pInfoBuffList[nTbIdx].nTID			= begin->tDT.tHInfo.nTID;
		pInfoBuffList[nTbIdx].nTType		= begin->tDT.tHInfo.nTType;
		pInfoBuffList[nTbIdx].nColNumber	= begin->tDT.tHInfo.nColNumber;
		pInfoBuffList[nTbIdx].nItemCnt		= begin->tDT.tHInfo.nItemCnt;
		pInfoBuffList[nTbIdx].nItemTCnt		= begin->tDT.tHInfo.nItemTCnt;
		pInfoBuffList[nTbIdx].nLastID		= begin->tDT.tHInfo.nLastID;
		pInfoBuffList[nTbIdx].nModifyDate	= begin->tDT.tHInfo.nModifyDate;
		pInfoBuffList[nTbIdx].nSchemaVer	= begin->tDT.tHInfo.nSchemaVer;
		pInfoBuffList[nTbIdx].szName		= begin->tDT.tHInfo.szName;
		nTbIdx++;
		*pnTableNum += 1;

		if(nTbIdx == nBuffLen)	break;
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::GetDBF_TableColumnList(CString strTName, PDBF_TABLE_COLUMN_RTN pColBuffList, UINT32 nBuffLen, PUINT32 pnColNum)
{
	*pnColNum	= 0;
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	UINT32 nColIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber > nColIdx)
	{
		PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nColIdx]);

		if(!pColBuffList)
		{
			*pnColNum += 1;
			nColIdx++;
			continue;
		}

		pColBuffList[nColIdx].nColOption	= pdtc->nColOption;
		pColBuffList[nColIdx].nDefaultValue	= pdtc->nDefaultValue;
		pColBuffList[nColIdx].nInitStrLen	= pdtc->nInitStrLen;
		pColBuffList[nColIdx].nMemberType	= pdtc->nMemberType;
		pColBuffList[nColIdx].szColName		= pdtc->szColName;
		nColIdx++;
		*pnColNum += 1;

		if(nColIdx == nBuffLen)	break;
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::GetDBF_TableColumn(CString strTName, CString strColName, PDBF_TABLE_COLUMN_RTN pDTCR)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	UINT32 nColIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber > nColIdx)
	{
		PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nColIdx]);
		if(!pdtc)
		{
			WriteLogE("not find table column : [%s][%d]", (char *)(LPCSTR)strTName, nColIdx);
			return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE_COL;
		}

		if(strColName.CompareNoCase(pdtc->szColName) == 0)	
		{
			pDTCR->nColIdx			= nColIdx;
			pDTCR->nColOption		= pdtc->nColOption;
			pDTCR->nDefaultValue	= pdtc->nDefaultValue;
			pDTCR->nInitStrLen		= pdtc->nInitStrLen;
			pDTCR->nMemberType		= pdtc->nMemberType;
			pDTCR->szColName		= pdtc->szColName;
			return 0;
		}
		nColIdx++;
	}
	WriteLogE("not find table column [%s]", (char *)(LPCSTR)strColName);
	return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE_COL;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::GetDBF_TableColumnIdx(CString strTName, CString strColName, PUINT32 pnTableColIdx)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	UINT32 nColIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber > nColIdx)
	{
		PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nColIdx]);
		if(!pdtc)
		{
			WriteLogE("not find table column : [%s][%d]", nColIdx);
			return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE_COL;
		}

		if(strColName.CompareNoCase(pdtc->szColName) == 0)	
		{
			*pnTableColIdx = nColIdx;
			return 0;
		}
		nColIdx++;
	}
	WriteLogE("not find table column [%s]", (char *)(LPCSTR)strColName);
	return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE_COL;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::GetTbItemPriKeyFP(CString strTName, CString strColName, UINT64 nPriKey, CString strPriKey, PUINT64 pnFP)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	if(pnFP == NULL)
	{
		WriteLogE("invalid argument : FP is NULL");
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	UINT32 nColIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber > nColIdx)
	{
		PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nColIdx]);
		if(!pdtc)
		{
			WriteLogE("not find table column : [%s][%d]", nColIdx);
			return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE_COL;
		}

		if(strColName.CompareNoCase(pdtc->szColName) != 0)	
		{
			nColIdx++;
			continue;
		}

		if((pdtc->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY) == 0)
		{
			return ASI_DBF_ERROR_TYPE_V2_NOT_PRIMARY_KEY;
		}


		switch(pdtc->nMemberType)
		{
			case ASI_DBF_MEMBER_TYPE_UINT16:				
			case ASI_DBF_MEMBER_TYPE_UINT32:
			case ASI_DBF_MEMBER_TYPE_UINT64:
			{
				*pnFP = pdts->_find_fp(nPriKey);
				break;
			}
			case ASI_DBF_MEMBER_TYPE_STRING:
			{
				*pnFP = pdts->_find_fp(strPriKey);
				break;
			}
			case ASI_DBF_MEMBER_TYPE_LSTRING:
			{
				*pnFP = pdts->_find_fp(strPriKey);
				break;
			}
			case ASI_DBF_MEMBER_TYPE_OBJECT:
			{
				*pnFP = pdts->_find_fp(strPriKey);
				break;
			}
		}

		if(*pnFP == 0)
		{
			return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_PRIMARY_KEY;
		}
		return 0;
	}
	WriteLogE("not find table column [%s]", (char *)(LPCSTR)strColName);
	return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE_COL;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::GetTbItemPriKeyFPList(CString strTName, CString strColName, UINT64 nChkValueS, UINT64 nChkValueE, CString strChkValueS, CString strChkValueE, UINT32 nCompType, PUINT64* ppnFPList, PUINT32 pnFPListSize)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	if(ppnFPList == NULL)
	{
		WriteLogE("invalid argument : FPList is NULL");
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	if(pdts->m_tPKeyFPMap.size() != pdts->tDT.tHInfo.nItemCnt && pdts->m_tSPKeyFPMap.size() != pdts->tDT.tHInfo.nItemCnt)
	{
		return ASI_DBF_ERROR_TYPE_V2_NO_MORE_DATA;
	}

	TListID64 tFPList;
	UINT32 nColIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber > nColIdx)
	{
		PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nColIdx]);
		if(!pdtc)
		{
			WriteLogE("not find table column : [%s][%d]", nColIdx);
			return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE_COL;
		}

		if(strColName.CompareNoCase(pdtc->szColName) != 0)	
		{
			nColIdx++;
			continue;
		}

		if((pdtc->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY) == 0)
		{
			return ASI_DBF_ERROR_TYPE_V2_NOT_PRIMARY_KEY;
		}

		
		UINT64 nCurFP;
		switch(pdtc->nMemberType)
		{
			case ASI_DBF_MEMBER_TYPE_UINT16:				
			case ASI_DBF_MEMBER_TYPE_UINT32:
			case ASI_DBF_MEMBER_TYPE_UINT64:
			{
				switch(nCompType)
				{
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL:
					{
						nCurFP = pdts->_find_fp(nChkValueS);
						if(nCurFP)	tFPList.push_back(nCurFP);
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_SMALL:
					{
						TMapID64Itor begin, end;
						begin = pdts->m_tPKeyFPMap.begin();	end = pdts->m_tPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first >= nChkValueS)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_LARGE:
					{
						TMapID64Itor begin, end;
						begin = pdts->m_tPKeyFPMap.begin();	end = pdts->m_tPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first <= nChkValueS)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_NOT_EQUAL:
					{
						TMapID64Itor begin, end;
						begin = pdts->m_tPKeyFPMap.begin();	end = pdts->m_tPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first == nChkValueS)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_SMALL:
					{
						TMapID64Itor begin, end;
						begin = pdts->m_tPKeyFPMap.begin();	end = pdts->m_tPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first > nChkValueS)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_LARGE:
					{
						TMapID64Itor begin, end;
						begin = pdts->m_tPKeyFPMap.begin();	end = pdts->m_tPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first < nChkValueS)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_BETWEEN:
					{
						TMapID64Itor begin, end;
						begin = pdts->m_tPKeyFPMap.begin();	end = pdts->m_tPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first < nChkValueS || nChkValueE < begin->first)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
				}
				break;
			}
			case ASI_DBF_MEMBER_TYPE_STRING:
			case ASI_DBF_MEMBER_TYPE_LSTRING:
			{
				switch(nCompType)
				{
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL:
					{
						nCurFP = pdts->_find_fp(strChkValueS);
						if(nCurFP)	tFPList.push_back(nCurFP);
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_SMALL:
					{
						TMapCStrID64Itor begin, end;
						begin = pdts->m_tSPKeyFPMap.begin();	end = pdts->m_tSPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first >= strChkValueS)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_LARGE:
					{
						TMapCStrID64Itor begin, end;
						begin = pdts->m_tSPKeyFPMap.begin();	end = pdts->m_tSPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first <= strChkValueS)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_NOT_EQUAL:
					{
						TMapCStrID64Itor begin, end;
						begin = pdts->m_tSPKeyFPMap.begin();	end = pdts->m_tSPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first == strChkValueS)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_SMALL:
					{
						TMapCStrID64Itor begin, end;
						begin = pdts->m_tSPKeyFPMap.begin();	end = pdts->m_tSPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first > strChkValueS)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_LARGE:
					{
						TMapCStrID64Itor begin, end;
						begin = pdts->m_tSPKeyFPMap.begin();	end = pdts->m_tSPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first < strChkValueS)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
					case ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_BETWEEN:
					{
						TMapCStrID64Itor begin, end;
						begin = pdts->m_tSPKeyFPMap.begin();	end = pdts->m_tSPKeyFPMap.end();
						for(begin; begin != end; begin++)
						{
							if(begin->first < strChkValueS || strChkValueE < begin->first)	continue;

							tFPList.push_back(begin->second);
						}
						break;
					}
				}
				break;
			}
		}

		if(tFPList.size() == 0)
		{
			return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_PRIMARY_KEY;
		}

		{
			UINT32 nMemAllocSize = UINT32(sizeof(UINT64) * (tFPList.size() + 1));
			*ppnFPList = (PUINT64)malloc(nMemAllocSize);
			if(*ppnFPList == NULL)
			{
				WriteLogE("malloc fail when convert pri key to fp [%s][%u]", (char *)(LPCSTR)strColName, nMemAllocSize);
				return ASI_DBF_ERROR_TYPE_MEM_ALLOC_FAIL;
			}
			memset(*ppnFPList, 0, nMemAllocSize);
			*pnFPListSize = (UINT32)tFPList.size();
			
			INT32 nCopyIdx = 0;
			TListID64Itor begin, end;
			begin = tFPList.begin();	end = tFPList.end();
			for(begin; begin != end; begin++)
			{
				(*ppnFPList)[nCopyIdx] = *begin;
				nCopyIdx += 1;
			}
		}
		return 0;
	}
	WriteLogE("not find table column [%s]", (char *)(LPCSTR)strColName);
	return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE_COL;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

UINT32		CASIDBFUtil::GetTableItemHeaderSize(PDBF_TABLE_SEARCH pdts)
{
	if(!pdts)	return 0;

	UINT32 nHLen = 0;
	UINT32 nItemIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
	{
		DBF_TABLE_COLUMN tDTC = pdts->tDT.tColInfo[nItemIdx];
		if(tDTC.nMemberType == ASI_DBF_MEMBER_TYPE_LSTRING)	nHLen += 4;
		else												nHLen += 2;

		nItemIdx += 1;
	}
	
	return nHLen;
}
//---------------------------------------------------------------------

UINT32		CASIDBFUtil::SetTableItemHeaderSize(PDBF_TABLE_SEARCH pdts)
{
	if(!pdts)	return 0;

	pdts->m_nItemHLen = GetTableItemHeaderSize(pdts);

	return pdts->m_nItemHLen;
}
//---------------------------------------------------------------------

UINT32		CASIDBFUtil::SetTableAutoItem(PDBF_TABLE_SEARCH pdts)
{
	if(!pdts)	return 0;

	pdts->m_tAutoColList.clear();

	UINT32 nItemIdx = 0;
	UINT32 nBuffPos = 0;
	while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
	{
		if((pdts->tDT.tColInfo[nItemIdx].nColOption & ASI_DBF_COL_OPTION_FLAG_AUTO_INC))
		{
			pdts->m_tAutoColList.push_back(nItemIdx);
		}
		nItemIdx += 1;
	}
	return pdts->m_tAutoColList.size();
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::CompareDBF_TableHeader(PDBF_TABLE_SEARCH pdts_src, PDBF_TABLE_SEARCH pdts_tar)
{
	if(pdts_src->tDT.tHInfo.nColNumber != pdts_tar->tDT.tHInfo.nColNumber)		return 1;

	UINT32 nChkIdx = 0;
	while(nChkIdx < pdts_src->tDT.tHInfo.nColNumber)
	{
		if(CompareDBF_TableHeaderCol(&(pdts_src->tDT.tColInfo[nChkIdx]), &(pdts_tar->tDT.tColInfo[nChkIdx])))	return 1;

		nChkIdx++;
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::CompareDBF_TableHeaderCol(PDBF_TABLE_COLUMN pdfhc_src, PDBF_TABLE_COLUMN pdfhc_tar)
{
	if(pdfhc_src->nMemberType != pdfhc_tar->nMemberType)			return -1;

	if(pdfhc_src->nMemberType == ASI_DBF_MEMBER_TYPE_STRING || 
		pdfhc_src->nMemberType == ASI_DBF_MEMBER_TYPE_LSTRING ||
		pdfhc_src->nMemberType == ASI_DBF_MEMBER_TYPE_OBJECT)
	{
		if(pdfhc_src->nInitStrLen != pdfhc_tar->nInitStrLen)		return -1;
	}	

	if(_stricmp(pdfhc_src->szColName, pdfhc_tar->szColName))		return -1;
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::CreateDBF_Table(PDBF_TABLE_SEARCH pdts)
{
	if(!GetFileHandle(ASI_DBF_FILE_HANDLE_WRITE))		return ASI_DBF_ERROR_TYPE_FILE_HANDLE_NULL;
	
	PDBF_TABLE_SEARCH pPreTabel = LastDBF_Table();
	if(pPreTabel)
	{
		pdts->tDT.tHInfo.nHeadTableFP = pPreTabel->tDT.nCurTableFP;
		pPreTabel->tDT.tHInfo.nTailTableFP = m_nDBFLastFP;
	}

	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, m_nDBFLastFP);
	pdts->tDT.nCurTableFP		= m_nDBFLastFP;
	pdts->tDT.tHInfo.nTID		= m_tDB.tInfo.nLastTID + 1;

	if(SetER(WriteDBF(&(pdts->tDT), sizeof(DBF_TABLE))))
	{
		WriteLogE("CreateDBF_Table fail [%.8x]", GetER());
		return GetER();
	}	

	if(pPreTabel)
	{
		UpdateDBF_Table(pPreTabel);
	}

	MakeDBF_TableColIdx(pdts);
	SetTableItemHeaderSize(pdts);
	SetTableAutoItem(pdts);

	m_tDBFTableSearchList.push_back(*pdts);

	{
		m_tDB.tInfo.nLastTID += 1;
		UpdateDBF_DI();
	}

	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0, FILE_END);
	m_nDBFLastFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::UpdateDBF_Table(PDBF_TABLE_SEARCH pdts)
{
	UINT64 nOldFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdts->tDT.nCurTableFP);

	if(SetER(WriteDBF(&(pdts->tDT), sizeof(DBF_TABLE))))
	{
		WriteLogE("UpdateDBF_Table fail [%.8x]", GetER());
	}
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nOldFP);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::DropDBF_Table(CString strTName)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}
	
	pdts->tDT.tHInfo.nDelFlag = 1;
	return UpdateDBF_Table(pdts);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::RenameDBF_Table(CString strTName, CString strNewTName)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	memset(pdts->tDT.tHInfo.szName, 0, ASI_DBF_TABLE_NAME_MAX_LEN);
	strncpy(pdts->tDT.tHInfo.szName, strNewTName, min(strNewTName.GetLength(), ASI_DBF_TABLE_NAME_MAX_LEN - 1));
	return UpdateDBF_Table(pdts);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::PreSetDBF_TableColumn(CString strTName, CString strColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption)
{
	DBF_TABLE_COLUMN dtc;
	dtc.nColOption		= nColOption;
	dtc.nDefaultValue	= nDefaultValue;
	dtc.nInitStrLen	= nInitStrLen;
	dtc.nMemberType	= nMemberType;
	strncpy(dtc.szColName, strColName, min(strColName.GetLength(), ASI_DBF_MAX_LEN_TYPE_COL_NAME - 1));

	CASIDBFMember* pMember = GetMember();
	
	TMapTColumnSetValueListItor find = pMember->m_tTColumSetValueListMap.find(strTName);
	if(find == pMember->m_tTColumSetValueListMap.end())
	{
		TListTColumSetValue tTColumnSetValueList;
		pMember->m_tTColumSetValueListMap[strTName]= tTColumnSetValueList;
		find = pMember->m_tTColumSetValueListMap.find(strTName);
	}

	find->second.push_back(dtc);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::PreSetDBF_TableItem(CString strTName, CString strColName, PVOID lpBuff, UINT32 nBufLen)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName); 
	if(!pdts)
	{
		WriteLogE("not find table name (pre set dbf table item) : [%s]", (char *)(LPCSTR)strTName);
		return -1;
	}
	
	PDBF_TABLE_COLUMN pdtc = FindFieldColum(pdts, strColName);
	if(!pdtc)
	{
		WriteLogE("not find column name (pre set dbf table item) : [%s][%s]", (char *)(LPCSTR)strTName, (char *)(LPCSTR)strColName);
		return -1;
	}

	DBF_TABLE_ITEM_SET_VALUE dfisv;

	switch(pdtc->nMemberType)
	{
		case ASI_DBF_MEMBER_TYPE_UINT16:		dfisv.pUINT16	= (PUINT16)lpBuff;		break;
		case ASI_DBF_MEMBER_TYPE_UINT32:		dfisv.pUINT32	= (PUINT32)lpBuff;		break;
		case ASI_DBF_MEMBER_TYPE_UINT64:		dfisv.pUINT64	= (PUINT64)lpBuff;		break;
		case ASI_DBF_MEMBER_TYPE_STRING:
		case ASI_DBF_MEMBER_TYPE_LSTRING:
		{
			dfisv.pString = (LPCTSTR)lpBuff;
			dfisv.nBuffLen = nBufLen;
			break;
		}
		case ASI_DBF_MEMBER_TYPE_OBJECT:
		{
			dfisv.lpObject = lpBuff;
			dfisv.nBuffLen = nBufLen;
			break;
		}
		default:
		{
			WriteLogE("not define member type pre set dbf table item : [%d]", pdtc->nMemberType);
			return -2;
			break;
		}
	}	

	//WriteLogN("pre set dbf table item : [%d][%s]", pdtc->nMemberType, dfisv.strString);

	CASIDBFMember* pMember = GetMember();
	TMapTItemSetValueMapItor find = pMember->m_tTItemSetValueMapMap.find(strTName);
	if(find == pMember->m_tTItemSetValueMapMap.end())
	{
		TMapTItemSetValue tTItemSetValueMap;
		pMember->m_tTItemSetValueMapMap[strTName] = tTItemSetValueMap;
		find = pMember->m_tTItemSetValueMapMap.find(strTName);
	}

	find->second[strColName] = dfisv;
	return 0;
}
//---------------------------------------------------------------------

// INT32		CASIDBFUtil::PreSetDBF_TableItem(CString strTName, CString strColName, PVOID lpBuff, UINT32 nBufLen)
// {
// 	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName); 
// 	if(!pdts)
// 	{
// 		WriteLogE("not find table name (pre set dbf table item) : [%s]", strTName);
// 		return -1;
// 	}
// 	
// 	PDBF_TABLE_COLUMN pdtc = FindFieldColum(pdts, strColName);
// 	if(!pdtc)
// 	{
// 		WriteLogE("not find column name (pre set dbf table item) : [%s][%s]", strTName, strColName);
// 		return -1;
// 	}
// 
// 	DBF_TABLE_ITEM_SET_VALUE dfisv;
// 
// 	switch(pdtc->nMemberType)
// 	{
// 		case ASI_DBF_MEMBER_TYPE_UINT16:		memcpy(&(dfisv.nUINT16), lpBuff, sizeof(UINT16));		break;
// 		case ASI_DBF_MEMBER_TYPE_UINT32:		memcpy(&(dfisv.nUINT32), lpBuff, sizeof(UINT32));		break;
// 		case ASI_DBF_MEMBER_TYPE_UINT64:		memcpy(&(dfisv.nUINT64), lpBuff, sizeof(UINT64));		break;
// 		case ASI_DBF_MEMBER_TYPE_STRING:		dfisv.strString = (LPTSTR)lpBuff;						break;
// 		case ASI_DBF_MEMBER_TYPE_LSTRING:		dfisv.strString = (LPTSTR)lpBuff;						break;
// 		case ASI_DBF_MEMBER_TYPE_OBJECT:
// 		{
// 			if(nBufLen == 0 || lpBuff == NULL)		break;
// 			
// 			dfisv.lpObject = (PVOID)malloc(nBufLen + 2);
// 			dfisv.nObjLen = nBufLen;
// 			memset(dfisv.lpObject, 0, nBufLen + 2);
// 			memcpy(dfisv.lpObject, lpBuff, nBufLen);
// 			break;
// 		}
// 		default:
// 		{
// 			WriteLogE("not define member type pre set dbf table item : [%d]", pdtc->nMemberType);
// 			return -2;
// 			break;
// 		}
// 	}	
// 
// 	//WriteLogN("pre set dbf table item : [%d][%s]", pdtc->nMemberType, dfisv.strString);
// 
// 	CASIDBFMember* pMember = GetMember();
// 	TMapTItemSetValueMapItor find = pMember->m_tTItemSetValueMapMap.find(strTName);
// 	if(find == pMember->m_tTItemSetValueMapMap.end())
// 	{
// 		TMapTItemSetValue tTItemSetValueMap;
// 		pMember->m_tTItemSetValueMapMap[strTName] = tTItemSetValueMap;
// 		find = pMember->m_tTItemSetValueMapMap.find(strTName);
// 	}
// 
// 	find->second[strColName] = dfisv;
// 	return 0;
// }
// //---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ForceCreateDBF_Table(CString strTName)
{
	CASIDBFMember* pMember = GetMember();

	DBF_TABLE_SEARCH dts;
	TMapTColumnSetValueListItor find = pMember->m_tTColumSetValueListMap.find(strTName);
	if(find == pMember->m_tTColumSetValueListMap.end())	
	{
		WriteLogE("can not force-create table because not find table name [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}
	strncpy(dts.tDT.tHInfo.szName, strTName, min(strTName.GetLength(), ASI_DBF_TABLE_NAME_MAX_LEN - 1));

	{
		TListTColumSetValueItor begin, end;
		begin = find->second.begin();	end = find->second.end();
		for(begin; begin != end; begin++)
		{
			dts.tDT.tColInfo[dts.tDT.tHInfo.nColNumber] = *begin;
			dts.tDT.tHInfo.nColNumber += 1;
		}
		find->second.clear();

		SetTableItemHeaderSize(&dts);
		SetTableAutoItem(&dts);
	}

	if(FindDBF_Table(strTName))
	{
		if(SetER(CheckDBF_Table(&dts)))
		{
			WriteLogE("force-create : CheckDBF_Table fail [%.8x]", GetER());
			return GetER();
		}
	}
	else
	{
		dts.tDT.tHInfo.nTType	= ASI_DBF_TABLE_TYPE_USER;
		if(SetER(CreateDBF_Table(&dts)))
		{
			WriteLogE("force-create : CreateDBF_Table fail [%.8x]", GetER());
			return GetER();
		}
	}	
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ModifyDBF_TableAddColumn(CString strTName, CString strColName, UINT16 nMemberType, UINT64 nDefaultValue, UINT16 nInitStrLen, UINT32 nColOption)
{
	DBF_TABLE_SEARCH dts;
	DBF_TABLE_COLUMN dtc;
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)	
	{
		WriteLogE("add column in table fail : not find table [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	strncpy(dts.tDT.tHInfo.szName, pdts->tDT.tHInfo.szName, strlen(pdts->tDT.tHInfo.szName));

	INT32 nCIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber != nCIdx)
	{
		dts.tDT.tColInfo[nCIdx] = pdts->tDT.tColInfo[nCIdx];
		dts.tDT.tHInfo.nColNumber += 1;	
		nCIdx++;
	}

	{
		dtc.nColOption		= nColOption;
		dtc.nDefaultValue	= nDefaultValue;
		dtc.nInitStrLen		= nInitStrLen;
		dtc.nMemberType		= nMemberType;
		strncpy(dtc.szColName, strColName, min(strColName.GetLength(), ASI_DBF_MAX_LEN_TYPE_COL_NAME - 1));
	}

	dts.tDT.tColInfo[nCIdx]	= dtc;
	dts.tDT.tHInfo.nColNumber += 1;

	SetTableItemHeaderSize(&dts);
	SetTableAutoItem(&dts);

	if(SetER(CheckDBF_Table(&dts)))
	{
		WriteLogE("ModifyDBF_TableAddColumn fail [%.8x]", GetER());
		return GetER();
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ModifyDBF_TableDelColumn(CString strTName, CString strColName)
{
	DBF_TABLE_SEARCH dts;
	DBF_TABLE_COLUMN dtc;
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)	
	{
		WriteLogE("del column in table fail : not find table [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	strncpy(dts.tDT.tHInfo.szName, pdts->tDT.tHInfo.szName, strlen(pdts->tDT.tHInfo.szName));

	INT32 nOCIdx = 0;
	INT32 nNCIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber != nOCIdx)
	{
		if(strColName.CompareNoCase(pdts->tDT.tColInfo[nOCIdx].szColName) == 0)
		{
			nOCIdx++;
			continue;
		}

		dts.tDT.tColInfo[nNCIdx] = pdts->tDT.tColInfo[nOCIdx];
		dts.tDT.tHInfo.nColNumber += 1;	
		nNCIdx++;
		nOCIdx++;
	}

	SetTableItemHeaderSize(&dts);
	SetTableAutoItem(&dts);
	
	if(SetER(CheckDBF_Table(&dts)))
	{
		WriteLogE("ModifyDBF_TableDelColumn fail [%.8x]", GetER());
		return GetER();
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ModifyDBF_TableRenameColumn(CString strTName, CString strColName, CString strNewColName)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)	
	{
		WriteLogE("del column in table fail : not find table [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	INT32 nCIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber != nCIdx)
	{
		if(strColName.CompareNoCase(pdts->tDT.tColInfo[nCIdx].szColName) == 0)
		{
			memset(pdts->tDT.tColInfo[nCIdx].szColName, 0, ASI_DBF_MAX_LEN_TYPE_COL_NAME);
			strncpy(pdts->tDT.tColInfo[nCIdx].szColName, strNewColName, min(strNewColName.GetLength(), ASI_DBF_MAX_LEN_TYPE_COL_NAME - 1));
		}

		nCIdx++;
	}
	
	if(SetER(UpdateDBF_Table(pdts)))
	{
		WriteLogE("ModifyDBF_TableRenameColumn fail [%.8x]", GetER());
		return GetER();
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ModifyDBF_TableReLenColumn(CString strTName, CString strColName, UINT32 nDefLen)
{
	DBF_TABLE_COLUMN dtc;
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)	
	{
		WriteLogE("del column in table fail : not find table [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	INT32 nCIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber != nCIdx)
	{
		if(strColName.CompareNoCase(pdts->tDT.tColInfo[nCIdx].szColName) == 0)
		{
			if(pdts->tDT.tColInfo[nCIdx].nMemberType == ASI_DBF_MEMBER_TYPE_STRING ||
				pdts->tDT.tColInfo[nCIdx].nMemberType == ASI_DBF_MEMBER_TYPE_LSTRING )	
			{
				pdts->tDT.tColInfo[nCIdx].nInitStrLen = nDefLen;
				UpdateDBF_Table(pdts);
			}
			break;
		}
		nCIdx++;
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ModifyDBF_TableInitAutoIncrement(CString strTName, UINT64 nInitValue)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("init auto increment table fail : not find table [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	INT32 nAutoColIdx = -1;
	INT32 nCIdx = 0;
	while(pdts->tDT.tHInfo.nColNumber != nCIdx)
	{
		if(pdts->tDT.tColInfo[nCIdx].nColOption & ASI_DBF_COL_OPTION_FLAG_AUTO_INC)	
		{
			nAutoColIdx = nCIdx;
			break;
		}
		nCIdx++;
	}

	if(nAutoColIdx == -1)	
	{
		WriteLogE("init auto increment table fail : not find auto_inc column [%s]", (char *)(LPCSTR)strTName);
		return 0;
	}

	UINT64 nSelInitValue = UINT64(nInitValue);
	UINT64 nCurValue = 0;

	{
		if(FirstDBF_Item(strTName))
		{
			WriteLogE("init auto increment table fail : select first item fail [%s]", (char *)(LPCSTR)strTName);
			return 0;
		}

		while(NextDBF_Item(strTName) == 0) 
		{
			CASIDBFMember* pMember = GetMember();
			if(pMember == NULL)
				continue;

			nCurValue = 0;
			GetField(nAutoColIdx);
			
			switch(pMember->m_tCurDBFIR.nRtnType)
			{
				case ASI_DBF_MEMBER_TYPE_UINT16:
					if(pMember->m_tCurDBFIR.pRtnUINT16 != NULL)
						nCurValue = UINT64(*(pMember->m_tCurDBFIR.pRtnUINT16));
					break;
				case ASI_DBF_MEMBER_TYPE_UINT32:
					if(pMember->m_tCurDBFIR.pRtnUINT32 != NULL)
						nCurValue = UINT64(*(pMember->m_tCurDBFIR.pRtnUINT32));
					break;
				case ASI_DBF_MEMBER_TYPE_UINT64:
					if(pMember->m_tCurDBFIR.pRtnUINT64 != NULL)
						nCurValue = *(pMember->m_tCurDBFIR.pRtnUINT64);
					break;
			}
			if(nSelInitValue < nCurValue)	nSelInitValue = nCurValue;
		}
	}

	nSelInitValue += 1;
	WriteLogN("init auto increment table succ : select auto_inc [%s][%I64u]", (char *)(LPCTSTR)strTName, nSelInitValue);

	pdts->tDT.tHInfo.nLastID = nSelInitValue;
	return UpdateDBF_Table(pdts);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::GetTbSVer(LPCTSTR szTName, PUINT32 nVer)
{
	if(!GetFileHandle(ASI_DBF_FILE_HANDLE_READ))		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

	PDBF_TABLE_SEARCH pdts = FindDBF_Table(szTName);
	if(!pdts)
	{
		*nVer = 0;
		WriteLogE("not find table : [%s]", szTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	*nVer = pdts->tDT.tHInfo.nSchemaVer;
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::SetTbSVer(LPCTSTR szTName, UINT32 nVer)
{
	if(!GetFileHandle(ASI_DBF_FILE_HANDLE_WRITE))		return ASI_DBF_ERROR_TYPE_INVALID_HANDLE;

	PDBF_TABLE_SEARCH pdts = FindDBF_Table(szTName);
	if(!pdts)
	{
		WriteLogE("not find table : [%s]", szTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}

	pdts->tDT.tHInfo.nSchemaVer = nVer;
	return UpdateDBF_Table(pdts);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::SetFPKeyMap(UINT64 nFP)
{
	CASIDBFMember* pMember = GetMember();	
	
	TMapID64Itor find = pMember->m_tFPKeyMap.find(nFP);
	if(find != pMember->m_tFPKeyMap.end())	return -1;

	pMember->m_tFPKeyMap[nFP] = 0;
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ClearFPKeyMap()
{
	CASIDBFMember* pMember = GetMember();	
	pMember->m_tFPKeyMap.clear();
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::SetDBF_SelInfo(CString strTName, UINT32 nSelIDNum, PINT32 pSelIDArray)
{
	CASIDBFMember* pMember = GetMember();

	{
		pMember->m_strSelTName	= strTName;
		pMember->m_nSelColNum	= nSelIDNum;
		if(nSelIDNum)	memcpy(pMember->m_tSelColIDArray, pSelIDArray, sizeof(UINT32) * nSelIDNum);
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::FirstDBF_Item(CString strTName)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table info : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}
	return FirstDBF_Item(pdts);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::FirstDBF_Item(PDBF_TABLE_SEARCH pdts)
{	
	pdts->tDT.tHInfo.nCheckItemFP = pdts->tDT.tHInfo.nFirstItemFP;
	pdts->tDT.tHInfo.nCheckItemEndFP = pdts->tDT.tHInfo.nLastItemFP;

	CASIDBFMember* pMember = GetMember();

	//pMember->m_nASyncUsedMode = 0;
	pMember->m_tCurSelTable = pdts;
	pMember->m_nCurSelDirection = 1;
	ClearFPKeyMap();
	StopDBF_Item();

// 	if(m_nASyncMode && pMember->m_strSelTName.CompareNoCase(pdts->tDT.tHInfo.szName) == 0)
// 	{		
// 		DBF_SELECT_WORK tDSW;
// 		tDSW.nDirection = 1;
// 		tDSW.strTable = pdts->tDT.tHInfo.szName;
// 		tDSW.m_nSelColNum = pMember->m_nSelColNum;
// 		if(tDSW.m_nSelColNum)	memcpy(tDSW.m_tSelColIDArray, pMember->m_tSelColIDArray, sizeof(UINT32) * tDSW.m_nSelColNum);
// 		pMember->m_nASyncUsedMode = 1;
// 
// 		pMember->GetThreadSel(this)->AddWork(tDSW);
// 	}
	
	//WriteLogN("asidbfutil _ first dbf item start : [%I64u]", pdt->tHInfo.nFirstItemFP);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::LastDBF_Item(CString strTName)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table info : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}
	return LastDBF_Item(pdts);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::LastDBF_Item(PDBF_TABLE_SEARCH pdts)
{
	pdts->tDT.tHInfo.nCheckItemFP	= pdts->tDT.tHInfo.nLastItemFP;
	pdts->tDT.tHInfo.nCheckItemEndFP = pdts->tDT.tHInfo.nLastItemFP;

	CASIDBFMember* pMember = GetMember();

	//pMember->m_nASyncUsedMode = 0;
	pMember->m_tCurSelTable = pdts;
	pMember->m_nCurSelDirection = 2;
	ClearFPKeyMap();
	StopDBF_Item();
	//WriteLogN("asidbfutil _ last dbf item start : [%I64u]", pdt->tHInfo.nLastItemFP);

// 	if(m_nASyncMode && pMember->m_strSelTName.CompareNoCase(pdts->tDT.tHInfo.szName) == 0)
// 	{
// 		DBF_SELECT_WORK tDSW;
// 		tDSW.nDirection = 2;
// 		tDSW.strTable = pdts->tDT.tHInfo.szName;
// 		tDSW.m_nSelColNum = pMember->m_nSelColNum;
// 		if(tDSW.m_nSelColNum)	memcpy(tDSW.m_tSelColIDArray, pMember->m_tSelColIDArray, sizeof(UINT32) * tDSW.m_nSelColNum);
// 		pMember->m_nASyncUsedMode = 1;
// 
// 		pMember->GetThreadSel(this)->AddWork(tDSW);
// 	}
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::FirstDBFKey_Item(CString strTName, UINT64 nKey)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table info : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}
	return FirstDBFKey_Item(pdts, nKey);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::FirstDBFKey_Item(PDBF_TABLE_SEARCH pdts, UINT64 nKey)
{	
	UINT64 nFirstFP = pdts->_find_fp(nKey);
	pdts->tDT.tHInfo.nCheckItemFP = (nFirstFP ? nFirstFP : pdts->tDT.tHInfo.nFirstItemFP);
	pdts->tDT.tHInfo.nCheckItemEndFP = pdts->tDT.tHInfo.nLastItemFP;

	CASIDBFMember* pMember = GetMember();

	//pMember->m_nASyncUsedMode = 0;
	pMember->m_tCurSelTable = pdts;
	pMember->m_nCurSelDirection = 1;
	ClearFPKeyMap();
	StopDBF_Item();

// 	if(m_nASyncMode && pMember->m_strSelTName.CompareNoCase(pdts->tDT.tHInfo.szName) == 0)
// 	{		
// 		DBF_SELECT_WORK tDSW;
// 		tDSW.nDirection = 1;
// 		tDSW.nSelArrayBuffSize = 100;
// 		tDSW.strTable = pdts->tDT.tHInfo.szName;
// 		tDSW.m_nSelColNum = pMember->m_nSelColNum;
// 		if(tDSW.m_nSelColNum)	memcpy(tDSW.m_tSelColIDArray, pMember->m_tSelColIDArray, sizeof(UINT32) * tDSW.m_nSelColNum);
// 		pMember->m_nASyncUsedMode = 1;
// 
// 		pMember->GetThreadSel(this)->AddWork(tDSW);
// 	}

	//WriteLogN("asidbfutil _ first dbf item start : [%I64u]", pdt->tHInfo.nFirstItemFP);
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::LastDBFKey_Item(CString strTName, UINT64 nKey)
{
	PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
	if(!pdts)
	{
		WriteLogE("not find table info : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
	}
	return LastDBFKey_Item(pdts, nKey);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::LastDBFKey_Item(PDBF_TABLE_SEARCH pdts, UINT64 nKey)
{
	UINT64 nLastFP = pdts->_find_fp(nKey);
	pdts->tDT.tHInfo.nCheckItemFP	= (nLastFP ? nLastFP : pdts->tDT.tHInfo.nLastItemFP);
	pdts->tDT.tHInfo.nCheckItemEndFP = pdts->tDT.tHInfo.nLastItemFP;

	CASIDBFMember* pMember = GetMember();

	//pMember->m_nASyncUsedMode = 0;
	pMember->m_tCurSelTable = pdts;
	pMember->m_nCurSelDirection = 2;
	ClearFPKeyMap();
	StopDBF_Item();
	//WriteLogN("asidbfutil _ last dbf item start : [%I64u]", pdt->tHInfo.nLastItemFP);

// 	if(m_nASyncMode && pMember->m_strSelTName.CompareNoCase(pdts->tDT.tHInfo.szName) == 0)
// 	{
// 		DBF_SELECT_WORK tDSW;
// 		tDSW.nDirection = 2;
// 		tDSW.nSelArrayBuffSize = 100;
// 		tDSW.strTable = pdts->tDT.tHInfo.szName;
// 		tDSW.m_nSelColNum = pMember->m_nSelColNum;
// 		if(tDSW.m_nSelColNum)	memcpy(tDSW.m_tSelColIDArray, pMember->m_tSelColIDArray, sizeof(UINT32) * tDSW.m_nSelColNum);
// 		pMember->m_nASyncUsedMode = 1;
// 
// 		pMember->GetThreadSel(this)->AddWork(tDSW);
// 	}
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::NextDBF_Item(CString strTName, PDBF_ITEM_RTN* pDIRArray)
{	
	CASIDBFMember* pMember = GetMember();
	INT32 nRtn = 0;
	
// 	if(m_nASyncMode && pMember->m_nASyncUsedMode)
// 	{
// 		if(pMember->m_pCurItemBuff)		FreeCommonBuff(&(pMember->m_pCurItemBuff));
// 		nRtn = pMember->GetThreadSel(this)->NextItem(strTName, pMember);
// 
// 		if(pDIRArray)	*pDIRArray = pMember->m_tCurDBFIRArray;
// 	}
// 	else
	{
		PDBF_TABLE_SEARCH pdts = NULL;
		if(pMember->m_tCurSelTable && strTName.CompareNoCase(pMember->m_tCurSelTable->tDT.tHInfo.szName) == 0)
		{
			pdts = pMember->m_tCurSelTable;
		}
		else
		{
			pdts = FindDBF_Table(strTName);
		}	

		if(!pdts)
		{
			WriteLogE("not find table info : [%s]", (char *)(LPCSTR)strTName);
			return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
		}
		nRtn = NextDBF_Item(pdts);
		if(!nRtn && pDIRArray)
		{
			MakeField(pdts, pMember);
			*pDIRArray = pMember->m_tCurDBFIRArray;
		}
	}	
	return nRtn;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::NextDBF_Item(PDBF_TABLE_SEARCH pdts)
{
	CASIDBFMember* pMember = GetMember();

	if(!pdts)
	{
		if(pMember->m_tCurSelTable)	pdts = pMember->m_tCurSelTable;
		else						return -1;	
	}

	return NextDBF_ItemBuff(pdts, pMember);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::NextDBF_ItemBuff(PDBF_TABLE_SEARCH pdts, CASIDBFMember* pMember)
{	
	INT32 nRtn = 0;
	INT32 nContinue = 1;

	m_tMutex.Lock();
	
	do 
	{
		//WriteLogN("asidbfutil _ next dbf item start : [%I64u]", pdts->tDT.tHInfo.nCheckItemFP);
// 		if(pdts->tDT.tHInfo.nCheckItemFP == 1436155792)
// 		{
// 			WriteLogN("asidbfutil _ next dbf item start : [%I64u]", pdts->tDT.tHInfo.nCheckItemFP);
// 		}

		if(pdts->tDT.tHInfo.nCheckItemFP == 0)// || pdt->tHInfo.nCheckItemEndFP < pdt->tHInfo.nCheckItemFP)
		{
			WriteLogN("next data end..[%s]", pdts->tDT.tHInfo.szName);
			pMember->m_tCurSelTable = NULL;
			nRtn = 1;
			break;
		}
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_READ, pdts->tDT.tHInfo.nCheckItemFP);
		if(pMember->m_pCurItemBuff)	FreeCommonBuff(&(pMember->m_pCurItemBuff));
		
		if(SetER(ReadDBF(&(pMember->m_tCurDBFIH), sizeof(DBF_TABLE_ITEM_HEADER))))
		{
			WriteLogN("NextDBF read header fail [empty data]: [%.8x][%I64u]", GetER(), pdts->tDT.tHInfo.nCheckItemFP);			
			nRtn = GetER();
			m_tDB.tInfo.nAutoRestore = 1;
			UpdateDBF_DI();
			break;
		}	

		if(pMember->m_tCurDBFIH.nDelFlag)
		{
			switch(pMember->m_nCurSelDirection)
			{
				case 1:		pdts->tDT.tHInfo.nCheckItemFP = pMember->m_tCurDBFIH.nNextFP;	break;
				case 2:		pdts->tDT.tHInfo.nCheckItemFP = pMember->m_tCurDBFIH.nPreFP;		break;
			}
			continue;
		}

		UINT8 pIHLBuff[ASI_DBF_MAX_COLUMN_NUMBER*4] = {0, };
		if(SetER(ReadDBF(pIHLBuff, pdts->m_nItemHLen)))
		{
			WriteLogN("NextDBF read header fail [empty data]: [%.8x]", GetER());
			nRtn = GetER();
			m_tDB.tInfo.nAutoRestore = 1;
			UpdateDBF_DI();
			break;
		}	
		INT32 nSvLen = CopyHLenBuffToMap(pIHLBuff, pMember->m_tCurDBFILenMap, pdts);
		//WriteLogN("NextDBF 2: [%I64u][%I64u]\n", GetFilePointerEx(), m_nNextDataFP);

		if(pMember->m_tCurDBFIH.nCurBlockSize == 0)
		{
			WriteLogN("NextDBF cur block size is zero");
			nRtn = ASI_DBF_ERROR_TYPE_INVALID_ARGUMENT;
			break;
		}		
		//TRACE("NextDBF block size log : cur block size[%d] and sv size[%d]\n", pMember->m_tCurDBFIH.nCurBlockSize, nSvLen);

		if(pMember->m_tCurDBFIH.nCurBlockSize < (UINT32)nSvLen)
		{
			m_tDB.tInfo.nAutoRestore = 1;
			UpdateDBF_DI();
			WriteLogE("NextDBF : detect invalid block size..reserved recovery dbf : [%s][%d/%d]", pdts->tDT.tHInfo.szName, pMember->m_tCurDBFIH.nCurBlockSize, nSvLen);
			nRtn = ASI_DBF_ERROR_TYPE_INVALID_BLOCK_SIZE;
			break;
		}

		pMember->m_pCurItemBuff = (PBYTE)MakeCommonBuff(pMember->m_tCurDBFIH.nCurBlockSize);
		if(!pMember->m_pCurItemBuff)
		{
			WriteLogE("NextDBF make malloc fail : [%d][%s]", pMember->m_tCurDBFIH.nCurBlockSize, pdts->tDT.tHInfo.szName);
			nRtn = ASI_DBF_ERROR_TYPE_MEM_ALLOC_FAIL;
			break;
		}

		if(SetER(ReadDBF(pMember->m_pCurItemBuff, pMember->m_tCurDBFIH.nCurBlockSize)))
		{
			WriteLogE("NextDBF Item fail : [%.8x]", GetER());
			FreeCommonBuff(&(pMember->m_pCurItemBuff));
			nRtn = GetER();
			break;
		}

		if(SetER(ChgTableItemDec(&(pMember->m_tCurDBFIH), &(pMember->m_pCurItemBuff))))
		{
			WriteLogE("NextDBF : change decrypt fail : [%.8x]", GetER());
			FreeCommonBuff(&(pMember->m_pCurItemBuff));
			nRtn = GetER();
			break;
		}

		if(SetER(ApplyKeyToFPInfo(pdts)))
		{

		}

		//WriteLogN("NextDBF 3: [%I64u][%I64u]\n", GetFilePointerEx(), m_nNextDataFP);
		UINT64 nCheckItemFP = 0;
		switch(pMember->m_nCurSelDirection)
		{
			case 1:		
			{
// 				if(pdts->tDT.tHInfo.nCheckItemFP != m_tCurDBFIH.nNextFP && 
// 					pdts->tDT.tHInfo.nCheckItemFP != pdts->tDT.tHInfo.nLastItemFP)
				{
					if(SetFPKeyMap(pMember->m_tCurDBFIH.nNextFP))
					{
						m_tDB.tInfo.nAutoRestore = 1;
						UpdateDBF_DI();
						WriteLogE("NextDBF : detect duplicate fp..reserved recovery dbf : [%s][%I64u]", pdts->tDT.tHInfo.szName, pMember->m_tCurDBFIH.nNextFP);
					}
					else
					{
						nCheckItemFP = pMember->m_tCurDBFIH.nNextFP;	
					}
				}
				break;
			}
			case 2:	
			{
// 				if(pdts->tDT.tHInfo.nCheckItemFP != m_tCurDBFIH.nPreFP && 
// 					pdts->tDT.tHInfo.nCheckItemFP != pdts->tDT.tHInfo.nFirstItemFP)
				{
					if(SetFPKeyMap(pMember->m_tCurDBFIH.nPreFP))
					{
						m_tDB.tInfo.nAutoRestore = 1;
						UpdateDBF_DI();
						WriteLogE("NextDBF : detect duplicate fp..reserved recovery dbf : [%s][%I64u]", pdts->tDT.tHInfo.szName, pMember->m_tCurDBFIH.nNextFP);
					}
					else
					{
						nCheckItemFP = pMember->m_tCurDBFIH.nPreFP;	
					}
				}
				break;
			}
		}
		pdts->tDT.tHInfo.nCheckItemFP = nCheckItemFP;
		nContinue = 0;
	} while(nContinue);	

	m_tMutex.UnLock();

	return nRtn;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::StopDBF_Item()
{
// 	if(m_nASyncMode)
// 	{
// 		CASIDBFMember* pMember = GetMember();
// 
// 		DBF_SELECT_WORK tDSW;
// 		tDSW.nDirection = 0;
// 
// 		pMember->GetThreadSel(this)->AddWork(tDSW);
// 	}
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFUtil::CheckDBF_Table(PDBF_TABLE_SEARCH pdts_new, INT32 nModify)
{	
	PDBF_TABLE_SEARCH pdts_old = FindDBF_Table(pdts_new->tDT.tHInfo.szName);
	if(CompareDBF_TableHeader(pdts_old, pdts_new) == 0)
	{
		return 0;
	}

	if(!nModify)		return -1;

	{
		pdts_new->tDT.tHInfo.nTType = pdts_old->tDT.tHInfo.nTType;
		pdts_new->tDT.tHInfo.nModifyDate = GetCurrentDateTimeInt();
	}

	{		
		CreateDBF_Table(pdts_new);
		SetDBF_SelInfo();
		FirstDBF_Item(pdts_old);
		while(NextDBF_Item(pdts_old) == 0)
		{
			DBF_TABLE_ITEM_HEADER dbfih;
			TMapID64 tHLenMap;

			UINT32 nItemIdx = 0;
			while(pdts_new->tDT.tHInfo.nColNumber > nItemIdx)
			{
				CString strColName = GetFieldName(pdts_new, nItemIdx);
				if(IsExistField(pdts_old, strColName))
				{
					if(GetFieldType(pdts_new, strColName) == GetFieldType(pdts_old, strColName))
					{
						dbfih.nCurBlockSize += GetFieldLength(pdts_old, strColName);
						SetBuffLength(tHLenMap, GetFieldLength(pdts_old, strColName), nItemIdx);
						nItemIdx += 1;
						continue;
					}
				}
				dbfih.nCurBlockSize += GetFieldInitLength(pdts_new, nItemIdx);
				SetBuffLength(tHLenMap, GetFieldInitLength(pdts_new, nItemIdx), nItemIdx);
				nItemIdx += 1;
			}
						
			dbfih.nCurFP = m_nDBFLastFP;
			
			PBYTE pItemBuff = MakeCommonBuff(dbfih.nCurBlockSize);
			nItemIdx = 0;
			UINT32 nBuffPos = 0;
			while(pdts_new->tDT.tHInfo.nColNumber > nItemIdx)
			{
				CString strColName = GetFieldName(pdts_new, nItemIdx);
				if(IsExistField(pdts_old, strColName))
				{
					if(GetFieldType(pdts_new, strColName) == GetFieldType(pdts_old, strColName))
					{
						PDBF_ITEM_RTN pDBFIR = GetField(pdts_old, strColName);

						switch(pDBFIR->nRtnType)
						{
							case ASI_DBF_MEMBER_TYPE_UINT16:
							{
								if(pDBFIR->pRtnUINT16)
								{
									memcpy(pItemBuff + nBuffPos, pDBFIR->pRtnUINT16, sizeof(UINT16));
								}								
								nBuffPos += sizeof(UINT16);
								break;
							}
							case ASI_DBF_MEMBER_TYPE_UINT32:
							{
								if(pDBFIR->pRtnUINT32)
								{
									memcpy(pItemBuff + nBuffPos, pDBFIR->pRtnUINT32, sizeof(UINT32));				
								}
								nBuffPos += sizeof(UINT32);
								break;
							}
							case ASI_DBF_MEMBER_TYPE_UINT64:
							{
								if(pDBFIR->pRtnUINT64)
								{
									memcpy(pItemBuff + nBuffPos, pDBFIR->pRtnUINT64, sizeof(UINT64));				
								}
								nBuffPos += sizeof(UINT64);
								break;
							}
							case ASI_DBF_MEMBER_TYPE_STRING:
							{
								if(pDBFIR->szRtnString)
								{
									memcpy(pItemBuff + nBuffPos, pDBFIR->szRtnString, pDBFIR->nRtnLen);		
								}
								nBuffPos += pDBFIR->nRtnLen;
								break;
							}
							case ASI_DBF_MEMBER_TYPE_LSTRING:
							{
								if(pDBFIR->szRtnString)
								{
									memcpy(pItemBuff + nBuffPos, pDBFIR->szRtnString, pDBFIR->nRtnLen);		
								}
								nBuffPos += pDBFIR->nRtnLen;
								break;
							}
							case ASI_DBF_MEMBER_TYPE_OBJECT:
							{
								if(pDBFIR->lpRtnObject)
								{
									memcpy(pItemBuff + nBuffPos, pDBFIR->lpRtnObject, pDBFIR->nRtnLen);		
								}
								nBuffPos += pDBFIR->nRtnLen;
								break;
							}
						}
						nItemIdx += 1;
						continue;
					}
				}

				{
					DBF_TABLE_ITEM dfir = GetFieldDefValue(pdts_new, nItemIdx);
					switch(dfir.nRtnType)
					{
						case ASI_DBF_MEMBER_TYPE_UINT16:
						{
							memcpy(pItemBuff + nBuffPos, &(dfir.nRtnUINT16), sizeof(UINT16));				
							nBuffPos += sizeof(UINT16);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_UINT32:
						{
							memcpy(pItemBuff + nBuffPos, &(dfir.nRtnUINT32), sizeof(UINT32));				
							nBuffPos += sizeof(UINT32);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_UINT64:
						{
							memcpy(pItemBuff + nBuffPos, &(dfir.nRtnUINT64), sizeof(UINT64));				
							nBuffPos += sizeof(UINT64);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_STRING:
						{
							UINT32 nInitLen = GetFieldInitLength(pdts_new, nItemIdx);
							nBuffPos += nInitLen;
							break;
						}
						case ASI_DBF_MEMBER_TYPE_LSTRING:
						{
							UINT32 nInitLen = GetFieldInitLength(pdts_new, nItemIdx);
							nBuffPos += nInitLen;
							break;
						}
						case ASI_DBF_MEMBER_TYPE_OBJECT:
						{
							UINT32 nInitLen = GetFieldInitLength(pdts_new, nItemIdx);
							nBuffPos += nInitLen;
							break;
						}
					}
				}
				nItemIdx += 1;
			}			
			InsertDBF_TItem(pdts_new, &dbfih, tHLenMap, pItemBuff);
			FreeCommonBuff(&pItemBuff);
		}
		
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0, FILE_END);
		m_nDBFLastFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);

		pdts_old->tDT.tHInfo.nDelFlag = 1;
		UpdateDBF_Table(pdts_old);

		{
			PDBF_TABLE_SEARCH pdts_find = FindDBF_Table(pdts_new->tDT.tHInfo.szName);
			if(pdts_find)
			{
				*pdts_find = *pdts_new;
			}
		}
	}

	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32				CASIDBFUtil::InsertDBF_TItem(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, PBYTE lpBuff)
{
	//TRACE("InsertDBF 1: [%I64u:%I64u][%d]\n", m_nCurrentFP, GetFilePointerEx(), pdfih->nCurBlockSize);
	PBYTE lpEncBuff = NULL;
	INT32 nRtn = 0;

	{
		SetAutoIncrementID(pdts, pdtih, tHLenMap, lpBuff);
	}
	
	ChkOpTickCntLog();
	if(SetER(ChgTableItemEnc(pdtih, lpBuff, &lpEncBuff)))
	{
		WriteLogE("InsertDBF_TItem fail by encrypt fail : [%.8x]", GetER());
	}
	if(lpEncBuff)	lpBuff = lpEncBuff;
	ChkOpTickCntLog();

	//WriteLogN("insert dbf file point : [%I64u]", m_nDBFLastFP);
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, m_nDBFLastFP);		
	ChkOpTickCntLog();

	if(m_nDBFLastFP == pdts->tDT.tHInfo.nLastItemFP)
	{
		DBF_TABLE_ITEM_HEADER tDTIHLast;
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdts->tDT.tHInfo.nLastItemFP);
		if(SetER(ReadDBF(&tDTIHLast, sizeof(DBF_TABLE_ITEM_HEADER), ASI_DBF_FILE_HANDLE_WRITE)))
		{
			WriteLogE("InsertDBF_TItem fail by old item read : [%.8x]", GetER());
			SetFileEOF(m_nDBFLastFP);
			nRtn = GetER();
			return nRtn;
		}

		pdts->tDT.tHInfo.nLastItemFP = tDTIHLast.nCurFP;
	}

	DBF_TABLE_ITEM_HEADER tDTIH;
	pdtih->nCurFP			= m_nDBFLastFP;	
	pdtih->nPreFP			= pdts->tDT.tHInfo.nLastItemFP;
	pdtih->nNextFP			= 0;
	pdtih->nTID				= pdts->tDT.tHInfo.nTID;
	pdtih->nInitBlockSize	= pdtih->nCurBlockSize;
	UINT8 pIHLBuff[ASI_DBF_MAX_COLUMN_NUMBER*4] = {0, };
	CopyHLenMapToBuff(tHLenMap, pIHLBuff, pdts);

	
	do 
	{
		if(SetER(WriteDBF(pdtih, sizeof(DBF_TABLE_ITEM_HEADER))))
		{
			WriteLogE("InsertDBF_TItem fail : [%.8x]", GetER());
			nRtn = GetER();
			break;
		}

		if(SetER(WriteDBF(pIHLBuff, pdts->m_nItemHLen)))
		{
			WriteLogE("InsertDBF_TItem fail : [%.8x]", GetER());
			nRtn = GetER();
			break;
		}

		//TRACE("InsertDBF 2: [%I64u][%d]\n", GetFilePointerEx(), pdfih->nInitBlockSize);		

		if(SetER(WriteDBF(lpBuff, pdtih->nInitBlockSize)))
		{
			WriteLogE("InsertDBF_TItem fail : [%.8x]", GetER());
			nRtn = GetER();
			break;
		}

		ChkOpTickCntLog();

		if(pdts->tDT.tHInfo.nLastItemFP)
		{
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdts->tDT.tHInfo.nLastItemFP);
			if(SetER(ReadDBF(&tDTIH, sizeof(DBF_TABLE_ITEM_HEADER), ASI_DBF_FILE_HANDLE_WRITE)))
			{
				WriteLogE("InsertDBF_TItem fail by old item read : [%.8x]", GetER());
				SetFileEOF(m_nDBFLastFP);
				nRtn = GetER();
				break;
			}
			tDTIH.nNextFP = m_nDBFLastFP;
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdts->tDT.tHInfo.nLastItemFP);
			if(SetER(WriteDBF(&tDTIH, sizeof(DBF_TABLE_ITEM_HEADER))))
			{
				WriteLogE("InsertDBF_TItem fail by old item write : [%.8x]", GetER());
				SetFileEOF(m_nDBFLastFP);
				nRtn = GetER();
				break;
			}
			ChkOpTickCntLog();
		}

		{
			if(pdts->tDT.tHInfo.nFirstItemFP == 0)	pdts->tDT.tHInfo.nFirstItemFP = m_nDBFLastFP;
			pdts->tDT.tHInfo.nLastItemFP	= m_nDBFLastFP;
			pdts->tDT.tHInfo.nItemCnt	+= 1;
			pdts->tDT.tHInfo.nItemTCnt	+= 1;

			if(SetER(UpdateDBF_Table(pdts)))
			{
				WriteLogE("InsertDBF_TItem fail by update table fail : [%.8x]", GetER());
				nRtn = GetER();
				break;
			}
			ChkOpTickCntLog(100);
		}

		//WriteLogN("asidbfutil _ [%d]:[%I64u][%I64u:%I64u][%I64u]", pdtih->nTID, pdtih->nPreFP, pdtih->nCurFP, pdt->tHInfo.nLastItemFP, tDTIH.nNextFP);

		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0, FILE_END);
		m_nDBFLastFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	} while (FALSE);
	
	if(lpEncBuff)	FreeCommonBuff(&lpEncBuff);
	//TRACE("InsertDBF 3: [%I64u][%d]\n", m_nCurrentFP, pdfih->nInitBlockSize);
	return nRtn;
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::UpdateDBF_TItem(PDBF_TABLE_SEARCH pdts, PDBF_TABLE_ITEM_HEADER pdtih, TMapID64& tHLenMap, PBYTE lpBuff)
{
	PBYTE lpEncBuff = NULL;
	INT32 nRtn = 0;
	
	if(SetER(ChgTableItemEnc(pdtih, lpBuff, &lpEncBuff)))
	{
		WriteLogE("InsertDBF_TItem fail by encrypt fail : [%.8x]", GetER());
	}
	if(lpEncBuff)	lpBuff = lpEncBuff;
	ChkOpTickCntLog();
	
	DBF_TABLE_ITEM_HEADER tDTIH;
	UINT8 tCurDFILen[ASI_DBF_MAX_COLUMN_NUMBER * 2] = {0, };
	UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdtih->nCurFP);
	
	do 
	{
		if(SetER(ReadDBF(&tDTIH, sizeof(DBF_TABLE_ITEM_HEADER), ASI_DBF_FILE_HANDLE_WRITE)))
		{
			WriteLogE("UpdateDBF_TItem fail : [%.8x]", GetER());
			nRtn = GetER();
			break;
		}

		if(tDTIH.nTID != pdts->tDT.tHInfo.nTID)
		{
			WriteLogN("UpdateDBF_TItem : current field is not request table item..[%d] != [%d]", tDTIH.nTID, pdts->tDT.tHInfo.nTID);
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = -10;
			break; 
		}

		if(tDTIH.nDelFlag)
		{
			WriteLogE("UpdateDBF_TItem fail : already delete row : [%s][%.8x]", pdts->tDT.tHInfo.szName, pdtih->nCurFP);
			nRtn = ASI_DBF_ERROR_TYPE_V2_ALREADY_DELETE_ITEM;
			break;
		}

		if(pdtih->nCurBlockSize > tDTIH.nInitBlockSize)
		{		
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, m_nDBFLastFP);
			WriteLogN("auto delete item by update item : [%s][%I64u][%u][%u]", pdts->tDT.tHInfo.szName, m_nDBFLastFP, pdtih->nCurBlockSize, tDTIH.nInitBlockSize);
			{
				pdtih->nCurFP			= m_nDBFLastFP;	
				pdtih->nPreFP			= tDTIH.nCurFP;
				pdtih->nNextFP			= tDTIH.nNextFP;

				pdtih->nUpCnt			= tDTIH.nUpCnt + 1;
				pdtih->nTID				= pdts->tDT.tHInfo.nTID;
				pdtih->nInitBlockSize	= pdtih->nCurBlockSize;

				if(SetER(WriteDBF(pdtih, sizeof(DBF_TABLE_ITEM_HEADER))))
				{
					WriteLogE("UpdateDBF_TItem fail : header : [%.8x]", GetER());
					nRtn = GetER();
					break;
				}

				UINT8 pIHLBuff[ASI_DBF_MAX_COLUMN_NUMBER*4] = {0, };
				CopyHLenMapToBuff(tHLenMap, pIHLBuff, pdts);

				if(SetER(WriteDBF(pIHLBuff, pdts->m_nItemHLen)))
				{
					WriteLogE("UpdateDBF_TItem fail : body header : [%.8x]", GetER());
					nRtn = GetER();
					break;
				}			

				if(SetER(WriteDBF(lpBuff, pdtih->nCurBlockSize)))
				{
					WriteLogE("UpdateDBF_TItem fail : body value : [%.8x]", GetER());
					nRtn = GetER();
					break;
				}			

				if(tDTIH.nNextFP)
				{
					DBF_TABLE_ITEM_HEADER tDTIH_Next;

					SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, tDTIH.nNextFP);
					if(SetER(ReadDBF(&tDTIH_Next, sizeof(DBF_TABLE_ITEM_HEADER), ASI_DBF_FILE_HANDLE_WRITE)))
					{
						WriteLogE("UpdateDBF_TItem fail by old item read : [%.8x]", GetER());
						SetFileEOF(m_nDBFLastFP);
						nRtn = GetER();
						break;
					}
					tDTIH_Next.nPreFP = m_nDBFLastFP;
					SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, tDTIH_Next.nCurFP);
					if(SetER(WriteDBF(&tDTIH_Next, sizeof(DBF_TABLE_ITEM_HEADER))))
					{
						WriteLogE("UpdateDBF_TItem fail by old item write : [%.8x]", GetER());
						SetFileEOF(m_nDBFLastFP);
						nRtn = GetER();
						break;
					}
				}

				{
					tDTIH.nDelFlag = 1;
					tDTIH.nNextFP = m_nDBFLastFP;
					SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, tDTIH.nCurFP);
					if(SetER(WriteDBF(&tDTIH, sizeof(DBF_TABLE_ITEM_HEADER))))
					{
						WriteLogE("UpdateDBF_TItem fail by old item update for del flag : [%.8x]", GetER());
						SetFileEOF(m_nDBFLastFP);
						nRtn = GetER();
						break;
					}
					//WriteLogN("auto delete item by update item : [%I64u]->[%I64u]", tDTIH.nCurFP, tDTIH.nNextFP);
				}
			}

			{
				if(pdts->tDT.tHInfo.nLastItemFP == tDTIH.nCurFP)
				{
					pdts->tDT.tHInfo.nLastItemFP = m_nDBFLastFP;
				}

				pdts->tDT.tHInfo.nItemTCnt	+= 1;

				if(SetER(UpdateDBF_Table(pdts)))
				{
					WriteLogE("UpdateDBF_TItem fail by update table fail : [%.8x]", GetER());
					nRtn = GetER();
					break;
				}
			}

			ChkOpTickCntLog();
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, 0, FILE_END);
			m_nDBFLastFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
			ChkOpTickCntLog(100);
		}
		else
		{
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, pdtih->nCurFP);	
			pdtih->nInitBlockSize	= tDTIH.nInitBlockSize;
			if(SetER(WriteDBF(pdtih, sizeof(DBF_TABLE_ITEM_HEADER))))
			{
				WriteLogE("UpdateDBF_TItem fail : header : [%.8x]", GetER());
				nRtn = GetER();
				break;
			}

			UINT8 pIHLBuff[ASI_DBF_MAX_COLUMN_NUMBER*4] = {0, };
			CopyHLenMapToBuff(tHLenMap, pIHLBuff, pdts);
			if(SetER(WriteDBF(pIHLBuff, pdts->m_nItemHLen)))
			{
				WriteLogE("UpdateDBF_TItem fail : item header : [%.8x]", GetER());
				nRtn = GetER();
				break;
			}

			if(SetER(WriteDBF(lpBuff, pdtih->nCurBlockSize)))
				//if(SetER(WriteDBF(lpBuff, tDTIH.nInitBlockSize)))
			{
				WriteLogE("UpdateDBF_TItem fail : body value : [%.8x]", GetER());
				nRtn = GetER();
				break;
			}
			ChkOpTickCntLog(101);
		}
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
		nRtn = 0;
	} while (FALSE);
	
	if(lpEncBuff)	FreeCommonBuff(&lpEncBuff);

	return nRtn;
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::DeleteDBF_TItem(PDBF_TABLE_SEARCH pdts, UINT64 nFP)
{
	UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nFP);

	DBF_TABLE_ITEM_HEADER tDTIH;	
	if(SetER(ReadDBF(&tDTIH, sizeof(DBF_TABLE_ITEM_HEADER), ASI_DBF_FILE_HANDLE_WRITE)))
	{
		WriteLogE("DeleteDBF_TItem fail : read old item header : [%.8x]", GetER());
		return GetER();
	}

	if(tDTIH.nDelFlag)
	{
		WriteLogE("DeleteDBF_TItem fail : already delete row : [%s][%.8x]", pdts->tDT.tHInfo.szName, nFP);
		return ASI_DBF_ERROR_TYPE_V2_ALREADY_DELETE_ITEM;
	}
	
	tDTIH.nDelFlag = 1;

	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nFP);
	if(SetER(WriteDBF(&tDTIH, sizeof(DBF_TABLE_ITEM_HEADER))))
	{
		WriteLogE("DeleteDBF_TItem fail : write old item header [%.8x]", GetER());
		return GetER();
	}
	SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);

	//WriteLogN("delete item by update item : [%I64u]", nFP);

	{
		pdts->tDT.tHInfo.nItemCnt -= 1;
		UpdateDBF_Table(pdts);
	}
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

PDBF_TABLE_ITEM_SET_VALUE	CASIDBFUtil::FindPreSetTItemValue(CString strTName, CString strColName)
{
	CASIDBFMember* pMember = GetMember();

	TMapTItemSetValueMapItor find_t = pMember->m_tTItemSetValueMapMap.find(strTName);
	if(find_t == pMember->m_tTItemSetValueMapMap.end())	return 0;

	TMapTItemSetValueItor find_c = find_t->second.find(strColName);
	if(find_c == find_t->second.end())		return 0;

	return &(find_c->second);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ExeSelectDBFByID(CString strTName, UINT32 nID)
{
	UINT64 nFP = 0;
	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	m_tMutex.Lock();
	do 
	{
		PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
		if(!pdts)
		{
			WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
			break;
		}

		nFP = pdts->_find_fp(nID);
	}while(FALSE);
	m_tMutex.UnLock();

	if(nRtn == ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE)
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;

	if(!nFP)
	{
		WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_SET_FILE_POINTER_FAIL;
	}

	return ExeSelectDBF(strTName, nFP);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ExeUpdateDBFByID(CString strTName, UINT32 nID)
{
	UINT64 nFP = 0;
	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	m_tMutex.Lock();
	do 
	{
		PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
		if(!pdts)
		{
			WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
			break;
		}

		nFP = pdts->_find_fp(nID);
	}while(FALSE);
	m_tMutex.UnLock();

	if(nRtn == ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE)
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;

	if(!nFP)
	{
		WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_SET_FILE_POINTER_FAIL;
	}

	return ExeUpdateDBF(strTName, &nFP);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ExeDeleteDBFByID(CString strTName, UINT32 nID)
{
	UINT64 nFP = 0;
	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	m_tMutex.Lock();
	do 
	{
		PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
		if(!pdts)
		{
			WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
			break;
		}

		nFP = pdts->_find_fp(nID);
	}while(FALSE);
	m_tMutex.UnLock();

	if(nRtn == ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE)
		return ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;

	if(!nFP)
	{
		WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
		return ASI_DBF_ERROR_TYPE_SET_FILE_POINTER_FAIL;
	}

	return ExeDeleteDBF(strTName, nFP);
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ExeSelectDBF(CString strTName, UINT64 nFP)
{
	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	m_tMutex.Lock();
	do 
	{
		PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
		if(!pdts)
		{
			WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
			break;
		}

		UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);	
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_READ, nFP);

		CASIDBFMember* pMember = GetMember();

		FreeCommonBuff(&(pMember->m_pCurItemBuff));

		if(SetER(ReadDBF(&(pMember->m_tCurDBFIH), sizeof(DBF_TABLE_ITEM_HEADER))))
		{
			WriteLogN("ExeSelectDBF read item header fail [empty data]: [%.8x]", GetER());
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = GetER();
			break;
		}	

		if(pMember->m_tCurDBFIH.nTID != pdts->tDT.tHInfo.nTID)
		{
			WriteLogN("ExeSelectDBF : current field is not request table item..[%d] != [%d]", pMember->m_tCurDBFIH.nTID, pdts->tDT.tHInfo.nTID);
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = -10;		
			break;
		}

		if(pMember->m_tCurDBFIH.nDelFlag)
		{
			WriteLogN("ExeSelectDBF : current field status is delete..[%I64u][%I64u][%I64u]:[%u][%u]", nFP, nCurFP, GetFilePointerEx(ASI_DBF_FILE_HANDLE_READ), pMember->m_tCurDBFIH.nCurBlockSize, pMember->m_tCurDBFIH.nInitBlockSize);
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = ASI_DBF_ERROR_TYPE_V2_ALREADY_DELETE_ITEM;		
			break;
		}

		UINT8 pIHLBuff[ASI_DBF_MAX_COLUMN_NUMBER*4] = {0, };
		if(SetER(ReadDBF(pIHLBuff, pdts->m_nItemHLen)))
		{
			WriteLogN("ExeSelectDBF : read header fail [empty data]: [%.8x]", GetER());
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = GetER();
			break;
		}	
		INT32 nSvLen = CopyHLenBuffToMap(pIHLBuff, pMember->m_tCurDBFILenMap, pdts);

		if(pMember->m_tCurDBFIH.nCurBlockSize == 0)
		{
			WriteLogN("ExeSelectDBF cur block size is zero");
			nRtn = ASI_DBF_ERROR_TYPE_INVALID_ARGUMENT;
			break;
		}

		if(pMember->m_tCurDBFIH.nCurBlockSize < (UINT32)nSvLen)
		{
			WriteLogN("NextDBF block size invalid : cur block size[%d] and sv size[%d]", pMember->m_tCurDBFIH.nCurBlockSize, nSvLen);
			nRtn = ASI_DBF_ERROR_TYPE_INVALID_ARGUMENT;
			break;
		}

		pMember->m_pCurItemBuff = (PBYTE)MakeCommonBuff(pMember->m_tCurDBFIH.nCurBlockSize);
		if(!pMember->m_pCurItemBuff)
		{
			WriteLogE("ExeSelectDBF : make malloc fail : [%d]", pMember->m_tCurDBFIH.nCurBlockSize);
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = ASI_DBF_ERROR_TYPE_MEM_ALLOC_FAIL;
			break;
		}

		if(SetER(ReadDBF(pMember->m_pCurItemBuff, pMember->m_tCurDBFIH.nCurBlockSize)))
		{
			WriteLogE("ExeSelectDBF : Item fail : [%.8x]", GetER());
			FreeCommonBuff(&(pMember->m_pCurItemBuff));
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = GetER();
			break;
		}

		if(SetER(ChgTableItemDec(&(pMember->m_tCurDBFIH), &(pMember->m_pCurItemBuff))))
		{
			WriteLogE("ExeSelectDBF : change decrypt fail : [%.8x]", GetER());
			FreeCommonBuff(&(pMember->m_pCurItemBuff));
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
			nRtn = GetER();
			break;
		}

		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
		pMember->m_tCurSelTable = pdts;
		nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;	
}
// //---------------------------------------------------------------------
// 
// INT32		CASIDBFUtil::ExeInsertDBF(CString strTName, PUINT64 pnNewFP, PUINT64 pnLastID)
// {
// 	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
// 	m_tMutex.Lock();
// 	do 
// 	{
// 		CASIDBFMember* pMember = GetMember();
// 
// 		PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
// 		if(!pdts)
// 		{
// 			WriteLogN("not find table info : [%s]", strTName);
// 			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
// 			break;
// 		}
// 
// 		InitOpTickCntLog();
// 		ChkOpTickCntLog(1, ASI_DBFQ_QUERY_COMMAND_TYPE_INSERT);
// 		ChkOpTickCntLog(2);
// 		
// 		DBF_TABLE_ITEM_HEADER tDTIH;	
// 		TMapID64 tHLenMap;
// 
// 		UINT32 nItemIdx = 0;
// 		while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
// 		{
// 			PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);
// 			
// 			UINT32 nInitLen = 0;
// 			switch(pdtc->nMemberType)
// 			{
// 				case ASI_DBF_MEMBER_TYPE_UINT16:		nInitLen = sizeof(UINT16);			break;
// 				case ASI_DBF_MEMBER_TYPE_UINT32:		nInitLen = sizeof(UINT32);			break;
// 				case ASI_DBF_MEMBER_TYPE_UINT64:		nInitLen = sizeof(UINT64);			break;
// 				case ASI_DBF_MEMBER_TYPE_STRING:
// 				{
// 					PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
// 					
// 					if(!pdtisv)				nInitLen = pdtc->nInitStrLen;
// 					else					nInitLen = max(pdtisv->strString.GetLength(), pdtc->nInitStrLen);
// 					break;
// 				}
// 				case ASI_DBF_MEMBER_TYPE_LSTRING:
// 				{
// 					PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
// 
// 					if(!pdtisv)				nInitLen = GetLStrLen(pdtc->nInitStrLen);
// 					else					nInitLen = max(pdtisv->strString.GetLength(), GetLStrLen(pdtc->nInitStrLen));
// 					break;
// 				}
// 				case ASI_DBF_MEMBER_TYPE_OBJECT:
// 				{
// 					PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
// 					
// 					if(!pdtisv)				nInitLen = pdtc->nInitStrLen;
// 					else					nInitLen = max(pdtisv->nObjLen, pdtc->nInitStrLen);
// 					break;
// 				}
// 			}			
// 			SetBuffLength(tHLenMap, nInitLen, nItemIdx);
// 			tDTIH.nCurBlockSize += nInitLen;
// 			nItemIdx++;
// 		}
// 
// 		ChkOpTickCntLog();
// 
// 		PBYTE lpBuff = (PBYTE)MakeCommonBuff(tDTIH.nCurBlockSize);
// 		if(!lpBuff)	
// 		{
// 			nRtn = -2;
// 			break;
// 		}
// 
// 		UINT32 nPriKeyType = 0;
// 		CString strPriKey = "";
// 
// 		PBYTE lpPosBuff = lpBuff;
// 		nItemIdx = 0;
// 		while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
// 		{
// 			PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);
// 			PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
// 					
// 			switch(pdtc->nMemberType)
// 			{
// 				case ASI_DBF_MEMBER_TYPE_UINT16:
// 				{
// 					if(!pdtisv)						memcpy(lpPosBuff, &(pdtc->nDefaultValue), sizeof(UINT16));
// 					else							memcpy(lpPosBuff, &(pdtisv->nUINT16), sizeof(UINT16));
// 					lpPosBuff += sizeof(UINT16);
// 					break;
// 				}
// 				case ASI_DBF_MEMBER_TYPE_UINT32:
// 				{
// 					if(!pdtisv)						memcpy(lpPosBuff, &(pdtc->nDefaultValue), sizeof(UINT32));
// 					else							memcpy(lpPosBuff, &(pdtisv->nUINT32), sizeof(UINT32));
// 					lpPosBuff += sizeof(UINT32);
// 					break;
// 				}
// 				case ASI_DBF_MEMBER_TYPE_UINT64:
// 				{
// 					if(!pdtisv)						memcpy(lpPosBuff, &(pdtc->nDefaultValue), sizeof(UINT64));
// 					else							memcpy(lpPosBuff, &(pdtisv->nUINT64), sizeof(UINT64));
// 					lpPosBuff += sizeof(UINT64);
// 					break;
// 				}
// 				case ASI_DBF_MEMBER_TYPE_STRING:		
// 				{
// 					if(!pdtisv)						lpPosBuff += pdtc->nInitStrLen;
// 					else	
// 					{
// 						memcpy(lpPosBuff, pdtisv->strString, pdtisv->strString.GetLength());				
// 						lpPosBuff += max(pdtisv->strString.GetLength(), pdtc->nInitStrLen);
// 					}
// 					break;
// 				}
// 				case ASI_DBF_MEMBER_TYPE_LSTRING:		
// 				{
// 					if(!pdtisv)						lpPosBuff += GetLStrLen(pdtc->nInitStrLen);
// 					else	
// 					{
// 						memcpy(lpPosBuff, pdtisv->strString, pdtisv->strString.GetLength());				
// 						lpPosBuff += max(pdtisv->strString.GetLength(), GetLStrLen(pdtc->nInitStrLen));
// 					}
// 					break;
// 				}
// 				case ASI_DBF_MEMBER_TYPE_OBJECT:		
// 				{
// 					if(!pdtisv)						lpPosBuff += pdtc->nInitStrLen;
// 					else	
// 					{
// 						memcpy(lpPosBuff, pdtisv->lpObject, pdtisv->nObjLen);				
// 						lpPosBuff += max(pdtisv->nObjLen, pdtc->nInitStrLen);
// 						pdtisv->nObjLen = 0;
// 					}
// 					break;
// 				}
// 			}
// 
// 			if(pdtc->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY)
// 			{
// 				switch(pdtc->nMemberType)
// 				{
// 					case ASI_DBF_MEMBER_TYPE_UINT16:				
// 					case ASI_DBF_MEMBER_TYPE_UINT32:
// 					case ASI_DBF_MEMBER_TYPE_UINT64:
// 					{
// 						nPriKeyType = ASI_DBF_MEMBER_TYPE_UINT64;
// 						break;
// 					}
// 					case ASI_DBF_MEMBER_TYPE_STRING:		
// 					{
// 						nPriKeyType = ASI_DBF_MEMBER_TYPE_STRING;
// 						if(pdtisv)	strPriKey = pdtisv->strString;
// 						break;
// 					}
// 					case ASI_DBF_MEMBER_TYPE_LSTRING:		
// 					{
// 						nPriKeyType = ASI_DBF_MEMBER_TYPE_LSTRING;
// 						if(pdtisv)	strPriKey = pdtisv->strString;
// 						break;
// 					}
// 					case ASI_DBF_MEMBER_TYPE_OBJECT:		
// 					{
// 						nPriKeyType = ASI_DBF_MEMBER_TYPE_OBJECT;
// 						//if(pdtisv)	strPriKey = pdtisv->lpObject;
// 						break;
// 					}
// 				}
// 			}
// 			nItemIdx++;
// 		}
// 		pMember->m_tTItemSetValueMapMap.erase(strTName);
// 
// 		ChkOpTickCntLog();
// 
// 		if(SetER(InsertDBF_TItem(pdts, &tDTIH, tHLenMap, lpBuff)))
// 		{
// 			FreeCommonBuff(&lpBuff);
// 			nRtn = GetER();
// 			break;
// 		}
// 
// 		ChkOpTickCntLog(200);
// 		WriteLogOpTickCnt(strTName);
// 
// 
// 		FreeCommonBuff(&lpBuff);
// 		if(pnNewFP)		*pnNewFP	= tDTIH.nCurFP;
// 		if(pnLastID)	*pnLastID	= pMember->m_nLastInsID;
// 
// 		switch(nPriKeyType)
// 		{
// 			case ASI_DBF_MEMBER_TYPE_UINT16:
// 			case ASI_DBF_MEMBER_TYPE_UINT32:
// 			case ASI_DBF_MEMBER_TYPE_UINT64:
// 			{
// 				pdts->_apply_fp(pdts->tDT.tHInfo.nLastID, tDTIH.nCurFP);
// 				break;
// 			}
// 			case ASI_DBF_MEMBER_TYPE_STRING:
// 			{
// 				pdts->_apply_fp(strPriKey, tDTIH.nCurFP);
// 				break;
// 			}
// 			case ASI_DBF_MEMBER_TYPE_LSTRING:
// 			{
// 				pdts->_apply_fp(strPriKey, tDTIH.nCurFP);
// 				break;
// 			}
// 		}
// 
// 	} while (FALSE);
// 
// 	m_tMutex.UnLock();
// 	return nRtn;
// }
// //---------------------------------------------------------------------
// 
// INT32				CASIDBFUtil::ExeUpdateDBF(CString strTName, PUINT64 pnFP)
// {
// 	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
// 	m_tMutex.Lock();
// 	do 
// 	{
// 		if(!pnFP)
// 		{
// 			nRtn = -1;
// 			break;
// 		}
// 
// 		if(*pnFP == 0)
// 		{
// 			WriteLogE("ExeUpdateDBF fail : FP value is 0 : [%s][%.8x]", m_strDBFName, *pnFP);
// 			nRtn = -2;
// 			break;
// 		}
// 		UINT32 nTickA = GetTickCount();
// 		InitOpTickCntLog();
// 		ChkOpTickCntLog(1, ASI_DBFQ_QUERY_COMMAND_TYPE_UPDATE);
// 		ChkOpTickCntLog(2, nTickA);
// 		ChkOpTickCntLog();
// 
// 		CASIDBFMember* pMember = GetMember();
// 		PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
// 		if(!pdts)
// 		{
// 			WriteLogN("not find table info : [%s]", strTName);
// 			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
// 			break;
// 		}
// 
// 		ChkOpTickCntLog();
// 
// 		DBF_TABLE_ITEM_HEADER tDTIH;
// 		TMapID64 tHLenMap;
// 		UINT32 nItemIdx = 0;
// 
// 		UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
// 		
// 		{
// 			ChkOpTickCntLog();
// 			if(SetER(ExeSelectDBF(strTName, *pnFP)))
// 			{
// 				WriteLogE("ExeUpdateDBF fail : old item read fail : [%s][%.8x]", strTName, *pnFP);
// 				nRtn = GetER();
// 				break;
// 			}
// 
// 			{
// 				tDTIH	= pMember->m_tCurDBFIH;
// 				tDTIH.nCurBlockSize	= 0;
// 			}
// 			ChkOpTickCntLog();
// 		}
// 		
// 		{
// 			while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
// 			{
// 				PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);
// 
// 				UINT32 nInitLen = 0;
// 				switch(pdtc->nMemberType)
// 				{
// 					case ASI_DBF_MEMBER_TYPE_UINT16:		nInitLen = sizeof(UINT16);			break;
// 					case ASI_DBF_MEMBER_TYPE_UINT32:		nInitLen = sizeof(UINT32);			break;
// 					case ASI_DBF_MEMBER_TYPE_UINT64:		nInitLen = sizeof(UINT64);			break;
// 					case ASI_DBF_MEMBER_TYPE_STRING:		
// 					{
// 						PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
// 
// 						if(!pdtisv)		nInitLen = max(pdtc->nInitStrLen, GetField(pdts, nItemIdx)->nRtnLen);
// 						else			nInitLen = max(pdtc->nInitStrLen, pdtisv->strString.GetLength());
// 						break;
// 					}
// 					case ASI_DBF_MEMBER_TYPE_LSTRING:		
// 					{
// 						PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
// 
// 						if(!pdtisv)		nInitLen = max(GetLStrLen(pdtc->nInitStrLen), (INT32)GetField(pdts, nItemIdx)->nRtnLen);
// 						else			nInitLen = max(GetLStrLen(pdtc->nInitStrLen), pdtisv->strString.GetLength());
// 						break;
// 					}
// 					case ASI_DBF_MEMBER_TYPE_OBJECT:		
// 					{
// 						PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
// 
// 						if(!pdtisv)		nInitLen = max(pdtc->nInitStrLen, (UINT16)GetField(pdts, nItemIdx)->nRtnLen);
// 						else			nInitLen = max(pdtc->nInitStrLen, pdtisv->nObjLen);
// 						break;
// 					}
// 				}			
// 				SetBuffLength(tHLenMap, nInitLen, nItemIdx);
// 				tDTIH.nCurBlockSize += nInitLen;
// 				nItemIdx++;
// 			}
// 		}
// 
// 		UINT64	nPriKey = 0;
// 		CString strPriKey = "";
// 		PBYTE lpBuff = NULL;
// 		lpBuff = (PBYTE)MakeCommonBuff(tDTIH.nCurBlockSize);
// 
// 		if(!lpBuff)
// 		{
// 			WriteLogE("ExeUpdateDBF fail : memory allocation fail : [%s][%.8x][%u]", strTName, *pnFP, tDTIH.nCurBlockSize);
// 			nRtn = ASI_DBF_ERROR_TYPE_MEM_ALLOC_FAIL;
// 			break;
// 		}
// 
// 		{
// 			PBYTE lpPosBuff = lpBuff;
// 			nItemIdx = 0;
// 			while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
// 			{
// 				PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);
// 				PDBF_ITEM_RTN pdir = GetField(pdts, nItemIdx);
// 				PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
// 				
// 				switch(pdtc->nMemberType)
// 				{
// 					case ASI_DBF_MEMBER_TYPE_UINT16:
// 					{
// 						if(!pdtisv)		memcpy(lpPosBuff, pdir->pRtnUINT16, sizeof(UINT16));
// 						else			memcpy(lpPosBuff, &(pdtisv->nUINT16), sizeof(UINT16));
// 						lpPosBuff += sizeof(UINT16);
// 						break;
// 					}
// 					case ASI_DBF_MEMBER_TYPE_UINT32:
// 					{
// 						if(!pdtisv)		memcpy(lpPosBuff, pdir->pRtnUINT32, sizeof(UINT32));
// 						else			memcpy(lpPosBuff, &(pdtisv->nUINT32), sizeof(UINT32));
// 						lpPosBuff += sizeof(UINT32);
// 						break;
// 					}
// 					case ASI_DBF_MEMBER_TYPE_UINT64:
// 					{
// 						if(!pdtisv)		memcpy(lpPosBuff, pdir->pRtnUINT64, sizeof(UINT64));
// 						else			memcpy(lpPosBuff, &(pdtisv->nUINT64), sizeof(UINT64));
// 						lpPosBuff += sizeof(UINT64);
// 						break;
// 					}
// 					case ASI_DBF_MEMBER_TYPE_STRING:		
// 					{
// 						if(!pdtisv)
// 						{
// 							memcpy(lpPosBuff, pdir->szRtnString, pdir->nRtnLen);
// 							lpPosBuff += max(pdtc->nInitStrLen, pdir->nRtnLen);
// 						}
// 						else									
// 						{
// 							memcpy(lpPosBuff, pdtisv->strString, pdtisv->strString.GetLength());
// 							lpPosBuff += max(pdtc->nInitStrLen, pdtisv->strString.GetLength());
// 						}
// 						break;
// 					}	
// 					case ASI_DBF_MEMBER_TYPE_LSTRING:		
// 					{
// 						if(!pdtisv)
// 						{
// 							memcpy(lpPosBuff, pdir->szRtnString, pdir->nRtnLen);
// 							lpPosBuff += max(GetLStrLen(pdtc->nInitStrLen), (INT32)pdir->nRtnLen);
// 						}
// 						else									
// 						{
// 							memcpy(lpPosBuff, pdtisv->strString, pdtisv->strString.GetLength());
// 							lpPosBuff += max(GetLStrLen(pdtc->nInitStrLen), pdtisv->strString.GetLength());
// 						}
// 						break;
// 					}			
// 					case ASI_DBF_MEMBER_TYPE_OBJECT:		
// 					{
// 						if(!pdtisv)
// 						{
// 							memcpy(lpPosBuff, pdir->lpRtnObject, pdir->nRtnLen);
// 							lpPosBuff += max(pdtc->nInitStrLen, pdir->nRtnLen);
// 						}
// 						else									
// 						{
// 							memcpy(lpPosBuff, pdtisv->lpObject, pdtisv->nObjLen);
// 							lpPosBuff += max(pdtc->nInitStrLen, pdtisv->nObjLen);
// 							pdtisv->nObjLen = 0;
// 						}
// 						break;
// 					}
// 				}
// 
// 				if(pdtc->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY)
// 				{
// 					switch(pdtc->nMemberType)
// 					{
// 						case ASI_DBF_MEMBER_TYPE_UINT16:
// 						{
// 							if(!pdtisv)		nPriKey = *(pdir->pRtnUINT16);
// 							else			nPriKey = pdtisv->nUINT16;
// 							break;
// 						}
// 						case ASI_DBF_MEMBER_TYPE_UINT32:
// 						{
// 							if(!pdtisv)		nPriKey = *(pdir->pRtnUINT32);
// 							else			nPriKey = pdtisv->nUINT32;
// 							break;
// 						}
// 						case ASI_DBF_MEMBER_TYPE_UINT64:
// 						{
// 							if(!pdtisv)		nPriKey = *(pdir->pRtnUINT64);
// 							else			nPriKey = pdtisv->nUINT64;
// 							break;
// 						}
// 						case ASI_DBF_MEMBER_TYPE_STRING:		
// 						{
// 							if(!pdtisv)		strPriKey = CString(pdir->szRtnString, pdir->nRtnLen);
// 							else			strPriKey = pdtisv->strString;
// 							break;
// 						}
// 						case ASI_DBF_MEMBER_TYPE_LSTRING:		
// 						{
// 							if(!pdtisv)		strPriKey = CString(pdir->szRtnString, pdir->nRtnLen);
// 							else			strPriKey = pdtisv->strString;
// 							break;
// 						}
// 						case ASI_DBF_MEMBER_TYPE_OBJECT:		
// 						{						
// 							break;
// 						}
// 					}	
// 
// 				}
// 				nItemIdx++;
// 			}
// 			pMember->m_tTItemSetValueMapMap.erase(strTName);	
// 		}
// 
// 		ChkOpTickCntLog();
// 		{
// 			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);		
// 
// 			if(SetER(UpdateDBF_TItem(pdts, &tDTIH, tHLenMap, lpBuff)))
// 			{
// 				FreeCommonBuff(&lpBuff);
// 				FreeCommonBuff(&(pMember->m_pCurItemBuff));
// 				nRtn = GetER();
// 				break;
// 			}
// 			FreeCommonBuff(&lpBuff);		
// 			FreeCommonBuff(&(pMember->m_pCurItemBuff));
// 		}
// 
// 		if(pnFP)		*pnFP = tDTIH.nCurFP;
// 
// 		if(nPriKey)
// 		{
// 			pdts->_apply_fp(nPriKey, tDTIH.nCurFP);
// 		}
// 		else if(strPriKey.IsEmpty() == FALSE)
// 		{
// 			pdts->_apply_fp(strPriKey, tDTIH.nCurFP);
// 		}
// 
// 		ChkOpTickCntLog(200);
// 		WriteLogOpTickCnt(strTName);
// 
// 	} while (FALSE);
// 
// 	m_tMutex.UnLock();
// 	return nRtn;
// }
// //---------------------------------------------------------------------

INT32		CASIDBFUtil::ExeInsertDBF(CString strTName, PUINT64 pnNewFP, PUINT64 pnLastID)
{
	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	m_tMutex.Lock();
	do 
	{
		CASIDBFMember* pMember = GetMember();

		PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
		if(!pdts)
		{
			WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
			break;
		}

		InitOpTickCntLog();
		ChkOpTickCntLog(1, ASI_DBFQ_QUERY_COMMAND_TYPE_INSERT);
		ChkOpTickCntLog(2);
		
		DBF_TABLE_ITEM_HEADER tDTIH;	
		TMapID64 tHLenMap;

		UINT32 nItemIdx = 0;
		while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
		{
			PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);
			
			UINT32 nInitLen = 0;
			switch(pdtc->nMemberType)
			{
				case ASI_DBF_MEMBER_TYPE_UINT16:		nInitLen = sizeof(UINT16);			break;
				case ASI_DBF_MEMBER_TYPE_UINT32:		nInitLen = sizeof(UINT32);			break;
				case ASI_DBF_MEMBER_TYPE_UINT64:		nInitLen = sizeof(UINT64);			break;
				case ASI_DBF_MEMBER_TYPE_STRING:
				{
					PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
					
					if(!pdtisv)				nInitLen = pdtc->nInitStrLen;
					else					nInitLen = max(pdtisv->nBuffLen, pdtc->nInitStrLen);
					break;
				}
				case ASI_DBF_MEMBER_TYPE_LSTRING:
				{
					PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);

					if(!pdtisv)				nInitLen = GetLStrLen(pdtc->nInitStrLen);
					else					nInitLen = max((INT32)pdtisv->nBuffLen, GetLStrLen(pdtc->nInitStrLen));
					break;
				}
				case ASI_DBF_MEMBER_TYPE_OBJECT:
				{
					PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
					
					if(!pdtisv)				nInitLen = pdtc->nInitStrLen;
					else					nInitLen = max(pdtisv->nBuffLen, pdtc->nInitStrLen);
					break;
				}
			}			
			SetBuffLength(tHLenMap, nInitLen, nItemIdx);
			tDTIH.nCurBlockSize += nInitLen;
			nItemIdx++;
		}

		ChkOpTickCntLog();

		PBYTE lpBuff = (PBYTE)MakeCommonBuff(tDTIH.nCurBlockSize);
		if(!lpBuff)	
		{
			nRtn = -2;
			break;
		}

		UINT32 nPriKeyType = 0;
		CString strPriKey = "";

		PBYTE lpPosBuff = lpBuff;
		nItemIdx = 0;
		while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
		{
			PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);
			PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
					
			switch(pdtc->nMemberType)
			{
				case ASI_DBF_MEMBER_TYPE_UINT16:
				{
					if(!pdtisv)						memcpy(lpPosBuff, &(pdtc->nDefaultValue), sizeof(UINT16));
					else							memcpy(lpPosBuff, pdtisv->pUINT16, sizeof(UINT16));
					lpPosBuff += sizeof(UINT16);
					break;
				}
				case ASI_DBF_MEMBER_TYPE_UINT32:
				{
					if(!pdtisv)						memcpy(lpPosBuff, &(pdtc->nDefaultValue), sizeof(UINT32));
					else							memcpy(lpPosBuff, pdtisv->pUINT32, sizeof(UINT32));
					lpPosBuff += sizeof(UINT32);
					break;
				}
				case ASI_DBF_MEMBER_TYPE_UINT64:
				{
					if(!pdtisv)						memcpy(lpPosBuff, &(pdtc->nDefaultValue), sizeof(UINT64));
					else							memcpy(lpPosBuff, pdtisv->pUINT64, sizeof(UINT64));
					lpPosBuff += sizeof(UINT64);
					break;
				}
				case ASI_DBF_MEMBER_TYPE_STRING:		
				{
					if(!pdtisv)						lpPosBuff += pdtc->nInitStrLen;
					else	
					{
						memcpy(lpPosBuff, pdtisv->pString, pdtisv->nBuffLen);				
						lpPosBuff += max(pdtisv->nBuffLen, pdtc->nInitStrLen);
					}
					break;
				}
				case ASI_DBF_MEMBER_TYPE_LSTRING:		
				{
					if(!pdtisv)						lpPosBuff += GetLStrLen(pdtc->nInitStrLen);
					else	
					{
						memcpy(lpPosBuff, pdtisv->pString, pdtisv->nBuffLen);				
						lpPosBuff += max((INT32)pdtisv->nBuffLen, GetLStrLen(pdtc->nInitStrLen));
					}
					break;
				}
				case ASI_DBF_MEMBER_TYPE_OBJECT:		
				{
					if(!pdtisv)						lpPosBuff += pdtc->nInitStrLen;
					else	
					{
						memcpy(lpPosBuff, pdtisv->lpObject, pdtisv->nBuffLen);				
						lpPosBuff += max(pdtisv->nBuffLen, pdtc->nInitStrLen);
						pdtisv->nBuffLen = 0;
					}
					break;
				}
			}

			if(pdtc->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY)
			{
				switch(pdtc->nMemberType)
				{
					case ASI_DBF_MEMBER_TYPE_UINT16:				
					case ASI_DBF_MEMBER_TYPE_UINT32:
					case ASI_DBF_MEMBER_TYPE_UINT64:
					{
						nPriKeyType = ASI_DBF_MEMBER_TYPE_UINT64;
						break;
					}
					case ASI_DBF_MEMBER_TYPE_STRING:		
					{
						nPriKeyType = ASI_DBF_MEMBER_TYPE_STRING;
						if(pdtisv)	strPriKey = CString(pdtisv->pString, pdtisv->nBuffLen);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_LSTRING:		
					{
						nPriKeyType = ASI_DBF_MEMBER_TYPE_LSTRING;
						if(pdtisv)	strPriKey = CString(pdtisv->pString, pdtisv->nBuffLen);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_OBJECT:		
					{
						nPriKeyType = ASI_DBF_MEMBER_TYPE_OBJECT;
						//if(pdtisv)	strPriKey = pdtisv->lpObject;
						break;
					}
				}
			}
			nItemIdx++;
		}
		pMember->m_tTItemSetValueMapMap.erase(strTName);

		ChkOpTickCntLog();

		if(SetER(InsertDBF_TItem(pdts, &tDTIH, tHLenMap, lpBuff)))
		{
			FreeCommonBuff(&lpBuff);
			nRtn = GetER();
			break;
		}

		ChkOpTickCntLog(200);
		WriteLogOpTickCnt(strTName);


		FreeCommonBuff(&lpBuff);
		if(pnNewFP)		*pnNewFP	= tDTIH.nCurFP;
		if(pnLastID)	*pnLastID	= pMember->m_nLastInsID;

		switch(nPriKeyType)
		{
			case ASI_DBF_MEMBER_TYPE_UINT16:
			case ASI_DBF_MEMBER_TYPE_UINT32:
			case ASI_DBF_MEMBER_TYPE_UINT64:
			{
				pdts->_apply_fp(pdts->tDT.tHInfo.nLastID, tDTIH.nCurFP);
				break;
			}
			case ASI_DBF_MEMBER_TYPE_STRING:
			{
				pdts->_apply_fp(strPriKey, tDTIH.nCurFP);
				break;
			}
			case ASI_DBF_MEMBER_TYPE_LSTRING:
			{
				pdts->_apply_fp(strPriKey, tDTIH.nCurFP);
				break;
			}
		}

	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------

INT32				CASIDBFUtil::ExeUpdateDBF(CString strTName, PUINT64 pnFP)
{
	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	m_tMutex.Lock();
	do 
	{
		if(!pnFP)
		{
			nRtn = -1;
			break;
		}

		if(*pnFP == 0)
		{
			WriteLogE("ExeUpdateDBF fail : FP value is 0 : [%s][%.8x]", (char *)(LPCTSTR)m_strDBFName, *pnFP);
			nRtn = -2;
			break;
		}
		UINT32 nTickA = GetTickCount();
		InitOpTickCntLog();
		ChkOpTickCntLog(1, ASI_DBFQ_QUERY_COMMAND_TYPE_UPDATE);
		ChkOpTickCntLog(2, nTickA);
		ChkOpTickCntLog();

		CASIDBFMember* pMember = GetMember();
		PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
		if(!pdts)
		{
			WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
			break;
		}

		ChkOpTickCntLog();

		DBF_TABLE_ITEM_HEADER tDTIH;
		TMapID64 tHLenMap;
		UINT32 nItemIdx = 0;

		UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);
		
		{
			ChkOpTickCntLog();
			if(SetER(ExeSelectDBF(strTName, *pnFP)))
			{
				WriteLogE("ExeUpdateDBF fail : old item read fail : [%s][%.8x]", (char *)(LPCTSTR)strTName, *pnFP);
				nRtn = GetER();
				break;
			}

			{
				tDTIH	= pMember->m_tCurDBFIH;
				tDTIH.nCurBlockSize	= 0;
			}
			ChkOpTickCntLog();
		}
		
		{
			while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
			{
				PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);

				UINT32 nInitLen = 0;
				switch(pdtc->nMemberType)
				{
					case ASI_DBF_MEMBER_TYPE_UINT16:		nInitLen = sizeof(UINT16);			break;
					case ASI_DBF_MEMBER_TYPE_UINT32:		nInitLen = sizeof(UINT32);			break;
					case ASI_DBF_MEMBER_TYPE_UINT64:		nInitLen = sizeof(UINT64);			break;
					case ASI_DBF_MEMBER_TYPE_STRING:		
					{
						PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);

						if(!pdtisv)		nInitLen = max(pdtc->nInitStrLen, GetField(pdts, nItemIdx)->nRtnLen);
						else			nInitLen = max(pdtc->nInitStrLen, pdtisv->nBuffLen);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_LSTRING:		
					{
						PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);

						if(!pdtisv)		nInitLen = max(GetLStrLen(pdtc->nInitStrLen), (INT32)GetField(pdts, nItemIdx)->nRtnLen);
						else			nInitLen = max(GetLStrLen(pdtc->nInitStrLen), (INT32)pdtisv->nBuffLen);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_OBJECT:		
					{
						PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);

						if(!pdtisv)		nInitLen = max(pdtc->nInitStrLen, (UINT16)GetField(pdts, nItemIdx)->nRtnLen);
						else			nInitLen = max(pdtc->nInitStrLen, pdtisv->nBuffLen);
						break;
					}
				}			
				SetBuffLength(tHLenMap, nInitLen, nItemIdx);
				tDTIH.nCurBlockSize += nInitLen;
				nItemIdx++;
			}
		}

		UINT64	nPriKey = 0;
		CString strPriKey = "";
		PBYTE lpBuff = NULL;
		lpBuff = (PBYTE)MakeCommonBuff(tDTIH.nCurBlockSize);

		if(!lpBuff)
		{
			WriteLogE("ExeUpdateDBF fail : memory allocation fail : [%s][%.8x][%u]", (char *)(LPCTSTR)strTName, *pnFP, tDTIH.nCurBlockSize);
			nRtn = ASI_DBF_ERROR_TYPE_MEM_ALLOC_FAIL;
			break;
		}

		{
			PBYTE lpPosBuff = lpBuff;
			nItemIdx = 0;
			while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
			{
				PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);
				PDBF_ITEM_RTN pdir = GetField(pdts, nItemIdx);
				PDBF_TABLE_ITEM_SET_VALUE pdtisv = FindPreSetTItemValue(strTName, pdtc->szColName);
				
				switch(pdtc->nMemberType)
				{
					case ASI_DBF_MEMBER_TYPE_UINT16:
					{
						if(!pdtisv)		memcpy(lpPosBuff, pdir->pRtnUINT16, sizeof(UINT16));
						else			memcpy(lpPosBuff, pdtisv->pUINT16, sizeof(UINT16));
						lpPosBuff += sizeof(UINT16);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_UINT32:
					{
						if(!pdtisv)		memcpy(lpPosBuff, pdir->pRtnUINT32, sizeof(UINT32));
						else			memcpy(lpPosBuff, pdtisv->pUINT32, sizeof(UINT32));
						lpPosBuff += sizeof(UINT32);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_UINT64:
					{
						if(!pdtisv)		memcpy(lpPosBuff, pdir->pRtnUINT64, sizeof(UINT64));
						else			memcpy(lpPosBuff, pdtisv->pUINT64, sizeof(UINT64));
						lpPosBuff += sizeof(UINT64);
						break;
					}
					case ASI_DBF_MEMBER_TYPE_STRING:		
					{
						if(!pdtisv)
						{
							memcpy(lpPosBuff, pdir->szRtnString, pdir->nRtnLen);
							lpPosBuff += max(pdtc->nInitStrLen, pdir->nRtnLen);
						}
						else									
						{
							memcpy(lpPosBuff, pdtisv->pString, pdtisv->nBuffLen);
							lpPosBuff += max(pdtc->nInitStrLen, pdtisv->nBuffLen);
						}
						break;
					}	
					case ASI_DBF_MEMBER_TYPE_LSTRING:		
					{
						if(!pdtisv)
						{
							memcpy(lpPosBuff, pdir->szRtnString, pdir->nRtnLen);
							lpPosBuff += max(GetLStrLen(pdtc->nInitStrLen), (INT32)pdir->nRtnLen);
						}
						else									
						{
							memcpy(lpPosBuff, pdtisv->pString, pdtisv->nBuffLen);
							lpPosBuff += max(GetLStrLen(pdtc->nInitStrLen), (INT32)pdtisv->nBuffLen);
						}
						break;
					}			
					case ASI_DBF_MEMBER_TYPE_OBJECT:		
					{
						if(!pdtisv)
						{
							memcpy(lpPosBuff, pdir->lpRtnObject, pdir->nRtnLen);
							lpPosBuff += max(pdtc->nInitStrLen, pdir->nRtnLen);
						}
						else									
						{
							memcpy(lpPosBuff, pdtisv->lpObject, pdtisv->nBuffLen);
							lpPosBuff += max(pdtc->nInitStrLen, pdtisv->nBuffLen);
							pdtisv->nBuffLen = 0;
						}
						break;
					}
				}

				if(pdtc->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY)
				{
					switch(pdtc->nMemberType)
					{
						case ASI_DBF_MEMBER_TYPE_UINT16:
						{
							if(!pdtisv)		nPriKey = *(pdir->pRtnUINT16);
							else			nPriKey = *(pdtisv->pUINT16);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_UINT32:
						{
							if(!pdtisv)		nPriKey = *(pdir->pRtnUINT32);
							else			nPriKey = *(pdtisv->pUINT32);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_UINT64:
						{
							if(!pdtisv)		nPriKey = *(pdir->pRtnUINT64);
							else			nPriKey = *(pdtisv->pUINT64);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_STRING:		
						{
							if(!pdtisv)		strPriKey = CString(pdir->szRtnString, pdir->nRtnLen);
							else			strPriKey = CString(pdtisv->pString, pdtisv->nBuffLen);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_LSTRING:		
						{
							if(!pdtisv)		strPriKey = CString(pdir->szRtnString, pdir->nRtnLen);
							else			strPriKey = CString(pdtisv->pString, pdtisv->nBuffLen);
							break;
						}
						case ASI_DBF_MEMBER_TYPE_OBJECT:		
						{						
							break;
						}
					}	

				}
				nItemIdx++;
			}
			pMember->m_tTItemSetValueMapMap.erase(strTName);	
		}

		ChkOpTickCntLog();
		{
			SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);		

			if(SetER(UpdateDBF_TItem(pdts, &tDTIH, tHLenMap, lpBuff)))
			{
				FreeCommonBuff(&lpBuff);
				FreeCommonBuff(&(pMember->m_pCurItemBuff));
				nRtn = GetER();
				break;
			}
			FreeCommonBuff(&lpBuff);		
			FreeCommonBuff(&(pMember->m_pCurItemBuff));
		}

		if(pnFP)		*pnFP = tDTIH.nCurFP;

		if(nPriKey)
		{
			pdts->_apply_fp(nPriKey, tDTIH.nCurFP);
		}
		else if(strPriKey.IsEmpty() == FALSE)
		{
			pdts->_apply_fp(strPriKey, tDTIH.nCurFP);
		}

		ChkOpTickCntLog(200);
		WriteLogOpTickCnt(strTName);

	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ExeDeleteDBF(CString strTName, UINT64 nFP)
{
	INT32 nRtn = ASI_DBF_ERROR_TYPE_SUCCESS;
	m_tMutex.Lock();
	do 
	{
		PDBF_TABLE_SEARCH pdts = FindDBF_Table(strTName);
		if(!pdts)
		{
			WriteLogN("not find table info : [%s]", (char *)(LPCSTR)strTName);
			nRtn = ASI_DBF_ERROR_TYPE_V2_NOT_FIND_DBF_TABLE;
			break;
		}

		ChkOpTickCntLog();

		DBF_TABLE_ITEM_HEADER tDTIH;
		UINT32 nItemIdx = 0;

		CASIDBFMember* pMember = GetMember();
		UINT64 nCurFP = GetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE);

		{
			ChkOpTickCntLog();
			if(SetER(ExeSelectDBF(strTName, nFP)))
			{
				WriteLogE("ExeDeleteDBF fail : old item read fail : [%s][%.8x]", (char *)(LPCTSTR)strTName, nFP);
				SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);	
				nRtn = GetER();
				break;
			}

			{
				tDTIH	= pMember->m_tCurDBFIH;
			}
			ChkOpTickCntLog();
		}

		UINT64	nPriKey = 0;
		CString strPriKey = "";
		
		{
			nItemIdx = 0;
			while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
			{
				PDBF_TABLE_COLUMN pdtc = &(pdts->tDT.tColInfo[nItemIdx]);
				
				if(pdtc->nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY)
				{
					PDBF_ITEM_RTN pdir = GetField(pdts, nItemIdx);

					switch(pdtc->nMemberType)
					{
						case ASI_DBF_MEMBER_TYPE_UINT16:
						{
							pdts->_del_fp(*(pdir->pRtnUINT16));
							break;
						}
						case ASI_DBF_MEMBER_TYPE_UINT32:
						{
							pdts->_del_fp(*(pdir->pRtnUINT32));
							break;
						}
						case ASI_DBF_MEMBER_TYPE_UINT64:
						{
							pdts->_del_fp(*(pdir->pRtnUINT64));
							break;
						}
						case ASI_DBF_MEMBER_TYPE_STRING:		
						{
							pdts->_del_fp(CString(pdir->szRtnString, pdir->nRtnLen));
							break;
						}
						case ASI_DBF_MEMBER_TYPE_LSTRING:		
						{
							pdts->_del_fp(CString(pdir->szRtnString, pdir->nRtnLen));
							break;
						}
						case ASI_DBF_MEMBER_TYPE_OBJECT:		
						{						
							break;
						}
					}
					break;
				}
				nItemIdx++;
			}
		}
		SetFilePointerEx(ASI_DBF_FILE_HANDLE_WRITE, nCurFP);
		
		nRtn = DeleteDBF_TItem(pdts, nFP);
	} while (FALSE);

	m_tMutex.UnLock();
	return nRtn;	
}
//---------------------------------------------------------------------

INT32		CASIDBFUtil::ApplyKeyToFPInfo(PDBF_TABLE_SEARCH pdts)
{
	if(!pdts)
	{
		WriteLogN("invalid table info");
		return -1;
	}

	INT32 nRtn = -10;
	UINT32 nItemIdx = 0;
	UINT32 nBuffPos = 0;
	while(pdts->tDT.tHInfo.nColNumber > nItemIdx)
	{
		DBF_TABLE_COLUMN tDTC = pdts->tDT.tColInfo[nItemIdx];
		
		if((tDTC.nColOption & ASI_DBF_COL_OPTION_FLAG_PRI_KEY))
		{
			UINT64 nKey = 0;
			CString strKey = "";

			PDBF_ITEM_RTN pDBFIR = GetField(pdts, nItemIdx);

			switch(pDBFIR->nRtnType)
			{
				case ASI_DBF_MEMBER_TYPE_UINT16:
				{
					nKey = *(pDBFIR->pRtnUINT16);
					break;
				}
				case ASI_DBF_MEMBER_TYPE_UINT32:
				{
					nKey = *(pDBFIR->pRtnUINT32);
					break;
				}
				case ASI_DBF_MEMBER_TYPE_UINT64:
				{
					nKey = *(pDBFIR->pRtnUINT64);
					break;
				}
				case ASI_DBF_MEMBER_TYPE_STRING:
				{
					strKey = CString(pDBFIR->szRtnString, pDBFIR->nRtnLen);
					break;
				}
				case ASI_DBF_MEMBER_TYPE_LSTRING:
				{
					strKey = CString(pDBFIR->szRtnString, pDBFIR->nRtnLen);
					break;
				}
				case ASI_DBF_MEMBER_TYPE_OBJECT:
				{
					break;
				}
			}

			switch(pDBFIR->nRtnType)
			{
				case ASI_DBF_MEMBER_TYPE_UINT16:				
				case ASI_DBF_MEMBER_TYPE_UINT32:
				case ASI_DBF_MEMBER_TYPE_UINT64:
				{
					pdts->_apply_fp(nKey, GetFieldFP());
					break;
				}
				case ASI_DBF_MEMBER_TYPE_STRING:
				{
					pdts->_apply_fp(strKey, GetFieldFP());
					break;
				}
				case ASI_DBF_MEMBER_TYPE_LSTRING:
				{
					pdts->_apply_fp(strKey, GetFieldFP());
					break;
				}
				case ASI_DBF_MEMBER_TYPE_OBJECT:
				{
					break;
				}
			}

			nRtn = 0;
			break;
		}
		else
		{
			nItemIdx += 1;
		}
	}
	return nRtn;
}
