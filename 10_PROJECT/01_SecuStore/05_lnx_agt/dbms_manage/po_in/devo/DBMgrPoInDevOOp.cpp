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


#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrPoInDevOOp.h"

//---------------------------------------------------------------------------

CDBMgrPoInDevOOp*		t_DBMgrPoInDevOOp = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInDevOOp::CDBMgrPoInDevOOp()
{
	m_strDBTName = "po_in_devo_op";
}
//---------------------------------------------------------------------------

CDBMgrPoInDevOOp::~CDBMgrPoInDevOOp()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOOp::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_devo_op][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOOp::LoadDB(TListDBPoInDevOOp& tDBPoInDevOOpList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_DEVO_OP data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", block_mode"
						" FROM po_in_devo_op WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);

		data.nBlockMode					= GetDBField_UInt(nIndex++);
		
        tDBPoInDevOOpList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInDevOOpList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOOp::InsertPoInDevOOp(DB_PO_IN_DEVO_OP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_devo_op("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", block_mode"
						") VALUES (%s "
						", %u"						
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nBlockMode);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOOp::UpdatePoInDevOOp(DB_PO_IN_DEVO_OP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_devo_op SET %s"
						", block_mode=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nBlockMode,
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

INT32	CDBMgrPoInDevOOp::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInDevOOp* ptDBList = (TListDBPoInDevOOp*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOOp::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_DEVO_OP pd_t = (PDB_PO_IN_DEVO_OP)lpTemplet;

    return InsertPoInDevOOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOOp::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_DEVO_OP pd_t = (PDB_PO_IN_DEVO_OP)lpTemplet;

    return UpdatePoInDevOOp(*pd_t);
}
//------------------------------------------po_in_vuln_op---------------------------------

INT32	CDBMgrPoInDevOOp::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






