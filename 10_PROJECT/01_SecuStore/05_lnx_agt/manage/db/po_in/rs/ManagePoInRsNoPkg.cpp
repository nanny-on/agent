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
#include "ManagePoInRsNoPkg.h"

//---------------------------------------------------------------------------

CManagePoInRsNoPkg*	t_ManagePoInRsNoPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInRsNoPkg::CManagePoInRsNoPkg()
{
	t_DBMgrPoInRsNoPkg	= new CDBMgrPoInRsNoPkg();
}
//---------------------------------------------------------------------------

CManagePoInRsNoPkg::~CManagePoInRsNoPkg()
{
	if(t_DBMgrPoInRsNoPkg)	{	delete t_DBMgrPoInRsNoPkg;	t_DBMgrPoInRsNoPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInRsNoPkg::LoadDBMS()
{
	TListDBPoInRsNoPkg tDBPoInRsNoPkgList;
    TListDBPoInRsNoPkgItor begin, end;
/*
	if(SetER(t_DBMgrPoInRsNoPkg->LoadExecute(&tDBPoInRsNoPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInRsNoPkgList.begin();	end = tDBPoInRsNoPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
*/
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::InitPkg()
{
	TMapDBPoInRsNoPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_RS_NO pdata = t_ManagePoInRsNo->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdata)
		{
			WriteLogE("not find po_in_rs_no_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdata->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_RS_NO_PKG pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_rs_no_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strItemHash;
		strItemHash = SPrintf("%u,", pdata->tDPH.nUnitID);
		strOrgValue += strItemHash;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::AddPoInRsNoPkg(DB_PO_IN_RS_NO_PKG&	data)
{
	if(SetER(t_DBMgrPoInRsNoPkg->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.tDPH.nID, data);
	AddKeyIDListPkg(&data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::EditPoInRsNoPkg(DB_PO_IN_RS_NO_PKG&	data)
{
	PDB_PO_IN_RS_NO_PKG pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInRsNoPkg->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::DelPoInRsNoPkg(UINT32 nID)
{
	PDB_PO_IN_RS_NO_PKG pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInRsNoPkg->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdata);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_RS_NO_PKG pdata = FindItem(*begin);
		if(pdata->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInRsNoPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
{	
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_HEADER pDPH = GetPoHdr(*begin);
		if(!pDPH || pDPH->nPolicyID != nPolicyID)		continue;

		DelPoInRsNoPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInRsNoPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_RS_NO_PKG pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInRsNoPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_NO_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::SetPkt(PDB_PO_IN_RS_NO_PKG pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_RS_NO_PKG& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsNoPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_NO_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



