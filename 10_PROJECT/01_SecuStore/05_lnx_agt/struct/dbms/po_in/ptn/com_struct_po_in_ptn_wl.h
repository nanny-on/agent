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
 * 
 */

#ifndef DB_PO_IN_PTN_WL_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_IN_PTN_WL_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_in_ptn_wl
{
	_db_po_in_ptn_wl()
	{
	}

	DB_PO_HEADER		tDPH;
	DB_PTN_FILE_INFO	tDPFI;
}DB_PO_IN_PTN_WL, *PDB_PO_IN_PTN_WL;

typedef list<DB_PO_IN_PTN_WL>				TListDBPoInPtnWL;
typedef TListDBPoInPtnWL::iterator			TListDBPoInPtnWLItor;

typedef map<UINT32, DB_PO_IN_PTN_WL>		TMapDBPoInPtnWL;
typedef TMapDBPoInPtnWL::iterator			TMapDBPoInPtnWLItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_ptn_wl_pkg
{
	_db_po_in_ptn_wl_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_IN_PTN_WL_PKG, *PDB_PO_IN_PTN_WL_PKG;

typedef list<DB_PO_IN_PTN_WL_PKG>			TListDBPoInPtnWLPkg;
typedef TListDBPoInPtnWLPkg::iterator		TListDBPoInPtnWLPkgItor;

typedef map<UINT32, DB_PO_IN_PTN_WL_PKG>	TMapDBPoInPtnWLPkg;
typedef TMapDBPoInPtnWLPkg::iterator		TMapDBPoInPtnWLPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_ptn_wl_unit
{
	_db_po_in_ptn_wl_unit()
	{
		nRTMode		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nRTMode;
	UINT32				nFltChkType;
	DB_FILE_INFO		tDFI;

}DB_PO_IN_PTN_WL_UNIT, *PDB_PO_IN_PTN_WL_UNIT;

typedef list<DB_PO_IN_PTN_WL_UNIT>			TListDBPoInPtnWLUnit;
typedef TListDBPoInPtnWLUnit::iterator		TListDBPoInPtnWLUnitItor;

typedef map<UINT32, DB_PO_IN_PTN_WL_UNIT>	TMapDBPoInPtnWLUnit;
typedef TMapDBPoInPtnWLUnit::iterator		TMapDBPoInPtnWLUnitItor;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


#endif //DB_PO_IN_PTN_WL_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



