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
#pragma once

typedef struct _db_po_in_vuln_sw
{
	_db_po_in_vuln_sw()
	{
	}

	DB_PO_HEADER		tDPH;
}DB_PO_IN_VULN_SW, *PDB_PO_IN_VULN_SW;

typedef list<DB_PO_IN_VULN_SW>				TListDBPoInVulnSw;
typedef TListDBPoInVulnSw::iterator			TListDBPoInVulnSwItor;

typedef map<UINT32, DB_PO_IN_VULN_SW>		TMapDBPoInVulnSw;
typedef TMapDBPoInVulnSw::iterator			TMapDBPoInVulnSwItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_vuln_sw_pkg
{
	_db_po_in_vuln_sw_pkg()
	{
	}

	DB_PO_HEADER		tDPH;
	String				strSwChkKey;
	String				strSwVersion;

}DB_PO_IN_VULN_SW_PKG, *PDB_PO_IN_VULN_SW_PKG;

typedef list<DB_PO_IN_VULN_SW_PKG>				TListDBPoInVulnSwPkg;
typedef TListDBPoInVulnSwPkg::iterator			TListDBPoInVulnSwPkgItor;

typedef map<UINT32, DB_PO_IN_VULN_SW_PKG>		TMapDBPoInVulnSwPkg;
typedef TMapDBPoInVulnSwPkg::iterator			TMapDBPoInVulnSwPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_vuln_sw_unit
{
	_db_po_in_vuln_sw_unit()
	{
		nSwID = 0;
		strClassName.empty();
		strSwRegKey.empty();
	}
	DB_PO_HEADER			tDPH;
	
	UINT32					nSwID;
	
	String					strClassName;
	String					strSwRegKey;

}DB_PO_IN_VULN_SW_UNIT, *PDB_PO_IN_VULN_SW_UNIT;

typedef list<DB_PO_IN_VULN_SW_UNIT>				TListDBPoInVulnSwUnit;
typedef TListDBPoInVulnSwUnit::iterator			TListDBPoInVulnSwUnitItor;

typedef map<UINT32, DB_PO_IN_VULN_SW_UNIT>		TMapDBPoInVulnSwUnit;
typedef TMapDBPoInVulnSwUnit::iterator			TMapDBPoInVulnSwUnitItor;
//---------------------------------------------------------------------------