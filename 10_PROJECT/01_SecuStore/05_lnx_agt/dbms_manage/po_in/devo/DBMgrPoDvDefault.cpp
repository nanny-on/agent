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


#include "stdafx.h"
#include "com_struct.h"
#include "DBMgrPoDvDefault.h"

//---------------------------------------------------------------------------

CDBMgrPoDvDefault*		t_DBMgrPoDvDefault = NULL;

//---------------------------------------------------------------------------

CDBMgrPoDvDefault::CDBMgrPoDvDefault()
{
	m_strDBTName = "po_dv_default";
}
//---------------------------------------------------------------------------

CDBMgrPoDvDefault::~CDBMgrPoDvDefault()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDvDefault::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_devo_info][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDvDefault::LoadDB(TListDBPoDvDefault& tDBPoDvDefaultList)
{
    UINT32 nReadCnt = 0;
    DB_PO_DV_DEFAULT ddd;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", policy_perm, device_pa_name, device_name, device_ins_id, device_pa_ins_id"
						" FROM po_dv_default WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = ddd.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		ddd.nPoPerm					= GetDBField_UInt(nIndex++);
		ddd.strDvPaName				= GetDBField_String(nIndex++);
		ddd.strDvName				= GetDBField_String(nIndex++);
		ddd.strDvInstanceID			= GetDBField_String(nIndex++);
		ddd.strDvPaInstanceID		= GetDBField_String(nIndex++);
        
        tDBPoDvDefaultList.push_back(ddd);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoDvDefaultList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDvDefault::InsertPoDvDefault(DB_PO_DV_DEFAULT& didi)
{
	DB_PO_HEADER& tDPH = didi.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_dv_default("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", policy_perm, device_pa_name, device_name, device_ins_id, device_pa_ins_id"
						") VALUES (%s"
                        ", %u, '%s', '%s', '%s', '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						didi.nPoPerm, didi.strDvPaName.c_str(), didi.strDvName.c_str(),	didi.strDvInstanceID.c_str(), didi.strDvPaInstanceID.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDvDefault::UpdatePoDvDefault(DB_PO_DV_DEFAULT& didi)
{
	DB_PO_HEADER& tDPH = didi.tDPH;

	m_strQuery = SPrintf("UPDATE po_dv_default SET %s"
						", policy_perm=%u, device_pa_name='%s', device_name='%s', device_ins_id='%s', device_pa_ins_id='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						didi.nPoPerm, didi.strDvPaName.c_str(), didi.strDvName.c_str(), didi.strDvInstanceID.c_str(), didi.strDvPaInstanceID.c_str(),
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

INT32	CDBMgrPoDvDefault::LoadExecute(PVOID lpTempletList)
{
	TListDBPoDvDefault* ptDBList = (TListDBPoDvDefault*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDvDefault::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_DV_DEFAULT pd_t = (PDB_PO_DV_DEFAULT)lpTemplet;

    return InsertPoDvDefault(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDvDefault::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_DV_DEFAULT pd_t = (PDB_PO_DV_DEFAULT)lpTemplet;

    return UpdatePoDvDefault(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDvDefault::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






