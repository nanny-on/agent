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
#include "DBMgrPoInVulnPatchExceptionUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnPatchExceptionUnit*		t_DBMgrPoInVulnPatchExceptionUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnPatchExceptionUnit::CDBMgrPoInVulnPatchExceptionUnit()
{
	m_strDBTName = "po_in_vuln_patch_exception_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnPatchExceptionUnit::~CDBMgrPoInVulnPatchExceptionUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnPatchExceptionUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_patch_exception_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnPatchExceptionUnit::LoadDB(TListDBPoInVulnPatchExceptionUnit& tDBPoInVulnPatchExceptionUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_PATCH_EXCEPTION_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", patch_info"
						" FROM po_in_vuln_patch_exception_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
		data.strPatchInfo			= GetDBField_String(nIndex++);
		
        tDBPoInVulnPatchExceptionUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInVulnPatchExceptionUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnPatchExceptionUnit::InsertPoInVulnPatchExceptionUnit(DB_PO_IN_VULN_PATCH_EXCEPTION_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_patch_exception_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", patch_info"
						") VALUES (%s"
						", '%s'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.strPatchInfo.c_str()
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnPatchExceptionUnit::UpdatePoInVulnPatchExceptionUnit(DB_PO_IN_VULN_PATCH_EXCEPTION_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_patch_exception_unit SET %s"
						", patch_info='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.strPatchInfo.c_str(), 
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

INT32	CDBMgrPoInVulnPatchExceptionUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnPatchExceptionUnit* ptDBList = (TListDBPoInVulnPatchExceptionUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnPatchExceptionUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_PATCH_EXCEPTION_UNIT pd_t = (PDB_PO_IN_VULN_PATCH_EXCEPTION_UNIT)lpTemplet;

    return InsertPoInVulnPatchExceptionUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnPatchExceptionUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_PATCH_EXCEPTION_UNIT pd_t = (PDB_PO_IN_VULN_PATCH_EXCEPTION_UNIT)lpTemplet;

    return UpdatePoInVulnPatchExceptionUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnPatchExceptionUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






