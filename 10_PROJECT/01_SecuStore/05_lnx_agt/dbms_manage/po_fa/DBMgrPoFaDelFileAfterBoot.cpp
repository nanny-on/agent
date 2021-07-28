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
#include "DBMgrPoFaDelFileAfterBoot.h"

//---------------------------------------------------------------------------

CDBMgrPoFaDelFileAfterBoot*		t_DBMgrPoFaDelFileAfterBoot = NULL;

//---------------------------------------------------------------------------

CDBMgrPoFaDelFileAfterBoot::CDBMgrPoFaDelFileAfterBoot()
{
	m_strDBTName = "po_fa_del_file_on_reboot";
}
//---------------------------------------------------------------------------

CDBMgrPoFaDelFileAfterBoot::~CDBMgrPoFaDelFileAfterBoot()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaDelFileAfterBoot::SetInitalize()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoFaDelFileAfterBoot::LoadDB(TListDelFileAfterBoot& tDBDelFileAfterBootList)
{
    UINT32 nReadCnt = 0;
    DB_DEL_FILE_AFTER_BOOT DelFileInfo;

    INT32 nIndex = 0;

    m_strQuery = SPrintf("SELECT id, del_file_path FROM po_fa_del_file_on_reboot;") ;

    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_SELECT_FAIL;

    do
    {    	
    	nIndex = 0;

        DelFileInfo.nID					= GetDBField_UInt(nIndex++);
        DelFileInfo.strDelFilePath		= GetDBField_String(nIndex++);
        
        tDBDelFileAfterBootList.push_back(DelFileInfo);
        if(m_nLoadMaxID < UINT32(DelFileInfo.nID))
			m_nLoadMaxID = DelFileInfo.nID;
        nReadCnt++;
    }while(Next());
		
	m_nLoadNumber = (UINT32)tDBDelFileAfterBootList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//--------------------------------------------------------------------------- 

INT32			CDBMgrPoFaDelFileAfterBoot::InsertDelFileInfo(LPCSTR pDelFilePath)
{
	INT32 nRetVal = 0;

	if(pDelFilePath == NULL || pDelFilePath[0] == 0)
		return -1;

	nRetVal = IsDelFileInfo(pDelFilePath);
	if(nRetVal == 0)
		return 0;

    m_strQuery = SPrintf("INSERT INTO po_fa_del_file_on_reboot (del_file_path) VALUES ('%s');", pDelFilePath);

    if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_INSERT_FAIL;
    
    return 0;
}

INT32			CDBMgrPoFaDelFileAfterBoot::IsDelFileInfo(LPCSTR pDelFilePath)
{
	if(pDelFilePath == NULL || pDelFilePath[0] == 0)
		return -1;

	m_strQuery = SPrintf("SELECT del_file_path FROM po_fa_del_file_on_reboot where del_file_path='%s';", pDelFilePath) ;

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return -1;

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

INT32	CDBMgrPoFaDelFileAfterBoot::LoadExecute(PVOID lpTempletList)
{
	TListDelFileAfterBoot* ptDBList = (TListDelFileAfterBoot*)lpTempletList;
	if(ptDBList == NULL)
		return -1;
    return LoadDB(*ptDBList);
}
//--------------------------------------------------------------------------- 

INT32	CDBMgrPoFaDelFileAfterBoot::InsertExecute(PVOID lpTemplet)
{
	PDB_DEL_FILE_AFTER_BOOT pd_t = (PDB_DEL_FILE_AFTER_BOOT)lpTemplet;
	if(pd_t == NULL)
		return -1;
    return InsertDelFileInfo(pd_t->strDelFilePath.c_str());
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoFaDelFileAfterBoot::GetCountExecute()
{
	TListDelFileAfterBoot tDBDelFileAfterBootList;

    return LoadDB(tDBDelFileAfterBootList);
}

INT32	CDBMgrPoFaDelFileAfterBoot::DeletePoFaDelFileAfterBoot(UINT32 nID)
{
	m_strQuery = SPrintf(DBMS_DEFAULT_QUERY_TYPE_DELETE, m_strDBTName.c_str(), nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))
		return ERR_DBMS_DELETE_FAIL;

	return 0;
}

INT32	CDBMgrPoFaDelFileAfterBoot::DeleteExecute(UINT32 nID)
{
	return DeletePoFaDelFileAfterBoot(nID);
}
//---------------------------------------------------------------------------







