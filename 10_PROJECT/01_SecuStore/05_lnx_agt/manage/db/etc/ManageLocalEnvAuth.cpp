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
#include "ManageLocalEnvAuth.h"

//---------------------------------------------------------------------------

CManageLocalEnvAuth*	t_ManageLocalEnvAuth = NULL;

//---------------------------------------------------------------------------

CManageLocalEnvAuth::CManageLocalEnvAuth()
{
	t_DBMgrLocalEnvAuth	= new CDBMgrLocalEnvAuth();	
}
//---------------------------------------------------------------------------

CManageLocalEnvAuth::~CManageLocalEnvAuth()
{
	if(t_DBMgrLocalEnvAuth)	{	delete t_DBMgrLocalEnvAuth;	t_DBMgrLocalEnvAuth = NULL;	}	
}
//---------------------------------------------------------------------------

INT32		CManageLocalEnvAuth::LoadDBMS()
{
	DB_LOCAL_ENV_AUTH dlea;
	TListDBLocalEnvAuth tDBLocalEnvAuthList;
	TListDBLocalEnvAuthItor begin, end;
	
	if(SetER(t_DBMgrLocalEnvAuth->LoadExecute(&tDBLocalEnvAuthList)))
	{
		if(tDBLocalEnvAuthList.size() == 0)
		{
			memset(&dlea, 0, sizeof(DB_LOCAL_ENV_AUTH));
			AddLocalEnvAuth(dlea);
		}	
		return g_nErrRtn;
	}

	begin = tDBLocalEnvAuthList.begin();	end = tDBLocalEnvAuthList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->nID, *begin);
	}

	if(tDBLocalEnvAuthList.size() == 0)
	{
		memset(&dlea, 0, sizeof(DB_LOCAL_ENV_AUTH));
		AddLocalEnvAuth(dlea);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvAuth::InitHash()
{
	return 0;
}
//---------------------------------------------------------------------------

String					CManageLocalEnvAuth::GetHash()
{
	return "";
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvAuth::AddLocalEnvAuth(DB_LOCAL_ENV_AUTH&	dlea)
{	
	if(SetER(t_DBMgrLocalEnvAuth->InsertExecute(&dlea)))
	{
		return g_nErrRtn;
	}

	AddItem(dlea.nID, dlea);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvAuth::EditLocalEnvAuth(DB_LOCAL_ENV_AUTH&	dlea)
{
	PDB_LOCAL_ENV_AUTH pdlea = FindItem(dlea.nID);
	if(!pdlea)	return ERR_DBMS_UPDATE_FAIL;	

	if(SetER(t_DBMgrLocalEnvAuth->UpdateExecute(&dlea)))
	{
		return g_nErrRtn;
	}

	EditItem(dlea.nID, dlea);
	InitHash();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvAuth::DelLocalEnvAuth(UINT32 nID)
{
	PDB_LOCAL_ENV_AUTH pdlea = FindItem(nID);
	if(!pdlea)	return ERR_DBMS_DELETE_FAIL;


	if(SetER(t_DBMgrLocalEnvAuth->DeleteExecute(pdlea->nID)))
	{
		return g_nErrRtn;
	}
	
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvAuth::ApplyLocalEnvAuth(DB_LOCAL_ENV_AUTH&	dlea)
{
	if(FindItem(dlea.nID))
	{
		return EditLocalEnvAuth(dlea);
	}
	return AddLocalEnvAuth(dlea);
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

INT32					CManageLocalEnvAuth::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBLocalEnvAuthItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvAuth::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_LOCAL_ENV_AUTH pdlea = FindItem(nID);
	if(!pdlea)	return -1;

	return SetPkt(pdlea, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvAuth::SetPkt(PDB_LOCAL_ENV_AUTH pdlea, MemToken& SendToken)
{

	SendToken.TokenAdd_32(pdlea->nID);
	SendToken.TokenAdd_32(pdlea->nUsedFlag);
	SendToken.TokenAdd_32(pdlea->nEvtTime);

	SendToken.TokenAdd_String(pdlea->strUniCode);
	SendToken.TokenAdd_String(pdlea->strAccountID);
	SendToken.TokenAdd_String(pdlea->strAccountPW);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLocalEnvAuth::GetPkt(MemToken& RecvToken, DB_LOCAL_ENV_AUTH& dlea)
{
	if (!RecvToken.TokenDel_32(dlea.nID))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dlea.nUsedFlag))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dlea.nEvtTime))					goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dlea.strUniCode) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dlea.strAccountID) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dlea.strAccountPW) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



