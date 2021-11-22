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
#include "DBMgrPtnVulnScan.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnVulnScan::CDBMgrPtnVulnScan()
{
	m_strDBTName = "ptn_vuln_scan";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnVulnScan::~CDBMgrPtnVulnScan()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVulnScan::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVulnScan::LoadDB(TListDBPtnVulnScan& tDBPtnVulnScanList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_VULN_SCAN data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_mode, reg_date"
						", vuln_id, vuln_sub_id, scan_order"
						", name"
						", sys_type, sys_sp_type, sys_arch_type"
						", scan_type, custom_id, scan_path, value_type"
						", decision_type, comp_type, comp_value"
						" FROM ptn_vuln_scan;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedMode				= GetDBField_Int(nIndex++);
		data.nRegDate				= GetDBField_Int(nIndex++);

		data.nVulnID				= GetDBField_Int(nIndex++);
		data.nVulnSubID				= GetDBField_Int(nIndex++);
		data.nScanOrder				= GetDBField_Int(nIndex++);

		data.strName				= GetDBField_String(nIndex++);		

		data.nSysType				= GetDBField_UInt64(nIndex++);
		data.nSysSPType				= GetDBField_Int(nIndex++);
		data.nSysArchType			= GetDBField_Int(nIndex++);

		data.nScanType				= GetDBField_Int(nIndex++);
		data.nCustomID				= GetDBField_Int(nIndex++);
		data.strScanPath			= GetDBField_String(nIndex++);
		data.nValueType				= GetDBField_Int(nIndex++);

		data.nDecisionType			= GetDBField_Int(nIndex++);
		data.nCompType				= GetDBField_Int(nIndex++);
		data.strCompValue			= GetDBField_String(nIndex++);

        tDBPtnVulnScanList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = (UINT32)tDBPtnVulnScanList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVulnScan::InsertPtnVulnScan(DB_PTN_VULN_SCAN& data)
{
	m_strQuery = SPrintf("INSERT INTO ptn_vuln_scan (id, used_mode, reg_date"
						", vuln_id, vuln_sub_id, scan_order"
						", name"
						", sys_type, sys_sp_type, sys_arch_type"
						", scan_type, custom_id, scan_path, value_type"
						", decision_type, comp_type, comp_value"
						") VALUES (%u, %u, %u"
						", %u, %u, %u"
						", '%s'"
						", '%llu', %u, %u"
						", %u, %u, '%s', %u"
						", %u, %u, '%s'"
						");",
						data.nID, data.nUsedMode, data.nRegDate, 
						data.nVulnID, data.nVulnSubID, data.nScanOrder,
						data.strName.c_str(),
						data.nSysType, data.nSysSPType, data.nSysArchType,
						data.nScanType, data.nCustomID, data.strScanPath.c_str(), data.nValueType, 
						data.nDecisionType, data.nCompType, data.strCompValue.c_str()
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

    if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVulnScan::UpdatePtnVulnScan(DB_PTN_VULN_SCAN& data)
{
	m_strQuery = SPrintf("UPDATE ptn_vuln_scan SET used_mode=%u, reg_date=%u"
						", vuln_id=%u, vuln_sub_id=%u, scan_order=%u"
						", name='%s'"
						", sys_type='%llu', sys_sp_type=%u, sys_arch_type=%u"
						", scan_type=%u, custom_id=%u, scan_path='%s', value_type=%u"
						", decision_type=%u, comp_type=%u, comp_value='%s'"
						" WHERE id=%u;",
						data.nUsedMode, data.nRegDate, 
						data.nVulnID, data.nVulnSubID, data.nScanOrder,
						data.strName.c_str(),
						data.nSysType, data.nSysSPType, data.nSysArchType,
						data.nScanType, data.nCustomID, data.strScanPath.c_str(), data.nValueType, 
						data.nDecisionType, data.nCompType, data.strCompValue.c_str(),
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVulnScan::DeletePtnVulnScan(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM ptn_vuln_scan WHERE id=%u;", nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_DELETE_FAIL;
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

INT32	CDBMgrPtnVulnScan::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnVulnScan* ptDBList = (TListDBPtnVulnScan*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVulnScan::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_VULN_SCAN pd_t = (PDB_PTN_VULN_SCAN)lpTemplet;

    return InsertPtnVulnScan(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVulnScan::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_VULN_SCAN pd_t = (PDB_PTN_VULN_SCAN)lpTemplet;

    return UpdatePtnVulnScan(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVulnScan::DeleteExecute(UINT32 nID)
{
	return DeletePtnVulnScan(nID);
}
//---------------------------------------------------------------------------






