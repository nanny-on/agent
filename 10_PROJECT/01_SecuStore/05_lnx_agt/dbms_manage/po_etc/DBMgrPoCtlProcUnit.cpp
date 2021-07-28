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
#include "DBMgrPoCtlProcUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoCtlProcUnit*		t_DBMgrPoCtlProcUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoCtlProcUnit::CDBMgrPoCtlProcUnit()
{
	m_strDBTName = "po_ctl_proc_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoCtlProcUnit::~CDBMgrPoCtlProcUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlProcUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlProcUnit::LoadDB(TListDBPoCtlProcUnit& tDBPoCtlProcUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_CTL_PROC_UNIT dpcpu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", block_mode, file_path, file_comp, file_desc, file_hash"
						", file_sign, file_sign_comp"
						" FROM po_ctl_proc_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpcpu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpcpu.nBlockMode			= GetDBField_UInt(nIndex++);
		dpcpu.strFilePath			= GetDBField_String(nIndex++);
		dpcpu.strFileComp			= GetDBField_String(nIndex++);
		dpcpu.strFileDesc			= GetDBField_String(nIndex++);
		dpcpu.strFileHash			= GetDBField_String(nIndex++);
		dpcpu.strFileSign			= GetDBField_String(nIndex++);
		dpcpu.strFileSignComp		= GetDBField_String(nIndex++);
        
        tDBPoCtlProcUnitList.push_back(dpcpu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoCtlProcUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlProcUnit::InsertPoCtlProcUnit(DB_PO_CTL_PROC_UNIT& dpcpu)
{
	DB_PO_HEADER& tDPH = dpcpu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_ctl_proc_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", block_mode"
						", file_path, file_comp, file_desc, file_hash"
						", file_sign, file_sign_comp"
						") VALUES (%s"
                        ", %u"
						", '%s', '%s', '%s', '%s'"
						", '%s', '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpcpu.nBlockMode, 
						dpcpu.strFilePath.c_str(), dpcpu.strFileComp.c_str(), dpcpu.strFileDesc.c_str(), dpcpu.strFileHash.c_str(),
						dpcpu.strFileSign.c_str(), dpcpu.strFileSignComp.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlProcUnit::UpdatePoCtlProcUnit(DB_PO_CTL_PROC_UNIT& dpcpu)
{
	DB_PO_HEADER& tDPH = dpcpu.tDPH;

	m_strQuery = SPrintf("UPDATE po_ctl_proc_unit SET %s"
						", block_mode=%u"
						", file_path='%s', file_comp='%s', file_desc='%s', file_hash='%s'"
						", file_sign='%s', file_sign_comp='%s' "
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dpcpu.nBlockMode,  
						dpcpu.strFilePath.c_str(), dpcpu.strFileComp.c_str(), dpcpu.strFileDesc.c_str(), dpcpu.strFileHash.c_str(),
						dpcpu.strFileSign.c_str(), dpcpu.strFileSignComp.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoCtlProcUnit::DeletePoCtlProcUnit(UINT32 nID)
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

INT32	CDBMgrPoCtlProcUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoCtlProcUnit* ptDBList = (TListDBPoCtlProcUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoCtlProcUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_CTL_PROC_UNIT pd_t = (PDB_PO_CTL_PROC_UNIT)lpTemplet;

    return InsertPoCtlProcUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoCtlProcUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_CTL_PROC_UNIT pd_t = (PDB_PO_CTL_PROC_UNIT)lpTemplet;

    return UpdatePoCtlProcUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoCtlProcUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoCtlProcUnit(nID);
}
//---------------------------------------------------------------------------






