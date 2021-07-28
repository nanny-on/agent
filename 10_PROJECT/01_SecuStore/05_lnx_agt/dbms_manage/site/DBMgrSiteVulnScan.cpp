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
#include "DBMgrSiteVulnScan.h"

//---------------------------------------------------------------------------

CDBMgrSiteVulnScan*		t_DBMgrSiteVulnScan = NULL;

//---------------------------------------------------------------------------

CDBMgrSiteVulnScan::CDBMgrSiteVulnScan()
{
	m_strDBTName = "site_vuln_scan";
}
//---------------------------------------------------------------------------

CDBMgrSiteVulnScan::~CDBMgrSiteVulnScan()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteVulnScan::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteVulnScan::LoadDB(TListDBSiteVulnScan& tDBSiteVulnScanList)
{
    UINT32 nReadCnt = 0;
    DB_SITE_VULN_SCAN data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT "
							"  id, used_flag, reg_date, used_mode, admin_id, ext_option"
							", name"
							", vuln_id, vuln_sub_id, scan_order"
							", sys_type, sys_sp_type, sys_arch_type"
							", scan_type, custom_id"
							", scan_path, value_type"
							", decision_type, comp_type, comp_value"
							" FROM site_vuln_scan;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID						= GetDBField_UInt(nIndex++);
		data.nUsedFlag					= GetDBField_UInt(nIndex++);
		data.nRegDate					= GetDBField_UInt(nIndex++);
		data.nUsedMode					= GetDBField_UInt(nIndex++);
		data.nAdminID					= GetDBField_UInt(nIndex++);
		data.nExtOption					= GetDBField_UInt(nIndex++);

		data.strName					= GetDBField_String(nIndex++);

		data.nVulnID					= GetDBField_Int(nIndex++);
		data.nVulnSubID					= GetDBField_Int(nIndex++);
		data.nScanOrder					= GetDBField_Int(nIndex++);

		data.nSysType					= GetDBField_UInt64(nIndex++);
		data.nSysSPType					= GetDBField_Int(nIndex++);
		data.nSysArchType				= GetDBField_Int(nIndex++);

		data.nScanType					= GetDBField_UInt(nIndex++);
		data.nCustomID					= GetDBField_UInt(nIndex++);

		data.strScanPath				= GetDBField_String(nIndex++);
		data.nValueType					= GetDBField_UInt(nIndex++);

		data.nDecisionType				= GetDBField_UInt(nIndex++);
		data.nCompType					= GetDBField_UInt(nIndex++);
		data.strCompValue				= GetDBField_String(nIndex++);

		{
			PathUnixToWin(data.strScanPath);
			PathUnixToWin(data.strCompValue);
		}
		
        tDBSiteVulnScanList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))
			m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBSiteVulnScanList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteVulnScan::InsertSiteVulnScan(DB_SITE_VULN_SCAN& data)
{
	String strScanPath = PathWinToUnix(data.strScanPath);
	String strCompValue = PathWinToUnix(data.strCompValue);

	m_strQuery = SPrintf("INSERT INTO site_vuln_scan ("
								"  reg_date, used_mode, admin_id, ext_option" //1
								", name" //2
								", vuln_id, vuln_sub_id, scan_order" //3
								", sys_type, sys_sp_type, sys_arch_type" // 4
								", scan_type, custom_id" //5
								", scan_path, value_type" //6
								", decision_type, comp_type, comp_value" //7
								") VALUES ("
								"  %u, %u, %u, %u" //1
								", '%s'"
								", %u, %u, %u"
								", '%I64u', %u, %u"
								", %u, %u" //5
								", '%s', %u" //6
								", %u, %u, '%s'" //7
								");",
								data.nRegDate, data.nUsedMode, data.nAdminID, data.nExtOption,
								data.strName.c_str(), 
								data.nVulnID, data.nVulnSubID, data.nScanOrder,
								data.nSysType, data.nSysSPType, data.nSysArchType,
								data.nScanType, data.nCustomID,
								strScanPath.c_str(), data.nValueType,
								data.nDecisionType, data.nCompType, strCompValue.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteVulnScan::UpdateSiteVulnScan(DB_SITE_VULN_SCAN& data)
{
	String strScanPath = PathWinToUnix(data.strScanPath);
	String strCompValue = PathWinToUnix(data.strCompValue);

	m_strQuery = SPrintf("UPDATE site_vuln_scan SET "
							"  reg_date=%u, used_mode=%u, admin_id=%u, ext_option=%u"
							", name='%s'"
							", vuln_id=%u, vuln_sub_id=%u, scan_order=%u"
							", sys_type='%I64u', sys_sp_type=%u, sys_arch_type=%u"
							", scan_type=%u, custom_id=%u" //5
							", scan_path='%s', value_type=%u" //6
							", decision_type=%u, comp_type=%u, comp_value='%s'"
							" WHERE id=%u;",
							data.nRegDate, data.nUsedMode, data.nAdminID, data.nExtOption,
							data.strName.c_str(),
							data.nVulnID, data.nVulnSubID, data.nScanOrder,
							data.nSysType, data.nSysSPType, data.nSysArchType,
							data.nScanType, data.nCustomID,
							strScanPath.c_str(), data.nValueType,
							data.nDecisionType, data.nCompType, strCompValue.c_str(), 
							data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteVulnScan::DeleteSiteVulnScan(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM site_vuln_scan WHERE id=%u;", nID);

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

INT32	CDBMgrSiteVulnScan::LoadExecute(PVOID lpTempletList)
{
	TListDBSiteVulnScan* ptDBList = (TListDBSiteVulnScan*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteVulnScan::InsertExecute(PVOID lpTemplet)
{
	PDB_SITE_VULN_SCAN pd_t = (PDB_SITE_VULN_SCAN)lpTemplet;

    return InsertSiteVulnScan(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteVulnScan::UpdateExecute(PVOID lpTemplet)
{
	PDB_SITE_VULN_SCAN pd_t = (PDB_SITE_VULN_SCAN)lpTemplet;

    return UpdateSiteVulnScan(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteVulnScan::DeleteExecute(UINT32 nID)
{
	return DeleteSiteVulnScan(nID);
}
//---------------------------------------------------------------------------






