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
#include "DBMgrHostPolicy.h"

//---------------------------------------------------------------------------

CDBMgrHostPolicy*		t_DBMgrHostPolicy = NULL;

//---------------------------------------------------------------------------

CDBMgrHostPolicy::CDBMgrHostPolicy()
{
	m_strDBTName = "host_policy";
}
//---------------------------------------------------------------------------

CDBMgrHostPolicy::~CDBMgrHostPolicy()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostPolicy::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostPolicy::LoadDB(TListDBHostPolicy& tDBHostPolicyList)
{
    UINT32 nReadCnt = 0;
    DB_HOST_POLICY dhp;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, host_id, "
						DBMS_POLICY_QUERY_SELECT
						" FROM host_policy WHERE used_flag = 1;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;

        dhp.nID						= GetDBField_UInt(nIndex++);
		dhp.nRegDate				= GetDBField_UInt(nIndex++);
		dhp.nHostID					= GetDBField_UInt(nIndex++);

		dhp.tDP						= GetDBField_Po(nIndex);

		tDBHostPolicyList.push_back(dhp);
        if(m_nLoadMaxID < UINT32(dhp.nID))
			m_nLoadMaxID = dhp.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBHostPolicyList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostPolicy::InsertHostPolicy(DB_HOST_POLICY& dhp)
{
	m_strQuery = SPrintf("INSERT INTO host_policy (used_flag, reg_date, host_id, "
								DBMS_POLICY_QUERY_INSERT_NAME
								") VALUES (%u, %u, %u, %s)",
								dhp.nUsedFlag, dhp.nRegDate, dhp.nHostID,
								GetPoQuery_InsertValue(dhp.tDP).c_str());
		
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
	if(dhp.nID == 0)
		dhp.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostPolicy::UpdateHostPolicy(DB_HOST_POLICY& dhp)
{
	m_strQuery = SPrintf("UPDATE host_policy SET "
						"%s"
						" WHERE id=%u;", 
						GetPoQuery_Update(dhp.tDP).c_str(),
						dhp.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostPolicy::DeleteHostPolicy(UINT32 nID)
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

INT32	CDBMgrHostPolicy::LoadExecute(PVOID lpTempletList)
{
	TListDBHostPolicy* ptDBList = (TListDBHostPolicy*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostPolicy::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST_POLICY pd_t = (PDB_HOST_POLICY)lpTemplet;

    return InsertHostPolicy(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostPolicy::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST_POLICY pd_t = (PDB_HOST_POLICY)lpTemplet;

    return UpdateHostPolicy(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostPolicy::DeleteExecute(UINT32 nID)
{
	return DeleteHostPolicy(nID);
}
//---------------------------------------------------------------------------






