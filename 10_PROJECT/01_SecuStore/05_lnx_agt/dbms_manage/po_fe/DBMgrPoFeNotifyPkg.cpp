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
#include "DBMgrPoFeNotifyPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoFeNotifyPkg*		t_DBMgrPoFeNotifyPkg;

//---------------------------------------------------------------------------

CDBMgrPoFeNotifyPkg::CDBMgrPoFeNotifyPkg()
{
	m_strDBTName = "po_fe_notify_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoFeNotifyPkg::~CDBMgrPoFeNotifyPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeNotifyPkg::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeNotifyPkg::LoadDB(TListDBPoFeNotifyPkg& tDBPoFeNotifyPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FE_NOTIFY_PKG dpfnp;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_fe_notify_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfnp.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoFeNotifyPkgList.push_back(dpfnp);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFeNotifyPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeNotifyPkg::InsertPoFeNotifyPkg(DB_PO_FE_NOTIFY_PKG& dpfnp)
{
	DB_PO_HEADER& tDPH = dpfnp.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fe_notify_pkg("
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

INT32			CDBMgrPoFeNotifyPkg::UpdatePoFeNotifyPkg(DB_PO_FE_NOTIFY_PKG& dpfnp)
{
	DB_PO_HEADER& tDPH = dpfnp.tDPH;

	m_strQuery = SPrintf("UPDATE po_fe_notify_pkg SET "
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

INT32	CDBMgrPoFeNotifyPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFeNotifyPkg* ptDBList = (TListDBPoFeNotifyPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeNotifyPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FE_NOTIFY_PKG pd_t = (PDB_PO_FE_NOTIFY_PKG)lpTemplet;

    return InsertPoFeNotifyPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeNotifyPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FE_NOTIFY_PKG pd_t = (PDB_PO_FE_NOTIFY_PKG)lpTemplet;

    return UpdatePoFeNotifyPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeNotifyPkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






