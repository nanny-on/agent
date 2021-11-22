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
#include "DBMgrPoFeSinglePtnUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoFeSinglePtnUnit*		t_DBMgrPoFeSinglePtnUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFeSinglePtnUnit::CDBMgrPoFeSinglePtnUnit()
{
	m_strDBTName = "po_fe_single_ptn_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoFeSinglePtnUnit::~CDBMgrPoFeSinglePtnUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeSinglePtnUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeSinglePtnUnit::LoadDB(TListDBPoFeSinglePtnUnit& tDBPoFeSinglePtnUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FE_SINGLE_PTN_UNIT dpfspu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", target_type, target_id, block_type, fe_key, pe_type, pe_bit, limit_time"
						" FROM po_fe_single_ptn_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfspu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfspu.nTargetType			= GetDBField_UInt(nIndex++);
		dpfspu.nTargetID			= GetDBField_UInt(nIndex++);
		dpfspu.nBlockType			= GetDBField_UInt(nIndex++);
		dpfspu.strFeKey				= GetDBField_String(nIndex++);
		dpfspu.nPeType				= GetDBField_Int(nIndex++) ;
		dpfspu.nPeBit				= GetDBField_Int(nIndex++) ;
		dpfspu.nLimitTime			= GetDBField_UInt(nIndex++);
        
        tDBPoFeSinglePtnUnitList.push_back(dpfspu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFeSinglePtnUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeSinglePtnUnit::InsertPoFeSinglePtnUnit(DB_PO_FE_SINGLE_PTN_UNIT& dpfspu)
{
	DB_PO_HEADER& tDPH = dpfspu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fe_single_ptn_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", target_type, target_id, block_type, fe_key, pe_type, pe_bit, limit_time"
						") VALUES (%s"
                        ", %u, %u, %u, '%s', %u, %u, %u"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpfspu.nTargetType, dpfspu.nTargetID, dpfspu.nBlockType, 
						dpfspu.strFeKey.c_str(), dpfspu.nPeType, dpfspu.nPeBit, dpfspu.nLimitTime);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeSinglePtnUnit::UpdatePoFeSinglePtnUnit(DB_PO_FE_SINGLE_PTN_UNIT& dpfspu)
{
	DB_PO_HEADER& tDPH = dpfspu.tDPH;

	m_strQuery = SPrintf("UPDATE po_fe_single_ptn_unit SET %s"
						", target_type=%u, target_id=%u, block_type=%u, fe_key='%s', pe_type=%u, pe_bit=%u, limit_time=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dpfspu.nTargetType, dpfspu.nTargetID, dpfspu.nBlockType, 
						dpfspu.strFeKey.c_str(), dpfspu.nPeType, dpfspu.nPeBit, dpfspu.nLimitTime,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

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

INT32	CDBMgrPoFeSinglePtnUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFeSinglePtnUnit* ptDBList = (TListDBPoFeSinglePtnUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeSinglePtnUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FE_SINGLE_PTN_UNIT pd_t = (PDB_PO_FE_SINGLE_PTN_UNIT)lpTemplet;

    return InsertPoFeSinglePtnUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeSinglePtnUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FE_SINGLE_PTN_UNIT pd_t = (PDB_PO_FE_SINGLE_PTN_UNIT)lpTemplet;

    return UpdatePoFeSinglePtnUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeSinglePtnUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






