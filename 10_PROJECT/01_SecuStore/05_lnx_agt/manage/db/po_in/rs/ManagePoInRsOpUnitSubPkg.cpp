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
#include "ManagePoInRsOpUnitSubPkg.h"

//---------------------------------------------------------------------------

CManagePoInRsOpUnitSubPkg*	t_ManagePoInRsOpUnitSubPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInRsOpUnitSubPkg::CManagePoInRsOpUnitSubPkg()
{
	t_DBMgrPoInRsOpUnitSubPkg	= new CDBMgrPoInRsOpUnitSubPkg();
}
//---------------------------------------------------------------------------

CManagePoInRsOpUnitSubPkg::~CManagePoInRsOpUnitSubPkg()
{
	if(t_DBMgrPoInRsOpUnitSubPkg)	{	delete t_DBMgrPoInRsOpUnitSubPkg;	t_DBMgrPoInRsOpUnitSubPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInRsOpUnitSubPkg::LoadDBMS()
{
	TListDBPoInRsOpUnitSubPkg tDBPoInRsOpUnitSubPkgList;
    TListDBPoInRsOpUnitSubPkgItor begin, end;
/*
	if(SetER(t_DBMgrPoInRsOpUnitSubPkg->LoadExecute(&tDBPoInRsOpUnitSubPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInRsOpUnitSubPkgList.begin();	end = tDBPoInRsOpUnitSubPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		//AddKeyIDListPkg(&(*begin));
    }
*/
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::InitPkg()
{
	TMapDBPoInRsOpUnitSubPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_RS_OP_UNIT pdpfou = t_ManagePoInRsOpUnit->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfou)
		{
			WriteLogE("not find po_in_rs_op_unit information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfou->tDPH._add_key_id_value(SS_PO_IN_RS_OP_UNIT_KEY_TYPE_SUBJECT, begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_RS_OP_UNIT_SUB_PKG pdpfousp 			= NULL;
	{
		if( (pdpfousp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_rs_op_unit_sub_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoInRsOpSubUnit->GetHash(pdpfousp->tDPH.nUnitID, EmptyStr(m_strHashValue));
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::AddPoInRsOpUnitSubPkg(DB_PO_IN_RS_OP_UNIT_SUB_PKG&	dpfousp)
{
	if(SetER(t_DBMgrPoInRsOpUnitSubPkg->InsertExecute(&dpfousp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfousp.tDPH.nID, dpfousp);
	AddKeyIDListPkg(&dpfousp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::EditPoInRsOpUnitSubPkg(DB_PO_IN_RS_OP_UNIT_SUB_PKG&	dpfousp)
{
	PDB_PO_IN_RS_OP_UNIT_SUB_PKG pdpfousp = FindItem(dpfousp.tDPH.nID);
	if(!pdpfousp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInRsOpUnitSubPkg->UpdateExecute(&dpfousp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfousp.tDPH.nID, dpfousp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::DelPoInRsOpUnitSubPkg(UINT32 nID)
{
	PDB_PO_IN_RS_OP_UNIT_SUB_PKG pdpfousp = FindItem(nID);
	if(!pdpfousp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInRsOpUnitSubPkg->DeleteExecute(pdpfousp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfousp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID)
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

INT32					CManagePoInRsOpUnitSubPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_RS_OP_UNIT_SUB_PKG pdpfousp = FindItem(*begin);
		if(pdpfousp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInRsOpUnitSubPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoInRsOpSubUnit->DelPoInRsOpSubUnit(pDPH->nUnitID);
		}

		DelPoInRsOpUnitSubPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInRsOpUnitSubPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_RS_OP_UNIT_SUB_PKG pdpfousp = FindItem(nID);
    if(!pdpfousp)	return "";
    return pdpfousp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInRsOpUnitSubPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP_UNIT_SUB_PKG pdpfousp = FindItem(nID);
	if(!pdpfousp)	return -1;

	return SetPkt(pdpfousp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::SetPkt(PDB_PO_IN_RS_OP_UNIT_SUB_PKG pdpfousp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfousp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_RS_OP_UNIT_SUB_PKG& dpfousp)
{
	if (!RecvToken.TokenDel_DPH(dpfousp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnitSubPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP_UNIT_SUB_PKG pdpfousp = FindItem(nID);
	if(!pdpfousp)	return -1;

	SetPkt(pdpfousp, SendToken);

	return t_ManagePoInRsOpSubUnit->SetPkt(pdpfousp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



