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

#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrPoInDevOExUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInDevOExUnit*		t_DBMgrPoInDevOExUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInDevOExUnit::CDBMgrPoInDevOExUnit()
{
	m_strDBTName = "po_in_devo_ex_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInDevOExUnit::~CDBMgrPoInDevOExUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOExUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_devo_ex_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOExUnit::LoadDB(TListDBPoInDevOExUnit& tDBPoInDevOExUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_DEVO_EX_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", block_type, exclude_ptn"
						" FROM po_in_devo_ex_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nBlockType				= GetDBField_UInt(nIndex++);
		data.nExPtn					= GetDBField_UInt(nIndex++);
		
        tDBPoInDevOExUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInDevOExUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOExUnit::InsertPoInDevOExUnit(DB_PO_IN_DEVO_EX_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_devo_ex_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", block_type, exclude_ptn"
						") VALUES (%s"
                        ", %u, %u"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nBlockType, data.nExPtn);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOExUnit::UpdatePoInDevOExUnit(DB_PO_IN_DEVO_EX_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_devo_ex_unit SET %s"
						", block_type=%u, exclude_ptn=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nBlockType, data.nExPtn,
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

INT32	CDBMgrPoInDevOExUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInDevOExUnit* ptDBList = (TListDBPoInDevOExUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOExUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_DEVO_EX_UNIT pd_t = (PDB_PO_IN_DEVO_EX_UNIT)lpTemplet;

    return InsertPoInDevOExUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOExUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_DEVO_EX_UNIT pd_t = (PDB_PO_IN_DEVO_EX_UNIT)lpTemplet;

    return UpdatePoInDevOExUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOExUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






