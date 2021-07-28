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
#include "ManagePoInRsBk.h"

//---------------------------------------------------------------------------

CManagePoInRsBk*	t_ManagePoInRsBk = NULL;

//---------------------------------------------------------------------------

CManagePoInRsBk::CManagePoInRsBk()
{
	t_DBMgrPoInRsBk	= new CDBMgrPoInRsBk();
}
//---------------------------------------------------------------------------

CManagePoInRsBk::~CManagePoInRsBk()
{
	if(t_DBMgrPoInRsBk)	{	delete t_DBMgrPoInRsBk;	t_DBMgrPoInRsBk = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInRsBk::LoadDBMS()
{
	TListDBPoInRsBk tDBPoInRsBkList;
    TListDBPoInRsBkItor begin, end;

	if(SetER(t_DBMgrPoInRsBk->LoadExecute(&tDBPoInRsBkList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInRsBkList.begin();	end = tDBPoInRsBkList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsBk::InitHash()
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

INT32					CManagePoInRsBk::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_IN_RS_BK pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_rs_bk by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%u,"
							"%u,%u,"
							"%u,%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nKeepSize, pdata->nFreeSize,
							pdata->nFileMaxSize, pdata->nFileKeepNum,
							pdata->nFExtType, pdata->strFExtInfo.c_str());
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdata->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsBk::AddPoInRsBk(DB_PO_IN_RS_BK&	data)
{
	if(SetER(t_DBMgrPoInRsBk->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsBk::EditPoInRsBk(DB_PO_IN_RS_BK&	data)
{
	PDB_PO_IN_RS_BK pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInRsBk->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsBk::DelPoInRsBk(UINT32 nID)
{
	PDB_PO_IN_RS_BK pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInRsBk->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsBk::ApplyPoInRsBk(DB_PO_IN_RS_BK&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInRsBk(data);
	}
	return AddPoInRsBk(data);
}
//---------------------------------------------------------------------------

String					CManagePoInRsBk::GetName(UINT32 nID)
{
	PDB_PO_IN_RS_BK pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsBk::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInRsBkItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsBk::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_BK pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsBk::SetPkt(PDB_PO_IN_RS_BK pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);
	
	SendToken.TokenAdd_32(pdata->nKeepSize);
	SendToken.TokenAdd_32(pdata->nFreeSize);
	SendToken.TokenAdd_32(pdata->nFileMaxSize);
	SendToken.TokenAdd_32(pdata->nFileKeepNum);

	SendToken.TokenAdd_32(pdata->nFExtType);
	SendToken.TokenAdd_String(pdata->strFExtInfo);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsBk::GetPkt(MemToken& RecvToken, DB_PO_IN_RS_BK& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nKeepSize))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nFreeSize))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nFileMaxSize))			goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nFileKeepNum))			goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nFExtType))				goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strFExtInfo) < 0)	goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



