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
#include "ManageSiteFileAlias.h"

//---------------------------------------------------------------------------

CManageSiteFileAlias*	t_ManageSiteFileAlias = NULL;

//---------------------------------------------------------------------------

CManageSiteFileAlias::CManageSiteFileAlias()
{
	t_DBMgrSiteFileAlias	= new CDBMgrSiteFileAlias();
}
//---------------------------------------------------------------------------

CManageSiteFileAlias::~CManageSiteFileAlias()
{
	if(t_DBMgrSiteFileAlias)		{	delete t_DBMgrSiteFileAlias;		t_DBMgrSiteFileAlias = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageSiteFileAlias::LoadDBMS()
{
	TListDBSiteFileAlias tDBSiteFileAliasList;
    TListDBSiteFileAliasItor begin, end;
	if(SetER(t_DBMgrSiteFileAlias->LoadExecute(&tDBSiteFileAliasList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBSiteFileAliasList.begin();	end = tDBSiteFileAliasList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
		
		{
			String strKey;
			strKey = SPrintf("%s_%s_%s", begin->strFeKey.c_str(), begin->strFilePath.c_str(), begin->strFileName.c_str());

			AddPosSKeyID(strKey, begin->nID, SS_SITE_FILE_ALIAS_KEY_TYPE_FE_PATH_NAME);
		}
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageSiteFileAlias::AddSiteFileAlias(DB_SITE_FILE_ALIAS&	data)
{
	if(SetER(t_DBMgrSiteFileAlias->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    

	{
		String strKey;
		strKey = SPrintf("%s_%s_%s", data.strFeKey.c_str(), data.strFilePath.c_str(), data.strFileName.c_str());

		AddPosSKeyID(strKey, data.nID, SS_SITE_FILE_ALIAS_KEY_TYPE_FE_PATH_NAME);
	}
	
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageSiteFileAlias::EditSiteFileAlias(DB_SITE_FILE_ALIAS&	data)
{
    if(t_DBMgrSiteFileAlias->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageSiteFileAlias::DelSiteFileAlias(UINT32 nID)
{
	PDB_SITE_FILE_ALIAS pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	{
		String strKey;
		strKey = SPrintf("%s_%s_%s", pdata->strFeKey.c_str(), pdata->strFilePath.c_str(), pdata->strFileName.c_str());

		DelPosSKeyID(strKey, SS_SITE_FILE_ALIAS_KEY_TYPE_FE_PATH_NAME);
	}

	if(SetER(t_DBMgrSiteFileAlias->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }

	DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

PDB_SITE_FILE_ALIAS			CManageSiteFileAlias::FindSiteFileAliasKey(DB_SITE_FILE_ALIAS& data)
{	
	String strKey;
	strKey = SPrintf("%s_%s_%s", data.strFeKey.c_str(), data.strFilePath.c_str(), data.strFileName.c_str());

	return FindPosSKeyID_Item(strKey, SS_SITE_FILE_ALIAS_KEY_TYPE_FE_PATH_NAME);
}
//---------------------------------------------------------------------------

INT32					CManageSiteFileAlias::ApplySiteFileAlias(DB_SITE_FILE_ALIAS& data)
{
	if(data.nID != 0 && FindItem(data.nID))
	{
		return EditSiteFileAlias(data);
	}
	return AddSiteFileAlias(data);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

INT32	CManageSiteFileAlias::GetPkt(MemToken& RecvToken, DB_SITE_FILE_ALIAS& data)
{
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedFlag))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nRegSvrID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSyncSvrStep))				goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strFeKey) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strFilePath) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strFileName) < 0)		goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32	CManageSiteFileAlias::GetPktSync(MemToken& RecvToken, DB_SITE_FILE_ALIAS& data)
{
	if (!RecvToken.TokenDel_32(data.nRegSvrID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSyncSvrStep))				goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strFeKey) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strFilePath) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strFileName) < 0)		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

INT32					CManageSiteFileAlias::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());
	{
		TMapDBSiteFileAliasItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			SetPkt(&(begin->second), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32			CManageSiteFileAlias::SetPktSync(TListPVOID& tIDList)
{
	{
		TMapDBSiteFileAliasItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(!(begin->second.nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO))			continue;

			tIDList.push_back(&(begin->second));
		}

		if(tIDList.empty())	return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32			CManageSiteFileAlias::SetPktSync(MemToken& SendToken)
{
	TListPVOID tSendList;
	{
		SetPktSync(tSendList);

		if(tSendList.empty())	return -1;
	}

	SendToken.TokenAdd_32(tSendList.size());
	{
		TListPVOIDItor begin, end;
		begin = tSendList.begin();	end = tSendList.end();
		for(begin; begin != end; begin++)
		{
			SetPkt((PDB_SITE_FILE_ALIAS)(*begin), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageSiteFileAlias::SetPkt(TListID& tIDList, MemToken& SendToken)
{
	SendToken.TokenAdd_32(tIDList.size());
	{
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			SetPkt(*begin, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageSiteFileAlias::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_SITE_FILE_ALIAS pdata = FindItem(nID);
	if(!pdata)	return 0;

	SetPkt(pdata, SendToken);
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageSiteFileAlias::SetPkt(PDB_SITE_FILE_ALIAS pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedFlag);
	SendToken.TokenAdd_32(pdata->nRegDate);
	SendToken.TokenAdd_32(pdata->nUsedMode);

	SendToken.TokenAdd_32(pdata->nRegSvrID);
	SendToken.TokenAdd_32(pdata->nSyncSvrStep);

	SendToken.TokenAdd_String(pdata->strFeKey);
	SendToken.TokenAdd_String(pdata->strFilePath);
	SendToken.TokenAdd_String(pdata->strFileName);

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

void 		CManageSiteFileAlias::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][site_file_alias] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





