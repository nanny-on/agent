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

#ifndef DB_HOST_HW_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_HOST_HW_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_host_hw
{
	_db_host_hw()
    {
		nID						= 0;
		nUsedFlag				= USED_FLAG_TYPE_USED;

		nPCType					= 0;
		nMemSize				= 0;
		nHDDSize				= 0;
		nLanNum					= 0;
    } 

    UINT32				nID ;
	UINT32				nUsedFlag;
	UINT32				nPCType;
	UINT32				nMemSize;
	UINT32				nHDDSize;
	UINT32				nLanNum;
	String				strBiosDate;
	String				strCPUName;
}DB_HOST_HW, *PDB_HOST_HW;

typedef list<DB_HOST_HW>			TListDBHostHw;
typedef TListDBHostHw::iterator		TListDBHostHwItor;

typedef map<UINT32, DB_HOST_HW>		TMapDBHostHw;
typedef TMapDBHostHw::iterator		TMapDBHostHwItor;

#endif //DB_HOST_HW_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



