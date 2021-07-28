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
#include "ManagePoInVulnAxPkg.h"

//---------------------------------------------------------------------------

CManagePoInVulnAxPkg*	t_ManagePoInVulnAxPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnAxPkg::CManagePoInVulnAxPkg()
{
	t_DBMgrPoInVulnAxPkg	= new CDBMgrPoInVulnAxPkg();
}
//---------------------------------------------------------------------------

CManagePoInVulnAxPkg::~CManagePoInVulnAxPkg()
{
	if(t_DBMgrPoInVulnAxPkg)	{	delete t_DBMgrPoInVulnAxPkg;	t_DBMgrPoInVulnAxPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnAxPkg::LoadDBMS()
{
	TListDBPoInVulnAxPkg tDBPoInVulnAxPkgList;
    TListDBPoInVulnAxPkgItor begin, end;
	if(SetER(t_DBMgrPoInVulnAxPkg->LoadExecute(&tDBPoInVulnAxPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInVulnAxPkgList.begin();	end = tDBPoInVulnAxPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::InitPkg()
{
	TMapDBPoInVulnAxPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_AX pdata = t_ManagePoInVulnAx->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdata)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_in_vuln_ax_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdata->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_VULN_AX_PKG pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_in_vuln_ax_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoInVulnAxUnit->GetHash(pdata->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::AddPoInVulnAxPkg(DB_PO_IN_VULN_AX_PKG&	data)
{
	if(SetER(t_DBMgrPoInVulnAxPkg->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.tDPH.nID, data);
	AddKeyIDListPkg(&data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::EditPoInVulnAxPkg(DB_PO_IN_VULN_AX_PKG&	data)
{
	PDB_PO_IN_VULN_AX_PKG pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnAxPkg->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::DelPoInVulnAxPkg(UINT32 nID)
{
	PDB_PO_IN_VULN_AX_PKG pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnAxPkg->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdata);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_AX_PKG pdata = FindItem(*begin);
		if(pdata->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInVulnAxPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoInVulnAxUnit->DelPoInVulnAxUnit(pDPH->nUnitID);
		}

		DelPoInVulnAxPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInVulnAxPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_AX_PKG pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnAxPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_AX_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::SetPkt(PDB_PO_IN_VULN_AX_PKG pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_AX_PKG& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_AX_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	return t_ManagePoInVulnAxUnit->SetPkt(pdata->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



