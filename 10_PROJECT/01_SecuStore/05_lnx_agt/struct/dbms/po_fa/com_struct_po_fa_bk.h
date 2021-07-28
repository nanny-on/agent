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

#ifndef DB_PO_FA_BK_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_FA_BK_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_fa_bk
{
	_db_po_fa_bk()
	{	  
		nKeepDay			= 0;
		nKeepSize			= 0;
		nFreeSize			= 0;
		nFileMaxSize		= 0;
	}

	DB_PO_HEADER		tDPH;		

	UINT32				nKeepDay;
	UINT32				nKeepSize;
	UINT32				nFreeSize;
	UINT32				nFileMaxSize;
}DB_PO_FA_BK, *PDB_PO_FA_BK;

typedef list<DB_PO_FA_BK>				TListDBPoFaBk;
typedef TListDBPoFaBk::iterator			TListDBPoFaBkItor;

typedef map<UINT32, DB_PO_FA_BK>		TMapDBPoFaBk;
typedef TMapDBPoFaBk::iterator			TMapDBPoFaBkItor;

#endif //DB_PO_FA_BK_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



