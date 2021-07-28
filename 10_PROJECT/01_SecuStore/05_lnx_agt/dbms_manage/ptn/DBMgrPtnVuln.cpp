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
#include "DBMgrPtnVuln.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnVuln::CDBMgrPtnVuln()
{
	m_strDBTName = "ptn_vuln";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnVuln::~CDBMgrPtnVuln()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVuln::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVuln::LoadDB(TListDBPtnVuln& tDBPtnVulnList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_VULN data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_mode, reg_date"
						", class"
						", name, descr"
						", sys_type, sys_sp_type, sys_arch_type"
						", support_option, risk_level, def_score"
						" FROM ptn_vuln;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;
    
    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedMode				= GetDBField_Int(nIndex++);
		data.nRegDate				= GetDBField_Int(nIndex++);

		data.nClass					= GetDBField_Int(nIndex++);

		data.strName				= GetDBField_String(nIndex++);
		data.strDescr				= GetDBField_String(nIndex++);		

		data.nSysType				= GetDBField_UInt64(nIndex++);
		data.nSysSPType				= GetDBField_Int(nIndex++);
		data.nSysArchType			= GetDBField_Int(nIndex++);

		data.nSupportOption			= GetDBField_Int(nIndex++);
		data.nRiskLevel				= GetDBField_Int(nIndex++);
		data.nDefScore				= GetDBField_Int(nIndex++);

        tDBPtnVulnList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = (UINT32)tDBPtnVulnList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVuln::InsertPtnVuln(DB_PTN_VULN& data)
{
	m_strQuery = SPrintf("INSERT INTO ptn_vuln (id, used_mode, reg_date"
						", class"
						", name, descr"
						", sys_type, sys_sp_type, sys_arch_type"
						", support_option, risk_level, def_score"
						") VALUES (%u, %u, %u"
						", %u"
						", '%s', '%s'"
						", '%llu', %u, %u"
						", %u, %u, %u"
						");",
						data.nID, data.nUsedMode, data.nRegDate,
						data.nClass,
						data.strName.c_str(), data.strDescr.c_str(),
						data.nSysType, data.nSysSPType, data.nSysArchType,
						data.nSupportOption, data.nRiskLevel, data.nDefScore
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVuln::UpdatePtnVuln(DB_PTN_VULN& data)
{
	m_strQuery = SPrintf("UPDATE ptn_vuln SET used_mode=%u, reg_date=%u"
						", class=%u"
						", name='%s', descr='%s'"
						", sys_type='%llu', sys_sp_type=%u, sys_arch_type=%u"
						", support_option=%u, risk_level=%u, def_score=%u"
						" WHERE id=%u;",
						data.nUsedMode, data.nRegDate,
						data.nClass,
						data.strName.c_str(), data.strDescr.c_str(),
						data.nSysType, data.nSysSPType, data.nSysArchType,
						data.nSupportOption, data.nRiskLevel, data.nDefScore,
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVuln::DeletePtnVuln(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM ptn_vuln WHERE id=%u;", nID);

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

INT32	CDBMgrPtnVuln::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnVuln* ptDBList = (TListDBPtnVuln*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVuln::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_VULN pd_t = (PDB_PTN_VULN)lpTemplet;

    return InsertPtnVuln(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVuln::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_VULN pd_t = (PDB_PTN_VULN)lpTemplet;

    return UpdatePtnVuln(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVuln::DeleteExecute(UINT32 nID)
{
	return DeletePtnVuln(nID);
}
//---------------------------------------------------------------------------






