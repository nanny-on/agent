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

#ifndef DB_SITE_VULN_SCAN_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_SITE_VULN_SCAN_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_site_vuln_scan
{
	_db_site_vuln_scan()
	{
		nID				= 0;
		nUsedFlag		= STATUS_USED_MODE_ON;
		nRegDate		= 0;
		nUsedMode		= 0;
		nAdminID		= 0;
		nExtOption		= 0;

		nVulnID			= 0;
		nVulnSubID		= 0;
		nScanOrder		= 0;

		nSysType		= 0;
		nSysSPType		= 0;
		nSysArchType	= 0;

		nScanType		= SS_SITE_VULN_TYPE_NONE;		
		nCustomID		= SS_SITE_VULN_CUSTOM_ID_NONE;

		nValueType		= SS_SITE_VULN_VALUE_TYPE_UNKNOWN;
		nCompType		= SS_SITE_VULN_COMPARE_TYPE_UNKNOWN;
		nDecisionType	= SS_SITE_VULN_DECISION_CRITERIA_AND;
	} 

	UINT32			nID ;
	UINT32			nUsedFlag;
	UINT32			nRegDate;
	UINT32			nUsedMode;
	UINT32			nAdminID;
	UINT32			nExtOption;

	String			strName;

	UINT32			nVulnID;
	UINT32			nVulnSubID;
	UINT32			nScanOrder;

	UINT64			nSysType;
	UINT32			nSysSPType;
	UINT32			nSysArchType;

	UINT32			nScanType;		// SS_SITE_VULN_TYPE_XXXX
	UINT32			nCustomID;		// SS_SITE_VULN_CUSTOM_ID_XXXX

	String			strScanPath;		
	UINT32			nValueType;		// SS_SITE_VULN_VALUE_TYPE_XXXX
	UINT32			nDecisionType;	// SS_SITE_VULN_DECISION_CRITERIA_XXXX
	UINT32			nCompType;		// SS_SITE_VULN_COMPARE_TYPE_XXXX
	String			strCompValue;	
}DB_SITE_VULN_SCAN, *PDB_SITE_VULN_SCAN;



typedef list<DB_SITE_VULN_SCAN>				TListDBSiteVulnScan;
typedef TListDBSiteVulnScan::iterator		TListDBSiteVulnScanItor;

typedef map<UINT32, DB_SITE_VULN_SCAN>		TMapDBSiteVulnScan;
typedef TMapDBSiteVulnScan::iterator		TMapDBSiteVulnScanItor;

//---------------------------------------------------------------------------------------

#endif //DB_SITE_VULN_SCAN_H_4D39CDDB_E289_4d56_9F98_FE42776F4467




