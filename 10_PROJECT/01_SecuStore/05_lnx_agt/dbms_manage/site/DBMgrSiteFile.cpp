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
#include "DBMgrSiteFile.h"

//---------------------------------------------------------------------------

CDBMgrSiteFile*		t_DBMgrSiteFile = NULL;

//---------------------------------------------------------------------------

CDBMgrSiteFile::CDBMgrSiteFile()
{
	m_strDBTName = "site_file";
}
//---------------------------------------------------------------------------

CDBMgrSiteFile::~CDBMgrSiteFile()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteFile::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteFile::LoadDB(TListDBSiteFile& tDBSiteFileList)
{
    UINT32 nReadCnt = 0;
    DB_SITE_FILE dsf;

    INT32 nIndex = 0;

	m_strQuery = SPrintf("SELECT id, used_flag, reg_date, used_mode, "
						"reg_svr_id, sync_svr_step, "
						"fe_key, file_hash, file_path, file_name, file_descr, file_ver, file_size, "
						"pub_name, pub_sn, cnt_name, cnt_sn, "
						"publisher_name, product_name, "
						"pe_type, pe_bit  "
						"FROM site_file;");

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
		dsf.nID					= GetDBField_Int(nIndex++) ;
		dsf.nUsedFlag			= GetDBField_Int(nIndex++) ;
		dsf.nRegDate			= GetDBField_Int(nIndex++) ;
		dsf.nUsedMode			= GetDBField_Int(nIndex++) ;

		dsf.nRegSvrID			= GetDBField_Int(nIndex++) ;
		dsf.nSyncSvrStep		= GetDBField_Int(nIndex++);

		dsf.strFeKey			= GetDBField_String(nIndex++);
		dsf.strFileHash			= GetDBField_String(nIndex++);
		dsf.strFilePath			= GetDBField_String(nIndex++);
		dsf.strFileName			= GetDBField_String(nIndex++);
		dsf.strFileDescr		= GetDBField_String(nIndex++);
		dsf.strFileVersion		= GetDBField_String(nIndex++);
		dsf.nFileSize			= GetDBField_Int(nIndex++) ;

		dsf.strPubName			= GetDBField_String(nIndex++);
		dsf.strPubSN			= GetDBField_String(nIndex++);
		dsf.strCntName			= GetDBField_String(nIndex++);
		dsf.strCntSN			= GetDBField_String(nIndex++);
		dsf.strPublisherName	= GetDBField_String(nIndex++);
		dsf.strProductName		= GetDBField_String(nIndex++);
		dsf.nPeType				= GetDBField_Int(nIndex++) ;
		dsf.nPeBit				= GetDBField_Int(nIndex++) ;

		{
			QueryToMem(dsf.strFilePath);
			QueryToMem(dsf.strFileName);
		}
		
        tDBSiteFileList.push_back(dsf);
        if(m_nLoadMaxID < UINT32(dsf.nID))
			m_nLoadMaxID = dsf.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = (UINT32)tDBSiteFileList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteFile::InsertSiteFile(DB_SITE_FILE& dsf)
{
	String strFilePath		= MemToQuery(dsf.strFilePath);
	String strFileName		= MemToQuery(dsf.strFileName);

	m_strQuery = SPrintf("INSERT INTO site_file (used_flag, reg_date, used_mode, "
											"reg_svr_id, sync_svr_step, "
											"fe_key, file_hash, file_path, file_name, file_descr, file_ver, file_size, "
											"pub_name, pub_sn, cnt_name, cnt_sn, "
											"publisher_name, product_name, "
											"pe_type, pe_bit )"
									"VALUES ( "
									"%u, %u, %u, "
									"%u, %u, "
									"'%s', '%s', '%s', '%s', '%s', '%s', %u, "
									"'%s', '%s', '%s', '%s',"
									"'%s', '%s', "
									"%u, %u);",
                                    dsf.nUsedFlag, dsf.nRegDate, dsf.nUsedMode, 
									dsf.nRegSvrID, dsf.nSyncSvrStep,
									dsf.strFeKey.c_str(), dsf.strFileHash.c_str(), strFilePath.c_str(), strFileName.c_str(),
									dsf.strFileDescr.c_str(), dsf.strFileVersion.c_str(), dsf.nFileSize,
									dsf.strPubName.c_str(), dsf.strPubSN.c_str(), dsf.strCntName.c_str(), dsf.strCntSN.c_str(),
									dsf.strPublisherName.c_str(), dsf.strProductName.c_str(),
									dsf.nPeType, dsf.nPeBit);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(dsf.nID == 0)
		dsf.nID      = GetLastID();
    
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteFile::UpdateSiteFile(DB_SITE_FILE& dsf)
{
	String strFilePath		= MemToQuery(dsf.strFilePath);
	String strFileName		= MemToQuery(dsf.strFileName);

	m_strQuery = SPrintf("UPDATE site_file SET "
						"used_flag=%u, reg_date=%u, used_mode=%u, "
						"reg_svr_id=%u, sync_svr_step=%u, "
						"fe_key='%s', file_hash='%s', file_path='%s', file_name='%s', file_descr='%s', file_ver='%s', file_size=%u, "
						"pub_name='%s', pub_sn='%s', cnt_name='%s', cnt_sn='%s', "
						"publisher_name='%s', product_name='%s', "
						"pe_type=%u, pe_bit=%u "
						"WHERE id=%u;",
						dsf.nUsedFlag, dsf.nRegDate, dsf.nUsedMode, 
						dsf.nRegSvrID, dsf.nSyncSvrStep,
						dsf.strFeKey.c_str(), dsf.strFileHash.c_str(), strFilePath.c_str(), strFileName.c_str(),
						dsf.strFileDescr.c_str(), dsf.strFileVersion.c_str(), dsf.nFileSize,
						dsf.strPubName.c_str(), dsf.strPubSN.c_str(), dsf.strCntName.c_str(), dsf.strCntSN.c_str(),
						dsf.strPublisherName.c_str(), dsf.strProductName.c_str(),
						dsf.nPeType, dsf.nPeBit,
						dsf.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrSiteFile::DeleteSiteFile(UINT32 nID)
{
	m_strQuery = SPrintf("DELETE FROM site_file WHERE id=%u;", nID);

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

INT32	CDBMgrSiteFile::LoadExecute(PVOID lpTempletList)
{
	TListDBSiteFile* ptDBList = (TListDBSiteFile*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteFile::InsertExecute(PVOID lpTemplet)
{
	PDB_SITE_FILE pd_t = (PDB_SITE_FILE)lpTemplet;

    return InsertSiteFile(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteFile::UpdateExecute(PVOID lpTemplet)
{
	PDB_SITE_FILE pd_t = (PDB_SITE_FILE)lpTemplet;

    return UpdateSiteFile(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrSiteFile::DeleteExecute(UINT32 nID)
{
	return DeleteSiteFile(nID);
}
//---------------------------------------------------------------------------






