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
#include "DBMgrPoInPtnExTrust.h"

//---------------------------------------------------------------------------

CDBMgrPoInPtnExTrust*		t_DBMgrPoInPtnExTrust = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInPtnExTrust::CDBMgrPoInPtnExTrust()
{
	m_strDBTName = "po_in_ptn_ex_trust";
}
//---------------------------------------------------------------------------

CDBMgrPoInPtnExTrust::~CDBMgrPoInPtnExTrust()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnExTrust::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnExTrust::LoadDB(TListDBPoInPtnExTrust& tDBPoInPtnExTrustList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_PTN_EX_TRUST data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", policy_id, unit_id, block_mode"
						", sf_name, of_name, file_key"
						" FROM po_in_ptn_ex_trust;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nPolicyID				= GetDBField_Int(nIndex++);
		data.nUnitID				= GetDBField_Int(nIndex++);
		data.nBlockMode				= GetDBField_UInt(nIndex++);

		data.strSFName				= GetDBField_String(nIndex++);
		data.strOFName				= GetDBField_String(nIndex++);
		data.strFileKey				= GetDBField_String(nIndex++);

		{
			QueryToMem(data.strSFName);
			QueryToMem(data.strOFName);
		}
	
		tDBPoInPtnExTrustList.push_back(data);
		if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBPoInPtnExTrustList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnExTrust::InsertPoInPtnExTrust(DB_PO_IN_PTN_EX_TRUST& data)
{
	String strSFName		= MemToQuery(data.strSFName);
	String strOFName		= MemToQuery(data.strOFName);

	DB_PO_HEADER& tDPH = data.tDPH;
	m_strQuery = SPrintf("INSERT INTO po_in_ptn_ex_trust("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						",  policy_id, unit_id, block_mode"
						", sf_name, of_name, file_key"
						") VALUES (%s"
						", %u, %u, %u"
						", '%s', '%s', '%s'"
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nPolicyID, data.nUnitID, data.nBlockMode,
						strSFName.c_str(), strOFName.c_str(), data.strFileKey.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnExTrust::UpdatePoInPtnExTrust(DB_PO_IN_PTN_EX_TRUST& data)
{
	String strSFName		= MemToQuery(data.strSFName);
	String strOFName		= MemToQuery(data.strOFName);

	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_ptn_ex_trust SET %s"
						", policy_id=%u, unit_id=%u, block_mode=%u"
						", sf_name='%s', of_name='%s', file_key='%s'"	
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						data.nPolicyID, data.nUnitID, data.nBlockMode,
						strSFName.c_str(), strOFName.c_str(), data.strFileKey.c_str(),	
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
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnExTrust::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInPtnExTrust* ptDBList = (TListDBPoInPtnExTrust*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnExTrust::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_EX_TRUST pd_t = (PDB_PO_IN_PTN_EX_TRUST)lpTemplet;

    return InsertPoInPtnExTrust(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnExTrust::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_EX_TRUST pd_t = (PDB_PO_IN_PTN_EX_TRUST)lpTemplet;

    return UpdatePoInPtnExTrust(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnExTrust::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






