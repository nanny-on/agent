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
#include "ManageEnvSOrgLink.h"

//---------------------------------------------------------------------------

CManageEnvSOrgLink*	t_ManageEnvSOrgLink = NULL;

//---------------------------------------------------------------------------

CManageEnvSOrgLink::CManageEnvSOrgLink()
{
	t_DBMgrEnvSOrgLink	= new CDBMgrEnvSOrgLink();
}
//---------------------------------------------------------------------------

CManageEnvSOrgLink::~CManageEnvSOrgLink()
{
	if(t_DBMgrEnvSOrgLink)	{	delete t_DBMgrEnvSOrgLink;	t_DBMgrEnvSOrgLink = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageEnvSOrgLink::LoadDBMS()
{
	DB_ENV_SORG_LINK desl;
	TListDBEnvSOrgLink tDBEnvSOrgLinkList;
    TListDBEnvSOrgLinkItor begin, end;
	
	if(SetER(t_DBMgrEnvSOrgLink->LoadExecute(&tDBEnvSOrgLinkList)))
    {
		if(tDBEnvSOrgLinkList.size() == 0)
		{
			memset(&desl, 0, sizeof(DB_ENV_SORG_LINK));
			desl.tDPH.nID = 1;
			AddEnvSOrgLink(desl);
		}    
    	return g_nErrRtn;
    }

    begin = tDBEnvSOrgLinkList.begin();	end = tDBEnvSOrgLinkList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
	}

	if(tDBEnvSOrgLinkList.size() == 0)
	{
		memset(&desl, 0, sizeof(DB_ENV_SORG_LINK));
		desl.tDPH.nID = 1;
		AddEnvSOrgLink(desl);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSOrgLink::InitHash()
{
	String strOrgValue;

	PDB_ENV_SORG_LINK pdesl 			= NULL;
	{
		if( (pdesl = FirstItem()) == NULL)
		{
//			WriteLogE("not find env_sorg_link by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,%u,%s,", 
							GetHdrHashInfo(pdesl).c_str(),
							pdesl->nUserCatchType, pdesl->strUserCatchValue.c_str());
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdesl->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManageEnvSOrgLink::GetHash()
{
	PDB_ENV_SORG_LINK pdesl 			= NULL;
	{
		if( (pdesl = FirstItem()) == NULL)
		{
//			WriteLogE("not find env_sorg_link by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return "";
		}
	}

	return pdesl->tDPH.strHash;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSOrgLink::AddEnvSOrgLink(DB_ENV_SORG_LINK&	desl)
{
	if(SetER(t_DBMgrEnvSOrgLink->InsertExecute(&desl)))
    {
    	return g_nErrRtn;
    }

    AddItem(desl.tDPH.nID, desl);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSOrgLink::EditEnvSOrgLink(DB_ENV_SORG_LINK&	desl)
{
	PDB_ENV_SORG_LINK pdesl = FindItem(desl.tDPH.nID);
	if(!pdesl)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrEnvSOrgLink->UpdateExecute(&desl)))
    {
    	return g_nErrRtn;
    }

	EditItem(desl.tDPH.nID, desl);
	InitHash();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSOrgLink::DelEnvSOrgLink(UINT32 nID)
{
	PDB_ENV_SORG_LINK pdesl = FindItem(nID);
	if(!pdesl)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrEnvSOrgLink->DeleteExecute(pdesl->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSOrgLink::ApplyEnvSOrgLink(DB_ENV_SORG_LINK&	desl)
{
	if(FindItem(desl.tDPH.nID))
	{
		return EditEnvSOrgLink(desl);
	}
	return AddEnvSOrgLink(desl);
}
//---------------------------------------------------------------------------

String					CManageEnvSOrgLink::GetName(UINT32 nID)
{
	PDB_ENV_SORG_LINK pdesl = FindItem(nID);
    if(!pdesl)	return "";
    return pdesl->tDPH.strName;
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageEnvSOrgLink::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBEnvSOrgLinkItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSOrgLink::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_ENV_SORG_LINK pdesl = FindItem(nID);
	if(!pdesl)	return -1;

	return SetPkt(pdesl, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageEnvSOrgLink::SetPkt(PDB_ENV_SORG_LINK pdesl, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdesl->tDPH);

	SendToken.TokenAdd_32(pdesl->nUserCatchType);
	SendToken.TokenAdd_String(pdesl->strUserCatchValue);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSOrgLink::GetPkt(MemToken& RecvToken, DB_ENV_SORG_LINK& desl)
{
	if (!RecvToken.TokenDel_DPH(desl.tDPH))							goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(desl.nUserCatchType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(desl.strUserCatchValue) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



