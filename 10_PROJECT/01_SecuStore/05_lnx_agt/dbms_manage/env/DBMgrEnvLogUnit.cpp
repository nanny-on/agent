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
#include "DBMgrEnvLogUnit.h"

//---------------------------------------------------------------------------

CDBMgrEnvLogUnit*		t_DBMgrEnvLogUnit;

//---------------------------------------------------------------------------

CDBMgrEnvLogUnit::CDBMgrEnvLogUnit()
{
	m_strDBTName = "env_log_unit";
}
//---------------------------------------------------------------------------

CDBMgrEnvLogUnit::~CDBMgrEnvLogUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLogUnit::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [env_log_unit][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLogUnit::LoadDB(TListDBEnvLogUnit& tDBEnvLogUnitList)
{
    UINT32 nReadCnt = 0;
    DB_ENV_LOG_UNIT deleu;

    INT32 nIndex = 0;
    String	strValue;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", evt_type, evt_code, evt_ecode "
						", subject_type, subject_info, target_type, target_info"
						", object_type, object_code, object_info"
						", operation_type, event_descr"
						", record_mode, sms_value"
						", ext_info"
						" FROM env_log_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = deleu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		deleu.nEvtType				= GetDBField_UInt(nIndex++);
		deleu.nEvtCode				= GetDBField_UInt(nIndex++);
		deleu.nEvtErrCode			= GetDBField_UInt(nIndex++);
		deleu.nSubjectType			= GetDBField_UInt(nIndex++);
		deleu.strSubjectInfo		= GetDBField_String(nIndex++);
		deleu.nTargetType			= GetDBField_UInt(nIndex++);
		deleu.strTargetInfo			= GetDBField_String(nIndex++);
		deleu.nObjectType			= GetDBField_UInt(nIndex++);
		deleu.nObjectCode			= GetDBField_UInt(nIndex++);
		deleu.strObjectInfo			= GetDBField_String(nIndex++);
		deleu.nOperationType		= GetDBField_UInt(nIndex++);
		deleu.strEvtDescr			= GetDBField_String(nIndex++);

		deleu.nRecordMode			= GetDBField_UInt(nIndex++);
		deleu.strSMSValue			= GetDBField_String(nIndex++);

		{
			strValue				= GetDBField_String(nIndex++);
			StrToMapID_Str(strValue, deleu.tIDMapStrExtInfo);
		}
        
        tDBEnvLogUnitList.push_back(deleu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBEnvLogUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLogUnit::InsertEnvLogUnit(DB_ENV_LOG_UNIT& deleu)
{
	DB_PO_HEADER& tDPH = deleu.tDPH;
   	String strExtInfo;
	strExtInfo = MapIDToStr_Str(deleu.tIDMapStrExtInfo);


	m_strQuery = SPrintf("INSERT INTO env_log_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", evt_type, evt_code, evt_ecode"
						", subject_type, subject_info, target_type, target_info"
						", object_type, object_code, object_info"
						", operation_type, event_descr"
						", record_mode, sms_value"
						", ext_info"
						") VALUES (%s"
						", %u, %u, %u"
						", %u, '%s', %u, '%s'"
						", %u, %u, '%s'"
						", %u, '%s'"
						", %u, '%s'"
						", '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						deleu.nEvtType, deleu.nEvtCode, deleu.nEvtErrCode, 
						deleu.nSubjectType, deleu.strSubjectInfo.c_str(), deleu.nTargetType, deleu.strTargetInfo.c_str(),
						deleu.nObjectType, deleu.nObjectCode, deleu.strObjectInfo.c_str(),
						deleu.nOperationType, deleu.strEvtDescr.c_str(), 
						deleu.nRecordMode, deleu.strSMSValue.c_str(),
						strExtInfo.c_str());
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLogUnit::UpdateEnvLogUnit(DB_ENV_LOG_UNIT& deleu)
{
	DB_PO_HEADER& tDPH = deleu.tDPH;
	String strExtInfo;
	strExtInfo = MapIDToStr_Str(deleu.tIDMapStrExtInfo);


	m_strQuery = SPrintf("UPDATE env_log_unit SET %s"
						", evt_type=%u, evt_code=%u, evt_ecode=%u"
						", subject_type=%u, subject_info='%s', target_type=%u, target_info='%s'"
						", object_type=%u, object_code=%u, object_info='%s'"
						", operation_type=%u, event_descr='%s'"
						", record_mode=%u, sms_value='%s'"
						", ext_info='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						deleu.nEvtType, deleu.nEvtCode, deleu.nEvtErrCode, 
						deleu.nSubjectType, deleu.strSubjectInfo.c_str(), deleu.nTargetType, deleu.strTargetInfo.c_str(),
						deleu.nObjectType, deleu.nObjectCode, deleu.strObjectInfo.c_str(),
						deleu.nOperationType, deleu.strEvtDescr.c_str(), 
						deleu.nRecordMode, deleu.strSMSValue.c_str(),
						strExtInfo.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLogUnit::DeleteEnvLogUnit(UINT32 nID)
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

INT32	CDBMgrEnvLogUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBEnvLogUnit* ptDBList = (TListDBEnvLogUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvLogUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_ENV_LOG_UNIT pd_t = (PDB_ENV_LOG_UNIT)lpTemplet;

    return InsertEnvLogUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvLogUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_ENV_LOG_UNIT pd_t = (PDB_ENV_LOG_UNIT)lpTemplet;

    return UpdateEnvLogUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvLogUnit::DeleteExecute(UINT32 nID)
{
	return DeleteEnvLogUnit(nID);
}
//---------------------------------------------------------------------------






