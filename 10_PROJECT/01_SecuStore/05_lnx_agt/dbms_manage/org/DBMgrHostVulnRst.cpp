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

#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrHostVulnRst.h"

//---------------------------------------------------------------------------

CDBMgrHostVulnRst*		t_DBMgrHostVulnRst = NULL;

//---------------------------------------------------------------------------

CDBMgrHostVulnRst::CDBMgrHostVulnRst()
{
	m_strDBTName = "host_vuln_rst";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrHostVulnRst::~CDBMgrHostVulnRst()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostVulnRst::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [host][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostVulnRst::LoadDB(TListDBHostVulnRst& tDBHostVulnRstList)
{
    UINT32 nReadCnt = 0;
    DB_HOST_VULN_RST data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, reg_date, sync_svr_step"
					", scan_mon, scan_date"
					", group_id, host_id"
					", ip_addr, user_name, sys_name, module_version"
					", scan_score, scan_rst"
					" FROM host_vuln_rst;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedFlag				= GetDBField_Int(nIndex++);
		data.nRegDate				= GetDBField_Int(nIndex++);		
		data.nSyncSvrStep			= GetDBField_Int(nIndex++);		

		data.nScanMon				= GetDBField_Int(nIndex++);		
		data.nScanDate				= GetDBField_Int(nIndex++);		
		data.nGroupID				= GetDBField_Int(nIndex++);		
		data.nHostID				= GetDBField_Int(nIndex++);		

		data.strIPAddr				= GetDBField_String(nIndex++);
		data.strUserName			= GetDBField_String(nIndex++);
		data.strSysName				= GetDBField_String(nIndex++);
		data.strVersion				= GetDBField_String(nIndex++);

		data.nScanScore				= GetDBField_Int(nIndex++);
		data.strScanRst				= GetDBField_String(nIndex++);

		tDBHostVulnRstList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBHostVulnRstList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostVulnRst::InsertHostVulnRst(DB_HOST_VULN_RST& data)
{
	m_strQuery = SPrintf("INSERT INTO host_vuln_rst (used_flag, reg_date, sync_svr_step"
							", scan_mon, scan_date"
							", group_id, host_id"
							", ip_addr, user_name, sys_name, module_version"
							", scan_score, scan_rst"
							") VALUES ( %u, %u, %u"
							", %u, %u"
							", %u, %u"
							", '%s', '%s', '%s', '%s'"
							", %u, '%s'"
							");",
							data.nUsedFlag, data.nRegDate, data.nSyncSvrStep,
							data.nScanMon, data.nScanDate,
							data.nGroupID, data.nHostID,
							data.strIPAddr.c_str(), data.strUserName.c_str(), data.strSysName.c_str(), data.strVersion.c_str(),
							data.nScanScore, data.strScanRst.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostVulnRst::UpdateHostVulnRst(DB_HOST_VULN_RST& data)
{
	m_strQuery = SPrintf("UPDATE host_vuln_rst SET reg_date=%u, sync_svr_step=%u"
							", scan_mon=%u, scan_date=%u"
							", group_id=%u, host_id=%u"
							", ip_addr='%s', user_name='%s', sys_name='%s', module_version='%u'"
							", scan_score=%u, scan_rst='%s'"
							" WHERE id=%u;",
							data.nRegDate, data.nSyncSvrStep,
							data.nScanMon, data.nScanDate,
							data.nGroupID, data.nHostID,
							data.strIPAddr.c_str(), data.strUserName.c_str(), data.strSysName.c_str(), data.nVersionID,
							data.nScanScore, data.strScanRst.c_str(), 
							data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostVulnRst::DeleteHostVulnRst(UINT32 nID)
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

INT32	CDBMgrHostVulnRst::LoadExecute(PVOID lpTempletList)
{
	TListDBHostVulnRst* ptDBList = (TListDBHostVulnRst*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostVulnRst::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST_VULN_RST pd_t = (PDB_HOST_VULN_RST)lpTemplet;

    return InsertHostVulnRst(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostVulnRst::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST_VULN_RST pd_t = (PDB_HOST_VULN_RST)lpTemplet;

    return UpdateHostVulnRst(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostVulnRst::DeleteExecute(UINT32 nID)
{
	return DeleteHostVulnRst(nID);
}
//---------------------------------------------------------------------------






