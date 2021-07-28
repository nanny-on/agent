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
#include "DBMgrPoInVulnSecuUsbUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnSecuUsbUnit*		t_DBMgrPoInVulnSecuUsbUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnSecuUsbUnit::CDBMgrPoInVulnSecuUsbUnit()
{
	m_strDBTName = "po_in_vuln_secu_usb_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnSecuUsbUnit::~CDBMgrPoInVulnSecuUsbUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsbUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_secu_usb_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsbUnit::LoadDB(TListDBPoInVulnSecuUsbUnit& tDBPoInVulnSecuUsbUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_SECU_USB_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", usb_value, reg_value, process_value, file_value"
						" FROM po_in_vuln_secu_usb_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;
    
    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.strUsbValue			= GetDBField_String(nIndex++);
		data.strRegValue			= GetDBField_String(nIndex++);
		data.strProcess				= GetDBField_String(nIndex++);
		data.strFilePath			= GetDBField_String(nIndex++);
		
        tDBPoInVulnSecuUsbUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInVulnSecuUsbUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsbUnit::InsertPoInVulnQnaUnit(DB_PO_IN_VULN_SECU_USB_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_secu_usb_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", usb_value, reg_value, process_value, file_value"
						") VALUES (%s"
						", '%s', '%s', '%s', '%s'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.strUsbValue.c_str(), data.strRegValue.c_str(), data.strProcess.c_str(), data.strFilePath.c_str()
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnSecuUsbUnit::UpdatePoInVulnQnaUnit(DB_PO_IN_VULN_SECU_USB_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_secu_usb_unit SET %s"
						", usb_value='%s', reg_value='%s', process_value='%s', file_value='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.strUsbValue.c_str(), data.strRegValue.c_str(), data.strProcess.c_str(), data.strFilePath.c_str(),
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

INT32	CDBMgrPoInVulnSecuUsbUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnSecuUsbUnit* ptDBList = (TListDBPoInVulnSecuUsbUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------


INT32	CDBMgrPoInVulnSecuUsbUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SECU_USB_UNIT pd_t = (PDB_PO_IN_VULN_SECU_USB_UNIT)lpTemplet;

    return InsertPoInVulnQnaUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSecuUsbUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_SECU_USB_UNIT pd_t = (PDB_PO_IN_VULN_SECU_USB_UNIT)lpTemplet;

    return UpdatePoInVulnQnaUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnSecuUsbUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






