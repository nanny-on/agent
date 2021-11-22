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
#include "ManagePoWinRun.h"

//---------------------------------------------------------------------------

CManagePoWinRun*	t_ManagePoWinRun = NULL;

//---------------------------------------------------------------------------

CManagePoWinRun::CManagePoWinRun()
{
	t_DBMgrPoWinRun	= new CDBMgrPoWinRun();
}
//---------------------------------------------------------------------------

CManagePoWinRun::~CManagePoWinRun()
{
	if(t_DBMgrPoWinRun)	{	delete t_DBMgrPoWinRun;	t_DBMgrPoWinRun = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoWinRun::LoadDBMS()
{
	TListDBPoWinRun tDBPoWinRunList;
    TListDBPoWinRunItor begin, end;
	if(SetER(t_DBMgrPoWinRun->LoadExecute(&tDBPoWinRunList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoWinRunList.begin();	end = tDBPoWinRunList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoWinRun::InitHash()
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

INT32					CManagePoWinRun::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_WIN_RUN pdpwr 			= NULL;
	{
		if( (pdpwr = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_win_run by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,", 
							GetHdrHashInfo(pdpwr).c_str(),
							pdpwr->nMbrUsedMode);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpwr->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoWinRun::AddPoWinRun(DB_PO_WIN_RUN&	dpwr)
{
	if(SetER(t_DBMgrPoWinRun->InsertExecute(&dpwr)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpwr.tDPH.nID, dpwr);
	InitHash(dpwr.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoWinRun::EditPoWinRun(DB_PO_WIN_RUN&	dpwr)
{
	PDB_PO_WIN_RUN pdpwr = FindItem(dpwr.tDPH.nID);
	if(!pdpwr)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoWinRun->UpdateExecute(&dpwr)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpwr.tDPH.nID, dpwr);
	InitHash(dpwr.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoWinRun::DelPoWinRun(UINT32 nID)
{
	PDB_PO_WIN_RUN pdpwr = FindItem(nID);
	if(!pdpwr)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoWinRun->DeleteExecute(pdpwr->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoWinRun::ApplyPoWinRun(DB_PO_WIN_RUN&	dpwr)
{
	if(FindItem(dpwr.tDPH.nID))
	{
		return EditPoWinRun(dpwr);
	}
	return AddPoWinRun(dpwr);
}
//---------------------------------------------------------------------------

String					CManagePoWinRun::GetName(UINT32 nID)
{
	PDB_PO_WIN_RUN pdpwr = FindItem(nID);
    if(!pdpwr)	return "";
    return pdpwr->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoWinRun::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoWinRunItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoWinRun::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_WIN_RUN pdpwr = FindItem(nID);
	if(!pdpwr)	return -1;

	return SetPkt(pdpwr, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoWinRun::SetPkt(PDB_PO_WIN_RUN pdpwr, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpwr->tDPH);

	SendToken.TokenAdd_32(pdpwr->nMbrUsedMode);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoWinRun::GetPkt(MemToken& RecvToken, DB_PO_WIN_RUN& dpwr)
{
	if (!RecvToken.TokenDel_DPH(dpwr.tDPH))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpwr.nMbrUsedMode))		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



