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
#include "DBMgrEnvNotifyInfo.h"

//---------------------------------------------------------------------------

CDBMgrEnvNotifyInfo*		t_DBMgrEnvNotifyInfo;

//---------------------------------------------------------------------------

CDBMgrEnvNotifyInfo::CDBMgrEnvNotifyInfo()
{
	m_strDBTName = "env_notify_info";
}
//---------------------------------------------------------------------------

CDBMgrEnvNotifyInfo::~CDBMgrEnvNotifyInfo()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvNotifyInfo::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvNotifyInfo::LoadDB(TListDBEnvNotifyInfo& tDBEnvNotifyInfoList)
{
    UINT32 nReadCnt = 0;
    DB_ENV_NOTIFY_INFO deni;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", policy_type, policy_code"
						", pos_type, show_size, show_pos, show_time"
						", msg_fmt_type, msg_info, notify_before_day"
						", match_info"
						", notify_skip_day"
						" FROM env_notify_info WHERE used_flag=1;");
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

	do
	{
		nIndex = 0;
		DB_PO_HEADER& tDPH = deni.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		deni.nPolicyType			= GetDBField_UInt64(nIndex++);
		deni.nPolicyCode			= GetDBField_UInt64(nIndex++);
		deni.nPosType				= GetDBField_UInt(nIndex++);
		deni.nShowSize				= GetDBField_UInt(nIndex++);
		deni.nShowPos				= GetDBField_UInt(nIndex++);
		deni.nShowTime				= GetDBField_UInt(nIndex++);
		deni.nMsgFmtType			= GetDBField_UInt(nIndex++);
		deni.strMsgInfo				= GetDBField_String(nIndex++);

		deni.nNotifyBeforeDay		= GetDBField_UInt(nIndex++);
		deni.strMatchInfo			= GetDBField_String(nIndex++);
		deni.nNotifySkipDay			= GetDBField_UInt(nIndex++);

		tDBEnvNotifyInfoList.push_back(deni);
		if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
		nReadCnt++;
	}while(Next());
	m_nLoadNumber = tDBEnvNotifyInfoList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvNotifyInfo::InsertEnvNotifyInfo(DB_ENV_NOTIFY_INFO& deni)
{
	DB_PO_HEADER& tDPH = deni.tDPH;

	m_strQuery = SPrintf("INSERT INTO env_notify_info("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", policy_type, policy_code, pos_type"
						", show_size, show_pos, show_time"
						", msg_fmt_type, msg_info"
						", notify_before_day"
						", match_info"
						", notify_skip_day"
						") VALUES (%s"
                        ", '%llu', '%llu', %u"
						", %u, %u, %u"
						", %u, '%s'"
						", %u"
						", '%s'"
						", %u"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						deni.nPolicyType, deni.nPolicyCode, deni.nPosType, 
						deni.nShowSize, deni.nShowPos, deni.nShowTime,
						deni.nMsgFmtType, deni.strMsgInfo.c_str(), 
						deni.nNotifyBeforeDay,
						deni.strMatchInfo.c_str(),
						deni.nNotifySkipDay
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvNotifyInfo::UpdateEnvNotifyInfo(DB_ENV_NOTIFY_INFO& deni)
{
	DB_PO_HEADER& tDPH = deni.tDPH;

	m_strQuery = SPrintf("UPDATE env_notify_info SET %s"
						", policy_type='%llu', policy_code='%llu', pos_type=%u"
						", show_size=%u, show_pos=%u, show_time=%u"
						", msg_fmt_type=%u, msg_info='%s'"
						", notify_before_day=%u"
						", match_info='%s'"
						", notify_skip_day=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						deni.nPolicyType, deni.nPolicyCode,
						deni.nPosType, deni.nShowSize, deni.nShowPos, deni.nShowTime,
						deni.nMsgFmtType, deni.strMsgInfo.c_str(), 
						deni.nNotifyBeforeDay,
						deni.strMatchInfo.c_str(),
						deni.nNotifySkipDay,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvNotifyInfo::DeleteEnvNotifyInfo(UINT32 nID)
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

INT32	CDBMgrEnvNotifyInfo::LoadExecute(PVOID lpTempletList)
{
	TListDBEnvNotifyInfo* ptDBList = (TListDBEnvNotifyInfo*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvNotifyInfo::InsertExecute(PVOID lpTemplet)
{
	PDB_ENV_NOTIFY_INFO pd_t = (PDB_ENV_NOTIFY_INFO)lpTemplet;

    return InsertEnvNotifyInfo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvNotifyInfo::UpdateExecute(PVOID lpTemplet)
{
	PDB_ENV_NOTIFY_INFO pd_t = (PDB_ENV_NOTIFY_INFO)lpTemplet;

    return UpdateEnvNotifyInfo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvNotifyInfo::DeleteExecute(UINT32 nID)
{
	return DeleteEnvNotifyInfo(nID);
}
//---------------------------------------------------------------------------






