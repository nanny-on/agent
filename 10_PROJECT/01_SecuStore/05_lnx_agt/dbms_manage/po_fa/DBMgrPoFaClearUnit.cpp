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
#include "DBMgrPoFaClearUnit.h"

//---------------------------------------------------------------------------

CDBMgrPoFaClearUnit*		t_DBMgrPoFaClearUnit = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaClearUnit::CDBMgrPoFaClearUnit()
{
	m_strDBTName = "po_fa_clear_unit";
}
//---------------------------------------------------------------------------

CDBMgrPoFaClearUnit::~CDBMgrPoFaClearUnit()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaClearUnit::SetInitalize()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaClearUnit::LoadDB(TListDBPoFaClearUnit& tDBPoFaClearUnitList)
{
	UINT32 nReadCnt = 0;
	DB_PO_FA_CLEAR_UNIT dpfcu;

	INT32 nIndex = 0;

	m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
		", file_ptn_type, file_ptn_local"
		", sch_type, sch_time, scan_type, del_cnt, limit_size, limit_cnt"
		", chk_fdt_type, chk_fdt_value"
		", ex_path, in_path"
		", exe_path, exe_command"
		", net_drive, del_method"
		", scan_dt"
		" FROM po_fa_clear_unit WHERE used_flag=1;");
	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

	do
	{
		nIndex = 0;
		DB_PO_HEADER& tDPH = dpfcu.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		dpfcu.nFilePtnType			= GetDBField_UInt(nIndex++);
		dpfcu.strFilePtnLocal		= GetDBField_String(nIndex++);
		
		dpfcu.nSchType				= GetDBField_UInt(nIndex++);
		dpfcu.nSchTime				= GetDBField_UInt(nIndex++);
		dpfcu.nScanType				= GetDBField_UInt(nIndex++);
		dpfcu.nDelCount				= GetDBField_UInt(nIndex++);
		dpfcu.nLimitSize			= GetDBField_UInt(nIndex++);
		dpfcu.nLimitCnt				= GetDBField_UInt(nIndex++);
		
		dpfcu.nChkFDTType			= GetDBField_UInt(nIndex++);
		dpfcu.nChkFDTValue			= GetDBField_UInt(nIndex++);

		dpfcu.strExPath				= GetDBField_String(nIndex++);
		dpfcu.strInPath				= GetDBField_String(nIndex++);

		dpfcu.strExePath			= GetDBField_String(nIndex++);
		dpfcu.strExeCommand			= GetDBField_String(nIndex++);

		dpfcu.nNetDriveMode			= GetDBField_UInt(nIndex++); 
		dpfcu.nDelMethod			= GetDBField_UInt(nIndex++); 

		dpfcu.nScanTime				= GetDBField_UInt(nIndex++);

		tDBPoFaClearUnitList.push_back(dpfcu);
		if(m_nLoadMaxID < UINT32(tDPH.nID))
			m_nLoadMaxID = tDPH.nID;
		nReadCnt++;
	}while(Next());
	m_nLoadNumber = tDBPoFaClearUnitList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaClearUnit::InsertPoFaClearUnit(DB_PO_FA_CLEAR_UNIT& dpfcu)
{
	DB_PO_HEADER& tDPH = dpfcu.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_fa_clear_unit("
		DBMS_POLICY_QUERY_HDR_INSERT_NAME
		", file_ptn_type, file_ptn_local"
		", sch_type, sch_time, scan_type, del_cnt, limit_size, limit_cnt"
		", chk_fdt_type, chk_fdt_value"
		", ex_path, in_path"
		", exe_path, exe_command"
		", net_drive, del_method"
		", scan_dt"
		") VALUES (%s"
		", %u, '%s'"
		", %u, %u, %u, %u, %u, %u"
		", %u, %u"
		", '%s', '%s'"
		", '%s', '%s'"
		", %u, %u"
		", %u"
		");",
		GetPoHDRQuery_InsertValue(tDPH).c_str(), 
		dpfcu.nFilePtnType, dpfcu.strFilePtnLocal.c_str(), 
		dpfcu.nSchType, dpfcu.nSchTime, dpfcu.nScanType, dpfcu.nDelCount, dpfcu.nLimitSize, dpfcu.nLimitCnt, 
		dpfcu.nChkFDTType, dpfcu.nChkFDTValue, 
		dpfcu.strExPath.c_str(), dpfcu.strInPath.c_str(), 
		dpfcu.strExePath.c_str(), dpfcu.strExeCommand.c_str(),
		dpfcu.nNetDriveMode, dpfcu.nDelMethod,
		dpfcu.nScanTime);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID = GetLastID();

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaClearUnit::UpdatePoFaClearUnit(DB_PO_FA_CLEAR_UNIT& dpfcu)
{
	DB_PO_HEADER& tDPH = dpfcu.tDPH;

	m_strQuery = SPrintf("UPDATE po_fa_clear_unit SET %s"
		", file_ptn_type=%u, file_ptn_local='%s'"
		", sch_type=%u, sch_time=%u, scan_type=%u, del_cnt=%u, limit_size=%u, limit_cnt=%u"
		", chk_fdt_type=%u, chk_fdt_value=%u"
		", ex_path='%s', in_path='%s'"
		", exe_path='%s', exe_command='%s'"
		", net_drive=%u, del_method=%u"
		", scan_dt=%u "
		"WHERE id=%u;",
		GetPoHDRQuery_Update(tDPH).c_str(),
		dpfcu.nFilePtnType, dpfcu.strFilePtnLocal.c_str(), 
		dpfcu.nSchType, dpfcu.nSchTime, dpfcu.nScanType, dpfcu.nDelCount, dpfcu.nLimitSize,  dpfcu.nLimitCnt,
		dpfcu.nChkFDTType, dpfcu.nChkFDTValue, 
		dpfcu.strExPath.c_str(), dpfcu.strInPath.c_str(),
		dpfcu.strExePath.c_str(), dpfcu.strExeCommand.c_str(),
		dpfcu.nNetDriveMode, dpfcu.nDelMethod,
		dpfcu.nScanTime,
		tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaClearUnit::DeletePoFaClearUnit(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

	return 0;
}

INT32			CDBMgrPoFaClearUnit::ClearPoFaClearUnit()
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_CLEAR, m_strDBTName.c_str());

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

INT32	CDBMgrPoFaClearUnit::LoadExecute(PVOID lpTempletList)
{
	TListDBPoFaClearUnit* ptDBList = (TListDBPoFaClearUnit*)lpTempletList;

	return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaClearUnit::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_FA_CLEAR_UNIT pd_t = (PDB_PO_FA_CLEAR_UNIT)lpTemplet;

	return InsertPoFaClearUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaClearUnit::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_FA_CLEAR_UNIT pd_t = (PDB_PO_FA_CLEAR_UNIT)lpTemplet;

	return UpdatePoFaClearUnit(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaClearUnit::DeleteExecute(UINT32 nID)
{
	return DeletePoFaClearUnit(nID);
}

INT32	CDBMgrPoFaClearUnit::ClearExecute()
{
	return ClearPoFaClearUnit();
}

//---------------------------------------------------------------------------






