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

#ifndef _THREAD_DBMS_QUERY__
#define _THREAD_DBMS_QUERY__

#include "DBMgrBase.h"
#include "MutexExt.h"

class CThreadDBMSQuery : public CThreadBase
{
private:
    INT32		m_nRunThread;

private:
	CDBMgrBase		m_tDBMgrBase;
    TListListStr	m_tStrListList;
    CMutexExt		m_tQueryMutex;

public:
	void		AddQueryList(TListStr& tQueryList);
    INT32		GetQueryList(TListStr& tQueryList);

public:
    INT32       IsRunThread();

private:
    void       	Logic();

private:
	INT32		IsConnectDBMS();
	INT32		ConnectDBMS();
    INT32		ExecuteQuery_Exe(String strQuery);


public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();

public:
	CThreadDBMSQuery();
	virtual ~CThreadDBMSQuery();
};
//---------------------------------------------------------------------------

extern CThreadDBMSQuery* t_ThreadDBMSQuery;

#endif
