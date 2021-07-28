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
#include "DBMgrPoInRsBk.h"

//---------------------------------------------------------------------------

CDBMgrPoInRsBk*		t_DBMgrPoInRsBk = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInRsBk::CDBMgrPoInRsBk()
{
	m_strDBTName = "po_in_rs_bk";
}
//---------------------------------------------------------------------------

CDBMgrPoInRsBk::~CDBMgrPoInRsBk()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsBk::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_rs_bk][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsBk::LoadDB(TListDBPoInRsBk& tDBPoInRsBkList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_RS_BK data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_HDR_SELECT
						", keep_size, free_size"
						", file_max_size, file_keep_num"
						", fext_type, fext_info"
						" FROM po_in_rs_bk WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    
    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoHDR(nIndex);

		data.nKeepSize				= GetDBField_UInt(nIndex++);
		data.nFreeSize				= GetDBField_UInt(nIndex++);
		data.nFileMaxSize			= GetDBField_UInt(nIndex++);
		data.nFileKeepNum			= GetDBField_UInt(nIndex++);

		data.nFExtType				= GetDBField_UInt(nIndex++);
		data.strFExtInfo			= GetDBField_String(nIndex++);

        tDBPoInRsBkList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInRsBkList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsBk::InsertPoInRsBk(DB_PO_IN_RS_BK& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_rs_bk("
						DBMS_POLICY_QUERY_HDR_INSERT_NAME
						", keep_size, free_size"
						", file_max_size, file_keep_num"
						", fext_type, fext_info"
						") VALUES (%s "
                        ", %u, %u"
						", %u, %u"
						", %u, '%s'"
						");",
                        GetPoHDRQuery_InsertValue(tDPH).c_str(), 
						data.nKeepSize, data.nFreeSize,
						data.nFileMaxSize, data.nFileKeepNum,
						data.nFExtType, data.strFExtInfo.c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsBk::UpdatePoInRsBk(DB_PO_IN_RS_BK& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_rs_bk SET %s"
						", keep_size=%u, free_size=%u"
						", file_max_size=%u, file_keep_num=%u"
						", fext_type=%u, fext_info='%s'"
						" WHERE id=%u;",
						GetPoHDRQuery_Update(tDPH).c_str(),
						data.nKeepSize, data.nFreeSize,
						data.nFileMaxSize, data.nFileKeepNum, 
						data.nFExtType, data.strFExtInfo.c_str(),
                        tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInRsBk::DeletePoInRsBk(UINT32 nID)
{
    m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);
    
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

INT32	CDBMgrPoInRsBk::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInRsBk* ptDBList = (TListDBPoInRsBk*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsBk::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_BK pd_t = (PDB_PO_IN_RS_BK)lpTemplet;

    return InsertPoInRsBk(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsBk::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_RS_BK pd_t = (PDB_PO_IN_RS_BK)lpTemplet;

    return UpdatePoInRsBk(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInRsBk::DeleteExecute(UINT32 nID)
{
	return DeletePoInRsBk(nID);
}
//---------------------------------------------------------------------------






