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

#ifndef DB_PTN_PATCH_SCAN_ENV_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE
#define DB_PTN_PATCH_SCAN_ENV_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE

typedef struct _db_ptn_patch_scan_env
{
	_db_ptn_patch_scan_env()
	{
		nID					= 0;
		nUsedMode			= 1;

		nRegDate			= 0;
	}

	UINT32		nID;
	UINT32		nUsedMode;

	String		strName;
	UINT32		nRegDate;

}DB_PTN_PATCH_SCAN_ENV, *PDB_PTN_PATCH_SCAN_ENV;

typedef list<DB_PTN_PATCH_SCAN_ENV>				TListDBPtnPatchScanEnv;
typedef TListDBPtnPatchScanEnv::iterator		TListDBPtnPatchScanEnvItor;

typedef map<UINT32, DB_PTN_PATCH_SCAN_ENV>		TMapDBPtnPatchScanEnv;
typedef TMapDBPtnPatchScanEnv::iterator			TMapDBPtnPatchScanEnvItor;
//--------------------------------------------------------------------------------------

typedef struct _db_ptn_patch_scan_env_unit
{
	_db_ptn_patch_scan_env_unit()
	{
		nID					= 0;
		nUsedMode			= 1;

		nPkgID				= 0;
		nScanType			= 0;

		nTarCompType		= 0;

		nSysType			= 0;
		nSysSPType			= 0;
		nSysArchType		= 0;
	}

	UINT32		nID;
	UINT32		nUsedMode;

	UINT32		nPkgID;
	UINT32		nScanType;	

	String		strTarPath;
	String		strTarName;
	String		strTarVerBegin;
	String		strTarVerEnd;
	UINT32		nTarCompType;

	UINT64		nSysType;
	UINT32		nSysSPType;
	UINT32		nSysArchType;

}DB_PTN_PATCH_SCAN_ENV_UNIT, *PDB_PTN_PATCH_SCAN_ENV_UNIT;

typedef list<DB_PTN_PATCH_SCAN_ENV_UNIT>				TListDBPtnPatchScanEnvUnit;
typedef TListDBPtnPatchScanEnvUnit::iterator			TListDBPtnPatchScanEnvUnitItor;

typedef map<UINT32, DB_PTN_PATCH_SCAN_ENV_UNIT>		TMapDBPtnPatchScanEnvUnit;
typedef TMapDBPtnPatchScanEnvUnit::iterator			TMapDBPtnPatchScanEnvUnitItor;


#endif /*DB_PTN_PATCH_SCAN_ENV_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE*/



