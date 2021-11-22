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
#include "DBMgrPoFePtnBL.h"

//---------------------------------------------------------------------------

CDBMgrPoFePtnBL*		t_DBMgrPoFePtnBL;

//---------------------------------------------------------------------------

CDBMgrPoFePtnBL::CDBMgrPoFePtnBL()
{
	m_strDBTName = "po_fe_ptn_bl";
}
//---------------------------------------------------------------------------

CDBMgrPoFePtnBL::~CDBMgrPoFePtnBL()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnBL::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnBL::LoadDB(TListDBPoFePtnBL& tDBPoFePtnBLList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FE_PTN_BL dpfpb;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", save_name, save_path, file_hash, ptn_version, "
						"ptn_unit_num, ptn_chk_value "
						" FROM po_fe_ptn_bl WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfpb.tDPH;

		tDPH							= GetDBField_PoHDR(nIndex);

		dpfpb.tDPFI.strSaveName			= GetDBField_String(nIndex++);
		dpfpb.tDPFI.strSavePath			= GetDBField_String(nIndex++);
		dpfpb.tDPFI.strFileHash			= GetDBField_String(nIndex++);
		dpfpb.tDPFI.strPtnVersion		= GetDBField_String(nIndex++);
		dpfpb.tDPFI.nPtnUnitNum			= GetDBField_Int(nIndex++);
		dpfpb.tDPFI.nPtnChkValue		= GetDBField_Int(nIndex++);
		
        tDBPoFePtnBLList.push_back(dpfpb);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBPoFePtnBLList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnBL::InsertPoFePtnBL(DB_PO_FE_PTN_BL& dpfpb)
{
	DB_PO_HEADER& tDPH = dpfpb.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fe_ptn_bl("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", save_name, save_path, file_hash, ptn_version"
						", ptn_unit_num, ptn_chk_value"
						") VALUES (%s"
						", '%s', '%s', '%s', '%s'"
						", %u, %u);",
						GetPoHDRQuery_InsertValue(tDPH).c_str(),
						dpfpb.tDPFI.strSaveName.c_str(), dpfpb.tDPFI.strSavePath.c_str(), dpfpb.tDPFI.strFileHash.c_str(), dpfpb.tDPFI.strPtnVersion.c_str(),
						dpfpb.tDPFI.nPtnUnitNum, dpfpb.tDPFI.nPtnChkValue);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnBL::UpdatePoFePtnBL(DB_PO_FE_PTN_BL& dpfpb)
{
	DB_PO_HEADER& tDPH = dpfpb.tDPH;

	m_strQuery = SPrintf("UPDATE po_fe_ptn_bl SET %s"
						", save_name='%s', save_path='%s', file_hash='%s', ptn_version='%s'"
						", ptn_unit_num=%u, ptn_chk_value=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfpb.tDPFI.strSaveName.c_str(), dpfpb.tDPFI.strSavePath.c_str(), dpfpb.tDPFI.strFileHash.c_str(), dpfpb.tDPFI.strPtnVersion.c_str(),
						dpfpb.tDPFI.nPtnUnitNum, dpfpb.tDPFI.nPtnChkValue,
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

INT32	CDBMgrPoFePtnBL::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFePtnBL* ptDBList = (TListDBPoFePtnBL*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnBL::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FE_PTN_BL pd_t = (PDB_PO_FE_PTN_BL)lpTemplet;

    return InsertPoFePtnBL(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnBL::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FE_PTN_BL pd_t = (PDB_PO_FE_PTN_BL)lpTemplet;

    return UpdatePoFePtnBL(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnBL::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






