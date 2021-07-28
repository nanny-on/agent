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
#include "DBMgrPoFaOpSubUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoFaOpSubUnit*		t_DBMgrPoFaOpSubUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaOpSubUnit::CDBMgrPoFaOpSubUnit()
{
	m_strDBTName = "po_fa_op_sub_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoFaOpSubUnit::~CDBMgrPoFaOpSubUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSubUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSubUnit::LoadDB(TListDBPoFaOpSubUnit& tDBPoFaOpSubUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_OP_SUB_UNIT dpfosu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", sb_type, sb_link_id, sb_unicode, sb_domain, sb_name, sb_proc, log_mode"
						" FROM po_fa_op_sub_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfosu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfosu.nSbType				= GetDBField_UInt(nIndex++);
		dpfosu.nSbLinkID			= GetDBField_UInt(nIndex++);
		dpfosu.strSbUnicode			= GetDBField_String(nIndex++);
		dpfosu.strSbDomain			= GetDBField_String(nIndex++);
		dpfosu.strSbName			= GetDBField_String(nIndex++);
		dpfosu.strSbProc			= GetDBField_String(nIndex++);
		m_strValue					= GetDBField_String(nIndex++);
		HexToMap_64(m_strValue, dpfosu.tLogModeMap, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);
		
        tDBPoFaOpSubUnitList.push_back(dpfosu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaOpSubUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSubUnit::InsertPoFaOpSubUnit(DB_PO_FA_OP_SUB_UNIT& dpfosu)
{
	DB_PO_HEADER& tDPH = dpfosu.tDPH;
	String strLogMode;
	MapToHex_64(dpfosu.tLogModeMap, strLogMode, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);

	m_strQuery = SPrintf("INSERT INTO po_fa_op_sub_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sb_type, sb_link_id, sb_unicode, sb_domain, sb_name, sb_proc, log_mode"
						") VALUES (%s"
                        ", %u, %u, '%s', '%s', '%s', '%s', '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpfosu.nSbType, dpfosu.nSbLinkID, dpfosu.strSbUnicode.c_str(), dpfosu.strSbDomain.c_str(), 
						dpfosu.strSbName.c_str(), dpfosu.strSbProc.c_str(), strLogMode.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSubUnit::UpdatePoFaOpSubUnit(DB_PO_FA_OP_SUB_UNIT& dpfosu)
{
	DB_PO_HEADER& tDPH = dpfosu.tDPH;
	String strLogMode;
	MapToHex_64(dpfosu.tLogModeMap, strLogMode, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);

	m_strQuery = SPrintf("UPDATE po_fa_op_sub_unit SET %s"
						", sb_type=%u, sb_link_id=%u, sb_unicode='%s', sb_domain='%s', sb_name='%s', sb_proc='%s', log_mode='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfosu.nSbType, dpfosu.nSbLinkID, dpfosu.strSbUnicode.c_str(), dpfosu.strSbDomain.c_str(),
						dpfosu.strSbName.c_str(), dpfosu.strSbProc.c_str(), strLogMode.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSubUnit::DeletePoFaOpSubUnit(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSubUnit::ClearPoFaOpSubUnit()
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

INT32	CDBMgrPoFaOpSubUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaOpSubUnit* ptDBList = (TListDBPoFaOpSubUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpSubUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_SUB_UNIT pd_t = (PDB_PO_FA_OP_SUB_UNIT)lpTemplet;

    return InsertPoFaOpSubUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpSubUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_SUB_UNIT pd_t = (PDB_PO_FA_OP_SUB_UNIT)lpTemplet;

    return UpdatePoFaOpSubUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpSubUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoFaOpSubUnit(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpSubUnit::ClearExecute()
{
	return ClearPoFaOpSubUnit();
}
//---------------------------------------------------------------------------





