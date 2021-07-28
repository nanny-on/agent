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
#include "ManagePoFaClearPkg.h"

//---------------------------------------------------------------------------

CManagePoFaClearPkg*	t_ManagePoFaClearPkg = NULL;

//---------------------------------------------------------------------------

CManagePoFaClearPkg::CManagePoFaClearPkg()
{
	t_DBMgrPoFaClearPkg	= new CDBMgrPoFaClearPkg();
}
//---------------------------------------------------------------------------

CManagePoFaClearPkg::~CManagePoFaClearPkg()
{
	if(t_DBMgrPoFaClearPkg)	{	delete t_DBMgrPoFaClearPkg;	t_DBMgrPoFaClearPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaClearPkg::LoadDBMS()
{
	TListDBPoFaClearPkg tDBPoFaClearPkgList;
    TListDBPoFaClearPkgItor begin, end;
	if(SetER(t_DBMgrPoFaClearPkg->LoadExecute(&tDBPoFaClearPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaClearPkgList.begin();	end = tDBPoFaClearPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::InitPkg()
{
	TMapDBPoFaClearPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_CLEAR pdpfc = t_ManagePoFaClear->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfc)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_fa_clear_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfc->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_CLEAR_PKG pdpfcp 			= NULL;
	{
		if( (pdpfcp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_clear_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoFaClearUnit->GetHash(pdpfcp->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::AddPoFaClearPkg(DB_PO_FA_CLEAR_PKG&	dpfcp)
{
	if(SetER(t_DBMgrPoFaClearPkg->InsertExecute(&dpfcp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfcp.tDPH.nID, dpfcp);
	AddKeyIDListPkg(&dpfcp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::EditPoFaClearPkg(DB_PO_FA_CLEAR_PKG&	dpfcp)
{
	PDB_PO_FA_CLEAR_PKG pdpfcp = FindItem(dpfcp.tDPH.nID);
	if(!pdpfcp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaClearPkg->UpdateExecute(&dpfcp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfcp.tDPH.nID, dpfcp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::DelPoFaClearPkg(UINT32 nID)
{
	PDB_PO_FA_CLEAR_PKG pdpfcp = FindItem(nID);
	if(!pdpfcp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaClearPkg->DeleteExecute(pdpfcp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfcp);
    DeleteItem(nID);
    return 0;
}

INT32					CManagePoFaClearPkg::ClearPoFaClearPkg()
{
	if(SetER(t_DBMgrPoFaClearPkg->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearKeyIDList();
	ClearItem();
	return 0;
}

//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::ClearAllItem()
{
	INT32 nRetVal = 0;
	nRetVal = t_ManagePoFaClearUnit->ClearPoFaClearUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 10000;
		return nRetVal;
	}
	nRetVal = ClearPoFaClearPkg();
	if(nRetVal < 0)
	{
		nRetVal -= 20000;
		return nRetVal;
	}
}

INT32					CManagePoFaClearPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_CLEAR_PKG pdpfcp = FindItem(*begin);
		if(pdpfcp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoFaClearPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoFaClearUnit->DelPoFaClearUnit(pDPH->nUnitID);
		}

		DelPoFaClearPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFaClearPkg::GetName(UINT32 nID)
{
	PDB_PO_FA_CLEAR_PKG pdpfcp = FindItem(nID);
    if(!pdpfcp)	return "";
    return pdpfcp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaClearPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_CLEAR_PKG pdpfcp = FindItem(nID);
	if(!pdpfcp)	return -1;

	return SetPkt(pdpfcp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::SetPkt(PDB_PO_FA_CLEAR_PKG pdpfcp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfcp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::GetPkt(MemToken& RecvToken, DB_PO_FA_CLEAR_PKG& dpfcp)
{
	if (!RecvToken.TokenDel_DPH(dpfcp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaClearPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_CLEAR_PKG pdpfcp = FindItem(nID);
	if(!pdpfcp)	return -1;

	SetPkt(pdpfcp, SendToken);

	return t_ManagePoFaClearUnit->SetPkt(pdpfcp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



