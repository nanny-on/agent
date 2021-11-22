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
#include "DBMgrPtnDeploy.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnDeploy::CDBMgrPtnDeploy()
{
	m_strDBTName = "ptn_deploy";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnDeploy::~CDBMgrPtnDeploy()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnDeploy::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnDeploy::LoadDB(TListDBPtnDeploy& tDBPtnDeployList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_DEPLOY data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_mode"
						", type"
						", sys_type, sys_sp_type, sys_arch_type"
						", match_package, match_policy, match_control"
						", sv_file_name, sv_file_hash"
						", dn_file_type, dn_file_path, dn_file_name, dn_file_hash"
						" FROM ptn_deploy;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;

		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedMode				= GetDBField_Int(nIndex++);    	

		data.nType					= GetDBField_Int(nIndex++);    	

		data.nSysType				= GetDBField_UInt64(nIndex++);    	
		data.nSysSPType				= GetDBField_Int(nIndex++);    	
		data.nSysArchType			= GetDBField_Int(nIndex++);    	

		data.nMatchPackage			= GetDBField_UInt64(nIndex++);    	
		data.nMatchPolicy			= GetDBField_UInt64(nIndex++);    	
		data.nMatchControl			= GetDBField_UInt64(nIndex++);    	

		data.strSvFileName			= GetDBField_String(nIndex++);    	
		data.strSvFileHash			= GetDBField_String(nIndex++);    

		data.nDnFileType			= GetDBField_Int(nIndex++);
		data.strDnFilePath			= GetDBField_String(nIndex++);
		data.strDnFileName			= GetDBField_String(nIndex++);
		data.strDnFileHash			= GetDBField_String(nIndex++);
				
        tDBPtnDeployList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = (UINT32)tDBPtnDeployList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnDeploy::InsertPtnDeploy(DB_PTN_DEPLOY& data)
{
	m_strQuery = SPrintf("INSERT INTO ptn_deploy (id, used_mode"
					", type"
					", sys_type, sys_sp_type, sys_arch_type"
					", match_package, match_policy, match_control"
					", sv_file_name, sv_file_hash"
					", dn_file_type, dn_file_path, dn_file_name, dn_file_hash"
					") VALUES (%u, %u"
					", %u"
					", '%llu', %u, %u"
					", '%llu', '%llu', '%llu'"
					", '%s', '%s'"
					", %u, '%s', '%s', '%s'"
					");",
					data.nID, data.nUsedMode, 
					data.nType,
					data.nSysType, data.nSysSPType, data.nSysArchType,
					data.nMatchPackage, data.nMatchPolicy, data.nMatchControl,
					data.strSvFileName.c_str(), data.strSvFileHash.c_str(),
					data.nDnFileType, data.strDnFilePath.c_str(), data.strDnFileName.c_str(), data.strDnFileHash.c_str()
					);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnDeploy::UpdatePtnDeploy(DB_PTN_DEPLOY& data)
{
	m_strQuery = SPrintf("UPDATE ptn_deploy SET used_mode=%u"
						", type=%u"
						", sys_type='%llu', sys_sp_type=%u, sys_arch_type=%u"
						", match_package='%llu', match_policy='%llu', match_control='%llu'"
						", sv_file_name='%s', sv_file_hash='%s'"
						", dn_file_type=%u, dn_file_path='%s', dn_file_name='%s', dn_file_hash='%s'"
						" WHERE id=%u;",
						data.nUsedMode, 
						data.nType,
						data.nSysType, data.nSysSPType, data.nSysArchType,
						data.nMatchPackage, data.nMatchPolicy, data.nMatchControl,
						data.strSvFileName.c_str(), data.strSvFileHash.c_str(),
						data.nDnFileType, data.strDnFilePath.c_str(), data.strDnFileName.c_str(), data.strDnFileHash.c_str(),
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnDeploy::DeletePtnDeploy(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM ptn_deploy WHERE id=%u;", nID);

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

INT32	CDBMgrPtnDeploy::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnDeploy* ptDBList = (TListDBPtnDeploy*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnDeploy::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_DEPLOY pd_t = (PDB_PTN_DEPLOY)lpTemplet;

    return InsertPtnDeploy(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnDeploy::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_DEPLOY pd_t = (PDB_PTN_DEPLOY)lpTemplet;

    return UpdatePtnDeploy(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnDeploy::DeleteExecute(UINT32 nID)
{
	return DeletePtnDeploy(nID);
}
//---------------------------------------------------------------------------






