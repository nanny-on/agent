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

#include "stdafx.h"
#include "as_db_query.h"
#include "as_db_class.h"

CASIDBUtil::CASIDBUtil(void)
{
	m_pStmt = NULL;
	t_SqliteQuery = new CASISQLiteQuery();
}

CASIDBUtil::~CASIDBUtil(void)
{
	if(t_SqliteQuery)
	{
		delete t_SqliteQuery;
		t_SqliteQuery = NULL;
	}
}

int CASIDBUtil::OpenDatabase(char *pcDBName, char *pcLogMsg)
{
	int rc = 0;
	int nRetVal = 0;
	do{
		if(pcDBName == NULL || pcDBName[0] == 0 || t_SqliteQuery == NULL)
		{
			if(pcLogMsg != NULL)
				snprintf(pcLogMsg, MAX_LOGMSG, "[OpenDatabase] invalid input data");
			nRetVal = -1;
			break;
		}

		rc = t_SqliteQuery->Open(pcDBName);
		if(rc != SQLITE_OK)
		{
			if(pcLogMsg != NULL)
				snprintf(pcLogMsg, MAX_LOGMSG, "[OpenDatabase] fail to open %s (%d)", pcDBName, rc);
			nRetVal = -2;
			break;
		}
		nRetVal = 0;
	}while(false);
	return nRetVal;
}


int CASIDBUtil::CreateDatabase(char *pcDBName, char *pcLogMsg)
{
	int rc = 0;
	int nRetVal = 0;

	do{
		if(pcDBName == NULL || pcDBName[0] == 0 || t_SqliteQuery == NULL)
		{
			if(pcLogMsg != NULL)
				snprintf(pcLogMsg, MAX_LOGMSG, "[CreateDatabase] invalid input data");
			nRetVal = -1;
			break;
		}
		rc = t_SqliteQuery->Open(pcDBName);
		if(rc != SQLITE_OK)
		{
			if(pcLogMsg != NULL)
				snprintf(pcLogMsg, MAX_LOGMSG, "[CreateDatabase] fail to open %s (%d)", pcDBName, rc);
			nRetVal = -2;
			break;
		}
		rc = t_SqliteQuery->Close();
		if(rc != SQLITE_OK)
		{
			if(pcLogMsg != NULL)
				snprintf(pcLogMsg, MAX_LOGMSG, "[CreateDatabase] fail to close database %d", rc);
			nRetVal = -3;
			break;
		}
		nRetVal = 0;
	}while(false);
	return nRetVal;
}

int CASIDBUtil::CloseDatabase(char *pcLogMsg)
{
	int rc = 0;
	if(t_SqliteQuery == NULL)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[CloseDatabase] invalid input data");
		return -1;
	}

	rc = t_SqliteQuery->Close();
	if(rc != SQLITE_OK)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[CloseDatabase] fail to close database %d", rc);
		return -2;
	}
	return 0;
}

int CASIDBUtil::ExecuteQuerySelect(char *pcQuery, char *pcLogMsg)
{
	int nRetVal = 0;
	PVOID pStmt = NULL;
	int rc = 0;

	if(pcQuery == NULL || pcQuery[0] == 0 || t_SqliteQuery == NULL)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[ExecuteQuery_Select] invalid input data");
		return -1;
	}
	if(m_pStmt != NULL)
	{
		t_SqliteQuery->Finalize(m_pStmt);
		m_pStmt = NULL;
	}
	rc = t_SqliteQuery->Prepare(pcQuery, &pStmt);
	if (rc != SQLITE_OK)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[ExecuteQuery_Select] fail to prefare (%d)", rc);
		return -2;
	}
	m_pStmt = pStmt;
	return nRetVal;
}

int CASIDBUtil::ExecuteQueryExe(char *pcQuery, char *pcLogMsg)
{
	int nRetVal = 0;
	PVOID pStmt = NULL;
	int rc = 0;

	if(pcQuery == NULL || pcQuery[0] == 0 || t_SqliteQuery == NULL)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[ExecuteQueryExe] invalid input data");
		return -1;
	}

	rc = t_SqliteQuery->Prepare(pcQuery, &pStmt);
	if (rc != SQLITE_OK)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[ExecuteQueryExe] fail to prefare (%d)", rc);
		return -2;
	}

	// begin
	nRetVal = Begin(pcLogMsg);
	if (nRetVal != 0)
	{
		nRetVal -= 100;
		return nRetVal;
	}

	rc = t_SqliteQuery->Step(pStmt);
	if(rc != SQLITE_DONE)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[ExecuteQueryExe] fail to step (%d)", rc);
		return -3;
	}

	// commit
	nRetVal = Commit();
	if (nRetVal != 0)
	{
		nRetVal -= 200;
		return nRetVal;
	}
	t_SqliteQuery->Finalize(pStmt);

	return nRetVal;
}

int	CASIDBUtil::ExecuteQuery(char *pcQuery, char *pcLogMsg)
{
	int nRetVal = 0;
	do{
		if(pcQuery == NULL || t_SqliteQuery == NULL)
		{
			if(pcLogMsg != NULL)
				snprintf(pcLogMsg, MAX_LOGMSG, "[ExecuteQuery] invalid input data");
			nRetVal = -1;
			break;
		}
		if(!_strnicmp(pcQuery, QUERY_SELECT, QUERY_SELECT_LEN))
		{
	    	nRetVal = ExecuteQuerySelect(pcQuery, pcLogMsg);
			if(nRetVal != 0)
			{
				nRetVal -= 100;
				break;
			}
		}
		else
		{
			nRetVal = ExecuteQueryExe(pcQuery, pcLogMsg);
			if(nRetVal != 0)
			{
				nRetVal -= 200;
				break;
			}
		}
		nRetVal = 0;
	}while(false);
	return nRetVal;
}

