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
#include "DBMgrPoFaOpUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoFaOpUnit*		t_DBMgrPoFaOpUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaOpUnit::CDBMgrPoFaOpUnit()
{
	m_strDBTName = "po_fa_op_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoFaOpUnit::~CDBMgrPoFaOpUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnit::LoadDB(TListDBPoFaOpUnit& tDBPoFaOpUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_OP_UNIT dpfpu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", log_mode, unit_type"
						" FROM po_fa_op_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfpu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		m_strValue					= GetDBField_String(nIndex++);
		HexToMap_64(m_strValue, dpfpu.tLogModeMap, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);
		dpfpu.nUnitType				= GetDBField_UInt(nIndex++);

        tDBPoFaOpUnitList.push_back(dpfpu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaOpUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnit::InsertPoFaOpUnit(DB_PO_FA_OP_UNIT& dpfpu)
{
	DB_PO_HEADER& tDPH = dpfpu.tDPH;
	String strLogMode;
	MapToHex_64(dpfpu.tLogModeMap, strLogMode, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);

	m_strQuery = SPrintf("INSERT INTO po_fa_op_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", log_mode, unit_type"
						") VALUES (%s"
                        ", '%s', %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						strLogMode.c_str(), dpfpu.nUnitType);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnit::UpdatePoFaOpUnit(DB_PO_FA_OP_UNIT& dpfpu)
{
	DB_PO_HEADER& tDPH = dpfpu.tDPH;
	String strLogMode;
	MapToHex_64(dpfpu.tLogModeMap, strLogMode, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);

	m_strQuery = SPrintf("UPDATE po_fa_op_unit SET %s"
						", log_mode='%s', unit_type=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						strLogMode.c_str(), dpfpu.nUnitType,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnit::DeletePoFaOpUnit(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}

//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpUnit::ClearPoFaOpUnit()
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

INT32	CDBMgrPoFaOpUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaOpUnit* ptDBList = (TListDBPoFaOpUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_UNIT pd_t = (PDB_PO_FA_OP_UNIT)lpTemplet;

    return InsertPoFaOpUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_UNIT pd_t = (PDB_PO_FA_OP_UNIT)lpTemplet;

    return UpdatePoFaOpUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoFaOpUnit(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpUnit::ClearExecute()
{
	return ClearPoFaOpUnit();
}
//---------------------------------------------------------------------------





