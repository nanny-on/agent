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
#include "DBMgrPoInRsOpObjUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInRsOpObjUnit*		t_DBMgrPoInRsOpObjUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInRsOpObjUnit::CDBMgrPoInRsOpObjUnit()
{
	m_strDBTName = "po_in_rs_op_obj_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInRsOpObjUnit::~CDBMgrPoInRsOpObjUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpObjUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_rs_op_obj_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpObjUnit::LoadDB(TListDBPoInRsOpObjUnit& tDBPoInRsOpObjUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_RS_OP_OBJ_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
							", chk_type, chk_path, chk_ext"
							" FROM po_in_rs_op_obj_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.strChkType				= GetDBField_String(nIndex++);
		data.strChkPath				= GetDBField_String(nIndex++);
		data.strChkExt				= GetDBField_String(nIndex++);
		
        tDBPoInRsOpObjUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInRsOpObjUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpObjUnit::InsertPoInRsOpObjUnit(DB_PO_IN_RS_OP_OBJ_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;
	
	m_strQuery = SPrintf("INSERT INTO po_in_rs_op_obj_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", chk_type, chk_path, chk_ext"
						") VALUES (%s"
						", '%s', '%s', '%s');",
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.strChkType.c_str(), data.strChkPath.c_str(), data.strChkExt.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpObjUnit::UpdatePoInRsOpObjUnit(DB_PO_IN_RS_OP_OBJ_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_rs_op_obj_unit SET %s"
						", chk_type='%s', chk_path='%s', chk_ext='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.strChkType.c_str(), data.strChkPath.c_str(), data.strChkExt.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpObjUnit::DeletePoInRsOpObjUnit(UINT32 nID)
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

INT32	CDBMgrPoInRsOpObjUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInRsOpObjUnit* ptDBList = (TListDBPoInRsOpObjUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpObjUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_OP_OBJ_UNIT pd_t = (PDB_PO_IN_RS_OP_OBJ_UNIT)lpTemplet;

    return InsertPoInRsOpObjUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpObjUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_OP_OBJ_UNIT pd_t = (PDB_PO_IN_RS_OP_OBJ_UNIT)lpTemplet;

    return UpdatePoInRsOpObjUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpObjUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoInRsOpObjUnit(nID);
}
//---------------------------------------------------------------------------






