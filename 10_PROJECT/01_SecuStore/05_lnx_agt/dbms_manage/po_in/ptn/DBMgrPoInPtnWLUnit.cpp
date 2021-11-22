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
#include "DBMgrPoInPtnWLUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInPtnWLUnit*		t_DBMgrPoInPtnWLUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInPtnWLUnit::CDBMgrPoInPtnWLUnit()
{
	m_strDBTName = "po_in_ptn_wl_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInPtnWLUnit::~CDBMgrPoInPtnWLUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnWLUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnWLUnit::LoadDB(TListDBPoInPtnWLUnit& tDBPoInPtnWLUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_PTN_WL_UNIT data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", rt_mode, flt_chk_type"
						", file_name, file_path"
						", file_comp, file_desc"
						", file_hash"
						", file_sign_pub_name, file_sign_pub_sn"
						", file_sign_cnt_name, file_sign_cnt_sn"
						" FROM po_in_ptn_wl_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nRTMode				= GetDBField_UInt(nIndex++);
		data.nFltChkType			= GetDBField_UInt(nIndex++);

		data.tDFI.strName			= GetDBField_String(nIndex++);
		data.tDFI.strPath			= GetDBField_String(nIndex++);
		data.tDFI.strPublisher		= GetDBField_String(nIndex++);
		data.tDFI.strDescription	= GetDBField_String(nIndex++);
		data.tDFI.strHash			= GetDBField_String(nIndex++);
		data.tDFI.strSignPubName	= GetDBField_String(nIndex++);
		data.tDFI.strSignPubSN		= GetDBField_String(nIndex++);
		data.tDFI.strSignCntName	= GetDBField_String(nIndex++);
		data.tDFI.strSignCntSN		= GetDBField_String(nIndex++);
		
        tDBPoInPtnWLUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInPtnWLUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnWLUnit::InsertPoInPtnWLUnit(DB_PO_IN_PTN_WL_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_ptn_wl_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", rt_mode, flt_chk_type"
						", file_name, file_path"
						", file_comp, file_desc"
						", file_hash"						
						", file_sign_pub_name, file_sign_pub_sn"
						", file_sign_cnt_name, file_sign_cnt_sn"	
						") VALUES (%s"
						", %u, %u"
						", '%s', '%s'"
						", '%s', '%s'"
						", '%s'"
						", '%s', '%s'"
						", '%s', '%s');",	
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nRTMode, data.nFltChkType,
						data.tDFI.strName.c_str(), data.tDFI.strPath.c_str(), 
						data.tDFI.strPublisher.c_str(), data.tDFI.strDescription.c_str(), 
						data.tDFI.strHash.c_str(),
						data.tDFI.strSignPubName.c_str(), data.tDFI.strSignPubSN.c_str(),
						data.tDFI.strSignCntName.c_str(), data.tDFI.strSignCntSN.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnWLUnit::UpdatePoInPtnWLUnit(DB_PO_IN_PTN_WL_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_ptn_wl_unit SET %s"
						", rt_mode=%u, flt_chk_type=%u"
						", file_name='%s', file_path='%s'"
						", file_comp='%s', file_desc='%s'"
						", file_hash='%s'"
						", file_sign_pub_name='%s', file_sign_pub_sn='%s'"
						", file_sign_cnt_name='%s', file_sign_cnt_sn='%s' "							
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 	
						data.nRTMode, data.nFltChkType,
						data.tDFI.strName.c_str(), data.tDFI.strPath.c_str(), 
						data.tDFI.strPublisher.c_str(),	data.tDFI.strDescription.c_str(), 
						data.tDFI.strHash.c_str(), 
						data.tDFI.strSignPubName.c_str(), data.tDFI.strSignPubSN.c_str(),
						data.tDFI.strSignCntName.c_str(), data.tDFI.strSignCntSN.c_str(),			
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnWLUnit::DeletePoInPtnWLUnit(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_DELETE_FAIL;

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

INT32	CDBMgrPoInPtnWLUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInPtnWLUnit* ptDWList = (TListDBPoInPtnWLUnit*)lpTempletList;

    return LoadDB(*ptDWList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnWLUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_WL_UNIT pd_t = (PDB_PO_IN_PTN_WL_UNIT)lpTemplet;

    return InsertPoInPtnWLUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnWLUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_WL_UNIT pd_t = (PDB_PO_IN_PTN_WL_UNIT)lpTemplet;

    return UpdatePoInPtnWLUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnWLUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoInPtnWLUnit(nID);
}
//---------------------------------------------------------------------------






