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
#include "DBMgrPoWinRun.h"

//---------------------------------------------------------------------------

CDBMgrPoWinRun*		t_DBMgrPoWinRun = NULL;

//---------------------------------------------------------------------------

CDBMgrPoWinRun::CDBMgrPoWinRun()
{
	m_strDBTName = "po_win_run";
}
//---------------------------------------------------------------------------

CDBMgrPoWinRun::~CDBMgrPoWinRun()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoWinRun::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoWinRun::LoadDB(TListDBPoWinRun& tDBPoWinRunList)
{
    UINT32 nReadCnt = 0;
    DB_PO_WIN_RUN dpwr;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", mbr_used_mode"
						" FROM po_win_run WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpwr.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpwr.nMbrUsedMode			= GetDBField_UInt(nIndex++);
        
        tDBPoWinRunList.push_back(dpwr);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoWinRunList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoWinRun::InsertPoWinRun(DB_PO_WIN_RUN& dpwr)
{
	DB_PO_HEADER& tDPH = dpwr.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_win_run("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", mbr_used_mode"
						") VALUES (%s"
                        ", %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpwr.nMbrUsedMode);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoWinRun::UpdatePoWinRun(DB_PO_WIN_RUN& dpwr)
{
	DB_PO_HEADER& tDPH = dpwr.tDPH;

	m_strQuery = SPrintf("UPDATE po_win_run SET %s"
						", mbr_used_mode=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dpwr.nMbrUsedMode,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoWinRun::DeletePoWinRun(UINT32 nID)
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

INT32	CDBMgrPoWinRun::LoadExecute(PVOID lpTempletList)
{
	TListDBPoWinRun* ptDBList = (TListDBPoWinRun*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoWinRun::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_WIN_RUN pd_t = (PDB_PO_WIN_RUN)lpTemplet;

    return InsertPoWinRun(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoWinRun::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_WIN_RUN pd_t = (PDB_PO_WIN_RUN)lpTemplet;

    return UpdatePoWinRun(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoWinRun::DeleteExecute(UINT32 nID)
{
	return DeletePoWinRun(nID);
}
//---------------------------------------------------------------------------






