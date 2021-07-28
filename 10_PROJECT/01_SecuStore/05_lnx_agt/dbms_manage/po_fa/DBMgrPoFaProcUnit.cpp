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
#include "DBMgrPoFaProcUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoFaProcUnit*		t_DBMgrPoFaProcUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaProcUnit::CDBMgrPoFaProcUnit()
{
	m_strDBTName = "po_fa_proc_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoFaProcUnit::~CDBMgrPoFaProcUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaProcUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaProcUnit::LoadDB(TListDBPoFaProcUnit& tDBPoFaProcUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FA_PROC_UNIT dpfpu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", block_mode, file_path, file_comp, file_desc, file_hash"
						", file_sign, file_sign_comp"
						" FROM po_fa_proc_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfpu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfpu.nBlockMode			= GetDBField_UInt(nIndex++);
		dpfpu.strFilePath			= GetDBField_String(nIndex++);
		dpfpu.strFileComp			= GetDBField_String(nIndex++);
		dpfpu.strFileDesc			= GetDBField_String(nIndex++);
		dpfpu.strFileHash			= GetDBField_String(nIndex++);
		dpfpu.strFileSign			= GetDBField_String(nIndex++);
		dpfpu.strFileSignComp		= GetDBField_String(nIndex++);
		
        tDBPoFaProcUnitList.push_back(dpfpu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFaProcUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaProcUnit::InsertPoFaProcUnit(DB_PO_FA_PROC_UNIT& dpfpu)
{
	DB_PO_HEADER& tDPH = dpfpu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_proc_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", block_mode"
						", file_path, file_comp, file_desc, file_hash"
						", file_sign, file_sign_comp "
						") VALUES (%s"
                        ", %u"
						", '%s', '%s', '%s', '%s'"
						", '%s', '%s');",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpfpu.nBlockMode, 
						dpfpu.strFilePath.c_str(), dpfpu.strFileComp.c_str(), dpfpu.strFileDesc.c_str(), dpfpu.strFileHash.c_str(), 
						dpfpu.strFileSign.c_str(), dpfpu.strFileSignComp.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaProcUnit::UpdatePoFaProcUnit(DB_PO_FA_PROC_UNIT& dpfpu)
{
	DB_PO_HEADER& tDPH = dpfpu.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_proc_unit SET %s"
						", block_mode=%u"
						", file_path='%s', file_comp='%s', file_desc='%s', file_hash='%s'"
						", file_sign='%s', file_sign_comp='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						dpfpu.nBlockMode, 
						dpfpu.strFilePath.c_str(), dpfpu.strFileComp.c_str(), dpfpu.strFileDesc.c_str(), dpfpu.strFileHash.c_str(), 
						dpfpu.strFileSign.c_str(), dpfpu.strFileSignComp.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaProcUnit::DeletePoFaProcUnit(UINT32 nID)
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

INT32	CDBMgrPoFaProcUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaProcUnit* ptDBList = (TListDBPoFaProcUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaProcUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_PROC_UNIT pd_t = (PDB_PO_FA_PROC_UNIT)lpTemplet;

    return InsertPoFaProcUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaProcUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_PROC_UNIT pd_t = (PDB_PO_FA_PROC_UNIT)lpTemplet;

    return UpdatePoFaProcUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaProcUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoFaProcUnit(nID);
}
//---------------------------------------------------------------------------






