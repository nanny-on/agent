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
#include "DBMgrPoPowerUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoPowerUnit*		t_DBMgrPoPowerUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoPowerUnit::CDBMgrPoPowerUnit()
{
	m_strDBTName = "po_power_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoPowerUnit::~CDBMgrPoPowerUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPowerUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPowerUnit::LoadDB(TListDBPoPowerUnit& tDBPoPowerUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_POWER_UNIT dppu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", sc_period_type, sc_start_time, sc_end_time, ctl_mode, ctl_wait_time"
						" FROM po_power_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dppu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dppu.nScPeriodType			= GetDBField_UInt(nIndex++);
		dppu.nScStartTime			= GetDBField_UInt(nIndex++);
		dppu.nScEndTime				= GetDBField_UInt(nIndex++);
		dppu.nCtlMode				= GetDBField_UInt(nIndex++);
		dppu.nCtlWaitTime			= GetDBField_UInt(nIndex++);
		
        tDBPoPowerUnitList.push_back(dppu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoPowerUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPowerUnit::InsertPoPowerUnit(DB_PO_POWER_UNIT& dppu)
{
	DB_PO_HEADER& tDPH = dppu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_power_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sc_period_type, sc_start_time, sc_end_time, ctl_mode, ctl_wait_time"
						") VALUES (%s"
                        ", %u, %u, %u, %u, %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dppu.nScPeriodType, dppu.nScStartTime, dppu.nScEndTime, dppu.nCtlMode, dppu.nCtlWaitTime);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPowerUnit::UpdatePoPowerUnit(DB_PO_POWER_UNIT& dppu)
{
	DB_PO_HEADER& tDPH = dppu.tDPH;

	m_strQuery = SPrintf("UPDATE po_power_unit SET %s"
						", sc_period_type=%u, sc_start_time=%u, sc_end_time=%u, ctl_mode=%u, ctl_wait_time=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dppu.nScPeriodType, dppu.nScStartTime, dppu.nScEndTime, dppu.nCtlMode, dppu.nCtlWaitTime,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPowerUnit::DeletePoPowerUnit(UINT32 nID)
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

INT32	CDBMgrPoPowerUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoPowerUnit* ptDBList = (TListDBPoPowerUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPowerUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_POWER_UNIT pd_t = (PDB_PO_POWER_UNIT)lpTemplet;

    return InsertPoPowerUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPowerUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_POWER_UNIT pd_t = (PDB_PO_POWER_UNIT)lpTemplet;

    return UpdatePoPowerUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPowerUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoPowerUnit(nID);
}
//---------------------------------------------------------------------------






