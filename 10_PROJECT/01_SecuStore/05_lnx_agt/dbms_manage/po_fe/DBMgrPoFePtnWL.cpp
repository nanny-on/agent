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
#include "DBMgrPoFePtnWL.h"

//---------------------------------------------------------------------------

CDBMgrPoFePtnWL*		t_DBMgrPoFePtnWL;

//---------------------------------------------------------------------------

CDBMgrPoFePtnWL::CDBMgrPoFePtnWL()
{
	m_strDBTName = "po_fe_ptn_wl";
}
//---------------------------------------------------------------------------

CDBMgrPoFePtnWL::~CDBMgrPoFePtnWL()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnWL::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnWL::LoadDB(TListDBPoFePtnWL& tDBPoFePtnWLList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FE_PTN_WL dpfpw;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", ptn_sch_time, ptn_pkg_id, ptn_obj_type, ptn_pe_bit"					
						", save_name, save_path, file_hash, ptn_version"
						", ptn_unit_num, ptn_chk_value "
						" FROM po_fe_ptn_wl WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfpw.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
		dpfpw.nPtnSchTime			= GetDBField_UInt(nIndex++);
		dpfpw.nPtnPkgID				= GetDBField_UInt(nIndex++);
		dpfpw.nPtnObjType			= GetDBField_UInt(nIndex++);
		dpfpw.nPtnPeBit				= GetDBField_UInt(nIndex++);

		dpfpw.tDPFI.strSaveName		= GetDBField_String(nIndex++);
		dpfpw.tDPFI.strSavePath		= GetDBField_String(nIndex++);
		dpfpw.tDPFI.strFileHash		= GetDBField_String(nIndex++);
		dpfpw.tDPFI.strPtnVersion	= GetDBField_String(nIndex++);
		dpfpw.tDPFI.nPtnUnitNum		= GetDBField_Int(nIndex++);
		dpfpw.tDPFI.nPtnChkValue	= GetDBField_Int(nIndex++);
		
        tDBPoFePtnWLList.push_back(dpfpw);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFePtnWLList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnWL::InsertPoFePtnWL(DB_PO_FE_PTN_WL& dpfpw)
{
	DB_PO_HEADER& tDPH = dpfpw.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fe_ptn_wl("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", ptn_sch_time, ptn_pkg_id, ptn_obj_type, ptn_pe_bit"
						", save_name, save_path, file_hash, ptn_version"
						", ptn_unit_num, ptn_chk_value"
						") VALUES (%s"
						", %u, %u, %u, %u"
						", '%s', '%s', '%s', '%s', "
						"%u, %u);",
						GetPoHDRQuery_InsertValue(tDPH).c_str(),
						dpfpw.nPtnSchTime, dpfpw.nPtnPkgID, dpfpw.nPtnObjType, dpfpw.nPtnPeBit,
						dpfpw.tDPFI.strSaveName.c_str(), dpfpw.tDPFI.strSavePath.c_str(), dpfpw.tDPFI.strFileHash.c_str(), dpfpw.tDPFI.strPtnVersion.c_str(),
						dpfpw.tDPFI.nPtnUnitNum, dpfpw.tDPFI.nPtnChkValue);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnWL::UpdatePoFePtnWL(DB_PO_FE_PTN_WL& dpfpw)
{
	DB_PO_HEADER& tDPH = dpfpw.tDPH;

	m_strQuery = SPrintf("UPDATE po_fe_ptn_wl SET %s"
						", ptn_sch_time=%u, ptn_pkg_id=%u, ptn_obj_type=%u, ptn_pe_bit=%u"
						", save_name='%s', save_path='%s', file_hash='%s', ptn_version='%s'"
						", ptn_unit_num=%u, ptn_chk_value=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfpw.nPtnSchTime, dpfpw.nPtnPkgID, dpfpw.nPtnObjType, dpfpw.nPtnPeBit,
						dpfpw.tDPFI.strSaveName.c_str(), dpfpw.tDPFI.strSavePath.c_str(), dpfpw.tDPFI.strFileHash.c_str(), dpfpw.tDPFI.strPtnVersion.c_str(),
						dpfpw.tDPFI.nPtnUnitNum, dpfpw.tDPFI.nPtnChkValue,
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

INT32	CDBMgrPoFePtnWL::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFePtnWL* ptDBList = (TListDBPoFePtnWL*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnWL::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FE_PTN_WL pd_t = (PDB_PO_FE_PTN_WL)lpTemplet;

    return InsertPoFePtnWL(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnWL::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FE_PTN_WL pd_t = (PDB_PO_FE_PTN_WL)lpTemplet;

    return UpdatePoFePtnWL(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnWL::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






