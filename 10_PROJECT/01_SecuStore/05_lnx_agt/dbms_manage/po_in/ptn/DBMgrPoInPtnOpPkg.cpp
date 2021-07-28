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
#include "DBMgrPoInPtnOpPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoInPtnOpPkg*		t_DBMgrPoInPtnOpPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInPtnOpPkg::CDBMgrPoInPtnOpPkg()
{
	m_strDBTName = "po_in_ptn_op_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoInPtnOpPkg::~CDBMgrPoInPtnOpPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnOpPkg::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnOpPkg::LoadDB(TListDBPoInPtnOpPkg& tDBPoInPtnOpPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_PTN_OP_PKG data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_in_ptn_op_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoInPtnOpPkgList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInPtnOpPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnOpPkg::InsertPoInPtnOpPkg(DB_PO_IN_PTN_OP_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_ptn_op_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						") VALUES (%s"
						");",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnOpPkg::UpdatePoInPtnOpPkg(DB_PO_IN_PTN_OP_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_ptn_op_pkg SET "
						"%s"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnOpPkg::DeletePoInPtnOpPkg(UINT32 nID)
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

INT32	CDBMgrPoInPtnOpPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInPtnOpPkg* ptDBList = (TListDBPoInPtnOpPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnOpPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_OP_PKG pd_t = (PDB_PO_IN_PTN_OP_PKG)lpTemplet;

    return InsertPoInPtnOpPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnOpPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_OP_PKG pd_t = (PDB_PO_IN_PTN_OP_PKG)lpTemplet;

    return UpdatePoInPtnOpPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnOpPkg::DeleteExecute(UINT32 nID)
{
	return DeletePoInPtnOpPkg(nID);
}
//---------------------------------------------------------------------------






