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

#include "stdafx.h"
#include "as_file.h"
#include "as_odbc_util.h"

void GetErrorMessage(SQLSMALLINT hType, SQLHANDLE hSql, SQLRETURN cliRC, char *acLogMsg)
{
	SQLCHAR acMessage[SQL_MAX_MESSAGE_LENGTH+1] = {0,};
	SQLCHAR acSqlState[SQL_SQLSTATE_SIZE+1] = {0,};
	SQLINTEGER nSqlCode = 0;
	SQLSMALLINT nLength = 0;
	SQLSMALLINT i = 1;
	if(hSql == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid handle input data");
		return;
	}
	if(cliRC == SQL_INVALID_HANDLE)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "sql invalid handle");
	}
	else if(cliRC == SQL_ERROR)
	{
		while (SQLGetDiagRec(hType, hSql, i, acSqlState, &nSqlCode, acMessage, SQL_MAX_MESSAGE_LENGTH, &nLength) == SQL_SUCCESS)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "sql error, state = %s, error = %d, %s", acSqlState, nSqlCode, acMessage);
			break;
		}
	}
	else
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "sql else error (%d)", cliRC);
	}
}

int CheckReturnValue(SQLRETURN cliRC)
{
	int rc = 0;

	switch (cliRC)
	{
	case SQL_SUCCESS:
	case SQL_SUCCESS_WITH_INFO:
	case SQL_NO_DATA_FOUND:
	case SQL_STILL_EXECUTING:
	case SQL_NEED_DATA:
		rc = 0;
		break;
	case SQL_INVALID_HANDLE:
		rc = -1;
		break;
	case SQL_ERROR:
		rc = -2;
		break;
	default:
		rc = -3;
		break;
	}

	return rc;
}

void StmtResourcesFree(SQLHANDLE hStmt)
{
	SQLRETURN cliRC = SQL_SUCCESS;
	int rc = 0;
	do{
		if(hStmt == NULL)
		{
			break;
		}
		/* free the statement handle */
		cliRC = SQLFreeStmt(hStmt, SQL_UNBIND);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			break;
		}

		/* free the statement handle */
		cliRC = SQLFreeStmt(hStmt, SQL_RESET_PARAMS);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			break;
		}

		/* free the statement handle */
		cliRC = SQLFreeStmt(hStmt, SQL_CLOSE);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			break;
		}
	}while(FALSE);
} /* StmtResourcesFree */

/* rollback transactions on a single connection */
void TransRollback(SQLHANDLE hDbc)
{
	if(hDbc != NULL)
		SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_ROLLBACK);
}

/* rollback transactions on mutiple connections */
void MultiConnTransRollback(SQLHANDLE hEnv)
{
	if(hEnv != NULL)
		SQLEndTran(SQL_HANDLE_ENV, hEnv, SQL_ROLLBACK);
}



/* initialize a CLI application by:
 * o * allocating an environment handle
 * o * allocating a connection handle
 * o * setting AUTOCOMMIT
 * o * connecting to the database
*/
int CLIAppInitLocal(char *acDbName, char *acUser, char *acPasswd, SQLHANDLE *ppEnv, SQLHANDLE *ppDbc, SQLPOINTER Value, char *acLogMsg)
{
	SQLRETURN cliRC = SQL_SUCCESS;
	int rc = 0;
	SQLHANDLE pEnv = NULL;
	SQLHANDLE pDbc = NULL;

	if(acDbName == NULL || acUser == NULL || ppEnv == NULL || ppDbc == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -1;
	}

	do{
		cliRC = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &pEnv);
		rc = CheckReturnValue(cliRC);
		if (rc != 0 || pEnv == NULL)
		{
			if(acLogMsg != NULL)
				snprintf(acLogMsg, MAX_LOGMSG, "fail to allocate handle (%d)", cliRC);
			rc = -2;
			break;
		}

		cliRC = SQLSetEnvAttr(pEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			ENV_HANDLE_CHECK(pEnv, cliRC, acLogMsg);
			rc = -3;
			break;
		}

		cliRC = SQLAllocHandle(SQL_HANDLE_DBC, pEnv, &pDbc);
		rc = CheckReturnValue(cliRC);
		if (rc != 0 || pDbc == NULL)
		{
			ENV_HANDLE_CHECK(pEnv, cliRC, acLogMsg);
			SQLFreeHandle(SQL_HANDLE_ENV, pEnv);
			rc = -4;
			break;
		}
		/* set AUTOCOMMIT off or on */
		cliRC = SQLSetConnectAttr(pDbc, SQL_ATTR_AUTOCOMMIT, Value, SQL_NTS);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			DBC_HANDLE_CHECK(pDbc, cliRC, acLogMsg);
			SQLFreeHandle(SQL_HANDLE_DBC, pDbc);
			SQLFreeHandle(SQL_HANDLE_ENV, pEnv);
			rc = -5;
			break;
		}
		/* connect to the database */
		if(acPasswd == NULL)
			cliRC = SQLConnect(pDbc, (SQLCHAR *)acDbName, SQL_NTS, (SQLCHAR *)acUser, SQL_NTS, (SQLCHAR *)"", SQL_NTS);
		else
			cliRC = SQLConnect(pDbc, (SQLCHAR *)acDbName, SQL_NTS, (SQLCHAR *)acUser, SQL_NTS, (SQLCHAR *)acPasswd, SQL_NTS);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			DBC_HANDLE_CHECK(pDbc, cliRC, acLogMsg);
			SQLFreeHandle(SQL_HANDLE_DBC, pDbc);
			SQLFreeHandle(SQL_HANDLE_ENV, pEnv);
			rc = -6;
			break;
		}
		*ppEnv = pEnv;
		*ppDbc = pDbc;
		rc = 0;
	}while(FALSE);
	return rc;
} /* CLIAppInit */

