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
#include "DBMgrHostSys.h"

//---------------------------------------------------------------------------

CDBMgrHostSys*		t_DBMgrHostSys = NULL;

//---------------------------------------------------------------------------

CDBMgrHostSys::CDBMgrHostSys()
{
	m_strDBTName = "host_sys";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrHostSys::~CDBMgrHostSys()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostSys::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostSys::LoadDB(TListDBHostSys& tDBHostSysList)
{
    UINT32 nReadCnt = 0;
    DB_HOST_SYS dhs;
	String strOsID;
    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, "
						"os_id, os_sp, os_pd, os_pd_lux, os_pa "
						"FROM host_sys WHERE used_flag <> 0;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		dhs.nID						= GetDBField_Int(nIndex++) ;
		dhs.nUsedFlag				= GetDBField_Int(nIndex++) ;
		if(dhs.nUsedFlag == 0)
			break;
		strOsID						= GetDBField_String(nIndex++); 
		dhs.nOsID					= StringToUInt64(strOsID.c_str());
		dhs.nOsSP					= GetDBField_Int(nIndex++);
		dhs.nOsPd					= GetDBField_Int(nIndex++);
		dhs.strOsPdLux				= GetDBField_String(nIndex++);
		dhs.nOsPa					= GetDBField_Int(nIndex++);

        tDBHostSysList.push_back(dhs);
        if(m_nLoadMaxID < UINT32(dhs.nID))	m_nLoadMaxID = dhs.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBHostSysList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostSys::InsertHostSys(DB_HOST_SYS& dhs)
{
	String strOsID = SPrintf("%llu", dhs.nOsID);
	m_strQuery = SPrintf("INSERT INTO host_sys (used_flag,  "
									"os_id, os_sp, os_pd, os_pd_lux, os_pa )"
									"VALUES ( "
									"%u, "
									"'%s', %u, %u, '%s', %u);",
                                    dhs.nUsedFlag,
									strOsID.c_str(), dhs.nOsSP, dhs.nOsPd, dhs.strOsPdLux.c_str(), dhs.nOsPa);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(dhs.nID == 0)
		dhs.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostSys::UpdateHostSys(DB_HOST_SYS& dhs)
{
	String strOsID = SPrintf("%llu", dhs.nOsID);
	m_strQuery = SPrintf("UPDATE host_sys SET "
						"os_id='%s', os_sp=%u, os_pd=%u, os_pd_lux='%s', os_pa=%u "
						"WHERE id=%u;",
							strOsID.c_str(), dhs.nOsSP, dhs.nOsPd, dhs.strOsPdLux.c_str(), dhs.nOsPa, 
							dhs.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostSys::DeleteHostSys(UINT32 nID)
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

INT32	CDBMgrHostSys::LoadExecute(PVOID lpTempletList)
{
	TListDBHostSys* ptDBList = (TListDBHostSys*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------


INT32	CDBMgrHostSys::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST_SYS pd_t = (PDB_HOST_SYS)lpTemplet;

    return InsertHostSys(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostSys::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST_SYS pd_t = (PDB_HOST_SYS)lpTemplet;

    return UpdateHostSys(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostSys::DeleteExecute(UINT32 nID)
{
	return DeleteHostSys(nID);
}
//---------------------------------------------------------------------------






