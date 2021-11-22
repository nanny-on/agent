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


#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrEnvLocal.h"

//---------------------------------------------------------------------------

CDBMgrEnvLocal*		t_DBMgrEnvLocal = NULL;

//---------------------------------------------------------------------------

CDBMgrEnvLocal::CDBMgrEnvLocal()
{
	m_strDBTName = "env_local";
}
//---------------------------------------------------------------------------

CDBMgrEnvLocal::~CDBMgrEnvLocal()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLocal::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [env_local][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLocal::LoadDB(TListDBEnvLocal& tDBEnvLocalList)
{
    UINT32 nReadCnt = 0;
    DB_ENV_LOCAL data;

    INT32 nIndex = 0;
	String	strNValue;
	String	strSValue;
	m_strQuery = SPrintf("SELECT id, used_flag, reg_date"
						", chk_type, chk_code"
						", n_value, s_value"
						" FROM env_local WHERE used_flag=1;");
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	do
	{
    	nIndex = 0;

		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedFlag				= GetDBField_Int(nIndex++);
		data.nRegDate				= GetDBField_Int(nIndex++);

		data.nChkType				= GetDBField_Int(nIndex++);
		data.nChkCode				= GetDBField_Int(nIndex++);

		strNValue			= GetDBField_String(nIndex++);		StrToMapID_ID(strNValue, data.tNValueMap);
		strSValue			= GetDBField_String(nIndex++);		StrToMapID_Str(strSValue, data.tSValueMap);		

        tDBEnvLocalList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBEnvLocalList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLocal::InsertEnvLocal(DB_ENV_LOCAL& data)
{
	String	strNValue			= MapIDToStr_ID(data.tNValueMap);
	String	strSValue			= MapIDToStr_Str(data.tSValueMap);		
	
	m_strQuery = SPrintf("INSERT INTO env_local(used_flag, reg_date"
									", chk_type, chk_code"
									", n_value, s_value"
									") VALUES (%u, %u"
									", %u, %u"
									", '%s', '%s'"
									");",
									data.nUsedFlag, data.nRegDate, 
									data.nChkType, data.nChkCode,
									strNValue.c_str(), strSValue.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLocal::UpdateEnvLocal(DB_ENV_LOCAL& data)
{	
	String	strNValue			= MapIDToStr_ID(data.tNValueMap);
	String	strSValue			= MapIDToStr_Str(data.tSValueMap);		
	
	m_strQuery = SPrintf("UPDATE env_local SET reg_date=%u"
						", chk_type=%u, chk_code=%u"
						", n_value='%s', s_value='%s'"
						" WHERE id=%u;",
						data.nRegDate, 
						data.nChkType, data.nChkCode,
						strNValue.c_str(), strSValue.c_str(),
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvLocal::DeleteEnvLocal(UINT32 nID)
{
    m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_DELETE_FAIL;

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

INT32	CDBMgrEnvLocal::LoadExecute(PVOID lpTempletList)
{
	TListDBEnvLocal* ptDBList = (TListDBEnvLocal*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvLocal::InsertExecute(PVOID lpTemplet)
{
	PDB_ENV_LOCAL pd_t = (PDB_ENV_LOCAL)lpTemplet;

    return InsertEnvLocal(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvLocal::UpdateExecute(PVOID lpTemplet)
{
	PDB_ENV_LOCAL pd_t = (PDB_ENV_LOCAL)lpTemplet;

    return UpdateEnvLocal(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvLocal::DeleteExecute(UINT32 nID)
{
	return DeleteEnvLocal(nID);
}
//---------------------------------------------------------------------------






