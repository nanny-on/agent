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
#include "ManagePoInVulnScanUnitPkg.h"

//---------------------------------------------------------------------------

CManagePoInVulnScanUnitPkg*	t_ManagePoInVulnScanUnitPkg = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnScanUnitPkg::CManagePoInVulnScanUnitPkg()
{
	t_DBMgrPoInVulnScanUnitPkg	= new CDBMgrPoInVulnScanUnitPkg();
}
//---------------------------------------------------------------------------

CManagePoInVulnScanUnitPkg::~CManagePoInVulnScanUnitPkg()
{
	if(t_DBMgrPoInVulnScanUnitPkg)	{	delete t_DBMgrPoInVulnScanUnitPkg;	t_DBMgrPoInVulnScanUnitPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnScanUnitPkg::LoadDBMS()
{
	TListDBPoInVulnScanUnitPkg tDBPoInVulnScanUnitPkgList;
    TListDBPoInVulnScanUnitPkgItor begin, end;
	if(SetER(t_DBMgrPoInVulnScanUnitPkg->LoadExecute(&tDBPoInVulnScanUnitPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInVulnScanUnitPkgList.begin();	end = tDBPoInVulnScanUnitPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::InitPkg()
{
	TMapDBPoInVulnScanUnitPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_SCAN_UNIT pdata = t_ManagePoInVulnScanUnit->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdata)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_in_vuln_op_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdata->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_VULN_SCAN_UNIT_PKG pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_vuln_op_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strItemHash;
		strItemHash = SPrintf("%u,"
							"%u,", 
							pdata->tDPH.nUnitID,
							pdata->nSiteScore);
		strOrgValue += strItemHash;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::AddPoInVulnScanUnitPkg(DB_PO_IN_VULN_SCAN_UNIT_PKG&	data)
{
	if(SetER(t_DBMgrPoInVulnScanUnitPkg->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.tDPH.nID, data);
	AddKeyIDListPkg(&data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::EditPoInVulnScanUnitPkg(DB_PO_IN_VULN_SCAN_UNIT_PKG&	data)
{
	PDB_PO_IN_VULN_SCAN_UNIT_PKG pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnScanUnitPkg->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::DelPoInVulnScanUnitPkg(UINT32 nID)
{
	PDB_PO_IN_VULN_SCAN_UNIT_PKG pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnScanUnitPkg->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdata);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_SCAN_UNIT_PKG pdata = FindItem(*begin);
		if(pdata->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoInVulnScanUnitPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
{	
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_HEADER pDPH = GetPoHdr(*begin);
		if(!pDPH || pDPH->nPolicyID != nPolicyID)		continue;

		DelPoInVulnScanUnitPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInVulnScanUnitPkg::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_SCAN_UNIT_PKG pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnScanUnitPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_SCAN_UNIT_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::SetPkt(PDB_PO_IN_VULN_SCAN_UNIT_PKG pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nSiteScore);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_SCAN_UNIT_PKG& data)
{
	if(!RecvToken.TokenDel_DPH(data.tDPH))				goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(data.nSiteScore))			goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnScanUnitPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_SCAN_UNIT_PKG pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);	
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



