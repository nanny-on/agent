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
#include "ManagePoInDevOBL.h"

//---------------------------------------------------------------------------

CManagePoInDevOBL*	t_ManagePoInDevOBL = NULL;

//---------------------------------------------------------------------------

CManagePoInDevOBL::CManagePoInDevOBL()
{
	t_DBMgrPoInDevOBL	= new CDBMgrPoInDevOBL();
}
//---------------------------------------------------------------------------

CManagePoInDevOBL::~CManagePoInDevOBL()
{
	if(t_DBMgrPoInDevOBL)	{	delete t_DBMgrPoInDevOBL;	t_DBMgrPoInDevOBL = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInDevOBL::LoadDBMS()
{
	TListDBPoInDevOBL tDBPoInDevOBLList;
    TListDBPoInDevOBLItor begin, end;
	if(SetER(t_DBMgrPoInDevOBL->LoadExecute(&tDBPoInDevOBLList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInDevOBLList.begin();	end = tDBPoInDevOBLList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOBL::InitHash()
{
	t_ManagePoInDevOBLPkg->InitPkg();
	
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

INT32					CManagePoInDevOBL::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_IN_DEVO_BL pdpfn 			= NULL;
	{
		if( (pdpfn = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_devo_bl by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,", 
							GetHdrHashInfo(pdpfn).c_str());

		{
			TMapIDItor begin, end;
			begin = pdpfn->tDPH.tSubIDMap.begin();	end = pdpfn->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoInDevOBLPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfn->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOBL::AddPoInDevOBL(DB_PO_IN_DEVO_BL&	dpfn)
{
	if(SetER(t_DBMgrPoInDevOBL->InsertExecute(&dpfn)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfn.tDPH.nID, dpfn);
	InitHash(dpfn.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOBL::EditPoInDevOBL(DB_PO_IN_DEVO_BL&	dpfn)
{
	PDB_PO_IN_DEVO_BL pdpfn = FindItem(dpfn.tDPH.nID);
	if(!pdpfn)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInDevOBL->UpdateExecute(&dpfn)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfn.tDPH.nID, dpfn);
	InitHash(dpfn.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOBL::DelPoInDevOBL(UINT32 nID)
{
	PDB_PO_IN_DEVO_BL pdpfn = FindItem(nID);
	if(!pdpfn)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInDevOBL->DeleteExecute(pdpfn->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOBL::ApplyPoInDevOBL(DB_PO_IN_DEVO_BL&	dpfn)
{
	if(FindItem(dpfn.tDPH.nID))
	{
		return EditPoInDevOBL(dpfn);
	}
	return AddPoInDevOBL(dpfn);
}
//---------------------------------------------------------------------------

String					CManagePoInDevOBL::GetName(UINT32 nID)
{
	PDB_PO_IN_DEVO_BL pdpfn = FindItem(nID);
    if(!pdpfn)	return "";
    return pdpfn->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInDevOBL::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInDevOBLItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOBL::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_DEVO_BL pdpfn = FindItem(nID);
	if(!pdpfn)	return -1;

	SetPkt(pdpfn, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpfn->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfn->tDPH.tSubIDMap.begin();	end = pdpfn->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInDevOBLPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOBL::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_DEVO_BL pdpfn = FindItem(nID);
	if(!pdpfn)	return -1;

	SetPkt(pdpfn, SendToken);

	{
		SendToken.TokenAdd_32(pdpfn->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfn->tDPH.tSubIDMap.begin();	end = pdpfn->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInDevOBLPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOBL::SetPkt(PDB_PO_IN_DEVO_BL pdpfn, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfn->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOBL::GetPkt(MemToken& RecvToken, DB_PO_IN_DEVO_BL& dpfn)
{
	if(!RecvToken.TokenDel_DPH(dpfn.tDPH))			goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



