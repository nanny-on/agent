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
#include "DBMgrPoDeployFileSchUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoDeployFileSchUnit*		t_DBMgrPoDeployFileSchUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoDeployFileSchUnit::CDBMgrPoDeployFileSchUnit()
{
	m_strDBTName = "po_deploy_file_sch_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoDeployFileSchUnit::~CDBMgrPoDeployFileSchUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileSchUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileSchUnit::LoadDB(TListDBPoDeployFileSchUnit& tDBPoDeployFileSchUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_DEPLOY_FILE_SCH_UNIT dpdfsu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", sch_type, sch_time"
						" FROM po_deploy_file_sch_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpdfsu.tDPH;
		tDPH					= GetDBField_PoHDR(nIndex);
		dpdfsu.nSchType			= GetDBField_UInt(nIndex++);
		dpdfsu.nSchTime			= GetDBField_UInt(nIndex++);
		
        tDBPoDeployFileSchUnitList.push_back(dpdfsu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoDeployFileSchUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileSchUnit::InsertPoDeployFileSchUnit(DB_PO_DEPLOY_FILE_SCH_UNIT& dpdfsu)
{
	DB_PO_HEADER& tDPH = dpdfsu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_deploy_file_sch_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sch_type, sch_time"
						") VALUES (%s"
                        ", %u, %u"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpdfsu.nSchType, dpdfsu.nSchTime);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileSchUnit::UpdatePoDeployFileSchUnit(DB_PO_DEPLOY_FILE_SCH_UNIT& dpdfsu)
{
	DB_PO_HEADER& tDPH = dpdfsu.tDPH;

	m_strQuery = SPrintf("UPDATE po_deploy_file_sch_unit SET %s"
						", sch_type=%u, sch_time=%u "
						"WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dpdfsu.nSchType, dpdfsu.nSchTime,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileSchUnit::DeletePoDeployFileSchUnit(UINT32 nID)
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

INT32	CDBMgrPoDeployFileSchUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoDeployFileSchUnit* ptDBList = (TListDBPoDeployFileSchUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileSchUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_SCH_UNIT pd_t = (PDB_PO_DEPLOY_FILE_SCH_UNIT)lpTemplet;

    return InsertPoDeployFileSchUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileSchUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_SCH_UNIT pd_t = (PDB_PO_DEPLOY_FILE_SCH_UNIT)lpTemplet;

    return UpdatePoDeployFileSchUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileSchUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoDeployFileSchUnit(nID);
}
//---------------------------------------------------------------------------






