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


#include "stdafx.h"
#include "com_struct.h"
#include "ThreadDBMSQuery.h"
#include "DBMgrBase.h"


CDBMgrBase*	t_DBMS_Base = NULL;


BOOL CDBMgrBase::m_bConnected = FALSE;
INT32 CDBMgrBase::m_nDBMSFlag = 0;
INT32 CDBMgrBase::m_nDBMSType = DBMS_TYPE_SQLITE;
INT32 CDBMgrBase::m_nUsedQueryPool = 0;
SQLHANDLE CDBMgrBase::m_hEnv = NULL;
SQLHANDLE CDBMgrBase::m_hDbc = NULL;
SQLHANDLE CDBMgrBase::m_hStmt = NULL;
pthread_mutex_t CDBMgrBase::m_ExecuteMutex;
INT32  CDBMgrBase::m_nExecuteCount = 0;
String CDBMgrBase::m_strDBMSAddr;
String CDBMgrBase::m_strMainDbName;
String CDBMgrBase::m_strAccountID;
String CDBMgrBase::m_strAccountPW;
String CDBMgrBase::m_strLastTableName;
TListStr CDBMgrBase::m_tQueryPoolList;


//---------------------------------------------------------------------------

CDBMgrBase::CDBMgrBase()
{
	m_nLoadMaxID 	= 0;
    m_nLoadNumber	= 0;
    m_nCols = 0;
	m_strDBTName = "mgr_base";
	memset(m_acLogMsg, 0, MAX_LOGMSG+1);
}
//---------------------------------------------------------------------------

CDBMgrBase::~CDBMgrBase()
{
    ClearBindings();
	DisconnectDB();
}

INT32		CDBMgrBase::DBOP_Check(INT32 nRtn)
{
    return nRtn;
}
//---------------------------------------------------------------------------

String		CDBMgrBase::ConvertListToString(TListID& tIDList)
{
	String strIDs;
	String strTemp;

    TListIDItor begin, end;
    begin = tIDList.begin();	end = tIDList.end();
    for(begin; begin != end; begin++)
    {
    	if(strIDs.empty())
		{
			strTemp = SPrintf("%u", *begin);	
			strIDs = strTemp;
		}
        else
		{
			strTemp = SPrintf(", %u", *begin);	
			strIDs += strTemp;
		}
    }
    return strIDs;
}

//---------------------------------------------------------------------------

String      CDBMgrBase::ChangeFilePathToDBPath(String strFilePath)
{
    String strResult;
    for(int i = 1 ; i < strFilePath.length() + 1; i++)
    {
        if(strFilePath[i] == STR_TOKEN_FILE_PATH)
        {
            strResult += STR_TOKEN_DB_PATH;
        }
        else
            strResult += strFilePath[i];
    }
    return strResult;
}
//------------------------------------------------------------------------------

String      CDBMgrBase::ChangeDBPathToFilePath(String strDBPath)
{
    String strResult;
    for(int i = 1 ; i < strDBPath.length() + 1; i++)
    {
        if(strDBPath[i] == STR_TOKEN_DB_PATH)
        {
            strResult += STR_TOKEN_FILE_PATH;
        }
        else
            strResult += strDBPath[i];
    }
    return strResult;
}


void CDBMgrBase::ClearBindings()
{
    for(int i = 0; i <m_nCols; i++)
    {
    	if(m_pColumns[i] != NULL)
    	{
			safe_free(m_pColumns[i]->name);
			safe_free(m_pColumns[i]->buffer);
	        safe_free(m_pColumns[i]);
    	}
    }
    m_nCols = 0;
}
//---------------------------------------------------------------------------

INT32   CDBMgrBase::SetProperty(DBMS_ACCOUNT_INFO& dai)
{
	m_nDBMSType			= dai.nDBType;
	m_nUsedQueryPool    = dai.nUsedQueryPool;
	m_strMainDbName    	= dai.strDB;
	m_strAccountID  	= dai.strID;
	return 0;
}
//---------------------------------------------------------------------------

INT32   CDBMgrBase::ConnectDB()
{
	int rc = 0;
    if(m_bConnected == TRUE)
        return 0;
    
	rc = CLIAppInitLocal((char*)m_strMainDbName.c_str(), (char*)m_strAccountID.c_str(), NULL, &m_hEnv, &m_hDbc, (SQLPOINTER)SQL_AUTOCOMMIT_ON, m_acLogMsg);
	if(rc != 0 || m_hEnv == NULL || m_hDbc == NULL)
	{
		WriteLogE("fail to connect db (%d) : %s", rc, m_acLogMsg);
		memset(m_acLogMsg, 0, MAX_LOGMSG+1);
		m_bConnected = FALSE;
		return -1;
	}

	pthread_mutex_init(&m_ExecuteMutex, NULL);
	ClearBindings();
	m_nExecuteCount = 0;

	m_bConnected = TRUE;
	return 0;
}
//---------------------------------------------------------------------------

