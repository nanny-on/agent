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
#include "DBMgrPoNcPtnOpUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoNcPtnOpUnit*		t_DBMgrPoNcPtnOpUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoNcPtnOpUnit::CDBMgrPoNcPtnOpUnit()
{
	m_strDBTName = "po_nc_ptn_op_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoNcPtnOpUnit::~CDBMgrPoNcPtnOpUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnOpUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_nc_ptn_op_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnOpUnit::LoadDB(TListDBPoNcPtnOpUnit& tDBPoNcPtnOpUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_NC_PTN_OP_UNIT data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT					
						", sch_time, last_scan_time"
						", scan_path, file_gather_type"
						" FROM po_nc_ptn_op_unit WHERE used_flag=1;");
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
		
        tDBPoNcPtnOpUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoNcPtnOpUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnOpUnit::InsertPoNcPtnOpUnit(DB_PO_NC_PTN_OP_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_nc_ptn_op_unit("
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

INT32			CDBMgrPoNcPtnOpUnit::UpdatePoNcPtnOpUnit(DB_PO_NC_PTN_OP_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_nc_ptn_op_unit SET %s"
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

INT32			CDBMgrPoNcPtnOpUnit::DeletePoNcPtnOpUnit(UINT32 nID)
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

INT32	CDBMgrPoNcPtnOpUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoNcPtnOpUnit* ptDBList = (TListDBPoNcPtnOpUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnOpUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_OP_UNIT pd_t = (PDB_PO_NC_PTN_OP_UNIT)lpTemplet;

    return InsertPoNcPtnOpUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnOpUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_OP_UNIT pd_t = (PDB_PO_NC_PTN_OP_UNIT)lpTemplet;

    return UpdatePoNcPtnOpUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnOpUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoNcPtnOpUnit(nID);
}
//---------------------------------------------------------------------------






