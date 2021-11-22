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
#include "ManagePoNcPtnExPkg.h"

//---------------------------------------------------------------------------

CManagePoNcPtnExPkg*	t_ManagePoNcPtnExPkg = NULL;

//---------------------------------------------------------------------------

CManagePoNcPtnExPkg::CManagePoNcPtnExPkg()
{
	t_DBMgrPoNcPtnExPkg	= new CDBMgrPoNcPtnExPkg();
}
//---------------------------------------------------------------------------

CManagePoNcPtnExPkg::~CManagePoNcPtnExPkg()
{
	if(t_DBMgrPoNcPtnExPkg)	{	delete t_DBMgrPoNcPtnExPkg;	t_DBMgrPoNcPtnExPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoNcPtnExPkg::LoadDBMS()
{
	TListDBPoNcPtnExPkg tDBPoNcPtnExPkgList;
    TListDBPoNcPtnExPkgItor begin, end;
	if(SetER(t_DBMgrPoNcPtnExPkg->LoadExecute(&tDBPoNcPtnExPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoNcPtnExPkgList.begin();	end = tDBPoNcPtnExPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::InitPkg()
{
	TMapDBPoNcPtnExPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_NC_PTN_EX pdata = t_ManagePoNcPtnEx->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdata)
		{
			WriteLogE("not find po_nc_ptn_ex_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdata->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_NC_PTN_EX_PKG pdatap 			= NULL;
	{
		if( (pdatap = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_nc_ptn_ex_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		EmptyStr(m_strHashValue);
		m_strHashValue = SPrintf("%u,", pdatap->nChkOrder);

		t_ManagePoNcPtnExUnit->GetHash(pdatap->tDPH.nUnitID, m_strHashValue);		
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::AddPoNcPtnExPkg(DB_PO_NC_PTN_EX_PKG&	datap)
{
	if(SetER(t_DBMgrPoNcPtnExPkg->InsertExecute(&datap)))
    {
    	return g_nErrRtn;
    }

	AddItem(datap.tDPH.nID, datap);
	AddKeyIDListPkg(&datap);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::EditPoNcPtnExPkg(DB_PO_NC_PTN_EX_PKG&	datap)
{
	PDB_PO_NC_PTN_EX_PKG pdatap = FindItem(datap.tDPH.nID);
	if(!pdatap)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoNcPtnExPkg->UpdateExecute(&datap)))
    {
    	return g_nErrRtn;
    }

    EditItem(datap.tDPH.nID, datap);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::DelPoNcPtnExPkg(UINT32 nID)
{
	PDB_PO_NC_PTN_EX_PKG pdatap = FindItem(nID);
	if(!pdatap)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoNcPtnExPkg->DeleteExecute(pdatap->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdatap);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_NC_PTN_EX_PKG pdatap = FindItem(*begin);
		if(pdatap->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoNcPtnExPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoNcPtnExUnit->DelPoNcPtnExUnit(pDPH->nUnitID);
		}

		DelPoNcPtnExPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoNcPtnExPkg::GetName(UINT32 nID)
{
	PDB_PO_NC_PTN_EX_PKG pdatap = FindItem(nID);
    if(!pdatap)	return "";
    return pdatap->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoNcPtnExPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_EX_PKG pdatap = FindItem(nID);
	if(!pdatap)	return -1;

	return SetPkt(pdatap, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::SetPkt(PDB_PO_NC_PTN_EX_PKG pdatap, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdatap->tDPH);

	SendToken.TokenAdd_32(pdatap->nChkOrder);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnExPkg::GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_EX_PKG& datap)
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

INT32					CManagePoNcPtnExPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_EX_PKG pdatap = FindItem(nID);
	if(!pdatap)	return -1;

	SetPkt(pdatap, SendToken);

	return t_ManagePoNcPtnExUnit->SetPkt(pdatap->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



