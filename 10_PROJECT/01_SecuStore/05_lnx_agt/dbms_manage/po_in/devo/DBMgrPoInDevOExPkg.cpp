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
#include "DBMgrPoInDevOExPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoInDevOExPkg*		t_DBMgrPoInDevOExPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInDevOExPkg::CDBMgrPoInDevOExPkg()
{
	m_strDBTName = "po_in_devo_ex_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoInDevOExPkg::~CDBMgrPoInDevOExPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOExPkg::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_devo_ex_pkg][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOExPkg::LoadDB(TListDBPoInDevOExPkg& tDBPoInDevOExPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_DEVO_EX_PKG data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_in_devo_ex_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoInDevOExPkgList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInDevOExPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOExPkg::InsertPoInDevOExPkg(DB_PO_IN_DEVO_EX_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_devo_ex_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						") VALUES (%s"
						");",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

    tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOExPkg::UpdatePoInDevOExPkg(DB_PO_IN_DEVO_EX_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_devo_ex_pkg SET "
						"%s"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
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

INT32	CDBMgrPoInDevOExPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInDevOExPkg* ptDBList = (TListDBPoInDevOExPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOExPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_DEVO_EX_PKG pd_t = (PDB_PO_IN_DEVO_EX_PKG)lpTemplet;

    return InsertPoInDevOExPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOExPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_DEVO_EX_PKG pd_t = (PDB_PO_IN_DEVO_EX_PKG)lpTemplet;

    return UpdatePoInDevOExPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOExPkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






