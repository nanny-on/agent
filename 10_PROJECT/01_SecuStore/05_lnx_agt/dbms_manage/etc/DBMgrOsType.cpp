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
#include "DBMgrOsType.h"

//---------------------------------------------------------------------------

CDBMgrOsType*		t_DBMgrOsType;

//---------------------------------------------------------------------------

CDBMgrOsType::CDBMgrOsType()
{
	m_strDBTName = "os_type";
}
//---------------------------------------------------------------------------

CDBMgrOsType::~CDBMgrOsType()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrOsType::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrOsType::LoadDB(TListDBOsType& tDBOsTypeList)
{
    UINT32 nReadCnt = 0;
    DB_OS_TYPE dot;

    INT32 nIndex = 0;

    m_strQuery = SPrintf("SELECT id, name, type FROM os_type;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {
    	nIndex = 0;
    	
        dot.nID				= GetDBField_Int(nIndex++);
        dot.strName			= GetDBField_String(nIndex++);
        dot.nType			= GetDBField_Int(nIndex++);

        tDBOsTypeList.push_back(dot);
        if(m_nLoadMaxID < UINT32(dot.nID))
			m_nLoadMaxID = dot.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBOsTypeList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrOsType::InsertOsType(DB_OS_TYPE& dot)
{
    m_strQuery = SPrintf("INSERT INTO os_type (id, name, type) VALUES (%u, '%s', %u);",
    								dot.nID, dot.strName.c_str(), dot.nType);

    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrOsType::UpdateOsType(DB_OS_TYPE& dot)
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrOsType::DeleteOsType(UINT32 nID)
{
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

INT32	CDBMgrOsType::LoadExecute(PVOID lpTempletList)
{
	TListDBOsType* ptDBList = (TListDBOsType*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------  

INT32	CDBMgrOsType::InsertExecute(PVOID lpTemplet)
{
	PDB_OS_TYPE pd_t = (PDB_OS_TYPE)lpTemplet;

    return InsertOsType(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrOsType::UpdateExecute(PVOID lpTemplet)
{
	PDB_OS_TYPE pd_t = (PDB_OS_TYPE)lpTemplet;

    return UpdateOsType(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrOsType::DeleteExecute(UINT32 nID)
{
	return DeleteOsType(nID);
}
//---------------------------------------------------------------------------






