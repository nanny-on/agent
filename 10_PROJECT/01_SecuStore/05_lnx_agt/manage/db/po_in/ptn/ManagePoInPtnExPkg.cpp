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
#include "ManagePoInPtnExPkg.h"

//---------------------------------------------------------------------------

CManagePoInPtnExPkg*	t_ManagePoInPtnExPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInPtnExPkg::CManagePoInPtnExPkg()
{
	t_DBMgrPoInPtnExPkg	= new CDBMgrPoInPtnExPkg();
}
//---------------------------------------------------------------------------

CManagePoInPtnExPkg::~CManagePoInPtnExPkg()
{
	if(t_DBMgrPoInPtnExPkg)	{	delete t_DBMgrPoInPtnExPkg;	t_DBMgrPoInPtnExPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInPtnExPkg::LoadDBMS()
{
	TListDBPoInPtnExPkg tDBPoInPtnExPkgList;
    TListDBPoInPtnExPkgItor begin, end;
	if(SetER(t_DBMgrPoInPtnExPkg->LoadExecute(&tDBPoInPtnExPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInPtnExPkgList.begin();	end = tDBPoInPtnExPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::InitPkg()
{
	TMapDBPoInPtnExPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_PTN_EX pdata = t_ManagePoInPtnEx->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdata)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_in_ptn_ex_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdata->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_PTN_EX_PKG pdatap 			= NULL;
	{
		if( (pdatap = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_ptn_ex_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		m_strHashValue.empty();
		m_strHashValue = SPrintf("%u,", pdatap->nChkOrder);
		t_ManagePoInPtnExUnit->GetHash(pdatap->tDPH.nUnitID, m_strHashValue);
        strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::AddPoInPtnExPkg(DB_PO_IN_PTN_EX_PKG&	datap)
{
	if(SetER(t_DBMgrPoInPtnExPkg->InsertExecute(&datap)))
    {
    	return g_nErrRtn;
    }

	AddItem(datap.tDPH.nID, datap);
	AddKeyIDListPkg(&datap);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::EditPoInPtnExPkg(DB_PO_IN_PTN_EX_PKG&	datap)
{
	PDB_PO_IN_PTN_EX_PKG pdatap = FindItem(datap.tDPH.nID);
	if(!pdatap)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInPtnExPkg->UpdateExecute(&datap)))
    {
    	return g_nErrRtn;
    }

    EditItem(datap.tDPH.nID, datap);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::DelPoInPtnExPkg(UINT32 nID)
{
	PDB_PO_IN_PTN_EX_PKG pdatap = FindItem(nID);
	if(!pdatap)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInPtnExPkg->DeleteExecute(pdatap->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdatap);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_PTN_EX_PKG pdatap = FindItem(*begin);
		if(pdatap->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInPtnExPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoInPtnExUnit->DelPoInPtnExUnit(pDPH->nUnitID);
		}

		DelPoInPtnExPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInPtnExPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_PTN_EX_PKG pdatap = FindItem(nID);
    if(!pdatap)	return "";
    return pdatap->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInPtnExPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_EX_PKG pdatap = FindItem(nID);
	if(!pdatap)	return -1;

	return SetPkt(pdatap, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::SetPkt(PDB_PO_IN_PTN_EX_PKG pdatap, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdatap->tDPH);

	SendToken.TokenAdd_32(pdatap->nChkOrder);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_PTN_EX_PKG& datap)
{
	if (!RecvToken.TokenDel_DPH(datap.tDPH))			goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(datap.nChkOrder))		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_EX_PKG pdatap = FindItem(nID);
	if(!pdatap)	return -1;

	SetPkt(pdatap, SendToken);

	return t_ManagePoInPtnExUnit->SetPkt(pdatap->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



