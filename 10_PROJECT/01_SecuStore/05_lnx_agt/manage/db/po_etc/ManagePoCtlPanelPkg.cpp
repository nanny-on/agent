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
#include "ManagePoCtlPanelPkg.h"

//---------------------------------------------------------------------------

CManagePoCtlPanelPkg*	t_ManagePoCtlPanelPkg = NULL;

//---------------------------------------------------------------------------

CManagePoCtlPanelPkg::CManagePoCtlPanelPkg()
{
	t_DBMgrPoCtlPanelPkg	= new CDBMgrPoCtlPanelPkg();
}
//---------------------------------------------------------------------------

CManagePoCtlPanelPkg::~CManagePoCtlPanelPkg()
{
	if(t_DBMgrPoCtlPanelPkg)	{	delete t_DBMgrPoCtlPanelPkg;	t_DBMgrPoCtlPanelPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoCtlPanelPkg::LoadDBMS()
{
	TListDBPoCtlPanelPkg tDBPoCtlPanelPkgList;
    TListDBPoCtlPanelPkgItor begin, end;
	if(SetER(t_DBMgrPoCtlPanelPkg->LoadExecute(&tDBPoCtlPanelPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoCtlPanelPkgList.begin();	end = tDBPoCtlPanelPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::InitPkg()
{
	TMapDBPoCtlPanelPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_CTL_PANEL pdpcp = t_ManagePoCtlPanel->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpcp)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_ctl_panel_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpcp->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_CTL_PANEL_PKG pdpcpp 			= NULL;
	{
		if( (pdpcpp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_ctl_panel_pkg by hash : [%d][ec:%d]", nID, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoCtlPanelUnit->GetHash(pdpcpp->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::AddPoCtlPanelPkg(DB_PO_CTL_PANEL_PKG&	dpcpp)
{
	if(SetER(t_DBMgrPoCtlPanelPkg->InsertExecute(&dpcpp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpcpp.tDPH.nID, dpcpp);
	AddKeyIDListPkg(&dpcpp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::EditPoCtlPanelPkg(DB_PO_CTL_PANEL_PKG&	dpcpp)
{
	PDB_PO_CTL_PANEL_PKG pdpcpp = FindItem(dpcpp.tDPH.nID);
	if(!pdpcpp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoCtlPanelPkg->UpdateExecute(&dpcpp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpcpp.tDPH.nID, dpcpp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::DelPoCtlPanelPkg(UINT32 nID)
{
	PDB_PO_CTL_PANEL_PKG pdpcpp = FindItem(nID);
	if(!pdpcpp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoCtlPanelPkg->DeleteExecute(pdpcpp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpcpp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_CTL_PANEL_PKG pdpcpp = FindItem(*begin);
		if(pdpcpp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoCtlPanelPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoCtlPanelUnit->DelPoCtlPanelUnit(pDPH->nUnitID);
		}

		DelPoCtlPanelPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoCtlPanelPkg::GetName(UINT32 nID)
{
	PDB_PO_CTL_PANEL_PKG pdpcpp = FindItem(nID);
    if(!pdpcpp)	return "";
    return pdpcpp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoCtlPanelPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PANEL_PKG pdpcpp = FindItem(nID);
	if(!pdpcpp)	return -1;

	return SetPkt(pdpcpp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::SetPkt(PDB_PO_CTL_PANEL_PKG pdpcpp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpcpp->tDPH);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::GetPkt(MemToken& RecvToken, DB_PO_CTL_PANEL_PKG& dpcpp)
{
	if (!RecvToken.TokenDel_DPH(dpcpp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PANEL_PKG pdpcpp = FindItem(nID);
	if(!pdpcpp)	return -1;

	SetPkt(pdpcpp, SendToken);

	return t_ManagePoCtlPanelUnit->SetPkt(pdpcpp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



