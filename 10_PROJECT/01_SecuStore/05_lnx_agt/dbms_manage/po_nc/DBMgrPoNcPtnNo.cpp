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
#include "DBMgrPoNcPtnNo.h"

//---------------------------------------------------------------------------

CDBMgrPoNcPtnNo*		t_DBMgrPoNcPtnNo = NULL;

//---------------------------------------------------------------------------

CDBMgrPoNcPtnNo::CDBMgrPoNcPtnNo()
{
	m_strDBTName = "po_nc_ptn_no";
}
//---------------------------------------------------------------------------

CDBMgrPoNcPtnNo::~CDBMgrPoNcPtnNo()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnNo::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_nc_ptn_no][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnNo::LoadDB(TListDBPoNcPtnNo& tDBPoNcPtnNoList)
{
    UINT32 nReadCnt = 0;
    DB_PO_NC_PTN_NO data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", msg_type, req_skip_opt "
						" FROM po_nc_ptn_no WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

	do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);
		data.nMsgType			= GetDBField_UInt(nIndex++);
		data.nReqSkipOpt		= GetDBField_UInt(nIndex++);
		
        tDBPoNcPtnNoList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoNcPtnNoList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnNo::InsertPoNcPtnNo(DB_PO_NC_PTN_NO& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_nc_ptn_no("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", msg_type, req_skip_opt"
						") VALUES (%s"
						", %u, %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(),
						data.nMsgType, data.nReqSkipOpt);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnNo::UpdatePoNcPtnNo(DB_PO_NC_PTN_NO& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_nc_ptn_no SET %s"
						", msg_type=%u, req_skip_opt=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nMsgType, data.nReqSkipOpt,
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

INT32	CDBMgrPoNcPtnNo::LoadExecute(PVOID lpTempletList)
{
	TListDBPoNcPtnNo* ptDBList = (TListDBPoNcPtnNo*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnNo::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_NO pd_t = (PDB_PO_NC_PTN_NO)lpTemplet;

    return InsertPoNcPtnNo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnNo::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_NO pd_t = (PDB_PO_NC_PTN_NO)lpTemplet;

    return UpdatePoNcPtnNo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnNo::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






