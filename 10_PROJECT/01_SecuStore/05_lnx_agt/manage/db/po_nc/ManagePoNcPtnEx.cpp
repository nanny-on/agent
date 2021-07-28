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
#include "ManagePoNcPtnEx.h"

//---------------------------------------------------------------------------

CManagePoNcPtnEx*	t_ManagePoNcPtnEx = NULL;

//---------------------------------------------------------------------------

CManagePoNcPtnEx::CManagePoNcPtnEx()
{
	t_DBMgrPoNcPtnEx	= new CDBMgrPoNcPtnEx();
}
//---------------------------------------------------------------------------

CManagePoNcPtnEx::~CManagePoNcPtnEx()
{
	if(t_DBMgrPoNcPtnEx)	{	delete t_DBMgrPoNcPtnEx;	t_DBMgrPoNcPtnEx = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoNcPtnEx::LoadDBMS()
{
	TListDBPoNcPtnEx tDBPoNcPtnExList;
    TListDBPoNcPtnExItor begin, end;
	if(SetER(t_DBMgrPoNcPtnEx->LoadExecute(&tDBPoNcPtnExList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoNcPtnExList.begin();	end = tDBPoNcPtnExList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnEx::InitHash()
{
	t_ManagePoNcPtnExPkg->InitPkg();
	
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

INT32					CManagePoNcPtnEx::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_NC_PTN_EX pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_nc_ptn_ex by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,", 
							GetHdrHashInfo(pdata).c_str());

		{
			TMapIDItor begin, end;
			begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoNcPtnExPkg->GetHash(begin->first, strPkgValue);
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

INT32					CManagePoNcPtnEx::AddPoNcPtnEx(DB_PO_NC_PTN_EX&	data)
{
	if(SetER(t_DBMgrPoNcPtnEx->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnEx::EditPoNcPtnEx(DB_PO_NC_PTN_EX&	data)
{
	PDB_PO_NC_PTN_EX pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoNcPtnEx->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnEx::DelPoNcPtnEx(UINT32 nID)
{
	PDB_PO_NC_PTN_EX pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoNcPtnEx->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnEx::ApplyPoNcPtnEx(DB_PO_NC_PTN_EX&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoNcPtnEx(data);
	}
	return AddPoNcPtnEx(data);
}
//---------------------------------------------------------------------------

String					CManagePoNcPtnEx::GetName(UINT32 nID)
{
	PDB_PO_NC_PTN_EX pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnEx::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoNcPtnExItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnEx::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_EX pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);
	
	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoNcPtnExPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnEx::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_EX pdata = FindItem(nID); 
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoNcPtnExPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnEx::SetPkt(PDB_PO_NC_PTN_EX pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnEx::GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_EX& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



