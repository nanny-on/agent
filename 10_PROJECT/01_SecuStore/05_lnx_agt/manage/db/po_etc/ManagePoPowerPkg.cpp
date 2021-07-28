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
#include "ManagePoPowerPkg.h"

//---------------------------------------------------------------------------

CManagePoPowerPkg*	t_ManagePoPowerPkg = NULL;

//---------------------------------------------------------------------------

CManagePoPowerPkg::CManagePoPowerPkg()
{
	t_DBMgrPoPowerPkg	= new CDBMgrPoPowerPkg();
}
//---------------------------------------------------------------------------

CManagePoPowerPkg::~CManagePoPowerPkg()
{
	if(t_DBMgrPoPowerPkg)	{	delete t_DBMgrPoPowerPkg;	t_DBMgrPoPowerPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoPowerPkg::LoadDBMS()
{
	TListDBPoPowerPkg tDBPoPowerPkgList;
    TListDBPoPowerPkgItor begin, end;
	if(SetER(t_DBMgrPoPowerPkg->LoadExecute(&tDBPoPowerPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoPowerPkgList.begin();	end = tDBPoPowerPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::InitPkg()
{
	INT32 nPolicyID = 0;
	PDB_PO_POWER pDpp = NULL;
	TMapDBPoPowerPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		nPolicyID = begin->second.tDPH.nPolicyID;
		pDpp = t_ManagePoPower->FindItem(nPolicyID);
		if(pDpp == NULL)
		{
//			WriteLogE("not find po_power_pkg information [%d]:po_id[%d]:[%d]", nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pDpp->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
		AddKeyIDList(begin->second.tDPH.nPolicyID, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_POWER_PKG pdppp 			= NULL;
	{
		if( (pdppp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_power_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoPowerUnit->GetHash(pdppp->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::AddPoPowerPkg(DB_PO_POWER_PKG&	dppp)
{
	if(SetER(t_DBMgrPoPowerPkg->InsertExecute(&dppp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dppp.tDPH.nID, dppp);
	AddKeyIDListPkg(&dppp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::EditPoPowerPkg(DB_PO_POWER_PKG&	dppp)
{
	PDB_PO_POWER_PKG pdppp = FindItem(dppp.tDPH.nID);
	if(!pdppp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoPowerPkg->UpdateExecute(&dppp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dppp.tDPH.nID, dppp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::DelPoPowerPkg(UINT32 nID)
{
	PDB_PO_POWER_PKG pdppp = FindItem(nID);
	if(!pdppp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoPowerPkg->DeleteExecute(pdppp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdppp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_POWER_PKG pdppp = FindItem(*begin);
		if(pdppp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoPowerPkg(*begin);
	}

	DelKeyIDList(nPolicyID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoPowerUnit->DelPoPowerUnit(pDPH->nUnitID);
		}

		DelPoPowerPkg(*begin);
	}

	DelKeyIDList(nPolicyID);
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoPowerPkg::GetName(UINT32 nID)
{
	PDB_PO_POWER_PKG pdppp = FindItem(nID);
    if(!pdppp)	return "";
    return pdppp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoPowerPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_POWER_PKG pdppp = FindItem(nID);
	if(!pdppp)	return -1;

	return SetPkt(pdppp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::SetPkt(PDB_PO_POWER_PKG pdppp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdppp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::GetPkt(MemToken& RecvToken, DB_PO_POWER_PKG& dppp)
{
	if (!RecvToken.TokenDel_DPH(dppp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoPowerPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_POWER_PKG pdppp = FindItem(nID);
	if(!pdppp)	return -1;

	SetPkt(pdppp, SendToken);

	return t_ManagePoPowerUnit->SetPkt(pdppp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



