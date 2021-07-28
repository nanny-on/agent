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

#ifndef DB_PO_FE_NOTIFY_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_FE_NOTIFY_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_fe_notify
{
	_db_po_fe_notify()
	{
		nMsgType		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nMsgType;

}DB_PO_FE_NOTIFY, *PDB_PO_FE_NOTIFY;

typedef list<DB_PO_FE_NOTIFY>				TListDBPoFeNotify;
typedef TListDBPoFeNotify::iterator			TListDBPoFeNotifyItor;

typedef map<UINT32, DB_PO_FE_NOTIFY>		TMapDBPoFeNotify;
typedef TMapDBPoFeNotify::iterator			TMapDBPoFeNotifyItor;
//---------------------------------------------------------------------------

typedef struct _db_po_fe_notify_pkg
{
	_db_po_fe_notify_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_FE_NOTIFY_PKG, *PDB_PO_FE_NOTIFY_PKG;

typedef list<DB_PO_FE_NOTIFY_PKG>				TListDBPoFeNotifyPkg;
typedef TListDBPoFeNotifyPkg::iterator			TListDBPoFeNotifyPkgItor;

typedef map<UINT32, DB_PO_FE_NOTIFY_PKG>		TMapDBPoFeNotifyPkg;
typedef TMapDBPoFeNotifyPkg::iterator			TMapDBPoFeNotifyPkgItor;
//---------------------------------------------------------------------------

#endif //DB_PO_FE_NOTIFY_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



