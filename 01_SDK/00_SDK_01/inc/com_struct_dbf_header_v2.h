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

#ifndef DBF_STRUCT_DBF_HEADER_V2_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DBF_STRUCT_DBF_HEADER_V2_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

#pragma pack(1)

typedef struct _dbf_database_ver
{
	_dbf_database_ver()
	{
		nVersion		= 0;
		nDBSize			= 0;
		memset(nResBuff, 0, 256);
	}
	UINT32	nVersion;
	UINT32	nDBSize;
	UINT8	nResBuff[256];
}DBF_DATABASE_VER, *PDBF_DATABASE_VER; 

typedef struct _dbf_database_info
{
	_dbf_database_info()
	{
		nLastTID		= 0;
		nCreateDT		= 0;
		nAutoRestore	= 0;
		memset(szLastProcName, 0, ASI_DBF_LAST_PROC_PATH_LEN);
		memset(nResBuff, 0, 256);
	}
	UINT32	nLastTID;
	UINT32	nCreateDT;
	CHAR	szLastProcName[ASI_DBF_LAST_PROC_PATH_LEN];
	UINT32	nAutoRestore;
	UINT8	nResBuff[252];
}DBF_DATABASE_INFO, *PDBF_DATABASE_INFO;

typedef struct _dbf_database_auth
{
	_dbf_database_auth()
	{
		nAuthChkType	= 0;
		nDefEncType		= 0;
		nDefCompType	= 0;
		memset(szUserID, 0, ASI_DBF_AUTH_USER_ID_MAX_LEN);
		memset(szUserPW, 0, ASI_DBF_AUTH_USER_PW_MAX_LEN);
		memset(nResBuff, 0, 252);
	}
	UINT32	nAuthChkType;
	UINT32	nDefEncType;
	UINT32	nDefCompType;
	UINT8	szUserID[ASI_DBF_AUTH_USER_ID_MAX_LEN];
	UINT8	szUserPW[ASI_DBF_AUTH_USER_PW_MAX_LEN];
	UINT8	nResBuff[252];
}DBF_DATABASE_AUTH, *PDBF_DATABASE_AUTH;
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
typedef struct _dbf_database
{
	DBF_DATABASE_VER		tVer;
	DBF_DATABASE_INFO		tInfo;
	DBF_DATABASE_AUTH		tAuth;
}DBF_DATABASE, *PDBF_DATABASE;
//--------------------------------------------------------------------------

typedef struct _dbf_table_info
{
	_dbf_table_info()
	{
		memset(szName, 0, ASI_DBF_TABLE_NAME_MAX_LEN);
		
		nTID			= 0;
		nTType			= 0;
		nSchemaVer		= 0;
		nLastID			= 0;
		nItemCnt		= 0;
		nItemTCnt		= 0;
		nModifyDate		= 0;
		nColNumber		= 0;

		nDelFlag		= 0;
		nHeadTableFP	= 0;
		nTailTableFP	= 0;
		nFirstItemFP	= 0;
		nLastItemFP		= 0;
		nCheckItemFP	= 0;
		nCheckItemEndFP	= 0;

		memset(nResBuff, 0, 256);
	}
	CHAR				szName[ASI_DBF_TABLE_NAME_MAX_LEN];
	UINT32				nTID;
	UINT32				nTType;
	UINT32				nSchemaVer;
	UINT64				nLastID;
	UINT32				nItemCnt;
	UINT32				nItemTCnt;
	UINT32				nModifyDate;
	UINT32				nColNumber;

	UINT32				nDelFlag;
	UINT64				nHeadTableFP;
	UINT64				nTailTableFP;
	UINT64				nFirstItemFP;
	UINT64				nLastItemFP;
	UINT64				nCheckItemFP;
	UINT64				nCheckItemEndFP;
	UINT8				nResBuff[256];
}DBF_TABLE_INFO, *PDBF_TABLE_INFO;
//--------------------------------------------------------------------------

typedef struct _dbf_table_column
{
	_dbf_table_column()
	{
		nMemberType		= 0;
		nInitStrLen		= 0;
		nDefaultValue	= 0;
		nColOption		= 0;
		memset(szColName, 0, ASI_DBF_MAX_LEN_TYPE_COL_NAME);
	}
	UINT16		nMemberType;
	UINT16		nInitStrLen;
	UINT64		nDefaultValue;
	UINT32		nColOption;
	CHAR		szColName[ASI_DBF_MAX_LEN_TYPE_COL_NAME];
}DBF_TABLE_COLUMN, *PDBF_TABLE_COLUMN;

typedef list<DBF_TABLE_COLUMN>			TListTColumSetValue;
typedef TListTColumSetValue::iterator	TListTColumSetValueItor;

