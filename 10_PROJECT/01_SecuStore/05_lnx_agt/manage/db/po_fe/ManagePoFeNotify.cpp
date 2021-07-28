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
#include "ManagePoFeNotify.h"

//---------------------------------------------------------------------------

CManagePoFeNotify*	t_ManagePoFeNotify = NULL;

//---------------------------------------------------------------------------

CManagePoFeNotify::CManagePoFeNotify()
{
	t_DBMgrPoFeNotify	= new CDBMgrPoFeNotify();
}
//---------------------------------------------------------------------------

CManagePoFeNotify::~CManagePoFeNotify()
{
	if(t_DBMgrPoFeNotify)	{	delete t_DBMgrPoFeNotify;	t_DBMgrPoFeNotify = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFeNotify::LoadDBMS()
{
	TListDBPoFeNotify tDBPoFeNotifyList;
    TListDBPoFeNotifyItor begin, end;
	if(SetER(t_DBMgrPoFeNotify->LoadExecute(&tDBPoFeNotifyList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFeNotifyList.begin();	end = tDBPoFeNotifyList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotify::InitHash()
{
	t_ManagePoFeNotifyPkg->InitPkg();
	
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

INT32					CManagePoFeNotify::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FE_NOTIFY pdpfn 			= NULL;
	{
		if( (pdpfn = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fe_notify by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,", 
							GetHdrHashInfo(pdpfn).c_str(),
							pdpfn->nMsgType);

		{
			TMapIDItor begin, end;
			begin = pdpfn->tDPH.tSubIDMap.begin();	end = pdpfn->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoFeNotifyPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfn->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotify::AddPoFeNotify(DB_PO_FE_NOTIFY&	dpfn)
{
	if(SetER(t_DBMgrPoFeNotify->InsertExecute(&dpfn)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfn.tDPH.nID, dpfn);
	InitHash(dpfn.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotify::EditPoFeNotify(DB_PO_FE_NOTIFY&	dpfn)
{
	PDB_PO_FE_NOTIFY pdpfn = FindItem(dpfn.tDPH.nID);
	if(!pdpfn)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFeNotify->UpdateExecute(&dpfn)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfn.tDPH.nID, dpfn);
	InitHash(dpfn.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotify::DelPoFeNotify(UINT32 nID)
{
	PDB_PO_FE_NOTIFY pdpfn = FindItem(nID);
	if(!pdpfn)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFeNotify->DeleteExecute(pdpfn->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotify::ApplyPoFeNotify(DB_PO_FE_NOTIFY&	dpfn)
{
	if(FindItem(dpfn.tDPH.nID))
	{
		return EditPoFeNotify(dpfn);
	}
	return AddPoFeNotify(dpfn);
}
//---------------------------------------------------------------------------

String					CManagePoFeNotify::GetName(UINT32 nID)
{
	PDB_PO_FE_NOTIFY pdpfn = FindItem(nID);
    if(!pdpfn)	return "";
    return pdpfn->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFeNotify::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFeNotifyItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotify::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_NOTIFY pdpfn = FindItem(nID);
	if(!pdpfn)	return -1;

	SetPkt(pdpfn, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpfn->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfn->tDPH.tSubIDMap.begin();	end = pdpfn->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFeNotifyPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotify::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_NOTIFY pdpfn = FindItem(nID);
	if(!pdpfn)	return -1;

	SetPkt(pdpfn, SendToken);

	{
		SendToken.TokenAdd_32(pdpfn->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfn->tDPH.tSubIDMap.begin();	end = pdpfn->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFeNotifyPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotify::SetPkt(PDB_PO_FE_NOTIFY pdpfn, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfn->tDPH);

	SendToken.TokenAdd_32(pdpfn->nMsgType);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotify::GetPkt(MemToken& RecvToken, DB_PO_FE_NOTIFY& dpfn)
{
	if(!RecvToken.TokenDel_DPH(dpfn.tDPH))			goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(dpfn.nMsgType))		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



