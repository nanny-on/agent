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
#include "DBMgrLogRsBk.h"

//---------------------------------------------------------------------------

CDBMgrLogRsBk*		t_DBMgrLogRsBk = NULL;

//---------------------------------------------------------------------------

CDBMgrLogRsBk::CDBMgrLogRsBk()
{
	m_strDBTName = "log_rs_bk";
}
//---------------------------------------------------------------------------

CDBMgrLogRsBk::~CDBMgrLogRsBk()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogRsBk::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [log_rs_bk][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogRsBk::LoadDB(TListDBLogRsBk& tDBLogRsBkList)
{
    UINT32 nReadCnt = 0;
    DB_LOG_RS_BK data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target"
						", notify_type, notify_id"
						", host_id, policy_type, op_type"
						", reg_svr_id, sync_svr_step"
						", backup_size, backup_type, backup_time"
						", subject_path, subject_name, object_path, object_name"
						", bk_file_name, or_file_hash"
						" FROM log_rs_bk WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;

        data.nID					= GetDBField_Int(nIndex++);
		data.nRegDate				= GetDBField_Int(nIndex++);
		data.nEvtTime				= GetDBField_Int(nIndex++);
		data.nEvtErrCode			= GetDBField_Int(nIndex++);
		data.nSkipTarget			= GetDBField_Int(nIndex++);

		data.nNotifyType			= GetDBField_Int(nIndex++);
		data.nNotifyID				= GetDBField_Int(nIndex++);

		data.nHostID				= GetDBField_Int(nIndex++);
		data.nPolicyType			= GetDBField_Int(nIndex++);
		data.nOpType				= GetDBField_Int(nIndex++);

		data.nRegSvrID				= GetDBField_Int(nIndex++);
		data.nSyncSvrStep			= GetDBField_Int(nIndex++);

		data.nBackupSize			= GetDBField_Int(nIndex++);
		data.nBackupType			= GetDBField_Int(nIndex++);
		data.nBackupTime			= GetDBField_Int(nIndex++);
		
		data.strSubjectPath			= GetDBField_String(nIndex++);
		data.strSubjectName			= GetDBField_String(nIndex++);
		data.strObjectPath			= GetDBField_String(nIndex++);
		data.strObjectName			= GetDBField_String(nIndex++);
//		data.strObjectPathW			= ConvertWideString(data.strObjectName);

		data.strBkFileName			= GetDBField_String(nIndex++);
		data.strOrFileHash			= GetDBField_String(nIndex++);

		{
			QueryToMem(data.strObjectName);
			QueryToMem(data.strSubjectName);
			QueryToMem(data.strBkFileName);
			QueryToMem(data.strSubjectPath);
			QueryToMem(data.strObjectPath);
		}

        tDBLogRsBkList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBLogRsBkList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogRsBk::InsertLogRsBk(DB_LOG_RS_BK& data)
{
	String strObjectName  = MemToQuery(data.strObjectName);
	String strSubJectName = MemToQuery(data.strSubjectName);
	String strBkFileName  = MemToQuery(data.strBkFileName);

	String strObjectPath  = MemToQuery(data.strObjectPath);
	String strSubjectPath = MemToQuery(data.strSubjectPath);

	m_strQuery = SPrintf("INSERT INTO log_rs_bk(used_flag, reg_date, evt_time, evt_ecode, skip_target"
									", notify_type, notify_id"
									", host_id, policy_type, op_type"
									", reg_svr_id, sync_svr_step"
									", backup_size, backup_type, backup_time"
									", subject_path, subject_name, object_path, object_name"
									", bk_file_name, or_file_hash "
    								") VALUES (%u, %u, %u, %u, %u"
									", %u, %u"
									", %u, %u, %u"
									", %u, %u"
									", %u, %u, %u"
									", '%s', '%s', '%s', '%s'"
									", '%s', '%s'"
									");",
									data.nUsedFlag, data.nRegDate, data.nEvtTime, data.nEvtErrCode, data.nSkipTarget,
									data.nNotifyType, data.nNotifyID, 
									data.nHostID, data.nPolicyType, data.nOpType, 
									data.nRegSvrID, data.nSyncSvrStep,
									data.nBackupSize, data.nBackupType, data.nBackupTime, 
									strSubjectPath.c_str(), strSubJectName.c_str(), strObjectPath.c_str(), strObjectName.c_str(), 
									strBkFileName.c_str(), data.strOrFileHash.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogRsBk::UpdateLogRsBk(DB_LOG_RS_BK& data)
{	
	String strObjectName  = MemToQuery(data.strObjectName);
	String strSubjectName = MemToQuery(data.strSubjectName);
	String strBkFileName  = MemToQuery(data.strBkFileName);

	String strObjectPath  = MemToQuery(data.strObjectPath);
	String strSubjectPath = MemToQuery(data.strSubjectPath);

	m_strQuery = SPrintf("UPDATE log_rs_bk SET reg_date=%u, evt_time=%u, evt_ecode=%u, skip_target=%u"
						", notify_type=%u, notify_id=%u"
						", host_id=%u, policy_type=%u, op_type=%u"
						", reg_svr_id=%u, sync_svr_step=%u"
						", backup_size=%u, backup_type=%u, backup_time=%u"
						", subject_path='%s', subject_name='%s', object_path='%s', object_name='%s'"
						", bk_file_name='%s', or_file_hash='%s'"
						" WHERE id=%u;",
						data.nRegDate, data.nEvtTime, data.nEvtErrCode, data.nSkipTarget,
						data.nNotifyType, data.nNotifyID, 
						data.nHostID, data.nPolicyType, data.nOpType, 
						data.nRegSvrID, data.nSyncSvrStep,
						data.nBackupSize, data.nBackupType, data.nBackupTime, 
						strSubjectPath.c_str(), strSubjectName.c_str(), strObjectPath.c_str(), strObjectName.c_str(), 
						strBkFileName.c_str(), data.strOrFileHash.c_str(),
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogRsBk::DeleteLogRsBk(UINT32 nID)
{
    m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_DELETE_FAIL;

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

INT32	CDBMgrLogRsBk::LoadExecute(PVOID lpTempletList)
{
	TListDBLogRsBk* ptDBList = (TListDBLogRsBk*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogRsBk::InsertExecute(PVOID lpTemplet)
{
	PDB_LOG_RS_BK pd_t = (PDB_LOG_RS_BK)lpTemplet;

    return InsertLogRsBk(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogRsBk::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOG_RS_BK pd_t = (PDB_LOG_RS_BK)lpTemplet;

    return UpdateLogRsBk(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogRsBk::DeleteExecute(UINT32 nID)
{
	return DeleteLogRsBk(nID);
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogRsBk::LoadDB(UINT32 nLogMode, UINT32 nLogNum, TListDBLogRsBk& tDBLogRsBkList)
{
	UINT32 nReadCnt = 0;
	DB_LOG_RS_BK data;

	INT32 nIndex = 0;
	INT32 nContinue = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target"
		", notify_type, notify_id"
		", host_id, policy_type, op_type"
		", reg_svr_id, sync_svr_step"
		", backup_size, backup_type, backup_time"
		", subject_path, subject_name, object_path, object_name, bk_file_name"
		" FROM log_rs_bk WHERE used_flag=1;");
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

	do
	{
		nContinue	= 0;
		nIndex		= 0;

		data.nID						= GetDBField_Int(nIndex++);
		data.nRegDate				= GetDBField_Int(nIndex++);

		switch(nLogMode)
		{
			case SS_ENV_LOG_LOAD_MODE_TYPE_DAY:	
			{
				if(nLogNum && data.nRegDate && data.nRegDate < nLogNum)	nContinue = 1;
				break;
			}
			case SS_ENV_LOG_LOAD_MODE_TYPE_COUNT:
			{
				if(nLogNum && nReadCnt > nLogNum)		nContinue = 1;
				break;
			}
		}

		if(nContinue)	continue;


		data.nEvtTime				= GetDBField_Int(nIndex++);
		data.nEvtErrCode			= GetDBField_Int(nIndex++);
		data.nSkipTarget			= GetDBField_Int(nIndex++);

		data.nNotifyType			= GetDBField_Int(nIndex++);
		data.nNotifyID				= GetDBField_Int(nIndex++);

		data.nHostID				= GetDBField_Int(nIndex++);
		data.nPolicyType			= GetDBField_Int(nIndex++);
		data.nOpType				= GetDBField_Int(nIndex++);

		data.nRegSvrID				= GetDBField_Int(nIndex++);
		data.nSyncSvrStep			= GetDBField_Int(nIndex++);

		data.nBackupSize			= GetDBField_Int(nIndex++);
		data.nBackupType			= GetDBField_Int(nIndex++);
		data.nBackupTime			= GetDBField_Int(nIndex++);

		data.strSubjectPath			= GetDBField_String(nIndex++);
		data.strSubjectName			= GetDBField_String(nIndex++);
		data.strObjectPath			= GetDBField_String(nIndex++);
		data.strObjectName			= GetDBField_String(nIndex++);
//		data.strObjectPathW			= ConvertWideString(data.strObjectName);
		data.strBkFileName			= GetDBField_String(nIndex++);

		tDBLogRsBkList.push_back(data);
		if(m_nLoadMaxID < UINT32(data.nID))
			m_nLoadMaxID = data.nID;
		nReadCnt++;
	}while(Next());
	m_nLoadNumber = (UINT32)tDBLogRsBkList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
	return 0;
}
//---------------------------------------------------------------------------