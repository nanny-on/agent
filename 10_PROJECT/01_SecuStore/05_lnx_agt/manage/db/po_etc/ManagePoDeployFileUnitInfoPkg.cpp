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
#include "ManagePoDeployFileUnitInfoPkg.h"

//---------------------------------------------------------------------------

CManagePoDeployFileUnitInfoPkg*	t_ManagePoDeployFileUnitInfoPkg = NULL;

//---------------------------------------------------------------------------

CManagePoDeployFileUnitInfoPkg::CManagePoDeployFileUnitInfoPkg()
{
	t_DBMgrPoDeployFileUnitInfoPkg	= new CDBMgrPoDeployFileUnitInfoPkg();
}
//---------------------------------------------------------------------------

CManagePoDeployFileUnitInfoPkg::~CManagePoDeployFileUnitInfoPkg()
{
	if(t_DBMgrPoDeployFileUnitInfoPkg)	{	delete t_DBMgrPoDeployFileUnitInfoPkg;	t_DBMgrPoDeployFileUnitInfoPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoDeployFileUnitInfoPkg::LoadDBMS()
{
	TListDBPoDeployFileUnitInfoPkg tDBPoDeployFileUnitInfoPkgList;
    TListDBPoDeployFileUnitInfoPkgItor begin, end;
	if(SetER(t_DBMgrPoDeployFileUnitInfoPkg->LoadExecute(&tDBPoDeployFileUnitInfoPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoDeployFileUnitInfoPkgList.begin();	end = tDBPoDeployFileUnitInfoPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::InitPkg()
{
	TMapDBPoDeployFileUnitInfoPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_DEPLOY_FILE_UNIT pdpdfu = t_ManagePoDeployFileUnit->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpdfu)
		{
			WriteLogE("not find po_deploy_file_unit_info_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpdfu->tDPH._add_key_id_value(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_INFO, begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pdpdfuip 			= NULL;
	{
		if( (pdpdfuip = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_deploy_file_unit_info_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoDeployFileInfoUnit->GetHash(pdpdfuip->tDPH.nUnitID, EmptyStr(m_strHashValue));
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::AddPoDeployFileUnitInfoPkg(DB_PO_DEPLOY_FILE_UNIT_INFO_PKG&	dpdfuip)
{
	if(SetER(t_DBMgrPoDeployFileUnitInfoPkg->InsertExecute(&dpdfuip)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpdfuip.tDPH.nID, dpdfuip);
	AddKeyIDListPkg(&dpdfuip);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::EditPoDeployFileUnitInfoPkg(DB_PO_DEPLOY_FILE_UNIT_INFO_PKG&	dpdfuip)
{
	PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pdpdfuip = FindItem(dpdfuip.tDPH.nID);
	if(!pdpdfuip)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoDeployFileUnitInfoPkg->UpdateExecute(&dpdfuip)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpdfuip.tDPH.nID, dpdfuip);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::DelPoDeployFileUnitInfoPkg(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pdpdfuip = FindItem(nID);
	if(!pdpdfuip)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoDeployFileUnitInfoPkg->DeleteExecute(pdpdfuip->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpdfuip);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID)
{
	TMapID tPoIDMap;
	TListID tUnitIDList;
	GetPoIDFromPkgList(nUnitID, tUnitIDList);

	{
		TListIDItor begin, end;
		begin = tUnitIDList.begin();	end = tUnitIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoDeployFilePkg->GetPoIDFromPkgMap(*begin, tPoIDMap);	
		}
	}
	ConvertMapToList(tPoIDMap, tListID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pdpdfuip = FindItem(*begin);
		if(pdpdfuip->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoDeployFileUnitInfoPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoDeployFileInfoUnit->DelPoDeployFileInfoUnit(pDPH->nUnitID);
		}

		DelPoDeployFileUnitInfoPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoDeployFileUnitInfoPkg::GetName(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pdpdfuip = FindItem(nID);
    if(!pdpdfuip)	return "";
    return pdpdfuip->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoDeployFileUnitInfoPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pdpdfuip = FindItem(nID);
	if(!pdpdfuip)	return -1;

	return SetPkt(pdpdfuip, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::SetPkt(PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pdpdfuip, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpdfuip->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::GetPkt(MemToken& RecvToken, DB_PO_DEPLOY_FILE_UNIT_INFO_PKG& dpdfuip)
{
	if (!RecvToken.TokenDel_DPH(dpdfuip.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitInfoPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pdpdfuip = FindItem(nID);
	if(!pdpdfuip)	return -1;

	SetPkt(pdpdfuip, SendToken);

	return t_ManagePoDeployFileInfoUnit->SetPkt(pdpdfuip->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



