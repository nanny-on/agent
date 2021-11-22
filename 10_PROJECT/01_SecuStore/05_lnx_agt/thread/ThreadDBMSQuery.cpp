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
#include "com_struct.h"
#include "ThreadDBMSQuery.h"


CThreadDBMSQuery* t_ThreadDBMSQuery = NULL;

CThreadDBMSQuery::CThreadDBMSQuery()
{
}

CThreadDBMSQuery::~CThreadDBMSQuery()
{      
}


BOOL CThreadDBMSQuery::InitInstance()
{
	return TRUE;
}

int CThreadDBMSQuery::ExitInstance()
{
	return CThreadBase::ExitInstance();
}


//--------------------------------------------------------------------------- 

INT32       CThreadDBMSQuery::IsRunThread()
{
    return m_nRunThread;
}
//---------------------------------------------------------------------------

void		CThreadDBMSQuery::AddQueryList(TListStr& tQueryList)
{
	m_tQueryMutex.Lock();
    m_tStrListList.push_back(tQueryList);
    m_tQueryMutex.UnLock();
}
//---------------------------------------------------------------------------

INT32		CThreadDBMSQuery::GetQueryList(TListStr& tQueryList)
{
	INT32 nSize = m_tStrListList.size();
	if(nSize == 0)
		return nSize;

    m_tQueryMutex.Lock();
    tQueryList = m_tStrListList.front();
    m_tStrListList.pop_front();

    m_tQueryMutex.UnLock();
    return nSize;
}

int CThreadDBMSQuery::Run() 
{
	m_nRunFlag = 1;
	WriteLogN("start %s thread : [%d]", m_strThreadName.c_str(), m_nRunFlag);

	INT32 nRet = 0;
	while(t_EnvInfoOp && t_EnvInfoOp->GetMainContinue() && GetContinue())
	{
		if(!m_nPause)
		{	
			Logic();
		}
		Sleep(10);
	}

	if(!t_EnvInfoOp)							WriteLogE("environment operation util is null : [%s]", m_strThreadName.c_str());
	else if(!t_EnvInfoOp->GetMainContinue())	WriteLogN("stop thread by main continue flag : [%s]", m_strThreadName.c_str());
	else if(!GetContinue())						WriteLogN("stop thread by sub continue flag : [%s]", m_strThreadName.c_str());

	return 0;
}
//---------------------------------------------------------------------------

void	CThreadDBMSQuery::Logic()
{
	INT32       nTotalPktOp;
    INT32		nTotalOpCnt = 0;
	String		strNewQuery;
	String		strQuery;
    TListStr	tQueryList;
	TListStrItor begin, end;

    if(IsConnectDBMS() && ConnectDBMS())
    {
        return;
    }

	if((nTotalPktOp = GetQueryList(tQueryList)) != 0)
    {
        begin = tQueryList.begin();	end = tQueryList.end();
        for(begin; begin != end; begin++)
        {
			strQuery = SPrintf("%s;", begin->c_str());
			strNewQuery += strQuery;
        }
        ExecuteQuery_Exe(strNewQuery);
		tQueryList.clear();
    }
}
//---------------------------------------------------------------------------

INT32		CThreadDBMSQuery::IsConnectDBMS()
{
    return m_tDBMgrBase.IsConnected();
}
//---------------------------------------------------------------------------

INT32		CThreadDBMSQuery::ConnectDBMS()
{
	INT32 nRtn = 0;
    if((nRtn = m_tDBMgrBase.ConnectDB()) != 0)
    {
    	WriteLogE("connect dbms fail on Express for dbms query : [%d]", nRtn);
    }
    return nRtn;
}
//---------------------------------------------------------------------------

INT32		CThreadDBMSQuery::ExecuteQuery_Exe(String strQuery)
{
  	INT32 nRtn = 0;
    if((nRtn = m_tDBMgrBase.ExecuteQuery(strQuery)) != 0)
    {
    	WriteLogE("execute query fail on Express for dbms query : [%d]", nRtn);
    }
    return nRtn;
}
//---------------------------------------------------------------------------



