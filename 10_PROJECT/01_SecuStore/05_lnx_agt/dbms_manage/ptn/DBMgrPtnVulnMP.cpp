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
#include "DBMgrPtnVulnMP.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnVulnMP::CDBMgrPtnVulnMP()
{
	m_strDBTName = "ptn_vuln_mp";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnVulnMP::~CDBMgrPtnVulnMP()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVulnMP::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVulnMP::LoadDB(TListDBPtnVulnMP& tDBPtnVulnMPList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_VULN_MP data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_mode, type, code"
						", company_name, family_name, product_name"
						", sys_type, sys_sp_type, sys_arch_type"
						", match_package, match_policy, match_control"
						", check_type, check_id"
						" FROM ptn_vuln_mp;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedMode				= GetDBField_Int(nIndex++);

		data.nType					= GetDBField_Int(nIndex++);
		data.nCode					= GetDBField_Int(nIndex++);

		data.strCompanyName			= GetDBField_String(nIndex++);
		data.strFamilyName			= GetDBField_String(nIndex++);
		data.strProductName			= GetDBField_String(nIndex++);

		data.nSysType				= GetDBField_UInt64(nIndex++);	
		data.nSysSPType				= GetDBField_Int(nIndex++);	
		data.nSysArchType			= GetDBField_Int(nIndex++);	

		data.nMatchPackage			= GetDBField_UInt64(nIndex++);	
		data.nMatchPolicy			= GetDBField_UInt64(nIndex++);	
		data.nMatchControl			= GetDBField_UInt64(nIndex++);	

		data.nChkType				= GetDBField_Int(nIndex++);	
		data.nChkID					= GetDBField_Int(nIndex++);	

        tDBPtnVulnMPList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = (UINT32)tDBPtnVulnMPList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVulnMP::InsertPtnVulnMP(DB_PTN_VULN_MP& data)
{
	m_strQuery = SPrintf("INSERT INTO ptn_vuln_mp (used_mode, type, code"
						", company_name, family_name, product_name"
						", sys_type, sys_sp_type, sys_arch_type"
						", match_package, match_policy, match_control"
						", check_type, check_id"
						") VALUES (%u, %u, %u"
						", '%s', '%s', '%s'"
						", '%llu', %u, %u"
						", '%llu', '%llu', '%llu'"
						", %u, %u"
						");",
						data.nUsedMode, data.nType, data.nCode,
						data.strCompanyName.c_str(), data.strFamilyName.c_str(), data.strProductName.c_str(),
						data.nSysType, data.nSysSPType, data.nSysArchType,
						data.nMatchPackage, data.nMatchPolicy, data.nMatchControl, 
						data.nChkType, data.nChkID
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVulnMP::UpdatePtnVulnMP(DB_PTN_VULN_MP& data)
{
	m_strQuery = SPrintf("UPDATE ptn_vuln_mp SET used_mode=%u, type=%u, code=%u"
						", company_name='%s', family_name='%s', product_name='%s'"
						", sys_type='%llu', sys_sp_type=%u, sys_arch_type=%u"
						", match_package='%llu', match_policy='%llu', match_control='%llu'"
						", check_type=%u, check_id=%u"
						" WHERE id=%u;",
						data.nUsedMode, data.nType, data.nCode,
						data.strCompanyName.c_str(), data.strFamilyName.c_str(), data.strProductName.c_str(),
						data.nSysType, data.nSysSPType, data.nSysArchType,
						data.nMatchPackage, data.nMatchPolicy, data.nMatchControl, 
						data.nChkType, data.nChkID,
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVulnMP::DeletePtnVulnMP(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM ptn_vuln_mp WHERE id=%u;", nID);

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

INT32	CDBMgrPtnVulnMP::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnVulnMP* ptDBList = (TListDBPtnVulnMP*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVulnMP::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_VULN_MP pd_t = (PDB_PTN_VULN_MP)lpTemplet;

    return InsertPtnVulnMP(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVulnMP::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_VULN_MP pd_t = (PDB_PTN_VULN_MP)lpTemplet;

    return UpdatePtnVulnMP(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVulnMP::DeleteExecute(UINT32 nID)
{
	return DeletePtnVulnMP(nID);
}
//---------------------------------------------------------------------------






