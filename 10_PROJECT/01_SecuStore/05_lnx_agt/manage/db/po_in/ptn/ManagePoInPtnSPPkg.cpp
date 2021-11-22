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
#include "ManagePoInPtnSPPkg.h"

//---------------------------------------------------------------------------

CManagePoInPtnSPPkg*	t_ManagePoInPtnSPPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInPtnSPPkg::CManagePoInPtnSPPkg()
{
	t_DBMgrPoInPtnSPPkg	= new CDBMgrPoInPtnSPPkg();
}
//---------------------------------------------------------------------------

CManagePoInPtnSPPkg::~CManagePoInPtnSPPkg()
{
	if(t_DBMgrPoInPtnSPPkg)	{	delete t_DBMgrPoInPtnSPPkg;	t_DBMgrPoInPtnSPPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInPtnSPPkg::LoadDBMS()
{
	TListDBPoInPtnSPPkg tDBPoInPtnSPPkgList;
    TListDBPoInPtnSPPkgItor begin, end;
	if(SetER(t_DBMgrPoInPtnSPPkg->LoadExecute(&tDBPoInPtnSPPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInPtnSPPkgList.begin();	end = tDBPoInPtnSPPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::InitPkg()
{
	TMapDBPoInPtnSPPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_PTN_SP pdata = t_ManagePoInPtnSP->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdata)
		{
//			WriteLogE("not find po_in_ptn_sp_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdata->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_PTN_SP_PKG pdatap 			= NULL;
	{
		if( (pdatap = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_ptn_sp_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		EmptyStr(m_strHashValue);
		m_strHashValue = SPrintf("%u,", pdatap->nChkOrder);

		t_ManagePoInPtnSPUnit->GetHash(pdatap->tDPH.nUnitID, m_strHashValue);		
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::AddPoInPtnSPPkg(DB_PO_IN_PTN_SP_PKG&	datap)
{
	if(SetER(t_DBMgrPoInPtnSPPkg->InsertExecute(&datap)))
    {
    	return g_nErrRtn;
    }

	AddItem(datap.tDPH.nID, datap);
	AddKeyIDListPkg(&datap);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::EditPoInPtnSPPkg(DB_PO_IN_PTN_SP_PKG&	datap)
{
	PDB_PO_IN_PTN_SP_PKG pdatap = FindItem(datap.tDPH.nID);
	if(!pdatap)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInPtnSPPkg->UpdateExecute(&datap)))
    {
    	return g_nErrRtn;
    }

    EditItem(datap.tDPH.nID, datap);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::DelPoInPtnSPPkg(UINT32 nID)
{
	PDB_PO_IN_PTN_SP_PKG pdatap = FindItem(nID);
	if(!pdatap)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInPtnSPPkg->DeleteExecute(pdatap->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdatap);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_PTN_SP_PKG pdatap = FindItem(*begin);
		if(pdatap->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInPtnSPPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoInPtnSPUnit->DelPoInPtnSPUnit(pDPH->nUnitID);
		}

		DelPoInPtnSPPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::GetUnitIDListByChkOrder(UINT32 nPolicyID, TListID& tIDList)
{
	TMapIDList tLMapIDList;

	{
		TMapDBPoInPtnSPPkgItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(begin->second.tDPH.nPolicyID != nPolicyID)	continue;

			TMapIDListItor find = tLMapIDList.find(begin->second.nChkOrder);
			if(find == tLMapIDList.end())
			{
				TListID tTIDList;
				tLMapIDList[begin->second.nChkOrder] = tTIDList;
				find = tLMapIDList.find(begin->second.nChkOrder);
			}

			find->second.push_back(begin->second.tDPH.nUnitID);
		}
	}

	{
		TMapIDListItor begin, end;
		begin = tLMapIDList.begin();	end = tLMapIDList.end();
		for(begin; begin != end; begin++)
		{
			AppandListToList(begin->second, tIDList);	
		}
	}
	return tIDList.size();
}
//---------------------------------------------------------------------------

String					CManagePoInPtnSPPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_PTN_SP_PKG pdatap = FindItem(nID);
    if(!pdatap)	return "";
    return pdatap->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInPtnSPPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_SP_PKG pdatap = FindItem(nID);
	if(!pdatap)	return -1;

	return SetPkt(pdatap, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::SetPkt(PDB_PO_IN_PTN_SP_PKG pdatap, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdatap->tDPH);

	SendToken.TokenAdd_32(pdatap->nChkOrder);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_PTN_SP_PKG& datap)
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

INT32					CManagePoInPtnSPPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_SP_PKG pdatap = FindItem(nID);
	if(!pdatap)	return -1;

	SetPkt(pdatap, SendToken);

	return t_ManagePoInPtnSPUnit->SetPkt(pdatap->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



