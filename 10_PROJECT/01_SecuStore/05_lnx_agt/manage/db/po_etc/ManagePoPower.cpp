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
#include "ManagePoPower.h"

//---------------------------------------------------------------------------

CManagePoPower*	t_ManagePoPower = NULL;

//---------------------------------------------------------------------------

CManagePoPower::CManagePoPower()
{
	t_DBMgrPoPower	= new CDBMgrPoPower();
}
//---------------------------------------------------------------------------

CManagePoPower::~CManagePoPower()
{
	if(t_DBMgrPoPower)	{	delete t_DBMgrPoPower;	t_DBMgrPoPower = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoPower::LoadDBMS()
{
	TListDBPoPower tDBPoPowerList;
    TListDBPoPowerItor begin, end;
	if(SetER(t_DBMgrPoPower->LoadExecute(&tDBPoPowerList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoPowerList.begin();	end = tDBPoPowerList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::InitHash()
{
	t_ManagePoPowerPkg->InitPkg();
	
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

INT32					CManagePoPower::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_POWER pdpp 			= NULL;
	{
		if( (pdpp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_power by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,", 
							GetHdrHashInfo(pdpp).c_str());

		{
			TMapIDItor begin, end;
			begin = pdpp->tDPH.tSubIDMap.begin();	end = pdpp->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoPowerPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpp->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::AddPoPower(DB_PO_POWER&	dpp)
{
	if(SetER(t_DBMgrPoPower->InsertExecute(&dpp)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpp.tDPH.nID, dpp);
	InitHash(dpp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::EditPoPower(DB_PO_POWER&	dpp)
{
	PDB_PO_POWER pdpp = FindItem(dpp.tDPH.nID);
	if(!pdpp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoPower->UpdateExecute(&dpp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpp.tDPH.nID, dpp);
	InitHash(dpp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::DelPoPower(UINT32 nID)
{
	PDB_PO_POWER pdpp = FindItem(nID);
	if(!pdpp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoPower->DeleteExecute(pdpp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::ApplyPoPower(DB_PO_POWER&	dpp)
{
	if(FindItem(dpp.tDPH.nID))
	{
		return EditPoPower(dpp);
	}
	return AddPoPower(dpp);
}
//---------------------------------------------------------------------------

String					CManagePoPower::GetName(UINT32 nID)
{
	PDB_PO_POWER pdpp = FindItem(nID);
    if(!pdpp)	return "";
    return pdpp->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::GetHostSysTypePkgListID(TMapID tSubIDMap, TListID& tPkgIDList)
{
	TMapIDItor begin, end;
	begin = tSubIDMap.begin();	end = tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_POWER_PKG pdppp = t_ManagePoPowerPkg->FindItem(begin->first);
		if(!pdppp)	continue;

		PDB_PO_POWER_UNIT pdppu = t_ManagePoPowerUnit->FindItem(pdppp->tDPH.nUnitID);
		if(!pdppu)	continue;

		if(t_EnvInfo->m_nHostSysType & pdppu->tDPH.nTargetOS)
			tPkgIDList.push_back(pdppp->tDPH.nID);
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoPower::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoPowerItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_POWER pdpp = FindItem(nID);
	if(!pdpp)	return -1;

	SetPkt(pdpp, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpp->tDPH.tSubIDMap.begin();	end = pdpp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoPowerPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_POWER pdpp = FindItem(nID);
	if(!pdpp)	return -1;

	SetPkt(pdpp, SendToken);

	{
		SendToken.TokenAdd_32(pdpp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpp->tDPH.tSubIDMap.begin();	end = pdpp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoPowerPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::SetPktHost_EPS(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_POWER pdpp = FindItem(nID);
	if(!pdpp)	return -1;

	SetPkt(pdpp, SendToken);

	{
		TListID tPkgIDList;
		GetHostSysTypePkgListID(pdpp->tDPH.tSubIDMap, tPkgIDList);	// OSType check

		SendToken.TokenAdd_32(tPkgIDList.size());		

		TListIDItor begin, end;
		begin = tPkgIDList.begin();	end = tPkgIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoPowerPkg->SetPktHost(*begin, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::SetPkt(PDB_PO_POWER pdpp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpp->tDPH);
	SendToken.TokenAdd_32(pdpp->nFastBoot);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPower::GetPkt(MemToken& RecvToken, DB_PO_POWER& dpp)
{
	if (!RecvToken.TokenDel_DPH(dpp.tDPH))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpp.nFastBoot))			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



