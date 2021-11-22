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
#include "DBMgrPtnPatch.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnPatch::CDBMgrPtnPatch()
{
	m_strDBTName = "ptn_patch";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnPatch::~CDBMgrPtnPatch()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatch::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatch::LoadDB(TListDBPtnPatch& tDBPtnPatchList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_PATCH data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_mode, rev_id, type"
						", announce, name, descr, name_eng, descr_eng"
						", risk, reboot, rollbk, apply_order"
						", vuln_name, vuln_publisher, vuln_guid, vuln_kb_id"
						", refer, replace_id"
						", target_comp, target_family, target_product, proc_arch"
						", source_type, ptn_cr_date, ptn_mod_date"
						" FROM ptn_patch");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;
    
    do
    {
    	nIndex = 0;
    	
		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedMode				= GetDBField_Int(nIndex++);
		data.nRevID					= GetDBField_Int(nIndex++);
		data.nType					= GetDBField_Int(nIndex++);

		data.nAnnounce				= GetDBField_Int(nIndex++);
		data.tLanKor.strName		= GetDBField_String(nIndex++);
		data.tLanKor.strDescr		= GetDBField_String(nIndex++);
		data.tLanEng.strName		= GetDBField_String(nIndex++);
		data.tLanEng.strDescr		= GetDBField_String(nIndex++);		

		data.nRisk					= GetDBField_Int(nIndex++);
		data.nReBoot				= GetDBField_Int(nIndex++);
		data.nRollBack				= GetDBField_Int(nIndex++);
		data.nApplyOrder			= GetDBField_Int(nIndex++);

		data.strVulnName			= GetDBField_String(nIndex++);
		data.strVulnPublisher		= GetDBField_String(nIndex++);
		data.strVulnGuid			= GetDBField_String(nIndex++);
		data.nVulnKBID				= GetDBField_Int(nIndex++);

		data.strRefer				= GetDBField_String(nIndex++);
		data.nReplace				= GetDBField_Int(nIndex++);

		data.strTarComp				= GetDBField_String(nIndex++);
		data.strTarFamily			= GetDBField_String(nIndex++);
		data.strTarProduct			= GetDBField_String(nIndex++);
		data.nProcArch				= GetDBField_Int(nIndex++);

		data.nSrcType				= GetDBField_Int(nIndex++);
		data.nPtnCrDate				= GetDBField_Int(nIndex++);
		data.nPtnModDate			= GetDBField_Int(nIndex++);
				
        tDBPtnPatchList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = (UINT32)tDBPtnPatchList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatch::InsertPtnPatch(DB_PTN_PATCH& data)
{
	m_strQuery = SPrintf("INSERT INTO ptn_patch (id, used_mode, rev_id, type"
						", announce, name, descr, name_eng, descr_eng"
						", risk, reboot, rollbk, apply_order"
						", vuln_name, vuln_publisher, vuln_guid, vuln_kb_id"
						", refer, replace_id"
						", target_comp, target_family, target_product, proc_arch"
						", source_type, ptn_cr_date, ptn_mod_date"
						") VALUES (%u, %u, %u, %u"
						", %u, '%s', '%s', '%s', '%s'"
						", %u, %u, %u, %u"
						", '%s', '%s', '%s', %u"
						", '%s', %u"
						", '%s', '%s', '%s', %u"
						", %u, %u, %u"
						");",
						data.nID, data.nUsedMode, data.nRevID, data.nType,
						data.nAnnounce, data.tLanKor.strName.c_str(), data.tLanKor.strDescr.c_str(), data.tLanEng.strName.c_str(), data.tLanEng.strDescr.c_str(), 
						data.nRisk, data.nReBoot, data.nRollBack, data.nApplyOrder,
						data.strVulnName.c_str(), data.strVulnPublisher.c_str(), data.strVulnGuid.c_str(), data.nVulnKBID,
						data.strRefer.c_str(), data.nReplace,
						data.strTarComp.c_str(), data.strTarFamily.c_str(), data.strTarProduct.c_str(), data.nProcArch,
						data.nSrcType, data.nPtnCrDate, data.nPtnModDate);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatch::UpdatePtnPatch(DB_PTN_PATCH& data)
{
	m_strQuery = SPrintf("UPDATE ptn_patch SET used_mode=%u, rev_id=%u, type=%u"
						", announce=%u, name='%s', descr='%s', name_eng='%s', descr_eng='%s'"
						", risk=%u, reboot=%u, rollbk=%u, apply_order=%u"
						", vuln_name='%s', vuln_publisher='%s', vuln_guid='%s', vuln_kb_id=%u"
						", refer='%s', replace_id=%u"
						", target_comp='%s', target_family='%s', target_product='%s', proc_arch=%u"
						", source_type=%u, ptn_cr_date=%u, ptn_mod_date=%u"
						" WHERE id=%u;",
						data.nUsedMode, data.nRevID, data.nType,
						data.nAnnounce, data.tLanKor.strName.c_str(), data.tLanKor.strDescr.c_str(), data.tLanEng.strName.c_str(), data.tLanEng.strDescr.c_str(), 
						data.nRisk, data.nReBoot, data.nRollBack, data.nApplyOrder,
						data.strVulnName.c_str(), data.strVulnPublisher.c_str(), data.strVulnGuid.c_str(), data.nVulnKBID,
						data.strRefer.c_str(), data.nReplace,
						data.strTarComp.c_str(), data.strTarFamily.c_str(), data.strTarProduct.c_str(), data.nProcArch,
						data.nSrcType, data.nPtnCrDate, data.nPtnModDate,
						data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatch::DeletePtnPatch(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM ptn_patch WHERE id=%u;", nID);

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

INT32	CDBMgrPtnPatch::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnPatch* ptDBList = (TListDBPtnPatch*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatch::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH pd_t = (PDB_PTN_PATCH)lpTemplet;

    return InsertPtnPatch(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatch::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH pd_t = (PDB_PTN_PATCH)lpTemplet;

    return UpdatePtnPatch(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatch::DeleteExecute(UINT32 nID)
{
	return DeletePtnPatch(nID);
}
//---------------------------------------------------------------------------






