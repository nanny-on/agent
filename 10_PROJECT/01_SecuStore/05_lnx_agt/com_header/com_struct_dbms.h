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


#ifndef	COM_STRUCT_DBMS_H_6AB3CD45_6B1E_4bCB_9AE9_F7924A2FF8B8
#define COM_STRUCT_DBMS_H_6AB3CD45_6B1E_4bCB_9AE9_F7924A2FF8B8


//DBMS Account Info
////////////////////////////////////////////////////////////////////////////////

typedef struct _dbms_account_info
{
    INT32 	nDBType;
    INT32	nUsedQueryPool;
    String	strIP;
    INT32	nPort;
    String	strDB;
    String	strID;
    String	strPw;

	_dbms_account_info()
    {
    	nDBType 		= 0;
        nPort			= 0;
		nUsedQueryPool	= 0;
    }
}DBMS_ACCOUNT_INFO, *PDBMS_ACCOUNT_INFO;


#endif //COM_STRUCT_DBMS_H_6AB3CD45_6B1E_4bCB_9AE9_F7924A2FF8B8
