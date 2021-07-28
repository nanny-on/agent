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
#include "DBMgrLogPatch.h"

//---------------------------------------------------------------------------

CDBMgrLogPatch*		t_DBMgrLogPatch = NULL;

//---------------------------------------------------------------------------

CDBMgrLogPatch::CDBMgrLogPatch()
{
	m_strDBTName = "log_patch";
}
//---------------------------------------------------------------------------

CDBMgrLogPatch::~CDBMgrLogPatch()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogPatch::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [log_patch][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogPatch::LoadDB(TListDBLogPatch& tDBLogPatchList)
{
    UINT32 nReadCnt = 0;
    DB_LOG_PATCH data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target"
						", notify_type, notify_id, host_id, user_id"
						", reg_svr_id, sync_svr_step"
						", patch_id, patch_status, patch_type"
						", apply_num"
						" FROM log_patch WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

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
		data.nUserID				= GetDBField_Int(nIndex++);

		data.nRegSvrID				= GetDBField_Int(nIndex++);
		data.nSyncSvrStep			= GetDBField_Int(nIndex++);

		data.nPatchID				= GetDBField_Int(nIndex++);
		data.nPatchStatus			= GetDBField_Int(nIndex++);	
		data.nPatchType				= GetDBField_Int(nIndex++);	

		data.nApplyNum				= GetDBField_Int(nIndex++);

        tDBLogPatchList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBLogPatchList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogPatch::InsertLogPatch(DB_LOG_PATCH& data)
{
	m_strQuery = SPrintf("INSERT INTO log_patch(used_flag, reg_date, evt_time, evt_ecode, skip_target"
									", notify_type, notify_id, host_id, user_id"
									", reg_svr_id, sync_svr_step"
									", patch_id, patch_status, patch_type"
									", apply_num"
    								" ) VALUES (%u, %u, %u, %u, %u"
									", %u, %u, %u, %u"
									", %u, %u"
									", %u, %u, %u"
									", %u"
									");",
									data.nUsedFlag, data.nRegDate, data.nEvtTime, data.nEvtErrCode, data.nSkipTarget,
									data.nNotifyType, data.nNotifyID, data.nHostID, data.nUserID,
									data.nRegSvrID, data.nSyncSvrStep,
									data.nPatchID, data.nPatchStatus, data.nPatchType,
									data.nApplyNum);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogPatch::UpdateLogPatch(DB_LOG_PATCH& data)
{	
	m_strQuery = SPrintf("UPDATE log_patch SET reg_date=%u, evt_time=%u, evt_ecode=%u, skip_target=%u"
						", notify_type=%u, notify_id=%u, host_id=%u, user_id=%u"
						", reg_svr_id=%u, sync_svr_step=%u"
						", patch_id=%u, patch_status=%u, patch_type=%u"
						", apply_num=%u"
						" WHERE id=%u;",
						data.nRegDate, data.nEvtTime, data.nEvtErrCode, data.nSkipTarget,
						data.nNotifyType, data.nNotifyID, data.nHostID, data.nUserID,
						data.nRegSvrID, data.nSyncSvrStep,
						data.nPatchID, data.nPatchStatus, data.nPatchType,
						data.nApplyNum,
						data.nID);

	//WriteLogN("[%d] [%s]", data.nApplyNum, m_strQuery);
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogPatch::DeleteLogPatch(UINT32 nID)
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

INT32	CDBMgrLogPatch::LoadExecute(PVOID lpTempletList)
{
	TListDBLogPatch* ptDBList = (TListDBLogPatch*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogPatch::InsertExecute(PVOID lpTemplet)
{
	PDB_LOG_PATCH pd_t = (PDB_LOG_PATCH)lpTemplet;

    return InsertLogPatch(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogPatch::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOG_PATCH pd_t = (PDB_LOG_PATCH)lpTemplet;

    return UpdateLogPatch(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogPatch::DeleteExecute(UINT32 nID)
{
	return DeleteLogPatch(nID);
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogPatch::LoadDB(UINT32 nLogMode, UINT32 nLogNum, TListDBLogPatch& tDBLogPatchList)
{
	UINT32 nReadCnt = 0;
	DB_LOG_PATCH data;

	INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target"
		", notify_type, notify_id, host_id, user_id"
		", reg_svr_id, sync_svr_step"
		", patch_id, patch_status, patch_type"
		", apply_num"
		" FROM log_patch WHERE used_flag=1;");
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

	do
	{
		nIndex		= 0;

		data.nID					= GetDBField_Int(nIndex++);
		data.nRegDate				= GetDBField_Int(nIndex++);
		data.nEvtTime				= GetDBField_Int(nIndex++);
		data.nEvtErrCode			= GetDBField_Int(nIndex++);
		data.nSkipTarget			= GetDBField_Int(nIndex++);

		data.nNotifyType			= GetDBField_Int(nIndex++);
		data.nNotifyID				= GetDBField_Int(nIndex++);
		data.nHostID				= GetDBField_Int(nIndex++);
		data.nUserID				= GetDBField_Int(nIndex++);
		
		data.nRegSvrID				= GetDBField_Int(nIndex++);
		data.nSyncSvrStep			= GetDBField_Int(nIndex++);

		data.nPatchID				= GetDBField_Int(nIndex++);
		data.nPatchStatus			= GetDBField_Int(nIndex++);

		data.nPatchType				= GetDBField_Int(nIndex++);
		data.nApplyNum				= GetDBField_Int(nIndex++);
		
		tDBLogPatchList.push_back(data);
		if(m_nLoadMaxID < UINT32(data.nID))
			m_nLoadMaxID = data.nID;
		nReadCnt++;
	}while(Next());
	m_nLoadNumber = (UINT32)tDBLogPatchList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
	return 0;
}
//---------------------------------------------------------------------------