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
#include "DBMgrPoFeExcludeUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoFeExcludeUnit*		t_DBMgrPoFeExcludeUnit;

//---------------------------------------------------------------------------

CDBMgrPoFeExcludeUnit::CDBMgrPoFeExcludeUnit()
{
	m_strDBTName = "po_fe_exclude_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoFeExcludeUnit::~CDBMgrPoFeExcludeUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeExcludeUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeExcludeUnit::LoadDB(TListDBPoFeExcludeUnit& tDBPoFeExcludeUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_FE_EXCLUDE_UNIT dpfeu;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", unit_type, block_mode, chk_level"
						", file_path, file_comp, file_desc, file_hash"
						", file_sign, file_sign_comp"
						", user_name"
						" FROM po_fe_exclude_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfeu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfeu.nUnitType				= GetDBField_UInt(nIndex++);
		dpfeu.nBlockMode			= GetDBField_UInt(nIndex++);
		dpfeu.nChkLevel				= GetDBField_UInt(nIndex++);

		dpfeu.strFilePath			= GetDBField_String(nIndex++);
		dpfeu.strFileComp			= GetDBField_String(nIndex++);
		dpfeu.strFileDesc			= GetDBField_String(nIndex++);
		dpfeu.strFileHash			= GetDBField_String(nIndex++);
		dpfeu.strFileSign			= GetDBField_String(nIndex++);
		dpfeu.strFileSignComp		= GetDBField_String(nIndex++);
		dpfeu.strUserName			= GetDBField_String(nIndex++);
		
        tDBPoFeExcludeUnitList.push_back(dpfeu);
        if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoFeExcludeUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeExcludeUnit::InsertPoFeExcludeUnit(DB_PO_FE_EXCLUDE_UNIT& dpfeu)
{
	DB_PO_HEADER& tDPH = dpfeu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fe_exclude_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", unit_type, block_mode, chk_level"
						", file_path, file_comp, file_desc, file_hash"
						", file_sign, file_sign_comp"
						", user_name "
						") VALUES (%s"
						", %u, %u, %u"
						", '%s', '%s', '%s', '%s'"
						", '%s', '%s'"
						", '%s');",
						GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpfeu.nUnitType, dpfeu.nBlockMode, dpfeu.nChkLevel,
						dpfeu.strFilePath.c_str(), dpfeu.strFileComp.c_str(), dpfeu.strFileDesc.c_str(), dpfeu.strFileHash.c_str(), 
						dpfeu.strFileSign.c_str(), dpfeu.strFileSignComp.c_str(),
						dpfeu.strUserName.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFeExcludeUnit::UpdatePoFeExcludeUnit(DB_PO_FE_EXCLUDE_UNIT& dpfeu)
{
	DB_PO_HEADER& tDPH = dpfeu.tDPH;

	m_strQuery = SPrintf("UPDATE po_fe_exclude_unit SET %s"
						", unit_type=%u, block_mode=%u, chk_level=%u"
						", file_path='%s', file_comp='%s', file_desc='%s', file_hash='%s'"
						", file_sign='%s', file_sign_comp='%s'"
						", user_name='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dpfeu.nUnitType, dpfeu.nBlockMode, dpfeu.nChkLevel, 
						dpfeu.strFilePath.c_str(), dpfeu.strFileComp.c_str(), dpfeu.strFileDesc.c_str(), dpfeu.strFileHash.c_str(), 
						dpfeu.strFileSign.c_str(), dpfeu.strFileSignComp.c_str(),
						dpfeu.strUserName.c_str(),
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

INT32	CDBMgrPoFeExcludeUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFeExcludeUnit* ptDBList = (TListDBPoFeExcludeUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeExcludeUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FE_EXCLUDE_UNIT pd_t = (PDB_PO_FE_EXCLUDE_UNIT)lpTemplet;

    return InsertPoFeExcludeUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeExcludeUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FE_EXCLUDE_UNIT pd_t = (PDB_PO_FE_EXCLUDE_UNIT)lpTemplet;

    return UpdatePoFeExcludeUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFeExcludeUnit::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






