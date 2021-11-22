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

#ifndef DBF_STRUCT_DBF_HEADER_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DBF_STRUCT_DBF_HEADER_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

#pragma pack(1)

typedef struct _dbf_header_column
{
	_dbf_header_column()
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
}DBF_HEADER_COLUMN, *PDBF_HEADER_COLUMN;

typedef list<DBF_HEADER_COLUMN>			TListColumSetValue;
typedef TListColumSetValue::iterator	TListColumSetValueItor;
//--------------------------------------------------------------------------

typedef struct _dbf_header_fix
{
	_dbf_header_fix()
	{
		nVersion		= 0;
		nHInfoSize		= 0;
		nHColSize		= 0;
	}
	UINT32	nVersion;
	UINT32	nHInfoSize;
	UINT32	nHColSize;
}DBF_HEADER_FIX, *PDBF_HEADER_FIX;

typedef struct _dbf_header_info
{
	_dbf_header_info()
	{
		nSchemaVer		= 0;
		nLastID			= 0;
		nItemCnt		= 0;
		nModifyDate		= 0;
		nColNumber		= 0;
	}
	UINT32				nSchemaVer;
	UINT64				nLastID;
	UINT32				nItemCnt;
	UINT32				nModifyDate;
	UINT32				nColNumber;
}DBF_HEADER_INFO, *PDBF_HEADER_INFO;

typedef struct _dbf_header
{
	_dbf_header()
    {    	
        memset(tColInfo, 0, sizeof(DBF_HEADER_COLUMN) * ASI_DBF_MAX_COLUMN_NUMBER);
    }
	DBF_HEADER_FIX		tHFix;
	DBF_HEADER_INFO		tHInfo;
    DBF_HEADER_COLUMN	tColInfo[ASI_DBF_MAX_COLUMN_NUMBER];
}DBF_HEADER, *PDBF_HEADER;
//--------------------------------------------------------------------------

typedef struct _dbf_item_header
{
	_dbf_item_header()
    {
    	nCurFP			= 0;
        nDelFlag		= 0;
        nInitBlockSize	= 0;
    }
	~_dbf_item_header()
	{
	}
	UINT64		nCurFP;
    UINT32		nDelFlag;
    UINT32		nInitBlockSize;
}DBF_ITEM_HEADER, *PDBF_ITEM_HEADER;
//--------------------------------------------------------------------------

typedef struct _dbf_item_set_value
{
	UINT16		nUINT16;
	UINT32		nUINT32;
	UINT64		nUINT64;
	CString		strString;	
}DBF_ITEM_SET_VALUE, *PDBF_ITEM_SET_VALUE;

typedef map<CString, DBF_ITEM_SET_VALUE>	TMapItemSetValue;
typedef TMapItemSetValue::iterator			TMapItemSetValueItor;
//--------------------------------------------------------------------------


#pragma pack()

#endif //DBF_STRUCT_DBF_HEADER_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



