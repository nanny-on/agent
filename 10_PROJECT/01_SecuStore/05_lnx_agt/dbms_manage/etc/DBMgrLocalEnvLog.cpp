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
#include "DBMgrLocalEnvLog.h"

//---------------------------------------------------------------------------

CDBMgrLocalEnvLog*		t_DBMgrLocalEnvLog;

//---------------------------------------------------------------------------

CDBMgrLocalEnvLog::CDBMgrLocalEnvLog()
{
	m_strDBTName = "local_env_log";
}
//---------------------------------------------------------------------------

CDBMgrLocalEnvLog::~CDBMgrLocalEnvLog()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLocalEnvLog::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLocalEnvLog::LoadDB(TListDBLocalEnvLog& tDBLocalEnvLogList)
{
    UINT32 nReadCnt = 0;
    DB_LOCAL_ENV_LOG dlel;

    INT32 nIndex = 0;

    m_strQuery = SPrintf("SELECT id, used_flag"
						", log_doc_id"
						" FROM local_env_log WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;

		dlel.nID			= GetDBField_UInt(nIndex++);
		dlel.nUsedFlag		= GetDBField_UInt(nIndex++);
		dlel.nLogDocID		= GetDBField_UInt(nIndex++);

        tDBLocalEnvLogList.push_back(dlel);
        if(m_nLoadMaxID < UINT32(dlel.nID))	m_nLoadMaxID = dlel.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBLocalEnvLogList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLocalEnvLog::InsertLocalEnvLog(DB_LOCAL_ENV_LOG& dlel)
{
	m_strQuery = SPrintf("INSERT INTO local_env_log("
						"used_flag, log_doc_id "
						") VALUES ("
						" %u, %u "
						");",                        
						dlel.nUsedFlag, dlel.nLogDocID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(dlel.nID == 0)
		dlel.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLocalEnvLog::UpdateLocalEnvLog(DB_LOCAL_ENV_LOG& dlel)
{
	m_strQuery = SPrintf("UPDATE local_env_log SET "
						"used_flag=%u, log_doc_id=%u "
						" WHERE id=%u;",
						dlel.nUsedFlag, dlel.nLogDocID, 
						dlel.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLocalEnvLog::DeleteLocalEnvLog(UINT32 nID)
{
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

INT32	CDBMgrLocalEnvLog::LoadExecute(PVOID lpTempletList)
{
	TListDBLocalEnvLog* ptDBList = (TListDBLocalEnvLog*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLocalEnvLog::InsertExecute(PVOID lpTemplet)
{
	PDB_LOCAL_ENV_LOG pd_t = (PDB_LOCAL_ENV_LOG)lpTemplet;

    return InsertLocalEnvLog(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLocalEnvLog::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOCAL_ENV_LOG pd_t = (PDB_LOCAL_ENV_LOG)lpTemplet;

    return UpdateLocalEnvLog(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLocalEnvLog::DeleteExecute(UINT32 nID)
{
	return DeleteLocalEnvLog(nID);
}
//---------------------------------------------------------------------------






