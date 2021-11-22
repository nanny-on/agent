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
#include "DBMgrLogDeployFile.h"

//---------------------------------------------------------------------------

CDBMgrLogDeployFile*		t_DBMgrLogDeployFile;

//---------------------------------------------------------------------------

CDBMgrLogDeployFile::CDBMgrLogDeployFile()
{
	m_strDBTName = "log_deploy_file";
}
//---------------------------------------------------------------------------

CDBMgrLogDeployFile::~CDBMgrLogDeployFile()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDeployFile::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDeployFile::LoadDB(TListDBLogDeployFile& tDBLogDeployFileList)
{
    UINT32 nReadCnt = 0;
    DB_LOG_DEPLOY_FILE dldf;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target, "
						"notify_type, notify_id, "
						"host_id, user_id, policy_id, unit_id, "
						"reg_svr_id, sync_svr_step, "
						"end_time, end_reason "
						"FROM log_deploy_file WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;

        dldf.nID						= GetDBField_Int(nIndex++);
		dldf.nRegDate					= GetDBField_Int(nIndex++);
		dldf.nEvtTime					= GetDBField_Int(nIndex++);
		dldf.nEvtErrCode				= GetDBField_Int(nIndex++);
		dldf.nSkipTarget				= GetDBField_Int(nIndex++);

		dldf.nNotifyType				= GetDBField_Int(nIndex++);
		dldf.nNotifyID					= GetDBField_Int(nIndex++);

		dldf.nHostID					= GetDBField_Int(nIndex++);
		dldf.nUserID					= GetDBField_Int(nIndex++);
		dldf.nPolicyID					= GetDBField_Int(nIndex++);
		dldf.nUnitID					= GetDBField_Int(nIndex++);

		dldf.nRegSvrID					= GetDBField_Int(nIndex++);
		dldf.nSyncSvrStep				= GetDBField_Int(nIndex++);

		dldf.nEndTime					= GetDBField_Int(nIndex++);
		dldf.nEndReason					= GetDBField_Int(nIndex++);

        tDBLogDeployFileList.push_back(dldf);
        if(m_nLoadMaxID < UINT32(dldf.nID))
			m_nLoadMaxID = dldf.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBLogDeployFileList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDeployFile::InsertLogDeployFile(DB_LOG_DEPLOY_FILE& dldf)
{
	m_strQuery = SPrintf("INSERT INTO log_deploy_file(used_flag, reg_date, evt_time, evt_ecode, skip_target, "
									"notify_type, notify_id, "
									"host_id, user_id, policy_id, unit_id, "
									"reg_svr_id, sync_svr_step, "
									"end_time, end_reason)"
    								"VALUES"
									"(%u, %u, %u, %u, %u, "
									"%u, %u, "
									"%u, %u, %u, %u, "
									"%u, %u, "
									"%u, %u);",
									dldf.nUsedFlag, dldf.nRegDate, dldf.nEvtTime, dldf.nEvtErrCode, dldf.nSkipTarget,
									dldf.nNotifyType, dldf.nNotifyID, 
									dldf.nHostID, dldf.nUserID, dldf.nPolicyID, dldf.nUnitID, 
									dldf.nRegSvrID, dldf.nSyncSvrStep,
									dldf.nEndTime, dldf.nEndReason);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(dldf.nID == 0)
		dldf.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDeployFile::UpdateLogDeployFile(DB_LOG_DEPLOY_FILE& dldf)
{	
	m_strQuery = SPrintf("UPDATE log_deploy_file SET reg_date=%u, evt_time=%u, evt_ecode=%u, skip_target=%u,"
						"notify_type=%u, notify_id=%u, "
						"host_id=%u, user_id=%u, policy_id=%u, unit_id=%u, "
						"reg_svr_id=%u, sync_svr_step=%u, "
						"end_time=%u, end_reason=%u "
						"WHERE id=%u;",
						dldf.nRegDate, dldf.nEvtTime, dldf.nEvtErrCode, dldf.nSkipTarget,
						dldf.nNotifyType, dldf.nNotifyID, 
						dldf.nHostID, dldf.nUserID, dldf.nPolicyID, dldf.nUnitID, 
						dldf.nRegSvrID, dldf.nSyncSvrStep,
						dldf.nEndTime, dldf.nEndReason,						
						dldf.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDeployFile::DeleteLogDeployFile(UINT32 nID)
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

INT32	CDBMgrLogDeployFile::LoadExecute(PVOID lpTempletList)
{
	TListDBLogDeployFile* ptDBList = (TListDBLogDeployFile*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDeployFile::InsertExecute(PVOID lpTemplet)
{
	PDB_LOG_DEPLOY_FILE pd_t = (PDB_LOG_DEPLOY_FILE)lpTemplet;

    return InsertLogDeployFile(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDeployFile::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOG_DEPLOY_FILE pd_t = (PDB_LOG_DEPLOY_FILE)lpTemplet;

    return UpdateLogDeployFile(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDeployFile::DeleteExecute(UINT32 nID)
{
	return DeleteLogDeployFile(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDeployFile::LoadDB(UINT32 nLogMode, UINT32 nLogNum, TListDBLogDeployFile& tDBLogDeployFileList)
{
	UINT32 nReadCnt = 0;
	DB_LOG_DEPLOY_FILE dldf;

	INT32 nIndex = 0;
	INT32 nContinue = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target, "
		"notify_type, notify_id, "
		"host_id, user_id, policy_id, unit_id, "
		"reg_svr_id, sync_svr_step, "
		"end_time, end_reason "
		"FROM log_deploy_file WHERE used_flag=1;");
	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

	do
	{
		nContinue	= 0;
		nIndex		= 0;

		dldf.nID						= GetDBField_Int(nIndex++);
		dldf.nRegDate					= GetDBField_Int(nIndex++);

		switch(nLogMode)
		{
		case SS_ENV_LOG_LOAD_MODE_TYPE_DAY:	
			{
				if(nLogNum && dldf.nRegDate && dldf.nRegDate < nLogNum)	nContinue = 1;
				break;
			}
		case SS_ENV_LOG_LOAD_MODE_TYPE_COUNT:
			{
				if(nLogNum && nReadCnt > nLogNum)		nContinue = 1;
				break;
			}
		}

		if(nContinue)	continue;

		dldf.nEvtTime					= GetDBField_Int(nIndex++);
		dldf.nEvtErrCode				= GetDBField_Int(nIndex++);
		dldf.nSkipTarget				= GetDBField_Int(nIndex++);

		dldf.nNotifyType				= GetDBField_Int(nIndex++);
		dldf.nNotifyID					= GetDBField_Int(nIndex++);

		dldf.nHostID					= GetDBField_Int(nIndex++);
		dldf.nUserID					= GetDBField_Int(nIndex++);
		dldf.nPolicyID					= GetDBField_Int(nIndex++);
		dldf.nUnitID					= GetDBField_Int(nIndex++);

		dldf.nRegSvrID					= GetDBField_Int(nIndex++);
		dldf.nSyncSvrStep				= GetDBField_Int(nIndex++);

		dldf.nEndTime					= GetDBField_Int(nIndex++);
		dldf.nEndReason					= GetDBField_Int(nIndex++);

		tDBLogDeployFileList.push_back(dldf);
		if(m_nLoadMaxID < UINT32(dldf.nID))	m_nLoadMaxID = dldf.nID;
		nReadCnt++;
	}while(Next());
	m_nLoadNumber = (UINT32)tDBLogDeployFileList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
	return 0;
}
//---------------------------------------------------------------------------