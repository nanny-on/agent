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

#ifndef DB_PO_PM_EX_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_PM_EX_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_pm_ex
{
	_db_po_pm_ex()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PM_EX, *PDB_PO_PM_EX;

typedef list<DB_PO_PM_EX>					TListDBPoPmEx;
typedef TListDBPoPmEx::iterator				TListDBPoPmExItor;

typedef map<UINT32, DB_PO_PM_EX>			TMapDBPoPmEx;
typedef TMapDBPoPmEx::iterator				TMapDBPoPmExItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pm_ex_pkg
{
	_db_po_pm_ex_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PM_EX_PKG, *PDB_PO_PM_EX_PKG;

typedef list<DB_PO_PM_EX_PKG>				TListDBPoPmExPkg;
typedef TListDBPoPmExPkg::iterator			TListDBPoPmExPkgItor;

typedef map<UINT32, DB_PO_PM_EX_PKG>		TMapDBPoPmExPkg;
typedef TMapDBPoPmExPkg::iterator			TMapDBPoPmExPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pm_ex_unit
{
	_db_po_pm_ex_unit()
	{
		nExMode			= 0;		
	}

	DB_PO_HEADER		tDPH;

	UINT32				nExMode;

	String				strPatchID;
	TMapID				tPatchIDMap;

}DB_PO_PM_EX_UNIT, *PDB_PO_PM_EX_UNIT;

typedef list<DB_PO_PM_EX_UNIT>			TListDBPoPmExUnit;
typedef TListDBPoPmExUnit::iterator		TListDBPoPmExUnitItor;

typedef map<UINT32, DB_PO_PM_EX_UNIT>	TMapDBPoPmExUnit;
typedef TMapDBPoPmExUnit::iterator		TMapDBPoPmExUnitItor;
//---------------------------------------------------------------------------


#endif //DB_PO_PM_EX_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



