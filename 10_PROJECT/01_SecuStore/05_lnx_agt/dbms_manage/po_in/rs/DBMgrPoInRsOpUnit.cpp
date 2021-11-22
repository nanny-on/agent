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
#include "DBMgrPoInRsOpUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInRsOpUnit*		t_DBMgrPoInRsOpUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInRsOpUnit::CDBMgrPoInRsOpUnit()
{
	m_strDBTName = "po_in_rs_op_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInRsOpUnit::~CDBMgrPoInRsOpUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_rs_op_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpUnit::LoadDB(TListDBPoInRsOpUnit& tDBPoInRsOpUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_RS_OP_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", ac_mode, log_mode "
						" FROM po_in_rs_op_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    
    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
		
		data.nAcMode				= GetDBField_UInt(nIndex++);
		m_strValue					= GetDBField_String(nIndex++);		HexToMap_64(m_strValue, data.tLogModeMap, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);
		
        tDBPoInRsOpUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInRsOpUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpUnit::InsertPoInRsOpUnit(DB_PO_IN_RS_OP_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;
	String strLogMode;
	MapToHex_64(data.tLogModeMap, strLogMode, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);
	
	m_strQuery = SPrintf("INSERT INTO po_in_rs_op_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", ac_mode, log_mode"
						") VALUES (%s"
						", %u, '%s');",
						GetPoHDRQuery_InsertValue(tDPH).c_str(),
						data.nAcMode, strLogMode.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpUnit::UpdatePoInRsOpUnit(DB_PO_IN_RS_OP_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;
	String strLogMode;
	MapToHex_64(data.tLogModeMap, strLogMode, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);
	
	m_strQuery = SPrintf("UPDATE po_in_rs_op_unit SET %s"
							", ac_mode=%u, log_mode='%s'"
							" WHERE id=%u;",
							GetPoHDRQuery_Update(tDPH).c_str(), 
							data.nAcMode, strLogMode.c_str(),
							tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsOpUnit::DeletePoInRsOpUnit(UINT32 nID)
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

INT32	CDBMgrPoInRsOpUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInRsOpUnit* ptDBList = (TListDBPoInRsOpUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_OP_UNIT pd_t = (PDB_PO_IN_RS_OP_UNIT)lpTemplet;

    return InsertPoInRsOpUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_OP_UNIT pd_t = (PDB_PO_IN_RS_OP_UNIT)lpTemplet;

    return UpdatePoInRsOpUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsOpUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoInRsOpUnit(nID);
}
//---------------------------------------------------------------------------






