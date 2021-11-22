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
#include "DBMgrEnvLog.h"

//---------------------------------------------------------------------------

CDBMgrEnvLog*		t_DBMgrEnvLog;

//---------------------------------------------------------------------------

CDBMgrEnvLog::CDBMgrEnvLog()
{
	m_strDBTName = "env_log";
}
//---------------------------------------------------------------------------

CDBMgrEnvLog::~CDBMgrEnvLog()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLog::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLog::LoadDB(TListDBEnvLog& tDBEnvLogList)
{
    UINT32 nReadCnt = 0;
    DB_ENV_LOG dele;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", record_mode"
						", svr_load_mode, svr_load_num"
						", agt_load_mode, agt_load_num"
						", sch_time, use_time"
						" FROM env_log WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dele.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);

		dele.nRecordMode		= GetDBField_UInt(nIndex++);
		m_strValue				= GetDBField_String(nIndex++);
		HexToMap(m_strValue, dele.tSvrLoadModeMap, SS_ENV_LOG_INDEX_TOTAL_NUMBER);
		m_strValue				= GetDBField_String(nIndex++);
		HexToMap(m_strValue, dele.tSvrLoadNumMap, SS_ENV_LOG_INDEX_TOTAL_NUMBER);
		m_strValue				= GetDBField_String(nIndex++);
		HexToMap(m_strValue, dele.tAgtLoadModeMap, SS_ENV_LOG_INDEX_TOTAL_NUMBER);
		m_strValue				= GetDBField_String(nIndex++);
		HexToMap(m_strValue, dele.tAgtLoadNumMap, SS_ENV_LOG_INDEX_TOTAL_NUMBER);

		dele.nLogClearStTime	= GetDBField_UInt(nIndex++);
		dele.nLogClearUseTime	= GetDBField_UInt(nIndex++);

        tDBEnvLogList.push_back(dele);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBEnvLogList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLog::InsertEnvLog(DB_ENV_LOG& dele)
{
	String strSvrLoadMode, strSvrLoadNum, strAgtLoadMode, strAgtLoadNum;

	MapToHex(dele.tSvrLoadModeMap,	strSvrLoadMode,	SS_ENV_LOG_INDEX_TOTAL_NUMBER);
	MapToHex(dele.tSvrLoadNumMap,	strSvrLoadNum,	SS_ENV_LOG_INDEX_TOTAL_NUMBER);
	MapToHex(dele.tAgtLoadModeMap,	strAgtLoadMode, SS_ENV_LOG_INDEX_TOTAL_NUMBER);
	MapToHex(dele.tAgtLoadNumMap,	strAgtLoadNum,	SS_ENV_LOG_INDEX_TOTAL_NUMBER);

	DB_PO_HEADER& tDPH = dele.tDPH;
	m_strQuery = SPrintf("INSERT INTO env_log("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", record_mode"
						", svr_load_mode, svr_load_num"
						", agt_load_mode, agt_load_num"
						", sch_time, use_time"
						") VALUES (%s"
						", %u"
						", '%s', '%s'"
						", '%s', '%s'"
						", %u, %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dele.nRecordMode, 
						strSvrLoadMode.c_str(), strSvrLoadNum.c_str(),
						strAgtLoadMode.c_str(), strAgtLoadNum.c_str(),
						dele.nLogClearStTime, dele.nLogClearUseTime);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLog::UpdateEnvLog(DB_ENV_LOG& dele)
{
	String strSvrLoadMode, strSvrLoadNum, strAgtLoadMode, strAgtLoadNum;

	MapToHex(dele.tSvrLoadModeMap,	strSvrLoadMode,	SS_ENV_LOG_INDEX_TOTAL_NUMBER);
	MapToHex(dele.tSvrLoadNumMap,	strSvrLoadNum,	SS_ENV_LOG_INDEX_TOTAL_NUMBER);
	MapToHex(dele.tAgtLoadModeMap,	strAgtLoadMode, SS_ENV_LOG_INDEX_TOTAL_NUMBER);
	MapToHex(dele.tAgtLoadNumMap,	strAgtLoadNum,	SS_ENV_LOG_INDEX_TOTAL_NUMBER);

	DB_PO_HEADER& tDPH = dele.tDPH;
	m_strQuery = SPrintf("UPDATE env_log SET %s"
						", record_mode=%u"
						", svr_load_mode='%s', svr_load_num='%s'"
						", agt_load_mode='%s', agt_load_num='%s'"
						", sch_time=%u, use_time=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dele.nRecordMode,
						strSvrLoadMode.c_str(), strSvrLoadNum.c_str(),
						strAgtLoadMode.c_str(), strAgtLoadNum.c_str(),
						dele.nLogClearStTime, dele.nLogClearUseTime,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLog::DeleteEnvLog(UINT32 nID)
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

INT32	CDBMgrEnvLog::LoadExecute(PVOID lpTempletList)
{
	TListDBEnvLog* ptDBList = (TListDBEnvLog*)lpTempletList;

    return LoadDB(*ptDBList);
}

//---------------------------------------------------------------------------

INT32	CDBMgrEnvLog::InsertExecute(PVOID lpTemplet)
{
	PDB_ENV_LOG pd_t = (PDB_ENV_LOG)lpTemplet;

    return InsertEnvLog(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvLog::UpdateExecute(PVOID lpTemplet)
{
	PDB_ENV_LOG pd_t = (PDB_ENV_LOG)lpTemplet;

    return UpdateEnvLog(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvLog::DeleteExecute(UINT32 nID)
{
	return DeleteEnvLog(nID);
}
//---------------------------------------------------------------------------






