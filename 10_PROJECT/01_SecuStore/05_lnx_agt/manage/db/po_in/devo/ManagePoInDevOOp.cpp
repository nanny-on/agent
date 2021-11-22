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
#include "ManagePoInDevOOp.h"

//---------------------------------------------------------------------------

CManagePoInDevOOp*	t_ManagePoInDevOOp = NULL;

//---------------------------------------------------------------------------

CManagePoInDevOOp::CManagePoInDevOOp()
{
	t_DBMgrPoInDevOOp	= new CDBMgrPoInDevOOp();
}
//---------------------------------------------------------------------------

CManagePoInDevOOp::~CManagePoInDevOOp()
{
	if(t_DBMgrPoInDevOOp)	{	delete t_DBMgrPoInDevOOp;	t_DBMgrPoInDevOOp = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInDevOOp::LoadDBMS()
{
	TListDBPoInDevOOp tDBPoInDevOOpList;
    TListDBPoInDevOOpItor begin, end;
	if(SetER(t_DBMgrPoInDevOOp->LoadExecute(&tDBPoInDevOOpList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInDevOOpList.begin();	end = tDBPoInDevOOpList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::InitHash()
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

INT32					CManagePoInDevOOp::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_IN_DEVO_OP pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_devo_op by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nBlockMode);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdata->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::AddPoInDevOOp(DB_PO_IN_DEVO_OP&	data)
{
	if(SetER(t_DBMgrPoInDevOOp->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::EditPoInDevOOp(DB_PO_IN_DEVO_OP&	data)
{
	PDB_PO_IN_DEVO_OP pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInDevOOp->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::DelPoInDevOOp(UINT32 nID)
{
	PDB_PO_IN_DEVO_OP pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInDevOOp->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::ApplyPoInDevOOp(DB_PO_IN_DEVO_OP&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInDevOOp(data);
	}
	return AddPoInDevOOp(data);
}
//---------------------------------------------------------------------------

String					CManagePoInDevOOp::GetName(UINT32 nID)
{
	PDB_PO_IN_DEVO_OP pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInDevOOpItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_DEVO_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::SetPkt(PDB_PO_IN_DEVO_OP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nBlockMode);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::GetPkt(MemToken& RecvToken, DB_PO_IN_DEVO_OP& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nBlockMode))			goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_DEVO_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPktHost(pdata, SendToken);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::SetPktHost(PDB_PO_IN_DEVO_OP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nBlockMode);
	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOOp::GetPktHost(MemToken& RecvToken, DB_PO_IN_DEVO_OP& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nBlockMode))			goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------



