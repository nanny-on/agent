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
#include "DBMgrPoSvrInfoUdt.h"

//---------------------------------------------------------------------------

CDBMgrPoSvrInfoUdt*		t_DBMgrPoSvrInfoUdt = NULL;

//---------------------------------------------------------------------------

CDBMgrPoSvrInfoUdt::CDBMgrPoSvrInfoUdt()
{
	m_strDBTName = "po_svr_info_udt";
}
//---------------------------------------------------------------------------

CDBMgrPoSvrInfoUdt::~CDBMgrPoSvrInfoUdt()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSvrInfoUdt::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSvrInfoUdt::LoadDB(TListDBPoSvrInfoUdt& tDBPoSvrInfoUdtList)
{
    UINT32 nReadCnt = 0;
    DB_PO_SVR_INFO_UDT dpsiu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", chk_svr_info_list, data_svr_info_list"
						" FROM po_svr_info_udt WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpsiu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpsiu.strChkSvrInfoList		= GetDBField_String(nIndex++);
		dpsiu.strDataSvrInfoList	= GetDBField_String(nIndex++);
        
        tDBPoSvrInfoUdtList.push_back(dpsiu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoSvrInfoUdtList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSvrInfoUdt::InsertPoSvrInfoUdt(DB_PO_SVR_INFO_UDT& dpsiu)
{
	DB_PO_HEADER& tDPH = dpsiu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_svr_info_udt("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", chk_svr_info_list, data_svr_info_list"
						") VALUES (%s"
                        ", '%s', '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpsiu.strChkSvrInfoList.c_str(), dpsiu.strDataSvrInfoList.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSvrInfoUdt::UpdatePoSvrInfoUdt(DB_PO_SVR_INFO_UDT& dpsiu)
{
	DB_PO_HEADER& tDPH = dpsiu.tDPH;

	m_strQuery = SPrintf("UPDATE po_svr_info_udt SET %s"
						", chk_svr_info_list='%s', data_svr_info_list='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpsiu.strChkSvrInfoList.c_str(), dpsiu.strDataSvrInfoList.c_str(), 
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSvrInfoUdt::DeletePoSvrInfoUdt(UINT32 nID)
{
    m_strQuery = SPrintf("UPDATE po_svr_info_udt SET used_flag=0 WHERE id=%u;", nID);
    
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

INT32	CDBMgrPoSvrInfoUdt::LoadExecute(PVOID lpTempletList)
{
	TListDBPoSvrInfoUdt* ptDBList = (TListDBPoSvrInfoUdt*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSvrInfoUdt::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_SVR_INFO_UDT pd_t = (PDB_PO_SVR_INFO_UDT)lpTemplet;

    return InsertPoSvrInfoUdt(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSvrInfoUdt::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_SVR_INFO_UDT pd_t = (PDB_PO_SVR_INFO_UDT)lpTemplet;

    return UpdatePoSvrInfoUdt(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSvrInfoUdt::DeleteExecute(UINT32 nID)
{
	return DeletePoSvrInfoUdt(nID);
}
//---------------------------------------------------------------------------






