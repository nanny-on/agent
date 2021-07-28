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

#ifndef DB_PO_UPDATE_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_PO_UPDATE_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_update
{
	_db_po_update()
    {
		SetEmpty();
	}
	DB_PO_HEADER			tDPH;	
    
	UINT32					nUpMethod;
    UINT32					nLastUpdateID;

	void _db_po_update::SetEmpty()
	{
		nUpMethod			= 0;
		nLastUpdateID		= 0;
	}
    
}DB_PO_UPDATE, *PDB_PO_UPDATE;

typedef list<DB_PO_UPDATE>				TListDBPoUpdate;
typedef TListDBPoUpdate::iterator		TListDBPoUpdateItor;

typedef map<UINT32, DB_PO_UPDATE>		TMapDBPoUpdate;
typedef TMapDBPoUpdate::iterator		TMapDBPoUpdateItor;

#endif //DB_PO_UPDATE_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



