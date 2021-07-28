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

#ifndef DB_PTN_VERSION_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE
#define DB_PTN_VERSION_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE

typedef struct _db_ptn_version
{
	_db_ptn_version()
	{
		nID					= 0;		
	}

	UINT32		nID;
	String		strPtnVersion;

}DB_PTN_VERSION, *PDB_PTN_VERSION;

typedef list<DB_PTN_VERSION>			TListDBPtnVersion;
typedef TListDBPtnVersion::iterator	TListDBPtnVersionItor;

typedef map<UINT32, DB_PTN_VERSION>	TMapDBPtnVersion;
typedef TMapDBPtnVersion::iterator		TMapDBPtnVersionItor;

#endif /*DB_PTN_VERSION_H_5DBB6BFC_032B_42a5_81C6_C86A909331BE*/



