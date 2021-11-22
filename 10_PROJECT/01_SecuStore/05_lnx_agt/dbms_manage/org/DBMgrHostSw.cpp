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
#include "DBMgrHostSw.h"

//---------------------------------------------------------------------------

CDBMgrHostSw*		t_DBMgrHostSw = NULL;

//---------------------------------------------------------------------------

CDBMgrHostSw::CDBMgrHostSw()
{
	m_strDBTName = "host_sw";
}
//---------------------------------------------------------------------------

CDBMgrHostSw::~CDBMgrHostSw()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostSw::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostSw::LoadDB(TListDBHostSw& tDBHostSwList)
{
    UINT32 nReadCnt = 0;
    DB_HOST_SW dhs;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, reg_date, "
						"host_id, proc_arch, chk_key, name, publisher, ins_date, version "
						"FROM host_sw;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		dhs.nID						= GetDBField_Int(nIndex++);
		dhs.nUsedFlag				= GetDBField_Int(nIndex++);
		dhs.nRegDate				= GetDBField_Int(nIndex++);

		dhs.nHostID					= GetDBField_Int(nIndex++);
		dhs.nProcArch				= GetDBField_Int(nIndex++);
		dhs.strChkKey				= GetDBField_String(nIndex++);
		dhs.strName					= GetDBField_String(nIndex++);
		dhs.strPublisher			= GetDBField_String(nIndex++);
		dhs.strInsDate				= GetDBField_String(nIndex++);
		dhs.strVersion				= GetDBField_String(nIndex++);

		{
			QueryToMem(dhs.strName);
			QueryToMem(dhs.strPublisher);
		}

        tDBHostSwList.push_back(dhs);
        if(m_nLoadMaxID < UINT32(dhs.nID))	m_nLoadMaxID = dhs.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBHostSwList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostSw::InsertHostSw(DB_HOST_SW& dhs)
{
	String strName		 = MemToQuery(dhs.strName);
	String strPublisher = MemToQuery(dhs.strPublisher);

	m_strQuery = SPrintf("INSERT INTO host_sw (used_flag, reg_date, "
									"host_id, proc_arch, chk_key, name, publisher, ins_date, version )"
									"VALUES ( "
									"%u, %u,"
									"%u, %u, '%s', '%s', '%s', '%s', '%s');",
                                    dhs.nUsedFlag, dhs.nRegDate,
									dhs.nHostID, dhs.nProcArch, dhs.strChkKey.c_str(), strName.c_str(), strPublisher.c_str(), dhs.strInsDate.c_str(), dhs.strVersion.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(dhs.nID == 0)
		dhs.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostSw::UpdateHostSw(DB_HOST_SW& dhs)
{
	String strName		 = MemToQuery(dhs.strName);
	String strPublisher = MemToQuery(dhs.strPublisher);

	m_strQuery = SPrintf("UPDATE host_sw SET reg_date=%u, "
						"host_id=%u, proc_arch=%u, chk_key='%s', name='%s', publisher='%s', ins_date='%s', version='%s' "
						"WHERE id=%u;",
						dhs.nRegDate,	
						dhs.nHostID, dhs.nProcArch, dhs.strChkKey.c_str(), strName.c_str(), strPublisher.c_str(), dhs.strInsDate.c_str(), dhs.strVersion.c_str(), 
						dhs.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostSw::DeleteHostSw(UINT32 nID)
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

INT32	CDBMgrHostSw::LoadExecute(PVOID lpTempletList)
{
	TListDBHostSw* ptDBList = (TListDBHostSw*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostSw::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST_SW pd_t = (PDB_HOST_SW)lpTemplet;

    return InsertHostSw(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostSw::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST_SW pd_t = (PDB_HOST_SW)lpTemplet;

    return UpdateHostSw(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostSw::DeleteExecute(UINT32 nID)
{
	return DeleteHostSw(nID);
}
//---------------------------------------------------------------------------






