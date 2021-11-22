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
#include "ManagePoFaOpUnitSchPkg.h"

//---------------------------------------------------------------------------

CManagePoFaOpUnitSchPkg*	t_ManagePoFaOpUnitSchPkg = NULL;

//---------------------------------------------------------------------------

CManagePoFaOpUnitSchPkg::CManagePoFaOpUnitSchPkg()
{
	t_DBMgrPoFaOpUnitSchPkg	= new CDBMgrPoFaOpUnitSchPkg();
}
//---------------------------------------------------------------------------

CManagePoFaOpUnitSchPkg::~CManagePoFaOpUnitSchPkg()
{
	if(t_DBMgrPoFaOpUnitSchPkg)	{	delete t_DBMgrPoFaOpUnitSchPkg;	t_DBMgrPoFaOpUnitSchPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaOpUnitSchPkg::LoadDBMS()
{
	TListDBPoFaOpUnitSchPkg tDBPoFaOpUnitSchPkgList;
    TListDBPoFaOpUnitSchPkgItor begin, end;
	if(SetER(t_DBMgrPoFaOpUnitSchPkg->LoadExecute(&tDBPoFaOpUnitSchPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaOpUnitSchPkgList.begin();	end = tDBPoFaOpUnitSchPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::InitPkg()
{
	TMapDBPoFaOpUnitSchPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP_UNIT pdpfou = t_ManagePoFaOpUnit->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfou)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_fa_op_unit_sch_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfou->tDPH._add_key_id_value(SS_PO_FA_OP_UNIT_KEY_TYPE_SCHEDULE, begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_OP_UNIT_SCH_PKG pdpfousp 			= NULL;
	{
		if( (pdpfousp = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_fa_op_unit_sch_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoFaOpSchUnit->GetHash(pdpfousp->tDPH.nUnitID, EmptyStr(m_strHashValue));
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::AddPoFaOpUnitSchPkg(DB_PO_FA_OP_UNIT_SCH_PKG&	dpfousp)
{
	if(SetER(t_DBMgrPoFaOpUnitSchPkg->InsertExecute(&dpfousp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfousp.tDPH.nID, dpfousp);
	AddKeyIDListPkg(&dpfousp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::EditPoFaOpUnitSchPkg(DB_PO_FA_OP_UNIT_SCH_PKG&	dpfousp)
{
	PDB_PO_FA_OP_UNIT_SCH_PKG pdpfousp = FindItem(dpfousp.tDPH.nID);
	if(!pdpfousp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaOpUnitSchPkg->UpdateExecute(&dpfousp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfousp.tDPH.nID, dpfousp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::DelPoFaOpUnitSchPkg(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT_SCH_PKG pdpfousp = FindItem(nID);
	if(!pdpfousp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaOpUnitSchPkg->DeleteExecute(pdpfousp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfousp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::ClearPoFaOpUnitSchPkg()
{
	if(SetER(t_DBMgrPoFaOpUnitSchPkg->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearKeyIDList();
	ClearItem();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID)
{
	TMapID tPoIDMap;
	TListID tUnitIDList;
	GetPoIDFromPkgList(nUnitID, tUnitIDList);

	{
		TListIDItor begin, end;
		begin = tUnitIDList.begin();	end = tUnitIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaOpPkg->GetPoIDFromPkgMap(*begin, tPoIDMap);	
		}
	}
	ConvertMapToList(tPoIDMap, tListID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP_UNIT_SCH_PKG pdpfousp = FindItem(*begin);
		if(pdpfousp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoFaOpUnitSchPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoFaOpSchUnit->DelPoFaOpSchUnit(pDPH->nUnitID);
		}

		DelPoFaOpUnitSchPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::ClearPkgUnit()
{
	INT32 nRetVal = 0;
	nRetVal = t_ManagePoFaOpSchUnit->ClearPoFaOpSchUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 100;
		return nRetVal;
	}
	nRetVal = ClearPoFaOpUnitSchPkg();
	if(nRetVal < 0)
	{
		nRetVal -= 200;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFaOpUnitSchPkg::GetName(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT_SCH_PKG pdpfousp = FindItem(nID);
    if(!pdpfousp)	return "";
    return pdpfousp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaOpUnitSchPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_UNIT_SCH_PKG pdpfousp = FindItem(nID);
	if(!pdpfousp)	return -1;

	return SetPkt(pdpfousp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::SetPkt(PDB_PO_FA_OP_UNIT_SCH_PKG pdpfousp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfousp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::GetPkt(MemToken& RecvToken, DB_PO_FA_OP_UNIT_SCH_PKG& dpfousp)
{
	if (!RecvToken.TokenDel_DPH(dpfousp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSchPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_UNIT_SCH_PKG pdpfousp = FindItem(nID);
	if(!pdpfousp)	return -1;

	SetPkt(pdpfousp, SendToken);

	return t_ManagePoFaOpSchUnit->SetPkt(pdpfousp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



