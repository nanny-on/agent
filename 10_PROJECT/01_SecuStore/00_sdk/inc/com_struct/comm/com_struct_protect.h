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

typedef struct _db_base_protect
{
	_db_base_protect()
	{
		nID			= 0;
		nUsedFlag		= STATUS_USED_MODE_ON;
		nRegDate		= 0;
		nUsedMode		= 0;
		nAdminID		= 0;
		nExtOption		= 0;

		nLockType		= SS_SITE_VULN_TYPE_NONE;
	} 

	UINT32			nID ;
	UINT32			nUsedFlag;
	UINT32			nRegDate;
	UINT32			nUsedMode;
	UINT32			nAdminID;
	UINT32			nExtOption;

	String			strName;
	String			strDescr;

	UINT32			nLockType;
}DB_BASE_PROTECT, *PDB_BASE_PROTECT;

typedef list<DB_BASE_PROTECT>			TListDBBaseProtect;
typedef TListDBBaseProtect::iterator		TListDBBaseProtectItor;

typedef map<UINT32, DB_BASE_PROTECT>	TMapDBBaseProtect;
typedef TMapDBBaseProtect::iterator		TMapDBBaseProtectItor;

//---------------------------------------------------------------------------------------

typedef struct _db_acls_protect
{
	_db_acls_protect()
	{
		nLockAnyPerm		= SS_SITE_VULN_PERM_PKG_READ;
		nExProcPerm		= SS_SITE_VULN_PERM_PKG_ALL;
	} 

	String			strLockPath;
	UINT32			nLockAnyPerm;		// SS_SITE_VULN_PERM_XXXX
	String			strExProcPath;
	UINT32			nExProcPerm;		// SS_SITE_VULN_PERM_XXXX

}DB_ACLS_PROTECT, *PDB_ACLS_PROTECT;

typedef list<DB_ACLS_PROTECT>		TListDBAclsProtect;
typedef TListDBAclsProtect::iterator		TListDBAclsProtectItor;

typedef map<UINT32, DB_ACLS_PROTECT>	TMapDBAclsProtect;
typedef TMapDBAclsProtect::iterator		TMapDBAclsProtectItor;

//---------------------------------------------------------------------------------------

typedef struct _db_vuln_protect : public _db_base_protect
{
	_db_vuln_protect()
	{
		nSVulnID			= 0;
		nSVulnSubID		= 0;
		nCustomID		= SS_SITE_VULN_CUSTOM_ID_NONE;
	} 

	UINT32			nSVulnID;
	UINT32			nSVulnSubID;
	UINT32			nCustomID;		// SS_SITE_VULN_CUSTOM_ID_XXXX

}DB_VULN_PROTECT, *PDB_VULN_PROTECT;

typedef list<DB_VULN_PROTECT>			TListDBVulnProtect;
typedef TListDBVulnProtect::iterator	TListDBVulnProtectItor;

typedef map<UINT32, DB_VULN_PROTECT>	TMapDBVulnProtect;
typedef TMapDBVulnProtect::iterator		TMapDBVulnProtectItor;

//---------------------------------------------------------------------------------------

typedef struct _db_vuln_object_protect : public _db_vuln_protect, public _db_acls_protect
{
	_db_vuln_object_protect()
	{
	} 

}DB_VULN_OBJECT_PROTECT, *PDB_VULN_OBJECT_PROTECT;

typedef list<DB_VULN_OBJECT_PROTECT>		TListDBVulnObjProtect;
typedef TListDBVulnObjProtect::iterator		TListDBVulnObjProtectItor;

typedef map<UINT32, DB_VULN_OBJECT_PROTECT>	TMapDBVulnObjProtect;
typedef TMapDBVulnObjProtect::iterator		TMapDBVulnObjProtectItor;

//---------------------------------------------------------------------------------------

typedef struct _db_vuln_file_protect : public _db_vuln_object_protect
{
	_db_vuln_file_protect()
	{
	} 

}DB_VULN_FILE_PROTECT, *PDB_VULN_FILE_PROTECT;

typedef list<DB_VULN_FILE_PROTECT>			TListDBVulnFileProtect;
typedef TListDBVulnFileProtect::iterator	TListDBVulnFileProtectItor;

typedef map<UINT32, DB_VULN_FILE_PROTECT>	TMapDBVulnFileProtect;
typedef TMapDBVulnFileProtect::iterator		TMapDBVulnFileProtectItor;

//---------------------------------------------------------------------------------------

typedef struct _db_vuln_reg_protect : _db_vuln_object_protect
{
	_db_vuln_reg_protect()
	{
	} 

}DB_VULN_REG_PROTECT, *PDB_VULN_REG_PROTECT;

typedef list<DB_VULN_REG_PROTECT>			TListDBVulnRegProtect;
typedef TListDBVulnRegProtect::iterator		TListDBVulnRegProtectItor;

typedef map<UINT32, DB_VULN_REG_PROTECT>		TMapDBVulnRegProtect;
typedef TMapDBVulnRegProtect::iterator		TMapDBVulnRegProtectItor;

//---------------------------------------------------------------------------------------

typedef struct _db_vuln_proc_protect : public _db_vuln_object_protect
{
	_db_vuln_proc_protect()
	{
	} 

}DB_VULN_PROC_PROTECT, *PDB_VULN_PROC_PROTECT;

typedef list<DB_VULN_PROC_PROTECT>		TListDBVulnProcProtect;
typedef TListDBVulnProcProtect::iterator		TListDBVulnProcProtectItor;

typedef map<UINT32, DB_VULN_PROC_PROTECT>	TMapDBVulnProcProtect;
typedef TMapDBVulnProcProtect::iterator		TMapDBVulnProcProtectItor;

//---------------------------------------------------------------------------------------

typedef struct _db_vuln_wmi_protect : public _db_vuln_protect
{
	_db_vuln_wmi_protect()
	{
	} 

}DB_VULN_WMI_PROTECT, *PDB_VULN_WMI_PROTECT;

typedef list<DB_VULN_WMI_PROTECT>		TListDBVulnWmiProtect;
typedef TListDBVulnWmiProtect::iterator		TListDBVulnWmiProtectItor;

typedef map<UINT32, DB_VULN_WMI_PROTECT>	TMapDBVulnWmiProtect;
typedef TMapDBVulnWmiProtect::iterator		TMapDBVulnWmiProtectItor;

//---------------------------------------------------------------------------------------