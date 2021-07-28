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
#include "DBMgrInfoProcReputation.h"

//---------------------------------------------------------------------------

CDBMgrInfoProcReputation*		t_DBMgrInfoProcReputation = NULL;

//---------------------------------------------------------------------------

CDBMgrInfoProcReputation::CDBMgrInfoProcReputation()
{
	m_strDBTName = "info_proc_reputation";
}
//---------------------------------------------------------------------------

CDBMgrInfoProcReputation::~CDBMgrInfoProcReputation()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrInfoProcReputation::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [info_proc_reputation][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrInfoProcReputation::LoadDB(TListDBInfoProcReputation& tDBInfoProcReputationList)
{
    UINT32 nReadCnt = 0;
    DB_INFO_PROC_REPUTATION data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf("SELECT id, edit_date"
						", site_file_id, reputation, num_of_user"
						" FROM info_proc_reputation;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;

		data.nID			= GetDBField_UInt(nIndex++);
		data.nEditDate		= GetDBField_UInt(nIndex++);

		data.nSiteFileID	= GetDBField_UInt(nIndex++);
		data.nReputation	= GetDBField_UInt(nIndex++);
		data.nNumOfUser		= GetDBField_UInt(nIndex++);

        tDBInfoProcReputationList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))
			m_nLoadMaxID = data.nID;
        nReadCnt++;
    
    }while(Next());
	m_nLoadNumber = tDBInfoProcReputationList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrInfoProcReputation::InsertInfoProcReputation(DB_INFO_PROC_REPUTATION& data)
{
	m_strQuery = SPrintf("INSERT INTO info_proc_reputation("
						"edit_date "
						", site_file_id, reputation, num_of_user "
						") VALUES ("
						" %u"
						", %u, %u, %u "
						");",                        
						data.nEditDate, 
						data.nSiteFileID, data.nReputation, data.nNumOfUser);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrInfoProcReputation::UpdateInfoProcReputation(DB_INFO_PROC_REPUTATION& data)
{
	m_strQuery = SPrintf("UPDATE info_proc_reputation SET "
						"edit_date=%u"
						", site_file_id=%u, reputation=%u, num_of_user=%u "
						" WHERE id=%u;",
						data.nEditDate, 
						data.nSiteFileID, data.nReputation, data.nNumOfUser, 
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrInfoProcReputation::DeleteInfoProcReputation(UINT32 nID)
{
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

INT32	CDBMgrInfoProcReputation::LoadExecute(PVOID lpTempletList)
{
	TListDBInfoProcReputation* ptDBList = (TListDBInfoProcReputation*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrInfoProcReputation::InsertExecute(PVOID lpTemplet)
{
	PDB_INFO_PROC_REPUTATION pd_t = (PDB_INFO_PROC_REPUTATION)lpTemplet;

    return InsertInfoProcReputation(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrInfoProcReputation::UpdateExecute(PVOID lpTemplet)
{
	PDB_INFO_PROC_REPUTATION pd_t = (PDB_INFO_PROC_REPUTATION)lpTemplet;

    return UpdateInfoProcReputation(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrInfoProcReputation::DeleteExecute(UINT32 nID)
{
	return DeleteInfoProcReputation(nID);
}
//---------------------------------------------------------------------------






