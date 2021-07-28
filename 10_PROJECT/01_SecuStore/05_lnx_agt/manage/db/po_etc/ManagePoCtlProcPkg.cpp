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
#include "ManagePoCtlProcPkg.h"

//---------------------------------------------------------------------------

CManagePoCtlProcPkg*	t_ManagePoCtlProcPkg = NULL;

//---------------------------------------------------------------------------

CManagePoCtlProcPkg::CManagePoCtlProcPkg()
{
	t_DBMgrPoCtlProcPkg	= new CDBMgrPoCtlProcPkg();
}
//---------------------------------------------------------------------------

CManagePoCtlProcPkg::~CManagePoCtlProcPkg()
{
	if(t_DBMgrPoCtlProcPkg)	{	delete t_DBMgrPoCtlProcPkg;	t_DBMgrPoCtlProcPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoCtlProcPkg::LoadDBMS()
{
	TListDBPoCtlProcPkg tDBPoCtlProcPkgList;
    TListDBPoCtlProcPkgItor begin, end;
	if(SetER(t_DBMgrPoCtlProcPkg->LoadExecute(&tDBPoCtlProcPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoCtlProcPkgList.begin();	end = tDBPoCtlProcPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::InitPkg()
{
	TMapDBPoCtlProcPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_CTL_PROC pdpcp = t_ManagePoCtlProc->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpcp)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_ctl_proc_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpcp->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_CTL_PROC_PKG pdpcpp 			= NULL;
	{
		if( (pdpcpp = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_ctl_proc_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoCtlProcUnit->GetHash(pdpcpp->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::AddPoCtlProcPkg(DB_PO_CTL_PROC_PKG&	dpcpp)
{
	if(SetER(t_DBMgrPoCtlProcPkg->InsertExecute(&dpcpp)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpcpp.tDPH.nID, dpcpp);
	AddKeyIDListPkg(&dpcpp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::EditPoCtlProcPkg(DB_PO_CTL_PROC_PKG&	dpcpp)
{
	PDB_PO_CTL_PROC_PKG pdpcpp = FindItem(dpcpp.tDPH.nID);
	if(!pdpcpp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoCtlProcPkg->UpdateExecute(&dpcpp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpcpp.tDPH.nID, dpcpp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::DelPoCtlProcPkg(UINT32 nID)
{
	PDB_PO_CTL_PROC_PKG pdpcpp = FindItem(nID);
	if(!pdpcpp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoCtlProcPkg->DeleteExecute(pdpcpp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpcpp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_CTL_PROC_PKG pdpcpp = FindItem(*begin);
		if(pdpcpp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoCtlProcPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoCtlProcPkg->DelPoCtlProcPkg(pDPH->nUnitID);
		}

		DelPoCtlProcPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoCtlProcPkg::GetName(UINT32 nID)
{
	PDB_PO_CTL_PROC_PKG pdpcpp = FindItem(nID);
    if(!pdpcpp)	return "";
    return pdpcpp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoCtlProcPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PROC_PKG pdpcpp = FindItem(nID);
	if(!pdpcpp)	return -1;

	return SetPkt(pdpcpp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::SetPkt(PDB_PO_CTL_PROC_PKG pdpcpp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpcpp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::GetPkt(MemToken& RecvToken, DB_PO_CTL_PROC_PKG& dpcpp)
{
	if (!RecvToken.TokenDel_DPH(dpcpp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PROC_PKG pdpcpp = FindItem(nID);
	if(!pdpcpp)	return -1;

	SetPkt(pdpcpp, SendToken);

	return t_ManagePoCtlProcUnit->SetPkt(pdpcpp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



