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
#include "DBMgrHostKey.h"

//---------------------------------------------------------------------------

CDBMgrHostKey*		t_DBMgrHostKey = NULL;

//---------------------------------------------------------------------------

CDBMgrHostKey::CDBMgrHostKey()
{
	m_strDBTName = "host_key";
}
//---------------------------------------------------------------------------

CDBMgrHostKey::~CDBMgrHostKey()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostKey::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostKey::LoadDB(TListDBHostKey& tDBHostKeyList)
{
    UINT32 nReadCnt = 0;
    DB_HOST_KEY data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag "
						", backup_key, cross_key "
						" FROM host_key;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_Int(nIndex++) ;
		data.nUsedFlag				= GetDBField_Int(nIndex++) ;
		data.strBkKey				= GetDBField_String(nIndex++);
		data.strCrKey				= GetDBField_String(nIndex++);

		if(data.nID != 0)
		{
			tDBHostKeyList.push_back(data);
			if(m_nLoadMaxID < UINT32(data.nID))
				m_nLoadMaxID = data.nID;
			nReadCnt++;
		}
    }while(Next());
	m_nLoadNumber = (UINT32)tDBHostKeyList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostKey::InsertHostKey(DB_HOST_KEY& data)
{
	m_strQuery = SPrintf("INSERT INTO host_key (used_flag "
									", backup_key, cross_key "
									") VALUES ( "
									"%u "
									", '%s', '%s'"
									");",
                                    data.nUsedFlag,
									data.strBkKey.c_str(), data.strCrKey.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostKey::UpdateHostKey(DB_HOST_KEY& data)
{
	m_strQuery = SPrintf("UPDATE host_key SET "
						"backup_key='%s', cross_key='%s' "
						" WHERE id=%u;",
							data.strBkKey.c_str(), data.strCrKey.c_str(), 
							data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostKey::DeleteHostKey(UINT32 nID)
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

INT32	CDBMgrHostKey::LoadExecute(PVOID lpTempletList)
{
	TListDBHostKey* ptDBList = (TListDBHostKey*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostKey::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST_KEY pd_t = (PDB_HOST_KEY)lpTemplet;

    return InsertHostKey(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostKey::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST_KEY pd_t = (PDB_HOST_KEY)lpTemplet;

    return UpdateHostKey(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostKey::DeleteExecute(UINT32 nID)
{
	return DeleteHostKey(nID);
}
//---------------------------------------------------------------------------






