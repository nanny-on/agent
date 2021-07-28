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
#include "ManagePoFaClear.h"

//---------------------------------------------------------------------------

CManagePoFaClear*	t_ManagePoFaClear = NULL;

//---------------------------------------------------------------------------

CManagePoFaClear::CManagePoFaClear()
{
	t_DBMgrPoFaClear	= new CDBMgrPoFaClear();
}
//---------------------------------------------------------------------------

CManagePoFaClear::~CManagePoFaClear()
{
	if(t_DBMgrPoFaClear)	{	delete t_DBMgrPoFaClear;	t_DBMgrPoFaClear = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaClear::LoadDBMS()
{
	TListDBPoFaClear tDBPoFaClearList;
    TListDBPoFaClearItor begin, end;
	if(SetER(t_DBMgrPoFaClear->LoadExecute(&tDBPoFaClearList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaClearList.begin();	end = tDBPoFaClearList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::InitHash()
{
	t_ManagePoFaClearPkg->InitPkg();
	
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

INT32					CManagePoFaClear::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FA_CLEAR pdpfc 			= NULL;
	{
		if( (pdpfc = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_clear by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,%u,",
							GetHdrHashInfo(pdpfc).c_str(), pdpfc->nMDelDefScanID);

		{
			TMapIDItor begin, end;
			begin = pdpfc->tDPH.tSubIDMap.begin();	end = pdpfc->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoFaClearPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfc->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::AddPoFaClear(DB_PO_FA_CLEAR&	dpfc)
{
	if(SetER(t_DBMgrPoFaClear->InsertExecute(&dpfc)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfc.tDPH.nID, dpfc);
	InitHash(dpfc.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::EditPoFaClear(DB_PO_FA_CLEAR&	dpfc)
{
	PDB_PO_FA_CLEAR pdpfc = FindItem(dpfc.tDPH.nID);
	if(!pdpfc)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaClear->UpdateExecute(&dpfc)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfc.tDPH.nID, dpfc);
	InitHash(dpfc.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::DelPoFaClear(UINT32 nID)
{
	PDB_PO_FA_CLEAR pdpfc = FindItem(nID);
	if(!pdpfc)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaClear->DeleteExecute(pdpfc->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}

INT32					CManagePoFaClear::ClearPoFaClear()
{
	if(SetER(t_DBMgrPoFaClear->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}

//---------------------------------------------------------------------------

INT32					CManagePoFaClear::ApplyPoFaClear(DB_PO_FA_CLEAR&	dpfc)
{
	if(FindItem(dpfc.tDPH.nID))
	{
		return EditPoFaClear(dpfc);
	}
	return AddPoFaClear(dpfc);
}
//---------------------------------------------------------------------------

String					CManagePoFaClear::GetName(UINT32 nID)
{
	PDB_PO_FA_CLEAR pdpfc = FindItem(nID);
    if(!pdpfc)	return "";
    return pdpfc->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::GetHostSysTypePkgListID(TMapID tSubIDMap, TListID& tPkgIDList)
{
	TMapIDItor begin, end;
	begin = tSubIDMap.begin();	end = tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_CLEAR_PKG pdpfcp = t_ManagePoFaClearPkg->FindItem(begin->first);
		if(!pdpfcp)	continue;

		PDB_PO_FA_CLEAR_UNIT pdpfcu = t_ManagePoFaClearUnit->FindItem(pdpfcp->tDPH.nUnitID);
		if(!pdpfcu)	continue;

		if(t_EnvInfo->m_nHostSysType & pdpfcu->tDPH.nTargetOS)
			tPkgIDList.push_back(pdpfcp->tDPH.nID);
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaClearItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_CLEAR pdpfc = FindItem(nID);
	if(!pdpfc)	return -1;

	SetPkt(pdpfc, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpfc->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfc->tDPH.tSubIDMap.begin();	end = pdpfc->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaClearPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_CLEAR pdpfc = FindItem(nID);
	if(!pdpfc)	return -1;

	SetPkt(pdpfc, SendToken);

	{
		SendToken.TokenAdd_32(pdpfc->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfc->tDPH.tSubIDMap.begin();	end = pdpfc->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaClearPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::SetPktHost_EPS(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_CLEAR pdpfc = FindItem(nID);
	if(!pdpfc)	return -1;

	SetPkt(pdpfc, SendToken);

	{
		TListID tPkgIDList;
		GetHostSysTypePkgListID(pdpfc->tDPH.tSubIDMap, tPkgIDList);	// OSType check

		SendToken.TokenAdd_32(tPkgIDList.size());		

		TListIDItor begin, end;
		begin = tPkgIDList.begin();	end = tPkgIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaClearPkg->SetPktHost(*begin, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::SetPkt(PDB_PO_FA_CLEAR pdpfc, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfc->tDPH);

	SendToken.TokenAdd_32(pdpfc->nMDelDefScanID);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClear::GetPkt(MemToken& RecvToken, DB_PO_FA_CLEAR& dpfc)
{
	if (!RecvToken.TokenDel_DPH(dpfc.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfc.nMDelDefScanID))	goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



