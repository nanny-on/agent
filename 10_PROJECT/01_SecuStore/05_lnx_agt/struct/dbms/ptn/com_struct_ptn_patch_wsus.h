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

#ifndef DB_PTN_PATCH_WSUS_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE
#define DB_PTN_PATCH_WSUS_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE


typedef struct _db_ptn_patch_wsus
{
	_db_ptn_patch_wsus()
	{
		nID			= 0;
		nSysID		= 0;
		nSysSPID	= 0;
	}

	UINT32		nID;
	String		strName;
	String		strHash;
	UINT64		nSysID;
	UINT32		nSysSPID;

}DB_PTN_PATCH_WSUS, *PDB_PTN_PATCH_WSUS;

typedef list<DB_PTN_PATCH_WSUS>			TListDBPtnPatchWsus;
typedef TListDBPtnPatchWsus::iterator		TListDBPtnPatchWsusItor;

typedef map<UINT32, DB_PTN_PATCH_WSUS>		TMapDBPtnPatchWsus;
typedef TMapDBPtnPatchWsus::iterator		TMapDBPtnPatchWsusItor;

#endif /*DB_PTN_PATCH_WSUS_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE*/



