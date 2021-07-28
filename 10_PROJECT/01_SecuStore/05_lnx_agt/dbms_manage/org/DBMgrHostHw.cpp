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
#include "DBMgrHostHw.h"

//---------------------------------------------------------------------------

CDBMgrHostHw*		t_DBMgrHostHw = NULL;

//---------------------------------------------------------------------------

CDBMgrHostHw::CDBMgrHostHw()
{
	m_strDBTName = "host_hw";
}
//---------------------------------------------------------------------------

CDBMgrHostHw::~CDBMgrHostHw()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostHw::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostHw::LoadDB(TListDBHostHw& tDBHostHwList)
{
    UINT32 nReadCnt = 0;
    DB_HOST_HW dhhi;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, "
						"pc_type, cpu_name, mem_size, hdd_size, lan_num, bios_date FROM host_hw WHERE used_flag <> 0;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		dhhi.nID					= GetDBField_Int(nIndex++) ;
		dhhi.nUsedFlag				= GetDBField_Int(nIndex++) ;
		
		dhhi.nPCType				= GetDBField_Int(nIndex++) ;
		dhhi.strCPUName				= GetDBField_String(nIndex++) ;
		dhhi.nMemSize				= GetDBField_Int(nIndex++) ;
		dhhi.nHDDSize				= GetDBField_Int(nIndex++) ;
		dhhi.nLanNum				= GetDBField_Int(nIndex++) ;
		dhhi.strBiosDate			= GetDBField_String(nIndex++);
		
        tDBHostHwList.push_back(dhhi);
        if(m_nLoadMaxID < UINT32(dhhi.nID))
			m_nLoadMaxID = dhhi.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBHostHwList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostHw::InsertHostHw(DB_HOST_HW& dhhi)
{
    m_strQuery = SPrintf("INSERT INTO host_hw (used_flag,  "
									"pc_type, cpu_name, mem_size, hdd_size, lan_num, bios_date)"
                                    "VALUES ( "
									"%u, "
                                    "%u, '%s', %u, %u, %u, '%s');",
                                    dhhi.nUsedFlag,
									dhhi.nPCType, dhhi.strCPUName.c_str(), dhhi.nMemSize, dhhi.nHDDSize, dhhi.nLanNum, dhhi.strBiosDate.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
	if(dhhi.nID == 0)
		dhhi.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostHw::UpdateHostHw(DB_HOST_HW& dhhi)
{
	m_strQuery = SPrintf("UPDATE host_hw SET "
						"used_flag=%u, pc_type=%u, cpu_name='%s', mem_size=%u, hdd_size=%u, lan_num=%u, bios_date='%s' "
						"WHERE id=%u;",
						1, dhhi.nPCType, dhhi.strCPUName.c_str(), dhhi.nMemSize, dhhi.nHDDSize, dhhi.nLanNum, dhhi.strBiosDate.c_str(), 
						dhhi.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostHw::DeleteHostHw(UINT32 nID)
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

INT32	CDBMgrHostHw::LoadExecute(PVOID lpTempletList)
{
	TListDBHostHw* ptDBList = (TListDBHostHw*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostHw::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST_HW pd_t = (PDB_HOST_HW)lpTemplet;

    return InsertHostHw(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostHw::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST_HW pd_t = (PDB_HOST_HW)lpTemplet;

    return UpdateHostHw(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostHw::DeleteExecute(UINT32 nID)
{
	return DeleteHostHw(nID);
}
//---------------------------------------------------------------------------






