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
#include "DBMgrPoFaNotify.h"

//---------------------------------------------------------------------------

CDBMgrPoFaNotify*		t_DBMgrPoFaNotify = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaNotify::CDBMgrPoFaNotify()
{
	m_strDBTName = "po_fa_notify";
}
//---------------------------------------------------------------------------

CDBMgrPoFaNotify::~CDBMgrPoFaNotify()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaNotify::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaNotify::LoadDB(TListDBPoFaNotify& tDBPoFaNotifyList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_NOTIFY dpfn;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", msg_type "
						" FROM po_fa_notify WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfn.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);
		dpfn.nMsgType			= GetDBField_UInt(nIndex++);
		
        tDBPoFaNotifyList.push_back(dpfn);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaNotifyList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaNotify::InsertPoFaNotify(DB_PO_FA_NOTIFY& dpfn)
{
	DB_PO_HEADER& tDPH = dpfn.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_notify("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", msg_type"
						") VALUES (%s"
						", %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(),
						dpfn.nMsgType);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaNotify::UpdatePoFaNotify(DB_PO_FA_NOTIFY& dpfn)
{
	DB_PO_HEADER& tDPH = dpfn.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_notify SET %s"
						", msg_type=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfn.nMsgType,
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

INT32	CDBMgrPoFaNotify::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaNotify* ptDBList = (TListDBPoFaNotify*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaNotify::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_NOTIFY pd_t = (PDB_PO_FA_NOTIFY)lpTemplet;

    return InsertPoFaNotify(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaNotify::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_NOTIFY pd_t = (PDB_PO_FA_NOTIFY)lpTemplet;

    return UpdatePoFaNotify(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaNotify::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






