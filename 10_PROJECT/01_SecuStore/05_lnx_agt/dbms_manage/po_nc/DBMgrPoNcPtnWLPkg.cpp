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
#include "DBMgrPoNcPtnWLPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoNcPtnWLPkg*		t_DBMgrPoNcPtnWLPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoNcPtnWLPkg::CDBMgrPoNcPtnWLPkg()
{
	m_strDBTName = "po_nc_ptn_wl_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoNcPtnWLPkg::~CDBMgrPoNcPtnWLPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnWLPkg::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_nc_ptn_wl_pkg][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnWLPkg::LoadDB(TListDBPoNcPtnWLPkg& tDBPoNcPtnWLPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_NC_PTN_WL_PKG data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_nc_ptn_wl_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;
	do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoNcPtnWLPkgList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoNcPtnWLPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnWLPkg::InsertPoNcPtnWLPkg(DB_PO_NC_PTN_WL_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_nc_ptn_wl_pkg("
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

INT32			CDBMgrPoNcPtnWLPkg::UpdatePoNcPtnWLPkg(DB_PO_NC_PTN_WL_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_nc_ptn_wl_pkg SET "
						"%s"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnWLPkg::DeletePoNcPtnWLPkg(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
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

INT32	CDBMgrPoNcPtnWLPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoNcPtnWLPkg* ptDWList = (TListDBPoNcPtnWLPkg*)lpTempletList;

    return LoadDB(*ptDWList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnWLPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_WL_PKG pd_t = (PDB_PO_NC_PTN_WL_PKG)lpTemplet;

    return InsertPoNcPtnWLPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnWLPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_WL_PKG pd_t = (PDB_PO_NC_PTN_WL_PKG)lpTemplet;

    return UpdatePoNcPtnWLPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnWLPkg::DeleteExecute(UINT32 nID)
{
	return DeletePoNcPtnWLPkg(nID);
}
//---------------------------------------------------------------------------






