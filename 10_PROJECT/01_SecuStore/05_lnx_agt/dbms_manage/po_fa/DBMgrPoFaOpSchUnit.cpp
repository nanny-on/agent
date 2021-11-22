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
#include "DBMgrPoFaOpSchUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoFaOpSchUnit*		t_DBMgrPoFaOpSchUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaOpSchUnit::CDBMgrPoFaOpSchUnit()
{
	m_strDBTName = "po_fa_op_sch_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoFaOpSchUnit::~CDBMgrPoFaOpSchUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSchUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSchUnit::LoadDB(TListDBPoFaOpSchUnit& tDBPoFaOpSchUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_OP_SCH_UNIT dpfosu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", sch_mon, sch_day, sch_week, sch_hour, sch_min"
						" FROM po_fa_op_sch_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfosu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfosu.nSchMon				= GetDBField_UInt(nIndex++);
		dpfosu.nSchDay				= GetDBField_UInt(nIndex++);
		dpfosu.nSchWeek				= GetDBField_UInt(nIndex++);
		dpfosu.nSchHour				= GetDBField_UInt(nIndex++);
		dpfosu.nSchMin				= GetDBField_UInt(nIndex++);
		
        tDBPoFaOpSchUnitList.push_back(dpfosu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaOpSchUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSchUnit::InsertPoFaOpSchUnit(DB_PO_FA_OP_SCH_UNIT& dpfosu)
{
	DB_PO_HEADER& tDPH = dpfosu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_op_sch_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sch_mon, sch_day, sch_week, sch_hour, sch_min"
						") VALUES (%s"
                        ", %u, %u, %u, %u, %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpfosu.nSchMon, dpfosu.nSchDay, dpfosu.nSchWeek, dpfosu.nSchHour, dpfosu.nSchMin);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSchUnit::UpdatePoFaOpSchUnit(DB_PO_FA_OP_SCH_UNIT& dpfosu)
{
	DB_PO_HEADER& tDPH = dpfosu.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_op_sch_unit SET %s"
						", sch_mon=%u, sch_day=%u, sch_week=%u, sch_hour=%u, sch_min=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfosu.nSchMon, dpfosu.nSchDay, dpfosu.nSchWeek, dpfosu.nSchHour, dpfosu.nSchMin,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOpSchUnit::DeletePoFaOpSchUnit(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------  

INT32			CDBMgrPoFaOpSchUnit::ClearPoFaOpSchUnit()
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

INT32	CDBMgrPoFaOpSchUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaOpSchUnit* ptDBList = (TListDBPoFaOpSchUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpSchUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_SCH_UNIT pd_t = (PDB_PO_FA_OP_SCH_UNIT)lpTemplet;

    return InsertPoFaOpSchUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpSchUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP_SCH_UNIT pd_t = (PDB_PO_FA_OP_SCH_UNIT)lpTemplet;

    return UpdatePoFaOpSchUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpSchUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoFaOpSchUnit(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOpSchUnit::ClearExecute()
{
	return ClearPoFaOpSchUnit();
}
//---------------------------------------------------------------------------





