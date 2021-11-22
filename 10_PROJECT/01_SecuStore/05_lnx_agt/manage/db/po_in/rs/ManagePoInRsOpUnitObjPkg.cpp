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
#include "ManagePoInRsOpUnitObjPkg.h"

//---------------------------------------------------------------------------

CManagePoInRsOpUnitObjPkg*	t_ManagePoInRsOpUnitObjPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInRsOpUnitObjPkg::CManagePoInRsOpUnitObjPkg()
{
	t_DBMgrPoInRsOpUnitObjPkg	= new CDBMgrPoInRsOpUnitObjPkg();
}
//---------------------------------------------------------------------------

CManagePoInRsOpUnitObjPkg::~CManagePoInRsOpUnitObjPkg()
{
	if(t_DBMgrPoInRsOpUnitObjPkg)	{	delete t_DBMgrPoInRsOpUnitObjPkg;	t_DBMgrPoInRsOpUnitObjPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInRsOpUnitObjPkg::LoadDBMS()
{
	TListDBPoInRsOpUnitObjPkg tDBPoInRsOpUnitObjPkgList;
    TListDBPoInRsOpUnitObjPkgItor begin, end;
/*
	if(SetER(t_DBMgrPoInRsOpUnitObjPkg->LoadExecute(&tDBPoInRsOpUnitObjPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInRsOpUnitObjPkgList.begin();	end = tDBPoInRsOpUnitObjPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		//AddKeyIDListPkg(&(*begin));
    }
*/
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::InitPkg()
{
	TMapDBPoInRsOpUnitObjPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_RS_OP_UNIT pdpfou = t_ManagePoInRsOpUnit->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfou)
		{
			WriteLogE("not find po_in_rs_op_unit information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfou->tDPH._add_key_id_value(SS_PO_IN_RS_OP_UNIT_KEY_TYPE_OBJECT, begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_RS_OP_UNIT_OBJ_PKG pdpfouop 			= NULL;
	{
		if( (pdpfouop = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_rs_op_unit_obj_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoInRsOpObjUnit->GetHash(pdpfouop->tDPH.nUnitID, EmptyStr(m_strHashValue));
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::AddPoInRsOpUnitObjPkg(DB_PO_IN_RS_OP_UNIT_OBJ_PKG&	dpfouop)
{
	if(SetER(t_DBMgrPoInRsOpUnitObjPkg->InsertExecute(&dpfouop)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfouop.tDPH.nID, dpfouop);
	AddKeyIDListPkg(&dpfouop);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::EditPoInRsOpUnitObjPkg(DB_PO_IN_RS_OP_UNIT_OBJ_PKG&	dpfouop)
{
	PDB_PO_IN_RS_OP_UNIT_OBJ_PKG pdpfouop = FindItem(dpfouop.tDPH.nID);
	if(!pdpfouop)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInRsOpUnitObjPkg->UpdateExecute(&dpfouop)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfouop.tDPH.nID, dpfouop);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::DelPoInRsOpUnitObjPkg(UINT32 nID)
{
	PDB_PO_IN_RS_OP_UNIT_OBJ_PKG pdpfouop = FindItem(nID);
	if(!pdpfouop)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInRsOpUnitObjPkg->DeleteExecute(pdpfouop->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfouop);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID)
{
	TMapID tPoIDMap;
	TListID tUnitIDList;
	GetPoIDFromPkgList(nUnitID, tUnitIDList);
	
	{
		TListIDItor begin, end;
		begin = tUnitIDList.begin();	end = tUnitIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInRsOpPkg->GetPoIDFromPkgMap(*begin, tPoIDMap);	
		}
	}
	ConvertMapToList(tPoIDMap, tListID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_RS_OP_UNIT_OBJ_PKG pdpfouop = FindItem(*begin);
		if(pdpfouop->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInRsOpUnitObjPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoInRsOpObjUnit->DelPoInRsOpObjUnit(pDPH->nUnitID);
		}

		DelPoInRsOpUnitObjPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInRsOpUnitObjPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_RS_OP_UNIT_OBJ_PKG pdpfouop = FindItem(nID);
    if(!pdpfouop)	return "";
    return pdpfouop->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInRsOpUnitObjPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP_UNIT_OBJ_PKG pdpfouop = FindItem(nID);
	if(!pdpfouop)	return -1;

	return SetPkt(pdpfouop, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::SetPkt(PDB_PO_IN_RS_OP_UNIT_OBJ_PKG pdpfouop, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfouop->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_RS_OP_UNIT_OBJ_PKG& dpfouop)
{
	if (!RecvToken.TokenDel_DPH(dpfouop.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitObjPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP_UNIT_OBJ_PKG pdpfouop = FindItem(nID);
	if(!pdpfouop)	return -1;

	SetPkt(pdpfouop, SendToken);

	return t_ManagePoInRsOpObjUnit->SetPkt(pdpfouop->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



