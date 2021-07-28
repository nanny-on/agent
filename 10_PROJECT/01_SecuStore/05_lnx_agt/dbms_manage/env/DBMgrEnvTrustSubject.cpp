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
#include "DBMgrEnvTrustSubject.h"

//---------------------------------------------------------------------------

CDBMgrEnvTrustSubject*		t_DBMgrEnvTrustSubject;

//---------------------------------------------------------------------------

CDBMgrEnvTrustSubject::CDBMgrEnvTrustSubject()
{
	m_strDBTName = "env_trust_subject";
}
//---------------------------------------------------------------------------

CDBMgrEnvTrustSubject::~CDBMgrEnvTrustSubject()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvTrustSubject::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvTrustSubject::LoadDB(TListDBEnvTrustSubject& tDBEnvTrustSubjectList)
{
    UINT32 nReadCnt = 0;
    DB_ENV_TRUST_SUBJECT dets;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", subject_info"
						" FROM env_trust_subject WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dets.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);
		dets.strSubjectInfo		= GetDBField_String(nIndex++);
		
        tDBEnvTrustSubjectList.push_back(dets);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBEnvTrustSubjectList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvTrustSubject::InsertEnvTrustSubject(DB_ENV_TRUST_SUBJECT& dets)
{
	DB_PO_HEADER& tDPH = dets.tDPH;

	m_strQuery = SPrintf("INSERT INTO env_trust_subject("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", subject_info"
						") VALUES (%s"
                        ", '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dets.strSubjectInfo.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvTrustSubject::UpdateEnvTrustSubject(DB_ENV_TRUST_SUBJECT& dets)
{
	DB_PO_HEADER& tDPH = dets.tDPH;

	m_strQuery = SPrintf("UPDATE env_trust_subject SET %s"
						", subject_info='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dets.strSubjectInfo.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvTrustSubject::DeleteEnvTrustSubject(UINT32 nID)
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

INT32	CDBMgrEnvTrustSubject::LoadExecute(PVOID lpTempletList)
{
	TListDBEnvTrustSubject* ptDBList = (TListDBEnvTrustSubject*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvTrustSubject::InsertExecute(PVOID lpTemplet)
{
	PDB_ENV_TRUST_SUBJECT pd_t = (PDB_ENV_TRUST_SUBJECT)lpTemplet;

    return InsertEnvTrustSubject(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvTrustSubject::UpdateExecute(PVOID lpTemplet)
{
	PDB_ENV_TRUST_SUBJECT pd_t = (PDB_ENV_TRUST_SUBJECT)lpTemplet;

    return UpdateEnvTrustSubject(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvTrustSubject::DeleteExecute(UINT32 nID)
{
	return DeleteEnvTrustSubject(nID);
}
//---------------------------------------------------------------------------






