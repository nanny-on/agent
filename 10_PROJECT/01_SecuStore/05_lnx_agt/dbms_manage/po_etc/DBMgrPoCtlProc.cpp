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
#include "DBMgrPoCtlProc.h"

//---------------------------------------------------------------------------

CDBMgrPoCtlProc*		t_DBMgrPoCtlProc;

//---------------------------------------------------------------------------

CDBMgrPoCtlProc::CDBMgrPoCtlProc()
{
	m_strDBTName = "po_ctl_proc";
}
//---------------------------------------------------------------------------

CDBMgrPoCtlProc::~CDBMgrPoCtlProc()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlProc::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlProc::LoadDB(TListDBPoCtlProc& tDBPoCtlProcList)
{
    UINT32 nReadCnt = 0;
    DB_PO_CTL_PROC dpcp;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
    					", block_mode "
						" FROM po_ctl_proc WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpcp.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
		
		dpcp.nBlockMode				= GetDBField_UInt(nIndex++);
		
        tDBPoCtlProcList.push_back(dpcp);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoCtlProcList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlProc::InsertPoCtlProc(DB_PO_CTL_PROC& dpcp)
{
	DB_PO_HEADER& tDPH = dpcp.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_ctl_proc("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", block_mode"
						") VALUES (%s"
						", %u);",
						GetPoHDRQuery_InsertValue(tDPH).c_str(),
						dpcp.nBlockMode);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlProc::UpdatePoCtlProc(DB_PO_CTL_PROC& dpcp)
{
	DB_PO_HEADER& tDPH = dpcp.tDPH;

	m_strQuery = SPrintf("UPDATE po_ctl_proc SET %s"
						", block_mode=%u "
						"WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dpcp.nBlockMode,
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlProc::DeletePoCtlProc(UINT32 nID)
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

INT32	CDBMgrPoCtlProc::LoadExecute(PVOID lpTempletList)
{
	TListDBPoCtlProc* ptDBList = (TListDBPoCtlProc*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoCtlProc::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_CTL_PROC pd_t = (PDB_PO_CTL_PROC)lpTemplet;

    return InsertPoCtlProc(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoCtlProc::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_CTL_PROC pd_t = (PDB_PO_CTL_PROC)lpTemplet;

    return UpdatePoCtlProc(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoCtlProc::DeleteExecute(UINT32 nID)
{
	return DeletePoCtlProc(nID);
}
//---------------------------------------------------------------------------






