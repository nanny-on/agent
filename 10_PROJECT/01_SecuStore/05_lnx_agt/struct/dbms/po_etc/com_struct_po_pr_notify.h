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

#ifndef DB_PO_PR_NOTIFY_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_PR_NOTIFY_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_pr_notify
{
	_db_po_pr_notify()
	{
		nMsgType		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nMsgType;

}DB_PO_PR_NOTIFY, *PDB_PO_PR_NOTIFY;

typedef list<DB_PO_PR_NOTIFY>				TListDBPoPrNotify;
typedef TListDBPoPrNotify::iterator			TListDBPoPrNotifyItor;

typedef map<UINT32, DB_PO_PR_NOTIFY>		TMapDBPoPrNotify;
typedef TMapDBPoPrNotify::iterator			TMapDBPoPrNotifyItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pr_notify_pkg
{
	_db_po_pr_notify_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PR_NOTIFY_PKG, *PDB_PO_PR_NOTIFY_PKG;

typedef list<DB_PO_PR_NOTIFY_PKG>				TListDBPoPrNotifyPkg;
typedef TListDBPoPrNotifyPkg::iterator			TListDBPoPrNotifyPkgItor;

typedef map<UINT32, DB_PO_PR_NOTIFY_PKG>		TMapDBPoPrNotifyPkg;
typedef TMapDBPoPrNotifyPkg::iterator			TMapDBPoPrNotifyPkgItor;
//---------------------------------------------------------------------------

#endif //DB_PO_PR_NOTIFY_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



