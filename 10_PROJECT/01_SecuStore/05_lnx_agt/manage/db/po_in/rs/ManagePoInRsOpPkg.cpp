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
#include "ManagePoInRsOpPkg.h"

//---------------------------------------------------------------------------

CManagePoInRsOpPkg*	t_ManagePoInRsOpPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInRsOpPkg::CManagePoInRsOpPkg()
{
	t_DBMgrPoInRsOpPkg	= new CDBMgrPoInRsOpPkg();
}
//---------------------------------------------------------------------------

CManagePoInRsOpPkg::~CManagePoInRsOpPkg()
{
	if(t_DBMgrPoInRsOpPkg)	{	delete t_DBMgrPoInRsOpPkg;	t_DBMgrPoInRsOpPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInRsOpPkg::LoadDBMS()
{
	TListDBPoInRsOpPkg tDBPoInRsOpPkgList;
    TListDBPoInRsOpPkgItor begin, end;
/*
	if(SetER(t_DBMgrPoInRsOpPkg->LoadExecute(&tDBPoInRsOpPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInRsOpPkgList.begin();	end = tDBPoInRsOpPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
*/
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::InitPkg()
{
	TMapDBPoInRsOpPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_RS_OP pdpfp = t_ManagePoInRsOp->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfp)
		{
			WriteLogE("not find po_in_rs_op_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfp->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_RS_OP_PKG pdpfpp 			= NULL;
	{
		if( (pdpfpp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_rs_op_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		EmptyStr(m_strHashValue);

		t_ManagePoInRsOpUnit->GetHash(pdpfpp->tDPH.nUnitID, m_strHashValue);		
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::AddPoInRsOpPkg(DB_PO_IN_RS_OP_PKG&	dpfpp)
{
	if(SetER(t_DBMgrPoInRsOpPkg->InsertExecute(&dpfpp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfpp.tDPH.nID, dpfpp);
	AddKeyIDListPkg(&dpfpp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::EditPoInRsOpPkg(DB_PO_IN_RS_OP_PKG&	dpfpp)
{
	PDB_PO_IN_RS_OP_PKG pdpfpp = FindItem(dpfpp.tDPH.nID);
	if(!pdpfpp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInRsOpPkg->UpdateExecute(&dpfpp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfpp.tDPH.nID, dpfpp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::DelPoInRsOpPkg(UINT32 nID)
{
	PDB_PO_IN_RS_OP_PKG pdpfpp = FindItem(nID);
	if(!pdpfpp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInRsOpPkg->DeleteExecute(pdpfpp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfpp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_RS_OP_PKG pdpfpp = FindItem(*begin);
		if(pdpfpp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInRsOpPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoInRsOpUnit->DelPoInRsOpUnitWithSubPkg(pDPH->nUnitID);
		}

		DelPoInRsOpPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInRsOpPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_RS_OP_PKG pdpfpp = FindItem(nID);
    if(!pdpfpp)	return "";
    return pdpfpp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInRsOpPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP_PKG pdpfpp = FindItem(nID);
	if(!pdpfpp)	return -1;

	return SetPkt(pdpfpp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::SetPkt(PDB_PO_IN_RS_OP_PKG pdpfpp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfpp->tDPH);
	
	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_RS_OP_PKG& dpfpp)
{
	if (!RecvToken.TokenDel_DPH(dpfpp.tDPH))				goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP_PKG pdpfpp = FindItem(nID);
	if(!pdpfpp)	return -1;

	SetPkt(pdpfpp, SendToken);

	return t_ManagePoInRsOpUnit->SetPktHost(pdpfpp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



