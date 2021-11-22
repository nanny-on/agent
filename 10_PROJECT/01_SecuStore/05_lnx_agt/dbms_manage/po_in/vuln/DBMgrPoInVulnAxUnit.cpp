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
#include "DBMgrPoInVulnAxUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnAxUnit*		t_DBMgrPoInVulnAxUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnAxUnit::CDBMgrPoInVulnAxUnit()
{
	m_strDBTName = "po_in_vuln_ax_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnAxUnit::~CDBMgrPoInVulnAxUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnAxUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_ax_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnAxUnit::LoadDB(TListDBPoInVulnAxUnit& tDBPoInVulnAxUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_AX_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", ax_filter, ax_name, ax_clsid"
						", download_site, publisher, ax_path"
						" FROM po_in_vuln_ax_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nFilter				= GetDBField_Int(nIndex++);
		data.strName				= GetDBField_String(nIndex++);
		data.strClsId				= GetDBField_String(nIndex++);
		data.strDownloadedSite		= GetDBField_String(nIndex++);
		data.strPublisher			= GetDBField_String(nIndex++);
		data.strFileName			= GetDBField_String(nIndex++);
		
        tDBPoInVulnAxUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInVulnAxUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnAxUnit::InsertPoInVulnAxUnit(DB_PO_IN_VULN_AX_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_ax_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", ax_filter, ax_name, ax_clsid"
						", download_site, publisher, ax_path"
						") VALUES (%s"
						", %u, '%s', '%s'"
						", '%s', '%s', '%s'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nFilter, data.strName.c_str(), data.strClsId.c_str(),
						data.strDownloadedSite.c_str(), data.strPublisher.c_str(), data.strFileName.c_str()
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnAxUnit::UpdatePoInVulnAxUnit(DB_PO_IN_VULN_AX_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_ax_unit SET %s"
						", ax_filter=%u, ax_name='%s', ax_clsid='%s'"
						", download_site='%s', publisher='%s', ax_path='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nFilter, data.strName.c_str(), data.strClsId.c_str(),
						data.strDownloadedSite.c_str(), data.strPublisher.c_str(), data.strFileName.c_str(),
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

INT32	CDBMgrPoInVulnAxUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnAxUnit* ptDBList = (TListDBPoInVulnAxUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnAxUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_AX_UNIT pd_t = (PDB_PO_IN_VULN_AX_UNIT)lpTemplet;

    return InsertPoInVulnAxUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnAxUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_AX_UNIT pd_t = (PDB_PO_IN_VULN_AX_UNIT)lpTemplet;

    return UpdatePoInVulnAxUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnAxUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






