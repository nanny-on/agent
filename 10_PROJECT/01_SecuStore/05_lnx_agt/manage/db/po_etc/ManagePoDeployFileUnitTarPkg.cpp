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
#include "ManagePoDeployFileUnitTarPkg.h"

//---------------------------------------------------------------------------

CManagePoDeployFileUnitTarPkg*	t_ManagePoDeployFileUnitTarPkg = NULL;

//---------------------------------------------------------------------------

CManagePoDeployFileUnitTarPkg::CManagePoDeployFileUnitTarPkg()
{
	t_DBMgrPoDeployFileUnitTarPkg	= new CDBMgrPoDeployFileUnitTarPkg();
}
//---------------------------------------------------------------------------

CManagePoDeployFileUnitTarPkg::~CManagePoDeployFileUnitTarPkg()
{
	if(t_DBMgrPoDeployFileUnitTarPkg)	{	delete t_DBMgrPoDeployFileUnitTarPkg;	t_DBMgrPoDeployFileUnitTarPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoDeployFileUnitTarPkg::LoadDBMS()
{
	TListDBPoDeployFileUnitTarPkg tDBPoDeployFileUnitTarPkgList;
    TListDBPoDeployFileUnitTarPkgItor begin, end;
	if(SetER(t_DBMgrPoDeployFileUnitTarPkg->LoadExecute(&tDBPoDeployFileUnitTarPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoDeployFileUnitTarPkgList.begin();	end = tDBPoDeployFileUnitTarPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::InitPkg()
{
	TMapDBPoDeployFileUnitTarPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_DEPLOY_FILE_UNIT pdpdfu = t_ManagePoDeployFileUnit->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpdfu)
		{
			WriteLogE("not find po_deploy_file_unit_tar_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpdfu->tDPH._add_key_id_value(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_TAR, begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_DEPLOY_FILE_UNIT_TAR_PKG pdpdfucp 			= NULL;
	{
		if( (pdpdfucp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_deploy_file_unit_tar_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoDeployFileTarUnit->GetHash(pdpdfucp->tDPH.nUnitID, EmptyStr(m_strHashValue));
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::AddPoDeployFileUnitTarPkg(DB_PO_DEPLOY_FILE_UNIT_TAR_PKG&	dpdfucp)
{
	if(SetER(t_DBMgrPoDeployFileUnitTarPkg->InsertExecute(&dpdfucp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpdfucp.tDPH.nID, dpdfucp);
	AddKeyIDListPkg(&dpdfucp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::EditPoDeployFileUnitTarPkg(DB_PO_DEPLOY_FILE_UNIT_TAR_PKG&	dpdfucp)
{
	PDB_PO_DEPLOY_FILE_UNIT_TAR_PKG pdpdfucp = FindItem(dpdfucp.tDPH.nID);
	if(!pdpdfucp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoDeployFileUnitTarPkg->UpdateExecute(&dpdfucp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpdfucp.tDPH.nID, dpdfucp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::DelPoDeployFileUnitTarPkg(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_UNIT_TAR_PKG pdpdfucp = FindItem(nID);
	if(!pdpdfucp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoDeployFileUnitTarPkg->DeleteExecute(pdpdfucp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpdfucp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID)
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

INT32					CManagePoDeployFileUnitTarPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_DEPLOY_FILE_UNIT_TAR_PKG pdpdfucp = FindItem(*begin);
		if(pdpdfucp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoDeployFileUnitTarPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoDeployFileTarUnit->DelPoDeployFileTarUnit(pDPH->nUnitID);
		}

		DelPoDeployFileUnitTarPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoDeployFileUnitTarPkg::GetName(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_UNIT_TAR_PKG pdpdfucp = FindItem(nID);
    if(!pdpdfucp)	return "";
    return pdpdfucp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoDeployFileUnitTarPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_UNIT_TAR_PKG pdpdfucp = FindItem(nID);
	if(!pdpdfucp)	return -1;

	return SetPkt(pdpdfucp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::SetPkt(PDB_PO_DEPLOY_FILE_UNIT_TAR_PKG pdpdfucp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpdfucp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::GetPkt(MemToken& RecvToken, DB_PO_DEPLOY_FILE_UNIT_TAR_PKG& dpdfucp)
{
	if (!RecvToken.TokenDel_DPH(dpdfucp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnitTarPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_UNIT_TAR_PKG pdpdfucp = FindItem(nID);
	if(!pdpdfucp)	return -1;

	SetPkt(pdpdfucp, SendToken);

	return t_ManagePoDeployFileTarUnit->SetPkt(pdpdfucp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



