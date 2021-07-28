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
#include "ManageEnvSelfProtectAgt.h"

//---------------------------------------------------------------------------

CManageEnvSelfProtectAgt*	t_ManageEnvSelfProtectAgt = NULL;

//---------------------------------------------------------------------------

CManageEnvSelfProtectAgt::CManageEnvSelfProtectAgt()
{
	t_DBMgrEnvSelfProtectAgt	= new CDBMgrEnvSelfProtectAgt();
}
//---------------------------------------------------------------------------

CManageEnvSelfProtectAgt::~CManageEnvSelfProtectAgt()
{
	if(t_DBMgrEnvSelfProtectAgt)	{	delete t_DBMgrEnvSelfProtectAgt;	t_DBMgrEnvSelfProtectAgt = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageEnvSelfProtectAgt::LoadDBMS()
{
	TListDBEnvSelfProtectAgt tDBEnvSelfProtectAgtList;
    TListDBEnvSelfProtectAgtItor begin, end;
	if(SetER(t_DBMgrEnvSelfProtectAgt->LoadExecute(&tDBEnvSelfProtectAgtList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBEnvSelfProtectAgtList.begin();	end = tDBEnvSelfProtectAgtList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSelfProtectAgt::InitHash()
{
	String strOrgValue;

	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_SELF_PROTECT_AGT pdespa 			= NULL;
		{
			if( (pdespa = FindItem(*begin)) == NULL)
			{
				WriteLogE("not find env_self_protect_agt by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
				return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
			}
		}

		{
 			String strOrgValueItem = SPrintf("%s,%u,%s,%u,", 
								GetHdrHashInfo(pdespa).c_str(),
								pdespa->nProtectType, pdespa->strProtectInfo.c_str(), pdespa->nApplyPerm);

			strOrgValue += strOrgValueItem;
		}
	}	
	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		m_strFullHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManageEnvSelfProtectAgt::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSelfProtectAgt::AddEnvSelfProtectAgt(DB_ENV_SELF_PROTECT_AGT&	despa)
{
	if(SetER(t_DBMgrEnvSelfProtectAgt->InsertExecute(&despa)))
    {
    	return g_nErrRtn;
    }

    AddItem(despa.tDPH.nID, despa);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSelfProtectAgt::EditEnvSelfProtectAgt(DB_ENV_SELF_PROTECT_AGT&	despa)
{
	PDB_ENV_SELF_PROTECT_AGT pdespa = FindItem(despa.tDPH.nID);
	if(!pdespa)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrEnvSelfProtectAgt->UpdateExecute(&despa)))
    {
    	return g_nErrRtn;
    }

    EditItem(despa.tDPH.nID, despa);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSelfProtectAgt::DelEnvSelfProtectAgt(UINT32 nID)
{
	PDB_ENV_SELF_PROTECT_AGT pdespa = FindItem(nID);
	if(!pdespa)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrEnvSelfProtectAgt->DeleteExecute(pdespa->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSelfProtectAgt::ApplyEnvSelfProtectAgt(DB_ENV_SELF_PROTECT_AGT&	despa)
{
	if(FindItem(despa.tDPH.nID))
	{
		return EditEnvSelfProtectAgt(despa);
	}
	return AddEnvSelfProtectAgt(despa);
}
//---------------------------------------------------------------------------

String					CManageEnvSelfProtectAgt::GetName(UINT32 nID)
{
	PDB_ENV_SELF_PROTECT_AGT pdespa = FindItem(nID);
    if(!pdespa)	return "";
    return pdespa->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageEnvSelfProtectAgt::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBEnvSelfProtectAgtItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSelfProtectAgt::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_ENV_SELF_PROTECT_AGT pdespa = FindItem(nID);
	if(!pdespa)	return -1;

	return SetPkt(pdespa, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageEnvSelfProtectAgt::SetPkt(PDB_ENV_SELF_PROTECT_AGT pdespa, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdespa->tDPH);

	SendToken.TokenAdd_32(pdespa->nProtectType);
	SendToken.TokenAdd_String(pdespa->strProtectInfo);
	SendToken.TokenAdd_32(pdespa->nApplyPerm);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvSelfProtectAgt::GetPkt(MemToken& RecvToken, DB_ENV_SELF_PROTECT_AGT& despa)
{
	if (!RecvToken.TokenDel_DPH(despa.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(despa.nProtectType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(despa.strProtectInfo) < 0)	goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(despa.nApplyPerm))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