void CDBMgrBase::OnAfterConnected()
{
    m_bConnected = TRUE;
}
//--------------------------------------------------------------------------- 

void CDBMgrBase::OnAfterDisconnected()
{
    m_bConnected = FALSE;
}
//---------------------------------------------------------------------------

INT32   CDBMgrBase::DisconnectDB()
{
    if(m_bConnected == FALSE)
        return 0;

	CLIAppTerm(&m_hEnv, &m_hDbc);
	pthread_mutex_destroy(&m_ExecuteMutex);
	m_nExecuteCount = 0;
	m_bConnected = FALSE;
	return 0;
}
//---------------------------------------------------------------------------

INT32   CDBMgrBase::ExecuteQuery(String strQuery)
{
	int nRetVal = 0;
	char acCmd[8] = {0,};
	if(m_bConnected == FALSE)
	{
        if(ConnectDB() != 0)
            return -1;
	}
	strncpy(acCmd, (char *)strQuery.c_str(), 6);
	acCmd[6] = 0;
//    String strCheck = _strlwr((char*)strQuery.c_str());
    if(!_stricmp(acCmd, "select"))
    {
    	nRetVal = Select_Query_ODBC(strQuery);
    }
	else
	{
		nRetVal = Execute_Query_ODBC(strQuery);
	}
    return nRetVal;
}
//---------------------------------------------------------------------------

void   CDBMgrBase::SaveLastTableName(String strQuery)
{
	INT32 nStart = 0;
	INT32 nEnd = 0;
	char *pQuery = NULL;
	if(strQuery.length() < 7)
		return;
	for(nStart=7; nStart + 4 < strQuery.length(); nStart++)
	{
		pQuery = (char *)(strQuery.c_str() + nStart);
		if(_strnicmp(pQuery, "FROM ", 5) == 0)
		{
			nStart = nStart + 5;
			for( ; nStart < strQuery.length(); nStart++)
			{
				if(strQuery[nStart] != ' ')
					break;
			}
			nEnd = nStart+1;
			for( ;nEnd < strQuery.length(); nEnd++)
			{
				if(strQuery[nEnd] == ' ')
				{
					strQuery[nEnd] = '\0';
					m_strLastTableName = &strQuery[nStart];
					nStart = strQuery.length();
					break;
				}
			}
		}
		else if(_strnicmp(pQuery, "INTO ", 5) == 0)
		{
			nEnd = strQuery.find("(");
			if(nEnd != string::npos && nStart < nEnd)
			{
				strQuery[nEnd] = '\0';
				m_strLastTableName = &strQuery[nStart + 5];
			}
		}
		
	}
}

