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
#include "DBMgrPoFePtnOp.h"

//---------------------------------------------------------------------------

CDBMgrPoFePtnOp*		t_DBMgrPoFePtnOp = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFePtnOp::CDBMgrPoFePtnOp()
{
	m_strDBTName = "po_fe_ptn_op";
}
//---------------------------------------------------------------------------

CDBMgrPoFePtnOp::~CDBMgrPoFePtnOp()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnOp::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnOp::LoadDB(TListDBPoFePtnOp& tDBPoFePtnOpList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FE_PTN_OP dpfpo;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", block_mode"
						", ptn_pkg_id, ptn_sch_time, ptn_scan_time"
						", ptn_range_type, ptn_obj_type"
						", new_file_send_type, new_file_confirm_mode"
						" FROM po_fe_ptn_op WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfpo.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfpo.nBlockMode			= GetDBField_UInt(nIndex++);
		dpfpo.nPtnPkgID				= GetDBField_UInt(nIndex++);
		dpfpo.nPtnSchTime			= GetDBField_UInt(nIndex++);
		dpfpo.nPtnScanTime			= GetDBField_UInt(nIndex++);
		dpfpo.nPtnRangeType			= GetDBField_UInt(nIndex++);
		dpfpo.nPtnObjType			= GetDBField_UInt(nIndex++);
		dpfpo.nNewFileSendType		= GetDBField_UInt(nIndex++);
		dpfpo.nNewFileConfirmMode	= GetDBField_UInt(nIndex++);
        
        tDBPoFePtnOpList.push_back(dpfpo);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFePtnOpList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnOp::InsertPoFePtnOp(DB_PO_FE_PTN_OP& dpfpo)
{
	DB_PO_HEADER& tDPH = dpfpo.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fe_ptn_op("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", block_mode"
						", ptn_pkg_id, ptn_sch_time, ptn_scan_time"
						", ptn_range_type, ptn_obj_type"
						", new_file_send_type, new_file_confirm_mode"
						") VALUES (%s "
						", %u"
						", %u, %u, %u"
						", %u, %u"
						", %u, %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpfpo.nBlockMode,
						dpfpo.nPtnPkgID, dpfpo.nPtnSchTime, dpfpo.nPtnScanTime,
						dpfpo.nPtnRangeType, dpfpo.nPtnObjType,
						dpfpo.nNewFileSendType, dpfpo.nNewFileConfirmMode);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnOp::UpdatePoFePtnOp(DB_PO_FE_PTN_OP& dpfpo)
{
	DB_PO_HEADER& tDPH = dpfpo.tDPH;

	m_strQuery = SPrintf("UPDATE po_fe_ptn_op SET %s"
						", block_mode=%u"
						", ptn_pkg_id=%u, ptn_sch_time=%u, ptn_scan_time=%u"
						", ptn_range_type=%u, ptn_obj_type=%u"
						", new_file_send_type=%u, new_file_confirm_mode=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfpo.nBlockMode,
						dpfpo.nPtnPkgID, dpfpo.nPtnSchTime, dpfpo.nPtnScanTime,
						dpfpo.nPtnRangeType, dpfpo.nPtnObjType,
						dpfpo.nNewFileSendType, dpfpo.nNewFileConfirmMode,
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

INT32	CDBMgrPoFePtnOp::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFePtnOp* ptDBList = (TListDBPoFePtnOp*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnOp::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FE_PTN_OP pd_t = (PDB_PO_FE_PTN_OP)lpTemplet;

    return InsertPoFePtnOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnOp::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FE_PTN_OP pd_t = (PDB_PO_FE_PTN_OP)lpTemplet;

    return UpdatePoFePtnOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnOp::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






