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
#include "ManagePoDeployFilePkg.h"

//---------------------------------------------------------------------------

CManagePoDeployFilePkg*	t_ManagePoDeployFilePkg = NULL;

//---------------------------------------------------------------------------

CManagePoDeployFilePkg::CManagePoDeployFilePkg()
{
	t_DBMgrPoDeployFilePkg	= new CDBMgrPoDeployFilePkg();
}
//---------------------------------------------------------------------------

CManagePoDeployFilePkg::~CManagePoDeployFilePkg()
{
	if(t_DBMgrPoDeployFilePkg)	{	delete t_DBMgrPoDeployFilePkg;	t_DBMgrPoDeployFilePkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoDeployFilePkg::LoadDBMS()
{
	TListDBPoDeployFilePkg tDBPoDeployFilePkgList;
    TListDBPoDeployFilePkgItor begin, end;
	if(SetER(t_DBMgrPoDeployFilePkg->LoadExecute(&tDBPoDeployFilePkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoDeployFilePkgList.begin();	end = tDBPoDeployFilePkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::InitPkg()
{
	TMapDBPoDeployFilePkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_DEPLOY_FILE pdpdf = t_ManagePoDeployFile->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpdf)
		{
//			WriteLogE("not find po_deploy_file_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpdf->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_DEPLOY_FILE_PKG pdpdfp 			= NULL;
	{
		if( (pdpdfp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_deploy_file_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoDeployFileUnit->GetHash(pdpdfp->tDPH.nUnitID, EmptyStr(m_strHashValue));
		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::AddPoDeployFilePkg(DB_PO_DEPLOY_FILE_PKG&	dpdfp)
{
	if(SetER(t_DBMgrPoDeployFilePkg->InsertExecute(&dpdfp)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpdfp.tDPH.nID, dpdfp);
	AddKeyIDListPkg(&dpdfp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::EditPoDeployFilePkg(DB_PO_DEPLOY_FILE_PKG&	dpdfp)
{
	PDB_PO_DEPLOY_FILE_PKG pdpdfp = FindItem(dpdfp.tDPH.nID);
	if(!pdpdfp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoDeployFilePkg->UpdateExecute(&dpdfp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpdfp.tDPH.nID, dpdfp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::DelPoDeployFilePkg(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_PKG pdpdfp = FindItem(nID);
	if(!pdpdfp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoDeployFilePkg->DeleteExecute(pdpdfp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpdfp);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_DEPLOY_FILE_PKG pdpdfp = FindItem(*begin);
		if(pdpdfp->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoDeployFilePkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoDeployFileUnit->DelPoDeployFileUnitWithSubPkg(pDPH->nUnitID);
		}

		DelPoDeployFilePkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoDeployFilePkg::GetName(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_PKG pdpdfp = FindItem(nID);
    if(!pdpdfp)	return "";
    return pdpdfp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoDeployFilePkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_PKG pdpdfp = FindItem(nID);
	if(!pdpdfp)	return -1;

	return SetPkt(pdpdfp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::SetPkt(PDB_PO_DEPLOY_FILE_PKG pdpdfp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpdfp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::GetPkt(MemToken& RecvToken, DB_PO_DEPLOY_FILE_PKG& dpdfp)
{
	if (!RecvToken.TokenDel_DPH(dpdfp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFilePkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_PKG pdpdfp = FindItem(nID);
	if(!pdpdfp)	return -1;

	SetPkt(pdpdfp, SendToken);

	return t_ManagePoDeployFileUnit->SetPkt(pdpdfp->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



