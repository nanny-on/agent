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
#include "DBMgrPoNcPtnExTrust.h"

//---------------------------------------------------------------------------

CDBMgrPoNcPtnExTrust*		t_DBMgrPoNcPtnExTrust = NULL;

//---------------------------------------------------------------------------

CDBMgrPoNcPtnExTrust::CDBMgrPoNcPtnExTrust()
{
	m_strDBTName = "po_nc_ptn_ex_trust";
}
//---------------------------------------------------------------------------

CDBMgrPoNcPtnExTrust::~CDBMgrPoNcPtnExTrust()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnExTrust::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_nc_ptn_ex_trust][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnExTrust::LoadDB(TListDBPoNcPtnExTrust& tDBPoNcPtnExTrustList)
{
    UINT32 nReadCnt = 0;
    DB_PO_NC_PTN_EX_TRUST data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", policy_id, unit_id, block_mode"
						", sf_name, of_name, file_key"
						" FROM po_nc_ptn_ex_trust;");
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
	
		tDBPoNcPtnExTrustList.push_back(data);
		if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoNcPtnExTrustList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnExTrust::InsertPoNcPtnExTrust(DB_PO_NC_PTN_EX_TRUST& data)
{
	String strSFName		= MemToQuery(data.strSFName);
	String strOFName		= MemToQuery(data.strOFName);

	DB_PO_HEADER& tDPH = data.tDPH;
	m_strQuery = SPrintf("INSERT INTO po_nc_ptn_ex_trust("
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

INT32			CDBMgrPoNcPtnExTrust::UpdatePoNcPtnExTrust(DB_PO_NC_PTN_EX_TRUST& data)
{
	String strSFName		= MemToQuery(data.strSFName);
	String strOFName		= MemToQuery(data.strOFName);

	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_nc_ptn_ex_trust SET %s"
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

INT32	CDBMgrPoNcPtnExTrust::LoadExecute(PVOID lpTempletList)
{
	TListDBPoNcPtnExTrust* ptDBList = (TListDBPoNcPtnExTrust*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnExTrust::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_EX_TRUST pd_t = (PDB_PO_NC_PTN_EX_TRUST)lpTemplet;

    return InsertPoNcPtnExTrust(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnExTrust::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_EX_TRUST pd_t = (PDB_PO_NC_PTN_EX_TRUST)lpTemplet;

    return UpdatePoNcPtnExTrust(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnExTrust::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






