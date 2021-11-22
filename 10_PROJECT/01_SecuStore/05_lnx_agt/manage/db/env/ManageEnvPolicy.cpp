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
#include "ManageEnvPolicy.h"

//---------------------------------------------------------------------------

CManageEnvPolicy*	t_ManageEnvPolicy = NULL;

//---------------------------------------------------------------------------

CManageEnvPolicy::CManageEnvPolicy()
{
	t_DBMgrEnvPolicy	= new CDBMgrEnvPolicy();
}
//---------------------------------------------------------------------------

CManageEnvPolicy::~CManageEnvPolicy()
{
	if(t_DBMgrEnvPolicy)	{	delete t_DBMgrEnvPolicy;	t_DBMgrEnvPolicy = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageEnvPolicy::LoadDBMS()
{
	DB_ENV_POLICY dep;
	TListDBEnvPolicy tDBEnvPolicyList;
    TListDBEnvPolicyItor begin, end;
	
	if(SetER(t_DBMgrEnvPolicy->LoadExecute(&tDBEnvPolicyList)))
    {
		if(tDBEnvPolicyList.size() == 0)
		{
			memset(&dep, 0, sizeof(DB_ENV_POLICY));
			AddEnvPolicy(dep);
		}    
    	return g_nErrRtn;
    }

    begin = tDBEnvPolicyList.begin();	end = tDBEnvPolicyList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
	}

	if(tDBEnvPolicyList.size() == 0)
	{
		memset(&dep, 0, sizeof(DB_ENV_POLICY));
		AddEnvPolicy(dep);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvPolicy::InitHash()
{
	String strOrgValue;

	PDB_ENV_POLICY pdep 			= NULL;
	{
		if( (pdep = FirstItem()) == NULL)
		{
			WriteLogE("not find env_policy by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,%u,%u,", 
			GetHdrHashInfo(pdep).c_str(),
			pdep->nPolicyType, pdep->nPolicyOption);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdep->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManageEnvPolicy::GetHash()
{
	PDB_ENV_POLICY pdep = FirstItem();
	if(!pdep)	return "";

	return pdep->tDPH.strHash;
}
//---------------------------------------------------------------------------

INT32					CManageEnvPolicy::AddEnvPolicy(DB_ENV_POLICY&	dep)
{
	if(SetER(t_DBMgrEnvPolicy->InsertExecute(&dep)))
    {
    	return g_nErrRtn;
    }

    AddItem(dep.tDPH.nID, dep);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvPolicy::EditEnvPolicy(DB_ENV_POLICY&	dep)
{
	PDB_ENV_POLICY pdep = FindItem(dep.tDPH.nID);
	if(!pdep)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrEnvPolicy->UpdateExecute(&dep)))
    {
    	return g_nErrRtn;
    }

    EditItem(dep.tDPH.nID, dep);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvPolicy::DelEnvPolicy(UINT32 nID)
{
	PDB_ENV_POLICY pdep = FindItem(nID);
	if(!pdep)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrEnvPolicy->DeleteExecute(pdep->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

String					CManageEnvPolicy::GetName(UINT32 nID)
{
	PDB_ENV_POLICY pdep = FindItem(nID);
    if(!pdep)	return "";
    return pdep->tDPH.strHash;
}
//---------------------------------------------------------------------------

UINT32					CManageEnvPolicy::GetCurPolicyType()
{
	PDB_ENV_POLICY pdep = FirstItem();
	if(!pdep)	return SS_ENV_POLICY_TYPE_OBJECT;

	return pdep->nPolicyType;
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

INT32					CManageEnvPolicy::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBEnvPolicyItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvPolicy::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_ENV_POLICY pdep = FindItem(nID);
	if(!pdep)	return -1;

	return SetPkt(pdep, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageEnvPolicy::SetPkt(PDB_ENV_POLICY pdep, MemToken& SendToken)
{

	SendToken.TokenAdd_DPH(pdep->tDPH);

	SendToken.TokenAdd_32(pdep->nPolicyType);
	SendToken.TokenAdd_32(pdep->nPolicyOption);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvPolicy::GetPkt(MemToken& RecvToken, DB_ENV_POLICY& dep)
{
	if (!RecvToken.TokenDel_DPH(dep.tDPH))						goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dep.nPolicyType))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dep.nPolicyOption))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



