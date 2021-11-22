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
#include "DBMgrLogDevice.h"

//---------------------------------------------------------------------------

CDBMgrLogDevice*		t_DBMgrLogDevice;

//---------------------------------------------------------------------------

CDBMgrLogDevice::CDBMgrLogDevice()
{
	m_strDBTName = "log_device";
}
//---------------------------------------------------------------------------

CDBMgrLogDevice::~CDBMgrLogDevice()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDevice::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [log_device][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDevice::LoadDB(TListDBLogDevice& tDBLogDeviceList)
{
    UINT32 nReadCnt = 0;
    DB_LOG_DEVICE dld;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target, "
						"notify_type, notify_id, "
						"host_id, user_id, policy_type, op_type, "
						"reg_svr_id, sync_svr_step, "
						"block_type, dv_instance_id "
						"FROM log_device WHERE used_flag=1;");
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

		dld.nBlockType				= GetDBField_Int(nIndex++);
		dld.strDvInstanceID			= GetDBField_String(nIndex++);

		{
			QueryToMem(dld.strDvInstanceID);
		}

        tDBLogDeviceList.push_back(dld);
        if(m_nLoadMaxID < UINT32(dld.nID))	m_nLoadMaxID = dld.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = (UINT32)tDBLogDeviceList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDevice::InsertLogDevice(DB_LOG_DEVICE& dld)
{
	String strDvInstanceID = MemToQuery(dld.strDvInstanceID);

	m_strQuery = SPrintf("INSERT INTO log_device(used_flag, reg_date, evt_time, evt_ecode, skip_target, "
									"notify_type, notify_id, "
									"host_id, user_id, policy_type, op_type, "
									"reg_svr_id, sync_svr_step, "
									"block_type, dv_instance_id)"
    								"VALUES"
									"(%u, %u, %u, %u, %u, "
									"%u, %u, "
									"%u, %u, %u, %u, "
									"%u, %u, "
									"%u, '%s');",
									dld.nUsedFlag, dld.nRegDate, dld.nEvtTime, dld.nEvtErrCode, dld.nSkipTarget,
									dld.nNotifyType, dld.nNotifyID, 
									dld.nHostID, dld.nUserID, dld.nPolicyType, dld.nOpType, 
									dld.nRegSvrID, dld.nSyncSvrStep,
									dld.nBlockType, dld.strDvInstanceID.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(dld.nID == 0)
		dld.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDevice::UpdateLogDevice(DB_LOG_DEVICE& dld)
{
	String strDvInstanceID = MemToQuery(dld.strDvInstanceID);

	m_strQuery = SPrintf("UPDATE log_device SET reg_date=%u, evt_time=%u, evt_ecode=%u, skip_target=%u,"
								"notify_type=%u, notify_id=%u, "
								"host_id=%u, user_id=%u, policy_type=%u, op_type=%u, "
								"reg_svr_id=%u, sync_svr_step=%u, "
								"block_type=%u, dv_instance_id='%s' "
								"WHERE id=%u;",
								dld.nRegDate, dld.nEvtTime, dld.nEvtErrCode, dld.nSkipTarget,
								dld.nNotifyType, dld.nNotifyID, 
								dld.nHostID, dld.nUserID, dld.nPolicyType, dld.nOpType, 
								dld.nRegSvrID, dld.nSyncSvrStep,
								dld.nBlockType, dld.strDvInstanceID.c_str(),
								dld.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrLogDevice::DeleteLogDevice(UINT32 nID)
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

INT32	CDBMgrLogDevice::LoadExecute(PVOID lpTempletList)
{
	TListDBLogDevice* ptDBList = (TListDBLogDevice*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------


INT32	CDBMgrLogDevice::InsertExecute(PVOID lpTemplet)
{
	PDB_LOG_DEVICE pd_t = (PDB_LOG_DEVICE)lpTemplet;

    return InsertLogDevice(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDevice::UpdateExecute(PVOID lpTemplet)
{
	PDB_LOG_DEVICE pd_t = (PDB_LOG_DEVICE)lpTemplet;

    return UpdateLogDevice(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDevice::DeleteExecute(UINT32 nID)
{
	return DeleteLogDevice(nID);
}
//---------------------------------------------------------------------------

INT32	CDBMgrLogDevice::LoadDB(UINT32 nLogMode, UINT32 nLogNum, TListDBLogDevice& tDBLogDeviceList)
{
	UINT32 nReadCnt = 0;
	DB_LOG_DEVICE dls;

	INT32 nIndex = 0;
	INT32 nContinue = 0;

	m_strQuery = SPrintf("SELECT id, reg_date, evt_time, evt_ecode, skip_target, "
		"notify_type, notify_id, "
		"host_id, user_id, policy_type, op_type, "
		"reg_svr_id, sync_svr_step, "
		"block_type, dv_instance_id "
		"FROM log_device WHERE used_flag=1;");
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
		dls.strDvInstanceID			= GetDBField_String(nIndex++);

		{
			QueryToMem(dls.strDvInstanceID);
		}

		tDBLogDeviceList.push_back(dls);
		if(m_nLoadMaxID < UINT32(dls.nID))	m_nLoadMaxID = dls.nID;
		nReadCnt++;
	}while(Next());
	m_nLoadNumber = (UINT32)tDBLogDeviceList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
	return 0;
}
//---------------------------------------------------------------------------