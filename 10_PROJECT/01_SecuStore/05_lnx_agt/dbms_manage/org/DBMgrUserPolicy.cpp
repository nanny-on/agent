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
#include "DBMgrUserPolicy.h"

//---------------------------------------------------------------------------

CDBMgrUserPolicy*		t_DBMgrUserPolicy = NULL;

//---------------------------------------------------------------------------

CDBMgrUserPolicy::CDBMgrUserPolicy()
{
	m_strDBTName = "user_policy";
}
//---------------------------------------------------------------------------

CDBMgrUserPolicy::~CDBMgrUserPolicy()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrUserPolicy::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrUserPolicy::LoadDB(TListDBUserPolicy& tDBUserPolicyList)
{
    UINT32 nReadCnt = 0;
    DB_USER_POLICY dup;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, user_id, "
						DBMS_POLICY_QUERY_SELECT
						" FROM user_policy WHERE used_flag = 1;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;

        dup.nID						= GetDBField_UInt(nIndex++);
		dup.nRegDate				= GetDBField_UInt(nIndex++);
		dup.nUserID					= GetDBField_UInt(nIndex++);

		dup.tDP						= GetDBField_Po(nIndex);
		

		tDBUserPolicyList.push_back(dup);
        if(m_nLoadMaxID < UINT32(dup.nID))	m_nLoadMaxID = dup.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBUserPolicyList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrUserPolicy::InsertUserPolicy(DB_USER_POLICY& dup)
{
	m_strQuery = SPrintf("INSERT INTO user_policy (used_flag, reg_date, host_id, "
						DBMS_POLICY_QUERY_INSERT_NAME
						") VALUES (%u, %u, %u, %s)",
						dup.nUsedFlag, dup.nRegDate, dup.nUserID,
						GetPoQuery_Update(dup.tDP).c_str());
	
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(dup.nID == 0)
		dup.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrUserPolicy::UpdateUserPolicy(DB_USER_POLICY& dup)
{
	m_strQuery = SPrintf("UPDATE user_policy SET "
						"%s"
						" WHERE id=%u;", 
						GetPoQuery_Update(dup.tDP).c_str(),
						dup.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrUserPolicy::DeleteUserPolicy(UINT32 nID)
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

INT32	CDBMgrUserPolicy::LoadExecute(PVOID lpTempletList)
{
	TListDBUserPolicy* ptDBList = (TListDBUserPolicy*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrUserPolicy::InsertExecute(PVOID lpTemplet)
{
	PDB_USER_POLICY pd_t = (PDB_USER_POLICY)lpTemplet;

    return InsertUserPolicy(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrUserPolicy::UpdateExecute(PVOID lpTemplet)
{
	PDB_USER_POLICY pd_t = (PDB_USER_POLICY)lpTemplet;

    return UpdateUserPolicy(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrUserPolicy::DeleteExecute(UINT32 nID)
{
	return DeleteUserPolicy(nID);
}
//---------------------------------------------------------------------------






