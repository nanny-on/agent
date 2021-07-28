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
#include "ManagePoInRsNo.h"

//---------------------------------------------------------------------------

CManagePoInRsNo*	t_ManagePoInRsNo = NULL;

//---------------------------------------------------------------------------

CManagePoInRsNo::CManagePoInRsNo()
{
	t_DBMgrPoInRsNo	= new CDBMgrPoInRsNo();
}
//---------------------------------------------------------------------------

CManagePoInRsNo::~CManagePoInRsNo()
{
	if(t_DBMgrPoInRsNo)	{	delete t_DBMgrPoInRsNo;	t_DBMgrPoInRsNo = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInRsNo::LoadDBMS()
{
	TListDBPoInRsNo tDBPoInRsNoList;
    TListDBPoInRsNoItor begin, end;

	if(SetER(t_DBMgrPoInRsNo->LoadExecute(&tDBPoInRsNoList)))
	{
		return g_nErrRtn;
	}

	begin = tDBPoInRsNoList.begin();	end = tDBPoInRsNoList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
	}

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNo::InitHash()
{
	t_ManagePoInRsNoPkg->InitPkg();
	
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

INT32					CManagePoInRsNo::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_IN_RS_NO pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_in_rs_no by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nMsgType);

		{
			TMapIDItor begin, end;
			begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoInRsNoPkg->GetHash(begin->first, strPkgValue);
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

INT32					CManagePoInRsNo::AddPoInRsNo(DB_PO_IN_RS_NO&	data)
{
	if(SetER(t_DBMgrPoInRsNo->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNo::EditPoInRsNo(DB_PO_IN_RS_NO&	data)
{
	PDB_PO_IN_RS_NO pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInRsNo->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNo::DelPoInRsNo(UINT32 nID)
{
	PDB_PO_IN_RS_NO pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInRsNo->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNo::ApplyPoInRsNo(DB_PO_IN_RS_NO&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInRsNo(data);
	}
	return AddPoInRsNo(data);
}
//---------------------------------------------------------------------------

String					CManagePoInRsNo::GetName(UINT32 nID)
{
	PDB_PO_IN_RS_NO pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsNo::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInRsNoItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNo::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_NO pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);
	
	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInRsNoPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNo::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_NO pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInRsNoPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNo::SetPkt(PDB_PO_IN_RS_NO pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nMsgType);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNo::GetPkt(MemToken& RecvToken, DB_PO_IN_RS_NO& data)
{
	if(!RecvToken.TokenDel_DPH(data.tDPH))			goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(data.nMsgType))		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



