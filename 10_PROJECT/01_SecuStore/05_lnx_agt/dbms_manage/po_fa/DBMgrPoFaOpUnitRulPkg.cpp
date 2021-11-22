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
#include "DBMgrPoFaOpUnitRulPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoFaOpUnitRulPkg*		t_DBMgrPoFaOpUnitRulPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaOpUnitRulPkg::CDBMgrPoFaOpUnitRulPkg()
{
	m_strDBTName = "po_fa_op_unit_rul_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoFaOpUnitRulPkg::~CDBMgrPoFaOpUnitRulPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnitRulPkg::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnitRulPkg::LoadDB(TListDBPoFaOpUnitRulPkg& tDBPoFaOpUnitRulPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_OP_UNIT_RUL_PKG dpfourp;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						", rule_order, ac_mode"
						" FROM po_fa_op_unit_rul_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfourp.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
		dpfourp.nRuleOrder			= GetDBField_UInt(nIndex++);
		dpfourp.nAcMode				= GetDBField_UInt(nIndex++);
		
        tDBPoFaOpUnitRulPkgList.push_back(dpfourp);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaOpUnitRulPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnitRulPkg::InsertPoFaOpUnitRulPkg(DB_PO_FA_OP_UNIT_RUL_PKG& dpfourp)
{
	DB_PO_HEADER& tDPH = dpfourp.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_op_unit_rul_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						", rule_order, ac_mode ) VALUES (%s"
						", %u, %u);",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str(),
						dpfourp.nRuleOrder, dpfourp.nAcMode);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnitRulPkg::UpdatePoFaOpUnitRulPkg(DB_PO_FA_OP_UNIT_RUL_PKG& dpfourp)
{
	DB_PO_HEADER& tDPH = dpfourp.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_op_unit_rul_pkg SET "
						"%s"
						", rule_order=%u, ac_mode=%u WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
						dpfourp.nRuleOrder, dpfourp.nAcMode,
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnitRulPkg::DeletePoFaOpUnitRulPkg(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnitRulPkg::ClearPoFaOpUnitRulPkg()
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

INT32	CDBMgrPoFaOpUnitRulPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaOpUnitRulPkg* ptDBList = (TListDBPoFaOpUnitRulPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpUnitRulPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_UNIT_RUL_PKG pd_t = (PDB_PO_FA_OP_UNIT_RUL_PKG)lpTemplet;

    return InsertPoFaOpUnitRulPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpUnitRulPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_UNIT_RUL_PKG pd_t = (PDB_PO_FA_OP_UNIT_RUL_PKG)lpTemplet;

    return UpdatePoFaOpUnitRulPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpUnitRulPkg::DeleteExecute(UINT32 nID)
{
	return DeletePoFaOpUnitRulPkg(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpUnitRulPkg::ClearExecute()
{
	return ClearPoFaOpUnitRulPkg();
}
//---------------------------------------------------------------------------






