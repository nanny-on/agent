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
#include "ManagePoNcPtnBLPkg.h"

//---------------------------------------------------------------------------

CManagePoNcPtnBLPkg*	t_ManagePoNcPtnBLPkg = NULL;

//---------------------------------------------------------------------------

CManagePoNcPtnBLPkg::CManagePoNcPtnBLPkg()
{
	t_DBMgrPoNcPtnBLPkg	= new CDBMgrPoNcPtnBLPkg();
}
//---------------------------------------------------------------------------

CManagePoNcPtnBLPkg::~CManagePoNcPtnBLPkg()
{
	if(t_DBMgrPoNcPtnBLPkg)	{	delete t_DBMgrPoNcPtnBLPkg;	t_DBMgrPoNcPtnBLPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoNcPtnBLPkg::LoadDBMS()
{
	TListDBPoNcPtnBLPkg tDBPoNcPtnBLPkgList;
    TListDBPoNcPtnBLPkgItor begin, end;
	if(SetER(t_DBMgrPoNcPtnBLPkg->LoadExecute(&tDBPoNcPtnBLPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoNcPtnBLPkgList.begin();	end = tDBPoNcPtnBLPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::InitPkg()
{
	TMapDBPoNcPtnBLPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_NC_PTN_BL pdata = t_ManagePoNcPtnBL->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdata)
		{
			WriteLogE("not find po_nc_ptn_bl_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdata->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_NC_PTN_BL_PKG pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_nc_ptn_bl_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoNcPtnBLUnit->GetHash(pdata->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::AddPoNcPtnBLPkg(DB_PO_NC_PTN_BL_PKG&	data)
{
	if(SetER(t_DBMgrPoNcPtnBLPkg->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.tDPH.nID, data);
	AddKeyIDListPkg(&data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::EditPoNcPtnBLPkg(DB_PO_NC_PTN_BL_PKG&	data)
{
	PDB_PO_NC_PTN_BL_PKG pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoNcPtnBLPkg->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::DelPoNcPtnBLPkg(UINT32 nID)
{
	PDB_PO_NC_PTN_BL_PKG pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoNcPtnBLPkg->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdata);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_NC_PTN_BL_PKG pdata = FindItem(*begin);
		if(pdata->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoNcPtnBLPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoNcPtnBLUnit->DelPoNcPtnBLUnit(pDPH->nUnitID);
		}

		DelPoNcPtnBLPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoNcPtnBLPkg::GetName(UINT32 nID)
{
	PDB_PO_NC_PTN_BL_PKG pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoNcPtnBLPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_BL_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::SetPkt(PDB_PO_NC_PTN_BL_PKG pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_BL_PKG& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnBLPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_BL_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	return t_ManagePoNcPtnBLUnit->SetPkt(pdata->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



