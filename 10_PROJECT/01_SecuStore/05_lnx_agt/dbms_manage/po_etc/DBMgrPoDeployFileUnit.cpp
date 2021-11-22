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
#include "DBMgrPoDeployFileUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoDeployFileUnit*		t_DBMgrPoDeployFileUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoDeployFileUnit::CDBMgrPoDeployFileUnit()
{
	m_strDBTName = "po_deploy_file_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoDeployFileUnit::~CDBMgrPoDeployFileUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnit::LoadDB(TListDBPoDeployFileUnit& tDBPoDeployFileUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_DEPLOY_FILE_UNIT dpdfu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						" FROM po_deploy_file_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpdfu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
				
        tDBPoDeployFileUnitList.push_back(dpdfu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBPoDeployFileUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnit::InsertPoDeployFileUnit(DB_PO_DEPLOY_FILE_UNIT& dpdfu)
{
	DB_PO_HEADER& tDPH = dpdfu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_deploy_file_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						") VALUES (%s"
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnit::UpdatePoDeployFileUnit(DB_PO_DEPLOY_FILE_UNIT& dpdfu)
{
	DB_PO_HEADER& tDPH = dpdfu.tDPH;

	m_strQuery = SPrintf("UPDATE po_deploy_file_unit SET %s"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileUnit::DeletePoDeployFileUnit(UINT32 nID)
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

INT32	CDBMgrPoDeployFileUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoDeployFileUnit* ptDBList = (TListDBPoDeployFileUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_UNIT pd_t = (PDB_PO_DEPLOY_FILE_UNIT)lpTemplet;

    return InsertPoDeployFileUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_UNIT pd_t = (PDB_PO_DEPLOY_FILE_UNIT)lpTemplet;

    return UpdatePoDeployFileUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoDeployFileUnit(nID);
}
//---------------------------------------------------------------------------






