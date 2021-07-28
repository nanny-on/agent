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
#include "DBMgrHostVulnRstUnit.h"

//---------------------------------------------------------------------------

CDBMgrHostVulnRstUnit*		t_DBMgrHostVulnRstUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrHostVulnRstUnit::CDBMgrHostVulnRstUnit()
{
	m_strDBTName = "host_vuln_rst_unit";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrHostVulnRstUnit::~CDBMgrHostVulnRstUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostVulnRstUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [host][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostVulnRstUnit::LoadDB(TListDBHostVulnRstUnit& tDBHostVulnRstUnitList)
{
    UINT32 nReadCnt = 0;
    DB_HOST_VULN_RST_UNIT data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, reg_date"
					", rst_vuln_id"
					", site_vuln_id, default_score, scan_result_type"
					", scan_result_info"
					" FROM host_vuln_rst_unit;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedFlag				= GetDBField_Int(nIndex++);
		data.nRegDate				= GetDBField_Int(nIndex++);		
		
		data.nVulnRstID				= GetDBField_Int(nIndex++);		
		
		data.tDHVRUS.nVulnID		= GetDBField_Int(nIndex++);		
		data.tDHVRUS.nDScore		= GetDBField_Int(nIndex++);		
		data.tDHVRUS.nRst			= GetDBField_Int(nIndex++);		

		data.strRstInfo				= GetDBField_String(nIndex++);

		tDBHostVulnRstUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBHostVulnRstUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostVulnRstUnit::InsertHostVulnRstUnit(DB_HOST_VULN_RST_UNIT& data)
{
	m_strQuery = SPrintf("INSERT INTO host_vuln_rst_unit (used_flag, reg_date"
							", rst_vuln_id, site_vuln_id"
							", default_score, scan_result_type"
							", scan_result_info"
							") VALUES ( %u, %u"
							", %u"
							", %u, %u, %u"
							", '%s'"
							");",
							data.nUsedFlag, data.nRegDate,
							data.nVulnRstID, 
							data.tDHVRUS.nVulnID, data.tDHVRUS.nDScore, data.tDHVRUS.nRst,
							data.strRstInfo.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostVulnRstUnit::UpdateHostVulnRstUnit(DB_HOST_VULN_RST_UNIT& data)
{
	m_strQuery = SPrintf("UPDATE host_vuln_rst_unit SET reg_date=%u"
							", rst_vuln_id=%u"
							", site_vuln_id=%u, default_score=%u, scan_result_type=%u"
							", scan_result_info='%s'"
							" WHERE id=%u;",
							data.nRegDate, 
							data.nVulnRstID, 
							data.tDHVRUS.nVulnID, data.tDHVRUS.nDScore, data.tDHVRUS.nRst,
							data.strRstInfo.c_str(), 
							data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostVulnRstUnit::DeleteHostVulnRstUnit(UINT32 nID)
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

INT32	CDBMgrHostVulnRstUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBHostVulnRstUnit* ptDBList = (TListDBHostVulnRstUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostVulnRstUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST_VULN_RST_UNIT pd_t = (PDB_HOST_VULN_RST_UNIT)lpTemplet;

    return InsertHostVulnRstUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostVulnRstUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST_VULN_RST_UNIT pd_t = (PDB_HOST_VULN_RST_UNIT)lpTemplet;

    return UpdateHostVulnRstUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostVulnRstUnit::DeleteExecute(UINT32 nID)
{
	return DeleteHostVulnRstUnit(nID);
}
//---------------------------------------------------------------------------






