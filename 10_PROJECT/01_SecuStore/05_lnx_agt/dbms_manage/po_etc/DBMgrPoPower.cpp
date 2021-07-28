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
#include "DBMgrPoPower.h"

//---------------------------------------------------------------------------

CDBMgrPoPower*		t_DBMgrPoPower = NULL;

//---------------------------------------------------------------------------

CDBMgrPoPower::CDBMgrPoPower()
{
	m_strDBTName = "po_power";
}
//---------------------------------------------------------------------------

CDBMgrPoPower::~CDBMgrPoPower()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPower::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPower::LoadDB(TListDBPoPower& tDBPoPowerList)
{
    UINT32 nReadCnt = 0;
    DB_PO_POWER dpp;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", fastboot_ctrl"
						" FROM po_power WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpp.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpp.nFastBoot				= GetDBField_UInt(nIndex++);
		
        tDBPoPowerList.push_back(dpp);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoPowerList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPower::InsertPoPower(DB_PO_POWER& dpp)
{
	DB_PO_HEADER& tDPH = dpp.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_power("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", fastboot_ctrl"
						") VALUES (%s"						
						", %u);",
						GetPoHDRQuery_InsertValue(tDPH).c_str(),
						dpp.nFastBoot);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPower::UpdatePoPower(DB_PO_POWER& dpp)
{
	DB_PO_HEADER& tDPH = dpp.tDPH;

	m_strQuery = SPrintf("UPDATE po_power SET %s"
						", fastboot_ctrl=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
                        dpp.nFastBoot,
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPower::DeletePoPower(UINT32 nID)
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

INT32	CDBMgrPoPower::LoadExecute(PVOID lpTempletList)
{
	TListDBPoPower* ptDBList = (TListDBPoPower*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPower::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_POWER pd_t = (PDB_PO_POWER)lpTemplet;

    return InsertPoPower(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPower::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_POWER pd_t = (PDB_PO_POWER)lpTemplet;

    return UpdatePoPower(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPower::DeleteExecute(UINT32 nID)
{
	return DeletePoPower(nID);
}
//---------------------------------------------------------------------------






