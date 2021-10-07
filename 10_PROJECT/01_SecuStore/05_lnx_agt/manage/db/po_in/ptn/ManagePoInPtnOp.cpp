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
#include "ManagePoInPtnOp.h"

//---------------------------------------------------------------------------

CManagePoInPtnOp*	t_ManagePoInPtnOp = NULL;

//---------------------------------------------------------------------------

CManagePoInPtnOp::CManagePoInPtnOp()
{
	t_DBMgrPoInPtnOp	= new CDBMgrPoInPtnOp();
}
//---------------------------------------------------------------------------

CManagePoInPtnOp::~CManagePoInPtnOp()
{
	if(t_DBMgrPoInPtnOp)	{	delete t_DBMgrPoInPtnOp;	t_DBMgrPoInPtnOp = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInPtnOp::LoadDBMS()
{
	TListDBPoInPtnOp tDBPoInPtnOpList;
    TListDBPoInPtnOpItor begin, end;
	if(SetER(t_DBMgrPoInPtnOp->LoadExecute(&tDBPoInPtnOpList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInPtnOpList.begin();	end = tDBPoInPtnOpList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::InitHash()
{
	t_ManagePoInPtnOpPkg->InitPkg();

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

INT32					CManagePoInPtnOp::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_IN_PTN_OP pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_ptn_op by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%u,%u,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nBlockMode, pdata->nKillMode, pdata->nRTFGMode);

		{
			TMapIDItor begin, end;
			begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoInPtnOpPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdata->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::AddPoInPtnOp(DB_PO_IN_PTN_OP&	data)
{
	if(SetER(t_DBMgrPoInPtnOp->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::EditPoInPtnOp(DB_PO_IN_PTN_OP&	data)
{
	PDB_PO_IN_PTN_OP pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInPtnOp->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::DelPoInPtnOp(UINT32 nID)
{
	PDB_PO_IN_PTN_OP pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInPtnOp->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::ApplyPoInPtnOp(DB_PO_IN_PTN_OP&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInPtnOp(data);
	}
	return AddPoInPtnOp(data);
}
//---------------------------------------------------------------------------

String					CManagePoInPtnOp::GetName(UINT32 nID)
{
	PDB_PO_IN_PTN_OP pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::IsValidPtnFile(UINT32 nID)
{	
	PDB_PO_IN_PTN_OP pdata = FindItem(nID);
	if(!pdata)	return 0;

	if(pdata->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		return 0;

	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInPtnOpItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInPtnOpPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::SetPkt(PDB_PO_IN_PTN_OP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nBlockMode);
	SendToken.TokenAdd_32(pdata->nKillMode);
	SendToken.TokenAdd_32(pdata->nRTFGMode);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::GetPkt(MemToken& RecvToken, DB_PO_IN_PTN_OP& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nBlockMode))			goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nKillMode))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRTFGMode))				goto INVALID_PKT;	

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPktHost(pdata, SendToken);

	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInPtnOpPkg->SetPktHost(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::SetPktHost(PDB_PO_IN_PTN_OP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nBlockMode);
	SendToken.TokenAdd_32(pdata->nKillMode);
	SendToken.TokenAdd_32(pdata->nRTFGMode);
	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOp::GetPktHost(MemToken& RecvToken, DB_PO_IN_PTN_OP& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nBlockMode))			goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nKillMode))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRTFGMode))				goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------


UINT32					CManagePoInPtnOp::GetUsedMode(UINT32 nID)
{
	PDB_PO_IN_PTN_OP pdata = NULL;

	if(nID)
		pdata = FindItem(nID);
	else
		pdata = FirstItem();

	if(!pdata)
		return STATUS_USED_MODE_OFF;

	return pdata->tDPH.nUsedMode;
}
