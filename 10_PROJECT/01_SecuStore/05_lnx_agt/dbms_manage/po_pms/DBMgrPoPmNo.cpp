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
#include "DBMgrPoPmNo.h"

//---------------------------------------------------------------------------

CDBMgrPoPmNo*		t_DBMgrPoPmNo = NULL;

//---------------------------------------------------------------------------

CDBMgrPoPmNo::CDBMgrPoPmNo()
{
	m_strDBTName = "po_pm_no";
}
//---------------------------------------------------------------------------

CDBMgrPoPmNo::~CDBMgrPoPmNo()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmNo::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_pm_no][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmNo::LoadDB(TListDBPoPmNo& tDBPoPmNoList)
{
    UINT32 nReadCnt = 0;
    DB_PO_PM_NO data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", msg_type "
						" FROM po_pm_no WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);
		data.nMsgType			= GetDBField_UInt(nIndex++);
		
        tDBPoPmNoList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoPmNoList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmNo::InsertPoPmNo(DB_PO_PM_NO& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_pm_no("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", msg_type"
						") VALUES (%s"
						", %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(),
						data.nMsgType);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmNo::UpdatePoPmNo(DB_PO_PM_NO& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_pm_no SET %s"
						", msg_type=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nMsgType,
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

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

INT32	CDBMgrPoPmNo::LoadExecute(PVOID lpTempletList)
{
	TListDBPoPmNo* ptDBList = (TListDBPoPmNo*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmNo::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_PM_NO pd_t = (PDB_PO_PM_NO)lpTemplet;

    return InsertPoPmNo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmNo::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_PM_NO pd_t = (PDB_PO_PM_NO)lpTemplet;

    return UpdatePoPmNo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmNo::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






