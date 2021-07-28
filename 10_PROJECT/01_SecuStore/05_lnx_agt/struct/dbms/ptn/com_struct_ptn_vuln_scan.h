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

#ifndef DB_PTN_VULN_SCAN_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE
#define DB_PTN_VULN_SCAN_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE

typedef struct _db_ptn_vuln_scan
{
	_db_ptn_vuln_scan()
	{
		nID					= 0;
		nUsedMode			= 1;		
		nRegDate			= 0;

		nVulnID				= 0;
		nVulnSubID			= 0;
		nScanOrder			= 0;
		
		nSysType			= 0;
		nSysSPType			= 0;
		nSysArchType		= 0;

		nScanType			= 0;
		nCustomID			= 0;
		nValueType			= 0;
		
		nDecisionType		= 0;
		nCompType			= 0;
	}

	UINT32		nID;
	UINT32		nUsedMode;
	UINT32		nRegDate;

	UINT32		nVulnID;
	UINT32		nVulnSubID;
	UINT32		nScanOrder;

	String		strName;

	UINT64		nSysType;
	UINT32		nSysSPType;
	UINT32		nSysArchType;

	UINT32		nScanType;
	UINT32		nCustomID;
	String		strScanPath;
	UINT32		nValueType;
	UINT32		nDecisionType;
	UINT32		nCompType;
	String		strCompValue;

}DB_PTN_VULN_SCAN, *PDB_PTN_VULN_SCAN;

typedef list<DB_PTN_VULN_SCAN>				TListDBPtnVulnScan;
typedef TListDBPtnVulnScan::iterator		TListDBPtnVulnScanItor;

typedef map<UINT32, DB_PTN_VULN_SCAN>		TMapDBPtnVulnScan;
typedef TMapDBPtnVulnScan::iterator			TMapDBPtnVulnScanItor;

#endif /*DB_PTN_VULN_SCAN_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE*/



