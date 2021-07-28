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
#include "DBMgrPoInPtnOpUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInPtnOpUnit*		t_DBMgrPoInPtnOpUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInPtnOpUnit::CDBMgrPoInPtnOpUnit()
{
	m_strDBTName = "po_in_ptn_op_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInPtnOpUnit::~CDBMgrPoInPtnOpUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnOpUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnOpUnit::LoadDB(TListDBPoInPtnOpUnit& tDBPoInPtnOpUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_PTN_OP_UNIT data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT					
						", sch_time, last_scan_time"
						", scan_path, file_gather_type"
						" FROM po_in_ptn_op_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);	

		data.nSchTime				= GetDBField_UInt64(nIndex++);
		data.nLastScanTime			= GetDBField_UInt(nIndex++);

		data.strScanPath			= GetDBField_String(nIndex++);
		data.nFileGatherType		= GetDBField_UInt(nIndex++);
		
        tDBPoInPtnOpUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInPtnOpUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnOpUnit::InsertPoInPtnOpUnit(DB_PO_IN_PTN_OP_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_ptn_op_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sch_time, last_scan_time"
						", scan_path, file_gather_type"
						") VALUES (%s"
						", '%llu', %u"
						", '%s', %u"
						");",						
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 	
						data.nSchTime, data.nLastScanTime,
						data.strScanPath.c_str(), data.nFileGatherType);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnOpUnit::UpdatePoInPtnOpUnit(DB_PO_IN_PTN_OP_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_ptn_op_unit SET %s"
						", sch_time='%llu', last_scan_time=%u"
						", scan_path='%s', file_gather_type=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 	
						data.nSchTime, data.nLastScanTime, 
						data.strScanPath.c_str(), data.nFileGatherType, 
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnOpUnit::DeletePoInPtnOpUnit(UINT32 nID)
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

INT32	CDBMgrPoInPtnOpUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInPtnOpUnit* ptDBList = (TListDBPoInPtnOpUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnOpUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_OP_UNIT pd_t = (PDB_PO_IN_PTN_OP_UNIT)lpTemplet;

    return InsertPoInPtnOpUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnOpUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_OP_UNIT pd_t = (PDB_PO_IN_PTN_OP_UNIT)lpTemplet;

    return UpdatePoInPtnOpUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnOpUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoInPtnOpUnit(nID);
}
//---------------------------------------------------------------------------






