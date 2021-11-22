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
#include "ManagePoFaOpPkg.h"

//---------------------------------------------------------------------------

CManagePoFaOpPkg*	t_ManagePoFaOpPkg = NULL;

//---------------------------------------------------------------------------

CManagePoFaOpPkg::CManagePoFaOpPkg()
{
	t_DBMgrPoFaOpPkg	= new CDBMgrPoFaOpPkg();
}
//---------------------------------------------------------------------------

CManagePoFaOpPkg::~CManagePoFaOpPkg()
{
	if(t_DBMgrPoFaOpPkg)	{	delete t_DBMgrPoFaOpPkg;	t_DBMgrPoFaOpPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaOpPkg::LoadDBMS()
{
	TListDBPoFaOpPkg tDBPoFaOpPkgList;
    TListDBPoFaOpPkgItor begin, end;
	if(SetER(t_DBMgrPoFaOpPkg->LoadExecute(&tDBPoFaOpPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaOpPkgList.begin();	end = tDBPoFaOpPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::InitPkg()
{
	TMapDBPoFaOpPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP pdpfp = t_ManagePoFaOp->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfp)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_fa_op_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfp->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_OP_PKG pdpfpp 			= NULL;
	{
		if( (pdpfpp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_op_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		EmptyStr(m_strHashValue);
		m_strHashValue = SPrintf("%u,", pdpfpp->nRuleOrder);

		t_ManagePoFaOpUnit->GetHash(pdpfpp->tDPH.nUnitID, m_strHashValue);		
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::AddPoFaOpPkg(DB_PO_FA_OP_PKG&	dpfpp)
{
	if(SetER(t_DBMgrPoFaOpPkg->InsertExecute(&dpfpp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfpp.tDPH.nID, dpfpp);
	AddKeyIDListPkg(&dpfpp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::EditPoFaOpPkg(DB_PO_FA_OP_PKG&	dpfpp)
{
	PDB_PO_FA_OP_PKG pdpfpp = FindItem(dpfpp.tDPH.nID);
	if(!pdpfpp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaOpPkg->UpdateExecute(&dpfpp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfpp.tDPH.nID, dpfpp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::DelPoFaOpPkg(UINT32 nID)
{
	PDB_PO_FA_OP_PKG pdpfpp = FindItem(nID);
	if(!pdpfpp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaOpPkg->DeleteExecute(pdpfpp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfpp);
    DeleteItem(nID);
    return 0;
}

INT32					CManagePoFaOpPkg::ClearPoFaOpPkg()
{
	if(SetER(t_DBMgrPoFaOpPkg->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearKeyIDList();
	ClearItem();
	return 0;
}

//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP_PKG pdpfpp = FindItem(*begin);
		if(pdpfpp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoFaOpPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoFaOpUnit->DelPoFaOpUnitWithSubPkg(pDPH->nUnitID);
		}

		DelPoFaOpPkg(*begin);
	}
	return 0;
}

INT32					CManagePoFaOpPkg::ClearAllPkgUnit()
{
	INT32 nRetVal = 0;
	nRetVal = t_ManagePoFaOpUnit->ClearPoFaOpUnitWithSubPkg();
	if(nRetVal < 0)
	{
		nRetVal -= 10000;
		return nRetVal;
	}
	nRetVal = ClearPoFaOpPkg();
	if(nRetVal < 0)
	{
		nRetVal -= 20000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFaOpPkg::GetName(UINT32 nID)
{
	PDB_PO_FA_OP_PKG pdpfpp = FindItem(nID);
    if(!pdpfpp)	return "";
    return pdpfpp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaOpPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_PKG pdpfpp = FindItem(nID);
	if(!pdpfpp)	return -1;

	return SetPkt(pdpfpp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::SetPkt(PDB_PO_FA_OP_PKG pdpfpp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfpp->tDPH);
	SendToken.TokenAdd_32(pdpfpp->nRuleOrder);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::GetPkt(MemToken& RecvToken, DB_PO_FA_OP_PKG& dpfpp)
{
	if (!RecvToken.TokenDel_DPH(dpfpp.tDPH))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpp.nRuleOrder))			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_PKG pdpfpp = FindItem(nID);
	if(!pdpfpp)	return -1;

	SetPkt(pdpfpp, SendToken);

	return t_ManagePoFaOpUnit->SetPktHost(pdpfpp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


INT32					CManagePoFaOpPkg::SetInotifyPkg(UINT32 nPkgID)
{
	PDB_PO_FA_OP_PKG pdpfpp = FindItem(nPkgID);
	if(!pdpfpp)
		return -11;

	return t_ManagePoFaOpUnit->SetInotifyUnit(pdpfpp->tDPH.nUnitID);
}


