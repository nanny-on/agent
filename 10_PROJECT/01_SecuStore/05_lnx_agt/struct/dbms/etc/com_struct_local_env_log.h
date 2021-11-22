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

#ifndef DB_ENV_LOCAL_ENV_LOG_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_ENV_LOCAL_ENV_LOG_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_local_env_log
{
	_db_local_env_log()
	{	
		nID			= 0;
		nUsedFlag	= STATUS_USED_MODE_ON;
		nLogDocID	= 0;	
	}

	UINT32				nID;
	UINT32				nUsedFlag;
	UINT32				nLogDocID;

}DB_LOCAL_ENV_LOG, *PDB_LOCAL_ENV_LOG;

typedef list<DB_LOCAL_ENV_LOG>				TListDBLocalEnvLog;
typedef TListDBLocalEnvLog::iterator		TListDBLocalEnvLogItor;

typedef map<UINT32, DB_LOCAL_ENV_LOG>		TMapDBLocalEnvLog;
typedef TMapDBLocalEnvLog::iterator			TMapDBLocalEnvLogItor;

#endif //DB_LOCAL_ENV_LOG_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



