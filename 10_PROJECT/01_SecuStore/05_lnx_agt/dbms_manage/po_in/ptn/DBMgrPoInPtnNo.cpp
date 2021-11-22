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
#include "DBMgrPoInPtnNo.h"

//---------------------------------------------------------------------------

CDBMgrPoInPtnNo*		t_DBMgrPoInPtnNo = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInPtnNo::CDBMgrPoInPtnNo()
{
	m_strDBTName = "po_in_ptn_no";
}
//---------------------------------------------------------------------------

CDBMgrPoInPtnNo::~CDBMgrPoInPtnNo()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnNo::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnNo::LoadDB(TListDBPoInPtnNo& tDBPoInPtnNoList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_PTN_NO data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", msg_type "
						" FROM po_in_ptn_no WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);
		data.nMsgType			= GetDBField_UInt(nIndex++);
		
        tDBPoInPtnNoList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInPtnNoList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnNo::InsertPoInPtnNo(DB_PO_IN_PTN_NO& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_ptn_no("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", msg_type"
						") VALUES (%s"
						", %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(),
						data.nMsgType);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnNo::UpdatePoInPtnNo(DB_PO_IN_PTN_NO& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_ptn_no SET %s"
						", msg_type=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nMsgType,
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

INT32	CDBMgrPoInPtnNo::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInPtnNo* ptDBList = (TListDBPoInPtnNo*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnNo::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_NO pd_t = (PDB_PO_IN_PTN_NO)lpTemplet;

    return InsertPoInPtnNo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnNo::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_NO pd_t = (PDB_PO_IN_PTN_NO)lpTemplet;

    return UpdatePoInPtnNo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnNo::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






