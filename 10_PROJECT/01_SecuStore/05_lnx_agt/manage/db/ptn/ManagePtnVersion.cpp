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
#include "ManagePtnVersion.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

CManagePtnVersion::CManagePtnVersion()
{
	m_tDBMgrPtnVersion	= new CDBMgrPtnVersion();
}
//---------------------------------------------------------------------------

CManagePtnVersion::~CManagePtnVersion()
{
	if(m_tDBMgrPtnVersion)		{	delete m_tDBMgrPtnVersion;		m_tDBMgrPtnVersion = NULL;	}
}
//---------------------------------------------------------------------------

void		CManagePtnVersion::SetDBFQUtil(CASIDBFQDLLUtil*	tASIDBFQDLLUtil)
{
	m_tDBMgrPtnVersion->SetDBFQUtil(tASIDBFQDLLUtil);
}

INT32		CManagePtnVersion::LoadDBMS()
{
	TListDBPtnVersion tDBPtnVersionList;
    TListDBPtnVersionItor begin, end;
	if(SetER(m_tDBMgrPtnVersion->LoadExecute(&tDBPtnVersionList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnVersionList.begin();	end = tDBPtnVersionList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVersion::AddPtnVersion(DB_PTN_VERSION&	data)
{
	if(SetER(m_tDBMgrPtnVersion->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    	
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVersion::EditPtnVersion(DB_PTN_VERSION&	data)
{
    if(m_tDBMgrPtnVersion->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVersion::DelPtnVersion(UINT32 nID)
{
	PDB_PTN_VERSION pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnVersion->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

String			CManagePtnVersion::GetDefaultVersion()
{
	PDB_PTN_VERSION pdata = FirstItem();
	if(!pdata)	return "";

	return pdata->strPtnVersion;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManagePtnVersion::GetPkt(MemToken& RecvToken, DB_PTN_VERSION& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strPtnVersion) < 0)		goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnVersion::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnVersionItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_VERSION pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnVersion::SetPkt(PDB_PTN_VERSION pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_String(pdata->strPtnVersion);

	SendToken.TokenSet_Block();
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
//--------------------------------------------------------------------------- 

void 		CManagePtnVersion::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_version] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





