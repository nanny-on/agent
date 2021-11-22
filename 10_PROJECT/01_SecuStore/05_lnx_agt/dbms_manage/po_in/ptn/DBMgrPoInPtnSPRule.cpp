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
#include "DBMgrPoInPtnSPRule.h"

//---------------------------------------------------------------------------

CDBMgrPoInPtnSPRule*		t_DBMgrPoInPtnSPRule = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInPtnSPRule::CDBMgrPoInPtnSPRule()
{
	m_strDBTName = SPrintf("%s", "po_in_ptn_sp_rule");
}
//---------------------------------------------------------------------------

CDBMgrPoInPtnSPRule::~CDBMgrPoInPtnSPRule()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnSPRule::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnSPRule::LoadDB(TListDBPoInPtnSPRule& tDBPoInPtnSPRuleList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_PTN_SP_RULE data;

    INT32 nIndex = 0;	

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", target_type, target_id, file_id"
						", req_level, req_day"
						", confirm_type, confirm_id, confirm_rst"
						", use_day"
						", file_key, file_path, file_name"
						", evt_date, end_date"
						" FROM po_in_ptn_sp_rule WHERE used_flag=1;");

    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH					= GetDBField_PoHDR(nIndex);	

		data.nTargetType		= GetDBField_Int(nIndex++);
		data.nTargetID			= GetDBField_Int(nIndex++);
		data.nFileID			= GetDBField_Int(nIndex++);

		data.nReqLevel			= GetDBField_Int(nIndex++);
		data.nReqDay			= GetDBField_Int(nIndex++);

		data.nConfirmType		= GetDBField_Int(nIndex++);
		data.nConfirmID			= GetDBField_Int(nIndex++);
		data.nConfirmRst		= GetDBField_Int(nIndex++);

		data.nUseDay			= GetDBField_Int(nIndex++);

		data.strFileKey			= GetDBField_String(nIndex++);
		data.strFilePath		= GetDBField_String(nIndex++);
		data.strFileName		= GetDBField_String(nIndex++);

		data.nEvtDate			= GetDBField_Int(nIndex++);
		data.nEndDate			= GetDBField_Int(nIndex++);
		
        tDBPoInPtnSPRuleList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInPtnSPRuleList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnSPRule::InsertPoInPtnSPRule(DB_PO_IN_PTN_SP_RULE& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_ptn_sp_rule("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", target_type, target_id, file_id"
						", req_level, req_day"
						", confirm_type, confirm_id, confirm_rst"
						", use_day"
						", file_key, file_path, file_name"
						", evt_date, end_date"
						") VALUES (%s"
						", %u, %u, %u"
						", %u, %u"
						", %u, %u, %u"
						", %u"
						", '%s', '%s', '%s'"
						", %u, %u"
						");",
						GetPoHDRQuery_InsertValue(tDPH).c_str(),
						data.nTargetType, data.nTargetID, data.nFileID,
						data.nReqLevel, data.nReqDay,
						data.nConfirmType, data.nConfirmID, data.nConfirmRst,
						data.nUseDay, 
						data.strFileKey.c_str(), data.strFilePath.c_str(), data.strFileName.c_str(),
						data.nEvtDate, data.nEndDate);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInPtnSPRule::UpdatePoInPtnSPRule(DB_PO_IN_PTN_SP_RULE& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_ptn_sp_rule SET "
						"%s"
						", target_type=%u, target_id=%u, file_id=%u"
						", req_level=%u, req_day=%u"
						", confirm_type=%u, confirm_id=%u, confirm_rst=%u"
						", use_day=%u"
						", file_key='%s', file_path='%s', file_name='%s'"
						", evt_date=%u, end_date=%u"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						data.nTargetType, data.nTargetID, data.nFileID,
						data.nReqLevel, data.nReqDay,
						data.nConfirmType, data.nConfirmID, data.nConfirmRst, 
						data.nUseDay, 
						data.strFileKey.c_str(), data.strFilePath.c_str(), data.strFileName.c_str(),
						data.nEvtDate, data.nEndDate,
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

INT32	CDBMgrPoInPtnSPRule::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInPtnSPRule* ptDBList = (TListDBPoInPtnSPRule*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnSPRule::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_SP_RULE pd_t = (PDB_PO_IN_PTN_SP_RULE)lpTemplet;

    return InsertPoInPtnSPRule(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnSPRule::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_PTN_SP_RULE pd_t = (PDB_PO_IN_PTN_SP_RULE)lpTemplet;

    return UpdatePoInPtnSPRule(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInPtnSPRule::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






