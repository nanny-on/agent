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
#include "DBMgrPoInDevOWL.h"

//---------------------------------------------------------------------------

CDBMgrPoInDevOWL*		t_DBMgrPoInDevOWL = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInDevOWL::CDBMgrPoInDevOWL()
{
	m_strDBTName = "po_in_devo_wl";
}
//---------------------------------------------------------------------------

CDBMgrPoInDevOWL::~CDBMgrPoInDevOWL()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOWL::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_devo_wl][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOWL::LoadDB(TListDBPoInDevOWL& tDBPoInDevOWLList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_DEVO_WL dpfn;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						" FROM po_in_devo_wl WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfn.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);
		
        tDBPoInDevOWLList.push_back(dpfn);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInDevOWLList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOWL::InsertPoInDevOWL(DB_PO_IN_DEVO_WL& dpfn)
{
	DB_PO_HEADER& tDPH = dpfn.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_devo_wl("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						") VALUES (%s"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOWL::UpdatePoInDevOWL(DB_PO_IN_DEVO_WL& dpfn)
{
	DB_PO_HEADER& tDPH = dpfn.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_devo_wl SET %s"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
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

INT32	CDBMgrPoInDevOWL::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInDevOWL* ptDBList = (TListDBPoInDevOWL*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOWL::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_DEVO_WL pd_t = (PDB_PO_IN_DEVO_WL)lpTemplet;

    return InsertPoInDevOWL(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOWL::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_DEVO_WL pd_t = (PDB_PO_IN_DEVO_WL)lpTemplet;

    return UpdatePoInDevOWL(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOWL::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






