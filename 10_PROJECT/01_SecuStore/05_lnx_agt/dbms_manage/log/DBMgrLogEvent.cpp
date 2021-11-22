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
#include "DBMgrLogEvent.h"

//---------------------------------------------------------------------------

CDBMgrLogEvent*		t_DBMgrLogEvent;

//---------------------------------------------------------------------------

CDBMgrLogEvent::CDBMgrLogEvent()
{
	m_strDBTName = "log_event";
}
//---------------------------------------------------------------------------

CDBMgrLogEvent::~CDBMgrLogEvent()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogEvent::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogEvent::LoadDB(TListDBLogEvent& tDBLogEventList)
{
    UINT32 nReadCnt = 0;
    DB_LOG_EVENT dle;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target, "
						"notify_type, notify_id, "
						"host_id, user_id, reg_svr_id, sync_svr_mode, sync_svr_step, "
						"subject_type, subject_id, subject_info, "
						"target_type, target_id, target_info, "
						"object_type, object_code, object_id, object_info, "
						"operation_type, evt_descr "
						"FROM log_event WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;

        dle.nID						= GetDBField_Int(nIndex++);
		dle.nRegDate				= GetDBField_Int(nIndex++);
		dle.nEvtTime				= GetDBField_Int(nIndex++);
		dle.nEvtErrCode				= GetDBField_Int(nIndex++);
		dle.nSkipTarget				= GetDBField_Int(nIndex++);

		dle.nNotifyType				= GetDBField_Int(nIndex++);
		dle.nNotifyID				= GetDBField_Int(nIndex++);

		dle.nHostID					= GetDBField_Int(nIndex++);
		dle.nUserID					= GetDBField_Int(nIndex++);
		dle.nRegSvrID				= GetDBField_Int(nIndex++);
		dle.nSyncSvrMode			= GetDBField_Int(nIndex++);
		dle.nSyncSvrStep			= GetDBField_Int(nIndex++);

		dle.nSubjectType			= GetDBField_Int(nIndex++);
		dle.nSubjectID				= GetDBField_Int(nIndex++);
		dle.strSubjectInfo			= GetDBField_String(nIndex++);

		dle.nTargetType				= GetDBField_Int(nIndex++);
		dle.nTargetID				= GetDBField_Int(nIndex++);
		dle.strTargetInfo			= GetDBField_String(nIndex++);

		dle.nObjectType				= GetDBField_Int(nIndex++);
		dle.nObjectCode				= GetDBField_Int(nIndex++);
        dle.nObjectID				= GetDBField_Int(nIndex++);
        dle.strObjectInfo			= GetDBField_String(nIndex++);

		dle.nOperationType			= GetDBField_Int(nIndex++);
		dle.strEventDescr			= GetDBField_String(nIndex++);
		{
			StrToMapID_Str(dle.strEventDescr, dle.tDescIDStrMap);
		}

        tDBLogEventList.push_back(dle);
        if(m_nLoadMaxID < UINT32(dle.nID))
			m_nLoadMaxID = dle.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBLogEventList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogEvent::InsertLogEvent(DB_LOG_EVENT& dle)
{
	char *pcQuery = (char *)malloc(MAX_BUFF);
	if(pcQuery == NULL)
		return ERR_DBMS_INSERT_FAIL;
	memset(pcQuery, 0, MAX_BUFF);
	snprintf(pcQuery, MAX_BUFF-1, "INSERT INTO log_event(used_flag, reg_date, evt_time, evt_ecode, skip_target, "
									"notify_type, notify_id, "
									"host_id, user_id, reg_svr_id, sync_svr_mode, sync_svr_step, "
									"subject_type, subject_id, subject_info, "
									"target_type, target_id, target_info, "
									"object_type, object_code, object_id, object_info, "
									"operation_type, evt_descr)"
    								"VALUES"
									"(%u, %u, %u, %u, %u, "
									"%u, %u, "
									"%u, %u, %u, %u, %u, "
									"%u, %u, '%s', "
									"%u, %u, '%s', "
									"%u, %u, %u, '%s', "
									"%u, '%s')",
									dle.nUsedFlag, dle.nRegDate, dle.nEvtTime, dle.nEvtErrCode, dle.nSkipTarget,
									dle.nNotifyType, dle.nNotifyID, 
									dle.nHostID, dle.nUserID, dle.nRegSvrID, dle.nSyncSvrMode, dle.nSyncSvrStep,
									dle.nSubjectType, dle.nSubjectID, dle.strSubjectInfo.c_str(), 
									dle.nTargetType, dle.nTargetID, dle.strTargetInfo.c_str(), 
									dle.nObjectType, dle.nObjectCode, dle.nObjectID, dle.strObjectInfo.c_str(), 
									dle.nOperationType, dle.strEventDescr.c_str());

	pcQuery[MAX_BUFF-1] = 0;

	if(DBOP_Check(ExecuteQuery(pcQuery)))
	{
		safe_free(pcQuery);
		return ERR_DBMS_INSERT_FAIL;
	}

	safe_free(pcQuery);

	if(dle.nID == 0)
		dle.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogEvent::UpdateLogEvent(DB_LOG_EVENT& dle)
{	
	m_strQuery = SPrintf("UPDATE log_event SET reg_date=%u, evt_time=%u, evt_ecode=%u, skip_target=%u,"
						"notify_type=%u, notify_id=%u, "
						"host_id=%u, user_id=%u, reg_svr_id=%u, sync_svr_mode=%u, sync_svr_step=%u, "
						"subject_type=%u, subject_id=%u, subject_info='%s', "
						"target_type=%u, target_id=%u, target_info='%s', "
						"object_type=%u, object_code=%u, object_id=%u, object_info='%s', "
						"operation_type=%u, evt_descr='%s' "
						"WHERE id=%u;",
						dle.nRegDate, dle.nEvtTime, dle.nEvtErrCode, dle.nSkipTarget,
						dle.nNotifyType, dle.nNotifyID, 
						dle.nHostID, dle.nUserID, dle.nRegSvrID, dle.nSyncSvrMode, dle.nSyncSvrStep,
						dle.nSubjectType, dle.nSubjectID, dle.strSubjectInfo.c_str(), 
						dle.nTargetType, dle.nTargetID, dle.strTargetInfo.c_str(), 
						dle.nObjectType, dle.nObjectCode, dle.nObjectID, dle.strObjectInfo.c_str(), 
						dle.nOperationType, dle.strEventDescr.c_str(),
						dle.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
    		return ERR_DBMS_UPDATE_FAIL;
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogEvent::DeleteLogEvent(UINT32 nID)
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

INT32	CDBMgrLogEvent::LoadExecute(PVOID lpTempletList)
{
	TListDBLogEvent* ptDBList = (TListDBLogEvent*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogEvent::InsertExecute(PVOID lpTemplet)
{
	PDB_LOG_EVENT pd_t = (PDB_LOG_EVENT)lpTemplet;

    return InsertLogEvent(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogEvent::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOG_EVENT pd_t = (PDB_LOG_EVENT)lpTemplet;

    return UpdateLogEvent(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogEvent::DeleteExecute(UINT32 nID)
{
	return DeleteLogEvent(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogEvent::LoadDB(UINT32 nLogMode, UINT32 nLogNum, TListDBLogEvent& tDBLogEventList)
{
	UINT32 nReadCnt = 0;
	DB_LOG_EVENT dle;

	INT32 nIndex = 0;
	INT32 nContinue = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target, "
							"notify_type, notify_id, "
							"host_id, user_id, reg_svr_id, sync_svr_mode, sync_svr_step, "
							"subject_type, subject_id, subject_info, "
							"target_type, target_id, target_info, "
							"object_type, object_code, object_id, object_info, "
							"operation_type, evt_descr "
							"FROM log_event WHERE used_flag=1;");
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
    		return ERR_DBMS_SELECT_FAIL;

	do
	{
		nContinue	= 0;
		nIndex		= 0;

		dle.nID						= GetDBField_Int(nIndex++);
		dle.nRegDate				= GetDBField_Int(nIndex++);

		switch(nLogMode)
		{
		case SS_ENV_LOG_LOAD_MODE_TYPE_DAY:	
			{
				if(nLogNum && dle.nRegDate < nLogNum)	nContinue = 1;
				break;
			}
		case SS_ENV_LOG_LOAD_MODE_TYPE_COUNT:
			{
				if(nLogNum && nReadCnt > nLogNum)		nContinue = 1;
				break;
			}
		}

		if(nContinue)	continue;

		dle.nEvtTime				= GetDBField_Int(nIndex++);
		dle.nEvtErrCode				= GetDBField_Int(nIndex++);
		dle.nSkipTarget				= GetDBField_Int(nIndex++);

		dle.nNotifyType				= GetDBField_Int(nIndex++);
		dle.nNotifyID				= GetDBField_Int(nIndex++);

		dle.nHostID					= GetDBField_Int(nIndex++);
		dle.nUserID					= GetDBField_Int(nIndex++);
		dle.nRegSvrID				= GetDBField_Int(nIndex++);
		dle.nSyncSvrMode			= GetDBField_Int(nIndex++);
		dle.nSyncSvrStep			= GetDBField_Int(nIndex++);

		dle.nSubjectType			= GetDBField_Int(nIndex++);
		dle.nSubjectID				= GetDBField_Int(nIndex++);
		dle.strSubjectInfo			= GetDBField_String(nIndex++);

		dle.nTargetType				= GetDBField_Int(nIndex++);
		dle.nTargetID				= GetDBField_Int(nIndex++);
		dle.strTargetInfo			= GetDBField_String(nIndex++);

		dle.nObjectType				= GetDBField_Int(nIndex++);
		dle.nObjectCode				= GetDBField_Int(nIndex++);
		dle.nObjectID				= GetDBField_Int(nIndex++);
		dle.strObjectInfo			= GetDBField_String(nIndex++);

		dle.nOperationType			= GetDBField_Int(nIndex++);
		dle.strEventDescr			= GetDBField_String(nIndex++);
		{
			StrToMapID_Str(dle.strEventDescr, dle.tDescIDStrMap);
		}

		tDBLogEventList.push_back(dle);
		if(m_nLoadMaxID < UINT32(dle.nID))	m_nLoadMaxID = dle.nID;
		nReadCnt++;
	}while(Next());
	m_nLoadNumber = (UINT32)tDBLogEventList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
	return 0;
}