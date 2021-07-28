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
#include "DBMgrPoSchPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoSchPkg*		t_DBMgrPoSchPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoSchPkg::CDBMgrPoSchPkg()
{
	m_strDBTName = "po_host_po_sch_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoSchPkg::~CDBMgrPoSchPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSchPkg::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSchPkg::LoadDB(TListDBPoSchPkg& tDBPoSchPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_SCH_PKG dphpsp;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						", scan_last_time FROM po_sch_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dphpsp.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		dphpsp.nScanLastTime		= GetDBField_UInt(nIndex++);
		
        tDBPoSchPkgList.push_back(dphpsp);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoSchPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSchPkg::InsertPoSchPkg(DB_PO_SCH_PKG& dphpsp)
{
	DB_PO_HEADER& tDPH = dphpsp.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_sch_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						", scan_last_time) VALUES (%s"
						", %u);",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str(),
						dphpsp.nScanLastTime);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSchPkg::UpdatePoSchPkg(DB_PO_SCH_PKG& dphpsp)
{
	DB_PO_HEADER& tDPH = dphpsp.tDPH;

	m_strQuery = SPrintf("UPDATE po_sch_pkg SET %s"
						", scan_last_time=%u"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(),
						dphpsp.nScanLastTime,
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
//---------------------------------------------------------------------------

INT32	CDBMgrPoSchPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoSchPkg* ptDBList = (TListDBPoSchPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSchPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_SCH_PKG pd_t = (PDB_PO_SCH_PKG)lpTemplet;

    return InsertPoSchPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSchPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_SCH_PKG pd_t = (PDB_PO_SCH_PKG)lpTemplet;

    return UpdatePoSchPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSchPkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






