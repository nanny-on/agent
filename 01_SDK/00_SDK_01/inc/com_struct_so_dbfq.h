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

#ifndef     COM_STRUCT_SO_DBFQ_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define     COM_STRUCT_SO_DBFQ_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//------------------------------------------------------------------------------

typedef struct _dbfq_cdt_info_where
{
	_dbfq_cdt_info_where()
	{
		nChkType = 0;
		nCompType	= 0;
		nChkValueS	= 0;
		nChkValueE	= 0;
	}
	CString		strName;
	UINT32		nChkType;
	UINT32		nCompType;
	UINT64		nChkValueS;
	UINT64		nChkValueE;
	CString		strChkValueS;
	CString		strChkValueE;
}DBFQ_CDT_INFO_WHERE, *PDBFQ_CDT_INFO_WHERE;

typedef list<DBFQ_CDT_INFO_WHERE>		TListDBFQCDTInfoWhere;
typedef TListDBFQCDTInfoWhere::iterator	TListDBFQCDTInfoWhereItor;

typedef struct _dbfq_cdt_info
{
	_dbfq_cdt_info()
	{
		nType		= 0;
		nOption		= 0;
		nDefault	= 0;

		lpDefault	= NULL;
		nDefLen		= 0;
	}
	
	CString		strName;
	CString		strNewName;
	UINT16		nType;
	UINT32		nOption;
	UINT64		nDefault;
	CString		strDefault;
	PVOID		lpDefault;
	UINT32		nDefLen;
}DBFQ_CDT_INFO, *PDBFQ_CDT_INFO;

typedef	list<DBFQ_CDT_INFO>			TListDBFQCDTInfo;
typedef	TListDBFQCDTInfo::iterator	TListDBFQCDTInfoItor;
//------------------------------------------------------------------------------

#endif      //COM_STRUCT_SO_DBFQ_H_938EF200_F60B_452A_BD30_A10E8507EDCC
