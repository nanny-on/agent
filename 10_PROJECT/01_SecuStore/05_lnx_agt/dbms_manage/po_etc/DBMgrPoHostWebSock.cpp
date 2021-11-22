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
#include "DBMgrPoHostWebSock.h"

//---------------------------------------------------------------------------

CDBMgrPoHostWebSock*		t_DBMgrPoHostWebSock = NULL;

//---------------------------------------------------------------------------

CDBMgrPoHostWebSock::CDBMgrPoHostWebSock()
{
	m_strDBTName = "po_host_web_sock";
}
//---------------------------------------------------------------------------

CDBMgrPoHostWebSock::~CDBMgrPoHostWebSock()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostWebSock::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_host_web_sock][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostWebSock::LoadDB(TListDBPoHostWebSock& tDBPoHostWebSockList)
{
    UINT32 nReadCnt = 0;
    DB_PO_HOST_WEB_SOCK data;
	String strPolicyType, strPolicyOpt;
    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", svr_port, web_key"
						", policy_type, policy_opt"
						" FROM po_host_web_sock WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nSvrPort				= GetDBField_UInt(nIndex++);
		data.strWebKey				= GetDBField_String(nIndex++);
		
		strPolicyType				= GetDBField_String(nIndex++);
		HexToMap_64(strPolicyType, data.tPolicyTypeMap, SS_ADMIN_RIGHT_TYPE_CLASS_NUM_POLICY);
		strPolicyOpt				= GetDBField_String(nIndex++);
		HexToMap_64(strPolicyOpt, data.tPolicyOptMap, SS_POLICY_INDEX_TOTAL_NUMBER);
        
        tDBPoHostWebSockList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoHostWebSockList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostWebSock::InsertPoHostWebSock(DB_PO_HOST_WEB_SOCK& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	String strPolicyType, strPolicyOpt;
	{
		MapToHex_64(data.tPolicyTypeMap, strPolicyType,	SS_ADMIN_RIGHT_TYPE_CLASS_NUM_POLICY);
		MapToHex_64(data.tPolicyOptMap, strPolicyOpt, SS_POLICY_INDEX_TOTAL_NUMBER);
	}

	m_strQuery = SPrintf("INSERT INTO po_host_web_sock("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", svr_port, web_key"
						", policy_type, policy_opt"
						") VALUES (%s"
                        ", %u, '%s'"
						", '%s', '%s'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nSvrPort, data.strWebKey.c_str(),
						strPolicyType.c_str(), strPolicyOpt.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostWebSock::UpdatePoHostWebSock(DB_PO_HOST_WEB_SOCK& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	String strPolicyType, strPolicyOpt;
	{
		MapToHex_64(data.tPolicyTypeMap, strPolicyType,	SS_ADMIN_RIGHT_TYPE_CLASS_NUM_POLICY);
		MapToHex_64(data.tPolicyOptMap,	strPolicyOpt, SS_POLICY_INDEX_TOTAL_NUMBER);
	}

	m_strQuery = SPrintf("UPDATE po_host_web_sock SET %s"
						", svr_port=%u, web_key='%s'"
						", policy_type='%s', policy_opt='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						data.nSvrPort, data.strWebKey.c_str(),
						strPolicyType.c_str(), strPolicyOpt.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostWebSock::DeletePoHostWebSock(UINT32 nID)
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

INT32	CDBMgrPoHostWebSock::LoadExecute(PVOID lpTempletList)
{
	TListDBPoHostWebSock* ptDBList = (TListDBPoHostWebSock*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostWebSock::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_HOST_WEB_SOCK pd_t = (PDB_PO_HOST_WEB_SOCK)lpTemplet;

    return InsertPoHostWebSock(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostWebSock::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_HOST_WEB_SOCK pd_t = (PDB_PO_HOST_WEB_SOCK)lpTemplet;

    return UpdatePoHostWebSock(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostWebSock::DeleteExecute(UINT32 nID)
{
	return DeletePoHostWebSock(nID);
}
//---------------------------------------------------------------------------






