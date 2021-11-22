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
#include "DBMgrEnvSOrgLink.h"

//---------------------------------------------------------------------------

CDBMgrEnvSOrgLink*		t_DBMgrEnvSOrgLink;

//---------------------------------------------------------------------------

CDBMgrEnvSOrgLink::CDBMgrEnvSOrgLink()
{
	m_strDBTName = "env_sorg_link";
}
//---------------------------------------------------------------------------

CDBMgrEnvSOrgLink::~CDBMgrEnvSOrgLink()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvSOrgLink::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvSOrgLink::LoadDB(TListDBEnvSOrgLink& tDBEnvSOrgLinkList)
{
    UINT32 nReadCnt = 0;
    DB_ENV_SORG_LINK desl;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", user_catch_type, user_catch_value"
						" FROM env_sorg_link WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = desl.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);

		desl.nUserCatchType		= GetDBField_UInt(nIndex++);
		desl.strUserCatchValue	= GetDBField_String(nIndex++);
		
        tDBEnvSOrgLinkList.push_back(desl);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBEnvSOrgLinkList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvSOrgLink::InsertEnvSOrgLink(DB_ENV_SORG_LINK& desl)
{
	DB_PO_HEADER& tDPH = desl.tDPH;

	m_strQuery = SPrintf("INSERT INTO env_sorg_link("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", user_catch_type, user_catch_value"
						") VALUES (%s"
						", %u, '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						desl.nUserCatchType, desl.strUserCatchValue.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrEnvSOrgLink::UpdateEnvSOrgLink(DB_ENV_SORG_LINK& desl)
{
	DB_PO_HEADER& tDPH = desl.tDPH;

	m_strQuery = SPrintf("UPDATE env_sorg_link SET %s"
						", user_catch_type=%u, user_catch_value='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						desl.nUserCatchType, desl.strUserCatchValue.c_str(),
                        tDPH.nID);

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

INT32	CDBMgrEnvSOrgLink::LoadExecute(PVOID lpTempletList)
{
	TListDBEnvSOrgLink* ptDBList = (TListDBEnvSOrgLink*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvSOrgLink::InsertExecute(PVOID lpTemplet)
{
	PDB_ENV_SORG_LINK pd_t = (PDB_ENV_SORG_LINK)lpTemplet;

    return InsertEnvSOrgLink(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvSOrgLink::UpdateExecute(PVOID lpTemplet)
{
	PDB_ENV_SORG_LINK pd_t = (PDB_ENV_SORG_LINK)lpTemplet;

    return UpdateEnvSOrgLink(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrEnvSOrgLink::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






