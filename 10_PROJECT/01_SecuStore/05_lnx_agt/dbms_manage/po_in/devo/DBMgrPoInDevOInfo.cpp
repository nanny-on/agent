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


#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrPoInDevOInfo.h"

//---------------------------------------------------------------------------

CDBMgrPoInDevOInfo*		t_DBMgrPoInDevOInfo = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInDevOInfo::CDBMgrPoInDevOInfo()
{
	m_strDBTName = "po_in_devo_info";
}
//---------------------------------------------------------------------------

CDBMgrPoInDevOInfo::~CDBMgrPoInDevOInfo()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOInfo::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_devo_info][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOInfo::LoadDB(TListDBPoInDevOInfo& tDBPoInDevOInfoList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_DEVO_INFO didi;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", policy_type, device_name, device_type, device_ins_id, device_pa_ins_id"
						" FROM po_in_devo_info WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = didi.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		didi.nPolicyType			= GetDBField_UInt64(nIndex++);
		didi.strDvName				= GetDBField_String(nIndex++);
		didi.nDvType				= GetDBField_UInt(nIndex++);
		didi.strDvInstanceID		= GetDBField_String(nIndex++);
		didi.strDvPaInstanceID		= GetDBField_String(nIndex++);
        
        tDBPoInDevOInfoList.push_back(didi);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());

	m_nLoadNumber = tDBPoInDevOInfoList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOInfo::InsertPoInDevOInfo(DB_PO_IN_DEVO_INFO& didi)
{
	DB_PO_HEADER& tDPH = didi.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_devo_info("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", policy_type, device_name, device_type, device_ins_id, device_pa_ins_id"
						") VALUES (%s"
                        ", '%llu', '%s', %u, '%s', '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						didi.nPolicyType, didi.strDvName.c_str(), didi.nDvType,	didi.strDvInstanceID.c_str(), didi.strDvPaInstanceID.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInDevOInfo::UpdatePoInDevOInfo(DB_PO_IN_DEVO_INFO& didi)
{
	DB_PO_HEADER& tDPH = didi.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_devo_info SET %s"
						", policy_type='%llu', device_name='%s', device_type=%u, device_ins_id='%s', device_pa_ins_id='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						didi.nPolicyType, didi.strDvName.c_str(), didi.nDvType,	didi.strDvInstanceID.c_str(), didi.strDvPaInstanceID.c_str(),
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

INT32	CDBMgrPoInDevOInfo::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInDevOInfo* ptDBList = (TListDBPoInDevOInfo*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOInfo::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_DEVO_INFO pd_t = (PDB_PO_IN_DEVO_INFO)lpTemplet;

    return InsertPoInDevOInfo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOInfo::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_DEVO_INFO pd_t = (PDB_PO_IN_DEVO_INFO)lpTemplet;

    return UpdatePoInDevOInfo(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInDevOInfo::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






