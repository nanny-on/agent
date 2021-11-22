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
#ifndef DB_PO_PM_SCAN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_PM_SCAN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_pm_scan
{
	_db_po_pm_scan()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PM_SCAN, *PDB_PO_PM_SCAN;

typedef list<DB_PO_PM_SCAN>					TListDBPoPmScan;
typedef TListDBPoPmScan::iterator			TListDBPoPmScanItor;

typedef map<UINT32, DB_PO_PM_SCAN>			TMapDBPoPmScan;
typedef TMapDBPoPmScan::iterator			TMapDBPoPmScanItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pm_scan_pkg
{
	_db_po_pm_scan_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PM_SCAN_PKG, *PDB_PO_PM_SCAN_PKG;

typedef list<DB_PO_PM_SCAN_PKG>				TListDBPoPmScanPkg;
typedef TListDBPoPmScanPkg::iterator		TListDBPoPmScanPkgItor;

typedef map<UINT32, DB_PO_PM_SCAN_PKG>		TMapDBPoPmScanPkg;
typedef TMapDBPoPmScanPkg::iterator			TMapDBPoPmScanPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pm_scan_unit
{
	_db_po_pm_scan_unit()
	{
		nScanType		= 0;
		nScanTime		= 0;	
		nScanSvrType	= 0;

		nScanLastTime	= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nScanType;
	UINT64				nScanTime;
	UINT32				nScanSvrType;

	UINT32				nScanLastTime;

}DB_PO_PM_SCAN_UNIT, *PDB_PO_PM_SCAN_UNIT;

typedef list<DB_PO_PM_SCAN_UNIT>			TListDBPoPmScanUnit;
typedef TListDBPoPmScanUnit::iterator		TListDBPoPmScanUnitItor;

typedef map<UINT32, DB_PO_PM_SCAN_UNIT>		TMapDBPoPmScanUnit;
typedef TMapDBPoPmScanUnit::iterator		TMapDBPoPmScanUnitItor;
//---------------------------------------------------------------------------

#endif //DB_PO_PM_SCAN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



