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
 * 
 */

#ifndef DB_PO_IN_PTN_NO_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_IN_PTN_NO_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_in_ptn_no
{
	_db_po_in_ptn_no()
	{
		nMsgType		= 0;
		nReqSkipOpt		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nMsgType;
	UINT32				nReqSkipOpt;

}DB_PO_IN_PTN_NO, *PDB_PO_IN_PTN_NO;

typedef list<DB_PO_IN_PTN_NO>				TListDBPoInPtnNo;
typedef TListDBPoInPtnNo::iterator			TListDBPoInPtnNoItor;

typedef map<UINT32, DB_PO_IN_PTN_NO>		TMapDBPoInPtnNo;
typedef TMapDBPoInPtnNo::iterator			TMapDBPoInPtnNoItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_ptn_no_pkg
{
	_db_po_in_ptn_no_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_IN_PTN_NO_PKG, *PDB_PO_IN_PTN_NO_PKG;

typedef list<DB_PO_IN_PTN_NO_PKG>				TListDBPoInPtnNoPkg;
typedef TListDBPoInPtnNoPkg::iterator			TListDBPoInPtnNoPkgItor;

typedef map<UINT32, DB_PO_IN_PTN_NO_PKG>		TMapDBPoInPtnNoPkg;
typedef TMapDBPoInPtnNoPkg::iterator			TMapDBPoInPtnNoPkgItor;
//---------------------------------------------------------------------------

#endif //DB_PO_IN_PTN_NO_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