typedef map<CString, TListTColumSetValue>	TMapTColumnSetValueList; 
typedef TMapTColumnSetValueList::iterator	TMapTColumnSetValueListItor;
//--------------------------------------------------------------------------

typedef struct _dbf_table
{
	_dbf_table()
    {    	
		memset(tColInfo, 0, sizeof(DBF_TABLE_COLUMN) * ASI_DBF_MAX_COLUMN_NUMBER);
		nCurTableFP = 0;
	}
	UINT64				nCurTableFP;
	DBF_TABLE_INFO		tHInfo;
    DBF_TABLE_COLUMN	tColInfo[ASI_DBF_MAX_COLUMN_NUMBER];
}DBF_TABLE, *PDBF_TABLE;

typedef list<DBF_TABLE>						TListDBFTable;
typedef TListDBFTable::iterator				TListDBFTableItor;
typedef TListDBFTable::reverse_iterator		TListDBFTableRItor;
//--------------------------------------------------------------------------

typedef struct _dbf_select_work
{
	_dbf_select_work()
	{
		nDirection			= 0;
		nNextStop			= 0;
		nSelArrayBuffSize	= 100000;
		m_nSelColNum		= 0;
		memset(m_tSelColIDArray, 0, sizeof(INT32) * ASI_DBF_MAX_COLUMN_NUMBER);
	}

	UINT32		nDirection;
	UINT32		nNextStop;
	UINT32		nSelArrayBuffSize;
	CString		strTable;

	UINT32		m_nSelColNum;
	INT32		m_tSelColIDArray[ASI_DBF_MAX_COLUMN_NUMBER];


}DBF_SELECT_WORK, *PDBF_SELECT_WORK;

typedef list<DBF_SELECT_WORK>					TListDBFSelectWork;
typedef TListDBFSelectWork::iterator			TListDBFSelectWorkItor;
typedef TListDBFSelectWork::reverse_iterator	TListDBFSelectWorkRItor;
//--------------------------------------------------------------------------

typedef struct _dbf_select_item
{
	_dbf_select_item()
	{
		pItemBuffer		= 0;
		nCurFP			= 0;
		nDIRArrayNum	= 0;
	}

	PBYTE				pItemBuffer;
	UINT64				nCurFP;
	TMapID64			tItemLenMap;
	INT32				nDIRArrayNum;
	DBF_ITEM_RTN		tDIRArray[ASI_DBF_MAX_COLUMN_NUMBER];
}DBF_SELECT_ITEM, *PDBF_SELECT_ITEM;

typedef list<DBF_SELECT_ITEM>			TListDBFSelectItem;
typedef TListDBFSelectItem::iterator	TListDBFSelectItemItor;

typedef map<UINT32, DBF_SELECT_ITEM>	TMapDBFSelectItem;
typedef TMapDBFSelectItem::iterator		TMapDBFSelectItemItor;
//--------------------------------------------------------------------------

typedef struct _dbf_table_search 
{
	_dbf_table_search()
	{
		m_nItemHLen = 0;
	}

	~_dbf_table_search()
	{
		
	}

	void	_clear()
	{
		m_tColNameIdxMap.clear();
		m_tPKeyFPMap.clear();
		m_tSPKeyFPMap.clear();
	}

	INT32	_find_idx(CString strName)
	{
		TMapCStrIDItor find = m_tColNameIdxMap.find(strName);
		if(find == m_tColNameIdxMap.end())
		{
			strName.MakeLower();
			if((find = m_tColNameIdxMap.find(strName)) == m_tColNameIdxMap.end())
				return -1;

		}
		return find->second;
	}
	INT32	_add_idx(CString strName, INT32 nIdx)
	{
		strName.MakeLower();
		m_tColNameIdxMap[strName] = nIdx;
		return 0;
	}
	//--------------------------------------------------------------------

	UINT64	_find_fp(UINT64 nKey)
	{
		TMapID64Itor find = m_tPKeyFPMap.find(nKey);
		if(find == m_tPKeyFPMap.end())
		{
			return 0;
		}
		return find->second;
	}
	INT32	_apply_fp(UINT64 nKey, UINT64 nFP)
	{
		m_tPKeyFPMap[nKey] = nFP;
		return 0;
	}
	INT32	_del_fp(UINT64 nKey)
	{
		m_tPKeyFPMap.erase(nKey);
		return 0;
	}
	//--------------------------------------------------------------------

	UINT64	_find_fp(CString strKey)
	{
		TMapCStrID64Itor find = m_tSPKeyFPMap.find(strKey);
		if(find == m_tSPKeyFPMap.end())
		{
			return 0;
		}
		return find->second;
	}
	INT32	_apply_fp(CString strKey, UINT64 nFP)
	{
		m_tSPKeyFPMap[strKey] = nFP;
		return 0;
	}
	INT32	_del_fp(CString strKey)
	{
		m_tSPKeyFPMap.erase(strKey);
		return 0;
	}
	//--------------------------------------------------------------------
	DBF_TABLE		tDT;

	UINT32			m_nItemHLen;
	TListID			m_tAutoColList;			

	TMapCStrID		m_tColNameIdxMap;
	TMapID64		m_tPKeyFPMap;
	TMapCStrID64	m_tSPKeyFPMap;	
	
}DBF_TABLE_SEARCH, *PDBF_TABLE_SEARCH;

