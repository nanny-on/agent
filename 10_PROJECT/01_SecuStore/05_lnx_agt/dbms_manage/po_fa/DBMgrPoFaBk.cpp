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
#include "DBMgrPoFaBk.h"

//---------------------------------------------------------------------------

CDBMgrPoFaBk*		t_DBMgrPoFaBk = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaBk::CDBMgrPoFaBk()
{
	m_strDBTName = "po_fa_bk";
}
//---------------------------------------------------------------------------

CDBMgrPoFaBk::~CDBMgrPoFaBk()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaBk::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaBk::LoadDB(TListDBPoFaBk& tDBPoFaBkList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_BK dpfb;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", keep_day, keep_size, free_size"
						", file_max_size"
						" FROM po_fa_bk WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfb.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfb.nKeepDay				= GetDBField_UInt(nIndex++);
		dpfb.nKeepSize				= GetDBField_UInt(nIndex++);
		dpfb.nFreeSize				= GetDBField_UInt(nIndex++);
		dpfb.nFileMaxSize			= GetDBField_UInt(nIndex++);
        
        tDBPoFaBkList.push_back(dpfb);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaBkList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaBk::InsertPoFaBk(DB_PO_FA_BK& dpfb)
{
	DB_PO_HEADER& tDPH = dpfb.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_bk("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", keep_day, keep_size, free_size"
						", file_max_size"
						") VALUES (%s "
                        ", %u, %u, %u"
						", %u"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpfb.nKeepDay, dpfb.nKeepSize, dpfb.nFreeSize,
						dpfb.nFileMaxSize);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaBk::UpdatePoFaBk(DB_PO_FA_BK& dpfb)
{
	DB_PO_HEADER& tDPH = dpfb.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_bk SET %s"
						", keep_day=%u, keep_size=%u, free_size=%u"
						", file_max_size=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfb.nKeepDay, dpfb.nKeepSize, dpfb.nFreeSize,
						dpfb.nFileMaxSize,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaBk::DeletePoFaBk(UINT32 nID)
{
    m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}

INT32			CDBMgrPoFaBk::ClearPoFaBk()
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_CLEAR, m_strDBTName.c_str());

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

INT32	CDBMgrPoFaBk::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaBk* ptDBList = (TListDBPoFaBk*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaBk::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_BK pd_t = (PDB_PO_FA_BK)lpTemplet;

    return InsertPoFaBk(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaBk::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_BK pd_t = (PDB_PO_FA_BK)lpTemplet;

    return UpdatePoFaBk(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaBk::DeleteExecute(UINT32 nID)
{
	return DeletePoFaBk(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaBk::ClearExecute()
{
	return ClearPoFaBk();
}






