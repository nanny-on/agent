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


#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrHostPatch.h"

//---------------------------------------------------------------------------

CDBMgrHostPatch*		t_DBMgrHostPatch = NULL;

//---------------------------------------------------------------------------

CDBMgrHostPatch::CDBMgrHostPatch()
{
	m_strDBTName = "host_patch";
}
//---------------------------------------------------------------------------

CDBMgrHostPatch::~CDBMgrHostPatch()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostPatch::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [host][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostPatch::LoadDB(TListDBHostPatch& tDBHostPatchList)
{
    UINT32 nReadCnt = 0;
    DB_HOST_PATCH data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, reg_date"
						", host_id, find_type, kb_id"
						", category, name, publisher, ins_date"
						", guid, no_remove, uninstall "
						"FROM host_patch;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedFlag				= GetDBField_Int(nIndex++);
		data.nRegDate				= GetDBField_Int(nIndex++);

		data.nHostID				= GetDBField_Int(nIndex++);
		data.nFindType				= GetDBField_Int(nIndex++);
		data.nKBID					= GetDBField_Int(nIndex++);

		data.strCategory			= GetDBField_String(nIndex++);
		data.strName				= GetDBField_String(nIndex++);
		data.strPublisher			= GetDBField_String(nIndex++);
		data.strInsDate				= GetDBField_String(nIndex++);
		data.strGuid				= GetDBField_String(nIndex++);
		data.nNoRemove				= GetDBField_Int(nIndex++);
		data.strUninstall			= GetDBField_String(nIndex++);

        tDBHostPatchList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))
			m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBHostPatchList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostPatch::InsertHostPatch(DB_HOST_PATCH& data)
{
	String strName		 = MemToQuery(data.strName);
	String strPublisher = MemToQuery(data.strPublisher);

	m_strQuery = SPrintf("INSERT INTO host_patch (used_flag, reg_date"
									", host_id, find_type, kb_id"
									", category, name, publisher, ins_date"
									", guid, no_remove, uninstall"
									") VALUES (%u, %u "
									", %u, %u, %u"
									", '%s', '%s', '%s', '%s'"
									", '%s', %u, '%s');",
                                    data.nUsedFlag, data.nRegDate,
									data.nHostID, data.nFindType, data.nKBID,
									data.strCategory.c_str(), strName.c_str(), strPublisher.c_str(), data.strInsDate.c_str(), 
									data.strGuid.c_str(), data.nNoRemove, data.strUninstall.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostPatch::UpdateHostPatch(DB_HOST_PATCH& data)
{
	String strName		 = MemToQuery(data.strName);
	String strPublisher = MemToQuery(data.strPublisher);

	m_strQuery = SPrintf("UPDATE host_patch SET used_flag=%u, reg_date=%u "
						", host_id=%u, find_type=%u, kb_id=%u"
						", category='%s', name='%s', publisher='%s', ins_date='%s'"
						", guid='%s', no_remove=%u, uninstall='%s' "
						"WHERE id=%u;",
						data.nUsedFlag, data.nRegDate,	
						data.nHostID, data.nFindType, data.nKBID,
						data.strCategory.c_str(), strName.c_str(), strPublisher.c_str(), data.strInsDate.c_str(), 
						data.strGuid.c_str(), data.nNoRemove, data.strUninstall.c_str(), 
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostPatch::DeleteHostPatch(UINT32 nID)
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

INT32	CDBMgrHostPatch::LoadExecute(PVOID lpTempletList)
{
	TListDBHostPatch* ptDBList = (TListDBHostPatch*)lpTempletList;

    return LoadDB(*ptDBList);
}

INT32	CDBMgrHostPatch::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST_PATCH pd_t = (PDB_HOST_PATCH)lpTemplet;

    return InsertHostPatch(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostPatch::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST_PATCH pd_t = (PDB_HOST_PATCH)lpTemplet;

    return UpdateHostPatch(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostPatch::DeleteExecute(UINT32 nID)
{
	return DeleteHostPatch(nID);
}
//---------------------------------------------------------------------------






