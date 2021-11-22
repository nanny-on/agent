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
#include "DBMgrPoPmDmUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoPmDmUnit*		t_DBMgrPoPmDmUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoPmDmUnit::CDBMgrPoPmDmUnit()
{
	m_strDBTName = "po_pm_dm_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoPmDmUnit::~CDBMgrPoPmDmUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmDmUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_pm_dm_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmDmUnit::LoadDB(TListDBPoPmDmUnit& tDBPoPmDmUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_PM_DM_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", apply_type, notify_id, patch_id"
						" FROM po_pm_dm_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nApplyType				= GetDBField_UInt(nIndex++);
		data.nNotifyID				= GetDBField_UInt(nIndex++);
		data.strPatchID				= GetDBField_String(nIndex++);		HexToMap(data.strPatchID, data.tPatchIDMap, 0);
	
        tDBPoPmDmUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoPmDmUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmDmUnit::InsertPoPmDmUnit(DB_PO_PM_DM_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_pm_dm_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", apply_type, notify_id, patch_id"
						") VALUES (%s"
						", %u, %u, '%s'"
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nApplyType, data.nNotifyID, data.strPatchID.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmDmUnit::UpdatePoPmDmUnit(DB_PO_PM_DM_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_pm_dm_unit SET %s"
						", apply_type=%u, notify_id=%u, patch_id='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						data.nApplyType,	data.nNotifyID, data.strPatchID.c_str(),
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

INT32	CDBMgrPoPmDmUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoPmDmUnit* ptDBList = (TListDBPoPmDmUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmDmUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_PM_DM_UNIT pd_t = (PDB_PO_PM_DM_UNIT)lpTemplet;

    return InsertPoPmDmUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmDmUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_PM_DM_UNIT pd_t = (PDB_PO_PM_DM_UNIT)lpTemplet;

    return UpdatePoPmDmUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmDmUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






