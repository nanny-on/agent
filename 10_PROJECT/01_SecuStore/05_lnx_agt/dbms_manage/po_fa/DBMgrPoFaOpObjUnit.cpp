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
#include "DBMgrPoFaOpObjUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoFaOpObjUnit*		t_DBMgrPoFaOpObjUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaOpObjUnit::CDBMgrPoFaOpObjUnit()
{
	m_strDBTName = "po_fa_op_obj_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoFaOpObjUnit::~CDBMgrPoFaOpObjUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpObjUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpObjUnit::LoadDB(TListDBPoFaOpObjUnit& tDBPoFaOpObjUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_OP_OBJ_UNIT dpfoou;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", file_path, chk_ac_path, chk_self_path"
						" FROM po_fa_op_obj_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfoou.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfoou.strFilePath			= GetDBField_String(nIndex++);
		dpfoou.nChkAcPath			= GetDBField_UInt(nIndex++);
		dpfoou.nChkSelfPath			= GetDBField_UInt(nIndex++);
		
        tDBPoFaOpObjUnitList.push_back(dpfoou);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBPoFaOpObjUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpObjUnit::InsertPoFaOpObjUnit(DB_PO_FA_OP_OBJ_UNIT& dpfoou)
{
	DB_PO_HEADER& tDPH = dpfoou.tDPH;
	
	m_strQuery = SPrintf("INSERT INTO po_fa_op_obj_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", file_path, chk_ac_path, chk_self_path"
						") VALUES (%s"
                        ", '%s', %u, %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpfoou.strFilePath.c_str(), dpfoou.nChkAcPath, dpfoou.nChkSelfPath);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpObjUnit::UpdatePoFaOpObjUnit(DB_PO_FA_OP_OBJ_UNIT& dpfoou)
{
	DB_PO_HEADER& tDPH = dpfoou.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_op_obj_unit SET %s"
						", file_path='%s', chk_ac_path=%u, chk_self_path=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfoou.strFilePath.c_str(), dpfoou.nChkAcPath, dpfoou.nChkSelfPath,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpObjUnit::DeletePoFaOpObjUnit(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}

//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpObjUnit::ClearPoFaOpObjUnit()
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

INT32	CDBMgrPoFaOpObjUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaOpObjUnit* ptDBList = (TListDBPoFaOpObjUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpObjUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_OBJ_UNIT pd_t = (PDB_PO_FA_OP_OBJ_UNIT)lpTemplet;

    return InsertPoFaOpObjUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpObjUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_OBJ_UNIT pd_t = (PDB_PO_FA_OP_OBJ_UNIT)lpTemplet;

    return UpdatePoFaOpObjUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpObjUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoFaOpObjUnit(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpObjUnit::ClearExecute()
{
	return ClearPoFaOpObjUnit();
}
//---------------------------------------------------------------------------





