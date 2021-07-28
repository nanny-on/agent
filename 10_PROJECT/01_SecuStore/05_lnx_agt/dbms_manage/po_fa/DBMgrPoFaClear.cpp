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
#include "DBMgrPoFaClear.h"

//---------------------------------------------------------------------------

CDBMgrPoFaClear*		t_DBMgrPoFaClear = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaClear::CDBMgrPoFaClear()
{
	m_strDBTName = "po_fa_clear";
}
//---------------------------------------------------------------------------

CDBMgrPoFaClear::~CDBMgrPoFaClear()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaClear::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaClear::LoadDB(TListDBPoFaClear& tDBPoFaClearList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_CLEAR dpfc;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", mdel_def_scan_id"
						" FROM po_fa_clear WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfc.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfc.nMDelDefScanID		= GetDBField_UInt(nIndex++);
		
        tDBPoFaClearList.push_back(dpfc);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaClearList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaClear::InsertPoFaClear(DB_PO_FA_CLEAR& dpfc)
{
	DB_PO_HEADER& tDPH = dpfc.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_clear("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", mdel_def_scan_id"
						") VALUES (%s"
						", %u"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(),
						dpfc.nMDelDefScanID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaClear::UpdatePoFaClear(DB_PO_FA_CLEAR& dpfc)
{
	DB_PO_HEADER& tDPH = dpfc.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_clear SET %s"
						", mdel_def_scan_id=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfc.nMDelDefScanID,
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaClear::DeletePoFaClear(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}

INT32			CDBMgrPoFaClear::ClearPoFaClear()
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

INT32	CDBMgrPoFaClear::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaClear* ptDBList = (TListDBPoFaClear*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------


INT32	CDBMgrPoFaClear::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_CLEAR pd_t = (PDB_PO_FA_CLEAR)lpTemplet;

    return InsertPoFaClear(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaClear::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_CLEAR pd_t = (PDB_PO_FA_CLEAR)lpTemplet;

    return UpdatePoFaClear(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaClear::DeleteExecute(UINT32 nID)
{
	return DeletePoFaClear(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaClear::ClearExecute()
{
	return ClearPoFaClear();
}
//---------------------------------------------------------------------------





