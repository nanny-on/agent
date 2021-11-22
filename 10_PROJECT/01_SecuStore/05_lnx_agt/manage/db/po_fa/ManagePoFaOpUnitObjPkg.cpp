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
#include "ManagePoFaOpUnitObjPkg.h"

//---------------------------------------------------------------------------

CManagePoFaOpUnitObjPkg*	t_ManagePoFaOpUnitObjPkg = NULL;

//---------------------------------------------------------------------------

CManagePoFaOpUnitObjPkg::CManagePoFaOpUnitObjPkg()
{
	t_DBMgrPoFaOpUnitObjPkg	= new CDBMgrPoFaOpUnitObjPkg();
}
//---------------------------------------------------------------------------

CManagePoFaOpUnitObjPkg::~CManagePoFaOpUnitObjPkg()
{
	if(t_DBMgrPoFaOpUnitObjPkg)	{	delete t_DBMgrPoFaOpUnitObjPkg;	t_DBMgrPoFaOpUnitObjPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaOpUnitObjPkg::LoadDBMS()
{
	TListDBPoFaOpUnitObjPkg tDBPoFaOpUnitObjPkgList;
    TListDBPoFaOpUnitObjPkgItor begin, end;
	if(SetER(t_DBMgrPoFaOpUnitObjPkg->LoadExecute(&tDBPoFaOpUnitObjPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaOpUnitObjPkgList.begin();	end = tDBPoFaOpUnitObjPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::InitPkg()
{
	TMapDBPoFaOpUnitObjPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP_UNIT pdpfou = t_ManagePoFaOpUnit->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfou)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_fa_op_unit information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfou->tDPH._add_key_id_value(SS_PO_FA_OP_UNIT_KEY_TYPE_OBJECT, begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_OP_UNIT_OBJ_PKG pdpfouop 			= NULL;
	{
		if( (pdpfouop = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_fa_op_unit_obj_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoFaOpObjUnit->GetHash(pdpfouop->tDPH.nUnitID, EmptyStr(m_strHashValue));
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::AddPoFaOpUnitObjPkg(DB_PO_FA_OP_UNIT_OBJ_PKG&	dpfouop)
{
	if(SetER(t_DBMgrPoFaOpUnitObjPkg->InsertExecute(&dpfouop)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfouop.tDPH.nID, dpfouop);
	AddKeyIDListPkg(&dpfouop);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::EditPoFaOpUnitObjPkg(DB_PO_FA_OP_UNIT_OBJ_PKG&	dpfouop)
{
	PDB_PO_FA_OP_UNIT_OBJ_PKG pdpfouop = FindItem(dpfouop.tDPH.nID);
	if(!pdpfouop)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaOpUnitObjPkg->UpdateExecute(&dpfouop)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfouop.tDPH.nID, dpfouop);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::DelPoFaOpUnitObjPkg(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT_OBJ_PKG pdpfouop = FindItem(nID);
	if(!pdpfouop)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaOpUnitObjPkg->DeleteExecute(pdpfouop->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfouop);
    DeleteItem(nID);
    return 0;
}

INT32					CManagePoFaOpUnitObjPkg::ClearPoFaOpUnitObjPkg()
{
	if(SetER(t_DBMgrPoFaOpUnitObjPkg->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearKeyIDList();
	ClearItem();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID)
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

INT32					CManagePoFaOpUnitObjPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP_UNIT_OBJ_PKG pdpfouop = FindItem(*begin);
		if(pdpfouop->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoFaOpUnitObjPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoFaOpObjUnit->DelPoFaOpObjUnit(pDPH->nUnitID);
		}

		DelPoFaOpUnitObjPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::ClearPkgUnit()
{
	INT32 nRetVal = 0;
	nRetVal = t_ManagePoFaOpObjUnit->ClearPoFaOpObjUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}
	nRetVal = ClearPoFaOpUnitObjPkg();
	if(nRetVal < 0)
	{
		nRetVal -= 20;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFaOpUnitObjPkg::GetName(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT_OBJ_PKG pdpfouop = FindItem(nID);
    if(!pdpfouop)	return "";
    return pdpfouop->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaOpUnitObjPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_UNIT_OBJ_PKG pdpfouop = FindItem(nID);
	if(!pdpfouop)	return -1;

	return SetPkt(pdpfouop, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::SetPkt(PDB_PO_FA_OP_UNIT_OBJ_PKG pdpfouop, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfouop->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::GetPkt(MemToken& RecvToken, DB_PO_FA_OP_UNIT_OBJ_PKG& dpfouop)
{
	if (!RecvToken.TokenDel_DPH(dpfouop.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_UNIT_OBJ_PKG pdpfouop = FindItem(nID);
	if(!pdpfouop)	return -1;

	SetPkt(pdpfouop, SendToken);

	return t_ManagePoFaOpObjUnit->SetPkt(pdpfouop->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitObjPkg::SetInotifyObjPkg(UINT32 nPolID, UINT32 nExtOption, UINT32 nObjPkgID)
{
	PDB_PO_FA_OP_UNIT_OBJ_PKG pdpfouop = FindItem(nObjPkgID);
	if(!pdpfouop)
		return -1001;

	return t_ManagePoFaOpObjUnit->SetInotifyObjUnit(nPolID, nExtOption, pdpfouop->tDPH.nUnitID);
}



