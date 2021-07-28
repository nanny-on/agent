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
#include "DBMgrPoHostRmInfo.h"

//---------------------------------------------------------------------------

CDBMgrPoHostRmInfo*		t_DBMgrPoHostRmInfo = NULL;

//---------------------------------------------------------------------------

CDBMgrPoHostRmInfo::CDBMgrPoHostRmInfo()
{
	m_strDBTName = "po_host_rm_info";
}
//---------------------------------------------------------------------------

CDBMgrPoHostRmInfo::~CDBMgrPoHostRmInfo()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostRmInfo::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostRmInfo::LoadDB(TListDBPoHostRmInfo& tDBPoHostRmInfoList)
{
    UINT32 nReadCnt = 0;
    DB_PO_HOST_RM_INFO dphri;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", rm_pw_type, rm_pw"
						" FROM po_host_rm_info WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dphri.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dphri.nRmPwType				= GetDBField_UInt(nIndex++);
		dphri.strRmPw				= GetDBField_String(nIndex++);
        
        tDBPoHostRmInfoList.push_back(dphri);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBPoHostRmInfoList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostRmInfo::InsertPoHostRmInfo(DB_PO_HOST_RM_INFO& dphri)
{
	DB_PO_HEADER& tDPH = dphri.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_host_rm_info("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", rm_pw_type, rm_pw"
						") VALUES (%s "
                        ", %u, '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dphri.nRmPwType, dphri.strRmPw.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostRmInfo::UpdatePoHostRmInfo(DB_PO_HOST_RM_INFO& dphri)
{
	DB_PO_HEADER& tDPH = dphri.tDPH;

	m_strQuery = SPrintf("UPDATE po_host_rm_info SET %s"
						", rm_pw_type=%u, rm_pw='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dphri.nRmPwType, dphri.strRmPw.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoHostRmInfo::DeletePoHostRmInfo(UINT32 nID)
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

INT32	CDBMgrPoHostRmInfo::LoadExecute(PVOID lpTempletList)
{
	TListDBPoHostRmInfo* ptDBList = (TListDBPoHostRmInfo*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostRmInfo::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_HOST_RM_INFO pd_t = (PDB_PO_HOST_RM_INFO)lpTemplet;

    return InsertPoHostRmInfo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostRmInfo::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_HOST_RM_INFO pd_t = (PDB_PO_HOST_RM_INFO)lpTemplet;

    return UpdatePoHostRmInfo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoHostRmInfo::DeleteExecute(UINT32 nID)
{
	return DeletePoHostRmInfo(nID);
}
//---------------------------------------------------------------------------






