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
#include "ManagePoSelfProtectPkg.h"

//---------------------------------------------------------------------------

CManagePoSelfProtectPkg*	t_ManagePoSelfProtectPkg = NULL;

//---------------------------------------------------------------------------

CManagePoSelfProtectPkg::CManagePoSelfProtectPkg()
{
	t_DBMgrPoSelfProtectPkg	= new CDBMgrPoSelfProtectPkg();
}
//---------------------------------------------------------------------------

CManagePoSelfProtectPkg::~CManagePoSelfProtectPkg()
{
	if(t_DBMgrPoSelfProtectPkg)	{	delete t_DBMgrPoSelfProtectPkg;	t_DBMgrPoSelfProtectPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoSelfProtectPkg::LoadDBMS()
{
	TListDBPoSelfProtectPkg tDBPoSelfProtectPkgList;
    TListDBPoSelfProtectPkgItor begin, end;
	if(SetER(t_DBMgrPoSelfProtectPkg->LoadExecute(&tDBPoSelfProtectPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoSelfProtectPkgList.begin();	end = tDBPoSelfProtectPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::InitPkg()
{
	TMapDBPoSelfProtectPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_SELF_PROTECT pdpsp = t_ManagePoSelfProtect->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpsp)
		{
//			WriteLogE("not find po_self_protect_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpsp->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_SELF_PROTECT_PKG pdpspp 			= NULL;
	{
		if( (pdpspp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_self_protect_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoSelfProtectUnit->GetHash(pdpspp->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::AddPoSelfProtectPkg(DB_PO_SELF_PROTECT_PKG&	dpspp)
{
	if(SetER(t_DBMgrPoSelfProtectPkg->InsertExecute(&dpspp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpspp.tDPH.nID, dpspp);
	AddKeyIDListPkg(&dpspp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::EditPoSelfProtectPkg(DB_PO_SELF_PROTECT_PKG&	dpspp)
{
	PDB_PO_SELF_PROTECT_PKG pdpspp = FindItem(dpspp.tDPH.nID);
	if(!pdpspp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoSelfProtectPkg->UpdateExecute(&dpspp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpspp.tDPH.nID, dpspp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::DelPoSelfProtectPkg(UINT32 nID)
{
	PDB_PO_SELF_PROTECT_PKG pdpspp = FindItem(nID);
	if(!pdpspp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoSelfProtectPkg->DeleteExecute(pdpspp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpspp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_SELF_PROTECT_PKG pdpspp = FindItem(*begin);
		if(pdpspp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoSelfProtectPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
{	
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_HEADER pDPH = GetPoHdr(*begin);
		if(!pDPH || pDPH->nPolicyID != nPolicyID)		continue;

		if(IsMultiUsedUnit(nPolicyID, pDPH->nUnitID) == 0)
		{
			t_ManagePoSelfProtectUnit->DelPoSelfProtectUnit(pDPH->nUnitID);
		}

		DelPoSelfProtectPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoSelfProtectPkg::GetName(UINT32 nID)
{
	PDB_PO_SELF_PROTECT_PKG pdpspp = FindItem(nID);
    if(!pdpspp)	return "";
    return pdpspp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoSelfProtectPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SELF_PROTECT_PKG pdpspp = FindItem(nID);
	if(!pdpspp)	return -1;

	return SetPkt(pdpspp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::SetPkt(PDB_PO_SELF_PROTECT_PKG pdpspp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpspp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::GetPkt(MemToken& RecvToken, DB_PO_SELF_PROTECT_PKG& dpspp)
{
	if (!RecvToken.TokenDel_DPH(dpspp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SELF_PROTECT_PKG pdpspp = FindItem(nID);
	if(!pdpspp)	return -1;

	SetPkt(pdpspp, SendToken);

	return t_ManagePoSelfProtectUnit->SetPkt(pdpspp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



