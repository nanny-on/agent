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
#include "DBMgrPoFaEnvUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoFaEnvUnit*		t_DBMgrPoFaEnvUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaEnvUnit::CDBMgrPoFaEnvUnit()
{
	m_strDBTName = "po_fa_env_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoFaEnvUnit::~CDBMgrPoFaEnvUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaEnvUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaEnvUnit::LoadDB(TListDBPoFaEnvUnit& tDBPoFaEnvUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_ENV_UNIT dpfeu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", file_ptn_type, file_ptn_global"
						" FROM po_fa_env_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfeu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfeu.nFilePtnType			= GetDBField_UInt(nIndex++);
		dpfeu.strFilePtnGlobal		= GetDBField_String(nIndex++);
		
        tDBPoFaEnvUnitList.push_back(dpfeu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaEnvUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaEnvUnit::InsertPoFaEnvUnit(DB_PO_FA_ENV_UNIT& dpfeu)
{
	DB_PO_HEADER& tDPH = dpfeu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_env_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", file_ptn_type, file_ptn_global"
						") VALUES (%s"
                        ", %u, '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpfeu.nFilePtnType, dpfeu.strFilePtnGlobal.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaEnvUnit::UpdatePoFaEnvUnit(DB_PO_FA_ENV_UNIT& dpfeu)
{
	DB_PO_HEADER& tDPH = dpfeu.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_env_unit SET %s"
						", file_ptn_type=%u, file_ptn_global='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfeu.nFilePtnType, dpfeu.strFilePtnGlobal.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaEnvUnit::DeletePoFaEnvUnit(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}

INT32			CDBMgrPoFaEnvUnit::ClearPoFaEnvUnit()
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_CLEAR, m_strDBTName.c_str());

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

INT32	CDBMgrPoFaEnvUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaEnvUnit* ptDBList = (TListDBPoFaEnvUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaEnvUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_ENV_UNIT pd_t = (PDB_PO_FA_ENV_UNIT)lpTemplet;

    return InsertPoFaEnvUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaEnvUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_ENV_UNIT pd_t = (PDB_PO_FA_ENV_UNIT)lpTemplet;

    return UpdatePoFaEnvUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaEnvUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoFaEnvUnit(nID);
}

INT32	CDBMgrPoFaEnvUnit::ClearExecute()
{
	return ClearPoFaEnvUnit();
}
//---------------------------------------------------------------------------






