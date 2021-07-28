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
#include "DBMgrHost.h"

//---------------------------------------------------------------------------

CDBMgrHost*		t_DBMgrHost = NULL;

//---------------------------------------------------------------------------

CDBMgrHost::CDBMgrHost()
{
	m_strDBTName = "host";
}
//---------------------------------------------------------------------------

CDBMgrHost::~CDBMgrHost()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHost::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHost::LoadDB(TListDBHost& tDBHostList)
{
    UINT32 nReadCnt = 0;
    DB_HOST dh;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, name, name_alias, work_group, con_date, dcon_date, "
						"pri_ip, nat_ip, mac, guid, "
						"policy_id, policy_seqno, policy_time, "
						"user_id, user_unicode FROM host WHERE used_flag <> 0;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		dh.nID					= GetDBField_Int(nIndex++) ;
		dh.nUsedFlag			= GetDBField_Int(nIndex++) ;
		dh.strName				= GetDBField_String(nIndex++);
		dh.strNameAlias			= GetDBField_String(nIndex++);
		dh.strWorkGroup			= GetDBField_String(nIndex++);
		dh.nConDate				= GetDBField_Int(nIndex++);
		dh.nDConDate			= GetDBField_Int(nIndex++);
		dh.strPriIP				= GetDBField_String(nIndex++);
		dh.strNatIP				= GetDBField_String(nIndex++);
		dh.strMac				= GetDBField_String(nIndex++);
		dh.strGUID				= GetDBField_String(nIndex++);
		
		m_strValue				= GetDBField_String(nIndex++);		HexToMap(m_strValue, dh.tPoIDMap, SS_POLICY_INDEX_TOTAL_NUMBER);
		m_strValue				= GetDBField_String(nIndex++);		HexToMap(m_strValue, dh.tPoSeqNoMap, SS_POLICY_INDEX_TOTAL_NUMBER);
		m_strValue				= GetDBField_String(nIndex++);		HexToMap(m_strValue, dh.tPoTimeMap, SS_POLICY_INDEX_TOTAL_NUMBER);
		
		dh.nUserID				= GetDBField_Int(nIndex++);
		dh.strUserUnicode		= GetDBField_String(nIndex++);

        tDBHostList.push_back(dh);
        if(m_nLoadMaxID < UINT32(dh.nID))
			m_nLoadMaxID = dh.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBHostList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHost::InsertHost(DB_HOST& dh)
{
	String strPoID, strPoSeqNo, strPoTime;

	MapToHex(dh.tPoIDMap, strPoID, SS_POLICY_INDEX_TOTAL_NUMBER);
	MapToHex(dh.tPoSeqNoMap, strPoSeqNo, SS_POLICY_INDEX_TOTAL_NUMBER);
	MapToHex(dh.tPoTimeMap,  strPoTime, SS_POLICY_INDEX_TOTAL_NUMBER);

	m_strQuery = SPrintf("INSERT INTO host (used_flag, "
									"name, name_alias, work_group, con_date, dcon_date, "
									"pri_ip, nat_ip, mac, guid, "
									"policy_id, policy_seqno, policy_time, "
									"user_id, user_unicode) "
                                    "VALUES (%u, "
                                    "'%s', '%s', '%s', %u, %u, "
                                    "'%s', '%s', '%s', '%s', "
									"'%s', '%s', '%s', "
									"%u, '%s');",
                                    dh.nUsedFlag, 
									dh.strName.c_str(), dh.strNameAlias.c_str(), dh.strWorkGroup.c_str(), dh.nConDate, dh.nDConDate,
									dh.strPriIP.c_str(), dh.strNatIP.c_str(), dh.strMac.c_str(), dh.strGUID.c_str(),
									strPoID.c_str(), strPoSeqNo.c_str(), strPoTime.c_str(),
									dh.nUserID, dh.strUserUnicode.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
	if(dh.nID == 0)
		dh.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHost::UpdateHost(DB_HOST& dh)
{
	String strPoID, strPoSeqNo, strPoTime;

	MapToHex(dh.tPoIDMap, strPoID, SS_POLICY_INDEX_TOTAL_NUMBER);
	MapToHex(dh.tPoSeqNoMap, strPoSeqNo, SS_POLICY_INDEX_TOTAL_NUMBER);
	MapToHex(dh.tPoTimeMap,  strPoTime, SS_POLICY_INDEX_TOTAL_NUMBER);

	m_strQuery = SPrintf("UPDATE host SET "
						"used_flag=%u, name='%s', name_alias='%s', work_group='%s', con_date=%u, dcon_date=%u, "
						"pri_ip='%s', nat_ip='%s', mac='%s', guid='%s', "
						"policy_id='%s', policy_seqno='%s', policy_time='%s', "
						"user_id=%u, user_unicode='%s' "
						"WHERE id=%u;",
							1, dh.strName.c_str(), dh.strNameAlias.c_str(), dh.strWorkGroup.c_str(), dh.nConDate, dh.nDConDate,
							dh.strPriIP.c_str(), dh.strNatIP.c_str(), dh.strMac.c_str(), dh.strGUID.c_str(),
							strPoID.c_str(), strPoSeqNo.c_str(), strPoTime.c_str(),
							dh.nUserID, dh.strUserUnicode.c_str(), 
							dh.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHost::DeleteHost(UINT32 nID)
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

INT32	CDBMgrHost::LoadExecute(PVOID lpTempletList)
{
	TListDBHost* ptDBList = (TListDBHost*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHost::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST pd_t = (PDB_HOST)lpTemplet;

    return InsertHost(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHost::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST pd_t = (PDB_HOST)lpTemplet;

    return UpdateHost(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHost::DeleteExecute(UINT32 nID)
{
	return DeleteHost(nID);
}
//---------------------------------------------------------------------------






