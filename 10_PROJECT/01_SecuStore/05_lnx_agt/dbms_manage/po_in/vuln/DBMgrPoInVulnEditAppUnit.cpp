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
#include "DBMgrPoInVulnEditAppUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnEditAppUnit*		t_DBMgrPoInVulnEditAppUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnEditAppUnit::CDBMgrPoInVulnEditAppUnit()
{
	m_strDBTName = "po_in_vuln_edit_app_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnEditAppUnit::~CDBMgrPoInVulnEditAppUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnEditAppUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_edit_app_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnEditAppUnit::LoadDB(TListDBPoInVulnEditAppUnit& tDBPoInVulnEditAppUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_EDIT_APP_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", class_name, product_name, reg_key"
						" FROM po_in_vuln_edit_app_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.strClassName			= GetDBField_String(nIndex++);
		data.strName				= GetDBField_String(nIndex++);
		data.strRegKey				= GetDBField_String(nIndex++);
		
        tDBPoInVulnEditAppUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInVulnEditAppUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnEditAppUnit::InsertPoInVulnEditAppUnit(DB_PO_IN_VULN_EDIT_APP_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_edit_app_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", class_name, product_name, reg_key"
						") VALUES (%s"
						", '%s', '%s', '%s'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.strClassName.c_str(), data.strName.c_str(), data.strRegKey.c_str()
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnEditAppUnit::UpdatePoInVulnEditAppUnit(DB_PO_IN_VULN_EDIT_APP_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_edit_app_unit SET %s"
						", class_name='%s', product_name='%s', reg_key='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.strClassName.c_str(), data.strName.c_str(), data.strRegKey.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

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
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnEditAppUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnEditAppUnit* ptDBList = (TListDBPoInVulnEditAppUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnEditAppUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_EDIT_APP_UNIT pd_t = (PDB_PO_IN_VULN_EDIT_APP_UNIT)lpTemplet;

    return InsertPoInVulnEditAppUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnEditAppUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_EDIT_APP_UNIT pd_t = (PDB_PO_IN_VULN_EDIT_APP_UNIT)lpTemplet;

    return UpdatePoInVulnEditAppUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnEditAppUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






