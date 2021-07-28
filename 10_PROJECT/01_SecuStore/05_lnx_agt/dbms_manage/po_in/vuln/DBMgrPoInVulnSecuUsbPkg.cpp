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
#include "DBMgrPoInVulnSecuUsbPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnSecuUsbPkg*		t_DBMgrPoInVulnSecuUsbPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnSecuUsbPkg::CDBMgrPoInVulnSecuUsbPkg()
{
	m_strDBTName = "po_in_vuln_secu_usb_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnSecuUsbPkg::~CDBMgrPoInVulnSecuUsbPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsbPkg::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_secu_usb_pkg][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsbPkg::LoadDB(TListDBPoInVulnSecuUsbPkg& tDBPoInVulnSecuUsbPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_SECU_USB_PKG data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_in_vuln_secu_usb_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoInVulnSecuUsbPkgList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInVulnSecuUsbPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsbPkg::InsertPoInVulnQnaPkg(DB_PO_IN_VULN_SECU_USB_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_secu_usb_pkg("
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

INT32			CDBMgrPoInVulnSecuUsbPkg::UpdatePoInVulnQnaPkg(DB_PO_IN_VULN_SECU_USB_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_secu_usb_pkg SET "
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

INT32	CDBMgrPoInVulnSecuUsbPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnSecuUsbPkg* ptDBList = (TListDBPoInVulnSecuUsbPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------


INT32	CDBMgrPoInVulnSecuUsbPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SECU_USB_PKG pd_t = (PDB_PO_IN_VULN_SECU_USB_PKG)lpTemplet;

    return InsertPoInVulnQnaPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSecuUsbPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SECU_USB_PKG pd_t = (PDB_PO_IN_VULN_SECU_USB_PKG)lpTemplet;

    return UpdatePoInVulnQnaPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSecuUsbPkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






