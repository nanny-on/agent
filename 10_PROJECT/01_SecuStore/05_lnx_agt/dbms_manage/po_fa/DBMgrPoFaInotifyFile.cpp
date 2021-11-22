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
#include "DBMgrPoFaInotifyFile.h"

//---------------------------------------------------------------------------

CDBMgrPoFaInotifyFile*		t_DBMgrPoFaInotifyFile = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaInotifyFile::CDBMgrPoFaInotifyFile()
{
	m_strDBTName = "log_notify_file";
}
//---------------------------------------------------------------------------

CDBMgrPoFaInotifyFile::~CDBMgrPoFaInotifyFile()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaInotifyFile::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaInotifyFile::LoadCount()
{
	INT32 nRtn = 0;
	m_strQuery = SPrintf("SELECT COUNT(*) FROM %s;", m_strDBTName.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;
	do
	{
		nRtn 		= GetDBField_Int(0);
	}while(Next());

	return nRtn;
}

INT32			CDBMgrPoFaInotifyFile::LoadDB(TListLogNotifyFile& tDBLogNotifyFileList)
{
    INT32 nCnt = 0;
    DB_LOG_NOTIFY_FILE stLogNotifyFile;
    INT32 nIndex = 0;

	nCnt = LoadCount();
	if(nCnt < 1)
	{
		m_nLoadNumber = 0;
		WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
		return 0;
	}
    m_strQuery = SPrintf("SELECT id, used_flag, reg_date, order_id, notify_file_path FROM %s;", m_strDBTName.c_str());

    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {    	
    	nIndex = 0;

        stLogNotifyFile.nID					= GetDBField_UInt(nIndex++);
		stLogNotifyFile.nUsedFlag			= GetDBField_UInt(nIndex++);
		stLogNotifyFile.nRegDate			= GetDBField_UInt(nIndex++);
		stLogNotifyFile.nOrderID			= GetDBField_UInt(nIndex++);
        stLogNotifyFile.strNotifyFilePath	= GetDBField_String(nIndex++);
        
        tDBLogNotifyFileList.push_back(stLogNotifyFile);
        if(m_nLoadMaxID < UINT32(stLogNotifyFile.nID))
			m_nLoadMaxID = stLogNotifyFile.nID;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBLogNotifyFileList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//--------------------------------------------------------------------------- 

INT32			CDBMgrPoFaInotifyFile::InsertInotifyFile(PDB_LOG_NOTIFY_FILE pLogNotify)
{
	INT32 nRetVal = 0;
	if(pLogNotify == NULL)
		return -1;

	nRetVal = IsInotifyFile(pLogNotify->strNotifyFilePath.c_str());
	if(nRetVal == 0)
		return 0;

	m_strQuery = SPrintf("INSERT INTO %s (used_flag, reg_date, order_id, notify_file_path) VALUES (%u, %u, %u, '%s');",
		m_strDBTName.c_str(), pLogNotify->nUsedFlag, pLogNotify->nRegDate, pLogNotify->nOrderID, pLogNotify->strNotifyFilePath.c_str());

    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
    pLogNotify->nID      = GetLastID();
    return 0;
}

INT32			CDBMgrPoFaInotifyFile::IsInotifyFile(LPCSTR pcNotifyFilePath)
{
	UINT32 nID = 0;
	if(pcNotifyFilePath == NULL || pcNotifyFilePath[0] == 0)
		return -1;

	m_strQuery = SPrintf("SELECT id FROM %s where notify_file_path='%s';", m_strDBTName.c_str(), pcNotifyFilePath) ;

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return -2;

	do
	{
		nID				= GetDBField_UInt(0);
	}while(Next());

	if(nID == 0)
		return -3;
	return 0;
}

INT32	CDBMgrPoFaInotifyFile::DeleteInotifyFile(UINT32 nID)
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

INT32	CDBMgrPoFaInotifyFile::LoadExecute(PVOID lpTempletList)
{
	TListLogNotifyFile* pDBLogList = (TListLogNotifyFile*)lpTempletList;
	if(pDBLogList == NULL)
		return -1;
    return LoadDB(*pDBLogList);
}
//--------------------------------------------------------------------------- 
INT32	CDBMgrPoFaInotifyFile::InsertExecute(PVOID lpTemplet)
{
	PDB_LOG_NOTIFY_FILE pLogNotify = (PDB_LOG_NOTIFY_FILE)lpTemplet;
	if(pLogNotify == NULL)
		return -1;
    return InsertInotifyFile(pLogNotify);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaInotifyFile::GetCountExecute()
{
	TListLogNotifyFile tDBLogNotifyFileList;

    return LoadDB(tDBLogNotifyFileList);
}

INT32	CDBMgrPoFaInotifyFile::DeleteExecute(UINT32 nID)
{
	return DeleteInotifyFile(nID);
}
//---------------------------------------------------------------------------







