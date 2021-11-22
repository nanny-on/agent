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

#ifndef DB_PO_PR_ROLE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_PR_ROLE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_pr_role
{
	_db_po_pr_role()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PR_ROLE, *PDB_PO_PR_ROLE;

typedef list<DB_PO_PR_ROLE>					TListDBPoPrRole;
typedef TListDBPoPrRole::iterator			TListDBPoPrRoleItor;

typedef map<UINT32, DB_PO_PR_ROLE>			TMapDBPoPrRole;
typedef TMapDBPoPrRole::iterator			TMapDBPoPrRoleItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pr_role_pkg
{
	_db_po_pr_role_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PR_ROLE_PKG, *PDB_PO_PR_ROLE_PKG;

typedef list<DB_PO_PR_ROLE_PKG>				TListDBPoPrRolePkg;
typedef TListDBPoPrRolePkg::iterator		TListDBPoPrRolePkgItor;

typedef map<UINT32, DB_PO_PR_ROLE_PKG>		TMapDBPoPrRolePkg;
typedef TMapDBPoPrRolePkg::iterator			TMapDBPoPrRolePkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pr_role_unit
{
	_db_po_pr_role_unit()
	{

	}

	DB_PO_HEADER		tDPH;

}DB_PO_PR_ROLE_UNIT, *PDB_PO_PR_ROLE_UNIT;

typedef list<DB_PO_PR_ROLE_UNIT>			TListDBPoPrRoleUnit;
typedef TListDBPoPrRoleUnit::iterator		TListDBPoPrRoleUnitItor;

typedef map<UINT32, DB_PO_PR_ROLE_UNIT>		TMapDBPoPrRoleUnit;
typedef TMapDBPoPrRoleUnit::iterator		TMapDBPoPrRoleUnitItor;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

typedef struct _db_po_pr_role_unit_proc_pkg
{
	_db_po_pr_role_unit_proc_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PR_ROLE_UNIT_PROC_PKG, *PDB_PO_PR_ROLE_UNIT_PROC_PKG;

typedef list<DB_PO_PR_ROLE_UNIT_PROC_PKG>			TListDBPoPrRoleUnitProcPkg;
typedef TListDBPoPrRoleUnitProcPkg::iterator		TListDBPoPrRoleUnitProcPkgItor;

typedef map<UINT32, DB_PO_PR_ROLE_UNIT_PROC_PKG>	TMapDBPoPrRoleUnitProcPkg;
typedef TMapDBPoPrRoleUnitProcPkg::iterator			TMapDBPoPrRoleUnitProcPkgItor;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


typedef struct _db_po_pr_role_proc_unit
{
	_db_po_pr_role_proc_unit()
	{	

	}

	DB_PO_HEADER		tDPH;

	UINT32				nBlockMode;
	String				strFilePath;
	String				strFileComp;
	String				strFileDesc;
	String				strFileHash;	
	String				strFileSign;
	String				strFileSignComp;
	String				strFileWLHash;

}DB_PO_PR_ROLE_PROC_UNIT, *PDB_PO_PR_ROLE_PROC_UNIT;

typedef list<DB_PO_PR_ROLE_PROC_UNIT>			TListDBPoPrRoleProcUnit;
typedef TListDBPoPrRoleProcUnit::iterator		TListDBPoPrRoleProcUnitItor;

typedef map<UINT32, DB_PO_PR_ROLE_PROC_UNIT>	TMapDBPoPrRoleProcUnit;
typedef TMapDBPoPrRoleProcUnit::iterator		TMapDBPoPrRoleProcUnitItor;
//---------------------------------------------------------------------------

#endif //DB_PO_PR_ROLE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467