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
#include "DBMgrPtnPatchScan.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnPatchScan::CDBMgrPtnPatchScan()
{
	m_strDBTName = "ptn_patch_scan";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnPatchScan::~CDBMgrPtnPatchScan()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScan::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScan::LoadDB(TListDBPtnPatchScan& tDBPtnPatchScanList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_PATCH_SCAN data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_mode, patch_id, scan_env_id, scan_type"
						", tar_path, tar_name, tar_ver_begin, tar_ver_end, tar_comp_type"
						", sys_type, sys_sp_type, sys_arch_type, lan_id"
						", file_name, file_hash, file_size, file_exe_session, file_cmd"
						" FROM ptn_patch_scan;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;
    
    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedMode				= GetDBField_Int(nIndex++);

		data.nPatchID				= GetDBField_Int(nIndex++);	
		data.nScanEnvID				= GetDBField_Int(nIndex++);	
		data.nScanType				= GetDBField_Int(nIndex++);	

		data.strTarPath				= GetDBField_String(nIndex++);
		data.strTarName				= GetDBField_String(nIndex++);
		data.strTarVerBegin			= GetDBField_String(nIndex++);
		data.strTarVerEnd			= GetDBField_String(nIndex++);
		data.nTarCompType			= GetDBField_Int(nIndex++);	

		data.nSysType				= GetDBField_UInt64(nIndex++);	
		data.nSysSPType				= GetDBField_Int(nIndex++);	
		data.nSysArchType			= GetDBField_Int(nIndex++);	

		data.nLanID					= GetDBField_Int(nIndex++);

		data.strFileName			= GetDBField_String(nIndex++);
		data.strFileHash			= GetDBField_String(nIndex++);
		data.nFileSize				= GetDBField_Int(nIndex++);
		data.nFileExeSession		= GetDBField_Int(nIndex++);
		data.strFileCmd				= GetDBField_String(nIndex++);	

        tDBPtnPatchScanList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = (UINT32)tDBPtnPatchScanList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScan::InsertPtnPatchScan(DB_PTN_PATCH_SCAN& data)
{
	m_strQuery = SPrintf("INSERT INTO ptn_patch_scan (used_mode, patch_id, scan_env_id, scan_type"
						", tar_path, tar_name, tar_ver_begin, tar_ver_end, tar_comp_type"
						", sys_type, sys_sp_type, sys_arch_type, lan_id"
						", file_name, file_hash, file_size, file_exe_session, file_cmd"
						") VALUES (%u, %u, %u, %u"
						", '%s', '%s', '%s', '%s', %u"
						", '%llu', %u, %u, %u"
						", '%s', '%s', %u, %u, '%s'"
						");",
						data.nUsedMode, data.nPatchID, data.nScanEnvID, data.nScanType,
						data.strTarPath.c_str(), data.strTarName.c_str(), data.strTarVerBegin.c_str(), data.strTarVerEnd.c_str(), data.nTarCompType,
						data.nSysType, data.nSysSPType, data.nSysArchType, data.nLanID,
						data.strFileName.c_str(), data.strFileHash.c_str(), data.nFileSize, data.nFileExeSession, data.strFileCmd.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScan::UpdatePtnPatchScan(DB_PTN_PATCH_SCAN& data)
{
	m_strQuery = SPrintf("UPDATE ptn_patch_scan SET used_mode=%u, patch_id=%u, scan_env_id=%u, scan_type=%u"
						", tar_path=%u, tar_name='%s', tar_ver_begin='%s', tar_ver_end='%s', tar_comp_type=%u"
						", sys_type='%llu', sys_sp_type=%u, sys_arch_type=%u, lan_id=%u"
						", file_name='%s', file_hash='%s', file_size=%u, file_exe_session=%u, file_cmd='%s'"
						" WHERE id=%u;",
						data.nUsedMode, data.nPatchID, data.nScanEnvID, data.nScanType,
						data.strTarPath.c_str(), data.strTarName.c_str(), data.strTarVerBegin.c_str(), data.strTarVerEnd.c_str(), data.nTarCompType,
						data.nSysType, data.nSysSPType, data.nSysArchType, data.nLanID,
						data.strFileName.c_str(), data.strFileHash.c_str(), data.nFileSize, data.nFileExeSession, data.strFileCmd.c_str(),
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScan::DeletePtnPatchScan(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM ptn_patch_scan WHERE id=%u;", nID);

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

INT32	CDBMgrPtnPatchScan::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnPatchScan* ptDBList = (TListDBPtnPatchScan*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchScan::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH_SCAN pd_t = (PDB_PTN_PATCH_SCAN)lpTemplet;

    return InsertPtnPatchScan(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchScan::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH_SCAN pd_t = (PDB_PTN_PATCH_SCAN)lpTemplet;

    return UpdatePtnPatchScan(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchScan::DeleteExecute(UINT32 nID)
{
	return DeletePtnPatchScan(nID);
}
//---------------------------------------------------------------------------






