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
#include "DBMgrPoInVulnSecuUsb.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnSecuUsb*		t_DBMgrPoInVulnSecuUsb = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnSecuUsb::CDBMgrPoInVulnSecuUsb()
{
	m_strDBTName = "po_in_vuln_secu_usb";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnSecuUsb::~CDBMgrPoInVulnSecuUsb()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsb::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_secu_usb][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsb::LoadDB(TListDBPoInVulnSecuUsb& tDBPoInVulnSecuUsbList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_SECU_USB data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						" FROM po_in_vuln_secu_usb WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
        tDBPoInVulnSecuUsbList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInVulnSecuUsbList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsb::InsertPoInVulnQna(DB_PO_IN_VULN_SECU_USB& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_secu_usb("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						") VALUES (%s"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsb::UpdatePoInVulnQna(DB_PO_IN_VULN_SECU_USB& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_secu_usb SET %s"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
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

INT32	CDBMgrPoInVulnSecuUsb::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnSecuUsb* ptDBList = (TListDBPoInVulnSecuUsb*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSecuUsb::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SECU_USB pd_t = (PDB_PO_IN_VULN_SECU_USB)lpTemplet;

    return InsertPoInVulnQna(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSecuUsb::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SECU_USB pd_t = (PDB_PO_IN_VULN_SECU_USB)lpTemplet;

    return UpdatePoInVulnQna(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSecuUsb::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






