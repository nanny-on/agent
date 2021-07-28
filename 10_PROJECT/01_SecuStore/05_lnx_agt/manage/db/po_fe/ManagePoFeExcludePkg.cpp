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
#include "ManagePoFeExcludePkg.h"

//---------------------------------------------------------------------------

CManagePoFeExcludePkg*	t_ManagePoFeExcludePkg = NULL;

//---------------------------------------------------------------------------

CManagePoFeExcludePkg::CManagePoFeExcludePkg()
{
	t_DBMgrPoFeExcludePkg	= new CDBMgrPoFeExcludePkg();
}
//---------------------------------------------------------------------------

CManagePoFeExcludePkg::~CManagePoFeExcludePkg()
{
	if(t_DBMgrPoFeExcludePkg)	{	delete t_DBMgrPoFeExcludePkg;	t_DBMgrPoFeExcludePkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFeExcludePkg::LoadDBMS()
{
	TListDBPoFeExcludePkg tDBPoFeExcludePkgList;
    TListDBPoFeExcludePkgItor begin, end;
	if(SetER(t_DBMgrPoFeExcludePkg->LoadExecute(&tDBPoFeExcludePkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFeExcludePkgList.begin();	end = tDBPoFeExcludePkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::InitPkg()
{
	TMapDBPoFeExcludePkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FE_EXCLUDE pdpfe = t_ManagePoFeExclude->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfe)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_fe_exclude_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfe->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FE_EXCLUDE_PKG pdpfep 			= NULL;
	{
		if( (pdpfep = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_fe_exclude_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoFeExcludeUnit->GetHash(pdpfep->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::AddPoFeExcludePkg(DB_PO_FE_EXCLUDE_PKG&	dpfep)
{
	if(SetER(t_DBMgrPoFeExcludePkg->InsertExecute(&dpfep)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfep.tDPH.nID, dpfep);
	AddKeyIDListPkg(&dpfep);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::EditPoFeExcludePkg(DB_PO_FE_EXCLUDE_PKG&	dpfep)
{
	PDB_PO_FE_EXCLUDE_PKG pdpfep = FindItem(dpfep.tDPH.nID);
	if(!pdpfep)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFeExcludePkg->UpdateExecute(&dpfep)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfep.tDPH.nID, dpfep);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::DelPoFeExcludePkg(UINT32 nID)
{
	PDB_PO_FE_EXCLUDE_PKG pdpfep = FindItem(nID);
	if(!pdpfep)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFeExcludePkg->DeleteExecute(pdpfep->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfep);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FE_EXCLUDE_PKG pdpfep = FindItem(*begin);
		if(pdpfep->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoFeExcludePkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoFeExcludeUnit->DelPoFeExcludeUnit(pDPH->nUnitID);
		}

		DelPoFeExcludePkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFeExcludePkg::GetName(UINT32 nID)
{
	PDB_PO_FE_EXCLUDE_PKG pdpfep = FindItem(nID);
    if(!pdpfep)	return "";
    return pdpfep->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFeExcludePkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_EXCLUDE_PKG pdpfep = FindItem(nID);
	if(!pdpfep)	return -1;

	return SetPkt(pdpfep, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::SetPkt(PDB_PO_FE_EXCLUDE_PKG pdpfep, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfep->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::GetPkt(MemToken& RecvToken, DB_PO_FE_EXCLUDE_PKG& dpfep)
{
	if (!RecvToken.TokenDel_DPH(dpfep.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludePkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_EXCLUDE_PKG pdpfep = FindItem(nID);
	if(!pdpfep)	return -1;

	SetPkt(pdpfep, SendToken);

	return t_ManagePoFeExcludeUnit->SetPkt(pdpfep->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



