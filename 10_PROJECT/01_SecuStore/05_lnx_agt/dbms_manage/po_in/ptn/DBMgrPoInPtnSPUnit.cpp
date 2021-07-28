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
#include "DBMgrPoInPtnSPUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoInPtnSPUnit*		t_DBMgrPoInPtnSPUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInPtnSPUnit::CDBMgrPoInPtnSPUnit()
{
	m_strDBTName = "po_in_ptn_sp_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoInPtnSPUnit::~CDBMgrPoInPtnSPUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnSPUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnSPUnit::LoadDB(TListDBPoInPtnSPUnit& tDBPoInPtnSPUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_PTN_SP_UNIT data;

    INT32 nIndex = 0;	

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT						
						", block_mode, confirm_info"
						", flt_chk_type"
						", file_name, file_path"
						", file_comp, file_desc"
						", file_hash"
						", file_sign_pub_name, file_sign_pub_sn"
						", file_sign_cnt_name, file_sign_cnt_sn"
						" FROM po_in_ptn_sp_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;
		UINT32 nConfirmInfo = 0;

		tDPH						= GetDBField_PoHDR(nIndex);	

		data.nBlockMode				= GetDBField_UInt(nIndex++);
		nConfirmInfo				= GetDBField_UInt(nIndex++);
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

		data.nConfirmLevel			= LOTYPEID32(nConfirmInfo);
		data.nConfirmDay			= HITYPEID32(nConfirmInfo);
		
        tDBPoInPtnSPUnitList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInPtnSPUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnSPUnit::InsertPoInPtnSPUnit(DB_PO_IN_PTN_SP_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;
	UINT32 nConfirmInfo = MAKETYPEID32(data.nConfirmLevel, data.nConfirmDay);

	m_strQuery = SPrintf("INSERT INTO po_in_ptn_sp_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME	
						", block_mode, confirm_info"
						", flt_chk_type"
						", file_name, file_path"
						", file_comp, file_desc"
						", file_hash"						
						", file_sign_pub_name, file_sign_pub_sn"
						", file_sign_cnt_name, file_sign_cnt_sn"		
						") VALUES (%s"
						", %u, %u"
						", %u"
						", '%s', '%s'"
						", '%s', '%s'"
						", '%s'"
						", '%s', '%s'"
						", '%s', '%s');",						
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 	
						data.nBlockMode, nConfirmInfo,
						data.nFltChkType,
						data.tDFI.strName.c_str(), data.tDFI.strPath.c_str(), 
						data.tDFI.strPublisher.c_str(), data.tDFI.strDescription.c_str(), 
						data.tDFI.strHash.c_str(),
						data.tDFI.strSignPubName.c_str(), data.tDFI.strSignPubSN.c_str(),
						data.tDFI.strSignCntName.c_str(), data.tDFI.strSignCntSN.c_str());		

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnSPUnit::UpdatePoInPtnSPUnit(DB_PO_IN_PTN_SP_UNIT& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;
	UINT32 nConfirmInfo = MAKETYPEID32(data.nConfirmLevel, data.nConfirmDay);

	m_strQuery = SPrintf("UPDATE po_in_ptn_sp_unit SET %s"	
						", block_mode=%u, confirm_info=%u"
						", flt_chk_type=%u"
						", file_name='%s', file_path='%s'"
						", file_comp='%s', file_desc='%s'"
						", file_hash='%s'"
						", file_sign_pub_name='%s', file_sign_pub_sn='%s'"
						", file_sign_cnt_name='%s', file_sign_cnt_sn='%s' "						
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 	
						data.nBlockMode, nConfirmInfo,
						data.nFltChkType,
						data.tDFI.strName.c_str(), data.tDFI.strPath.c_str(), 
						data.tDFI.strPublisher.c_str(),	data.tDFI.strDescription.c_str(), 
						data.tDFI.strHash.c_str(), 
						data.tDFI.strSignPubName.c_str(), data.tDFI.strSignPubSN.c_str(),
						data.tDFI.strSignCntName.c_str(), data.tDFI.strSignCntSN.c_str(),
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

INT32	CDBMgrPoInPtnSPUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInPtnSPUnit* ptDBList = (TListDBPoInPtnSPUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnSPUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_SP_UNIT pd_t = (PDB_PO_IN_PTN_SP_UNIT)lpTemplet;

    return InsertPoInPtnSPUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnSPUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_SP_UNIT pd_t = (PDB_PO_IN_PTN_SP_UNIT)lpTemplet;

    return UpdatePoInPtnSPUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnSPUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






