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
#include "DBMgrLogDoc.h"

//---------------------------------------------------------------------------

CDBMgrLogDoc*		t_DBMgrLogDoc = NULL;

//---------------------------------------------------------------------------

CDBMgrLogDoc::CDBMgrLogDoc()
{
	m_strDBTName = "log_doc";
	m_nLogDocID = 0;
}
//---------------------------------------------------------------------------

CDBMgrLogDoc::~CDBMgrLogDoc()
{
}


//---------------------------------------------------------------------------

INT32			CDBMgrLogDoc::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDoc::LoadDB(TListDBLogDoc& tDBLogDocList)
{
    UINT32 nReadCnt = 0;
    DB_LOG_DOC dld;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target, "
						"notify_type, notify_id, "
						"host_id, user_id, policy_type, op_type, "
						"reg_svr_id, sync_svr_step, "
						"remove_time, backup_type, backup_time, "
						"subject_path, subject_name, object_path, object_name, bk_file_name, file_cr_date, file_md_date, file_ac_date "
						"FROM log_doc WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;

        dld.nID						= GetDBField_Int(nIndex++);
		dld.nRegDate				= GetDBField_Int(nIndex++);
		dld.nEvtTime				= GetDBField_Int(nIndex++);
		dld.nEvtErrCode				= GetDBField_Int(nIndex++);
		dld.nSkipTarget				= GetDBField_Int(nIndex++);

		dld.nNotifyType				= GetDBField_Int(nIndex++);
		dld.nNotifyID				= GetDBField_Int(nIndex++);

		dld.nHostID					= GetDBField_Int(nIndex++);
		dld.nUserID					= GetDBField_Int(nIndex++);
		dld.nPolicyType				= GetDBField_Int(nIndex++);
		dld.nOpType					= GetDBField_Int(nIndex++);

		dld.nRegSvrID				= GetDBField_Int(nIndex++);
		dld.nSyncSvrStep			= GetDBField_Int(nIndex++);

		dld.nRemoveTime				= GetDBField_Int(nIndex++);
		dld.nBackupType				= GetDBField_Int(nIndex++);
		dld.nBackupTime				= GetDBField_Int(nIndex++);
		
		dld.strSubjectPath			= GetDBField_String(nIndex++);
		dld.strSubjectName			= GetDBField_String(nIndex++);
		dld.strObjectPath			= GetDBField_String(nIndex++);
		dld.strObjectName			= GetDBField_String(nIndex++);
		dld.strBkFileName			= GetDBField_String(nIndex++);

		dld.nFileCrTime				= GetDBField_Int(nIndex++);
		dld.nFileMdTime				= GetDBField_Int(nIndex++);
		dld.nFileAcTime				= GetDBField_Int(nIndex++);

		{
			QueryToMem(dld.strObjectName);
			QueryToMem(dld.strSubjectName);
			QueryToMem(dld.strBkFileName);
			QueryToMem(dld.strSubjectPath);
			QueryToMem(dld.strObjectPath);
		}

        tDBLogDocList.push_back(dld);
        if(m_nLoadMaxID < UINT32(dld.nID))
			m_nLoadMaxID = dld.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBLogDocList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDoc::InsertLogDoc(DB_LOG_DOC& dld)
{
	String strObjectName  = MemToQuery(dld.strObjectName);
	String strSubJectName = MemToQuery(dld.strSubjectName);
	String strBkFileName  = MemToQuery(dld.strBkFileName);

	String strObjectPath  = MemToQuery(dld.strObjectPath);
	String strSubjectPath = MemToQuery(dld.strSubjectPath);

	m_strQuery = SPrintf("INSERT INTO log_doc(used_flag, reg_date, evt_time, evt_ecode, skip_target, "
									"notify_type, notify_id, "
									"host_id, user_id, policy_type, op_type, "
									"reg_svr_id, sync_svr_step, "
									"remove_time, backup_type, backup_time, "
									"subject_path, subject_name, object_path, object_name, "
									"bk_file_name, file_cr_date, file_md_date, file_ac_date)"
    								"VALUES"
									"(%u, %u, %u, %u, %u, "
									"%u, %u, "
									"%u, %u, %u, %u, "
									"%u, %u, "
									"%u, %u, %u, "
									"'%s', '%s', '%s', '%s', "
									"'%s', %u, %u, %u);",
									dld.nUsedFlag, dld.nRegDate, dld.nEvtTime, dld.nEvtErrCode, dld.nSkipTarget,
									dld.nNotifyType, dld.nNotifyID, 
									dld.nHostID, dld.nUserID, dld.nPolicyType, dld.nOpType, 
									dld.nRegSvrID, dld.nSyncSvrStep,
									dld.nRemoveTime, dld.nBackupType, dld.nBackupTime, 
									strSubjectPath.c_str(), strSubJectName.c_str(), strObjectPath.c_str(), strObjectName.c_str(),
									strBkFileName.c_str(), dld.nFileCrTime, dld.nFileMdTime, dld.nFileAcTime);

	
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(m_nLogDocID == 0)
	{
		m_nLogDocID = GetLastID();
		dld.nID      = m_nLogDocID;
	}
	else
	{
		m_nLogDocID++;
		dld.nID      = m_nLogDocID;
	}

    return 0;
}


//---------------------------------------------------------------------------

INT32			CDBMgrLogDoc::UpdateLogDoc(DB_LOG_DOC& dld)
{	
	String strObjectName  = MemToQuery(dld.strObjectName);
	String strSubjectName = MemToQuery(dld.strSubjectName);
	String strBkFileName  = MemToQuery(dld.strBkFileName);

	String strObjectPath  = MemToQuery(dld.strObjectPath);
	String strSubjectPath = MemToQuery(dld.strSubjectPath);

	m_strQuery = SPrintf("UPDATE log_doc SET reg_date=%u, evt_time=%u, evt_ecode=%u, skip_target=%u,"
						"notify_type=%u, notify_id=%u, "
						"host_id=%u, user_id=%u, policy_type=%u, op_type=%u, "
						"reg_svr_id=%u, sync_svr_step=%u, "
						"remove_time=%u, backup_type=%u, backup_time=%u, "
						"subject_path='%s', subject_name='%s', object_path='%s', object_name='%s', "
						"bk_file_name='%s', file_cr_date=%u, file_md_date=%u, file_ac_date=%u "
						"WHERE id=%u;",
						dld.nRegDate, dld.nEvtTime, dld.nEvtErrCode, dld.nSkipTarget,
						dld.nNotifyType, dld.nNotifyID, 
						dld.nHostID, dld.nUserID, dld.nPolicyType, dld.nOpType, 
						dld.nRegSvrID, dld.nSyncSvrStep,
						dld.nRemoveTime, dld.nBackupType, dld.nBackupTime, 
						strSubjectPath.c_str(), strSubjectName.c_str(), strObjectPath.c_str(), strObjectName.c_str(),
						strBkFileName.c_str(), dld.nFileCrTime, dld.nFileMdTime, dld.nFileAcTime,
						dld.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDoc::DeleteLogDoc(UINT32 nID)
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

INT32	CDBMgrLogDoc::LoadExecute(PVOID lpTempletList)
{
	TListDBLogDoc* ptDBList = (TListDBLogDoc*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDoc::InsertExecute(PVOID lpTemplet)
{
	PDB_LOG_DOC pd_t = (PDB_LOG_DOC)lpTemplet;

    return InsertLogDoc(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDoc::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOG_DOC pd_t = (PDB_LOG_DOC)lpTemplet;

    return UpdateLogDoc(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDoc::DeleteExecute(UINT32 nID)
{
	return DeleteLogDoc(nID);
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDoc::LoadDB(UINT32 nLogMode, UINT32 nLogNum, TListDBLogDoc& tDBLogDocList)
{
	UINT32 nReadCnt = 0;
	DB_LOG_DOC dld;

	INT32 nIndex = 0;
	INT32 nContinue = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target, "
		"notify_type, notify_id, "
		"host_id, user_id, policy_type, op_type, "
		"reg_svr_id, sync_svr_step, "
		"remove_time, backup_type, backup_time, "
		"subject_path, subject_name, object_path, object_name, bk_file_name, file_cr_date, file_md_date, file_ac_date "
		"FROM log_doc WHERE used_flag=1;");
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
    		return ERR_DBMS_SELECT_FAIL;

	
	do
	{
		nContinue	= 0;
		nIndex		= 0;

		dld.nID						= GetDBField_Int(nIndex++);
		dld.nRegDate				= GetDBField_Int(nIndex++);

		switch(nLogMode)
		{
			case SS_ENV_LOG_LOAD_MODE_TYPE_DAY:	
			{
				if(nLogNum && dld.nRegDate && dld.nRegDate < nLogNum)	nContinue = 1;
				break;
			}
			case SS_ENV_LOG_LOAD_MODE_TYPE_COUNT:
			{
				if(nLogNum && nReadCnt > nLogNum)		nContinue = 1;
				break;
			}
		}

		if(nContinue)	continue;


		dld.nEvtTime				= GetDBField_Int(nIndex++);
		dld.nEvtErrCode				= GetDBField_Int(nIndex++);
		dld.nSkipTarget				= GetDBField_Int(nIndex++);

		dld.nNotifyType				= GetDBField_Int(nIndex++);
		dld.nNotifyID				= GetDBField_Int(nIndex++);

		dld.nHostID					= GetDBField_Int(nIndex++);
		dld.nUserID					= GetDBField_Int(nIndex++);
		dld.nPolicyType				= GetDBField_Int(nIndex++);
		dld.nOpType					= GetDBField_Int(nIndex++);

		dld.nRegSvrID				= GetDBField_Int(nIndex++);
		dld.nSyncSvrStep			= GetDBField_Int(nIndex++);

		dld.nRemoveTime				= GetDBField_Int(nIndex++);
		dld.nBackupType				= GetDBField_Int(nIndex++);
		dld.nBackupTime				= GetDBField_Int(nIndex++);

		dld.strSubjectPath			= GetDBField_String(nIndex++);
		dld.strSubjectName			= GetDBField_String(nIndex++);
		dld.strObjectPath			= GetDBField_String(nIndex++);
		dld.strObjectName			= GetDBField_String(nIndex++);
		dld.strBkFileName			= GetDBField_String(nIndex++);

		dld.nFileCrTime				= GetDBField_Int(nIndex++);
		dld.nFileMdTime				= GetDBField_Int(nIndex++);
		dld.nFileAcTime				= GetDBField_Int(nIndex++);

		tDBLogDocList.push_back(dld);
		if(m_nLoadMaxID < UINT32(dld.nID))	m_nLoadMaxID = dld.nID;
		nReadCnt++;
	}while(Next());
	m_nLoadNumber = (UINT32)tDBLogDocList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
	return 0;
}
//---------------------------------------------------------------------------