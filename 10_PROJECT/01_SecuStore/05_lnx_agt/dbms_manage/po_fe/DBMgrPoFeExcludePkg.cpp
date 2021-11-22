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
#include "DBMgrPoFeExcludePkg.h"

//---------------------------------------------------------------------------

CDBMgrPoFeExcludePkg*		t_DBMgrPoFeExcludePkg;

//---------------------------------------------------------------------------

CDBMgrPoFeExcludePkg::CDBMgrPoFeExcludePkg()
{
	m_strDBTName = "po_fe_exclude_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoFeExcludePkg::~CDBMgrPoFeExcludePkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeExcludePkg::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeExcludePkg::LoadDB(TListDBPoFeExcludePkg& tDBPoFeExcludePkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FE_EXCLUDE_PKG dpfep;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_fe_exclude_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfep.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoFeExcludePkgList.push_back(dpfep);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFeExcludePkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeExcludePkg::InsertPoFeExcludePkg(DB_PO_FE_EXCLUDE_PKG& dpfep)
{
	DB_PO_HEADER& tDPH = dpfep.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fe_exclude_pkg("
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

INT32			CDBMgrPoFeExcludePkg::UpdatePoFeExcludePkg(DB_PO_FE_EXCLUDE_PKG& dpfep)
{
	DB_PO_HEADER& tDPH = dpfep.tDPH;

	m_strQuery = SPrintf("UPDATE po_fe_exclude_pkg SET "
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

INT32	CDBMgrPoFeExcludePkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFeExcludePkg* ptDBList = (TListDBPoFeExcludePkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeExcludePkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FE_EXCLUDE_PKG pd_t = (PDB_PO_FE_EXCLUDE_PKG)lpTemplet;

    return InsertPoFeExcludePkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeExcludePkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FE_EXCLUDE_PKG pd_t = (PDB_PO_FE_EXCLUDE_PKG)lpTemplet;

    return UpdatePoFeExcludePkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeExcludePkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






