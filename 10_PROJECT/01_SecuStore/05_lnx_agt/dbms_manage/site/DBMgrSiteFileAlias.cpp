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
#include "DBMgrSiteFileAlias.h"

//---------------------------------------------------------------------------

CDBMgrSiteFileAlias*		t_DBMgrSiteFileAlias = NULL;

//---------------------------------------------------------------------------

CDBMgrSiteFileAlias::CDBMgrSiteFileAlias()
{
	m_strDBTName = "site_file_alias";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrSiteFileAlias::~CDBMgrSiteFileAlias()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteFileAlias::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteFileAlias::LoadDB(TListDBSiteFileAlias& tDBSiteFileAliasList)
{
    UINT32 nReadCnt = 0;
    DB_SITE_FILE_ALIAS data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, reg_date, used_mode"
						", reg_svr_id, sync_svr_step"
						", fe_key, file_path, file_name"
						" FROM site_file_alias;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;
    
	do
    {
    	nIndex = 0;
    	
		data.nID				= GetDBField_Int(nIndex++) ;
		data.nUsedFlag			= GetDBField_Int(nIndex++) ;
		data.nRegDate			= GetDBField_Int(nIndex++) ;
		data.nUsedMode			= GetDBField_Int(nIndex++) ;

		data.nRegSvrID			= GetDBField_Int(nIndex++) ;
		data.nSyncSvrStep		= GetDBField_Int(nIndex++);

		data.strFeKey			= GetDBField_String(nIndex++);
		data.strFilePath		= GetDBField_String(nIndex++);
		data.strFileName		= GetDBField_String(nIndex++);
		
		{
			QueryToMem(data.strFilePath);
			QueryToMem(data.strFileName);
		}
		
        tDBSiteFileAliasList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBSiteFileAliasList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteFileAlias::InsertSiteFileAlias(DB_SITE_FILE_ALIAS& data)
{
	String strFilePath	= MemToQuery(data.strFilePath);
	String strFileName	= MemToQuery(data.strFileName);

	m_strQuery = SPrintf("INSERT INTO site_file_alias (used_flag, reg_date, used_mode"
									", reg_svr_id, sync_svr_step"
									", fe_key, file_path, file_name"
									") VALUES ( "
									"%u, %u, %u, "
									"%u, %u, "
									"'%s', '%s', '%s'"
									");",
                                    data.nUsedFlag, data.nRegDate, data.nUsedMode, 
									data.nRegSvrID, data.nSyncSvrStep,
									data.strFeKey.c_str(), strFilePath.c_str(), strFileName.c_str()
									);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteFileAlias::UpdateSiteFileAlias(DB_SITE_FILE_ALIAS& data)
{
	String strFilePath		= MemToQuery(data.strFilePath);
	String strFileName		= MemToQuery(data.strFileName);

	m_strQuery = SPrintf("UPDATE site_file_alias SET "
						"  used_flag=%u, reg_date=%u, used_mode=%u"
						", reg_svr_id=%u, sync_svr_step=%u"
						", fe_key='%s', file_path='%s', file_name='%s'"
						"  WHERE id=%u;",
						data.nUsedFlag, data.nRegDate, data.nUsedMode, 
						data.nRegSvrID, data.nSyncSvrStep,
						data.strFeKey.c_str(), strFilePath.c_str(), strFileName.c_str(),
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteFileAlias::DeleteSiteFileAlias(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM site_file_alias WHERE id=%u;", nID);

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

INT32	CDBMgrSiteFileAlias::LoadExecute(PVOID lpTempletList)
{
	TListDBSiteFileAlias* ptDBList = (TListDBSiteFileAlias*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteFileAlias::InsertExecute(PVOID lpTemplet)
{
	PDB_SITE_FILE_ALIAS pd_t = (PDB_SITE_FILE_ALIAS)lpTemplet;

    return InsertSiteFileAlias(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteFileAlias::UpdateExecute(PVOID lpTemplet)
{
	PDB_SITE_FILE_ALIAS pd_t = (PDB_SITE_FILE_ALIAS)lpTemplet;

    return UpdateSiteFileAlias(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteFileAlias::DeleteExecute(UINT32 nID)
{
	return DeleteSiteFileAlias(nID);
}
//---------------------------------------------------------------------------






