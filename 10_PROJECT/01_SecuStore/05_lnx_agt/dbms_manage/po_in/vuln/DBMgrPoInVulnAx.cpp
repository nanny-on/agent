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

#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrPoInVulnAx.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnAx*		t_DBMgrPoInVulnAx = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnAx::CDBMgrPoInVulnAx()
{
	m_strDBTName = "po_in_vuln_ax";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnAx::~CDBMgrPoInVulnAx()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnAx::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_ax][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnAx::LoadDB(TListDBPoInVulnAx& tDBPoInVulnAxList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_AX data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", unuse_day"
						" FROM po_in_vuln_ax WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
		data.nUnUseDay				= GetDBField_Int(nIndex++);

        tDBPoInVulnAxList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInVulnAxList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnAx::InsertPoInVulnAx(DB_PO_IN_VULN_AX& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_ax("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", unuse_day"
						") VALUES (%s"
						", %u"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(),
						data.nUnUseDay);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnAx::UpdatePoInVulnAx(DB_PO_IN_VULN_AX& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_ax SET %s"
						", unuse_day=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nUnUseDay,
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

INT32	CDBMgrPoInVulnAx::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnAx* ptDBList = (TListDBPoInVulnAx*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnAx::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_AX pd_t = (PDB_PO_IN_VULN_AX)lpTemplet;

    return InsertPoInVulnAx(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnAx::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_AX pd_t = (PDB_PO_IN_VULN_AX)lpTemplet;

    return UpdatePoInVulnAx(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnAx::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






