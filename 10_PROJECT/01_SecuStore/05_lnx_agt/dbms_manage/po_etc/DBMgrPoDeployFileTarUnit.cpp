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
#include "DBMgrPoDeployFileTarUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoDeployFileTarUnit*		t_DBMgrPoDeployFileTarUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoDeployFileTarUnit::CDBMgrPoDeployFileTarUnit()
{
	m_strDBTName = "po_deploy_file_tar_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoDeployFileTarUnit::~CDBMgrPoDeployFileTarUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileTarUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileTarUnit::LoadDB(TListDBPoDeployFileTarUnit& tDBPoDeployFileTarUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_DEPLOY_FILE_TAR_UNIT dpdfcu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", sys_mode, sys_info"
						" FROM po_deploy_file_tar_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpdfcu.tDPH;
		tDPH						= GetDBField_PoHDR(nIndex);

		dpdfcu.nSysMode			= GetDBField_UInt64(nIndex++);
		dpdfcu.nSysInfo			= GetDBField_UInt64(nIndex++);
		
        tDBPoDeployFileTarUnitList.push_back(dpdfcu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoDeployFileTarUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileTarUnit::InsertPoDeployFileTarUnit(DB_PO_DEPLOY_FILE_TAR_UNIT& dpdfcu)
{
	DB_PO_HEADER& tDPH = dpdfcu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_deploy_file_tar_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sys_mode, sys_info"
						") VALUES (%s"
                        ", '%llu', '%llu'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpdfcu.nSysMode, dpdfcu.nSysInfo);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
	
	if(tDPH.nID == 0)
	    tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileTarUnit::UpdatePoDeployFileTarUnit(DB_PO_DEPLOY_FILE_TAR_UNIT& dpdfcu)
{
	DB_PO_HEADER& tDPH = dpdfcu.tDPH;

	m_strQuery = SPrintf("UPDATE po_deploy_file_tar_unit SET %s"
						", sys_mode='%llu', sys_info='%llu'"
						"WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dpdfcu.nSysMode, dpdfcu.nSysInfo,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileTarUnit::DeletePoDeployFileTarUnit(UINT32 nID)
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

INT32	CDBMgrPoDeployFileTarUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoDeployFileTarUnit* ptDBList = (TListDBPoDeployFileTarUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileTarUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_TAR_UNIT pd_t = (PDB_PO_DEPLOY_FILE_TAR_UNIT)lpTemplet;

    return InsertPoDeployFileTarUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileTarUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_TAR_UNIT pd_t = (PDB_PO_DEPLOY_FILE_TAR_UNIT)lpTemplet;

    return UpdatePoDeployFileTarUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileTarUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoDeployFileTarUnit(nID);
}
//---------------------------------------------------------------------------






