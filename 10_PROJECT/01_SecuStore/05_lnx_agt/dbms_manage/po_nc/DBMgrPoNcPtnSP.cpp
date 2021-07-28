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
#include "DBMgrPoNcPtnSP.h"

//---------------------------------------------------------------------------

CDBMgrPoNcPtnSP*		t_DBMgrPoNcPtnSP = NULL;

//---------------------------------------------------------------------------

CDBMgrPoNcPtnSP::CDBMgrPoNcPtnSP()
{
	m_strDBTName = "po_nc_ptn_sp";
}
//---------------------------------------------------------------------------

CDBMgrPoNcPtnSP::~CDBMgrPoNcPtnSP()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnSP::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_nc_ptn_sp][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnSP::LoadDB(TListDBPoNcPtnSP& tDBPoNcPtnSPList)
{
    UINT32 nReadCnt = 0;
    DB_PO_NC_PTN_SP data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", confirm_level, confirm_day"
						" FROM po_nc_ptn_sp WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
		data.nConfirmLevel			= GetDBField_UInt(nIndex++);
		data.nConfirmDay			= GetDBField_UInt(nIndex++);
		
        tDBPoNcPtnSPList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoNcPtnSPList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnSP::InsertPoNcPtnSP(DB_PO_NC_PTN_SP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_nc_ptn_sp("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", confirm_level, confirm_day"
						") VALUES (%s"
						", %u, %u"
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str(),
						data.nConfirmLevel, data.nConfirmDay);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnSP::UpdatePoNcPtnSP(DB_PO_NC_PTN_SP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_nc_ptn_sp SET %s"
						", confirm_level=%u, confirm_day=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nConfirmLevel, data.nConfirmDay,
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

INT32	CDBMgrPoNcPtnSP::LoadExecute(PVOID lpTempletList)
{
	TListDBPoNcPtnSP* ptDBList = (TListDBPoNcPtnSP*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnSP::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_SP pd_t = (PDB_PO_NC_PTN_SP)lpTemplet;

    return InsertPoNcPtnSP(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnSP::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_SP pd_t = (PDB_PO_NC_PTN_SP)lpTemplet;

    return UpdatePoNcPtnSP(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnSP::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






