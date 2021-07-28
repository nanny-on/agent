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

#ifndef DB_PTN_VULN_MP_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE
#define DB_PTN_VULN_MP_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE

typedef struct _db_ptn_vuln_mp
{
	_db_ptn_vuln_mp()
	{
		nID					= 0;
		nUsedMode			= 1;
		
		nType				= 0;
		nCode				= 0;
		
		nSysType			= 0;
		nSysSPType			= 0;
		nSysArchType		= 0;

		nMatchPackage		= 0;
		nMatchPolicy		= 0;
		nMatchControl		= 0;
		
		nChkType			= 0;
		nChkID				= 0;
	}

	UINT32		nID;
	UINT32		nUsedMode;

	UINT32		nType;
	UINT32		nCode;

	String		strCompanyName;
	String		strFamilyName;
	String		strProductName;

	UINT64		nSysType;
	UINT32		nSysSPType;
	UINT32		nSysArchType;

	UINT64		nMatchPackage;
	UINT64		nMatchPolicy;
	UINT64		nMatchControl;

	UINT32		nChkType;
	UINT32		nChkID;

}DB_PTN_VULN_MP, *PDB_PTN_VULN_MP;

typedef list<DB_PTN_VULN_MP>				TListDBPtnVulnMP;
typedef TListDBPtnVulnMP::iterator		TListDBPtnVulnMPItor;

typedef map<UINT32, DB_PTN_VULN_MP>		TMapDBPtnVulnMP;
typedef TMapDBPtnVulnMP::iterator			TMapDBPtnVulnMPItor;

#endif /*DB_PTN_VULN_MP_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE*/



