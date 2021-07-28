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
#include "ManagePoFaOpUnitSubPkg.h"

//---------------------------------------------------------------------------

CManagePoFaOpUnitSubPkg*	t_ManagePoFaOpUnitSubPkg = NULL;

//---------------------------------------------------------------------------

CManagePoFaOpUnitSubPkg::CManagePoFaOpUnitSubPkg()
{
	t_DBMgrPoFaOpUnitSubPkg	= new CDBMgrPoFaOpUnitSubPkg();
}
//---------------------------------------------------------------------------

CManagePoFaOpUnitSubPkg::~CManagePoFaOpUnitSubPkg()
{
	if(t_DBMgrPoFaOpUnitSubPkg)	{	delete t_DBMgrPoFaOpUnitSubPkg;	t_DBMgrPoFaOpUnitSubPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaOpUnitSubPkg::LoadDBMS()
{
	TListDBPoFaOpUnitSubPkg tDBPoFaOpUnitSubPkgList;
    TListDBPoFaOpUnitSubPkgItor begin, end;
	if(SetER(t_DBMgrPoFaOpUnitSubPkg->LoadExecute(&tDBPoFaOpUnitSubPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaOpUnitSubPkgList.begin();	end = tDBPoFaOpUnitSubPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::InitPkg()
{
	TMapDBPoFaOpUnitSubPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP_UNIT pdpfou = t_ManagePoFaOpUnit->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfou)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_fa_op_unit_sub_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfou->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_OP_UNIT_SUB_PKG pdpfousp 			= NULL;
	{
		if( (pdpfousp = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_fa_op_unit_sub_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoFaOpSubUnit->GetHash(pdpfousp->tDPH.nUnitID, EmptyStr(m_strHashValue));
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::AddPoFaOpUnitSubPkg(DB_PO_FA_OP_UNIT_SUB_PKG&	dpfousp)
{
	if(SetER(t_DBMgrPoFaOpUnitSubPkg->InsertExecute(&dpfousp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfousp.tDPH.nID, dpfousp);
	AddKeyIDListPkg(&dpfousp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::EditPoFaOpUnitSubPkg(DB_PO_FA_OP_UNIT_SUB_PKG&	dpfousp)
{
	PDB_PO_FA_OP_UNIT_SUB_PKG pdpfousp = FindItem(dpfousp.tDPH.nID);
	if(!pdpfousp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaOpUnitSubPkg->UpdateExecute(&dpfousp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfousp.tDPH.nID, dpfousp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::DelPoFaOpUnitSubPkg(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT_SUB_PKG pdpfousp = FindItem(nID);
	if(!pdpfousp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaOpUnitSubPkg->DeleteExecute(pdpfousp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfousp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::ClearPoFaOpUnitSubPkg()
{
	if(SetER(t_DBMgrPoFaOpUnitSubPkg->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearKeyIDList();
	ClearItem();
	return 0;
}
//---------------------------------------------------------------------------


INT32					CManagePoFaOpUnitSubPkg::GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID)
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

INT32					CManagePoFaOpUnitSubPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP_UNIT_SUB_PKG pdpfousp = FindItem(*begin);
		if(pdpfousp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoFaOpUnitSubPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
{	
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_HEADER pDPH = GetPoHdr(*begin);
		if(!pDPH || pDPH->nPolicyID != nPolicyID)		continue;

		if(IsMultiUsedUnit(nPolicyID, pDPH->nUnitID) == 0 && 
			t_ManagePoFaOpUnitRulPkg->IsMultiUsedUnit(nPolicyID, pDPH->nUnitID) == 0)
		{
			t_ManagePoFaOpSubUnit->DelPoFaOpSubUnit(pDPH->nUnitID);
		}

		DelPoFaOpUnitSubPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::ClearPkgUnit()
{
	INT32 nRetVal = 0;
	nRetVal = t_ManagePoFaOpSubUnit->ClearPoFaOpSubUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 100;
		return nRetVal;
	}
	nRetVal = ClearPoFaOpUnitSubPkg();
	if(nRetVal < 0)
	{
		nRetVal -= 100;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFaOpUnitSubPkg::GetName(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT_SUB_PKG pdpfousp = FindItem(nID);
    if(!pdpfousp)	return "";
    return pdpfousp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaOpUnitSubPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_UNIT_SUB_PKG pdpfousp = FindItem(nID);
	if(!pdpfousp)	return -1;

	return SetPkt(pdpfousp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::SetPkt(PDB_PO_FA_OP_UNIT_SUB_PKG pdpfousp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfousp->tDPH);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::GetPkt(MemToken& RecvToken, DB_PO_FA_OP_UNIT_SUB_PKG& dpfousp)
{
	if (!RecvToken.TokenDel_DPH(dpfousp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_UNIT_SUB_PKG pdpfousp = FindItem(nID);
	if(!pdpfousp)	return -1;

	SetPkt(pdpfousp, SendToken);

	return t_ManagePoFaOpSubUnit->SetPkt(pdpfousp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitSubPkg::SetInotifySubPkg(UINT32 nOpID, UINT32 nSubPkgID)
{
	return 0;
/*
	PDB_PO_FA_OP_UNIT_SUB_PKG pdpfousp = FindItem(nSubPkgID);
	if(!pdpfousp)
		return -1001;

	return t_ManagePoFaOpSubUnit->SetPkt(pdpfousp->tDPH.nUnitID, SendToken);
*/
}




