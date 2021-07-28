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
#include "DBMgrPoFePtnLo.h"

//---------------------------------------------------------------------------

CDBMgrPoFePtnLo*		t_DBMgrPoFePtnLo = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFePtnLo::CDBMgrPoFePtnLo()
{
	m_strDBTName = "po_fe_ptn_lo";
}
//---------------------------------------------------------------------------

CDBMgrPoFePtnLo::~CDBMgrPoFePtnLo()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnLo::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnLo::LoadDB(TListDBPoFePtnLo& tDBPoFePtnLoList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FE_PTN_LO dpfpl;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", ptn_range_type, ptn_obj_type"
						", new_file_send_type"
						", save_name, save_path, file_hash, ptn_version"
						", ptn_unit_num, ptn_chk_value "
						" FROM po_fe_ptn_lo WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfpl.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfpl.nPtnRangeType			= GetDBField_UInt(nIndex++);
		dpfpl.nPtnObjType			= GetDBField_UInt(nIndex++);
		dpfpl.nNewFileSendType		= GetDBField_UInt(nIndex++);

		dpfpl.strSaveName			= GetDBField_String(nIndex++);
		dpfpl.strSavePath			= GetDBField_String(nIndex++);
		dpfpl.strFileHash			= GetDBField_String(nIndex++);
		dpfpl.strPtnVersion			= GetDBField_String(nIndex++);
		dpfpl.nPtnUnitNum			= GetDBField_Int(nIndex++);
		dpfpl.nPtnChkValue			= GetDBField_Int(nIndex++);
        
        tDBPoFePtnLoList.push_back(dpfpl);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFePtnLoList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnLo::InsertPoFePtnLo(DB_PO_FE_PTN_LO& dpfpl)
{
	DB_PO_HEADER& tDPH = dpfpl.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fe_ptn_lo("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", ptn_range_type, ptn_obj_type"
						", new_file_send_type"
						", save_name, save_path, file_hash, ptn_version"
						", ptn_unit_num, ptn_chk_value"
						") VALUES (%s "
						", %u, %u"
						", %u"
						", '%s', '%s', '%s', '%s'"
						", %u, %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpfpl.nPtnRangeType, dpfpl.nPtnObjType,
						dpfpl.nNewFileSendType,
						dpfpl.strSaveName.c_str(), dpfpl.strSavePath.c_str(), dpfpl.strFileHash.c_str(), dpfpl.strPtnVersion.c_str(),
						dpfpl.nPtnUnitNum, dpfpl.nPtnChkValue);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		
		return ERR_DBMS_INSERT_FAIL;
	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFePtnLo::UpdatePoFePtnLo(DB_PO_FE_PTN_LO& dpfpl)
{
	DB_PO_HEADER& tDPH = dpfpl.tDPH;

	m_strQuery = SPrintf("UPDATE po_fe_ptn_lo SET %s"
						", ptn_range_type=%u, ptn_obj_type=%u"
						", new_file_send_type=%u"
						", save_name='%s', save_path='%s', file_hash='%s', ptn_version='%s'"
						", ptn_unit_num=%u, ptn_chk_value=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfpl.nPtnRangeType, dpfpl.nPtnObjType,
						dpfpl.nNewFileSendType,
						dpfpl.strSaveName.c_str(), dpfpl.strSavePath.c_str(), dpfpl.strFileHash.c_str(), dpfpl.strPtnVersion.c_str(),
						dpfpl.nPtnUnitNum, dpfpl.nPtnChkValue,
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

INT32	CDBMgrPoFePtnLo::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFePtnLo* ptDBList = (TListDBPoFePtnLo*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnLo::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FE_PTN_LO pd_t = (PDB_PO_FE_PTN_LO)lpTemplet;

    return InsertPoFePtnLo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnLo::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FE_PTN_LO pd_t = (PDB_PO_FE_PTN_LO)lpTemplet;

    return UpdatePoFePtnLo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFePtnLo::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






