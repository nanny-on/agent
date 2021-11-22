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
#include "ManagePoFaEnvPkg.h"

//---------------------------------------------------------------------------

CManagePoFaEnvPkg*	t_ManagePoFaEnvPkg = NULL;

//---------------------------------------------------------------------------

CManagePoFaEnvPkg::CManagePoFaEnvPkg()
{
	t_DBMgrPoFaEnvPkg	= new CDBMgrPoFaEnvPkg();
}
//---------------------------------------------------------------------------

CManagePoFaEnvPkg::~CManagePoFaEnvPkg()
{
	if(t_DBMgrPoFaEnvPkg)	{	delete t_DBMgrPoFaEnvPkg;	t_DBMgrPoFaEnvPkg = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaEnvPkg::LoadDBMS()
{
	TListDBPoFaEnvPkg tDBPoFaEnvPkgList;
    TListDBPoFaEnvPkgItor begin, end;
	if(SetER(t_DBMgrPoFaEnvPkg->LoadExecute(&tDBPoFaEnvPkgList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaEnvPkgList.begin();	end = tDBPoFaEnvPkgList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->tDPH.nID, *begin);
		AddKeyIDListPkg(&(*begin));
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::InitPkg()
{
	TMapDBPoFaEnvPkgItor begin, end;
	begin = m_tMap.begin();		end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_ENV pdpfe = t_ManagePoFaEnv->FindItem(begin->second.tDPH.nPolicyID);
		if(!pdpfe)
		{
			if(begin->second.tDPH.nPolicyID != 0)
				WriteLogE("not find po_fa_env_pkg information [%d]:po_id[%d]:[%d]", begin->second.tDPH.nPolicyID, begin->first, ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			continue;
		}

		pdpfe->tDPH._add_id_value(begin->first, begin->second.tDPH.nUnitID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_ENV_PKG pdpfep 			= NULL;
	{
		if( (pdpfep = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_env_pkg by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		t_ManagePoFaEnvUnit->GetHash(pdpfep->tDPH.nUnitID, strOrgValue);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::AddPoFaEnvPkg(DB_PO_FA_ENV_PKG&	dpfep)
{
	if(SetER(t_DBMgrPoFaEnvPkg->InsertExecute(&dpfep)))
    {
    	return g_nErrRtn;
    }

	AddItem(dpfep.tDPH.nID, dpfep);
	AddKeyIDListPkg(&dpfep);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::EditPoFaEnvPkg(DB_PO_FA_ENV_PKG&	dpfep)
{
	PDB_PO_FA_ENV_PKG pdpfep = FindItem(dpfep.tDPH.nID);
	if(!pdpfep)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaEnvPkg->UpdateExecute(&dpfep)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfep.tDPH.nID, dpfep);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::DelPoFaEnvPkg(UINT32 nID)
{
	PDB_PO_FA_ENV_PKG pdpfep = FindItem(nID);
	if(!pdpfep)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaEnvPkg->DeleteExecute(pdpfep->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyIDListPkg(pdpfep);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::ClearItemByPolicyID(UINT32 nPolicyID)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_ENV_PKG pdpfep = FindItem(*begin);
		if(pdpfep->tDPH.nPolicyID != nPolicyID)	continue;

		DelPoFaEnvPkg(*begin);
	}
	return 0;
}


INT32					CManagePoFaEnvPkg::ClearPoFaEnvPkg()
{
	if(SetER(t_DBMgrPoFaEnvPkg->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearKeyIDList();
	ClearItem();
	return 0;
}


INT32					CManagePoFaEnvPkg::ClearAllItem()
{
	INT32 nRetVal = 0;
	nRetVal = t_ManagePoFaEnvUnit->ClearPoFaEnvUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 10000;
		return nRetVal;
	}
	nRetVal = ClearPoFaEnvPkg();
	if(nRetVal < 0)
	{
		nRetVal -= 20000;
		return nRetVal;
	}
	return 0;
}

//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::ClearPkgUnitByPolicyID(UINT32 nPolicyID)
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
			t_ManagePoFaEnvUnit->DelPoFaEnvUnit(pDPH->nUnitID);
		}

		DelPoFaEnvPkg(*begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFaEnvPkg::GetName(UINT32 nID)
{
	PDB_PO_FA_ENV_PKG pdpfep = FindItem(nID);
    if(!pdpfep)	return "";
    return pdpfep->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaEnvPkgItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_ENV_PKG pdpfep = FindItem(nID);
	if(!pdpfep)	return -1;

	return SetPkt(pdpfep, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::SetPkt(PDB_PO_FA_ENV_PKG pdpfep, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfep->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::GetPkt(MemToken& RecvToken, DB_PO_FA_ENV_PKG& dpfep)
{
	if (!RecvToken.TokenDel_DPH(dpfep.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvPkg::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_ENV_PKG pdpfep = FindItem(nID);
	if(!pdpfep)	return -1;

	SetPkt(pdpfep, SendToken);

	return t_ManagePoFaEnvUnit->SetPkt(pdpfep->tDPH.nUnitID, SendToken);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



