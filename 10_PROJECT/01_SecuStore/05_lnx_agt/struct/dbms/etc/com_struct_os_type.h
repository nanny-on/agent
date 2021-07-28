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

#ifndef DB_OS_TYPE_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_OS_TYPE_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_os_type
{
	_db_os_type()
    {
        nID			= 0;
        nType		= 0;
        
    }

    UINT32				nID ;
    String				strName;
    INT32				nType;

}DB_OS_TYPE, *PDB_OS_TYPE;

typedef list<DB_OS_TYPE>				TListDBOsType;
typedef TListDBOsType::iterator			TListDBOsTypeItor;

typedef map<UINT32, DB_OS_TYPE>			TMapDBOsType;
typedef TMapDBOsType::iterator			TMapDBOsTypeItor;

#endif //DB_OS_TYPE_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



