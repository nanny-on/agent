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
#include "DBMgrPoNcPtnOp.h"

//---------------------------------------------------------------------------

CDBMgrPoNcPtnOp*		t_DBMgrPoNcPtnOp = NULL;

//---------------------------------------------------------------------------

CDBMgrPoNcPtnOp::CDBMgrPoNcPtnOp()
{
	m_strDBTName = "po_nc_ptn_op";
}
//---------------------------------------------------------------------------

CDBMgrPoNcPtnOp::~CDBMgrPoNcPtnOp()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnOp::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_nc_ptn_op][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnOp::LoadDB(TListDBPoNcPtnOp& tDBPoNcPtnOpList)
{
    UINT32 nReadCnt = 0;
    DB_PO_NC_PTN_OP data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", block_mode, kill_mode, rt_fg_mode"		
						" FROM po_nc_ptn_op WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH							= GetDBField_PoHDR(nIndex);

		data.nBlockMode					= GetDBField_UInt(nIndex++);
		data.nKillMode					= GetDBField_UInt(nIndex++);
		data.nRTFGMode					= GetDBField_UInt(nIndex++);
        
        tDBPoNcPtnOpList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoNcPtnOpList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnOp::InsertPoNcPtnOp(DB_PO_NC_PTN_OP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_nc_ptn_op("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", block_mode, kill_mode, rt_fg_mode"
						") VALUES (%s "
						", %u, %u, %u"						
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nBlockMode, data.nKillMode, data.nRTFGMode);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnOp::UpdatePoNcPtnOp(DB_PO_NC_PTN_OP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_nc_ptn_op SET %s"
						", block_mode=%u, kill_mode=%u, rt_fg_mode=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nBlockMode, data.nKillMode, data.nRTFGMode,
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

INT32	CDBMgrPoNcPtnOp::LoadExecute(PVOID lpTempletList)
{
	TListDBPoNcPtnOp* ptDBList = (TListDBPoNcPtnOp*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnOp::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_OP pd_t = (PDB_PO_NC_PTN_OP)lpTemplet;

    return InsertPoNcPtnOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnOp::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_OP pd_t = (PDB_PO_NC_PTN_OP)lpTemplet;

    return UpdatePoNcPtnOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnOp::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






