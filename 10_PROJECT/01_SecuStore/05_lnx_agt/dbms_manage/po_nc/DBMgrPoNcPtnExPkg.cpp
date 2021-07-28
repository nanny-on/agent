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
#include "DBMgrPoNcPtnExPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoNcPtnExPkg*		t_DBMgrPoNcPtnExPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoNcPtnExPkg::CDBMgrPoNcPtnExPkg()
{
	m_strDBTName = "po_nc_ptn_ex_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoNcPtnExPkg::~CDBMgrPoNcPtnExPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnExPkg::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_nc_ptn_ex_pkg][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnExPkg::LoadDB(TListDBPoNcPtnExPkg& tDBPoNcPtnExPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_NC_PTN_EX_PKG datap;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						", chk_order"
						" FROM po_nc_ptn_ex_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = datap.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);

		datap.nChkOrder				= GetDBField_Int(nIndex++);
		
        tDBPoNcPtnExPkgList.push_back(datap);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoNcPtnExPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnExPkg::InsertPoNcPtnExPkg(DB_PO_NC_PTN_EX_PKG& datap)
{
	DB_PO_HEADER& tDPH = datap.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_nc_ptn_ex_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						", chk_order"
						") VALUES (%s"
						", %u"
						");",
						GetPoPkgHDRQuery_InsertValue(tDPH).c_str(),
						datap.nChkOrder);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnExPkg::UpdatePoNcPtnExPkg(DB_PO_NC_PTN_EX_PKG& datap)
{
	DB_PO_HEADER& tDPH = datap.tDPH;

	m_strQuery = SPrintf("UPDATE po_nc_ptn_ex_pkg SET "
						"%s"
						", chk_order=%u"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
						datap.nChkOrder,
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

INT32	CDBMgrPoNcPtnExPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoNcPtnExPkg* ptDBList = (TListDBPoNcPtnExPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnExPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_EX_PKG pd_t = (PDB_PO_NC_PTN_EX_PKG)lpTemplet;

    return InsertPoNcPtnExPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnExPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_EX_PKG pd_t = (PDB_PO_NC_PTN_EX_PKG)lpTemplet;

    return UpdatePoNcPtnExPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnExPkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






