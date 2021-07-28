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
#include "ManagePoHostNotifyPkg.h"

//---------------------------------------------------------------------------

CManagePoHostNotifyPkg*	t_ManagePoHostNotifyPkg = NULL;

//---------------------------------------------------------------------------

CManagePoHostNotifyPkg::CManagePoHostNotifyPkg()
{
	t_DBMgrPoHostNotifyPkg	= new CDBMgrPoHostNotifyPkg();
}
//---------------------------------------------------------------------------

CManagePoHostNotifyPkg::~CManagePoHostNotifyPkg()
{
	if(t_DBMgrPoHostNotifyPkg)	{	delete t_DBMgrPoHostNotifyPkg;	t_DBMgrPoHostNotifyPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoHostNotifyPkg::LoadDBMS()
{
	TListDBPoHostNotifyPkg tDBPoHostNotifyPkgList;
    TListDBPoHostNotifyPkgItor begin, end;
	if(SetER(t_DBMgrPoHostNotifyPkg->LoadExecute(&tDBPoHostNotifyPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoHostNotifyPkgList.begin();	end = tDBPoHostNotifyPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::InitPkg()
{
	TMapDBPoHostNotifyPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_HOST_NOTIFY pdpp = t_ManagePoHostNotify->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpp)
		{
//			WriteLogE("not find po_power_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpp->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
		//AddKeyIDList(begin->second.tDPH.nPolicyID, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_HOST_NOTIFY_PKG pdppp 			= NULL;
	{
		if( (pdppp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_power_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		//t_ManagePoHostNotifyUnit->GetHash(pdppp->tDPH.nUnitID, strOrgValue);
		String strItemHash;
		strItemHash = SPrintf("%u,", pdppp->tDPH.nUnitID);
		strOrgValue += strItemHash;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::AddPoHostNotifyPkg(DB_PO_HOST_NOTIFY_PKG&	dphnp)
{
	if(SetER(t_DBMgrPoHostNotifyPkg->InsertExecute(&dphnp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dphnp.tDPH.nID, dphnp);
	AddKeyIDListPkg(&dphnp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::EditPoHostNotifyPkg(DB_PO_HOST_NOTIFY_PKG&	dphnp)
{
	PDB_PO_HOST_NOTIFY_PKG pdppp = FindItem(dphnp.tDPH.nID);
	if(!pdppp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoHostNotifyPkg->UpdateExecute(&dphnp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dphnp.tDPH.nID, dphnp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::DelPoHostNotifyPkg(UINT32 nID)
{
	PDB_PO_HOST_NOTIFY_PKG pdppp = FindItem(nID);
	if(!pdppp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoHostNotifyPkg->DeleteExecute(pdppp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdppp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_HOST_NOTIFY_PKG pdppp = FindItem(*begin);
		if(pdppp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoHostNotifyPkg(*begin);
	}

	DelKeyIDList(nPolicyID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoHostNotifyUnit->DelPoHostNotifyUnit(pDPH->nUnitID);
		}

		DelPoHostNotifyPkg(*begin);
	}

	DelKeyIDList(nPolicyID);
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoHostNotifyPkg::GetName(UINT32 nID)
{
	PDB_PO_HOST_NOTIFY_PKG pdppp = FindItem(nID);
    if(!pdppp)	return "";
    return pdppp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoHostNotifyPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_HOST_NOTIFY_PKG pdppp = FindItem(nID);
	if(!pdppp)	return -1;

	return SetPkt(pdppp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::SetPkt(PDB_PO_HOST_NOTIFY_PKG pdphnp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdphnp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::GetPkt(MemToken& RecvToken, DB_PO_HOST_NOTIFY_PKG& dphnp)
{
	if (!RecvToken.TokenDel_DPH(dphnp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_HOST_NOTIFY_PKG pdppp = FindItem(nID);
	if(!pdppp)	return -1;

	SetPkt(pdppp, SendToken);

	return t_ManagePoHostNotifyUnit->SetPkt(pdppp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



