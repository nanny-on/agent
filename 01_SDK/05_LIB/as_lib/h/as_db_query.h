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

#ifndef _AS_SQLITE_QUERY_H__
#define _AS_SQLITE_QUERY_H__

#include "sqlite3.h"

typedef void *PVOID;	

class CASISQLiteQuery
{
private:
	sqlite3 *m_pSqlite;
public:
	int Open(char *pcDBName);
	int Close();
	int Prepare(char *pInQuery, PVOID* ppOutStmt);
	int Step(PVOID pInStmt);
	int ColumnCount(PVOID pInStmt);
	int ColumnType(PVOID pInStmt, int nIndex);
	char *ColumnText(PVOID pInStmt, int nIndex);
	int ColumnInt(PVOID pInStmt, int nIndex);
	float ColumnFloat(PVOID pInStmt, int nIndex);
	int ColumnBytes(PVOID pInStmt, int nIndex);
	char *ColumnBlob(PVOID pInStmt, int nIndex);
	void Finalize(PVOID pInStmt);
public:
	CASISQLiteQuery();
	~CASISQLiteQuery();
};

extern CASISQLiteQuery* t_SqliteQuery;


#endif /*_AS_SQLITE_QUERY_H__*/

