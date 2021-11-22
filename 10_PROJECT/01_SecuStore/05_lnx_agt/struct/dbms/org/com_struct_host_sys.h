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

#ifndef DB_HOST_SYS_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_HOST_SYS_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_host_sys
{
	_db_host_sys()
	{
		nID						= 0;
		nUsedFlag				= USED_FLAG_TYPE_USED;

		nOsID					= 0;
		nOsSP					= 0;
		nOsPd					= 0;
		nOsPa					= 0;
	} 

	UINT32				nID ;
	UINT32				nUsedFlag;
	UINT64				nOsID;	
	UINT32				nOsSP;	
	UINT32				nOsPd;
	String				strOsPdLux;	
	UINT32				nOsPa;
}DB_HOST_SYS, *PDB_HOST_SYS;

typedef list<DB_HOST_SYS>			TListDBHostSys;
typedef TListDBHostSys::iterator	TListDBHostSysItor;

typedef map<UINT32, DB_HOST_SYS>	TMapDBHostSys;
typedef TMapDBHostSys::iterator		TMapDBHostSysItor;

#endif //DB_HOST_SYS_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



