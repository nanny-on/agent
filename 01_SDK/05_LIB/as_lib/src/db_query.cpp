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

#include "stdafx.h"
#include "as_db_query.h"


CASISQLiteQuery* t_SqliteQuery;


CASISQLiteQuery::CASISQLiteQuery(void)
{
	m_pSqlite = NULL;
}

CASISQLiteQuery::~CASISQLiteQuery(void)
{
	Close();
}


int CASISQLiteQuery::Open(char *pcDBName)
{
	int i;
	int rc = 0;
	if(pcDBName == NULL)
	{
		return SQLITE_ERROR;
	}

	for(i=0; i<10; i++)
	{
		rc = sqlite3_open_v2(pcDBName, &m_pSqlite, SQLITE_OPEN_READONLY, NULL);
		if(rc == SQLITE_BUSY)
		{
			usleep(100000);
		}
		else
		{
			return rc;
		}
	}
	return SQLITE_BUSY;
}

int CASISQLiteQuery::Close()
{
	int i = 0;
	int rc = 0;
	if(m_pSqlite == NULL)
	{
		return SQLITE_OK;
	}
	for(i=0; i<10; i++)
	{
		rc = sqlite3_close(m_pSqlite);
		if(rc == SQLITE_BUSY)
		{
			usleep(100000);
		}
		else
		{
			if(rc == SQLITE_OK)
			{
				m_pSqlite = NULL;
			}
			return rc;
		}
	}
	return SQLITE_BUSY;
}


int CASISQLiteQuery::Prepare(char *pInQuery, PVOID* ppOutStmt)
{
	sqlite3_stmt *pStmt;
	int rc = 0;
	int i = 0;
	if(pInQuery == NULL || ppOutStmt == NULL || m_pSqlite == NULL)
	{
		return SQLITE_ERROR;
	}

	for(i=0; i<10; i++)
	{
		rc = sqlite3_prepare_v2(m_pSqlite, pInQuery, -1, &pStmt, NULL);
		if(rc == SQLITE_BUSY)
		{
			usleep(100000);
		}
		else
		{
			if(rc == SQLITE_OK)
			{
				*ppOutStmt = (PVOID)pStmt;
			}
			return rc;
		}
	}
	return SQLITE_BUSY;
}



int CASISQLiteQuery::Step(PVOID pInStmt)
{
	sqlite3_stmt *pStmt;
	int rc = 0;
	int i = 0;
	if(pInStmt == NULL)
	{
		return SQLITE_ERROR;
	}
	pStmt = (sqlite3_stmt *)pInStmt;
	for(i=0; i<10; i++)
	{
		rc = sqlite3_step(pStmt);
		if(rc == SQLITE_BUSY)
		{
			usleep(100000);
		}
		else
		{
			return rc;
		}
	}
	return SQLITE_BUSY;
}


int CASISQLiteQuery::ColumnCount(PVOID pInStmt)
{
	sqlite3_stmt *pStmt;
	if(pInStmt == NULL)
	{
		return 0;
	}
	pStmt = (sqlite3_stmt *)pInStmt;

	return sqlite3_column_count(pStmt);
}


int CASISQLiteQuery::ColumnType(PVOID pInStmt, int nIndex)
{
	sqlite3_stmt *pStmt;
	if(pInStmt == NULL || nIndex < 0)
	{
		return SQLITE_NULL;
	}
	pStmt = (sqlite3_stmt *)pInStmt;

	return sqlite3_column_type(pStmt, nIndex);
}

char *CASISQLiteQuery::ColumnText(PVOID pInStmt, int nIndex)
{
	sqlite3_stmt *pStmt;
	if(pInStmt == NULL || nIndex < 0)
	{
		return (char *)"";
	}
	pStmt = (sqlite3_stmt *)pInStmt;

	return (char *)sqlite3_column_text(pStmt, nIndex);
}

int CASISQLiteQuery::ColumnInt(PVOID pInStmt, int nIndex)
{
	sqlite3_stmt *pStmt;
	if(pInStmt == NULL || nIndex < 0)
	{
		return 0;
	}
	pStmt = (sqlite3_stmt *)pInStmt;

	return sqlite3_column_int(pStmt, nIndex);
}

float CASISQLiteQuery::ColumnFloat(PVOID pInStmt, int nIndex)
{
	sqlite3_stmt *pStmt;
	if(pInStmt == NULL || nIndex < 0)
	{
		return 0.0;
	}
	pStmt = (sqlite3_stmt *)pInStmt;

	return sqlite3_column_double(pStmt, nIndex);
}

char *CASISQLiteQuery::ColumnBlob(PVOID pInStmt, int nIndex)
{
	sqlite3_stmt *pStmt;
	if(pInStmt == NULL || nIndex < 0)
	{
		return (char *)"";
	}
	pStmt = (sqlite3_stmt *)pInStmt;

	return (char*)sqlite3_column_blob(pStmt, nIndex);
}

int CASISQLiteQuery::ColumnBytes(PVOID pInStmt, int nIndex)
{
	sqlite3_stmt *pStmt;
	if(pInStmt == NULL || nIndex < 0)
	{
		return 0;
	}
	pStmt = (sqlite3_stmt *)pInStmt;

	return sqlite3_column_bytes(pStmt, nIndex);
}

void CASISQLiteQuery::Finalize(PVOID pInStmt)
{
	sqlite3_stmt *pStmt;
	if(pInStmt != NULL)
	{
		pStmt = (sqlite3_stmt *)pInStmt;
		sqlite3_reset(pStmt);
		sqlite3_finalize(pStmt);
	}
}
