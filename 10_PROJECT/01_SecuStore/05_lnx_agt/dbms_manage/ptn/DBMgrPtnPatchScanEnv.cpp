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
#include "DBMgrPtnPatchScanEnv.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnPatchScanEnv::CDBMgrPtnPatchScanEnv()
{
	m_strDBTName = "ptn_patch_scan_env";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnPatchScanEnv::~CDBMgrPtnPatchScanEnv()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScanEnv::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScanEnv::LoadDB(TListDBPtnPatchScanEnv& tDBPtnPatchScanEnvList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_PATCH_SCAN_ENV data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_mode, name, reg_date"
						" FROM ptn_patch_scan_env;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID			= GetDBField_Int(nIndex++);
		data.nUsedMode		= GetDBField_Int(nIndex++);

		data.strName		= GetDBField_String(nIndex++);
		data.nRegDate		= GetDBField_Int(nIndex++);	

        tDBPtnPatchScanEnvList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBPtnPatchScanEnvList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScanEnv::InsertPtnPatchScanEnv(DB_PTN_PATCH_SCAN_ENV& data)
{
	m_strQuery = SPrintf("INSERT INTO ptn_patch_scan_env (used_mode, name, reg_date"
						") VALUES (%u, '%s', %u"
						");",
						data.nUsedMode, data.strName.c_str(), data.nRegDate);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScanEnv::UpdatePtnPatchScanEnv(DB_PTN_PATCH_SCAN_ENV& data)
{
	m_strQuery = SPrintf("UPDATE ptn_patch_scan_env SET used_mode=%u, name='%s', reg_date=%u"
						" WHERE id=%u;",
						data.nUsedMode, data.strName.c_str(), data.nRegDate,
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchScanEnv::DeletePtnPatchScanEnv(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM ptn_patch_scan_env WHERE id=%u;", nID);

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

INT32	CDBMgrPtnPatchScanEnv::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnPatchScanEnv* ptDBList = (TListDBPtnPatchScanEnv*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------
INT32	CDBMgrPtnPatchScanEnv::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH_SCAN_ENV pd_t = (PDB_PTN_PATCH_SCAN_ENV)lpTemplet;

    return InsertPtnPatchScanEnv(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchScanEnv::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH_SCAN_ENV pd_t = (PDB_PTN_PATCH_SCAN_ENV)lpTemplet;

    return UpdatePtnPatchScanEnv(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchScanEnv::DeleteExecute(UINT32 nID)
{
	return DeletePtnPatchScanEnv(nID);
}
//---------------------------------------------------------------------------






