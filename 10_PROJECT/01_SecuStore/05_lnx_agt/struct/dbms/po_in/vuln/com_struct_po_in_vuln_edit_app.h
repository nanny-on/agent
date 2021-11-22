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
#pragma once

typedef struct _db_po_in_vuln_edit_app
{
	_db_po_in_vuln_edit_app()
	{
	}

	DB_PO_HEADER		tDPH;
}DB_PO_IN_VULN_EDIT_APP, *PDB_PO_IN_VULN_EDIT_APP;

typedef list<DB_PO_IN_VULN_EDIT_APP>				TListDBPoInVulnEditApp;
typedef TListDBPoInVulnEditApp::iterator		TListDBPoInVulnEditAppItor;

typedef map<UINT32, DB_PO_IN_VULN_EDIT_APP>		TMapDBPoInVulnEditApp;
typedef TMapDBPoInVulnEditApp::iterator			TMapDBPoInVulnEditAppItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_vuln_edit_app_pkg
{
	_db_po_in_vuln_edit_app_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_IN_VULN_EDIT_APP_PKG, *PDB_PO_IN_VULN_EDIT_APP_PKG;

typedef list<DB_PO_IN_VULN_EDIT_APP_PKG>				TListDBPoInVulnEditAppPkg;
typedef TListDBPoInVulnEditAppPkg::iterator			TListDBPoInVulnEditAppPkgItor;

typedef map<UINT32, DB_PO_IN_VULN_EDIT_APP_PKG>		TMapDBPoInVulnEditAppPkg;
typedef TMapDBPoInVulnEditAppPkg::iterator			TMapDBPoInVulnEditAppPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_vuln_edit_app_unit
{
	_db_po_in_vuln_edit_app_unit()
	{
		strClassName.empty();
		strName.empty();
		strRegKey.empty();
	}
	
	String		strClassName;
	String		strName;
	String		strRegKey;
	
	DB_PO_HEADER			tDPH;

}DB_PO_IN_VULN_EDIT_APP_UNIT, *PDB_PO_IN_VULN_EDIT_APP_UNIT;

typedef list<DB_PO_IN_VULN_EDIT_APP_UNIT>			TListDBPoInVulnEditAppUnit;
typedef TListDBPoInVulnEditAppUnit::iterator		TListDBPoInVulnEditAppUnitItor;

typedef map<UINT32, DB_PO_IN_VULN_EDIT_APP_UNIT>		TMapDBPoInVulnEditAppUnit;
typedef TMapDBPoInVulnEditAppUnit::iterator			TMapDBPoInVulnEditAppUnitItor;
//---------------------------------------------------------------------------