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
#include "DBMgrPoFaOp.h"

//---------------------------------------------------------------------------

CDBMgrPoFaOp*		t_DBMgrPoFaOp = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaOp::CDBMgrPoFaOp()
{
	m_strDBTName = "po_fa_op";
}
//---------------------------------------------------------------------------

CDBMgrPoFaOp::~CDBMgrPoFaOp()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOp::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOp::LoadDB(TListDBPoFaOp& tDBPoFaOpList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_OP dpfp;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", sch_time, del_cnt, limit_size, limit_cnt"
						", scan_dt, chk_fdt_type, chk_fdt_value"
						", del_method"
						",lnx_sch_time, lnx_del_cnt, lnx_limit_size, lnx_limit_cnt"
						", lnx_chk_fdt_type, lnx_chk_fdt_value, lnx_del_method"
						" FROM po_fa_op WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfp.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);
		dpfp.nSchTime				= GetDBField_UInt(nIndex++);
		dpfp.nDelCount				= GetDBField_UInt(nIndex++);
		dpfp.nLimitSize				= GetDBField_UInt(nIndex++);
		dpfp.nLimitCnt				= GetDBField_UInt(nIndex++);

		dpfp.nScanTime				= GetDBField_UInt(nIndex++);

		dpfp.nChkFDTType			= GetDBField_UInt(nIndex++);
		dpfp.nChkFDTValue			= GetDBField_UInt(nIndex++);
		dpfp.nDelMethod				= GetDBField_UInt(nIndex++);
		
		// linux
		dpfp.nLnxSchTime			= GetDBField_UInt(nIndex++);
		dpfp.nLnxDelCount			= GetDBField_UInt(nIndex++);
		dpfp.nLnxLimitSize			= GetDBField_UInt(nIndex++);
		dpfp.nLnxLimitCnt			= GetDBField_UInt(nIndex++);
		dpfp.nLnxChkFDTType			= GetDBField_UInt(nIndex++);
		dpfp.nLnxChkFDTValue		= GetDBField_UInt(nIndex++);
		dpfp.nLnxDelMethod			= GetDBField_UInt(nIndex++);

		
        tDBPoFaOpList.push_back(dpfp);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = tDBPoFaOpList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOp::InsertPoFaOp(DB_PO_FA_OP& dpfp)
{
	DB_PO_HEADER& tDPH = dpfp.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_op("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", sch_time, del_cnt, limit_size, limit_cnt"
						", scan_dt"
						", chk_fdt_type, chk_fdt_value, del_method"
						", lnx_sch_time, lnx_del_cnt, lnx_limit_size, lnx_limit_cnt"
						", lnx_chk_fdt_type, lnx_chk_fdt_value, lnx_del_method"
						") VALUES (%s"
						", %u, %u, %u, %u"
						", %u"
						", %u, %u, %u"
						", %u, %u, %u, %u"
						", %u, %u, %u"
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str(),
						dpfp.nSchTime, dpfp.nDelCount, dpfp.nLimitSize, dpfp.nLimitCnt,
						dpfp.nScanTime,
						dpfp.nChkFDTType, dpfp.nChkFDTValue, dpfp.nDelMethod,
						dpfp.nLnxSchTime, dpfp.nLnxDelCount, dpfp.nLnxLimitSize, dpfp.nLnxLimitCnt,
						dpfp.nLnxChkFDTType, dpfp.nLnxChkFDTValue, dpfp.nLnxDelMethod);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOp::UpdatePoFaOp(DB_PO_FA_OP& dpfp)
{
	DB_PO_HEADER& tDPH = dpfp.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_op SET %s"
						", sch_time=%u, del_cnt=%u, limit_size=%u, limit_cnt=%u"
						", scan_dt=%u"
						", chk_fdt_type=%u, chk_fdt_value=%u, del_method=%u"
						", lnx_sch_time=%u, lnx_del_cnt=%u, lnx_limit_size=%u, lnx_limit_cnt=%u"
						", lnx_chk_fdt_type=%u, lnx_chk_fdt_value=%u, lnx_del_method=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfp.nSchTime, dpfp.nDelCount, dpfp.nLimitSize, dpfp.nLimitCnt,
						dpfp.nScanTime,
						dpfp.nChkFDTType, dpfp.nChkFDTValue, dpfp.nDelMethod,
						dpfp.nLnxSchTime, dpfp.nLnxDelCount, dpfp.nLnxLimitSize, dpfp.nLnxLimitCnt,
						dpfp.nLnxChkFDTType, dpfp.nLnxChkFDTValue, dpfp.nLnxDelMethod,
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaOp::DeletePoFaOp(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------  

INT32			CDBMgrPoFaOp::ClearPoFaOp()
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_CLEAR, m_strDBTName.c_str());

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

INT32	CDBMgrPoFaOp::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaOp* ptDBList = (TListDBPoFaOp*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOp::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP pd_t = (PDB_PO_FA_OP)lpTemplet;

    return InsertPoFaOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOp::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_OP pd_t = (PDB_PO_FA_OP)lpTemplet;

    return UpdatePoFaOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaOp::DeleteExecute(UINT32 nID)
{
	return DeletePoFaOp(nID);
}
//---------------------------------------------------------------------------


INT32	CDBMgrPoFaOp::ClearExecute()
{
	return ClearPoFaOp();
}
//---------------------------------------------------------------------------





