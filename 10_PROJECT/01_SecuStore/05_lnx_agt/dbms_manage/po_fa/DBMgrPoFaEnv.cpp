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
#include "DBMgrPoFaEnv.h"

//---------------------------------------------------------------------------

CDBMgrPoFaEnv*		t_DBMgrPoFaEnv = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaEnv::CDBMgrPoFaEnv()
{
	m_strDBTName = "po_fa_env";
}
//---------------------------------------------------------------------------

CDBMgrPoFaEnv::~CDBMgrPoFaEnv()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaEnv::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaEnv::LoadDB(TListDBPoFaEnv& tDBPoFaEnvList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_ENV dpfe;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
    					", sys_off_max_work_time"
						", notify_info_id"
						", lnx_sys_off_max_work_time"
						" FROM po_fa_env WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfe.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfe.nSysOffMaxWorkTime		= GetDBField_UInt(nIndex++);
		dpfe.nNotifyInfoID			= GetDBField_UInt(nIndex++);

		dpfe.nLnxSysOffMaxWorkTime  = GetDBField_UInt(nIndex++);
		
        tDBPoFaEnvList.push_back(dpfe);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaEnvList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaEnv::InsertPoFaEnv(DB_PO_FA_ENV& dpfe)
{
	DB_PO_HEADER& tDPH = dpfe.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_env("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sys_off_max_work_time"
						", notify_info_id"
						", lnx_sys_off_max_work_time "
						") VALUES (%s"
						", %u"
						", %u"
						", %u"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(),
						dpfe.nSysOffMaxWorkTime,
						dpfe.nNotifyInfoID,
						dpfe.nLnxSysOffMaxWorkTime);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaEnv::UpdatePoFaEnv(DB_PO_FA_ENV& dpfe)
{
	DB_PO_HEADER& tDPH = dpfe.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_env SET %s"
						", sys_off_max_work_time=%u"
						", notify_info_id=%u"
						", lnx_sys_off_max_work_time=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfe.nSysOffMaxWorkTime,
						dpfe.nNotifyInfoID,
						dpfe.nLnxSysOffMaxWorkTime,
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaEnv::DeletePoFaEnv(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}

INT32			CDBMgrPoFaEnv::ClearPoFaEnv()
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_CLEAR, m_strDBTName.c_str());

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

INT32	CDBMgrPoFaEnv::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaEnv* ptDBList = (TListDBPoFaEnv*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaEnv::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_ENV pd_t = (PDB_PO_FA_ENV)lpTemplet;

    return InsertPoFaEnv(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaEnv::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_ENV pd_t = (PDB_PO_FA_ENV)lpTemplet;

    return UpdatePoFaEnv(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaEnv::DeleteExecute(UINT32 nID)
{
	return DeletePoFaEnv(nID);
}

INT32	CDBMgrPoFaEnv::ClearExecute()
{
	return ClearPoFaEnv();
}
//---------------------------------------------------------------------------






