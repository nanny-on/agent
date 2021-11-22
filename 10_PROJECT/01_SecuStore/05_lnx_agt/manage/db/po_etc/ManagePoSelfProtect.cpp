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
#include "ManagePoSelfProtect.h"

//---------------------------------------------------------------------------

CManagePoSelfProtect*	t_ManagePoSelfProtect = NULL;

//---------------------------------------------------------------------------

CManagePoSelfProtect::CManagePoSelfProtect()
{
	t_DBMgrPoSelfProtect	= new CDBMgrPoSelfProtect();
}
//---------------------------------------------------------------------------

CManagePoSelfProtect::~CManagePoSelfProtect()
{
	if(t_DBMgrPoSelfProtect)	{	delete t_DBMgrPoSelfProtect;	t_DBMgrPoSelfProtect = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoSelfProtect::LoadDBMS()
{
	TListDBPoSelfProtect tDBPoSelfProtectList;
    TListDBPoSelfProtectItor begin, end;
	if(SetER(t_DBMgrPoSelfProtect->LoadExecute(&tDBPoSelfProtectList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoSelfProtectList.begin();	end = tDBPoSelfProtectList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::InitHash()
{
	t_ManagePoSelfProtectPkg->InitPkg();
	
	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		InitHash(*begin);
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_SELF_PROTECT pdpsp 			= NULL;
	{
		if( (pdpsp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_self_protect by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,%u,", 
							GetHdrHashInfo(pdpsp).c_str(),
							pdpsp->nProtectMode);

		{
			TMapIDItor begin, end;
			begin = pdpsp->tDPH.tSubIDMap.begin();	end = pdpsp->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoSelfProtectPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpsp->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::AddPoSelfProtect(DB_PO_SELF_PROTECT&	dpsp)
{
	if(SetER(t_DBMgrPoSelfProtect->InsertExecute(&dpsp)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpsp.tDPH.nID, dpsp);
	InitHash(dpsp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::EditPoSelfProtect(DB_PO_SELF_PROTECT&	dpsp)
{
	PDB_PO_SELF_PROTECT pdpsp = FindItem(dpsp.tDPH.nID);
	if(!pdpsp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoSelfProtect->UpdateExecute(&dpsp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpsp.tDPH.nID, dpsp);
	InitHash(dpsp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::DelPoSelfProtect(UINT32 nID)
{
	PDB_PO_SELF_PROTECT pdpsp = FindItem(nID);
	if(!pdpsp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoSelfProtect->DeleteExecute(pdpsp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::ApplyPoSelfProtect(DB_PO_SELF_PROTECT&	dpsp)
{
	if(FindItem(dpsp.tDPH.nID))
	{
		return EditPoSelfProtect(dpsp);
	}
	return AddPoSelfProtect(dpsp);
}
//---------------------------------------------------------------------------

String					CManagePoSelfProtect::GetName(UINT32 nID)
{
	PDB_PO_SELF_PROTECT pdpsp = FindItem(nID);
    if(!pdpsp)	return "";
    return pdpsp->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::GetHostSysTypePkgListID(TMapID tSubIDMap, TListID& tPkgIDList)
{
	TMapIDItor begin, end;
	begin = tSubIDMap.begin();	end = tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_SELF_PROTECT_PKG pdpspp = t_ManagePoSelfProtectPkg->FindItem(begin->first);
		if(!pdpspp)	continue;

		PDB_PO_SELF_PROTECT_UNIT pdpspu = t_ManagePoSelfProtectUnit->FindItem(pdpspp->tDPH.nUnitID);
		if(!pdpspu)	continue;

		if(t_EnvInfo->m_nHostSysType & pdpspu->tDPH.nTargetOS)
			tPkgIDList.push_back(pdpspp->tDPH.nID);
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoSelfProtectItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SELF_PROTECT pdpsp = FindItem(nID);
	if(!pdpsp)	return -1;

	SetPkt(pdpsp, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpsp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpsp->tDPH.tSubIDMap.begin();	end = pdpsp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoSelfProtectPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SELF_PROTECT pdpsp = FindItem(nID);
	if(!pdpsp)	return -1;

	SetPkt(pdpsp, SendToken);

	{
		SendToken.TokenAdd_32(pdpsp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpsp->tDPH.tSubIDMap.begin();	end = pdpsp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoSelfProtectPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::SetPktHost_EPS(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SELF_PROTECT pdpsp = FindItem(nID);
	if(!pdpsp)	return -1;

	SetPkt(pdpsp, SendToken);

	{
		TListID tPkgIDList;
		GetHostSysTypePkgListID(pdpsp->tDPH.tSubIDMap, tPkgIDList);	// OSType check

		SendToken.TokenAdd_32(tPkgIDList.size());		

		TListIDItor begin, end;
		begin = tPkgIDList.begin();	end = tPkgIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoSelfProtectPkg->SetPktHost(*begin, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::SetPkt(PDB_PO_SELF_PROTECT pdpsp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpsp->tDPH);
	SendToken.TokenAdd_32(pdpsp->nProtectMode);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtect::GetPkt(MemToken& RecvToken, DB_PO_SELF_PROTECT& dpsp)
{
	if (!RecvToken.TokenDel_DPH(dpsp.tDPH))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpsp.nProtectMode))		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



