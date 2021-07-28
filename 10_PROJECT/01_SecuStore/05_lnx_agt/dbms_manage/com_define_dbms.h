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


#ifndef	COM_DEFINE_DBMS_H_6AB3CD45_6B1E_4bCB_9AE9_F7924A2FF8B8
#define COM_DEFINE_DBMS_H_6AB3CD45_6B1E_4bCB_9AE9_F7924A2FF8B8


//DBMS Type
////////////////////////////////////////////////////////////////////////////////
#define		DBMS_TYPE_EMPTY		0
#define     DBMS_TYPE_MYSQL     1
#define     DBMS_TYPE_MYSQL5X   2
#define     DBMS_TYPE_MARIADB	3

#define     DBMS_TYPE_MSSQL     10
#define		DBMS_TYPE_EXCEL		20
#define     DBMS_TYPE_DB2	    30


#define		QUERY_OP_SUCCESS	0
#define		DBMS_LOAD_EMPTY		10

//Default Function Type
////////////////////////////////////////////////////////////////////////////////
typedef INT32   	(__closure	*ExecuteQueryType)(String strQuery);
typedef UINT32  	(__closure	*GetLastInsertIDType)();
typedef INT32       (__closure	*NextType)();
typedef UINT64      (__closure	*GetDBField_UInt64Type)(UINT32 nIndex);
typedef UINT32      (__closure	*GetDBField_UIntType)(UINT32 nIndex);
typedef INT32       (__closure	*GetDBField_IntType)(UINT32 nIndex);
typedef String      (__closure	*GetDBField_StringType)(UINT32 nIndex);
#ifdef __BCPLUSPLUS__ // Borland C++
typedef TDateTime   (__closure	*GetDBField_DateType)(UINT32 nIndex);
#endif

#endif //COM_DEFINE_DBMS_H_6AB3CD45_6B1E_4bCB_9AE9_F7924A2FF8B8
