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

#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrPoInRsOp.h"

//---------------------------------------------------------------------------

CDBMgrPoInRsOp*		t_DBMgrPoInRsOp = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInRsOp::CDBMgrPoInRsOp()
{
	m_strDBTName = "po_in_rs_op";
}
//---------------------------------------------------------------------------

CDBMgrPoInRsOp::~CDBMgrPoInRsOp()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOp::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_rs_op][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOp::LoadDB(TListDBPoInRsOp& tDBPoInRsOpList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_RS_OP data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						" "
						" FROM po_in_rs_op WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
		
        tDBPoInRsOpList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInRsOpList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOp::InsertPoInRsOp(DB_PO_IN_RS_OP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_rs_op("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						""
						") VALUES (%s"
						""
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOp::UpdatePoInRsOp(DB_PO_IN_RS_OP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_rs_op SET %s"
						""
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOp::DeletePoInRsOp(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
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

INT32	CDBMgrPoInRsOp::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInRsOp* ptDBList = (TListDBPoInRsOp*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOp::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_OP pd_t = (PDB_PO_IN_RS_OP)lpTemplet;

    return InsertPoInRsOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOp::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_OP pd_t = (PDB_PO_IN_RS_OP)lpTemplet;

    return UpdatePoInRsOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOp::DeleteExecute(UINT32 nID)
{
	return DeletePoInRsOp(nID);
}
//---------------------------------------------------------------------------






