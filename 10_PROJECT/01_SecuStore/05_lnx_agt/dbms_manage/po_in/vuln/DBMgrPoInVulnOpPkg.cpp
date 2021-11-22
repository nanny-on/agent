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
#include "DBMgrPoInVulnOpPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnOpPkg*		t_DBMgrPoInVulnOpPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnOpPkg::CDBMgrPoInVulnOpPkg()
{
	m_strDBTName = "po_in_vuln_op_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnOpPkg::~CDBMgrPoInVulnOpPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnOpPkg::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnOpPkg::LoadDB(TListDBPoInVulnOpPkg& tDBPoInVulnOpPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_OP_PKG data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						", site_score"
						" FROM po_in_vuln_op_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		data.nScore					= GetDBField_UInt(nIndex);
		
        tDBPoInVulnOpPkgList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInVulnOpPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnOpPkg::InsertPoInVulnOpPkg(DB_PO_IN_VULN_OP_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_op_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						", site_score"
						") VALUES (%s"
						", %u"
						");",
						GetPoPkgHDRQuery_InsertValue(tDPH).c_str(),
						data.nScore);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnOpPkg::UpdatePoInVulnOpPkg(DB_PO_IN_VULN_OP_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_op_pkg SET "
						"%s"
						", site_score=%u"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
						data.nScore,
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

INT32	CDBMgrPoInVulnOpPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnOpPkg* ptDBList = (TListDBPoInVulnOpPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnOpPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_OP_PKG pd_t = (PDB_PO_IN_VULN_OP_PKG)lpTemplet;

    return InsertPoInVulnOpPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnOpPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_OP_PKG pd_t = (PDB_PO_IN_VULN_OP_PKG)lpTemplet;

    return UpdatePoInVulnOpPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnOpPkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






