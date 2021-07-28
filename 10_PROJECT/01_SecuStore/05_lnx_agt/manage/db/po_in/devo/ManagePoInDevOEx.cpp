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
#include "ManagePoInDevOEx.h"

//---------------------------------------------------------------------------

CManagePoInDevOEx*	t_ManagePoInDevOEx = NULL;

//---------------------------------------------------------------------------

CManagePoInDevOEx::CManagePoInDevOEx()
{
	t_DBMgrPoInDevOEx	= new CDBMgrPoInDevOEx();
}
//---------------------------------------------------------------------------

CManagePoInDevOEx::~CManagePoInDevOEx()
{
	if(t_DBMgrPoInDevOEx)	{	delete t_DBMgrPoInDevOEx;	t_DBMgrPoInDevOEx = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInDevOEx::LoadDBMS()
{
	TListDBPoInDevOEx tDBPoInDevOExList;
    TListDBPoInDevOExItor begin, end;
	if(SetER(t_DBMgrPoInDevOEx->LoadExecute(&tDBPoInDevOExList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInDevOExList.begin();	end = tDBPoInDevOExList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOEx::InitHash()
{
	t_ManagePoInDevOExPkg->InitPkg();
	
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

INT32					CManagePoInDevOEx::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_IN_DEVO_EX pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_devo_ex by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
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
				t_ManagePoInDevOExPkg->GetHash(begin->first, strPkgValue);
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

INT32					CManagePoInDevOEx::AddPoInDevOEx(DB_PO_IN_DEVO_EX&	data)
{
	if(SetER(t_DBMgrPoInDevOEx->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOEx::EditPoInDevOEx(DB_PO_IN_DEVO_EX&	data)
{
	PDB_PO_IN_DEVO_EX pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInDevOEx->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOEx::DelPoInDevOEx(UINT32 nID)
{
	PDB_PO_IN_DEVO_EX pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInDevOEx->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOEx::ApplyPoInDevOEx(DB_PO_IN_DEVO_EX&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInDevOEx(data);
	}
	return AddPoInDevOEx(data);
}
//---------------------------------------------------------------------------

String					CManagePoInDevOEx::GetName(UINT32 nID)
{
	PDB_PO_IN_DEVO_EX pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInDevOEx::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInDevOExItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOEx::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_DEVO_EX pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);
	
	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInDevOExPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOEx::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_DEVO_EX pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInDevOExPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOEx::SetPkt(PDB_PO_IN_DEVO_EX pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOEx::GetPkt(MemToken& RecvToken, DB_PO_IN_DEVO_EX& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



