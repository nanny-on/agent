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
#include "DBMgrSiteVuln.h"

//---------------------------------------------------------------------------

CDBMgrSiteVuln*		t_DBMgrSiteVuln = NULL;

//---------------------------------------------------------------------------

CDBMgrSiteVuln::CDBMgrSiteVuln()
{
	m_strDBTName = "site_vuln";
}
//---------------------------------------------------------------------------

CDBMgrSiteVuln::~CDBMgrSiteVuln()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteVuln::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteVuln::LoadDB(TListDBSiteVuln& tDBSiteVulnList)
{
    UINT32 nReadCnt = 0;
    DB_SITE_VULN data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT "
							"  id, used_flag, reg_date, used_mode, admin_id, ext_option"
							", class"
							", name, descr"
							", sys_type, sys_sp_type, sys_arch_type"
							", support_option, risk_level, def_score"
							" FROM site_vuln;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_UInt(nIndex++);
		data.nUsedFlag				= GetDBField_UInt(nIndex++);
		data.nRegDate				= GetDBField_UInt(nIndex++);
		data.nUsedMode				= GetDBField_UInt(nIndex++);
		data.nAdminID				= GetDBField_UInt(nIndex++);
		data.nExtOption				= GetDBField_UInt(nIndex++);

		data.strName				= GetDBField_String(nIndex++);
		data.strDescr				= GetDBField_String(nIndex++);

		data.nSysType				= GetDBField_UInt64(nIndex++);
		data.nSysSPType				= GetDBField_Int(nIndex++);
		data.nSysArchType			= GetDBField_Int(nIndex++);

		data.nSupportOption			= GetDBField_Int(nIndex++);
		data.nRiskLevel				= GetDBField_Int(nIndex++);
		data.nDefScore				= GetDBField_Int(nIndex++);
		
        tDBSiteVulnList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))
			m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBSiteVulnList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteVuln::InsertSiteVuln(DB_SITE_VULN& data)
{
	m_strQuery = SPrintf("INSERT INTO site_vuln ("
								"  reg_date, used_mode, admin_id, ext_option"
								", class"
								", name, descr"
								", sys_type, sys_sp_type, sys_arch_type"
								", support_option, risk_level, def_score"
								") VALUES ("
								"  %u, %u, %u, %u"
								", %u"
								", '%s', '%s'"
								", '%I64u', %u, %u"
								", %u, %u, %u"
								");",
								data.nRegDate, data.nUsedMode, data.nAdminID, data.nExtOption,
								data.nClass,
								data.strName.c_str(), data.strDescr.c_str(), 
								data.nClass,
								data.nSysType, data.nSysSPType, data.nSysArchType,
								data.nSupportOption, data.nRiskLevel, data.nDefScore);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteVuln::UpdateSiteVuln(DB_SITE_VULN& data)
{
	m_strQuery = SPrintf("UPDATE site_vuln SET "
								"  reg_date=%u, used_mode=%u, admin_id=%u, ext_option=%u"
								", class=%u"
								", name='%s', descr='%s'"
								", sys_type='%llu', sys_sp_type=%u, sys_arch_type=%u"
								", support_option=%u, risk_level=%u, def_score=%u"
								" WHERE id=%u;",
								data.nRegDate, data.nUsedMode, data.nAdminID, data.nExtOption,
								data.nClass,
								data.strName.c_str(), data.strDescr.c_str(),
								data.nSysType, data.nSysSPType, data.nSysArchType,
								data.nSupportOption, data.nRiskLevel, data.nDefScore,
								data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteVuln::DeleteSiteVuln(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM site_vuln WHERE id=%u;", nID);

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

INT32	CDBMgrSiteVuln::LoadExecute(PVOID lpTempletList)
{
	TListDBSiteVuln* ptDBList = (TListDBSiteVuln*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteVuln::InsertExecute(PVOID lpTemplet)
{
	PDB_SITE_VULN pd_t = (PDB_SITE_VULN)lpTemplet;

    return InsertSiteVuln(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteVuln::UpdateExecute(PVOID lpTemplet)
{
	PDB_SITE_VULN pd_t = (PDB_SITE_VULN)lpTemplet;

    return UpdateSiteVuln(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteVuln::DeleteExecute(UINT32 nID)
{
	return DeleteSiteVuln(nID);
}
//---------------------------------------------------------------------------






