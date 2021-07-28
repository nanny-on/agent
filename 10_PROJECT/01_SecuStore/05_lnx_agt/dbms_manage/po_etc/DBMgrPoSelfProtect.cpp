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
#include "DBMgrPoSelfProtect.h"

//---------------------------------------------------------------------------

CDBMgrPoSelfProtect*		t_DBMgrPoSelfProtect = NULL;

//---------------------------------------------------------------------------

CDBMgrPoSelfProtect::CDBMgrPoSelfProtect()
{
	m_strDBTName = "po_self_protect";
}
//---------------------------------------------------------------------------

CDBMgrPoSelfProtect::~CDBMgrPoSelfProtect()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSelfProtect::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSelfProtect::LoadDB(TListDBPoSelfProtect& tDBPoSelfProtectList)
{
    UINT32 nReadCnt = 0;
    DB_PO_SELF_PROTECT dpsp;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
    					", protect_mode "
						" FROM po_self_protect WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpsp.tDPH;		
		
		tDPH						= GetDBField_PoHDR(nIndex);
		dpsp.nProtectMode			= GetDBField_UInt(nIndex++);
		
        tDBPoSelfProtectList.push_back(dpsp);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoSelfProtectList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSelfProtect::InsertPoSelfProtect(DB_PO_SELF_PROTECT& dpsp)
{
	DB_PO_HEADER& tDPH = dpsp.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_self_protect("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", protect_mode"
						") VALUES (%s"
						", %u);",
						GetPoHDRQuery_InsertValue(tDPH).c_str(),
						dpsp.nProtectMode);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSelfProtect::UpdatePoSelfProtect(DB_PO_SELF_PROTECT& dpsp)
{
	DB_PO_HEADER& tDPH = dpsp.tDPH;

	m_strQuery = SPrintf("UPDATE po_self_protect SET %s"
						", protect_mode=%u "
						"WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dpsp.nProtectMode,
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoSelfProtect::DeletePoSelfProtect(UINT32 nID)
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

INT32	CDBMgrPoSelfProtect::LoadExecute(PVOID lpTempletList)
{
	TListDBPoSelfProtect* ptDBList = (TListDBPoSelfProtect*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSelfProtect::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_SELF_PROTECT pd_t = (PDB_PO_SELF_PROTECT)lpTemplet;

    return InsertPoSelfProtect(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSelfProtect::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_SELF_PROTECT pd_t = (PDB_PO_SELF_PROTECT)lpTemplet;

    return UpdatePoSelfProtect(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoSelfProtect::DeleteExecute(UINT32 nID)
{
	return DeletePoSelfProtect(nID);
}
//---------------------------------------------------------------------------






