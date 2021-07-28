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
#include "DBMgrEnvPolicy.h"

//---------------------------------------------------------------------------

CDBMgrEnvPolicy*		t_DBMgrEnvPolicy;

//---------------------------------------------------------------------------

CDBMgrEnvPolicy::CDBMgrEnvPolicy()
{
	m_strDBTName = "env_policy";
}
//---------------------------------------------------------------------------

CDBMgrEnvPolicy::~CDBMgrEnvPolicy()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvPolicy::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvPolicy::LoadDB(TListDBEnvPolicy& tDBEnvPolicyList)
{
    UINT32 nReadCnt = 0;
    DB_ENV_POLICY dep;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", policy_type , policy_option"
						" FROM env_policy WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		
		DB_PO_HEADER& tDPH = dep.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);

		dep.nPolicyType			= GetDBField_UInt(nIndex++);
		dep.nPolicyOption		= GetDBField_UInt(nIndex++);

        tDBEnvPolicyList.push_back(dep);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBEnvPolicyList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvPolicy::InsertEnvPolicy(DB_ENV_POLICY& dep)
{
	DB_PO_HEADER& tDPH = dep.tDPH;
		
	m_strQuery = SPrintf("INSERT INTO env_policy("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", policy_type, policy_option"
						") VALUES (%s"
						", %u, %u "
						");",                        
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dep.nPolicyType, dep.nPolicyOption);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvPolicy::UpdateEnvPolicy(DB_ENV_POLICY& dep)
{
	DB_PO_HEADER& tDPH = dep.tDPH;
	
	m_strQuery = SPrintf("UPDATE env_policy SET %s"
						", policy_type=%u, policy_option=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dep.nPolicyType, dep.nPolicyOption,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvPolicy::DeleteEnvPolicy(UINT32 nID)
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

INT32	CDBMgrEnvPolicy::LoadExecute(PVOID lpTempletList)
{
	TListDBEnvPolicy* ptDBList = (TListDBEnvPolicy*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvPolicy::InsertExecute(PVOID lpTemplet)
{
	PDB_ENV_POLICY pd_t = (PDB_ENV_POLICY)lpTemplet;

    return InsertEnvPolicy(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvPolicy::UpdateExecute(PVOID lpTemplet)
{
	PDB_ENV_POLICY pd_t = (PDB_ENV_POLICY)lpTemplet;

    return UpdateEnvPolicy(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvPolicy::DeleteExecute(UINT32 nID)
{
	return DeleteEnvPolicy(nID);
}
//---------------------------------------------------------------------------