INT32   CDBMgrBase::Select_Query_ODBC(String strQuery)
{
	SQLRETURN cliRC = SQL_SUCCESS;
	INT32 i, rc = 0;
	INT32 nRetVal = 0;
	INT32 nNameLen = 0;
	SQLCHAR *pStmt = NULL;
	COL* pCol = NULL;
	SQLLEN nLen = 0;
	SQLSMALLINT nQueryLen = 0;

	if(m_hDbc == NULL)
	{
		WriteLogE("[Select_Query_ODBC] invalid input handle");
		return -1;
	}
	m_strLastTableName = "";
	memset(m_acLogMsg, 0, MAX_LOGMSG+1);
	
	pthread_mutex_lock(&m_ExecuteMutex);
	ClearBindings();
	m_nExecuteCount++;
	do{
		cliRC = SQLSetConnectAttr(m_hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			DBC_HANDLE_CHECK(m_hDbc, cliRC, m_acLogMsg);
			WriteLogE("[Select_Query_ODBC] fail to set auto commit on (%d) : %s", cliRC, m_acLogMsg);
			nRetVal = -2;
			break;
		}
		/* allocate a statement handle */
		cliRC = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
		rc = CheckReturnValue(cliRC);
		if (rc != 0 || m_hStmt == NULL)
		{
			DBC_HANDLE_CHECK(m_hDbc, cliRC, m_acLogMsg);
			WriteLogE("[Select_Query_ODBC] fail to allocate handle (%d) : %s", cliRC, m_acLogMsg);
			nRetVal = -3;
			break;
		}

		pStmt = (SQLCHAR *)strQuery.c_str();
		cliRC = SQLPrepare(m_hStmt, pStmt, SQL_NTS);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			STMT_HANDLE_CHECK(m_hStmt, m_hDbc, cliRC, m_acLogMsg);
			WriteLogE("[Select_Query_ODBC] fail to prepare %s (%d) : %s", (char *)pStmt, cliRC, m_acLogMsg);
			nRetVal = -4;
			break;
		}

		cliRC = SQLNumResultCols(m_hStmt, (SQLSMALLINT*)&m_nCols);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			STMT_HANDLE_CHECK(m_hStmt, m_hDbc, cliRC, m_acLogMsg);
			WriteLogE("[Select_Query_ODBC] fail to get num clos %s (%d) : %s",  (char *)pStmt, cliRC, m_acLogMsg);
			nRetVal = -5;
			break;
		}
	
		if(m_nCols > MAX_COL_NUM)
			m_nCols = MAX_COL_NUM;

		rc = 0;
		nNameLen = sizeof(SQLCHAR)*MAX_COL_NAME_LEN;
		for(i = 0; i< m_nCols; i++)
		{
			m_pColumns[i] = (COL*)malloc(sizeof(COL));
			if(m_pColumns[i] == NULL)
			{
				WriteLogE("[Select_Query_ODBC] fail to get allocate col memory (%d)", errno);
				rc = -6;
				break;
			}
			memset(m_pColumns[i], 0, sizeof(COL));
			pCol = m_pColumns[i];
			pCol->name = (char*)malloc(nNameLen+1);
			if(pCol->name == NULL)
			{
				WriteLogN("[Select_Query_ODBC] fail to get allocate name memory (%d)", errno);
				rc = -7;
				break;
			}
			memset(pCol->name, 0, nNameLen+1);
		
			pCol->type = SQL_UNKNOWN_TYPE;
			nQueryLen = 0;
			cliRC = SQLDescribeCol(m_hStmt, 
					i+1,
					(SQLCHAR*)pCol->name,
					nNameLen,
					&nQueryLen,
					(SQLSMALLINT*)&pCol->type,
					(SQLULEN *)&pCol->size,
					(SQLSMALLINT*)&pCol->scale,
					(SQLSMALLINT*)&pCol->nullable);
			rc = CheckReturnValue(cliRC);
			if (rc != 0)
			{
				STMT_HANDLE_CHECK(m_hStmt, m_hDbc, cliRC, m_acLogMsg); 
				WriteLogE("[Select_Query_ODBC] fail to get describe col (%d) : %s", cliRC, m_acLogMsg);
				rc = -8;
				break;
			}

			if(pCol->size < 0)
			{
				WriteLogE("[Select_Query_ODBC] invalid col size (%d)", pCol->size);
				rc = -9;
				break;
			}
			pCol->type = SQL_C_BINARY;
			pCol->buffer = (char*)malloc(pCol->size + 1);
			if(pCol->buffer == NULL)
			{
				WriteLogE("[Select_Query_ODBC] fail to allocate buffer memory (%d)", errno);
				rc = -11;
				break;
			}
			memset(pCol->buffer, 0, pCol->size + 1);
		
			cliRC = SQLBindCol(m_hStmt, i+1, (SQLSMALLINT)pCol->type, pCol->buffer, pCol->size, &nLen);  
			rc = CheckReturnValue(cliRC);
			if (rc != 0)
			{
				STMT_HANDLE_CHECK(m_hStmt, m_hDbc, cliRC, m_acLogMsg); 
				WriteLogE("[Select_Query_ODBC] fail to bind col (%d) : %s", cliRC, m_acLogMsg);
				rc = -12;
				break;
			}
			rc = 0;
		}

		if (rc != 0)
		{
			ClearBindings();
			nRetVal = rc;
			break;
		}
	
		cliRC = SQLExecute(m_hStmt);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			STMT_HANDLE_CHECK(m_hStmt, m_hDbc, cliRC, m_acLogMsg); 
			WriteLogE("[Select_Query_ODBC] fail to execute (%d) : %s", cliRC, m_acLogMsg);
			ClearBindings();
			nRetVal = -13;
			break;
		}


		cliRC = SQLFetch(m_hStmt);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			STMT_HANDLE_CHECK(m_hStmt, m_hDbc, cliRC, m_acLogMsg); 
			WriteLogE("[Select_Query_ODBC] fail to fetch (%d) : %s", cliRC, m_acLogMsg);
			ClearBindings();
			nRetVal = -14;
			break;
		}

		if(cliRC == SQL_NO_DATA_FOUND)
		{
			nRetVal = DBMS_LOAD_EMPTY;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	
	if(nRetVal != 0)
	{
		FreeStmtHandle(m_hStmt);
		m_nExecuteCount--;
		if(m_nExecuteCount != 0)
		{
			WriteLogE("[Select_Query_ODBC] invalid mutex lock : %d", m_nExecuteCount);
		}
		pthread_mutex_unlock(&m_ExecuteMutex);
		return 0;
	}

	SaveLastTableName(strQuery);
	return 0;
}

//---------------------------------------------------------------------------

