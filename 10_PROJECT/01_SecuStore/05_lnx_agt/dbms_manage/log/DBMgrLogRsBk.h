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
//---------------------------------------------------------------------------

#ifndef DBMgrLogRsBkH
#define DBMgrLogRsBkH
//---------------------------------------------------------------------------

class CDBMgrLogRsBk : public CDBMgrBase
{
public:
	INT32			SetInitalize();

public:
	INT32			LoadDB(TListDBLogRsBk& tDBLogRsBkList);

public:
    INT32			InsertLogRsBk(DB_LOG_RS_BK& data);
    INT32			UpdateLogRsBk(DB_LOG_RS_BK& data);
    INT32			DeleteLogRsBk(UINT32 nID);    

public:
	virtual INT32	LoadExecute(PVOID lpTempletList);     
	virtual INT32	InsertExecute(PVOID lpTemplet);
	virtual INT32	UpdateExecute(PVOID lpTemplet);
	virtual INT32	DeleteExecute(UINT32 nID);

public:
	INT32			LoadDB(UINT32 nLogMode, UINT32 nLogNum, TListDBLogRsBk& tDBLogRsBkList);


public:
	CDBMgrLogRsBk();
    ~CDBMgrLogRsBk();
};

extern CDBMgrLogRsBk*		t_DBMgrLogRsBk;

#endif
