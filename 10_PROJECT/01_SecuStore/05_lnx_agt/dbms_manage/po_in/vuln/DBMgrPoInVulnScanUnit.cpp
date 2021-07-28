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
#include "DBMgrPoInVulnScanUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnScanUnit*		t_DBMgrPoInVulnScanUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnScanUnit::CDBMgrPoInVulnScanUnit()
{
	m_strDBTName = "po_in_vuln_scan_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnScanUnit::~CDBMgrPoInVulnScanUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnScanUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnScanUnit::LoadDB(TListDBPoInVulnScanUnit& tDBPoInVulnScanUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_SCAN_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", sch_time, scan_type"
						" FROM po_in_vuln_scan_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nSchTime				= GetDBField_UInt64(nIndex++);
		data.nScanType				= GetDBField_UInt(nIndex++);
		
        tDBPoInVulnScanUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInVulnScanUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnScanUnit::InsertPoInVulnScanUnit(DB_PO_IN_VULN_SCAN_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_scan_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sch_time, scan_type"
						") VALUES (%s"
						", '%llu', %u"
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nSchTime, data.nScanType);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnScanUnit::UpdatePoInVulnScanUnit(DB_PO_IN_VULN_SCAN_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_scan_unit SET %s"
						", sch_time='%llu', scan_type=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						data.nSchTime, data.nScanType, 
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

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

INT32	CDBMgrPoInVulnScanUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnScanUnit* ptDBList = (TListDBPoInVulnScanUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnScanUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SCAN_UNIT pd_t = (PDB_PO_IN_VULN_SCAN_UNIT)lpTemplet;

    return InsertPoInVulnScanUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnScanUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SCAN_UNIT pd_t = (PDB_PO_IN_VULN_SCAN_UNIT)lpTemplet;

    return UpdatePoInVulnScanUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnScanUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






