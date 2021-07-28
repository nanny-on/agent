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
#ifndef DB_HOST_VULN_RST_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_HOST_VULN_RST_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_host_vuln_rst_unit_simple
{
	_db_host_vuln_rst_unit_simple()
	{
		nVulnID		= 0;
		nDScore		= 0;
		nRst		= 0;
	}
	UINT32	nVulnID;
	UINT32	nDScore;
	UINT32	nRst;
}DB_HOST_VULN_RST_UNIT_SIMPLE, *PDB_HOST_VULN_RST_UNIT_SIMPLE;

typedef list<DB_HOST_VULN_RST_UNIT_SIMPLE>				TListDBHostVulnRstUnitSimple;
typedef TListDBHostVulnRstUnitSimple::iterator			TListDBHostVulnRstUnitSimpleItor;

typedef map<UINT32, DB_HOST_VULN_RST_UNIT_SIMPLE>		TMapDBHostVulnRstUnitSimple;
typedef TMapDBHostVulnRstUnitSimple::iterator			TMapDBHostVulnRstUnitSimpleItor;
//-------------------------------------------------------------
//-------------------------------------------------------------

typedef struct _db_host_vuln_rst
{
	_db_host_vuln_rst()
	{
		nID						= 0;
		nUsedFlag				= USED_FLAG_TYPE_USED;
		nRegDate				= 0;
		nSyncSvrStep			= 0;

		nScanMon				= 0;
		nScanDate				= 0;

		nGroupID				= 0;
		nHostID					= 0;

		nVersionID				= 0;

		nScanScore				= 0;
	} 

	UINT32						nID ;
	UINT32						nUsedFlag;
	UINT32						nRegDate;
	UINT32						nSyncSvrStep;

	UINT32						nScanMon;
	UINT32						nScanDate;
	
	UINT32						nGroupID;
	UINT32						nHostID;
	
	String						strIPAddr;
	String						strUserName;
	String						strSysName;
	String						strVersion;
	UINT32						nVersionID;

	UINT32						nScanScore;
	String						strScanRst;

	TMapDBHostVulnRstUnitSimple	tRstUnitMap;
	

}DB_HOST_VULN_RST, *PDB_HOST_VULN_RST;

typedef list<DB_HOST_VULN_RST>				TListDBHostVulnRst;
typedef TListDBHostVulnRst::iterator		TListDBHostVulnRstItor;

typedef map<UINT32, DB_HOST_VULN_RST>		TMapDBHostVulnRst;
typedef TMapDBHostVulnRst::iterator			TMapDBHostVulnRstItor;

//-------------------------------------------------------------

typedef struct _db_host_vuln_rst_unit
{
	_db_host_vuln_rst_unit()
	{
		nID						= 0;
		nUsedFlag				= USED_FLAG_TYPE_USED;
		nRegDate				= 0;
		
		nVulnRstID				= 0;
	} 

	UINT32							nID ;
	UINT32							nUsedFlag;
	UINT32							nRegDate;
	
	UINT32							nVulnRstID;
	
	DB_HOST_VULN_RST_UNIT_SIMPLE	tDHVRUS;
		
	String							strRstInfo;

}DB_HOST_VULN_RST_UNIT, *PDB_HOST_VULN_RST_UNIT;

typedef list<DB_HOST_VULN_RST_UNIT>				TListDBHostVulnRstUnit;
typedef TListDBHostVulnRstUnit::iterator		TListDBHostVulnRstUnitItor;

typedef map<UINT32, DB_HOST_VULN_RST_UNIT>		TMapDBHostVulnRstUnit;
typedef TMapDBHostVulnRstUnit::iterator			TMapDBHostVulnRstUnitItor;

//-------------------------------------------------------------


#endif //DB_HOST_VULN_RST_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



