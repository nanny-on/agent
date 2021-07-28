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
#include "ManagePoFeNotifyPkg.h"

//---------------------------------------------------------------------------

CManagePoFeNotifyPkg*	t_ManagePoFeNotifyPkg = NULL;

//---------------------------------------------------------------------------

CManagePoFeNotifyPkg::CManagePoFeNotifyPkg()
{
	t_DBMgrPoFeNotifyPkg	= new CDBMgrPoFeNotifyPkg();
}
//---------------------------------------------------------------------------

CManagePoFeNotifyPkg::~CManagePoFeNotifyPkg()
{
	if(t_DBMgrPoFeNotifyPkg)	{	delete t_DBMgrPoFeNotifyPkg;	t_DBMgrPoFeNotifyPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFeNotifyPkg::LoadDBMS()
{
	TListDBPoFeNotifyPkg tDBPoFeNotifyPkgList;
    TListDBPoFeNotifyPkgItor begin, end;
	if(SetER(t_DBMgrPoFeNotifyPkg->LoadExecute(&tDBPoFeNotifyPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFeNotifyPkgList.begin();	end = tDBPoFeNotifyPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::InitPkg()
{
	TMapDBPoFeNotifyPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FE_NOTIFY pdpfn = t_ManagePoFeNotify->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfn)
		{
//			WriteLogE("not find po_fe_notify_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfn->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FE_NOTIFY_PKG pdpfnp 			= NULL;
	{
		if( (pdpfnp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fe_notify_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
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

INT32					CManagePoFeNotifyPkg::AddPoFeNotifyPkg(DB_PO_FE_NOTIFY_PKG&	dpfnp)
{
	if(SetER(t_DBMgrPoFeNotifyPkg->InsertExecute(&dpfnp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfnp.tDPH.nID, dpfnp);
	AddKeyIDListPkg(&dpfnp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::EditPoFeNotifyPkg(DB_PO_FE_NOTIFY_PKG&	dpfnp)
{
	PDB_PO_FE_NOTIFY_PKG pdpfnp = FindItem(dpfnp.tDPH.nID);
	if(!pdpfnp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFeNotifyPkg->UpdateExecute(&dpfnp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfnp.tDPH.nID, dpfnp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::DelPoFeNotifyPkg(UINT32 nID)
{
	PDB_PO_FE_NOTIFY_PKG pdpfnp = FindItem(nID);
	if(!pdpfnp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFeNotifyPkg->DeleteExecute(pdpfnp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfnp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FE_NOTIFY_PKG pdpfnp = FindItem(*begin);
		if(pdpfnp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoFeNotifyPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
{	
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_HEADER pDPH = GetPoHdr(*begin);
		if(!pDPH || pDPH->nPolicyID != nPolicyID)		continue;

		DelPoFeNotifyPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFeNotifyPkg::GetName(UINT32 nID)
{
	PDB_PO_FE_NOTIFY_PKG pdpfnp = FindItem(nID);
    if(!pdpfnp)	return "";
    return pdpfnp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFeNotifyPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_NOTIFY_PKG pdpfnp = FindItem(nID);
	if(!pdpfnp)	return -1;

	return SetPkt(pdpfnp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::SetPkt(PDB_PO_FE_NOTIFY_PKG pdpfnp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfnp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::GetPkt(MemToken& RecvToken, DB_PO_FE_NOTIFY_PKG& dpfnp)
{
	if (!RecvToken.TokenDel_DPH(dpfnp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFeNotifyPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_NOTIFY_PKG pdpfnp = FindItem(nID);
	if(!pdpfnp)	return -1;

	SetPkt(pdpfnp, SendToken);

	return t_ManageEnvNotifyInfo->SetPkt(pdpfnp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



