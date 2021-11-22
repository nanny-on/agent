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

//---------------------------------------------------------------------------

#ifndef DBMgrBaseH
#define DBMgrBaseH

#include "com_define_dbms.h"
#include "com_struct_dbms.h"

#include <string.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <errno.h>

typedef struct _COL
{
	SQLULEN size;
	int status;
	int reserved;
    SQLSMALLINT type, subtype, precision, nullable, scale;
	SQLSMALLINT reserved2[3];
    char* name;
    char* buffer;
	char* reserved3[6];
} COL;


#define MAX_COL_NUM				100
#define DBCONNECTION_TIMEOUT	10
#define DBQUERY_TIMEOUT			10
#define MAX_COL_NAME_LEN		256



//---------------------------------------------------------------------------

class CDBMgrBase
{
private:
    COL* m_pColumns[MAX_COL_NUM];
    int  m_nCols;
    void ClearBindings();
public:
   	static BOOL m_bConnected;
    static INT32 m_nDBMSFlag;
    static INT32 m_nDBMSType;
	static String m_strDBMSAddr;
	static String m_strMainDbName;
	static String m_strAccountID;
	static String m_strAccountPW;
	static String m_strLastTableName;
    static INT32 m_nUsedQueryPool;
	static pthread_mutex_t m_ExecuteMutex;
	static INT32  m_nExecuteCount;
	static SQLHANDLE m_hEnv; // environment handle
	static SQLHANDLE m_hDbc; // connection handle
	static SQLHANDLE m_hStmt;	// sql statement handle
	static TListStr m_tQueryPoolList;

public:
	String m_strQuery;
	String m_strDBTName;
	String m_strValue;
	char m_acLogMsg[MAX_LOGMSG+1];
    UINT32 m_nLoadMaxID;
    UINT32 m_nLoadNumber;

private:
	void  		SaveLastTableName(String strQuery);
    INT32   	Select_Query_ODBC(String strQuery);
	INT32		Execute_Query_ODBC(String strQuery);

public:
	INT32		InitSubControl();
    INT32		FreeSubControl();
    INT32   	SetProperty(DBMS_ACCOUNT_INFO& dai);
    INT32   	ConnectDB();
    INT32		IsConnected()	{	return !m_bConnected;	};
    INT32   	DisconnectDB();
	void 		OnAfterConnected();
	void		OnAfterDisconnected();
    INT32   	IsTableExists(String strTableName);
	UINT32  	GetLastID(); 
	UINT32  	GetDBVersion();
	INT32   	ExecuteQuery(String strQuery);
    INT32	    Next();                        
    UINT64		GetDBField_UInt64(UINT32 nIndex);
    UINT32		GetDBField_UInt(UINT32 nIndex);
    INT32		GetDBField_Int(UINT32 nIndex);
    String		GetDBField_String(UINT32 nIndex);
	StringW		GetDBField_StringW(UINT32 nIndex);
	DB_PO_HEADER	GetDBField_PoHDR(INT32& nIndex);
	DB_POLICY		GetDBField_Po(INT32& nIndex);
	DB_PO_HEADER	GetDBField_PoPkgHDR(INT32& nIndex);
	String			GetPoQuery_InsertValue(DB_POLICY& tDP);
	String			GetPoQuery_Update(DB_POLICY& tDP);
	String			GetPoHDRQuery_InsertValue(DB_PO_HEADER& tDPH);
	String			GetPoPkgHDRQuery_InsertValue(DB_PO_HEADER& tDPH);
	String			GetPoHDRQuery_Update(DB_PO_HEADER& tDPH);
	String			GetPoPkgHDRQuery_Update(DB_PO_HEADER& tDPH);
	INT32		DBOP_Check(INT32 nRtn);
    String      ChangeFilePathToDBPath(String strFilePath);
    String      ChangeDBPathToFilePath(String strDBPath);
	String		ConvertListToString(TListID& tIDList);

	INT32			ResetTable();

public:
	virtual	INT32	LoadExecute(PVOID lpTempletList)					{	return 0;	};
	virtual	INT32	InsertExecute(PVOID lpTemplet)						{	return 0;	};
	virtual	INT32	UpdateExecute(PVOID lpTemplet)						{	return 0;	};
	virtual	INT32	DeleteExecute(UINT32 nID);
	virtual	INT32	GetCountExecute()									{	return 0;	};

public:
	CDBMgrBase();
    ~CDBMgrBase();
};

extern CDBMgrBase*	t_DBMS_Base;

#endif
