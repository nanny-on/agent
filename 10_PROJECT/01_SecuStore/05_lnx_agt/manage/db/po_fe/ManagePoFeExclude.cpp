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
#include "ManagePoFeExclude.h"

//---------------------------------------------------------------------------

CManagePoFeExclude*	t_ManagePoFeExclude = NULL;

//---------------------------------------------------------------------------

CManagePoFeExclude::CManagePoFeExclude()
{
	t_DBMgrPoFeExclude	= new CDBMgrPoFeExclude();
}
//---------------------------------------------------------------------------

CManagePoFeExclude::~CManagePoFeExclude()
{
	if(t_DBMgrPoFeExclude)	{	delete t_DBMgrPoFeExclude;	t_DBMgrPoFeExclude = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFeExclude::LoadDBMS()
{
	TListDBPoFeExclude tDBPoFeExcludeList;
    TListDBPoFeExcludeItor begin, end;
	if(SetER(t_DBMgrPoFeExclude->LoadExecute(&tDBPoFeExcludeList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFeExcludeList.begin();	end = tDBPoFeExcludeList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::InitHash()
{
	t_ManagePoFeExcludePkg->InitPkg();
	
	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		InitHash(*begin);
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FE_EXCLUDE pdpfe 			= NULL;
	{
		if( (pdpfe = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fe_exclude by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,", 
							GetHdrHashInfo(pdpfe).c_str());

		{
			TMapIDItor begin, end;
			begin = pdpfe->tDPH.tSubIDMap.begin();	end = pdpfe->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoFeExcludePkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfe->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::AddPoFeExclude(DB_PO_FE_EXCLUDE&	dpfe)
{
	if(SetER(t_DBMgrPoFeExclude->InsertExecute(&dpfe)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfe.tDPH.nID, dpfe);
	InitHash(dpfe.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::EditPoFeExclude(DB_PO_FE_EXCLUDE&	dpfe)
{
	PDB_PO_FE_EXCLUDE pdpfe = FindItem(dpfe.tDPH.nID);
	if(!pdpfe)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFeExclude->UpdateExecute(&dpfe)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfe.tDPH.nID, dpfe);
	InitHash(dpfe.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::DelPoFeExclude(UINT32 nID)
{
	PDB_PO_FE_EXCLUDE pdpfe = FindItem(nID);
	if(!pdpfe)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFeExclude->DeleteExecute(pdpfe->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::ApplyPoFeExclude(DB_PO_FE_EXCLUDE&	dpfe)
{
	if(FindItem(dpfe.tDPH.nID))
	{
		return EditPoFeExclude(dpfe);
	}
	return AddPoFeExclude(dpfe);
}
//---------------------------------------------------------------------------

String					CManagePoFeExclude::GetName(UINT32 nID)
{
	PDB_PO_FE_EXCLUDE pdpfe = FindItem(nID);
    if(!pdpfe)	return "";
    return pdpfe->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFeExcludeItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_EXCLUDE pdpfe = FindItem(nID);
	if(!pdpfe)	return -1;

	SetPkt(pdpfe, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpfe->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfe->tDPH.tSubIDMap.begin();	end = pdpfe->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFeExcludePkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_EXCLUDE pdpfe = FindItem(nID);
	if(!pdpfe)	return -1;

	SetPkt(pdpfe, SendToken);

	{
		SendToken.TokenAdd_32(pdpfe->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfe->tDPH.tSubIDMap.begin();	end = pdpfe->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFeExcludePkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::SetPkt(PDB_PO_FE_EXCLUDE pdpfe, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfe->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExclude::GetPkt(MemToken& RecvToken, DB_PO_FE_EXCLUDE& dpfe)
{
	if (!RecvToken.TokenDel_DPH(dpfe.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



