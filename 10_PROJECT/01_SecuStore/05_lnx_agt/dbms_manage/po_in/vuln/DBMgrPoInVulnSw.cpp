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
#include "DBMgrPoInVulnSw.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnSw*		t_DBMgrPoInVulnSw = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnSw::CDBMgrPoInVulnSw()
{
	m_strDBTName = "po_in_vuln_sw";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnSw::~CDBMgrPoInVulnSw()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSw::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_sw][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSw::LoadDB(TListDBPoInVulnSw& tDBPoInVulnSwList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_SW data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						" FROM po_in_vuln_sw WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
        tDBPoInVulnSwList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInVulnSwList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSw::InsertPoInVulnSw(DB_PO_IN_VULN_SW& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_sw("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						") VALUES (%s"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSw::UpdatePoInVulnSw(DB_PO_IN_VULN_SW& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_sw SET %s"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
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

INT32	CDBMgrPoInVulnSw::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnSw* ptDBList = (TListDBPoInVulnSw*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSw::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SW pd_t = (PDB_PO_IN_VULN_SW)lpTemplet;

    return InsertPoInVulnSw(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSw::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SW pd_t = (PDB_PO_IN_VULN_SW)lpTemplet;

    return UpdatePoInVulnSw(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSw::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






