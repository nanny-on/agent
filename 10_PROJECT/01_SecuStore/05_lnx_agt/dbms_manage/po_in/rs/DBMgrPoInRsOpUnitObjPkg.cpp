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
#include "DBMgrPoInRsOpUnitObjPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoInRsOpUnitObjPkg*		t_DBMgrPoInRsOpUnitObjPkg;

//---------------------------------------------------------------------------

CDBMgrPoInRsOpUnitObjPkg::CDBMgrPoInRsOpUnitObjPkg()
{
	m_strDBTName = "po_in_rs_op_unit_obj_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoInRsOpUnitObjPkg::~CDBMgrPoInRsOpUnitObjPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpUnitObjPkg::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_rs_op_unit_obj_pkg][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpUnitObjPkg::LoadDB(TListDBPoInRsOpUnitObjPkg& tDBPoInRsOpUnitObjPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_RS_OP_UNIT_OBJ_PKG dpfouop;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_in_rs_op_unit_obj_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfouop.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoInRsOpUnitObjPkgList.push_back(dpfouop);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInRsOpUnitObjPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpUnitObjPkg::InsertPoInRsOpUnitObjPkg(DB_PO_IN_RS_OP_UNIT_OBJ_PKG& dpfouop)
{
	DB_PO_HEADER& tDPH = dpfouop.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_rs_op_unit_obj_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						") VALUES (%s"
						");",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpUnitObjPkg::UpdatePoInRsOpUnitObjPkg(DB_PO_IN_RS_OP_UNIT_OBJ_PKG& dpfouop)
{
	DB_PO_HEADER& tDPH = dpfouop.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_rs_op_unit_obj_pkg SET "
						"%s"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpUnitObjPkg::DeletePoInRsOpUnitObjPkg(UINT32 nID)
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

INT32	CDBMgrPoInRsOpUnitObjPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInRsOpUnitObjPkg* ptDBList = (TListDBPoInRsOpUnitObjPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpUnitObjPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_OP_UNIT_OBJ_PKG pd_t = (PDB_PO_IN_RS_OP_UNIT_OBJ_PKG)lpTemplet;

    return InsertPoInRsOpUnitObjPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpUnitObjPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_OP_UNIT_OBJ_PKG pd_t = (PDB_PO_IN_RS_OP_UNIT_OBJ_PKG)lpTemplet;

    return UpdatePoInRsOpUnitObjPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpUnitObjPkg::DeleteExecute(UINT32 nID)
{
	return DeletePoInRsOpUnitObjPkg(nID);
}
//---------------------------------------------------------------------------






