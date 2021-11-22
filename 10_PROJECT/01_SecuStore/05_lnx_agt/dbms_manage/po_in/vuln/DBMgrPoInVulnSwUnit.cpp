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
#include "DBMgrPoInVulnSwUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnSwUnit*		t_DBMgrPoInVulnSwUnit;

//---------------------------------------------------------------------------

CDBMgrPoInVulnSwUnit::CDBMgrPoInVulnSwUnit()
{
	m_strDBTName = "po_in_vuln_sw_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnSwUnit::~CDBMgrPoInVulnSwUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSwUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_sw_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSwUnit::LoadDB(TListDBPoInVulnSwUnit& tDBPoInVulnSwUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_SW_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", sw_id, class_name, sw_regkey"
						" FROM po_in_vuln_sw_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nSwID					= GetDBField_UInt(nIndex++);
		data.strClassName			= GetDBField_String(nIndex++);
		data.strSwRegKey			= GetDBField_String(nIndex++);
		
        tDBPoInVulnSwUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInVulnSwUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSwUnit::InsertPoInVulnSwUnit(DB_PO_IN_VULN_SW_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_sw_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sw_id, class_name, sw_regkey"
						") VALUES (%s"
						", %u, '%s', '%s'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nSwID, data.strClassName.c_str(), data.strSwRegKey.c_str()
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSwUnit::UpdatePoInVulnSwUnit(DB_PO_IN_VULN_SW_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_sw_unit SET %s"
						", sw_id=%u,  class_name='%s', sw_regkey='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nSwID, data.strClassName.c_str(), data.strSwRegKey.c_str(),
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

INT32	CDBMgrPoInVulnSwUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnSwUnit* ptDBList = (TListDBPoInVulnSwUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSwUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SW_UNIT pd_t = (PDB_PO_IN_VULN_SW_UNIT)lpTemplet;

    return InsertPoInVulnSwUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSwUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SW_UNIT pd_t = (PDB_PO_IN_VULN_SW_UNIT)lpTemplet;

    return UpdatePoInVulnSwUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSwUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






