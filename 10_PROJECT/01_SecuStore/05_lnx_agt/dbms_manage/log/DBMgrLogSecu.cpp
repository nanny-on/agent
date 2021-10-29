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
#include "DBMgrLogSecu.h"

//---------------------------------------------------------------------------

CDBMgrLogSecu*		t_DBMgrLogSecu;

//---------------------------------------------------------------------------

CDBMgrLogSecu::CDBMgrLogSecu()
{
	m_strDBTName = "log_secu";
}
//---------------------------------------------------------------------------

CDBMgrLogSecu::~CDBMgrLogSecu()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogSecu::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogSecu::LoadDB(TListDBLogSecu& tDBLogSecuList)
{
    UINT32 nReadCnt = 0;
    DB_LOG_SECU dls;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target, "
						"notify_type, notify_id, "
						"host_id, user_id, policy_type, op_type, "
						"reg_svr_id, sync_svr_step, "
						"block_type, object_type, "
						"subject_path, subject_name, object_path, object_name, ext_info "
						"FROM log_secu WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;

        dls.nID						= GetDBField_Int(nIndex++);
		dls.nRegDate				= GetDBField_Int(nIndex++);
		dls.nEvtTime				= GetDBField_Int(nIndex++);
		dls.nEvtErrCode				= GetDBField_Int(nIndex++);
		dls.nSkipTarget				= GetDBField_Int(nIndex++);

		dls.nNotifyType				= GetDBField_Int(nIndex++);
		dls.nNotifyID				= GetDBField_Int(nIndex++);

		dls.nHostID					= GetDBField_Int(nIndex++);
		dls.nUserID					= GetDBField_Int(nIndex++);
		dls.nPolicyType				= GetDBField_Int(nIndex++);
		dls.nOpType					= GetDBField_Int(nIndex++);

		dls.nRegSvrID				= GetDBField_Int(nIndex++);
		dls.nSyncSvrStep			= GetDBField_Int(nIndex++);

		dls.nBlockType				= GetDBField_Int(nIndex++);
		dls.nObjectType				= GetDBField_Int(nIndex++);
		
		dls.strSubjectPath			= GetDBField_String(nIndex++);
		dls.strSubjectName			= GetDBField_String(nIndex++);
		dls.strObjectPath			= GetDBField_String(nIndex++);
		dls.strObjectName			= GetDBField_String(nIndex++);
//        dls.strObjectPathW			= ConvertWideString(dls.strObjectName);

		dls.strExtInfo				= GetDBField_String(nIndex++);

        tDBLogSecuList.push_back(dls);
        if(m_nLoadMaxID < UINT32(dls.nID))
			m_nLoadMaxID = dls.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBLogSecuList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogSecu::InsertLogSecu(DB_LOG_SECU& dls)
{
	String strObjectName = MemToQuery(dls.strObjectName);
	String strSubJectName = MemToQuery(dls.strSubjectName);
	String strObjectPath  = MemToQuery(dls.strObjectPath);
	String strSubjectPath = MemToQuery(dls.strSubjectPath);

	m_strQuery = SPrintf("INSERT INTO log_secu(used_flag, reg_date, evt_time, evt_ecode, skip_target,"
									"notify_type, notify_id, "
									"host_id, user_id, policy_type, op_type, "
									"reg_svr_id, sync_svr_step, "
									"block_type, object_type, "
									"subject_path, subject_name, object_path, object_name, ext_info)"
    								"VALUES"
									"(%u, %u, %u, %u, %u, "
									"%u, %u, "
									"%u, %u, %u, %u, "
									"%u, %u, "
									"%u, %u, "
									"'%s', '%s', '%s', '%s', '%s');",
									dls.nUsedFlag, dls.nRegDate, dls.nEvtTime, dls.nEvtErrCode, dls.nSkipTarget,
									dls.nNotifyType, dls.nNotifyID, 
									dls.nHostID, dls.nUserID, dls.nPolicyType, dls.nOpType, 
									dls.nRegSvrID, dls.nSyncSvrStep,
									dls.nBlockType, dls.nObjectType,
									strSubjectPath.c_str(), strSubJectName.c_str(), strObjectPath.c_str(), strObjectName.c_str(), dls.strExtInfo.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(dls.nID == 0)
		dls.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogSecu::UpdateLogSecu(DB_LOG_SECU& dls)
{

	String strObjectName = MemToQuery(dls.strObjectName);
	String strSubjectName = MemToQuery(dls.strSubjectName);
	String strObjectPath  = MemToQuery(dls.strObjectPath);
	String strSubjectPath = MemToQuery(dls.strSubjectPath);

	m_strQuery = SPrintf("UPDATE log_secu SET reg_date=%u, evt_time=%u, evt_ecode=%u, skip_target=%u,"
								"notify_type=%u, notify_id=%u, "
								"host_id=%u, user_id=%u, policy_type=%u, op_type=%u, "
								"reg_svr_id=%u, sync_svr_step=%u, "
								"block_type=%u, object_type=%u, "
								"subject_path='%s', subject_name='%s', object_path='%s', object_name='%s', ext_info='%s' WHERE id=%u;",
								dls.nRegDate, dls.nEvtTime, dls.nEvtErrCode, dls.nSkipTarget,
								dls.nNotifyType, dls.nNotifyID, 
								dls.nHostID, dls.nUserID, dls.nPolicyType, dls.nOpType, 
								dls.nRegSvrID, dls.nSyncSvrStep,
								dls.nBlockType, dls.nObjectType,
								strSubjectPath.c_str(), strSubjectName.c_str(), strObjectPath.c_str(), strObjectName.c_str(), dls.strExtInfo.c_str(), dls.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogSecu::DeleteLogSecu(UINT32 nID)
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

INT32	CDBMgrLogSecu::LoadExecute(PVOID lpTempletList)
{
	TListDBLogSecu* ptDBList = (TListDBLogSecu*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogSecu::InsertExecute(PVOID lpTemplet)
{
	PDB_LOG_SECU pd_t = (PDB_LOG_SECU)lpTemplet;

    return InsertLogSecu(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogSecu::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOG_SECU pd_t = (PDB_LOG_SECU)lpTemplet;

    return UpdateLogSecu(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogSecu::DeleteExecute(UINT32 nID)
{
	return DeleteLogSecu(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogSecu::LoadDB(UINT32 nLogMode, UINT32 nLogNum, TListDBLogSecu& tDBLogSecuList)
{
	UINT32 nReadCnt = 0;
	DB_LOG_SECU dls;

	INT32 nIndex = 0;
	INT32 nContinue = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target, "
		"notify_type, notify_id, "
		"host_id, user_id, policy_type, op_type, "
		"reg_svr_id, sync_svr_step, "
		"block_type, object_type, "
		"subject_path, subject_name, object_path, object_name, ext_info "
		"FROM log_secu WHERE used_flag=1;");
	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

	do
	{
		nContinue	= 0;
		nIndex		= 0;

		dls.nID						= GetDBField_Int(nIndex++);
		dls.nRegDate				= GetDBField_Int(nIndex++);

		switch(nLogMode)
		{
		case SS_ENV_LOG_LOAD_MODE_TYPE_DAY:	
			{
				if(nLogNum && dls.nRegDate && dls.nRegDate < nLogNum)	nContinue = 1;
				break;
			}
		case SS_ENV_LOG_LOAD_MODE_TYPE_COUNT:
			{
				if(nLogNum && nReadCnt > nLogNum)		nContinue = 1;
				break;
			}
		}

		if(nContinue)	continue;

		dls.nEvtTime				= GetDBField_Int(nIndex++);
		dls.nEvtErrCode				= GetDBField_Int(nIndex++);
		dls.nSkipTarget				= GetDBField_Int(nIndex++);

		dls.nNotifyType				= GetDBField_Int(nIndex++);
		dls.nNotifyID				= GetDBField_Int(nIndex++);

		dls.nHostID					= GetDBField_Int(nIndex++);
		dls.nUserID					= GetDBField_Int(nIndex++);
		dls.nPolicyType				= GetDBField_Int(nIndex++);
		dls.nOpType					= GetDBField_Int(nIndex++);

		dls.nRegSvrID				= GetDBField_Int(nIndex++);
		dls.nSyncSvrStep			= GetDBField_Int(nIndex++);

		dls.nBlockType				= GetDBField_Int(nIndex++);
		dls.nObjectType				= GetDBField_Int(nIndex++);

		dls.strSubjectPath			= GetDBField_String(nIndex++);
		dls.strSubjectName			= GetDBField_String(nIndex++);
		dls.strObjectPath			= GetDBField_String(nIndex++);
		dls.strObjectName			= GetDBField_String(nIndex++);
//		dls.strObjectPathW			= ConvertWideString(dls.strObjectName);

		dls.strExtInfo				= GetDBField_String(nIndex++);

		tDBLogSecuList.push_back(dls);
		if(m_nLoadMaxID < UINT32(dls.nID))	m_nLoadMaxID = dls.nID;
		nReadCnt++;
	}while(Next());

	m_nLoadNumber = (UINT32)tDBLogSecuList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
	return 0;
}
//---------------------------------------------------------------------------