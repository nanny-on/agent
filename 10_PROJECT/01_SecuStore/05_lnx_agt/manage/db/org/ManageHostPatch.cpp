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
#include "ManageHostPatch.h"

//---------------------------------------------------------------------------

CManageHostPatch*	t_ManageHostPatch = NULL;

//---------------------------------------------------------------------------

CManageHostPatch::CManageHostPatch()
{
	t_DBMgrHostPatch	= new CDBMgrHostPatch();
}
//---------------------------------------------------------------------------

CManageHostPatch::~CManageHostPatch()
{
	if(t_DBMgrHostPatch)		{	delete t_DBMgrHostPatch;		t_DBMgrHostPatch = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageHostPatch::LoadDBMS()
{
	TListDBHostPatch tDBHostPatchList;
    TListDBHostPatchItor begin, end;
	if(SetER(t_DBMgrHostPatch->LoadExecute(&tDBHostPatchList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBHostPatchList.begin();	end = tDBHostPatchList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
		AddSKeyID(GetKey(&(*begin)), begin->nID);
		AddKeyIDList(begin->nKBID, begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostPatch::AddHostPatch(DB_HOST_PATCH&	data)
{
	if(SetER(t_DBMgrHostPatch->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    
	AddSKeyID(GetKey(&data), data.nID);
	AddKeyIDList(data.nKBID, data.nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostPatch::EditHostPatch(DB_HOST_PATCH&	data)
{
    if(t_DBMgrHostPatch->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostPatch::DelHostPatch(UINT32 nID)
{
	PDB_HOST_PATCH pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrHostPatch->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDList(pdata->nKBID, pdata->nID);
	DelSKeyID(GetKey(pdata));
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

PDB_HOST_PATCH		CManageHostPatch::FindChkKey(PDB_HOST_PATCH pdata)
{
	PDB_HOST_PATCH pdata_find = FindSKeyID_Item(GetKey(pdata));
	return pdata_find;
}
//---------------------------------------------------------------------------

String				CManageHostPatch::GetKey(PDB_HOST_PATCH pdata)
{
	String strKey;
	strKey = SPrintf("%u_%s_%s_%s_%s", pdata->nKBID, pdata->strCategory.c_str(), pdata->strName.c_str(), pdata->strPublisher.c_str(), pdata->strGuid.c_str());
	return strKey;
}
//---------------------------------------------------------------------------

INT32			CManageHostPatch::IsExistLocalPatch(TListDBHostPatch* tPatchInfoList, PDB_HOST_PATCH pdata)
{
	String strTarKey = GetKey(pdata);

	TListDBHostPatchItor begin, end;
	begin = tPatchInfoList->begin();	end = tPatchInfoList->end();
	for(begin; begin != end; begin++)
	{
		String strSrcKey = GetKey(&(*begin));
		
		if(strTarKey == strSrcKey)		return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32	CManageHostPatch::GetPkt(MemToken& RecvToken, DB_HOST_PATCH& data)
{
	if (!RecvToken.TokenDel_32(data.nID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedFlag))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nHostID))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nFindType))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nKBID))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strCategory) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strPublisher) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strInsDate) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strGuid) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nNoRemove))				goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strUninstall) < 0)	goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block(); 
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------

INT32		CManageHostPatch::SetPkt(MemToken& SendToken)
{
	TListID tIDList;
	GetItemIDList(tIDList);
	
	SendToken.TokenAdd_32(tIDList.size());
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_HOST_PATCH pdata = FindItem(*begin);
		if(!pdata)  	continue;

		SetPkt(pdata, SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageHostPatch::SetPkt(TListDBHostPatch& tDBHostPatchList, MemToken& SendToken)
{
	SendToken.TokenAdd_32(tDBHostPatchList.size());
	TListDBHostPatchItor begin, end;
	begin = tDBHostPatchList.begin();	end = tDBHostPatchList.end();
	for(begin; begin != end; begin++)
	{
		PDB_HOST_PATCH pdata = &(*begin);
		if(!pdata)  	continue;

		SetPkt(pdata, SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageHostPatch::SetPkt(PDB_HOST_PATCH pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedFlag);
	SendToken.TokenAdd_32(pdata->nRegDate);
	SendToken.TokenAdd_32(pdata->nHostID);

	SendToken.TokenAdd_32(pdata->nFindType);

	SendToken.TokenAdd_32(pdata->nKBID);
	SendToken.TokenAdd_String(pdata->strCategory);
	SendToken.TokenAdd_String(pdata->strName);
	SendToken.TokenAdd_String(pdata->strPublisher);
	SendToken.TokenAdd_String(pdata->strInsDate);
	SendToken.TokenAdd_String(pdata->strGuid);
	SendToken.TokenAdd_32(pdata->nNoRemove);
	SendToken.TokenAdd_String(pdata->strUninstall);

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

void 		CManageHostPatch::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][host_patch] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





