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
#include "ManageHost.h"

//---------------------------------------------------------------------------

CManageHost*	t_ManageHost = NULL;

//---------------------------------------------------------------------------

CManageHost::CManageHost()
{
	t_DBMgrHost	= new CDBMgrHost();
}
//---------------------------------------------------------------------------

CManageHost::~CManageHost()
{
	if(t_DBMgrHost)		{	delete t_DBMgrHost;		t_DBMgrHost = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageHost::LoadDBMS()
{
	DB_HOST dh;
	TListDBHost tDBHostList;
    TListDBHostItor begin, end;
	
	if(SetER(t_DBMgrHost->LoadExecute(&tDBHostList)))
    {
 		if(tDBHostList.size() == 0)
		{   
			memset(&dh, 0, sizeof(DB_HOST));
			AddHost(dh);
			return 0;
 		}
    	return g_nErrRtn;
    }

    begin = tDBHostList.begin();	end = tDBHostList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);		
    }

	if(tDBHostList.size() == 0)
	{
		memset(&dh, 0, sizeof(DB_HOST));
		AddHost(dh);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHost::AddHost(DB_HOST&	dh)
{
	if(SetER(t_DBMgrHost->InsertExecute(&dh)))
    {
    	return g_nErrRtn;
    }

    AddItem(dh.nID, dh);     	
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHost::EditHost(DB_HOST&	dh)
{
    if(t_DBMgrHost->UpdateExecute(&dh))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(dh.nID, dh);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHost::EditHost_UsedFlag(UINT32 nID, UINT32 nUsedFlag)
{
    PDB_HOST pdh = FindItem(nID);
    if(!pdh)	return ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND;
    pdh->nUsedFlag		= nUsedFlag;

	if(t_DBMgrHost->UpdateExecute(pdh))			return ERR_DBMS_UPDATE_FAIL;
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHost::DelHost(UINT32 nID)
{
	PDB_HOST pdh = FindItem(nID);
	if(!pdh)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;
    
	if(SetER(t_DBMgrHost->DeleteExecute(pdh->nID)))
    {
    	return g_nErrRtn;
    }                                                
	DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageHost::GetUserID(UINT32 nHID)
{
	PDB_HOST pdh = FindItem(nHID);
	if(!pdh)	return 0;

	return pdh->nUserID;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManageHost::GetPktHost(MemToken& RecvToken, DB_HOST& dh)
{
	if (!RecvToken.TokenDel_32(dh.nID))							goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dh.nUsedFlag))					goto INVALID_PKT;
	
	if ( RecvToken.TokenDel_String(dh.strName) < 0)				goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dh.strNameAlias) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dh.strWorkGroup) < 0)		goto INVALID_PKT;
	
	if (!RecvToken.TokenDel_32(dh.nConDate))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dh.nDConDate))					goto INVALID_PKT;
	
	if ( RecvToken.TokenDel_String(dh.strPriIP) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dh.strNatIP) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dh.strMac) < 0)				goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dh.strGUID) < 0)				goto INVALID_PKT;

	if ( RecvToken.TokenDel_IDMap(dh.tPoIDMap) < 0)				goto INVALID_PKT;
	if ( RecvToken.TokenDel_IDMap(dh.tPoSeqNoMap) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_IDMap(dh.tPoTimeMap) < 0)			goto INVALID_PKT;
	
	if (!RecvToken.TokenDel_32(dh.nUserID))						goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dh.strUserUnicode) < 0)		goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManageHost::SetPktHost(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBHostItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_HOST pdh = &(begin->second);
		if(!pdh)  	continue;

		SetPktHost(pdh, SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageHost::SetPktHost(PDB_HOST pdh, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdh->nID);
	SendToken.TokenAdd_32(pdh->nUsedFlag);
    
	SendToken.TokenAdd_String(pdh->strName);
	SendToken.TokenAdd_String(pdh->strNameAlias);
	SendToken.TokenAdd_String(pdh->strWorkGroup);
	
	SendToken.TokenAdd_32(pdh->nConDate);
	SendToken.TokenAdd_32(pdh->nDConDate);

	SendToken.TokenAdd_String(pdh->strPriIP);
	SendToken.TokenAdd_String(pdh->strNatIP);
	SendToken.TokenAdd_String(pdh->strMac);
	SendToken.TokenAdd_String(pdh->strGUID);

	SendToken.TokenAdd_IDMap(pdh->tPoIDMap);
	SendToken.TokenAdd_IDMap(pdh->tPoSeqNoMap);
	SendToken.TokenAdd_IDMap(pdh->tPoTimeMap);

	SendToken.TokenAdd_32(pdh->nUserID);
	SendToken.TokenAdd_String(pdh->strUserUnicode);

	SendToken.TokenAdd_32(pdh->nConnected);
	
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





