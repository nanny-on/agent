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
#include "ManagePoFaBk.h"

//---------------------------------------------------------------------------

CManagePoFaBk*	t_ManagePoFaBk = NULL;

//---------------------------------------------------------------------------

CManagePoFaBk::CManagePoFaBk()
{
	t_DBMgrPoFaBk	= new CDBMgrPoFaBk();
}
//---------------------------------------------------------------------------

CManagePoFaBk::~CManagePoFaBk()
{
	if(t_DBMgrPoFaBk)	{	delete t_DBMgrPoFaBk;	t_DBMgrPoFaBk = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaBk::LoadDBMS()
{
	TListDBPoFaBk tDBPoFaBkList;
    TListDBPoFaBkItor begin, end;
	if(SetER(t_DBMgrPoFaBk->LoadExecute(&tDBPoFaBkList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaBkList.begin();	end = tDBPoFaBkList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaBk::InitHash()
{
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

INT32					CManagePoFaBk::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FA_BK pdpfb 			= NULL;
	{
		if( (pdpfb = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_bk by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%u,%u,"
							"%u,", 
							GetHdrHashInfo(pdpfb).c_str(),
							pdpfb->nKeepDay, pdpfb->nKeepSize, pdpfb->nFreeSize,
							pdpfb->nFileMaxSize);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfb->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaBk::AddPoFaBk(DB_PO_FA_BK&	dpfb)
{
	if(SetER(t_DBMgrPoFaBk->InsertExecute(&dpfb)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfb.tDPH.nID, dpfb);
	InitHash(dpfb.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaBk::EditPoFaBk(DB_PO_FA_BK&	dpfb)
{
	PDB_PO_FA_BK pdpfb = FindItem(dpfb.tDPH.nID);
	if(!pdpfb)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaBk->UpdateExecute(&dpfb)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfb.tDPH.nID, dpfb);
	InitHash(dpfb.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaBk::DelPoFaBk(UINT32 nID)
{
	PDB_PO_FA_BK pdpfb = FindItem(nID);
	if(!pdpfb)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaBk->DeleteExecute(pdpfb->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}

INT32					CManagePoFaBk::ClearPoFaBk()
{
	if(SetER(t_DBMgrPoFaBk->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}

//---------------------------------------------------------------------------

INT32					CManagePoFaBk::ApplyPoFaBk(DB_PO_FA_BK&	dpfb)
{
	if(FindItem(dpfb.tDPH.nID))
	{
		return EditPoFaBk(dpfb);
	}
	return AddPoFaBk(dpfb);
}
//---------------------------------------------------------------------------

String					CManagePoFaBk::GetName(UINT32 nID)
{
	PDB_PO_FA_BK pdpfb = FindItem(nID);
    if(!pdpfb)	return "";
    return pdpfb->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaBk::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaBkItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaBk::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_BK pdpfb = FindItem(nID);
	if(!pdpfb)	return -1;

	return SetPkt(pdpfb, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaBk::SetPkt(PDB_PO_FA_BK pdpfb, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfb->tDPH);
	
	SendToken.TokenAdd_32(pdpfb->nKeepDay);
	SendToken.TokenAdd_32(pdpfb->nKeepSize);
	SendToken.TokenAdd_32(pdpfb->nFreeSize);
	SendToken.TokenAdd_32(pdpfb->nFileMaxSize);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaBk::GetPkt(MemToken& RecvToken, DB_PO_FA_BK& dpfb)
{
	if (!RecvToken.TokenDel_DPH(dpfb.tDPH))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfb.nKeepDay))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfb.nKeepSize))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfb.nFreeSize))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfb.nFileMaxSize))		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



