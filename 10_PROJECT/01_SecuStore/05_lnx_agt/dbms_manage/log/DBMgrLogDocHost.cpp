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

//---------------------------------------------------------------------------
#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrLogDocHost.h"

//---------------------------------------------------------------------------

CDBMgrLogDocHost*		t_DBMgrLogDocHost;

//---------------------------------------------------------------------------

CDBMgrLogDocHost::CDBMgrLogDocHost()
{
	m_strDBTName = "log_doc_host";
}
//---------------------------------------------------------------------------

CDBMgrLogDocHost::~CDBMgrLogDocHost()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDocHost::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDocHost::LoadDB(TListDBLogDocHost& tDBLogDocHostList)
{
    UINT32 nReadCnt = 0;
    DB_LOG_DOC_HOST data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, log_id, evt_time "
						"FROM log_doc_host WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;

		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedFlag				= GetDBField_Int(nIndex++);
		data.nLogID					= GetDBField_Int(nIndex++);
		data.nEvtTime				= GetDBField_Int(nIndex++);

        tDBLogDocHostList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))
			m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBLogDocHostList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDocHost::InsertLogDocHost(DB_LOG_DOC_HOST& data)
{
	m_strQuery = SPrintf("INSERT INTO log_doc_host(used_flag, log_id, evt_time)"
    								"VALUES"
									"(%u, %u, %u);",
									data.nUsedFlag, data.nLogID, data.nEvtTime);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDocHost::UpdateLogDocHost(DB_LOG_DOC_HOST& data)
{	
	m_strQuery = SPrintf("UPDATE log_doc_host SET log_id=%u, evt_time=%u "
						"WHERE id=%u;",
						data.nLogID, data.nEvtTime,						
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDocHost::DeleteLogDocHost(UINT32 nID)
{
    m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------   
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CDBMgrLogDocHost::LoadExecute(PVOID lpTempletList)
{
	TListDBLogDocHost* ptDBList = (TListDBLogDocHost*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDocHost::InsertExecute(PVOID lpTemplet)
{
	PDB_LOG_DOC_HOST pd_t = (PDB_LOG_DOC_HOST)lpTemplet;

    return InsertLogDocHost(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDocHost::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOG_DOC_HOST pd_t = (PDB_LOG_DOC_HOST)lpTemplet;

    return UpdateLogDocHost(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDocHost::DeleteExecute(UINT32 nID)
{
	return DeleteLogDocHost(nID);
}
//---------------------------------------------------------------------------






