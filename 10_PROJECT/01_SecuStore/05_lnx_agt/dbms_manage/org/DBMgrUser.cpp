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
#include "DBMgrUser.h"

//---------------------------------------------------------------------------

CDBMgrUser*		t_DBMgrUser = NULL;

//---------------------------------------------------------------------------

CDBMgrUser::CDBMgrUser()
{
	m_strDBTName = "user";
}
//---------------------------------------------------------------------------

CDBMgrUser::~CDBMgrUser()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrUser::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrUser::LoadDB(TListDBUser& tDBUserList)
{
    UINT32 nReadCnt = 0;
    DB_USER du;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, reg_date, ins_type, "
						"group_id, ugroup_id, unicode, account_id, account_pw, "
						"name, group_code, co_id, co_rank, co_respon, co_email, co_phone, user_hphone, src_group_name,"
						"descr, ext_option "
						"FROM user WHERE used_flag=1;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		du.nID					= GetDBField_Int(nIndex++) ;
		du.nUsedFlag			= GetDBField_Int(nIndex++) ;
		du.nRegDate				= GetDBField_Int(nIndex++) ;
		du.nInsType				= GetDBField_Int(nIndex++) ;

		du.nGroupID				= GetDBField_Int(nIndex++) ;
		du.nUGroupID			= GetDBField_Int(nIndex++) ;
		du.strUniCode			= GetDBField_String(nIndex++);
		du.strAccountID			= GetDBField_String(nIndex++);
		du.strAccountPW			= GetDBField_String(nIndex++);

		du.strName				= GetDBField_String(nIndex++);
		du.strGCode				= GetDBField_String(nIndex++);
		du.strCoID				= GetDBField_String(nIndex++);
		du.strCoRank			= GetDBField_String(nIndex++);
		du.strCoRespon			= GetDBField_String(nIndex++);
		du.strCoEMail			= GetDBField_String(nIndex++);
		du.strCoPhone			= GetDBField_String(nIndex++);
		du.strUserHPhone		= GetDBField_String(nIndex++);
		du.strSrcGroupName			= GetDBField_String(nIndex++);

		du.strDescr				= GetDBField_String(nIndex++);

		du.nExtOption			= GetDBField_Int(nIndex++);
		
		tDBUserList.push_back(du);
        if(m_nLoadMaxID < UINT32(du.nID))
			m_nLoadMaxID = du.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBUserList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrUser::InsertUser(DB_USER& du)
{
    m_strQuery = SPrintf("INSERT INTO user (id, used_flag, reg_date, ins_type, "
									"group_id, ugroup_id, unicode, account_id, account_pw, "
									"name, group_code, co_id, co_rank, co_respon, co_email, co_phone, user_hphone, src_group_name,"
									"descr, ext_option) "
                                    "VALUES (%u, %u, %u, %u, "
                                    "%u, %u, '%s', '%s', '%s', "
									"'%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s',"
									"'%s', %u);",
                                    du.nID, du.nUsedFlag, du.nRegDate, du.nInsType,								
									du.nGroupID, du.nUGroupID, du.strUniCode.c_str(), du.strAccountID.c_str(), du.strAccountPW.c_str(),
									du.strName.c_str(), du.strGCode.c_str(), du.strCoID.c_str(), du.strCoRank.c_str(), du.strCoRespon.c_str(), du.strCoEMail.c_str(), du.strCoPhone.c_str(), du.strUserHPhone.c_str(), du.strSrcGroupName.c_str(),
									du.strDescr.c_str(), du.nExtOption);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrUser::UpdateUser(DB_USER& du)
{
	m_strQuery = SPrintf("UPDATE user SET "
						"ins_type=%u, "
						"group_id=%u, ugroup_id=%u, unicode='%s', account_id='%s', account_pw='%s', "
						"name='%s', group_code='%s', co_id='%s', co_rank='%s', co_respon='%s', co_email='%s', co_phone='%s', user_hphone='%s', src_group_name='%s',"
						"descr='%s', ext_option=%u "
						"WHERE id=%u;",
						du.nInsType,
						du.nGroupID, du.nUGroupID, du.strUniCode.c_str(), du.strAccountID.c_str(), du.strAccountPW.c_str(),
						du.strName.c_str(), du.strGCode.c_str(), du.strCoID.c_str(), du.strCoRank.c_str(), du.strCoRespon.c_str(), du.strCoEMail.c_str(), du.strCoPhone.c_str(), du.strUserHPhone.c_str(), du.strSrcGroupName.c_str(),
						du.strDescr.c_str(), du.nExtOption,
						du.nID);

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

INT32	CDBMgrUser::LoadExecute(PVOID lpTempletList)
{
	TListDBUser* ptDBList = (TListDBUser*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrUser::InsertExecute(PVOID lpTemplet)
{
	PDB_USER pd_t = (PDB_USER)lpTemplet;

    return InsertUser(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrUser::UpdateExecute(PVOID lpTemplet)
{
	PDB_USER pd_t = (PDB_USER)lpTemplet;

    return UpdateUser(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrUser::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






