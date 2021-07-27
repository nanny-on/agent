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

#define		DBMS_TYPE_NOT_USED		0
#define		DBMS_TYPE_MYSQL			1
#define		DBMS_TYPE_MYSQL_5X		2
#define     DBMS_TYPE_MARIADB		3
#define     DBMS_TYPE_SQLITE		5
#define		DBMS_TYPE_MSSQL			10
#define		DBMS_TYPE_MSSQL_25E		11
#define		DBMS_TYPE_MSSQL_28		12

#define		DBMS_TYPE_ASHIN_FILEDB	100

#define		QUERY_OP_SUCCESS		0
#define		DBMS_LOAD_EMPTY			-10

////////////////////////////////////////////////////////////////////////////////
typedef INT32   	(*ExecuteQueryType)(LPCTSTR strQuery);
typedef UINT32  	(*GetLastInsertIDType)();
typedef INT32       (*NextType)();
typedef UINT64      (*GetDBField_UInt64Type)(UINT32 nIndex);
typedef UINT32      (*GetDBField_UIntType)(UINT32 nIndex);
typedef INT32       (*GetDBField_IntType)(UINT32 nIndex);
typedef String		(*GetDBField_StringType)(UINT32 nIndex);
typedef INT32		(*SetSelQueryNameType)(LPCTSTR lpName, ...);


#endif //COM_DEFINE_DBMS_H_6AB3CD45_6B1E_4bCB_9AE9_F7924A2FF8B8
