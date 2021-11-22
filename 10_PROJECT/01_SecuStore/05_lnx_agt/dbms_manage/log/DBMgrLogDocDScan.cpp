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
#include "DBMgrLogDocDScan.h"

//---------------------------------------------------------------------------

CDBMgrLogDocDScan*		t_DBMgrLogDocDScan;

//---------------------------------------------------------------------------

CDBMgrLogDocDScan::CDBMgrLogDocDScan()
{
	m_strDBTName = "log_doc_dscan";
}
//---------------------------------------------------------------------------

CDBMgrLogDocDScan::~CDBMgrLogDocDScan()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDocDScan::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDocDScan::LoadDB(TListDBLogDocDScan& tDBLogDocDScanList)
{
    UINT32 nReadCnt = 0;
    DB_LOG_DOC_DSCAN dldd;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, reg_date, ext_option, scan_path, scan_ptn, find_num, del_num FROM log_doc_dscan WHERE used_flag = 1");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;

		dldd.nID				= GetDBField_Int(nIndex++);
		dldd.nUsedFlag			= GetDBField_Int(nIndex++);
		dldd.nRegDate			= GetDBField_Int(nIndex++);
		dldd.nExtOption			= GetDBField_Int(nIndex++);
		dldd.strScanPath		= GetDBField_String(nIndex++);
		dldd.strScanPtn			= GetDBField_String(nIndex++);
		dldd.nFindNum			= GetDBField_Int(nIndex++);
		dldd.nDelNum			= GetDBField_Int(nIndex++);

        tDBLogDocDScanList.push_back(dldd);
		if(m_nLoadMaxID < UINT32(dldd.nID))	m_nLoadMaxID = dldd.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBLogDocDScanList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDocDScan::InsertLogDocDScan(DB_LOG_DOC_DSCAN& dldd)
{
	m_strQuery = SPrintf("INSERT INTO log_doc_dscan(used_flag, reg_date, ext_option"
						", scan_path, scan_ptn"
						", find_num, del_num"
						" ) VALUES ("
						"  %u, %u, %u"
						", '%s', '%s'"
						", %u, %u);",
						dldd.nUsedFlag, dldd.nRegDate, dldd.nExtOption, 
						dldd.strScanPath.c_str(), dldd.strScanPtn.c_str(), 
						dldd.nFindNum, dldd.nDelNum);


	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(dldd.nID == 0)
		dldd.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDocDScan::UpdateLogDocDScan(DB_LOG_DOC_DSCAN& dldd)
{
	m_strQuery = SPrintf("UPDATE log_doc_dscan SET reg_date=%u, ext_option=%u"
						", scan_path='%s', scan_ptn='%s'"
						", find_num=%u, del_num=%u"
						" WHERE id=%u;",
						dldd.nRegDate, dldd.nExtOption, 
						dldd.strScanPath.c_str(), dldd.strScanPtn.c_str(), 
						dldd.nFindNum, dldd.nDelNum,
						dldd.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDocDScan::DeleteLogDocDScan(UINT32 nID)
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

INT32	CDBMgrLogDocDScan::LoadExecute(PVOID lpTempletList)
{
	TListDBLogDocDScan* ptDBList = (TListDBLogDocDScan*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CDBMgrLogDocDScan::InsertExecute(PVOID lpTemplet)
{
	PDB_LOG_DOC_DSCAN pd_t = (PDB_LOG_DOC_DSCAN)lpTemplet;

    return InsertLogDocDScan(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDocDScan::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOG_DOC_DSCAN pd_t = (PDB_LOG_DOC_DSCAN)lpTemplet;

    return UpdateLogDocDScan(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDocDScan::DeleteExecute(UINT32 nID)
{
	return DeleteLogDocDScan(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDocDScan::LoadDB(UINT32 nLogMode, UINT32 nLogNum, TListDBLogDocDScan& tDBLogDocDScanList)
{
	UINT32 nReadCnt = 0;
	DB_LOG_DOC_DSCAN dldd;

	INT32 nIndex = 0;
	INT32 nContinue = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, reg_date, ext_option, scan_path, scan_ptn, find_num, del_num FROM log_doc_dscan WHERE used_flag = 1");
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nContinue	= 0;
		nIndex		= 0;

		dldd.nID				= GetDBField_Int(nIndex++);
		dldd.nUsedFlag			= GetDBField_Int(nIndex++);
		dldd.nRegDate			= GetDBField_Int(nIndex++);

		switch(nLogMode)
		{
		case SS_ENV_LOG_LOAD_MODE_TYPE_DAY:	
			{
				if(nLogNum && dldd.nRegDate < nLogNum)	nContinue = 1;
				break;
			}
		case SS_ENV_LOG_LOAD_MODE_TYPE_COUNT:
			{
				if(nLogNum && nReadCnt > nLogNum)		nContinue = 1;
				break;
			}
		}

		if(nContinue)	continue;

		dldd.nExtOption			= GetDBField_Int(nIndex++);
		dldd.strScanPath		= GetDBField_String(nIndex++);
		dldd.strScanPtn			= GetDBField_String(nIndex++);
		dldd.nFindNum			= GetDBField_Int(nIndex++);
		dldd.nDelNum			= GetDBField_Int(nIndex++);

		tDBLogDocDScanList.push_back(dldd);
		if(m_nLoadMaxID < UINT32(dldd.nID))	m_nLoadMaxID = dldd.nID;
		nReadCnt++;
	}while(Next());
	m_nLoadNumber = tDBLogDocDScanList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
	return 0;
}
//---------------------------------------------------------------------------