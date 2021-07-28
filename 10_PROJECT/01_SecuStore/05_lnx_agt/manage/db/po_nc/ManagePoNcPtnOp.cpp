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
#include "ManagePoNcPtnOp.h"

//---------------------------------------------------------------------------

CManagePoNcPtnOp*	t_ManagePoNcPtnOp = NULL;

//---------------------------------------------------------------------------

CManagePoNcPtnOp::CManagePoNcPtnOp()
{
	t_DBMgrPoNcPtnOp	= new CDBMgrPoNcPtnOp();
}
//---------------------------------------------------------------------------

CManagePoNcPtnOp::~CManagePoNcPtnOp()
{
	if(t_DBMgrPoNcPtnOp)	{	delete t_DBMgrPoNcPtnOp;	t_DBMgrPoNcPtnOp = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoNcPtnOp::LoadDBMS()
{
	TListDBPoNcPtnOp tDBPoNcPtnOpList;
    TListDBPoNcPtnOpItor begin, end;
	if(SetER(t_DBMgrPoNcPtnOp->LoadExecute(&tDBPoNcPtnOpList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoNcPtnOpList.begin();	end = tDBPoNcPtnOpList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::InitHash()
{
	t_ManagePoNcPtnOpPkg->InitPkg();

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

INT32					CManagePoNcPtnOp::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_NC_PTN_OP pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_nc_ptn_op by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
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
				t_ManagePoNcPtnOpPkg->GetHash(begin->first, strPkgValue);
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

INT32					CManagePoNcPtnOp::AddPoNcPtnOp(DB_PO_NC_PTN_OP&	data)
{
	if(SetER(t_DBMgrPoNcPtnOp->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::EditPoNcPtnOp(DB_PO_NC_PTN_OP&	data)
{
	PDB_PO_NC_PTN_OP pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoNcPtnOp->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::DelPoNcPtnOp(UINT32 nID)
{
	PDB_PO_NC_PTN_OP pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoNcPtnOp->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::ApplyPoNcPtnOp(DB_PO_NC_PTN_OP&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoNcPtnOp(data);
	}
	return AddPoNcPtnOp(data);
}
//---------------------------------------------------------------------------

String					CManagePoNcPtnOp::GetName(UINT32 nID)
{
	PDB_PO_NC_PTN_OP pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::IsValidPtnFile(UINT32 nID)
{	
	PDB_PO_NC_PTN_OP pdata = FindItem(nID);
	if(!pdata)	return 0;

	if(pdata->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		return 0;

	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoNcPtnOpItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoNcPtnOpPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::SetPkt(PDB_PO_NC_PTN_OP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nBlockMode);
	SendToken.TokenAdd_32(pdata->nKillMode);
	SendToken.TokenAdd_32(pdata->nRTFGMode);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_OP& data)
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

INT32					CManagePoNcPtnOp::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPktHost(pdata, SendToken);

	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoNcPtnOpPkg->SetPktHost(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::SetPktHost(PDB_PO_NC_PTN_OP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nBlockMode);
	SendToken.TokenAdd_32(pdata->nKillMode);
	SendToken.TokenAdd_32(pdata->nRTFGMode);
	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnOp::GetPktHost(MemToken& RecvToken, DB_PO_NC_PTN_OP& data)
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



