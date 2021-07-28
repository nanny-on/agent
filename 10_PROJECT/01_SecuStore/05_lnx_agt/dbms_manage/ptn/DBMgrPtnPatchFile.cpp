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
#include "DBMgrPtnPatchFile.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnPatchFile::CDBMgrPtnPatchFile()
{
	m_strDBTName = "ptn_patch_file";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnPatchFile::~CDBMgrPtnPatchFile()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchFile::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchFile::LoadDB(TListDBPtnPatchFile& tDBPtnPatchFileList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_PATCH_FILE data;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_mode, rev_id, file_rev_id, lan_id, patch_type"
						", file_name, file_hash, file_size"
						", down_url"
						" FROM ptn_patch_file;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))        return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;

		data.nID					= GetDBField_Int(nIndex++);
		data.nUsedMode				= GetDBField_Int(nIndex++);
		data.nRevID					= GetDBField_Int(nIndex++);    	
		data.nFileRevID				= GetDBField_Int(nIndex++);    	
		data.nLanID					= GetDBField_Int(nIndex++);    	
		data.nPatchType				= GetDBField_Int(nIndex++);    	

		data.strFileName			= GetDBField_String(nIndex++);    	
		data.strFileHash			= GetDBField_String(nIndex++);    

		data.nFileSize				= GetDBField_Int(nIndex++);
		data.strDownURL				= GetDBField_String(nIndex++);
				
        tDBPtnPatchFileList.push_back(data);
        if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBPtnPatchFileList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchFile::InsertPtnPatchFile(DB_PTN_PATCH_FILE& data)
{
	m_strQuery = SPrintf("INSERT INTO ptn_patch_file (id, used_mode, rev_id, file_rev_id, lan_id, patch_type"
						", file_name, file_hash, file_size"
						", down_url"
						") VALUES (%u, %u, %u, %u, %u, %u"
						", '%s', '%s', %u"
						", '%s'"
						");",
						data.nID, data.nUsedMode, data.nRevID, data.nFileRevID, data.nLanID, data.nPatchType,
						data.strFileName.c_str(), data.strFileHash.c_str(), data.nFileSize,
						data.strDownURL.c_str()
						);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(data.nID == 0)
		data.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchFile::UpdatePtnPatchFile(DB_PTN_PATCH_FILE& data)
{
	m_strQuery = SPrintf("UPDATE ptn_patch_file SET used_mode=%u, rev_id=%u, file_rev_id=%u, lan_id=%u, patch_type=%u"
					", file_name='%s', file_hash='%s', file_size=%u"
					", down_url='%s'"
					" WHERE id=%u;",
					data.nUsedMode, data.nRevID, data.nFileRevID, data.nLanID, data.nPatchType,
					data.strFileName.c_str(), data.strFileHash.c_str(), data.nFileSize,
					data.strDownURL.c_str(),
					data.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnPatchFile::DeletePtnPatchFile(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM ptn_patch_file WHERE id=%u;", nID);

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

INT32	CDBMgrPtnPatchFile::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnPatchFile* ptDBList = (TListDBPtnPatchFile*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchFile::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH_FILE pd_t = (PDB_PTN_PATCH_FILE)lpTemplet;

    return InsertPtnPatchFile(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchFile::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_PATCH_FILE pd_t = (PDB_PTN_PATCH_FILE)lpTemplet;

    return UpdatePtnPatchFile(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnPatchFile::DeleteExecute(UINT32 nID)
{
	return DeletePtnPatchFile(nID);
}
//---------------------------------------------------------------------------






