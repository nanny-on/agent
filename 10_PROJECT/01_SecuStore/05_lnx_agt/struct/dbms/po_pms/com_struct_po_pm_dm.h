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

#ifndef DB_PO_PM_DM_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_PM_DM_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_pm_dm
{
	_db_po_pm_dm()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PM_DM, *PDB_PO_PM_DM;

typedef list<DB_PO_PM_DM>					TListDBPoPmDm;
typedef TListDBPoPmDm::iterator				TListDBPoPmDmItor;

typedef map<UINT32, DB_PO_PM_DM>			TMapDBPoPmDm;
typedef TMapDBPoPmDm::iterator				TMapDBPoPmDmItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pm_dm_pkg
{
	_db_po_pm_dm_pkg()
	{	
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PM_DM_PKG, *PDB_PO_PM_DM_PKG;

typedef list<DB_PO_PM_DM_PKG>				TListDBPoPmDmPkg;
typedef TListDBPoPmDmPkg::iterator			TListDBPoPmDmPkgItor;

typedef map<UINT32, DB_PO_PM_DM_PKG>		TMapDBPoPmDmPkg;
typedef TMapDBPoPmDmPkg::iterator			TMapDBPoPmDmPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pm_dm_unit
{
	_db_po_pm_dm_unit()
	{
		nApplyType		= 0;		
		nNotifyID		= 0;	
	}

	DB_PO_HEADER		tDPH;

	UINT32				nApplyType;
	UINT32				nNotifyID;

	String				strPatchID;
	TMapID				tPatchIDMap;

}DB_PO_PM_DM_UNIT, *PDB_PO_PM_DM_UNIT;

typedef list<DB_PO_PM_DM_UNIT>			TListDBPoPmDmUnit;
typedef TListDBPoPmDmUnit::iterator		TListDBPoPmDmUnitItor;

typedef map<UINT32, DB_PO_PM_DM_UNIT>	TMapDBPoPmDmUnit;
typedef TMapDBPoPmDmUnit::iterator		TMapDBPoPmDmUnitItor;
//---------------------------------------------------------------------------

#endif //DB_PO_PM_DM_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