INT32   CDBMgrBase::Execute_Query_ODBC(String strQuery)
{
	INT32 i, nRetVal = 0;
	INT32 nResult, nLen = 0;
	SQLRETURN cliRC = SQL_SUCCESS;	
	int rc = 0;
	char *pData = NULL;
	if(m_hDbc == NULL)
	{
		WriteLogN("[ExecuteQuery_Exe] invalid input data");
		return -1;
	}
    
	m_strLastTableName = "";
	memset(m_acLogMsg, 0, MAX_LOGMSG+1);
	pthread_mutex_lock(&m_ExecuteMutex);
	ClearBindings();
	m_nExecuteCount++;
	/* set AUTOCOMMIT OFF */  
	do{
		cliRC = SQLSetConnectAttr(m_hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			DBC_HANDLE_CHECK(m_hDbc, cliRC, m_acLogMsg);
			WriteLogN("[ExecuteQuery_Exe] fail to set auto commit off (%d) : %s", cliRC, m_acLogMsg);
			nRetVal = -2;
			break;
		}
	
		/* allocate a statement handle */  
		cliRC = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
		rc = CheckReturnValue(cliRC);
		if (rc != 0 || m_hStmt == NULL)
		{
			DBC_HANDLE_CHECK(m_hDbc, cliRC, m_acLogMsg);
			WriteLogN("[ExecuteQuery_Exe] fail to allocate handle (%d) : %s", cliRC, m_acLogMsg);
			nRetVal = -3;
			break;
		}
	
		/* directly execute a statement*/  
		cliRC = SQLExecDirect(m_hStmt, (SQLCHAR *)strQuery.c_str(), SQL_NTS);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			Sleep(200);
			nLen = strQuery.length();
			pData = (char *)strQuery.c_str();
			if(pData[nLen-1] == ';')
			{
				DelLastChar(strQuery, ';');
			}
			cliRC = SQLExecDirect(m_hStmt, (SQLCHAR *)strQuery.c_str(), SQL_NTS);
			rc = CheckReturnValue(cliRC);
			if (rc != 0)
			{
				STMT_HANDLE_CHECK(m_hStmt, m_hDbc, cliRC, m_acLogMsg);
				WriteLogE("[ExecuteQuery_Exe] fail to exec %s (%d) : %s", (char *)strQuery.c_str(), cliRC, m_acLogMsg);
				nRetVal = -4;
				break;
			}
		}
		/* end the transactions on the connection */
		cliRC = SQLEndTran(SQL_HANDLE_DBC, m_hDbc, SQL_COMMIT);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			DBC_HANDLE_CHECK(m_hDbc, cliRC, m_acLogMsg);
			WriteLogN("[ExecuteQuery_Exe] fail to end tran (%d) : %s", cliRC, m_acLogMsg);
			nRetVal = -5;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	FreeStmtHandle(m_hStmt);
	m_nExecuteCount--;
	if(m_nExecuteCount != 0)
	{
		WriteLogE("[Execute_Query_ODBC] invalid mutex lock : %d", m_nExecuteCount);
	}
	pthread_mutex_unlock(&m_ExecuteMutex);
	if(_strnicmp(strQuery.c_str(), "INSERT ", 7) == 0)
	{
		SaveLastTableName(strQuery);
	}

	return nRetVal;
}
//---------------------------------------------------------------------------

UINT32  CDBMgrBase::GetLastID()
{
    UINT32 nID = 0;
	if(m_strLastTableName.length() == 0)
		return 0;
    String strQuery = SPrintf("SELECT MAX(id) from %s;", m_strLastTableName.c_str());
	if(Select_Query_ODBC(strQuery) != 0)
	{
		return 0;
    }
	nID = GetDBField_UInt(0);
	FreeStmtHandle(m_hStmt);

	m_nExecuteCount--;
	if(m_nExecuteCount != 0)
	{
		WriteLogE("[GetLastID] invalid mutex lock : %d", m_nExecuteCount);
	}
	pthread_mutex_unlock(&m_ExecuteMutex);

    return nID;
}
//---------------------------------------------------------------------------

UINT32  CDBMgrBase::GetDBVersion()
{
    UINT32 nID = 0;
    String strQuery = "select sqlite_version()";
	if(Select_Query_ODBC(strQuery) != 0)
	{
		return 0;
    }
	nID = GetDBField_UInt(0);
	FreeStmtHandle(m_hStmt);
	m_nExecuteCount--;
	if(m_nExecuteCount != 0)
	{
		WriteLogE("[GetDBVersion] invalid mutex lock : %d", m_nExecuteCount);
	}
	pthread_mutex_unlock(&m_ExecuteMutex);
    return nID;
}

INT32			CDBMgrBase::DeleteExecute(UINT32 nID)
{
	String strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
	if(Execute_Query_ODBC(strQuery) != 0)
		return ERR_DBMS_DELETE_FAIL;
	return 0;
}

INT32       CDBMgrBase::IsTableExists(String strTableName)
{
    String strQuery = String("SELECT sql FROM sqlite_master WHERE name='%s';", strTableName.c_str());
	if(Select_Query_ODBC(strQuery) != 0)
	{
		return 0;
    }
	FreeStmtHandle(m_hStmt);
	m_nExecuteCount--;
	if(m_nExecuteCount != 0)
	{
		WriteLogE("[IsTableExists] invalid mutex lock : %d", m_nExecuteCount);
	}
	pthread_mutex_unlock(&m_ExecuteMutex);
    return 1;
}
//---------------------------------------------------------------------------

