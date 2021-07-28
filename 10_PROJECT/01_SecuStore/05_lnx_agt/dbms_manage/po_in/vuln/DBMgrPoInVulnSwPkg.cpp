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
#include "DBMgrPoInVulnSwPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnSwPkg*		t_DBMgrPoInVulnSwPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnSwPkg::CDBMgrPoInVulnSwPkg()
{
	m_strDBTName = "po_in_vuln_sw_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnSwPkg::~CDBMgrPoInVulnSwPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSwPkg::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_sw_pkg][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSwPkg::LoadDB(TListDBPoInVulnSwPkg& tDBPoInVulnSwPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_SW_PKG data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						", sw_chk_key, sw_version"
						" FROM po_in_vuln_sw_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);

		data.strSwChkKey			= GetDBField_String(nIndex++);
		data.strSwVersion			= GetDBField_String(nIndex++);
		
        tDBPoInVulnSwPkgList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInVulnSwPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSwPkg::InsertPoInVulnSwPkg(DB_PO_IN_VULN_SW_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_sw_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						", sw_chk_key, sw_version"
						") VALUES (%s"
						", '%s', '%s'"
						");",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str(),
						data.strSwChkKey.c_str(), data.strSwVersion.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSwPkg::UpdatePoInVulnSwPkg(DB_PO_IN_VULN_SW_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_sw_pkg SET %s"
						", sw_chk_key='%s', sw_version='%s'"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(),
						data.strSwChkKey.c_str(), data.strSwVersion.c_str(),
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

INT32	CDBMgrPoInVulnSwPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnSwPkg* ptDBList = (TListDBPoInVulnSwPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSwPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SW_PKG pd_t = (PDB_PO_IN_VULN_SW_PKG)lpTemplet;

    return InsertPoInVulnSwPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSwPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SW_PKG pd_t = (PDB_PO_IN_VULN_SW_PKG)lpTemplet;

    return UpdatePoInVulnSwPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSwPkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






