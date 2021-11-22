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
#include "DBMgrPoInVulnOp.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnOp*		t_DBMgrPoInVulnOp = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnOp::CDBMgrPoInVulnOp()
{
	m_strDBTName = "po_in_vuln_op";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnOp::~CDBMgrPoInVulnOp()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnOp::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnOp::LoadDB(TListDBPoInVulnOp& tDBPoInVulnOpList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_OP data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", ptn_file_sv_name, ptn_version, ptn_file_name, ptn_file_hash"
						", ptn_dn_file_type, ptn_dn_file_name, ptn_dn_file_hash"
						" FROM po_in_vuln_op WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);
		
		data.tDPIVOPF.strPtnFileSvName			= GetDBField_String(nIndex++);
		data.tDPIVOPF.strPtnVersion				= GetDBField_String(nIndex++);
		data.tDPIVOPF.strPtnFileName			= GetDBField_String(nIndex++);
		data.tDPIVOPF.strPtnFileHash			= GetDBField_String(nIndex++);

		data.tDPIVOPF.nPtnDnFileType			= GetDBField_UInt(nIndex++);
		data.tDPIVOPF.strPtnDnFileName			= GetDBField_String(nIndex++);
		data.tDPIVOPF.strPtnDnFileHash			= GetDBField_String(nIndex++);

        tDBPoInVulnOpList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInVulnOpList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnOp::InsertPoInVulnOp(DB_PO_IN_VULN_OP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_op("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", ptn_file_sv_name, ptn_version, ptn_file_name, ptn_file_hash"
						", ptn_dn_file_type, ptn_dn_file_name, ptn_dn_file_hash"
						") VALUES (%s"
						", '%s', '%s', '%s', '%s'"
						", %u, '%s', '%s'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(),
						data.tDPIVOPF.strPtnFileSvName.c_str(), data.tDPIVOPF.strPtnVersion.c_str(), data.tDPIVOPF.strPtnFileName.c_str(), data.tDPIVOPF.strPtnFileHash.c_str(),
						data.tDPIVOPF.nPtnDnFileType, data.tDPIVOPF.strPtnDnFileName.c_str(), data.tDPIVOPF.strPtnDnFileHash.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnOp::UpdatePoInVulnOp(DB_PO_IN_VULN_OP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_op SET %s"
						", ptn_file_sv_name='%s', ptn_version='%s', ptn_file_name='%s', ptn_file_hash='%s'"
						", ptn_dn_file_type=%u, ptn_dn_file_name='%s', ptn_dn_file_hash='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.tDPIVOPF.strPtnFileSvName.c_str(), data.tDPIVOPF.strPtnVersion.c_str(), data.tDPIVOPF.strPtnFileName.c_str(), data.tDPIVOPF.strPtnFileHash.c_str(),
						data.tDPIVOPF.nPtnDnFileType, data.tDPIVOPF.strPtnDnFileName.c_str(), data.tDPIVOPF.strPtnDnFileHash.c_str(),
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

INT32	CDBMgrPoInVulnOp::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnOp* ptDBList = (TListDBPoInVulnOp*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnOp::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_OP pd_t = (PDB_PO_IN_VULN_OP)lpTemplet;

    return InsertPoInVulnOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnOp::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_OP pd_t = (PDB_PO_IN_VULN_OP)lpTemplet;

    return UpdatePoInVulnOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnOp::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






