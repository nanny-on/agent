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
#include "ManageEnvTrustSubject.h"

//---------------------------------------------------------------------------

CManageEnvTrustSubject*	t_ManageEnvTrustSubject = NULL;

//---------------------------------------------------------------------------

CManageEnvTrustSubject::CManageEnvTrustSubject()
{
	t_DBMgrEnvTrustSubject	= new CDBMgrEnvTrustSubject();
}
//---------------------------------------------------------------------------

CManageEnvTrustSubject::~CManageEnvTrustSubject()
{
	if(t_DBMgrEnvTrustSubject)	{	delete t_DBMgrEnvTrustSubject;	t_DBMgrEnvTrustSubject = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageEnvTrustSubject::LoadDBMS()
{
	TListDBEnvTrustSubject tDBEnvTrustSubjectList;
    TListDBEnvTrustSubjectItor begin, end;
	if(SetER(t_DBMgrEnvTrustSubject->LoadExecute(&tDBEnvTrustSubjectList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBEnvTrustSubjectList.begin();	end = tDBEnvTrustSubjectList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvTrustSubject::InitHash()
{
	String strOrgValue;

	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_TRUST_SUBJECT pdets 			= NULL;
		{
			if( (pdets = FindItem(*begin)) == NULL)
			{
				WriteLogE("not find env_trust_subject by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
				return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
			}
		}

		{
 			String strOrgValueItem = SPrintf("%s,%s,", 
								GetHdrHashInfo(pdets).c_str(),
								pdets->strSubjectInfo.c_str());

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

String					CManageEnvTrustSubject::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManageEnvTrustSubject::AddEnvTrustSubject(DB_ENV_TRUST_SUBJECT&	dets)
{
	if(SetER(t_DBMgrEnvTrustSubject->InsertExecute(&dets)))
    {
    	return g_nErrRtn;
    }

    AddItem(dets.tDPH.nID, dets);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvTrustSubject::EditEnvTrustSubject(DB_ENV_TRUST_SUBJECT&	dets)
{
	PDB_ENV_TRUST_SUBJECT pdets = FindItem(dets.tDPH.nID);
	if(!pdets)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrEnvTrustSubject->UpdateExecute(&dets)))
    {
    	return g_nErrRtn;
    }

    EditItem(dets.tDPH.nID, dets);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvTrustSubject::DelEnvTrustSubject(UINT32 nID)
{
	PDB_ENV_TRUST_SUBJECT pdets = FindItem(nID);
	if(!pdets)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrEnvTrustSubject->DeleteExecute(pdets->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvTrustSubject::ApplyEnvTrustSubject(DB_ENV_TRUST_SUBJECT&	dets)
{
	if(FindItem(dets.tDPH.nID))
	{
		return EditEnvTrustSubject(dets);
	}
	return AddEnvTrustSubject(dets);
}
//---------------------------------------------------------------------------

String					CManageEnvTrustSubject::GetName(UINT32 nID)
{
	PDB_ENV_TRUST_SUBJECT pdets = FindItem(nID);
    if(!pdets)	return "";
    return pdets->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageEnvTrustSubject::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBEnvTrustSubjectItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvTrustSubject::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_ENV_TRUST_SUBJECT pdets = FindItem(nID);
	if(!pdets)	return -1;

	return SetPkt(pdets, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageEnvTrustSubject::SetPkt(PDB_ENV_TRUST_SUBJECT pdets, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdets->tDPH);

	SendToken.TokenAdd_String(pdets->strSubjectInfo);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvTrustSubject::GetPkt(MemToken& RecvToken, DB_ENV_TRUST_SUBJECT& dets)
{
	if (!RecvToken.TokenDel_DPH(dets.tDPH))						goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dets.strSubjectInfo) < 0)	goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageEnvTrustSubject::SetPktDbg(MemToken& SendToken)
{
	return 0;
}
//---------------------------------------------------------------------------



