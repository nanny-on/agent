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
#include "ManagePoInVulnScanPkg.h"

//---------------------------------------------------------------------------

CManagePoInVulnScanPkg*	t_ManagePoInVulnScanPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnScanPkg::CManagePoInVulnScanPkg()
{
	t_DBMgrPoInVulnScanPkg	= new CDBMgrPoInVulnScanPkg();
}
//---------------------------------------------------------------------------

CManagePoInVulnScanPkg::~CManagePoInVulnScanPkg()
{
	if(t_DBMgrPoInVulnScanPkg)	{	delete t_DBMgrPoInVulnScanPkg;	t_DBMgrPoInVulnScanPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnScanPkg::LoadDBMS()
{
	TListDBPoInVulnScanPkg tDBPoInVulnScanPkgList;
    TListDBPoInVulnScanPkgItor begin, end;
	if(SetER(t_DBMgrPoInVulnScanPkg->LoadExecute(&tDBPoInVulnScanPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInVulnScanPkgList.begin();	end = tDBPoInVulnScanPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::InitPkg()
{
	TMapDBPoInVulnScanPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_SCAN pdata = t_ManagePoInVulnScan->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdata)
		{
//			WriteLogE("not find po_in_vuln_scan_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdata->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_VULN_SCAN_PKG pdatap 			= NULL;
	{
		if( (pdatap = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_vuln_scan_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoInVulnScanUnit->GetHash(pdatap->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::AddPoInVulnScanPkg(DB_PO_IN_VULN_SCAN_PKG&	datap)
{
	if(SetER(t_DBMgrPoInVulnScanPkg->InsertExecute(&datap)))
    {
    	return g_nErrRtn;
    }

	AddItem(datap.tDPH.nID, datap);
	AddKeyIDListPkg(&datap);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::EditPoInVulnScanPkg(DB_PO_IN_VULN_SCAN_PKG&	datap)
{
	PDB_PO_IN_VULN_SCAN_PKG pdatap = FindItem(datap.tDPH.nID);
	if(!pdatap)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnScanPkg->UpdateExecute(&datap)))
    {
    	return g_nErrRtn;
    }

    EditItem(datap.tDPH.nID, datap);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::DelPoInVulnScanPkg(UINT32 nID)
{
	PDB_PO_IN_VULN_SCAN_PKG pdatap = FindItem(nID);
	if(!pdatap)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnScanPkg->DeleteExecute(pdatap->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdatap);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_SCAN_PKG pdatap = FindItem(*begin);
		if(pdatap->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInVulnScanPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoInVulnScanUnit->DelPoInVulnScanUnit(pDPH->nUnitID);
		}

		DelPoInVulnScanPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInVulnScanPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_SCAN_PKG pdatap = FindItem(nID);
    if(!pdatap)	return "";
    return pdatap->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnScanPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_SCAN_PKG pdatap = FindItem(nID);
	if(!pdatap)	return -1;

	return SetPkt(pdatap, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::SetPkt(PDB_PO_IN_VULN_SCAN_PKG pdatap, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdatap->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_SCAN_PKG& datap)
{
	if (!RecvToken.TokenDel_DPH(datap.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_SCAN_PKG pdatap = FindItem(nID);
	if(!pdatap)	return -1;

	SetPkt(pdatap, SendToken);

	return t_ManagePoInVulnScanUnit->SetPkt(pdatap->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



