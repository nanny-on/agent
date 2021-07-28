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
#include "DBMgrHostStatusPoNc.h"

//---------------------------------------------------------------------------

CDBMgrHostStatusPoNc*		t_DBMgrHostStatusPoNc = NULL;

//---------------------------------------------------------------------------

CDBMgrHostStatusPoNc::CDBMgrHostStatusPoNc()
{
	m_strDBTName = "host_status_po_nc";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrHostStatusPoNc::~CDBMgrHostStatusPoNc()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostStatusPoNc::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [host][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostStatusPoNc::LoadDB(TListDBHostStatusPoNc& tDBHostStatusPoNcList)
{
    UINT32 nReadCnt = 0;
    DB_HOST_STATUS_PO_NC data;
	String strWorkMode, strPtnVer;
    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, host_id"
						", work_mode, ptn_ver"
						" FROM host_status_po_nc WHERE used_flag <> 0;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		data.nID				= GetDBField_Int(nIndex++);
		data.nUsedFlag			= GetDBField_Int(nIndex++);		
		data.nHostID			= GetDBField_Int(nIndex++);

		strWorkMode		= GetDBField_String(nIndex++);
		HexToMap(strWorkMode, data.tWorkModeMap, SS_HOST_STATUS_PO_NC_INFO_INDEX_LAST);
		strPtnVer		= GetDBField_String(nIndex++);
		StringToMapIDStr(strPtnVer, data.tPtnVerMap, SS_HOST_STATUS_PO_NC_INFO_INDEX_LAST);

        tDBHostStatusPoNcList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))
			m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBHostStatusPoNcList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostStatusPoNc::InsertHostStatusPoNc(DB_HOST_STATUS_PO_NC& data)
{
	String strWorkMode, strPtnVer;

	MapToHex(data.tWorkModeMap,	strWorkMode, SS_HOST_STATUS_PO_NC_INFO_INDEX_LAST);
	MapIDStrToString(data.tPtnVerMap, strPtnVer, SS_HOST_STATUS_PO_NC_INFO_INDEX_LAST);
	
	m_strQuery = SPrintf("INSERT INTO host_status_po_nc (used_flag, host_id"
									", work_mode, ptn_ver"
									") VALUES ( %u, %u"
									", '%s', '%s'"
									");",
                                    data.nUsedFlag,	data.nHostID, 
									strWorkMode.c_str(), strPtnVer.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrHostStatusPoNc::UpdateHostStatusPoNc(DB_HOST_STATUS_PO_NC& data)
{
	String strWorkMode, strPtnVer;

	MapToHex(data.tWorkModeMap,	strWorkMode, SS_HOST_STATUS_PO_NC_INFO_INDEX_LAST);
	MapIDStrToString(data.tPtnVerMap, strPtnVer, SS_HOST_STATUS_PO_NC_INFO_INDEX_LAST);
	
	m_strQuery = SPrintf("UPDATE host_status_po_nc SET "
						" used_flag=%u, host_id=%u"
						", work_mode='%s', ptn_ver='%s'"
						" WHERE id=%u;",
						data.nUsedFlag,	data.nHostID, 
						strWorkMode.c_str(), strPtnVer.c_str(), 
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

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
//---------------------------------------------------------------------------

INT32	CDBMgrHostStatusPoNc::LoadExecute(PVOID lpTempletList)
{
	TListDBHostStatusPoNc* ptDBList = (TListDBHostStatusPoNc*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostStatusPoNc::InsertExecute(PVOID lpTemplet)
{
	PDB_HOST_STATUS_PO_NC pd_t = (PDB_HOST_STATUS_PO_NC)lpTemplet;

    return InsertHostStatusPoNc(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostStatusPoNc::UpdateExecute(PVOID lpTemplet)
{
	PDB_HOST_STATUS_PO_NC pd_t = (PDB_HOST_STATUS_PO_NC)lpTemplet;

    return UpdateHostStatusPoNc(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrHostStatusPoNc::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






