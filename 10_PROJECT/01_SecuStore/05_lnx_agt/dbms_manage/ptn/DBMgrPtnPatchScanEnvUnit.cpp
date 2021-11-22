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
#include "DBMgrPtnPatchScanEnvUnit.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnPatchScanEnvUnit::CDBMgrPtnPatchScanEnvUnit()
{
	m_strDBTName = "ptn_patch_scan_env_unit";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnPatchScanEnvUnit::~CDBMgrPtnPatchScanEnvUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScanEnvUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScanEnvUnit::LoadDB(TListDBPtnPatchScanEnvUnit& tDBPtnPatchScanEnvUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_PATCH_SCAN_ENV_UNIT data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_mode, pkg_id, scan_type"
						", tar_path, tar_name, tar_ver_begin, tar_ver_end, tar_comp_type"
						", sys_type, sys_sp_type, sys_arch_type"
						" FROM ptn_patch_scan_env_unit;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID			= GetDBField_Int(nIndex++);
		data.nUsedMode		= GetDBField_Int(nIndex++);

		data.nPkgID			= GetDBField_Int(nIndex++);	
		data.nScanType		= GetDBField_Int(nIndex++);	

		data.strTarPath		= GetDBField_String(nIndex++);
		data.strTarName		= GetDBField_String(nIndex++);
		data.strTarVerBegin	= GetDBField_String(nIndex++);
		data.strTarVerEnd	= GetDBField_String(nIndex++);
		data.nTarCompType	= GetDBField_Int(nIndex++);	

		data.nSysType		= GetDBField_UInt64(nIndex++);	
		data.nSysSPType		= GetDBField_Int(nIndex++);	
		data.nSysArchType	= GetDBField_Int(nIndex++);	
		
        tDBPtnPatchScanEnvUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = (UINT32)tDBPtnPatchScanEnvUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScanEnvUnit::InsertPtnPatchScanEnvUnit(DB_PTN_PATCH_SCAN_ENV_UNIT& data)
{
	m_strQuery = SPrintf("INSERT INTO ptn_patch_scan_env_unit (used_mode, pkg_id, scan_type"
						", tar_path, tar_name, tar_ver_begin, tar_ver_end, tar_comp_type"
						", sys_type, sys_sp_type, sys_arch_type"
						") VALUES (%u, %u, %u"
						", '%s', '%s', '%s', '%s', %u"
						", '%llu', %u, %u"
						");",
						data.nUsedMode, data.nPkgID, data.nScanType,
						data.strTarPath.c_str(), data.strTarName.c_str(), data.strTarVerBegin.c_str(), data.strTarVerEnd.c_str(), data.nTarCompType,
						data.nSysType, data.nSysSPType, data.nSysArchType);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScanEnvUnit::UpdatePtnPatchScanEnvUnit(DB_PTN_PATCH_SCAN_ENV_UNIT& data)
{
	m_strQuery = SPrintf("UPDATE ptn_patch_scan_env_unit SET used_mode=%u, pkg_id=%u, scan_type=%u"
						", tar_path=%u, tar_name='%s', tar_ver_begin='%s', tar_ver_end='%s', tar_comp_type=%u"
						", sys_type='%llu', sys_sp_type=%u, sys_arch_type=%u"
						" WHERE id=%u;",
						data.nUsedMode, data.nPkgID, data.nScanType,
						data.strTarPath.c_str(), data.strTarName.c_str(), data.strTarVerBegin.c_str(), data.strTarVerEnd.c_str(), data.nTarCompType,
						data.nSysType, data.nSysSPType, data.nSysArchType,
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScanEnvUnit::DeletePtnPatchScanEnvUnit(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM ptn_patch_scan_env_unit WHERE id=%u;", nID);

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

INT32	CDBMgrPtnPatchScanEnvUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnPatchScanEnvUnit* ptDBList = (TListDBPtnPatchScanEnvUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchScanEnvUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH_SCAN_ENV_UNIT pd_t = (PDB_PTN_PATCH_SCAN_ENV_UNIT)lpTemplet;

    return InsertPtnPatchScanEnvUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchScanEnvUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH_SCAN_ENV_UNIT pd_t = (PDB_PTN_PATCH_SCAN_ENV_UNIT)lpTemplet;

    return UpdatePtnPatchScanEnvUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchScanEnvUnit::DeleteExecute(UINT32 nID)
{
	return DeletePtnPatchScanEnvUnit(nID);
}
//---------------------------------------------------------------------------