INT32       CDBMgrBase::Next()
{
	if(m_hStmt == NULL)
		return 0;
	for(int i = 0; i<m_nCols; i++)
	{
		if(m_pColumns[i]->buffer && m_pColumns[i]->size)
		{
			memset(m_pColumns[i]->buffer, 0, m_pColumns[i]->size+1);
		}
	}

	if(SQLFetch(m_hStmt) != SQL_NO_DATA_FOUND)
	{
		return 1;
	}
	FreeStmtHandle(m_hStmt);
	m_nExecuteCount--;
	if(m_nExecuteCount != 0)
	{
		WriteLogE("[Next] invalid mutex lock : %d", m_nExecuteCount);
	}
	pthread_mutex_unlock(&m_ExecuteMutex);
	return 0;
}
//---------------------------------------------------------------------------

UINT64		CDBMgrBase::GetDBField_UInt64(UINT32 nIndex)
{
	UINT64 ddwRetVal = 0;
    if(!m_nCols || nIndex > MAX_COL_NUM-1)
        return 0;

    COL* pCol = m_pColumns[nIndex];
	if(pCol == NULL)
		return 0;
	
	if(pCol->status == -1 || pCol->size < 1)
		return 0;

	ddwRetVal = (UINT64)atoll(pCol->buffer);
	return ddwRetVal;

}
//---------------------------------------------------------------------------

UINT32      CDBMgrBase::GetDBField_UInt(UINT32 nIndex)
{
	UINT32 dwRetVal = 0;
    if(!m_nCols || nIndex > MAX_COL_NUM-1)
        return 0;

    COL* pCol = m_pColumns[nIndex];
	if(pCol == NULL)
		return 0;
	
	if(pCol->status == -1 || pCol->size < 1)
		return 0;

	dwRetVal = (UINT32)atoi(pCol->buffer);
	return dwRetVal;
}
//---------------------------------------------------------------------------

INT32       CDBMgrBase::GetDBField_Int(UINT32 nIndex)
{
	INT32 nRetVal = 0;
    if(!m_nCols || nIndex > MAX_COL_NUM-1)
        return 0;

    COL* pCol = m_pColumns[nIndex];
	if(pCol == NULL)
		return 0;
	
	if(pCol->status == -1 || pCol->size < 1)
		return 0;

	nRetVal = atoi(pCol->buffer);

	return nRetVal;
}
//---------------------------------------------------------------------------

String      CDBMgrBase::GetDBField_String(UINT32 nIndex)
{
    if(!m_nCols || nIndex > MAX_COL_NUM-1)
        return 0;

    COL* pCol = m_pColumns[nIndex];
	if(pCol == NULL)
		return "";

	if(pCol->status == -1)
		return "";

	return pCol->buffer;		
}

StringW		CDBMgrBase::GetDBField_StringW(UINT32 nIndex)
{
    if(!m_nCols || nIndex > MAX_COL_NUM-1)
        return 0;

    COL* pCol = m_pColumns[nIndex];
	if(pCol == NULL)
		return L"";

	if(pCol->status == -1)
		return L"";

	return (PWCHAR)pCol->buffer;
}


