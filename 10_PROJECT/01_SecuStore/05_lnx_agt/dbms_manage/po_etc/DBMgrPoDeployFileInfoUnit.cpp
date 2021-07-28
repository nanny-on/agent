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
#include "DBMgrPoDeployFileInfoUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoDeployFileInfoUnit*		t_DBMgrPoDeployFileInfoUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoDeployFileInfoUnit::CDBMgrPoDeployFileInfoUnit()
{
	m_strDBTName = "po_deploy_file_info_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoDeployFileInfoUnit::~CDBMgrPoDeployFileInfoUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileInfoUnit::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileInfoUnit::LoadDB(TListDBPoDeployFileInfoUnit& tDBPoDeployFileInfoUnitList)
{
    UINT32 nReadCnt = 0;
    DB_PO_DEPLOY_FILE_INFO_UNIT dpdfui;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", exe_user_type, file_type, file_enc_type, file_or_name, file_save_name, file_hash, file_command"
						", target_path, exe_num_type, schedule_used_mode"
						" FROM po_deploy_file_info_unit WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpdfui.tDPH;
		tDPH						= GetDBField_PoHDR(nIndex);

		dpdfui.nExeUserType			= GetDBField_UInt(nIndex++);
		dpdfui.nFileType			= GetDBField_UInt(nIndex++);
		dpdfui.nFileEncType			= GetDBField_UInt(nIndex++);
		dpdfui.strFileOrName		= GetDBField_String(nIndex++);
		dpdfui.strFileSaveName		= GetDBField_String(nIndex++);
		dpdfui.strFileHash			= GetDBField_String(nIndex++);
		dpdfui.strFileCommand		= GetDBField_String(nIndex++);

		dpdfui.strTargetPath		= GetDBField_String(nIndex++);
		dpdfui.nExeNumType			= GetDBField_UInt(nIndex++);
		dpdfui.nSchUsedMode			= GetDBField_UInt(nIndex++);
        
        tDBPoDeployFileInfoUnitList.push_back(dpdfui);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoDeployFileInfoUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileInfoUnit::InsertPoDeployFileInfoUnit(DB_PO_DEPLOY_FILE_INFO_UNIT& dpdfui)
{
	DB_PO_HEADER& tDPH = dpdfui.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_deploy_file_info_unit("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", exe_user_type, file_type, file_enc_type"
						", file_or_name, file_save_name, file_hash, file_command"
						", target_path, exe_num_type, schedule_used_mode"
						") VALUES (%s"
                        ", %u, %u, %u"
						", '%s', '%s', '%s', '%s'"
						", '%s', %u, %u"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						dpdfui.nExeUserType, dpdfui.nFileType, dpdfui.nFileEncType, 
						dpdfui.strFileOrName.c_str(), dpdfui.strFileSaveName.c_str(), dpdfui.strFileHash.c_str(), dpdfui.strFileCommand.c_str(),
						dpdfui.strTargetPath.c_str(), dpdfui.nExeNumType, dpdfui.nSchUsedMode);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileInfoUnit::UpdatePoDeployFileInfoUnit(DB_PO_DEPLOY_FILE_INFO_UNIT& dpdfui)
{
	DB_PO_HEADER& tDPH = dpdfui.tDPH;

	m_strQuery = SPrintf("UPDATE po_deploy_file_info_unit SET %s"
						", exe_user_type=%u, file_type=%u, file_enc_type=%u"
						", file_or_name='%s', file_save_name='%s', file_hash='%s', file_command='%s' "
						", target_path='%s', exe_num_type=%u, schedule_used_mode=%u "
						"WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(), 
						dpdfui.nExeUserType, dpdfui.nFileType, dpdfui.nFileEncType, 
						dpdfui.strFileOrName.c_str(), dpdfui.strFileSaveName.c_str(), dpdfui.strFileHash.c_str(), dpdfui.strFileCommand.c_str(),
						dpdfui.strTargetPath.c_str(), dpdfui.nExeNumType, dpdfui.nSchUsedMode,
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoDeployFileInfoUnit::DeletePoDeployFileInfoUnit(UINT32 nID)
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

INT32	CDBMgrPoDeployFileInfoUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoDeployFileInfoUnit* ptDBList = (TListDBPoDeployFileInfoUnit*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileInfoUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_INFO_UNIT pd_t = (PDB_PO_DEPLOY_FILE_INFO_UNIT)lpTemplet;

    return InsertPoDeployFileInfoUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileInfoUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_DEPLOY_FILE_INFO_UNIT pd_t = (PDB_PO_DEPLOY_FILE_INFO_UNIT)lpTemplet;

    return UpdatePoDeployFileInfoUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoDeployFileInfoUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoDeployFileInfoUnit(nID);
}
//---------------------------------------------------------------------------






