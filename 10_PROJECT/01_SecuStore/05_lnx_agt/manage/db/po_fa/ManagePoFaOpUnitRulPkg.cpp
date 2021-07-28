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
#include "ManagePoFaOpUnitRulPkg.h"

//---------------------------------------------------------------------------

CManagePoFaOpUnitRulPkg*	t_ManagePoFaOpUnitRulPkg = NULL;

//---------------------------------------------------------------------------

CManagePoFaOpUnitRulPkg::CManagePoFaOpUnitRulPkg()
{
	t_DBMgrPoFaOpUnitRulPkg	= new CDBMgrPoFaOpUnitRulPkg();
}
//---------------------------------------------------------------------------

CManagePoFaOpUnitRulPkg::~CManagePoFaOpUnitRulPkg()
{
	if(t_DBMgrPoFaOpUnitRulPkg)	{	delete t_DBMgrPoFaOpUnitRulPkg;	t_DBMgrPoFaOpUnitRulPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaOpUnitRulPkg::LoadDBMS()
{
	TListDBPoFaOpUnitRulPkg tDBPoFaOpUnitRulPkgList;
    TListDBPoFaOpUnitRulPkgItor begin, end;
	if(SetER(t_DBMgrPoFaOpUnitRulPkg->LoadExecute(&tDBPoFaOpUnitRulPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaOpUnitRulPkgList.begin();	end = tDBPoFaOpUnitRulPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::InitPkg()
{
	TMapDBPoFaOpUnitRulPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP_UNIT pdpfou = t_ManagePoFaOpUnit->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfou)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_fa_op_unit_rule_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfou->tDPH._add_key_id_value(SS_PO_FA_OP_UNIT_KEY_TYPE_RULE, begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_OP_UNIT_RUL_PKG pdpfourp 			= NULL;
	{
		if( (pdpfourp = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_fa_op_unit_rul_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		EmptyStr(m_strHashValue);
		m_strHashValue = SPrintf("%u,%u,", pdpfourp->nRuleOrder, pdpfourp->nAcMode);
		
		t_ManagePoFaOpSubUnit->GetHash(pdpfourp->tDPH.nUnitID, m_strHashValue);		
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::AddPoFaOpUnitRulPkg(DB_PO_FA_OP_UNIT_RUL_PKG&	dpfourp)
{
	if(SetER(t_DBMgrPoFaOpUnitRulPkg->InsertExecute(&dpfourp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfourp.tDPH.nID, dpfourp);
	AddKeyIDListPkg(&dpfourp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::EditPoFaOpUnitRulPkg(DB_PO_FA_OP_UNIT_RUL_PKG&	dpfourp)
{
	PDB_PO_FA_OP_UNIT_RUL_PKG pdpfourp = FindItem(dpfourp.tDPH.nID);
	if(!pdpfourp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaOpUnitRulPkg->UpdateExecute(&dpfourp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfourp.tDPH.nID, dpfourp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::DelPoFaOpUnitRulPkg(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT_RUL_PKG pdpfourp = FindItem(nID);
	if(!pdpfourp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaOpUnitRulPkg->DeleteExecute(pdpfourp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfourp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::ClearPoFaOpUnitRulPkg()
{
	if(SetER(t_DBMgrPoFaOpUnitRulPkg->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearKeyIDList();
	ClearItem();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID)
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

INT32					CManagePoFaOpUnitRulPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP_UNIT_RUL_PKG pdpfourp = FindItem(*begin);
		if(pdpfourp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoFaOpUnitRulPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoFaOpSubUnit->IsMultiUsedUnit(nPolicyID, pDPH->nUnitID) == 0)
		{
			t_ManagePoFaOpSubUnit->DelPoFaOpSubUnit(pDPH->nUnitID);
		}

		DelPoFaOpUnitRulPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::ClearPkgUnit()
{	
	INT32 nRetVal = 0;
	nRetVal = t_ManagePoFaOpSubUnit->ClearPoFaOpSubUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 100;
		return nRetVal;
	}
	nRetVal = ClearPoFaOpUnitRulPkg();
	if(nRetVal < 0)
	{
		nRetVal -= 200;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFaOpUnitRulPkg::GetName(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT_RUL_PKG pdpfourp = FindItem(nID);
    if(!pdpfourp)	return "";
    return pdpfourp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaOpUnitRulPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_UNIT_RUL_PKG pdpfourp = FindItem(nID);
	if(!pdpfourp)	return -1;

	return SetPkt(pdpfourp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::SetPkt(PDB_PO_FA_OP_UNIT_RUL_PKG pdpfourp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfourp->tDPH);

	SendToken.TokenAdd_32(pdpfourp->nRuleOrder);
	SendToken.TokenAdd_32(pdpfourp->nAcMode);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::GetPkt(MemToken& RecvToken, DB_PO_FA_OP_UNIT_RUL_PKG& dpfourp)
{
	if (!RecvToken.TokenDel_DPH(dpfourp.tDPH))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfourp.nRuleOrder))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfourp.nAcMode))			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnitRulPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_UNIT_RUL_PKG pdpfourp = FindItem(nID);
	if(!pdpfourp)	return -1;

	SetPkt(pdpfourp, SendToken);

	return t_ManagePoFaOpSubUnit->SetPkt(pdpfourp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



