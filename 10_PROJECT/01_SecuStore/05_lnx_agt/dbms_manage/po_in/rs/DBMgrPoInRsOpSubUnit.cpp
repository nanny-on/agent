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

#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrPoInRsOpSubUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInRsOpSubUnit*		t_DBMgrPoInRsOpSubUnit;

//---------------------------------------------------------------------------

CDBMgrPoInRsOpSubUnit::CDBMgrPoInRsOpSubUnit()
{
	m_strDBTName = "po_in_rs_op_sub_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInRsOpSubUnit::~CDBMgrPoInRsOpSubUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpSubUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_rs_op_sub_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpSubUnit::LoadDB(TListDBPoInRsOpSubUnit& tDBPoInRsOpSubUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_RS_OP_SUB_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
							", sb_type, sb_proc, ac_mode"
							" FROM po_in_rs_op_sub_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.strSbType				= GetDBField_String(nIndex++);
		data.strSbProc				= GetDBField_String(nIndex++);
		data.nAcMode				= GetDBField_UInt(nIndex++);
		
        tDBPoInRsOpSubUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInRsOpSubUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpSubUnit::InsertPoInRsOpSubUnit(DB_PO_IN_RS_OP_SUB_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;
	
	m_strQuery = SPrintf("INSERT INTO po_in_rs_op_sub_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sb_type, sb_proc, ac_mode"
						") VALUES (%s"
						", '%s', '%s', %u);",
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.strSbType.c_str(), data.strSbProc.c_str(), data.nAcMode);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpSubUnit::UpdatePoInRsOpSubUnit(DB_PO_IN_RS_OP_SUB_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;
	
	m_strQuery = SPrintf("UPDATE po_in_rs_op_sub_unit SET %s"
						", sb_type='%s', sb_proc='%s', ac_mode=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.strSbType.c_str(), data.strSbProc.c_str(), data.nAcMode,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpSubUnit::DeletePoInRsOpSubUnit(UINT32 nID)
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

INT32	CDBMgrPoInRsOpSubUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInRsOpSubUnit* ptDBList = (TListDBPoInRsOpSubUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------


INT32	CDBMgrPoInRsOpSubUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_OP_SUB_UNIT pd_t = (PDB_PO_IN_RS_OP_SUB_UNIT)lpTemplet;

    return InsertPoInRsOpSubUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpSubUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_OP_SUB_UNIT pd_t = (PDB_PO_IN_RS_OP_SUB_UNIT)lpTemplet;

    return UpdatePoInRsOpSubUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpSubUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoInRsOpSubUnit(nID);
}
//---------------------------------------------------------------------------






