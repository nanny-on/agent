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
#include "DBMgrPoNcPtnLo.h"

//---------------------------------------------------------------------------

CDBMgrPoNcPtnLo*		t_DBMgrPoNcPtnLo = NULL;

//---------------------------------------------------------------------------

CDBMgrPoNcPtnLo::CDBMgrPoNcPtnLo()
{
	m_strDBTName = "po_nc_ptn_lo";
}
//---------------------------------------------------------------------------

CDBMgrPoNcPtnLo::~CDBMgrPoNcPtnLo()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnLo::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_nc_ptn_lo][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnLo::LoadDB(TListDBPoNcPtnLo& tDBPoNcPtnLoList)
{
    UINT32 nReadCnt = 0;
    DB_PO_NC_PTN_LO data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", ptn_range_type, ptn_obj_type, user_edit_ptn"
						", save_name, save_path, file_hash, ptn_version"
						", ptn_unit_num, ptn_chk_value "
						" FROM po_nc_ptn_lo WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nPtnRangeType			= GetDBField_UInt(nIndex++);
		data.nPtnObjType			= GetDBField_UInt(nIndex++);
		data.nUserEditPtn			= GetDBField_UInt(nIndex++);

		data.strSaveName			= GetDBField_String(nIndex++);
		data.strSavePath			= GetDBField_String(nIndex++);
		data.strFileHash			= GetDBField_String(nIndex++);
		data.strPtnVersion			= GetDBField_String(nIndex++);
		data.nPtnUnitNum			= GetDBField_Int(nIndex++);
		data.nPtnChkValue			= GetDBField_Int(nIndex++);
        
        tDBPoNcPtnLoList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoNcPtnLoList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnLo::InsertPoNcPtnLo(DB_PO_NC_PTN_LO& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_nc_ptn_lo("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", ptn_range_type, ptn_obj_type, user_edit_ptn"
						", save_name, save_path, file_hash, ptn_version"
						", ptn_unit_num, ptn_chk_value"
						") VALUES (%s "
						", %u, %u, %u"
						", '%s', '%s', '%s', '%s'"
						", %u, %u);",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nPtnRangeType, data.nPtnObjType, data.nUserEditPtn,
						data.strSaveName.c_str(), data.strSavePath.c_str(), data.strFileHash.c_str(), data.strPtnVersion.c_str(),
						data.nPtnUnitNum, data.nPtnChkValue);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoNcPtnLo::UpdatePoNcPtnLo(DB_PO_NC_PTN_LO& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_nc_ptn_lo SET %s"
						", ptn_range_type=%u, ptn_obj_type=%u, user_edit_ptn=%u"
						", save_name='%s', save_path='%s', file_hash='%s', ptn_version='%s'"
						", ptn_unit_num=%u, ptn_chk_value=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nPtnRangeType, data.nPtnObjType, data.nUserEditPtn,
						data.strSaveName.c_str(), data.strSavePath.c_str(), data.strFileHash.c_str(), data.strPtnVersion.c_str(),
						data.nPtnUnitNum, data.nPtnChkValue,
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

INT32	CDBMgrPoNcPtnLo::LoadExecute(PVOID lpTempletList)
{
	TListDBPoNcPtnLo* ptDBList = (TListDBPoNcPtnLo*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnLo::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_LO pd_t = (PDB_PO_NC_PTN_LO)lpTemplet;

    return InsertPoNcPtnLo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnLo::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_NC_PTN_LO pd_t = (PDB_PO_NC_PTN_LO)lpTemplet;

    return UpdatePoNcPtnLo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoNcPtnLo::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