int	CASIDBUtil::IsTableExist(char *pcQuery, char *pcLogMsg)
{
	int nRetVal = 0;
	PVOID pStmt = NULL;
	int rc = 0;

	if(pcQuery == NULL || t_SqliteQuery == NULL)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[IsTableExist] invalid input data");
		return -1;
	}

	rc = t_SqliteQuery->Prepare(pcQuery, &pStmt);
	if (rc != SQLITE_OK)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[IsTableExist] fail to prefare (%d)", rc);
		return -2;
	}

	rc = t_SqliteQuery->Step(pStmt);
	if(rc == SQLITE_ROW)
	{
		nRetVal = 0;
	}
	else
	{
		nRetVal = -3;
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[IsTableExist] fail to step (%d)", rc);
	}
	return nRetVal;
}


bool CASIDBUtil::Next()
{
	bool bRetVal = true;
	int rc = 0;

	if(m_pStmt == NULL || t_SqliteQuery == NULL)
	{
		return false;
	}

	rc = t_SqliteQuery->Step(m_pStmt);
	if(rc == SQLITE_ROW)
	{
		bRetVal = true;
	}
	else
	{
		bRetVal = false;
		t_SqliteQuery->Finalize(m_pStmt);
		m_pStmt = NULL;
	}
	return bRetVal;
}

int	CASIDBUtil::Execute(char *pcQuery, char *pcLogMsg)
{
	int nRetVal = 0;
	PVOID pStmt = NULL;
	int rc = 0;

	if(pcQuery == NULL || pcQuery[0] == 0 || t_SqliteQuery == NULL)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[Execute] invalid input data");
		return -1;
	}

	rc = t_SqliteQuery->Prepare(pcQuery, &pStmt);
	if (rc != SQLITE_OK)
	{
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[Execute] fail to prefare (%d)", rc);
		return -2;
	}

	rc = t_SqliteQuery->Step(pStmt);
	if(rc == SQLITE_DONE)
	{
		nRetVal = 0;
	}
	else
	{
		nRetVal = -3;
		if(pcLogMsg != NULL)
			snprintf(pcLogMsg, MAX_LOGMSG, "[Execute] fail to step (%d)", rc);
	}

	t_SqliteQuery->Finalize(pStmt);
	return nRetVal;
}


int CASIDBUtil::GetDBFieldCount()
{
	if(m_pStmt == NULL || t_SqliteQuery == NULL)
	{
		return 0;
	}

	return t_SqliteQuery->ColumnCount(m_pStmt);
}

int CASIDBUtil::GetDBFieldType(int nIndex)
{
	int nType = COLUMN_TYPE_NULL;
	int rc = 0;
	if(m_pStmt == NULL || t_SqliteQuery == NULL)
	{
		return COLUMN_TYPE_NULL;
	}
	rc = t_SqliteQuery->ColumnType(m_pStmt, nIndex);
	switch (rc)
	{
	case (SQLITE_INTEGER):
		nType = COLUMN_TYPE_INT;
		break;
	case (SQLITE3_TEXT):
		nType = COLUMN_TYPE_TEXT;
		break;
	case (SQLITE_FLOAT):
		nType = COLUMN_TYPE_FLOAT;
		break;
	case (SQLITE_BLOB):
		nType = COLUMN_TYPE_BLOB;
		break;
	case (SQLITE_NULL):
	default:
		nType = COLUMN_TYPE_NULL;
		break;
	}
	return nType;
}

int CASIDBUtil::GetDBFieldInt(int nIndex)
{
	if(m_pStmt == NULL || t_SqliteQuery == NULL)
	{
		return 0;
	}

	return t_SqliteQuery->ColumnInt(m_pStmt, nIndex);
}

char *CASIDBUtil::GetDBFieldString(int nIndex)
{
	if(m_pStmt == NULL || t_SqliteQuery == NULL)
	{
		return (char *)"";
	}

	return t_SqliteQuery->ColumnText(m_pStmt, nIndex);
}

float CASIDBUtil::GetDBFieldFloat(int nIndex)
{
	if(m_pStmt == NULL || t_SqliteQuery == NULL)
	{
		return (float)0.0;
	}

	return t_SqliteQuery->ColumnFloat(m_pStmt, nIndex);
}


int CASIDBUtil::GetDBFieldBytes(int nIndex)
{
	if(m_pStmt == NULL || t_SqliteQuery == NULL)
	{
		return 0;
	}

	return t_SqliteQuery->ColumnBytes(m_pStmt, nIndex);
}

char *CASIDBUtil::GetDBFieldBlob(int nIndex)
{
	if(m_pStmt == NULL || t_SqliteQuery == NULL)
	{
		return (char *)"";
	}

	return t_SqliteQuery->ColumnBlob(m_pStmt, nIndex);
}

int CASIDBUtil::Begin(char *pcLogMsg)
{
	char acQuery[SQL_QUERY_LENGTH] = "BEGIN;";
	return Execute(acQuery, pcLogMsg);
}

int CASIDBUtil::Commit(char *pcLogMsg)
{
	char acQuery[SQL_QUERY_LENGTH] = "COMMIT;";
	return Execute(acQuery, pcLogMsg);
}

int CASIDBUtil::Rollback(char *pcLogMsg)
{
	char acQuery[SQL_QUERY_LENGTH] = "ROLLBACK;";
	return Execute(acQuery, pcLogMsg);
}

