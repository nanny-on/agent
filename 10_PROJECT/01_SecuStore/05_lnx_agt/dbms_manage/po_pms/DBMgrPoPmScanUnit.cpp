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
#include "DBMgrPoPmScanUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoPmScanUnit*		t_DBMgrPoPmScanUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoPmScanUnit::CDBMgrPoPmScanUnit()
{
	m_strDBTName = "po_pm_scan_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoPmScanUnit::~CDBMgrPoPmScanUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmScanUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_pm_scan_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmScanUnit::LoadDB(TListDBPoPmScanUnit& tDBPoPmScanUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_PM_SCAN_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", scan_type, scan_time, scan_svr_type"
						", scan_last_time"
						" FROM po_pm_scan_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;
    
    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nScanType				= GetDBField_UInt(nIndex++);
		data.nScanTime				= GetDBField_UInt64(nIndex++);
		data.nScanSvrType			= GetDBField_UInt(nIndex++);

		data.nScanLastTime			= GetDBField_UInt(nIndex++);
		
        tDBPoPmScanUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoPmScanUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmScanUnit::InsertPoPmScanUnit(DB_PO_PM_SCAN_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_pm_scan_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", scan_type, scan_time, scan_svr_type"
						", scan_last_time"
						") VALUES (%s"
						", %u, '%llu', %u"
						", %u"
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nScanType, data.nScanTime, data.nScanSvrType,
						data.nScanLastTime);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmScanUnit::UpdatePoPmScanUnit(DB_PO_PM_SCAN_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_pm_scan_unit SET %s"
						", scan_type=%u, scan_time='%llu', scan_svr_type=%u"
						", scan_last_time=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						data.nScanType,	data.nScanTime, data.nScanSvrType,
						data.nScanLastTime,	
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

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
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmScanUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoPmScanUnit* ptDBList = (TListDBPoPmScanUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmScanUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_PM_SCAN_UNIT pd_t = (PDB_PO_PM_SCAN_UNIT)lpTemplet;

    return InsertPoPmScanUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmScanUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_PM_SCAN_UNIT pd_t = (PDB_PO_PM_SCAN_UNIT)lpTemplet;

    return UpdatePoPmScanUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmScanUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






