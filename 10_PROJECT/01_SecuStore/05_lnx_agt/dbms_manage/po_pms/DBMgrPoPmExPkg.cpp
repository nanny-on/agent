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
#include "DBMgrPoPmExPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoPmExPkg*		t_DBMgrPoPmExPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoPmExPkg::CDBMgrPoPmExPkg()
{
	m_strDBTName = "po_pm_ex_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoPmExPkg::~CDBMgrPoPmExPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmExPkg::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_pm_ex_pkg][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmExPkg::LoadDB(TListDBPoPmExPkg& tDBPoPmExPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_PM_EX_PKG data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_pm_ex_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoPmExPkgList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoPmExPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmExPkg::InsertPoPmExPkg(DB_PO_PM_EX_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_pm_ex_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						") VALUES (%s"
						");",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmExPkg::UpdatePoPmExPkg(DB_PO_PM_EX_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_pm_ex_pkg SET "
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

INT32	CDBMgrPoPmExPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoPmExPkg* ptDBList = (TListDBPoPmExPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmExPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_PM_EX_PKG pd_t = (PDB_PO_PM_EX_PKG)lpTemplet;

    return InsertPoPmExPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmExPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_PM_EX_PKG pd_t = (PDB_PO_PM_EX_PKG)lpTemplet;

    return UpdatePoPmExPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmExPkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






