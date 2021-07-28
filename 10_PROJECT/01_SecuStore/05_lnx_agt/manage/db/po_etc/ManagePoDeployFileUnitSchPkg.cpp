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
#include "ManagePoDeployFileUnitSchPkg.h"

//---------------------------------------------------------------------------

CManagePoDeployFileUnitSchPkg*	t_ManagePoDeployFileUnitSchPkg = NULL;

//---------------------------------------------------------------------------

CManagePoDeployFileUnitSchPkg::CManagePoDeployFileUnitSchPkg()
{
	t_DBMgrPoDeployFileUnitSchPkg	= new CDBMgrPoDeployFileUnitSchPkg();
}
//---------------------------------------------------------------------------

CManagePoDeployFileUnitSchPkg::~CManagePoDeployFileUnitSchPkg()
{
	if(t_DBMgrPoDeployFileUnitSchPkg)	{	delete t_DBMgrPoDeployFileUnitSchPkg;	t_DBMgrPoDeployFileUnitSchPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoDeployFileUnitSchPkg::LoadDBMS()
{
	TListDBPoDeployFileUnitSchPkg tDBPoDeployFileUnitSchPkgList;
    TListDBPoDeployFileUnitSchPkgItor begin, end;
	if(SetER(t_DBMgrPoDeployFileUnitSchPkg->LoadExecute(&tDBPoDeployFileUnitSchPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoDeployFileUnitSchPkgList.begin();	end = tDBPoDeployFileUnitSchPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::InitPkg()
{
	TMapDBPoDeployFileUnitSchPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_DEPLOY_FILE_UNIT pdpdfu = t_ManagePoDeployFileUnit->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpdfu)
		{
			WriteLogE("not find po_deploy_file_unit_sch_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpdfu->tDPH._add_key_id_value(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_SCH, begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp 			= NULL;
	{
		if( (pdpdfusp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_deploy_file_unit_sch_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoDeployFileSchUnit->GetHash(pdpdfusp->tDPH.nUnitID, EmptyStr(m_strHashValue));
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::AddPoDeployFileUnitSchPkg(DB_PO_DEPLOY_FILE_UNIT_SCH_PKG&	dpdfusp)
{
	if(SetER(t_DBMgrPoDeployFileUnitSchPkg->InsertExecute(&dpdfusp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpdfusp.tDPH.nID, dpdfusp);
	AddKeyIDListPkg(&dpdfusp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::EditPoDeployFileUnitSchPkg(DB_PO_DEPLOY_FILE_UNIT_SCH_PKG&	dpdfusp)
{
	PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp = FindItem(dpdfusp.tDPH.nID);
	if(!pdpdfusp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoDeployFileUnitSchPkg->UpdateExecute(&dpdfusp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpdfusp.tDPH.nID, dpdfusp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::DelPoDeployFileUnitSchPkg(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp = FindItem(nID);
	if(!pdpdfusp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoDeployFileUnitSchPkg->DeleteExecute(pdpdfusp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpdfusp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID)
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

PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG		CManagePoDeployFileUnitSchPkg::FindItemByPUID(UINT32 nPolicyID, UINT32 nUnitID)
{
	TMapDBPoDeployFileUnitSchPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.tDPH.nPolicyID != nPolicyID || begin->second.tDPH.nUnitID != nUnitID)	continue;

		return &(begin->second);
	}
	return NULL;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp = FindItem(*begin);
		if(pdpdfusp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoDeployFileUnitSchPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoDeployFileSchUnit->DelPoDeployFileSchUnit(pDPH->nUnitID);
		}

		DelPoDeployFileUnitSchPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoDeployFileUnitSchPkg::GetName(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp = FindItem(nID);
    if(!pdpdfusp)	return "";
    return pdpdfusp->tDPH.strName;
}
//---------------------------------------------------------------------------

void					CManagePoDeployFileUnitSchPkg::CopyContinueInfo(TListDBPoDeployFileUnitSchPkg& tCurUnitPkgList)
{
	TListDBPoDeployFileUnitSchPkgItor begin, end;
	begin = tCurUnitPkgList.begin();	end	= tCurUnitPkgList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp = FindItem(begin->tDPH.nID);
		if(!pdpdfusp)		continue;

		begin->nScanTime = pdpdfusp->nScanTime;
	}
	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoDeployFileUnitSchPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp = FindItem(nID);
	if(!pdpdfusp)	return -1;

	return SetPkt(pdpdfusp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::SetPkt(PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpdfusp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::GetPkt(MemToken& RecvToken, DB_PO_DEPLOY_FILE_UNIT_SCH_PKG& dpdfusp)
{
	if (!RecvToken.TokenDel_DPH(dpdfusp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitSchPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG pdpdfusp = FindItem(nID);
	if(!pdpdfusp)	return -1;

	SetPkt(pdpdfusp, SendToken);

	return t_ManagePoDeployFileSchUnit->SetPkt(pdpdfusp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



