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
#include "DBMgrPoInPtnWL.h"

//---------------------------------------------------------------------------

CDBMgrPoInPtnWL*		t_DBMgrPoInPtnWL = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInPtnWL::CDBMgrPoInPtnWL()
{
	m_strDBTName = "po_in_ptn_wl";
}
//---------------------------------------------------------------------------

CDBMgrPoInPtnWL::~CDBMgrPoInPtnWL()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnWL::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnWL::LoadDB(TListDBPoInPtnWL& tDBPoInPtnWLList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_PTN_WL data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", save_name, save_path, file_hash, ptn_version"
						", ptn_unit_num, ptn_chk_value "
						" FROM po_in_ptn_wl WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.tDPFI.strSaveName		= GetDBField_String(nIndex++);
		data.tDPFI.strSavePath		= GetDBField_String(nIndex++);
		data.tDPFI.strFileHash		= GetDBField_String(nIndex++);
		data.tDPFI.strPtnVersion	= GetDBField_String(nIndex++);
		data.tDPFI.nPtnUnitNum		= GetDBField_Int(nIndex++);
		data.tDPFI.nPtnChkValue		= GetDBField_Int(nIndex++);
		
        tDBPoInPtnWLList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInPtnWLList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnWL::InsertPoInPtnWL(DB_PO_IN_PTN_WL& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_ptn_wl("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", save_name, save_path, file_hash, ptn_version"
						", ptn_unit_num, ptn_chk_value"
						") VALUES (%s"
						", '%s', '%s', '%s', '%s'"
						", %u, %u);",
						GetPoHDRQuery_InsertValue(tDPH).c_str(),
						data.tDPFI.strSaveName.c_str(), data.tDPFI.strSavePath.c_str(), data.tDPFI.strFileHash.c_str(), data.tDPFI.strPtnVersion.c_str(),
						data.tDPFI.nPtnUnitNum, data.tDPFI.nPtnChkValue);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnWL::UpdatePoInPtnWL(DB_PO_IN_PTN_WL& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_ptn_wl SET %s"
						", save_name='%s', save_path='%s', file_hash='%s', ptn_version='%s'"
						", ptn_unit_num=%u, ptn_chk_value=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.tDPFI.strSaveName.c_str(), data.tDPFI.strSavePath.c_str(), data.tDPFI.strFileHash.c_str(), data.tDPFI.strPtnVersion.c_str(),
						data.tDPFI.nPtnUnitNum, data.tDPFI.nPtnChkValue,
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnWL::DeletePoInPtnWL(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
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

INT32	CDBMgrPoInPtnWL::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInPtnWL* ptDWList = (TListDBPoInPtnWL*)lpTempletList;

    return LoadDB(*ptDWList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnWL::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_WL pd_t = (PDB_PO_IN_PTN_WL)lpTemplet;

    return InsertPoInPtnWL(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnWL::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_WL pd_t = (PDB_PO_IN_PTN_WL)lpTemplet;

    return UpdatePoInPtnWL(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnWL::DeleteExecute(UINT32 nID)
{
	return DeletePoInPtnWL(nID);
}
//---------------------------------------------------------------------------






