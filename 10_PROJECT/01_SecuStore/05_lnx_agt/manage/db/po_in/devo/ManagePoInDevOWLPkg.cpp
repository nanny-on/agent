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
#include "ManagePoInDevOWLPkg.h"

//---------------------------------------------------------------------------

CManagePoInDevOWLPkg*	t_ManagePoInDevOWLPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInDevOWLPkg::CManagePoInDevOWLPkg()
{
	t_DBMgrPoInDevOWLPkg	= new CDBMgrPoInDevOWLPkg();
}
//---------------------------------------------------------------------------

CManagePoInDevOWLPkg::~CManagePoInDevOWLPkg()
{
	if(t_DBMgrPoInDevOWLPkg)	{	delete t_DBMgrPoInDevOWLPkg;	t_DBMgrPoInDevOWLPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInDevOWLPkg::LoadDBMS()
{
	TListDBPoInDevOWLPkg tDBPoInDevOWLPkgList;
    TListDBPoInDevOWLPkgItor begin, end;
	if(SetER(t_DBMgrPoInDevOWLPkg->LoadExecute(&tDBPoInDevOWLPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInDevOWLPkgList.begin();	end = tDBPoInDevOWLPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::InitPkg()
{
	TMapDBPoInDevOWLPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_DEVO_WL pdpfn = t_ManagePoInDevOWL->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfn)
		{
//			WriteLogE("not find po_in_devo_wl_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfn->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_DEVO_WL_PKG pdpfnp 			= NULL;
	{
		if( (pdpfnp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_notify_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strItemHash;
		strItemHash = SPrintf("%u,", pdpfnp->tDPH.nUnitID);
		strOrgValue += strItemHash;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::AddPoInDevOWLPkg(DB_PO_IN_DEVO_WL_PKG&	dpfnp)
{
	if(SetER(t_DBMgrPoInDevOWLPkg->InsertExecute(&dpfnp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfnp.tDPH.nID, dpfnp);
	AddKeyIDListPkg(&dpfnp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::EditPoInDevOWLPkg(DB_PO_IN_DEVO_WL_PKG&	dpfnp)
{
	PDB_PO_IN_DEVO_WL_PKG pdpfnp = FindItem(dpfnp.tDPH.nID);
	if(!pdpfnp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInDevOWLPkg->UpdateExecute(&dpfnp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfnp.tDPH.nID, dpfnp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::DelPoInDevOWLPkg(UINT32 nID)
{
	PDB_PO_IN_DEVO_WL_PKG pdpfnp = FindItem(nID);
	if(!pdpfnp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInDevOWLPkg->DeleteExecute(pdpfnp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfnp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_DEVO_WL_PKG pdpfnp = FindItem(*begin);
		if(pdpfnp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInDevOWLPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
{	
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_HEADER pDPH = GetPoHdr(*begin);
		if(!pDPH || pDPH->nPolicyID != nPolicyID)		continue;

		DelPoInDevOWLPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInDevOWLPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_DEVO_WL_PKG pdpfnp = FindItem(nID);
    if(!pdpfnp)	return "";
    return pdpfnp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInDevOWLPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_DEVO_WL_PKG pdpfnp = FindItem(nID);
	if(!pdpfnp)	return -1;

	return SetPkt(pdpfnp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::SetPkt(PDB_PO_IN_DEVO_WL_PKG pdpfnp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfnp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_DEVO_WL_PKG& dpfnp)
{
	if (!RecvToken.TokenDel_DPH(dpfnp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInDevOWLPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_DEVO_WL_PKG pdpfnp = FindItem(nID);
	if(!pdpfnp)	return -1;

	return SetPkt(pdpfnp, SendToken);	
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



