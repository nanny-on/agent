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
#include "DBMgrPoPmExUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoPmExUnit*		t_DBMgrPoPmExUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoPmExUnit::CDBMgrPoPmExUnit()
{
	m_strDBTName = "po_pm_ex_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoPmExUnit::~CDBMgrPoPmExUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmExUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_pm_ex_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmExUnit::LoadDB(TListDBPoPmExUnit& tDBPoPmExUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_PM_EX_UNIT data;

    INT32 nIndex = 0;


	

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", ex_mode, patch_id"
						" FROM po_pm_ex_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nExMode				= GetDBField_UInt(nIndex++);
		
		data.strPatchID				= GetDBField_String(nIndex++);		HexToMap(data.strPatchID, data.tPatchIDMap, 0);
		
        tDBPoPmExUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoPmExUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmExUnit::InsertPoPmExUnit(DB_PO_PM_EX_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_pm_ex_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", ex_mode, patch_id"
						") VALUES (%s"
						", %u, '%s'"
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nExMode, data.strPatchID.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmExUnit::UpdatePoPmExUnit(DB_PO_PM_EX_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_pm_ex_unit SET %s"
						", ex_mode=%u, patch_id='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						data.nExMode, data.strPatchID.c_str(),
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

INT32	CDBMgrPoPmExUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoPmExUnit* ptDBList = (TListDBPoPmExUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmExUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_PM_EX_UNIT pd_t = (PDB_PO_PM_EX_UNIT)lpTemplet;

    return InsertPoPmExUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmExUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_PM_EX_UNIT pd_t = (PDB_PO_PM_EX_UNIT)lpTemplet;

    return UpdatePoPmExUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmExUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






