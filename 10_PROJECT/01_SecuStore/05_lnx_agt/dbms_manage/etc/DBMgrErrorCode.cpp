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
#include "DBMgrErrorCode.h"

//---------------------------------------------------------------------------

CDBMgrErrorCode*		t_DBMgrErrorCode;

//---------------------------------------------------------------------------

CDBMgrErrorCode::CDBMgrErrorCode()
{
	m_strDBTName = "error_code";
}
//---------------------------------------------------------------------------

CDBMgrErrorCode::~CDBMgrErrorCode()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrErrorCode::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrErrorCode::LoadDB(TListDBErrorCode& tDBErrorCodeList)
{
    UINT32 nReadCnt = 0;
    DB_ERROR_CODE dec;

    INT32 nIndex = 0;

    m_strQuery = SPrintf("SELECT id, type_kor, descr_kor, type_eng, descr_eng FROM error_code;") ;
    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {    	
    	nIndex = 0;

        dec.nID							= GetDBField_UInt(nIndex++);
        dec.lan_kor.strType				= GetDBField_String(nIndex++);
        dec.lan_kor.strDescr			= GetDBField_String(nIndex++);
        dec.lan_eng.strType				= GetDBField_String(nIndex++);
        dec.lan_eng.strDescr			= GetDBField_String(nIndex++);
        
        tDBErrorCodeList.push_back(dec);
        if(m_nLoadMaxID < UINT32(dec.nID))
			m_nLoadMaxID = dec.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBErrorCodeList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//--------------------------------------------------------------------------- 

INT32			CDBMgrErrorCode::InsertErrorCode(DB_ERROR_CODE& dec)
{
    m_strQuery = SPrintf("INSERT INTO error_code (id, type_kor, descr_kor, type_eng, descr_eng) VALUES "
    								"(%u, '%s', '%s', '%s', '%s');",
                                    dec.nID,
                                    dec.lan_kor.strType.c_str(), dec.lan_kor.strDescr.c_str(),
                                    dec.lan_eng.strType.c_str(), dec.lan_eng.strDescr.c_str());

    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
    
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

INT32	CDBMgrErrorCode::LoadExecute(PVOID lpTempletList)
{
	TListDBErrorCode* ptDBList = (TListDBErrorCode*)lpTempletList;

    return LoadDB(*ptDBList);
}
//--------------------------------------------------------------------------- 

INT32	CDBMgrErrorCode::InsertExecute(PVOID lpTemplet)
{
	PDB_ERROR_CODE pd_t = (PDB_ERROR_CODE)lpTemplet;

    return InsertErrorCode(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrErrorCode::GetCountExecute()
{
	TListDBErrorCode tTempList;

    return LoadDB(tTempList);
}
//---------------------------------------------------------------------------







