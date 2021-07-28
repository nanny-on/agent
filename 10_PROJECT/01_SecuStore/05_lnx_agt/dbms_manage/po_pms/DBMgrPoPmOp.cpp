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
#include "DBMgrPoPmOp.h"

//---------------------------------------------------------------------------

CDBMgrPoPmOp*		t_DBMgrPoPmOp = NULL;

//---------------------------------------------------------------------------

CDBMgrPoPmOp::CDBMgrPoPmOp()
{
	m_strDBTName = "po_pm_op";
}
//---------------------------------------------------------------------------

CDBMgrPoPmOp::~CDBMgrPoPmOp()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmOp::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_pm_op][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmOp::LoadDB(TListDBPoPmOp& tDBPoPmOpList)
{
    UINT32 nReadCnt = 0;
    DB_PO_PM_OP data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", log_sync_mode"
						", dn_try_num, mon_rm_patch, auto_patch_type"
						", ptn_file_sv_name, ptn_version, ptn_file_name, ptn_file_hash"
						", ptn_dn_file_type, ptn_dn_file_name, ptn_dn_file_hash"
						" FROM po_pm_op WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH							= GetDBField_PoHDR(nIndex);

		data.nLogSyncMode				= GetDBField_UInt(nIndex++);
		data.nDnTryNum					= GetDBField_UInt(nIndex++);
		data.nMonRmPatch				= GetDBField_UInt64(nIndex++);
		data.nAutoPatchType				= GetDBField_UInt(nIndex++);

		data.strPtnFileSvName			= GetDBField_String(nIndex++);
		data.strPtnVersion				= GetDBField_String(nIndex++);
		data.strPtnFileName				= GetDBField_String(nIndex++);
		data.strPtnFileHash				= GetDBField_String(nIndex++);

		data.nPtnDnFileType				= GetDBField_UInt(nIndex++);
		data.strPtnDnFileName			= GetDBField_String(nIndex++);
		data.strPtnDnFileHash			= GetDBField_String(nIndex++);
        
        tDBPoPmOpList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoPmOpList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmOp::InsertPoPmOp(DB_PO_PM_OP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_pm_op("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", log_sync_mode"
						", dn_try_num, mon_rm_patch, auto_patch_type"
						", ptn_file_sv_name, ptn_version, ptn_file_name, ptn_file_hash"
						", ptn_dn_file_type, ptn_dn_file_name, ptn_dn_file_hash"
						") VALUES (%s "
						", %u"
						", %u, '%llu', %u"
						", '%s', '%s', '%s', '%s'"
						", %u, '%s', '%s'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nLogSyncMode,
						data.nDnTryNum, data.nMonRmPatch, data.nAutoPatchType,
						data.strPtnFileSvName.c_str(), data.strPtnVersion.c_str(), data.strPtnFileName.c_str(), data.strPtnFileHash.c_str(),
						data.nPtnDnFileType, data.strPtnDnFileName.c_str(), data.strPtnDnFileHash.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoPmOp::UpdatePoPmOp(DB_PO_PM_OP& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_pm_op SET %s"
						", log_sync_mode=%u"	
						", dn_try_num=%u, mon_rm_patch='%llu', auto_patch_type=%u"
						", ptn_file_sv_name='%s', ptn_version='%s', ptn_file_name='%s', ptn_file_hash='%s'"
						", ptn_dn_file_type=%u, ptn_dn_file_name='%s', ptn_dn_file_hash='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nLogSyncMode,
						data.nDnTryNum, data.nMonRmPatch, data.nAutoPatchType,
						data.strPtnFileSvName.c_str(), data.strPtnVersion.c_str(), data.strPtnFileName.c_str(), data.strPtnFileHash.c_str(),
						data.nPtnDnFileType, data.strPtnDnFileName.c_str(), data.strPtnDnFileHash.c_str(),
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

INT32	CDBMgrPoPmOp::LoadExecute(PVOID lpTempletList)
{
	TListDBPoPmOp* ptDBList = (TListDBPoPmOp*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmOp::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_PM_OP pd_t = (PDB_PO_PM_OP)lpTemplet;

    return InsertPoPmOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmOp::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_PM_OP pd_t = (PDB_PO_PM_OP)lpTemplet;

    return UpdatePoPmOp(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoPmOp::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






