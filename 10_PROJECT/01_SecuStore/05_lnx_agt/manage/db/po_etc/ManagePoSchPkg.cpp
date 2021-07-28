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
#include "ManagePoSchPkg.h"

//---------------------------------------------------------------------------

CManagePoSchPkg*	t_ManagePoSchPkg = NULL;

//---------------------------------------------------------------------------

CManagePoSchPkg::CManagePoSchPkg()
{
	t_DBMgrPoSchPkg	= new CDBMgrPoSchPkg();
}
//---------------------------------------------------------------------------

CManagePoSchPkg::~CManagePoSchPkg()
{
	if(t_DBMgrPoSchPkg)	{	delete t_DBMgrPoSchPkg;	t_DBMgrPoSchPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoSchPkg::LoadDBMS()
{
	TListDBPoSchPkg tDBPoSchPkgList;
	TListDBPoSchPkgItor begin, end;
	if(SetER(t_DBMgrPoSchPkg->LoadExecute(&tDBPoSchPkgList)))
	{
		return g_nErrRtn;
	}

	begin = tDBPoSchPkgList.begin();	end = tDBPoSchPkgList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::InitPkg()
{
	TMapDBPoSchPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_SCH pdphps = t_ManagePoSch->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdphps)
		{
			WriteLogE("not find po_sch_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdphps->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_SCH_PKG pdphpsp 			= NULL;
	{
		if( (pdphpsp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_sch_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}		
	}

	{
		String strItemHash = SPrintf("%u,", pdphpsp->nScanLastTime);
		strOrgValue += strItemHash;
	}

	{
		t_ManagePoSchUnit->GetHash(pdphpsp->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::AddPoSchPkg(DB_PO_SCH_PKG&	dphpsp)
{
	if(SetER(t_DBMgrPoSchPkg->InsertExecute(&dphpsp)))
	{
		return g_nErrRtn;
	}

	AddItem(dphpsp.tDPH.nID, dphpsp);
	AddKeyIDListPkg(&dphpsp);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::EditPoSchPkg(DB_PO_SCH_PKG&	dphpsp)
{
	PDB_PO_SCH_PKG pdphpsp = FindItem(dphpsp.tDPH.nID);
	if(!pdphpsp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoSchPkg->UpdateExecute(&dphpsp)))
	{
		return g_nErrRtn;
	}

	EditItem(dphpsp.tDPH.nID, dphpsp);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::DelPoSchPkg(UINT32 nID)
{
	PDB_PO_SCH_PKG pdphpsp = FindItem(nID);
	if(!pdphpsp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoSchPkg->DeleteExecute(pdphpsp->tDPH.nID)))
	{
		return g_nErrRtn;
	}

	DelKeyIDListPkg(pdphpsp);
	DeleteItem(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_SCH_PKG pdphpsp = FindItem(*begin);
		if(pdphpsp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoSchPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoSchUnit->DelPoSchUnit(pDPH->nUnitID);
		}

		DelPoSchPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoSchPkg::GetName(UINT32 nID)
{
	PDB_PO_SCH_PKG pdphpsp = FindItem(nID);
	if(!pdphpsp)	return "";
	return pdphpsp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoSchPkgItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SCH_PKG pdphpsp = FindItem(nID);
	if(!pdphpsp)	return -1;

	return SetPkt(pdphpsp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::SetPkt(PDB_PO_SCH_PKG pdphpsp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdphpsp->tDPH);

	SendToken.TokenAdd_32(pdphpsp->nScanLastTime);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::GetPkt(MemToken& RecvToken, DB_PO_SCH_PKG& dphpsp)
{
	if (!RecvToken.TokenDel_DPH(dphpsp.tDPH))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dphpsp.nScanLastTime))		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoSchPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SCH_PKG pdphpsp = FindItem(nID);
	if(!pdphpsp)	return -1;

	SetPkt(pdphpsp, SendToken);

	return t_ManagePoSchUnit->SetPkt(pdphpsp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



