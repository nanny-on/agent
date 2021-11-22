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
#include "DBMgrPoDeployFileUnitInfoPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoDeployFileUnitInfoPkg*		t_DBMgrPoDeployFileUnitInfoPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoDeployFileUnitInfoPkg::CDBMgrPoDeployFileUnitInfoPkg()
{
	m_strDBTName = "po_deploy_file_unit_info_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoDeployFileUnitInfoPkg::~CDBMgrPoDeployFileUnitInfoPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnitInfoPkg::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnitInfoPkg::LoadDB(TListDBPoDeployFileUnitInfoPkg& tDBPoDeployFileUnitInfoPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_DEPLOY_FILE_UNIT_INFO_PKG dpdfuip;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_deploy_file_unit_info_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpdfuip.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoDeployFileUnitInfoPkgList.push_back(dpdfuip);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoDeployFileUnitInfoPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnitInfoPkg::InsertPoDeployFileUnitInfoPkg(DB_PO_DEPLOY_FILE_UNIT_INFO_PKG& dpdfuip)
{
	DB_PO_HEADER& tDPH = dpdfuip.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_deploy_file_unit_info_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						") VALUES (%s"
						");",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnitInfoPkg::UpdatePoDeployFileUnitInfoPkg(DB_PO_DEPLOY_FILE_UNIT_INFO_PKG& dpdfuip)
{
	DB_PO_HEADER& tDPH = dpdfuip.tDPH;

	m_strQuery = SPrintf("UPDATE po_deploy_file_unit_info_pkg SET "
						"%s"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnitInfoPkg::DeletePoDeployFileUnitInfoPkg(UINT32 nID)
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

INT32	CDBMgrPoDeployFileUnitInfoPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoDeployFileUnitInfoPkg* ptDBList = (TListDBPoDeployFileUnitInfoPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileUnitInfoPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pd_t = (PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG)lpTemplet;

    return InsertPoDeployFileUnitInfoPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileUnitInfoPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pd_t = (PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG)lpTemplet;

    return UpdatePoDeployFileUnitInfoPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileUnitInfoPkg::DeleteExecute(UINT32 nID)
{
	return DeletePoDeployFileUnitInfoPkg(nID);
}
//---------------------------------------------------------------------------






