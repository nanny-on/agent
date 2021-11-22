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
#include "DBMgrPtnProcFile.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CDBMgrPtnProcFile::CDBMgrPtnProcFile()
{
	m_strDBTName = "ptn_proc_file";
	//SetInitalize();
}
//---------------------------------------------------------------------------

CDBMgrPtnProcFile::~CDBMgrPtnProcFile()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnProcFile::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [site][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnProcFile::LoadDB(TListDBPtnProcFile& tDBPtnProcFileList)
{
    UINT32 nReadCnt = 0;
    DB_PTN_PROC_FILE data;
    INT32 nIndex = 0;

	GetMDMM().strQuery.Format("SELECT id, reg_date, used_mode"
		", fe_key, file_hash"
		", file_path, file_name"
		", file_descr, file_ver, file_size"
		", file_cs_pub_name, file_cs_pub_sn, file_cs_cnt_name, file_cs_cnt_sn"
		", file_publisher, file_product"
		", os_id, build_id"
		", pe_type, pe_bit"
		", ptn_src_type, ptn_type, ptn_risk"
		" FROM ptn_proc_file;");

	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))
		return ERR_DBMS_SELECT_FAIL;

	First();
	while(Next() == 0)
	{
		nIndex = 0;

		data.nID					= GetDBField_Int(nIndex++);
		data.nRegDate				= GetDBField_Int(nIndex++);
		data.nUsedMode				= GetDBField_Int(nIndex++);

		data.strFeKey				= GetDBField_String(nIndex++);	
		data.strFileHash			= GetDBField_String(nIndex++);	

		data.strFilePath			= GetDBField_String(nIndex++);	
		data.strFileName			= GetDBField_String(nIndex++);	
		data.strFileDescr			= GetDBField_String(nIndex++);	

		data.strFileVersion			= GetDBField_String(nIndex++);	
		data.nFileSize				= GetDBField_Int(nIndex++);

		data.strPubName				= GetDBField_String(nIndex++);	
		data.strPubSN				= GetDBField_String(nIndex++);	
		data.strCntName				= GetDBField_String(nIndex++);	
		data.strCntSN				= GetDBField_String(nIndex++);	

		data.strPublisherName		= GetDBField_String(nIndex++);	
		data.strProductName			= GetDBField_String(nIndex++);	

		data.nOsID					= GetDBField_UInt64(nIndex++);	
		data.nBuildSN				= GetDBField_Int(nIndex++);
		data.nPeType				= GetDBField_Int(nIndex++);
		data.nPeBit					= GetDBField_Int(nIndex++);

		data.nPtnSrcType			= GetDBField_Int(nIndex++);
		data.nPtnType				= GetDBField_Int(nIndex++);
		data.nPtnRisk				= GetDBField_Int(nIndex++);

		data.strFilePath.ConvertWinToUnix();

		tDBPtnProcFileList.push_back(data);
		if(m_nLoadMaxID < UINT32(data.nID))	m_nLoadMaxID = data.nID;
		nReadCnt++;
	}
	m_nLoadNumber = (UINT32)tDBPtnProcFileList.size();
	WriteLogN("load database : [%s][%u]", (char *)(LPCSTR)m_strDBTName, m_nLoadNumber);

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnProcFile::InsertPtnProcFile(DB_PTN_PROC_FILE& data)
{
	GetMDMM().strQuery.Format("INSERT INTO ptn_proc_file (id, reg_date, used_mode"
		", fe_key, file_hash"
		", file_path, file_name"
		", file_descr, file_ver, file_size"
		", file_cs_pub_name, file_cs_pub_sn, file_cs_cnt_name, file_cs_cnt_sn"
		", file_publisher, file_product"
		", os_id, build_id"
		", pe_type, pe_bit"
		", ptn_src_type, ptn_type, ptn_risk"
		") VALUES (%u, %u, %u"
		", '%s', '%s'"
		", '%s', '%s'"
		", '%s', '%s', %u"
		", '%s', '%s', '%s', '%s'"
		", '%s', '%s'"
		", '%llu', %u"
		", %u, %u"
		", %u, %u, %u"
		");",
		data.nID, data.nRegDate, data.nUsedMode,
		data.strFeKey, data.strFileHash,
		data.strFilePath, data.strFileName,
		data.strFileDescr, data.strFileVersion, data.nFileSize,
		data.strPubName, data.strPubSN, data.strCntName, data.strCntSN,
		data.strPublisherName, data.strProductName,
		data.nOsID, data.nBuildSN,
		data.nPeType, data.nPeBit,
		data.nPtnSrcType, data.nPtnType, data.nPtnRisk);

	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))		return ERR_DBMS_INSERT_FAIL;

	data.nID = GetLastID();


	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnProcFile::UpdatePtnProcFile(DB_PTN_PROC_FILE& data)
{
	GetMDMM().strQuery.Format("UPDATE ptn_proc_file SET reg_date=%u, used_mode=%u"
		", file_path='%s', file_name='%s'"
		", file_descr='%s', file_ver='%s', file_size=%u"
		", file_cs_pub_name='%s', file_cs_pub_sn='%s', file_cs_cnt_name='%s', file_cs_cnt_sn='%s'"
		", file_publisher='%s', file_product='%s'"
		", os_id='%llu', build_id=%u"
		", pe_type=%u, pe_bit=%u"
		", ptn_src_type=%u, ptn_type=%u, ptn_risk=%u"
		" WHERE id=%u;",
		data.nRegDate, data.nUsedMode,
		data.strFilePath, data.strFileName,
		data.strFileDescr, data.strFileVersion, data.nFileSize,
		data.strPubName, data.strPubSN, data.strCntName, data.strCntSN,
		data.strPublisherName, data.strProductName,
		data.nOsID, data.nBuildSN,
		data.nPeType, data.nPeBit,
		data.nPtnSrcType, data.nPtnType, data.nPtnRisk,
		data.nID);

	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPtnProcFile::DeletePtnProcFile(UINT32 nID)
{
	GetMDMM().strQuery.Format("DELETE FROM ptn_proc_file WHERE id=%u;", nID);

	if(DBOP_Check(ExecuteQuery(GetMDMM().strQuery)))
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

INT32	CDBMgrPtnProcFile::LoadExecute(PVOID lpTempletList)
{
	TListDBPtnProcFile* ptDBList = (TListDBPtnProcFile*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnProcFile::SyncDBMSToDB(PVOID lpTempletList)
{
	TListDBPtnProcFile* ptDBList = (TListDBPtnProcFile*)lpTempletList;
	TListDBPtnProcFileItor begin, end;
	begin = ptDBList->begin();	end = ptDBList->end();
	for(begin; begin != end; begin++)
	{
		if(begin->nID <= m_nLoadMaxID)	continue;

		InsertExecute(&(*begin));
		m_nSyncDBNum++;
	}
	return 0;
}


INT32	CDBMgrPtnProcFile::InsertExecute(PVOID lpTemplet)
{
	PDB_PTN_PROC_FILE pd_t = (PDB_PTN_PROC_FILE)lpTemplet;

    return InsertPtnProcFile(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnProcFile::UpdateExecute(PVOID lpTemplet)
{
	PDB_PTN_PROC_FILE pd_t = (PDB_PTN_PROC_FILE)lpTemplet;

    return UpdatePtnProcFile(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPtnProcFile::DeleteExecute(UINT32 nID)
{
	return DeletePtnProcFile(nID);
}
//---------------------------------------------------------------------------