DB_POLICY		CDBMgrBase::GetDBField_Po(INT32& nIndex)
{
	DB_POLICY tDP;

	tDP.nPUpdateID			= GetDBField_UInt(nIndex++);  
	tDP.nPSvrInfoLgnID		= GetDBField_UInt(nIndex++);  
	tDP.nPHostRmInfoID		= GetDBField_UInt(nIndex++); 
	tDP.nPHostRunID			= GetDBField_UInt(nIndex++);
	tDP.nPHostClearID		= GetDBField_UInt(nIndex++);
	tDP.nPPowerID			= GetDBField_UInt(nIndex++); 
	tDP.nPCtlPanelID		= GetDBField_UInt(nIndex++); 
	tDP.nPCtlProcID			= GetDBField_UInt(nIndex++); 
	tDP.nPSelfProtectID		= GetDBField_UInt(nIndex++);
	tDP.nPDeployFileID		= GetDBField_UInt(nIndex++);
	tDP.nPFaOpID			= GetDBField_UInt(nIndex++);
	tDP.nPFaClearID			= GetDBField_UInt(nIndex++);
	tDP.nPFaProcID			= GetDBField_UInt(nIndex++);
	tDP.nPFaEnvID			= GetDBField_UInt(nIndex++);
	tDP.nPFaBkID			= GetDBField_UInt(nIndex++);
	tDP.nPWinRunID			= GetDBField_UInt(nIndex++);
	tDP.nPSvrInfoUdtID		= GetDBField_UInt(nIndex++); 
	tDP.nPSvrInfoMgrID		= GetDBField_UInt(nIndex++); 

	tDP.nPFePtnOpID			= GetDBField_UInt(nIndex++); 
	tDP.nPFePtnBLID			= GetDBField_UInt(nIndex++); 
	tDP.nPFePtnWLID			= GetDBField_UInt(nIndex++); 
	tDP.nPFeExcludeID		= GetDBField_UInt(nIndex++); 
	tDP.nPFeNotifyID		= GetDBField_UInt(nIndex++); 
	tDP.nPFeSinglePtnID		= GetDBField_UInt(nIndex++); 
	tDP.nPFePtnLoID			= GetDBField_UInt(nIndex++); 

	tDP.nPFaNotifyID		= GetDBField_UInt(nIndex++); 
	tDP.nPSchID				= GetDBField_UInt(nIndex++);

	tDP.nPInPtnOpID			= GetDBField_UInt(nIndex++);
	tDP.nPInPtnBLID			= GetDBField_UInt(nIndex++);
	tDP.nPInPtnWLID			= GetDBField_UInt(nIndex++);
	tDP.nPInPtnExID			= GetDBField_UInt(nIndex++);
	tDP.nPInPtnSPID			= GetDBField_UInt(nIndex++);
	tDP.nPInPtnNoID			= GetDBField_UInt(nIndex++);

	tDP.nReservedA			= GetDBField_UInt(nIndex++);
	tDP.nReservedB			= GetDBField_UInt(nIndex++);
	tDP.nReservedC			= GetDBField_UInt(nIndex++);	

	tDP.nPInVulnOpID		= GetDBField_UInt(nIndex++);
	tDP.nPInVulnScanID		= GetDBField_UInt(nIndex++);
	tDP.nPHostNotifyID		= GetDBField_UInt(nIndex++);
	tDP.nPInDevOOpID		= GetDBField_UInt(nIndex++);
	tDP.nPInDevOBLID		= GetDBField_UInt(nIndex++);
	tDP.nPInDevOWLID		= GetDBField_UInt(nIndex++);
	tDP.nPInDevOExID		= GetDBField_UInt(nIndex++);

	tDP.nPPmOpID			= GetDBField_UInt(nIndex++);
	tDP.nPPmDmID			= GetDBField_UInt(nIndex++);
	tDP.nPPmScanID			= GetDBField_UInt(nIndex++);
	tDP.nPPmExID			= GetDBField_UInt(nIndex++);
	tDP.nPPmNoID			= GetDBField_UInt(nIndex++);

	tDP.nPInVulnAxID			= GetDBField_UInt(nIndex++);
	tDP.nPInVulnEditAppID		= GetDBField_UInt(nIndex++);
	tDP.nPInVulnQnaID			= GetDBField_UInt(nIndex++);
	tDP.nPInVulnSecuUsbID		= GetDBField_UInt(nIndex++);
	tDP.nPInVulnSwID			= GetDBField_UInt(nIndex++);
	tDP.nPInVulnPatchExceptionID		= GetDBField_UInt(nIndex++);
	tDP.nPInDevONotifyID		= GetDBField_UInt(nIndex++);

	tDP.nPInRsOpID			= GetDBField_UInt(nIndex++);
	tDP.nPInRsNoID			= GetDBField_UInt(nIndex++);
	tDP.nPInRsBkID			= GetDBField_UInt(nIndex++);

	tDP.nPNcPtnOpID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnLoID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnBLID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnWLID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnExID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnSPID			= GetDBField_UInt(nIndex++);
	tDP.nPNcPtnNoID			= GetDBField_UInt(nIndex++);
	
	tDP.nPHostWebSockID		= GetDBField_UInt(nIndex++);


	return tDP;
}
//---------------------------------------------------------------------------

DB_PO_HEADER	CDBMgrBase::GetDBField_PoHDR(INT32& nIndex)
{
	DB_PO_HEADER tDPH;

	tDPH.nID					= GetDBField_UInt(nIndex++);
	tDPH.nRegDate				= GetDBField_UInt(nIndex++);
	tDPH.nSeqNo					= GetDBField_UInt(nIndex++);
	tDPH.strName				= GetDBField_String(nIndex++);
	tDPH.strDescr				= GetDBField_String(nIndex++);
	tDPH.nAdminID				= GetDBField_UInt(nIndex++);
	tDPH.nSubAdminLock			= GetDBField_UInt(nIndex++);
	tDPH.nTargetLock			= GetDBField_UInt(nIndex++);
	tDPH.nTargetOS				= GetDBField_UInt(nIndex++);
	tDPH.nExtOption				= GetDBField_UInt(nIndex++);
	tDPH.nUsedMode				= GetDBField_UInt(nIndex++);

	return tDPH;
}
//---------------------------------------------------------------------------

