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
#include "ManagePoInVulnSwPkg.h"

//---------------------------------------------------------------------------

CManagePoInVulnSwPkg*	t_ManagePoInVulnSwPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnSwPkg::CManagePoInVulnSwPkg()
{
	t_DBMgrPoInVulnSwPkg	= new CDBMgrPoInVulnSwPkg();
}
//---------------------------------------------------------------------------

CManagePoInVulnSwPkg::~CManagePoInVulnSwPkg()
{
	if(t_DBMgrPoInVulnSwPkg)	{	delete t_DBMgrPoInVulnSwPkg;	t_DBMgrPoInVulnSwPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnSwPkg::LoadDBMS()
{
	TListDBPoInVulnSwPkg tDBPoInVulnSwPkgList;
    TListDBPoInVulnSwPkgItor begin, end;
	if(SetER(t_DBMgrPoInVulnSwPkg->LoadExecute(&tDBPoInVulnSwPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInVulnSwPkgList.begin();	end = tDBPoInVulnSwPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::InitPkg()
{
	TMapDBPoInVulnSwPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_SW pdata = t_ManagePoInVulnSw->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdata)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_in_vuln_sw_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdata->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_VULN_SW_PKG pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_vuln_sw_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	if(LOTYPEID32(pdata->tDPH.nUnitID) == SS_COMMON_UNIT_TYPE_SITE)
	{
		t_ManagePoInVulnSwUnit->GetHash(pdata->tDPH.nUnitID, strOrgValue);
	}
	else
	{
		strOrgValue = SPrintf("%s,%s,", pdata->strSwChkKey.c_str(), pdata->strSwVersion.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::AddPoInVulnSwPkg(DB_PO_IN_VULN_SW_PKG&	data)
{
	if(SetER(t_DBMgrPoInVulnSwPkg->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.tDPH.nID, data);
	AddKeyIDListPkg(&data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::EditPoInVulnSwPkg(DB_PO_IN_VULN_SW_PKG&	data)
{
	PDB_PO_IN_VULN_SW_PKG pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnSwPkg->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::DelPoInVulnSwPkg(UINT32 nID)
{
	PDB_PO_IN_VULN_SW_PKG pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnSwPkg->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdata);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------
PDB_PO_IN_VULN_SW_PKG	CManagePoInVulnSwPkg::FindPoInVulnSwPkg(UINT32 nPoID, UINT32 nUnitID)
{
	TMapDBPoInVulnSwPkgItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.tDPH.nPolicyID != nPoID)	continue;
		if(begin->second.tDPH.nUnitID != nUnitID)	continue;

		return &(begin->second);
	}
	return NULL;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_SW_PKG pdata = FindItem(*begin);
		if(pdata->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInVulnSwPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoInVulnSwUnit->DelPoInVulnSwUnit(pDPH->nUnitID);
		}

		DelPoInVulnSwPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInVulnSwPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_SW_PKG pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnSwPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_SW_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::SetPkt(PDB_PO_IN_VULN_SW_PKG pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);
	SendToken.TokenAdd_String(pdata->strSwChkKey);
	SendToken.TokenAdd_String(pdata->strSwVersion);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_SW_PKG& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))						goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strSwChkKey) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strSwVersion) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSwPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_SW_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	return t_ManagePoInVulnSwUnit->SetPkt(pdata->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



