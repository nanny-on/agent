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
#ifndef DB_PO_IN_VULN_SCAN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_IN_VULN_SCAN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_in_vuln_scan
{
	_db_po_in_vuln_scan()
	{
		nRecStdTime		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT64				nRecStdTime;

}DB_PO_IN_VULN_SCAN, *PDB_PO_IN_VULN_SCAN;

typedef list<DB_PO_IN_VULN_SCAN>					TListDBPoInVulnScan;
typedef TListDBPoInVulnScan::iterator				TListDBPoInVulnScanItor;

typedef map<UINT32, DB_PO_IN_VULN_SCAN>				TMapDBPoInVulnScan;
typedef TMapDBPoInVulnScan::iterator				TMapDBPoInVulnScanItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_vuln_scan_pkg
{
	_db_po_in_vuln_scan_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_IN_VULN_SCAN_PKG, *PDB_PO_IN_VULN_SCAN_PKG;

typedef list<DB_PO_IN_VULN_SCAN_PKG>				TListDBPoInVulnScanPkg;
typedef TListDBPoInVulnScanPkg::iterator			TListDBPoInVulnScanPkgItor;

typedef map<UINT32, DB_PO_IN_VULN_SCAN_PKG>			TMapDBPoInVulnScanPkg;
typedef TMapDBPoInVulnScanPkg::iterator				TMapDBPoInVulnScanPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_vuln_scan_unit
{
	_db_po_in_vuln_scan_unit()
	{
		nSchTime			= 0;
		nScanType			= 0;
		nLastScanTime		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT64				nSchTime;
	UINT32				nScanType;
	UINT32				nLastScanTime;

}DB_PO_IN_VULN_SCAN_UNIT, *PDB_PO_IN_VULN_SCAN_UNIT;

typedef list<DB_PO_IN_VULN_SCAN_UNIT>				TListDBPoInVulnScanUnit;
typedef TListDBPoInVulnScanUnit::iterator			TListDBPoInVulnScanUnitItor;

typedef map<UINT32, DB_PO_IN_VULN_SCAN_UNIT>		TMapDBPoInVulnScanUnit;
typedef TMapDBPoInVulnScanUnit::iterator			TMapDBPoInVulnScanUnitItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_vuln_scan_unit_pkg
{
	_db_po_in_vuln_scan_unit_pkg()
	{
		nSiteScore		= 0;
	}

	DB_PO_HEADER		tDPH;
	UINT32				nSiteScore;

}DB_PO_IN_VULN_SCAN_UNIT_PKG, *PDB_PO_IN_VULN_SCAN_UNIT_PKG;

typedef list<DB_PO_IN_VULN_SCAN_UNIT_PKG>			TListDBPoInVulnScanUnitPkg;
typedef TListDBPoInVulnScanUnitPkg::iterator		TListDBPoInVulnScanUnitPkgItor;

typedef map<UINT32, DB_PO_IN_VULN_SCAN_UNIT_PKG>	TMapDBPoInVulnScanUnitPkg;
typedef TMapDBPoInVulnScanUnitPkg::iterator			TMapDBPoInVulnScanUnitPkgItor;
//---------------------------------------------------------------------------

#endif //DB_PO_IN_VULN_SCAN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



