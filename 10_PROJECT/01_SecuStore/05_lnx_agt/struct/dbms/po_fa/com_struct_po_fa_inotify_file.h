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

#ifndef _COM_STRUCT_PO_FA_INOTIFY_FILE_H__
#define _COM_STRUCT_PO_FA_INOTIFY_FILE_H__

typedef struct _db_log_notify
{
    _db_log_notify()
    {
        nID 	= 0;
    }

	UINT32				nID ;
	UINT32				nUsedFlag;
	UINT32				nRegDate;
	UINT32				nOrderID;

	String				strNotifyFilePath;
}DB_LOG_NOTIFY_FILE, *PDB_LOG_NOTIFY_FILE;

typedef list<DB_LOG_NOTIFY_FILE>				TListLogNotifyFile;
typedef TListLogNotifyFile::iterator			TListLogNotifyFileItor;

typedef map<UINT32, DB_LOG_NOTIFY_FILE>			TMapLogNotifyFile;
typedef TMapLogNotifyFile::iterator			TMapLogNotifyFileItor;

#endif //_COM_STRUCT_PO_FA_INOTIFY_FILE_H__



