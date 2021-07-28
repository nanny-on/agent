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
 */
#ifndef DB_ENV_LOCAL_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_ENV_LOCAL_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_env_local
{
	_db_env_local()
	{
		nID				= 0;
		nUsedFlag		= 1;
		nRegDate		= 0;

		nChkType		= 0;
		nChkCode		= 0;
	}

	UINT32				nID;
	UINT32				nUsedFlag;
	UINT32				nRegDate;


	UINT32				nChkType;
	UINT32				nChkCode;
	TMapID				tNValueMap;
	TMapIDStr			tSValueMap;

}DB_ENV_LOCAL, *PDB_ENV_LOCAL;

typedef list<DB_ENV_LOCAL>					TListDBEnvLocal;
typedef TListDBEnvLocal::iterator			TListDBEnvLocalItor;

typedef map<UINT32, DB_ENV_LOCAL>			TMapDBEnvLocal;
typedef TMapDBEnvLocal::iterator			TMapDBEnvLocalItor;

#endif //DB_ENV_LOCAL_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