DB_PO_HEADER	CDBMgrBase::GetDBField_PoPkgHDR(INT32& nIndex)
{
	DB_PO_HEADER tDPH;

	tDPH.nID					= GetDBField_UInt(nIndex++);
	tDPH.nRegDate				= GetDBField_UInt(nIndex++);
	tDPH.strName				= GetDBField_String(nIndex++);
	tDPH.strDescr				= GetDBField_String(nIndex++);
	tDPH.nAdminID				= GetDBField_UInt(nIndex++);
	tDPH.nSubAdminLock			= GetDBField_UInt(nIndex++);
	tDPH.nPolicyID				= GetDBField_UInt(nIndex++);
	tDPH.nUnitID				= GetDBField_UInt(nIndex++);
	tDPH.nTargetOS				= GetDBField_UInt(nIndex++);
	tDPH.nExtOption				= GetDBField_UInt(nIndex++);
	tDPH.nUsedMode				= GetDBField_UInt(nIndex++);

	return tDPH;
}

String			CDBMgrBase::GetPoQuery_InsertValue(DB_POLICY& tDP)
{
	String strRtn;
	strRtn = SPrintf(DBMS_POLICY_QUERY_INSERT_VALUE, 
		tDP.nPUpdateID, tDP.nPSvrInfoLgnID, tDP.nPHostRmInfoID, tDP.nPHostRunID, tDP.nPHostClearID, 
		tDP.nPPowerID, tDP.nPCtlPanelID, tDP.nPCtlProcID, tDP.nPSelfProtectID, tDP.nPDeployFileID,
		tDP.nPFaOpID, tDP.nPFaClearID, tDP.nPFaProcID, tDP.nPFaEnvID, tDP.nPFaBkID,
		tDP.nPWinRunID, tDP.nPSvrInfoUdtID, tDP.nPSvrInfoMgrID,
		tDP.nPFePtnOpID, tDP.nPFePtnBLID, tDP.nPFePtnWLID, tDP.nPFeExcludeID, tDP.nPFeNotifyID, tDP.nPFeSinglePtnID, tDP.nPFePtnLoID,
		tDP.nPFaNotifyID, tDP.nPSchID,
		tDP.nPInPtnOpID, tDP.nPInPtnBLID, tDP.nPInPtnWLID, tDP.nPInPtnExID, tDP.nPInPtnSPID, tDP.nPInPtnNoID,
		tDP.nReservedA, tDP.nReservedB, tDP.nReservedC,
		tDP.nPInVulnOpID, tDP.nPInVulnScanID, tDP.nPHostNotifyID,
		tDP.nPInDevOOpID, tDP.nPInDevOBLID, tDP.nPInDevOWLID, tDP.nPInDevOExID,
		tDP.nPPmOpID, tDP.nPPmDmID, tDP.nPPmScanID, tDP.nPPmExID, tDP.nPPmNoID,
		tDP.nPInVulnAxID, tDP.nPInVulnEditAppID, tDP.nPInVulnQnaID, tDP.nPInVulnSecuUsbID, tDP.nPInVulnSwID, tDP.nPInVulnPatchExceptionID,
		tDP.nPInDevONotifyID,
		tDP.nPInRsOpID, tDP.nPInRsNoID, tDP.nPInRsBkID,
		tDP.nPNcPtnOpID, tDP.nPNcPtnLoID, tDP.nPNcPtnBLID, tDP.nPNcPtnWLID, tDP.nPNcPtnExID, tDP.nPNcPtnSPID, tDP.nPNcPtnNoID,
		tDP.nPHostWebSockID
		);

	return strRtn;
}
//---------------------------------------------------------------------------

