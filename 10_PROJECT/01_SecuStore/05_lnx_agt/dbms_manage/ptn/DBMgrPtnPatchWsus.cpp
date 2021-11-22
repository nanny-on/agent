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
#include "DBMgrPtnPatchWsus.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnPatchWsus::CDBMgrPtnPatchWsus()
{
	m_strDBTName = "ptn_patch_wsus";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnPatchWsus::~CDBMgrPtnPatchWsus()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchWsus::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchWsus::LoadDB(TListDBPtnPatchWsus& tDBPtnPatchWsusList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_PATCH_WSUS data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id"
						", name, hash, sys_id, sys_sp_id"
						" FROM ptn_patch_wsus;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_Int(nIndex++);
		data.strName				= GetDBField_String(nIndex++);
		data.strHash				= GetDBField_String(nIndex++);

		data.nSysID					= GetDBField_UInt64(nIndex++);
		data.nSysSPID				= GetDBField_Int(nIndex++);

        tDBPtnPatchWsusList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = (UINT32)tDBPtnPatchWsusList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchWsus::InsertPtnPatchWsus(DB_PTN_PATCH_WSUS& data)
{
	m_strQuery = SPrintf("INSERT INTO ptn_patch_wsus (id"
						", name, hash, sys_id, sys_sp_id"
						") VALUES ( %u"
						", '%s', '%s', '%llu', %u"
						");",
						data.nID, 
						data.strName.c_str(), data.strHash.c_str(), data.nSysID, data.nSysSPID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchWsus::UpdatePtnPatchWsus(DB_PTN_PATCH_WSUS& data)
{
	m_strQuery = SPrintf("UPDATE ptn_patch_wsus SET "
						"name='%s', hash='%s', sys_id='%llu', sys_sp_id=%u"
						" WHERE id=%u;",
						data.strName.c_str(), data.strHash.c_str(), data.nSysID, data.nSysSPID,
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchWsus::DeletePtnPatchWsus(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM ptn_patch_wsus WHERE id=%u;", nID);

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

INT32	CDBMgrPtnPatchWsus::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnPatchWsus* ptDBList = (TListDBPtnPatchWsus*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchWsus::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH_WSUS pd_t = (PDB_PTN_PATCH_WSUS)lpTemplet;

    return InsertPtnPatchWsus(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchWsus::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH_WSUS pd_t = (PDB_PTN_PATCH_WSUS)lpTemplet;

    return UpdatePtnPatchWsus(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchWsus::DeleteExecute(UINT32 nID)
{
	return DeletePtnPatchWsus(nID);
}
//---------------------------------------------------------------------------






