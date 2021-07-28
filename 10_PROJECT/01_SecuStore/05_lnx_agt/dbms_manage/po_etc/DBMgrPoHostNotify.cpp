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
#include "DBMgrPoHostNotify.h"

//---------------------------------------------------------------------------

CDBMgrPoHostNotify*		t_DBMgrPoHostNotify = NULL;

//---------------------------------------------------------------------------

CDBMgrPoHostNotify::CDBMgrPoHostNotify()
{
	m_strDBTName = "po_host_notify";
}
//---------------------------------------------------------------------------

CDBMgrPoHostNotify::~CDBMgrPoHostNotify()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostNotify::SetInitalize()
{
    {
    	
	}
	
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostNotify::LoadDB(TListDBPoHostNotify& tDBPoHostNotifyList)
{
    UINT32 nReadCnt = 0;
    DB_PO_HOST_NOTIFY dphps;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						" FROM po_host_notify WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;
    
    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dphps.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
		
        tDBPoHostNotifyList.push_back(dphps);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoHostNotifyList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostNotify::InsertPoHostNotify(DB_PO_HOST_NOTIFY& dphn)
{
	DB_PO_HEADER& tDPH = dphn.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_host_notify("
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

INT32			CDBMgrPoHostNotify::UpdatePoHostNotify(DB_PO_HOST_NOTIFY& dphn)
{
	DB_PO_HEADER& tDPH = dphn.tDPH;

	m_strQuery = SPrintf("UPDATE po_host_notify SET %s"
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

INT32	CDBMgrPoHostNotify::LoadExecute(PVOID lpTempletList)
{
	TListDBPoHostNotify* ptDBList = (TListDBPoHostNotify*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostNotify::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_HOST_NOTIFY pd_t = (PDB_PO_HOST_NOTIFY)lpTemplet;

    return InsertPoHostNotify(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostNotify::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_HOST_NOTIFY pd_t = (PDB_PO_HOST_NOTIFY)lpTemplet;

    return UpdatePoHostNotify(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostNotify::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






