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
#include "DBMgrPoInVulnScanPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnScanPkg*		t_DBMgrPoInVulnScanPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnScanPkg::CDBMgrPoInVulnScanPkg()
{
	m_strDBTName = "po_in_vuln_scan_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnScanPkg::~CDBMgrPoInVulnScanPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnScanPkg::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnScanPkg::LoadDB(TListDBPoInVulnScanPkg& tDBPoInVulnScanPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_SCAN_PKG datap;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_in_vuln_scan_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = datap.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoInVulnScanPkgList.push_back(datap);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInVulnScanPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnScanPkg::InsertPoInVulnScanPkg(DB_PO_IN_VULN_SCAN_PKG& datap)
{
	DB_PO_HEADER& tDPH = datap.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_scan_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						") VALUES (%s"
						");",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnScanPkg::UpdatePoInVulnScanPkg(DB_PO_IN_VULN_SCAN_PKG& datap)
{
	DB_PO_HEADER& tDPH = datap.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_scan_pkg SET "
						"%s"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
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

INT32	CDBMgrPoInVulnScanPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnScanPkg* ptDBList = (TListDBPoInVulnScanPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnScanPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SCAN_PKG pd_t = (PDB_PO_IN_VULN_SCAN_PKG)lpTemplet;

    return InsertPoInVulnScanPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnScanPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SCAN_PKG pd_t = (PDB_PO_IN_VULN_SCAN_PKG)lpTemplet;

    return UpdatePoInVulnScanPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnScanPkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






