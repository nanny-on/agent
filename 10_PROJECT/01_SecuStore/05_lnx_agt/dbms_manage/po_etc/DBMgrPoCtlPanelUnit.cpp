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
#include "DBMgrPoCtlPanelUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoCtlPanelUnit*		t_DBMgrPoCtlPanelUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoCtlPanelUnit::CDBMgrPoCtlPanelUnit()
{
	m_strDBTName = "po_ctl_panel_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoCtlPanelUnit::~CDBMgrPoCtlPanelUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlPanelUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlPanelUnit::LoadDB(TListDBPoCtlPanelUnit& tDBPoCtlPanelUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_CTL_PANEL_UNIT dpcpu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", control_type, ctl_reg_info, ctl_file_info"
						" FROM po_ctl_panel_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpcpu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpcpu.nControlType			= GetDBField_UInt(nIndex++);
		dpcpu.strCtlRegInfo			= GetDBField_String(nIndex++);
		dpcpu.strCtlFileInfo		= GetDBField_String(nIndex++);
        
        tDBPoCtlPanelUnitList.push_back(dpcpu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBPoCtlPanelUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlPanelUnit::InsertPoCtlPanelUnit(DB_PO_CTL_PANEL_UNIT& dpcpu)
{
	DB_PO_HEADER& tDPH = dpcpu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_ctl_panel_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", control_type, ctl_reg_info, ctl_file_info"
						") VALUES (%s"
                        ", %u, '%s', '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpcpu.nControlType, dpcpu.strCtlRegInfo.c_str(), dpcpu.strCtlFileInfo.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlPanelUnit::UpdatePoCtlPanelUnit(DB_PO_CTL_PANEL_UNIT& dpcpu)
{
	DB_PO_HEADER& tDPH = dpcpu.tDPH;

	m_strQuery = SPrintf("UPDATE po_ctl_panel_unit SET %s"
						", control_type=%u, ctl_reg_info='%s', ctl_file_info='%s' "
						"WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dpcpu.nControlType, dpcpu.strCtlRegInfo.c_str(), dpcpu.strCtlFileInfo.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlPanelUnit::DeletePoCtlPanelUnit(UINT32 nID)
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

INT32	CDBMgrPoCtlPanelUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoCtlPanelUnit* ptDBList = (TListDBPoCtlPanelUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoCtlPanelUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_CTL_PANEL_UNIT pd_t = (PDB_PO_CTL_PANEL_UNIT)lpTemplet;

    return InsertPoCtlPanelUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoCtlPanelUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_CTL_PANEL_UNIT pd_t = (PDB_PO_CTL_PANEL_UNIT)lpTemplet;

    return UpdatePoCtlPanelUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoCtlPanelUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoCtlPanelUnit(nID);
}
//---------------------------------------------------------------------------