typedef list<DBF_TABLE_SEARCH>						TListDBFTableSearch;
typedef	TListDBFTableSearch::iterator				TListDBFTableSearchItor;
typedef TListDBFTableSearch::reverse_iterator		TListDBFTableSearchRItor;
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

typedef struct _dbf_table_item
{
	_dbf_table_item()
	{
		nRtnType		= 0;
		nRtnUINT16		= 0;
		nRtnUINT32		= 0;
		nRtnUINT64		= 0;

		lpRtnObject		= NULL;
		nRtnLen			= 0;
	}

	~_dbf_table_item()
	{		
	}

	void _clear()
	{
		nRtnType		= 0;

		nRtnUINT16		= 0;
		nRtnUINT32		= 0;
		nRtnUINT64		= 0;

		lpRtnObject		= NULL;

		nRtnLen			= 0;
	}

	UINT16		nRtnType;

	UINT16		nRtnUINT16;
	UINT32		nRtnUINT32;
	UINT64		nRtnUINT64;
	CString		strRtnString;
	PVOID		lpRtnObject;
	UINT32		nRtnLen;
}DBF_TABLE_ITEM, *PDBF_TABLE_ITEM;

typedef struct _dbf_table_item_header
{
	_dbf_table_item_header()
    {
		nPreFP			= 0;
    	nCurFP			= 0;
		nNextFP			= 0;

		nTID			= 0;
        nDelFlag		= 0;
		nUpCnt			= 0;
		nEncType		= 0;
		nCompType		= 0;
        nInitBlockSize	= 0;
		nCurBlockSize	= 0;
		memset(nResBuff, 0, 32);
    }
	~_dbf_table_item_header()
	{
	}
	UINT64		nPreFP;
	UINT64		nCurFP;
	UINT64		nNextFP;

	UINT32		nTID;
    UINT32		nDelFlag;
	UINT32		nUpCnt;
	UINT32		nEncType;
	UINT32		nCompType;
    UINT32		nInitBlockSize;
	UINT32		nCurBlockSize;
	UINT8		nResBuff[32];
}DBF_TABLE_ITEM_HEADER, *PDBF_TABLE_ITEM_HEADER;
//--------------------------------------------------------------------------

typedef struct _dbf_table_item_set_value
{
	_dbf_table_item_set_value()
	{
		lpObject	= NULL;
		pUINT16		= NULL;
		pUINT32		= NULL;
		pUINT64		= NULL;
		pString		= NULL;
		nBuffLen	= 0;
	}

	PUINT16		pUINT16;
	PUINT32		pUINT32;
	PUINT64		pUINT64;
	LPCTSTR		pString;
	PVOID		lpObject;
	UINT32		nBuffLen;
}DBF_TABLE_ITEM_SET_VALUE, *PDBF_TABLE_ITEM_SET_VALUE;


typedef map<CString, DBF_TABLE_ITEM_SET_VALUE>	TMapTItemSetValue;
typedef TMapTItemSetValue::iterator				TMapTItemSetValueItor;

typedef map<CString, TMapTItemSetValue>			TMapTItemSetValueMap;
typedef TMapTItemSetValueMap::iterator			TMapTItemSetValueMapItor;
//--------------------------------------------------------------------------

typedef struct _dbf_fast_buff_info
{
	_dbf_fast_buff_info()
	{
		pBuffer		= NULL;
		nBuffLen	= 0;
		nBuffPos	= 0;
		nMkBufSize	= 0;
	}

	void _clear()
	{
		if(pBuffer)		free(pBuffer);

		pBuffer		= NULL;
		nBuffLen	= 0;
		nBuffPos	= 0;
		nMkBufSize	= 0;
	}

	PBYTE		pBuffer;
	UINT32		nBuffLen;
	UINT32		nBuffPos;
	UINT32		nMkBufSize;
}DBF_FAST_BUFF_INFO, *PDBF_FAST_BUFF_INFO;
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------



#pragma pack()

#endif //DBF_STRUCT_DBF_HEADER_V2_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



