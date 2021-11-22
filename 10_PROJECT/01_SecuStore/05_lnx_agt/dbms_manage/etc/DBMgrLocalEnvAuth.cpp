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
#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrLocalEnvAuth.h"

//---------------------------------------------------------------------------

CDBMgrLocalEnvAuth*		t_DBMgrLocalEnvAuth;

//---------------------------------------------------------------------------

CDBMgrLocalEnvAuth::CDBMgrLocalEnvAuth()
{
	m_strDBTName = "local_env_auth";
}
//---------------------------------------------------------------------------

CDBMgrLocalEnvAuth::~CDBMgrLocalEnvAuth()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLocalEnvAuth::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLocalEnvAuth::LoadDB(TListDBLocalEnvAuth& tDBLocalEnvAuthList)
{
    UINT32 nReadCnt = 0;
    DB_LOCAL_ENV_AUTH dlea;

    INT32 nIndex = 0;

    m_strQuery = SPrintf("SELECT id, used_flag, evt_time"
						", unicode"
						", account_id, account_pw"
						" FROM local_env_auth WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;

		dlea.nID			= GetDBField_UInt(nIndex++);
		dlea.nUsedFlag		= GetDBField_UInt(nIndex++);
		dlea.nEvtTime		= GetDBField_UInt(nIndex++);
		dlea.strUniCode		= GetDBField_String(nIndex++);
		dlea.strAccountID	= GetDBField_String(nIndex++);
		dlea.strAccountPW	= GetDBField_String(nIndex++);

        tDBLocalEnvAuthList.push_back(dlea);
        if(m_nLoadMaxID < UINT32(dlea.nID))
			m_nLoadMaxID = dlea.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBLocalEnvAuthList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLocalEnvAuth::InsertLocalEnvAuth(DB_LOCAL_ENV_AUTH& dlea)
{
	m_strQuery = SPrintf("INSERT INTO local_env_auth("
						" used_flag, evt_time"
						", unicode"
						", account_id, account_pw"
						") VALUES ("
						" %u, %u "
						", '%s'"
						", '%s', '%s' "
						");",                        
						dlea.nUsedFlag, dlea.nEvtTime,
						dlea.strUniCode.c_str(),
						dlea.strAccountID.c_str(), dlea.strAccountPW.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(dlea.nID == 0)
		dlea.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLocalEnvAuth::UpdateLocalEnvAuth(DB_LOCAL_ENV_AUTH& dlea)
{
	m_strQuery = SPrintf("UPDATE local_env_auth SET "
						"used_flag=%u, evt_time=%u "
						", unicode='%s'"
						", account_id='%s', account_pw='%s'"
						" WHERE id=%u;",
						dlea.nUsedFlag, dlea.nEvtTime,
						dlea.strUniCode.c_str(),
						dlea.strAccountID.c_str(), dlea.strAccountPW.c_str(), 
						dlea.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLocalEnvAuth::DeleteLocalEnvAuth(UINT32 nID)
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

INT32	CDBMgrLocalEnvAuth::LoadExecute(PVOID lpTempletList)
{
	TListDBLocalEnvAuth* ptDBList = (TListDBLocalEnvAuth*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLocalEnvAuth::InsertExecute(PVOID lpTemplet)
{
	PDB_LOCAL_ENV_AUTH pd_t = (PDB_LOCAL_ENV_AUTH)lpTemplet;

    return InsertLocalEnvAuth(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLocalEnvAuth::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOCAL_ENV_AUTH pd_t = (PDB_LOCAL_ENV_AUTH)lpTemplet;

    return UpdateLocalEnvAuth(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLocalEnvAuth::DeleteExecute(UINT32 nID)
{
	return DeleteLocalEnvAuth(nID);
}
//---------------------------------------------------------------------------