String			CDBMgrBase::GetPoQuery_Update(DB_POLICY& tDP)
{
	String strRtn;
	strRtn = SPrintf(DBMS_POLICY_QUERY_UPDATE, 
		tDP.nPUpdateID, tDP.nPSvrInfoLgnID, tDP.nPHostRmInfoID, tDP.nPHostRunID, tDP.nPHostClearID, 
		tDP.nPPowerID, tDP.nPCtlPanelID, tDP.nPCtlProcID, tDP.nPSelfProtectID, tDP.nPDeployFileID,
		tDP.nPFaOpID, tDP.nPFaClearID, tDP.nPFaProcID, tDP.nPFaEnvID, tDP.nPFaBkID,
		tDP.nPWinRunID, tDP.nPSvrInfoUdtID, tDP.nPSvrInfoMgrID,
		tDP.nPFePtnOpID, tDP.nPFePtnBLID, tDP.nPFePtnWLID, tDP.nPFeExcludeID, tDP.nPFeNotifyID, tDP.nPFeSinglePtnID, tDP.nPFePtnLoID,
		tDP.nPFaNotifyID, tDP.nPSchID,
		tDP.nPInPtnOpID, tDP.nPInPtnBLID, tDP.nPInPtnWLID, tDP.nPInPtnExID, tDP.nPInPtnSPID, tDP.nPInPtnNoID,
		tDP.nReservedA, tDP.nReservedB, tDP.nReservedC,
		tDP.nPInVulnOpID, tDP.nPInVulnScanID, tDP.nPHostNotifyID, 
		tDP.nPInDevOOpID, tDP.nPInDevOBLID, tDP.nPInDevOWLID, tDP.nPInDevOExID,
		tDP.nPPmOpID, tDP.nPPmDmID, tDP.nPPmScanID, tDP.nPPmExID, tDP.nPPmNoID,
		tDP.nPInVulnAxID, tDP.nPInVulnEditAppID, tDP.nPInVulnQnaID, tDP.nPInVulnSecuUsbID, tDP.nPInVulnSwID, tDP.nPInVulnPatchExceptionID,
		tDP.nPInDevONotifyID,
		tDP.nPInRsOpID, tDP.nPInRsNoID, tDP.nPInRsBkID,
		tDP.nPNcPtnOpID, tDP.nPNcPtnLoID, tDP.nPNcPtnBLID, tDP.nPNcPtnWLID, tDP.nPNcPtnExID, tDP.nPNcPtnSPID, tDP.nPNcPtnNoID,
		tDP.nPHostWebSockID
		);

	return strRtn;
}
//------------------------------------------------------------------------------

String			CDBMgrBase::GetPoHDRQuery_InsertValue(DB_PO_HEADER& tDPH)
{
	String strRtn;
	strRtn = SPrintf(DBMS_POLICY_QUERY_HDR_INSERT_VALUE, 
#ifdef _DB_DEMAND_GEN_INDEX
					tDPH.nID, tDPH.nUsedFlag, tDPH.nRegDate, tDPH.nSeqNo, 
#else
					tDPH.nUsedFlag, tDPH.nRegDate, tDPH.nSeqNo, 
#endif
					tDPH.strName.c_str(), tDPH.strDescr.c_str(), 
					tDPH.nAdminID, tDPH.nSubAdminLock, tDPH.nTargetLock, tDPH.nTargetOS, tDPH.nExtOption, tDPH.nUsedMode);

	return strRtn;
}
//---------------------------------------------------------------------------
String			CDBMgrBase::GetPoHDRQuery_Update(DB_PO_HEADER& tDPH)
{
	String strRtn;
	strRtn = SPrintf(DBMS_POLICY_QUERY_HDR_UPDATE, 
					tDPH.nRegDate, tDPH.nSeqNo, 
					tDPH.strName.c_str(), tDPH.strDescr.c_str(), 
					tDPH.nAdminID, tDPH.nSubAdminLock, tDPH.nTargetLock, tDPH.nTargetOS, tDPH.nExtOption, tDPH.nUsedMode);

	return strRtn;
}
//------------------------------------------------------------------------------

String			CDBMgrBase::GetPoPkgHDRQuery_InsertValue(DB_PO_HEADER& tDPH)
{
	String strRtn;
	strRtn = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_INSERT_VALUE,
#ifdef _DB_DEMAND_GEN_INDEX
		tDPH.nID, tDPH.nUsedFlag, tDPH.nRegDate,
#else
		tDPH.nUsedFlag, tDPH.nRegDate,  
#endif
		tDPH.strName.c_str(), tDPH.strDescr.c_str(), 
		tDPH.nAdminID, tDPH.nSubAdminLock, tDPH.nPolicyID, tDPH.nUnitID, tDPH.nTargetOS, tDPH.nExtOption, tDPH.nUsedMode);

	return strRtn;
}
//------------------------------------------------------------------------------

String			CDBMgrBase::GetPoPkgHDRQuery_Update(DB_PO_HEADER& tDPH)
{
	String strRtn;
	strRtn = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_UPDATE, 
		tDPH.nUsedFlag, tDPH.nRegDate,
		tDPH.strName.c_str(), tDPH.strDescr.c_str(), 
		tDPH.nAdminID, tDPH.nSubAdminLock, tDPH.nPolicyID, tDPH.nUnitID, tDPH.nTargetOS, tDPH.nExtOption, tDPH.nUsedMode);

	return strRtn;
}
//------------------------------------------------------------------------------

INT32		CDBMgrBase::ResetTable()
{
	String strDeleteQuery = SPrintf(DBMS_DEFAULT_QUERY_DELETE_TABLE, m_strDBTName.c_str());
	if(DBOP_Check(ExecuteQuery(strDeleteQuery)))		return ERR_DBMS_DELETE_FAIL;

	String strResetQuery = SPrintf(DBMS_DEFAULT_QUERY_AUTO_INC_RESET, m_strDBTName.c_str());
	if(DBOP_Check(ExecuteQuery(strResetQuery)))		return ERR_DBMS_DELETE_FAIL;

	return 0;
}
//------------------------------------------------------------------------------


