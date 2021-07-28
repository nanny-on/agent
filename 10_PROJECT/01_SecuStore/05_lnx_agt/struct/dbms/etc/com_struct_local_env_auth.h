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

#ifndef DB_LOCAL_ENV_AUTH_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_LOCAL_ENV_AUTH_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_local_env_auth
{
	_db_local_env_auth()
	{	
		nID			= 0;
		nUsedFlag	= STATUS_USED_MODE_ON;
		nEvtTime	= 0;	
	}

	UINT32				nID;
	UINT32				nUsedFlag;
	UINT32				nEvtTime;

	String				strUniCode;
	String				strAccountID;
	String				strAccountPW;

}DB_LOCAL_ENV_AUTH, *PDB_LOCAL_ENV_AUTH;

typedef list<DB_LOCAL_ENV_AUTH>				TListDBLocalEnvAuth;
typedef TListDBLocalEnvAuth::iterator		TListDBLocalEnvAuthItor;

typedef map<UINT32, DB_LOCAL_ENV_AUTH>		TMapDBLocalEnvAuth;
typedef TMapDBLocalEnvAuth::iterator		TMapDBLocalEnvAuthItor;

#endif //DB_LOCAL_ENV_AUTH_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