/* terminate a CLI application by:
 * o * disconnecting from the database
 * o * freeing the connection handle
 * o * freeing the environment handle */

void CLIAppTerm(SQLHANDLE *ppEnv, SQLHANDLE *ppDbc)
{
	/* disconnect from the database */
	if(ppDbc != NULL && *ppDbc != NULL)
	{
		SQLDisconnect(*ppDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, *ppDbc);
		*ppDbc = NULL;
	}
	/* free environment handle */
	if(ppEnv != NULL && *ppEnv != NULL)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, *ppEnv);
		*ppEnv = NULL;
	}
}

void FreeStmtHandle(SQLHANDLE& hStmt)
{
	if(hStmt != NULL)
	{
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		hStmt = NULL;
	}
}

int StmtExecDirectFromFile(SQLHANDLE hDbc, const char *pFilePath, char *acLogMsg)
{
	int rc = 0;
	SQLRETURN cliRC = SQL_SUCCESS;
	SQLHANDLE hStmt = NULL;
	SQLCHAR *pStmt1 = NULL;
	char *pcSqlCmd = NULL;
	FILE* fpDb = NULL;
	int nSqlMax = 40960;
	char *pcLine = NULL;
	int nLineMax = 4096;
	int nLength = 0;
	int nTotalLength = 0;
	int nCount = 0;

	if(hDbc == NULL || pFilePath == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -1;
	}

	fpDb = fopen(pFilePath, "r");
	if(fpDb == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to open %s (%d)", pFilePath, errno);
		return -2;
	}
	pcSqlCmd = (char *)malloc(nSqlMax);
	if(pcSqlCmd == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to allocate sql memory %d (%d)", nSqlMax, errno);
		fclose(fpDb);
		return -3;
	}
	memset(pcSqlCmd, 0, nSqlMax);
	
	pcLine = (char *)malloc(nLineMax);
	if(pcLine == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "fail to allocate line memory %d (%d)", nLineMax, errno);
		fclose(fpDb);
		safe_free(pcSqlCmd);
		return -4;
	}
	memset(pcLine, 0, nLineMax);

	/* allocate a statement handle */
	cliRC = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	rc = CheckReturnValue(cliRC);
	if (rc != 0 || hStmt == NULL)
	{
		DBC_HANDLE_CHECK(hDbc, cliRC, acLogMsg);
		fclose(fpDb);
		safe_free(pcSqlCmd);
		safe_free(pcLine);
		return -5;
	}

	rc = 0;
	while ( fgets_query_file(pcLine, nLineMax-1, fpDb) )
	{
		nLength = strlen(pcLine);
		if(nLength < 2)
			continue;
		if(pcLine[nLength-2] == ';' && pcLine[nLength-1] == 12)
		{
			pcLine[nLength-1] = 0;
			nTotalLength = strlen(pcSqlCmd) + nLength -1;
			if(nTotalLength > nSqlMax-2)
			{
				if(acLogMsg != NULL)
					snprintf(acLogMsg, MAX_LOGMSG, "invalid total size (%d)", nTotalLength);
				rc = -6;
				break;
			}
			strncat(pcSqlCmd, pcLine, nLength-1);
			pStmt1 = (SQLCHAR*)pcSqlCmd;
			nCount++;

			cliRC = SQLExecDirect(hStmt, pStmt1, SQL_NTS);
			rc = CheckReturnValue(cliRC);
			if (rc != 0)
			{
				STMT_HANDLE_CHECK(hStmt, hDbc, cliRC, acLogMsg);
				rc = -7;
				break;
			}

			if(!_strnicmp(pcSqlCmd, "UPDATE schema_info SET db_ver", 29))
			{
				cliRC = SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_COMMIT);
				rc = CheckReturnValue(cliRC);
				if (rc != 0)
				{
					DBC_HANDLE_CHECK(hDbc, cliRC, acLogMsg);
					rc = -8;
					break;
				}
			}
			memset(pcSqlCmd, 0, nSqlMax);
			memset(pcLine, 0, nLineMax);
		}
		else
		{
			nTotalLength = strlen(pcSqlCmd) + nLength;
			if(nTotalLength > nSqlMax-2)
			{
				if(acLogMsg != NULL)
					snprintf(acLogMsg, MAX_LOGMSG, "invalid total size (%d)", nTotalLength);
				rc = -9;
				break;
			}
			strncat(pcSqlCmd, pcLine, nLength);
			strncat(pcSqlCmd, " ", 1);
		}
		rc = 0;
	}
	fclose(fpDb);
	safe_free(pcSqlCmd);
	safe_free(pcLine);
	if(rc == 0)
	{
		cliRC = SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_COMMIT);
		rc = CheckReturnValue(cliRC);
		if (rc != 0)
		{
			DBC_HANDLE_CHECK(hDbc, cliRC, acLogMsg);
			rc = -10;
		}
	}
	/* free the statement handle */
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return rc;
}


