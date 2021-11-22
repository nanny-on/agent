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
#include "DBMgrPtnVersion.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnVersion::CDBMgrPtnVersion()
{
	m_strDBTName = "ptn_version";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnVersion::~CDBMgrPtnVersion()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVersion::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVersion::LoadDB(TListDBPtnVersion& tDBPtnVersionList)
{
	UINT32 nReadCnt = 0;
	DB_PTN_VERSION data;

	INT32 nIndex = 0;

	GetMDMM().strQuery.Format("SELECT id"
		", ptn_version"
		" FROM ptn_version;");

	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))        return ERR_DBMS_SELECT_FAIL;

	First();
	while(Next() == 0)
	{
		nIndex = 0;

		data.nID					= GetDBField_Int(nIndex++);

		data.strPtnVersion				= GetDBField_String(nIndex++);

		tDBPtnVersionList.push_back(data);
		if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
		nReadCnt++;
	}
	m_nLoadNumber = (UINT32)tDBPtnVersionList.size();
	WriteLogN("load database : [%s][%u]", (char *)(LPCSTR)m_strDBTName, m_nLoadNumber);
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVersion::InsertPtnVersion(DB_PTN_VERSION& data)
{
	GetMDMM().strQuery.Format("INSERT INTO ptn_version (id"
		", ptn_version)"
		"VALUES ( %u"
		"'%s'"
		");",
		data.nID, 
		data.strPtnVersion);

	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))		return ERR_DBMS_INSERT_FAIL;

	data.nID      = GetLastID();

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVersion::UpdatePtnVersion(DB_PTN_VERSION& data)
{
	GetMDMM().strQuery.Format("UPDATE ptn_version SET "
		" ptn_version='%s'"
		" WHERE id=%u;",
		data.strPtnVersion,
		data.nID);

	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnVersion::DeletePtnVersion(UINT32 nID)
{
	GetMDMM().strQuery.Format("DELETE FROM ptn_version WHERE id=%u;", nID);

	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))		return ERR_DBMS_DELETE_FAIL;
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

INT32	CDBMgrPtnVersion::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnVersion* ptDBList = (TListDBPtnVersion*)lpTempletList;

    return LoadDB(*ptDBList);
}

INT32	CDBMgrPtnVersion::SyncDBMSToDB(PVOID lpTempletList)
{
	TListDBPtnVersion* ptDBList = (TListDBPtnVersion*)lpTempletList;
	TListDBPtnVersionItor begin, end;
	begin = ptDBList->begin();	end = ptDBList->end();
	for(begin; begin != end; begin++)
	{
		if(begin->nID <= m_nLoadMaxID)	continue;

		InsertExecute(&(*begin));
		m_nSyncDBNum++;
	}
	return 0;
}

//---------------------------------------------------------------------------

INT32	CDBMgrPtnVersion::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_VERSION pd_t = (PDB_PTN_VERSION)lpTemplet;

    return InsertPtnVersion(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVersion::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_VERSION pd_t = (PDB_PTN_VERSION)lpTemplet;

    return UpdatePtnVersion(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnVersion::DeleteExecute(UINT32 nID)
{
	return DeletePtnVersion(nID);
}
//---------------------------------------------------------------------------






