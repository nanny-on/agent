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
#include "ManagePoInVulnEditAppPkg.h"

//---------------------------------------------------------------------------

CManagePoInVulnEditAppPkg*	t_ManagePoInVulnEditAppPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnEditAppPkg::CManagePoInVulnEditAppPkg()
{
	t_DBMgrPoInVulnEditAppPkg	= new CDBMgrPoInVulnEditAppPkg();
}
//---------------------------------------------------------------------------

CManagePoInVulnEditAppPkg::~CManagePoInVulnEditAppPkg()
{
	if(t_DBMgrPoInVulnEditAppPkg)	{	delete t_DBMgrPoInVulnEditAppPkg;	t_DBMgrPoInVulnEditAppPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnEditAppPkg::LoadDBMS()
{
	TListDBPoInVulnEditAppPkg tDBPoInVulnEditAppPkgList;
    TListDBPoInVulnEditAppPkgItor begin, end;
	if(SetER(t_DBMgrPoInVulnEditAppPkg->LoadExecute(&tDBPoInVulnEditAppPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInVulnEditAppPkgList.begin();	end = tDBPoInVulnEditAppPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::InitPkg()
{
	TMapDBPoInVulnEditAppPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_EDIT_APP pdata = t_ManagePoInVulnEditApp->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdata)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_in_vuln_edit_app_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdata->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_VULN_EDIT_APP_PKG pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_in_vuln_edit_app_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	if(LOTYPEID32(pdata->tDPH.nUnitID) == SS_COMMON_UNIT_TYPE_SITE)
	{
		t_ManagePoInVulnEditAppUnit->GetHash(pdata->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::AddPoInVulnEditAppPkg(DB_PO_IN_VULN_EDIT_APP_PKG&	data)
{
	if(SetER(t_DBMgrPoInVulnEditAppPkg->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.tDPH.nID, data);
	AddKeyIDListPkg(&data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::EditPoInVulnEditAppPkg(DB_PO_IN_VULN_EDIT_APP_PKG&	data)
{
	PDB_PO_IN_VULN_EDIT_APP_PKG pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnEditAppPkg->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::DelPoInVulnEditAppPkg(UINT32 nID)
{
	PDB_PO_IN_VULN_EDIT_APP_PKG pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnEditAppPkg->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdata);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_EDIT_APP_PKG pdata = FindItem(*begin);
		if(pdata->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInVulnEditAppPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoInVulnEditAppUnit->DelPoInVulnEditAppUnit(pDPH->nUnitID);
		}

		DelPoInVulnEditAppPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInVulnEditAppPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_EDIT_APP_PKG pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnEditAppPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_EDIT_APP_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::SetPkt(PDB_PO_IN_VULN_EDIT_APP_PKG pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_EDIT_APP_PKG& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_EDIT_APP_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	return t_ManagePoInVulnEditAppUnit->SetPkt(pdata->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



