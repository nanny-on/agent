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



#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrDocDeleteInfo.h"

//---------------------------------------------------------------------------

CDBMgrDocDeleteInfo*		t_DBMgrDocDeleteInfo = NULL;

//---------------------------------------------------------------------------

CDBMgrDocDeleteInfo::CDBMgrDocDeleteInfo()
{
	m_strDBTName = "doc_delete_info";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrDocDeleteInfo::~CDBMgrDocDeleteInfo()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrDocDeleteInfo::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrDocDeleteInfo::LoadDB(TListDBDocDeleteInfo& tDBDocDeleteInfoList)
{
    UINT32 nReadCnt = 0;
    DB_DOC_DELETE_INFO dddi;

    INT32 nIndex = 0;

    m_strQuery = SPrintf("SELECT id, used_flag, reg_date, last_scan_time, last_del_time, doc_cnt, sch_cnt, menual_cnt FROM doc_delete_info;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

	do
	{  
    	nIndex = 0;
    	
        dddi.nID				= GetDBField_Int(nIndex++);
        dddi.nUsedFlag			= GetDBField_Int(nIndex++);
        dddi.nRegDate			= GetDBField_Int(nIndex++);
		dddi.nLastScanTime		= GetDBField_Int(nIndex++);
		dddi.nLastDelTime		= GetDBField_Int(nIndex++);
		dddi.nDocCnt			= GetDBField_Int(nIndex++);
		dddi.nSchCnt			= GetDBField_Int(nIndex++);
		dddi.nMenualCnt			= GetDBField_Int(nIndex++);

        tDBDocDeleteInfoList.push_back(dddi);
        if(m_nLoadMaxID < UINT32(dddi.nID))
			m_nLoadMaxID = dddi.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBDocDeleteInfoList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrDocDeleteInfo::InsertDocDeleteInfo(DB_DOC_DELETE_INFO& dddi)
{
    m_strQuery = SPrintf("INSERT INTO doc_delete_info (used_flag, reg_date, last_scan_time, last_del_time, doc_cnt, sch_cnt, menual_cnt) VALUES (%u, %u, %u, %u, %u, %u, %u);",
    								dddi.nUsedFlag, dddi.nRegDate, dddi.nLastScanTime, dddi.nLastDelTime, dddi.nDocCnt, dddi.nSchCnt, dddi.nMenualCnt);

    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
	if(dddi.nID == 0)
		dddi.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrDocDeleteInfo::UpdateDocDeleteInfo(DB_DOC_DELETE_INFO& dddi)
{
	m_strQuery = SPrintf("UPDATE doc_delete_info SET used_flag=%u, reg_date=%u, last_scan_time=%u, last_del_time=%u, doc_cnt=%u, sch_cnt=%u, menual_cnt=%u WHERE id=%d;",
									dddi.nUsedFlag, dddi.nRegDate, dddi.nLastScanTime, dddi.nLastDelTime, dddi.nDocCnt, dddi.nSchCnt, dddi.nMenualCnt, dddi.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrDocDeleteInfo::DeleteDocDeleteInfo(UINT32 nID)
{
    return 0;
}
//--------------------------------------------------------------------------- 
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------

INT32	CDBMgrDocDeleteInfo::LoadExecute(PVOID lpTempletList)
{
	TListDBDocDeleteInfo* ptDBList = (TListDBDocDeleteInfo*)lpTempletList;

    return LoadDB(*ptDBList);
}

INT32	CDBMgrDocDeleteInfo::InsertExecute(PVOID lpTemplet)
{
	PDB_DOC_DELETE_INFO pd_t = (PDB_DOC_DELETE_INFO)lpTemplet;

    return InsertDocDeleteInfo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrDocDeleteInfo::UpdateExecute(PVOID lpTemplet)
{
	PDB_DOC_DELETE_INFO pd_t = (PDB_DOC_DELETE_INFO)lpTemplet;

    return UpdateDocDeleteInfo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrDocDeleteInfo::DeleteExecute(UINT32 nID)
{
	return DeleteDocDeleteInfo(nID);
}
//---------------------------------------------------------------------------






