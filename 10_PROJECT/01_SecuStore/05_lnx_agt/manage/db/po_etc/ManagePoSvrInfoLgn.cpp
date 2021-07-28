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
#include "ManagePoSvrInfoLgn.h"

//---------------------------------------------------------------------------

CManagePoSvrInfoLgn*	t_ManagePoSvrInfoLgn = NULL;

//---------------------------------------------------------------------------

CManagePoSvrInfoLgn::CManagePoSvrInfoLgn()
{
	t_DBMgrPoSvrInfoLgn	= new CDBMgrPoSvrInfoLgn();
}
//---------------------------------------------------------------------------

CManagePoSvrInfoLgn::~CManagePoSvrInfoLgn()
{
	if(t_DBMgrPoSvrInfoLgn)	{	delete t_DBMgrPoSvrInfoLgn;	t_DBMgrPoSvrInfoLgn = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoSvrInfoLgn::LoadDBMS()
{
	TListDBPoSvrInfoLgn tDBPoSvrInfoLgnList;
    TListDBPoSvrInfoLgnItor begin, end;
	if(SetER(t_DBMgrPoSvrInfoLgn->LoadExecute(&tDBPoSvrInfoLgnList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoSvrInfoLgnList.begin();	end = tDBPoSvrInfoLgnList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoLgn::InitHash()
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

INT32					CManagePoSvrInfoLgn::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_SVR_INFO_LGN pdpsil 			= NULL;
	{
		if( (pdpsil = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_svr_info_lgn by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		pdpsil->tDPH.strHash = pdpsil->strSvrInfoList;
	}

// 	{
// 		char pszHashValue[CHAR_MAX_SIZE] = {0, };
// 		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
// 		pdpsil->tDPH.strHash = pszHashValue;
// 	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoLgn::AddPoSvrInfoLgn(DB_PO_SVR_INFO_LGN&	dpsil)
{
	if(SetER(t_DBMgrPoSvrInfoLgn->InsertExecute(&dpsil)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpsil.tDPH.nID, dpsil);
	InitHash(dpsil.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoLgn::EditPoSvrInfoLgn(DB_PO_SVR_INFO_LGN&	dpsil)
{
	PDB_PO_SVR_INFO_LGN pdpsil = FindItem(dpsil.tDPH.nID);
	if(!pdpsil)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoSvrInfoLgn->UpdateExecute(&dpsil)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpsil.tDPH.nID, dpsil);
	InitHash(dpsil.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoLgn::DelPoSvrInfoLgn(UINT32 nID)
{
	PDB_PO_SVR_INFO_LGN pdpsil = FindItem(nID);
	if(!pdpsil)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoSvrInfoLgn->DeleteExecute(pdpsil->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoLgn::ApplyPoSvrInfoLgn(DB_PO_SVR_INFO_LGN&	dpsil)
{
	if(FindItem(dpsil.tDPH.nID))
	{
		return EditPoSvrInfoLgn(dpsil);
	}
	return AddPoSvrInfoLgn(dpsil);
}
//---------------------------------------------------------------------------

String					CManagePoSvrInfoLgn::GetName(UINT32 nID)
{
	PDB_PO_SVR_INFO_LGN pdpsil = FindItem(nID);
    if(!pdpsil)	return "";
    return pdpsil->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoLgn::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoSvrInfoLgnItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoLgn::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SVR_INFO_LGN pdpsil = FindItem(nID);
	if(!pdpsil)	return -1;

	return SetPkt(pdpsil, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoLgn::SetPkt(PDB_PO_SVR_INFO_LGN pdpsil, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpsil->tDPH);

	SendToken.TokenAdd_String(pdpsil->strSvrInfoList);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoLgn::GetPkt(MemToken& RecvToken, DB_PO_SVR_INFO_LGN& dpsil)
{
	if (!RecvToken.TokenDel_DPH(dpsil.tDPH))							goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dpsil.strSvrInfoList) < 0)			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



