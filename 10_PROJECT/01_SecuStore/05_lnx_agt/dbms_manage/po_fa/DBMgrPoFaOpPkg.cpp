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
#include "DBMgrPoFaOpPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoFaOpPkg*		t_DBMgrPoFaOpPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaOpPkg::CDBMgrPoFaOpPkg()
{
	m_strDBTName = "po_fa_op_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoFaOpPkg::~CDBMgrPoFaOpPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpPkg::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpPkg::LoadDB(TListDBPoFaOpPkg& tDBPoFaOpPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_OP_PKG dpfpp;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						", rule_order FROM po_fa_op_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfpp.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		dpfpp.nRuleOrder			= GetDBField_UInt(nIndex++);
		
        tDBPoFaOpPkgList.push_back(dpfpp);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaOpPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpPkg::InsertPoFaOpPkg(DB_PO_FA_OP_PKG& dpfpp)
{
	DB_PO_HEADER& tDPH = dpfpp.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_op_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						", rule_order"
						") VALUES (%s"
						", %u);",
						GetPoPkgHDRQuery_InsertValue(tDPH).c_str(), 
						dpfpp.nRuleOrder);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpPkg::UpdatePoFaOpPkg(DB_PO_FA_OP_PKG& dpfpp)
{
	DB_PO_HEADER& tDPH = dpfpp.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_op_pkg SET "
						"%s"
						", rule_order=%u WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
						dpfpp.nRuleOrder, 
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpPkg::DeletePoFaOpPkg(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}

//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpPkg::ClearPoFaOpPkg()
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

INT32	CDBMgrPoFaOpPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaOpPkg* ptDBList = (TListDBPoFaOpPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_PKG pd_t = (PDB_PO_FA_OP_PKG)lpTemplet;

    return InsertPoFaOpPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_PKG pd_t = (PDB_PO_FA_OP_PKG)lpTemplet;

    return UpdatePoFaOpPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpPkg::DeleteExecute(UINT32 nID)
{
	return DeletePoFaOpPkg(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpPkg::ClearExecute()
{
	return ClearPoFaOpPkg();
}
//---------------------------------------------------------------------------





