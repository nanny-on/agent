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
#include "ManagePoHostNotify.h"

//---------------------------------------------------------------------------

CManagePoHostNotify*	t_ManagePoHostNotify = NULL;

//---------------------------------------------------------------------------

CManagePoHostNotify::CManagePoHostNotify()
{
	t_DBMgrPoHostNotify	= new CDBMgrPoHostNotify();
}
//---------------------------------------------------------------------------

CManagePoHostNotify::~CManagePoHostNotify()
{
	if(t_DBMgrPoHostNotify)	{	delete t_DBMgrPoHostNotify;	t_DBMgrPoHostNotify = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoHostNotify::LoadDBMS()
{
	TListDBPoHostNotify tDBPoHostNotifyList;
    TListDBPoHostNotifyItor begin, end;
	if(SetER(t_DBMgrPoHostNotify->LoadExecute(&tDBPoHostNotifyList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoHostNotifyList.begin();	end = tDBPoHostNotifyList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotify::InitHash()
{
	t_ManagePoHostNotifyPkg->InitPkg();
	
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

INT32					CManagePoHostNotify::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_HOST_NOTIFY pdphn 			= NULL;
	{
		if( (pdphn = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_power by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,",
							GetHdrHashInfo(pdphn).c_str(),
							pdphn->nMsgType);

		{
			TMapIDItor begin, end;
			begin = pdphn->tDPH.tSubIDMap.begin();	end = pdphn->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoHostNotifyPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdphn->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotify::AddPoHostNotify(DB_PO_HOST_NOTIFY&	dphn)
{
	if(SetER(t_DBMgrPoHostNotify->InsertExecute(&dphn)))
    {
    	return g_nErrRtn;
    }

    AddItem(dphn.tDPH.nID, dphn);
	InitHash(dphn.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotify::EditPoHostNotify(DB_PO_HOST_NOTIFY&	dphn)
{
	PDB_PO_HOST_NOTIFY pdphn = FindItem(dphn.tDPH.nID);
	if(!pdphn)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoHostNotify->UpdateExecute(&dphn)))
    {
    	return g_nErrRtn;
    }

    EditItem(dphn.tDPH.nID, dphn);
	InitHash(dphn.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotify::DelPoHostNotify(UINT32 nID)
{
	PDB_PO_HOST_NOTIFY pdpp = FindItem(nID);
	if(!pdpp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoHostNotify->DeleteExecute(pdpp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotify::ApplyPoHostNotify(DB_PO_HOST_NOTIFY&	dphn)
{
	if(FindItem(dphn.tDPH.nID))
	{
		return EditPoHostNotify(dphn);
	}
	return AddPoHostNotify(dphn);
}
//---------------------------------------------------------------------------

String					CManagePoHostNotify::GetName(UINT32 nID)
{
	PDB_PO_HOST_NOTIFY pdpp = FindItem(nID);
    if(!pdpp)	return "";
    return pdpp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoHostNotify::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoHostNotifyItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotify::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_HOST_NOTIFY pdpp = FindItem(nID);
	if(!pdpp)	return -1;

	SetPkt(pdpp, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpp->tDPH.tSubIDMap.begin();	end = pdpp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoHostNotifyPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotify::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_HOST_NOTIFY pdpp = FindItem(nID);
	if(!pdpp)	return -1;

	SetPkt(pdpp, SendToken);

	{
		SendToken.TokenAdd_32(pdpp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpp->tDPH.tSubIDMap.begin();	end = pdpp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoHostNotifyPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotify::SetPkt(PDB_PO_HOST_NOTIFY pdpp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpp->tDPH);

	SendToken.TokenAdd_32(pdpp->nMsgType);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotify::GetPkt(MemToken& RecvToken, DB_PO_HOST_NOTIFY& dpp)
{
	if (!RecvToken.TokenDel_DPH(dpp.tDPH))				goto	INVALID_PKT;

	if(!RecvToken.TokenDel_32(dpp.nMsgType))		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



