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
#include "DBMgrPoDeployFileUnitSchPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoDeployFileUnitSchPkg*		t_DBMgrPoDeployFileUnitSchPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoDeployFileUnitSchPkg::CDBMgrPoDeployFileUnitSchPkg()
{
	m_strDBTName = "po_deploy_file_unit_sch_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoDeployFileUnitSchPkg::~CDBMgrPoDeployFileUnitSchPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnitSchPkg::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnitSchPkg::LoadDB(TListDBPoDeployFileUnitSchPkg& tDBPoDeployFileUnitSchPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_DEPLOY_FILE_UNIT_SCH_PKG dpdfusp;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						", scan_dt"
						" FROM po_deploy_file_unit_sch_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpdfusp.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		dpdfusp.nScanTime			= GetDBField_UInt(nIndex++);
		
        tDBPoDeployFileUnitSchPkgList.push_back(dpdfusp);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoDeployFileUnitSchPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnitSchPkg::InsertPoDeployFileUnitSchPkg(DB_PO_DEPLOY_FILE_UNIT_SCH_PKG& dpdfusp)
{
	DB_PO_HEADER& tDPH = dpdfusp.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_deploy_file_unit_sch_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						", scan_dt "
						") VALUES (%s"
						", %u"
						");",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str(),
						dpdfusp.nScanTime);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnitSchPkg::UpdatePoDeployFileUnitSchPkg(DB_PO_DEPLOY_FILE_UNIT_SCH_PKG& dpdfusp)
{
	DB_PO_HEADER& tDPH = dpdfusp.tDPH;

	m_strQuery = SPrintf("UPDATE po_deploy_file_unit_sch_pkg SET "
						"%s, "
						" scan_dt=%u"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
						dpdfusp.nScanTime,
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnitSchPkg::DeletePoDeployFileUnitSchPkg(UINT32 nID)
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

INT32	CDBMgrPoDeployFileUnitSchPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoDeployFileUnitSchPkg* ptDBList = (TListDBPoDeployFileUnitSchPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileUnitSchPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pd_t = (PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG)lpTemplet;

    return InsertPoDeployFileUnitSchPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileUnitSchPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pd_t = (PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG)lpTemplet;

    return UpdatePoDeployFileUnitSchPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileUnitSchPkg::DeleteExecute(UINT32 nID)
{
	return DeletePoDeployFileUnitSchPkg(nID);
}
//---------------------------------------------------------------------------






