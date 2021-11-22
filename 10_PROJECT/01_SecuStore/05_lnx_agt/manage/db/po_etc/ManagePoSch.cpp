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
#include "ManagePoSch.h"

//---------------------------------------------------------------------------

CManagePoSch*	t_ManagePoSch = NULL;

//---------------------------------------------------------------------------

CManagePoSch::CManagePoSch()
{
	t_DBMgrPoSch	= new CDBMgrPoSch();
}
//---------------------------------------------------------------------------

CManagePoSch::~CManagePoSch()
{
	if(t_DBMgrPoSch)	{	delete t_DBMgrPoSch;	t_DBMgrPoSch = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoSch::LoadDBMS()
{
	TListDBPoSch tDBPoSchList;
	TListDBPoSchItor begin, end;
	if(SetER(t_DBMgrPoSch->LoadExecute(&tDBPoSchList)))
	{
		return g_nErrRtn;
	}

	begin = tDBPoSchList.begin();	end = tDBPoSchList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSch::InitHash()
{
	t_ManagePoSchPkg->InitPkg();

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

INT32					CManagePoSch::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_SCH pdphps 			= NULL;
	{
		if( (pdphps = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_host_po_sch by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,", GetHdrHashInfo(pdphps).c_str());

		{
			TMapIDItor begin, end;
			begin = pdphps->tDPH.tSubIDMap.begin();	end = pdphps->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoSchPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdphps->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSch::AddPoSch(DB_PO_SCH&	dphps)
{
	if(SetER(t_DBMgrPoSch->InsertExecute(&dphps)))
	{
		return g_nErrRtn;
	}

	AddItem(dphps.tDPH.nID, dphps);
	InitHash(dphps.tDPH.nID);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSch::EditPoSch(DB_PO_SCH&	dphps)
{
	PDB_PO_SCH pdphps = FindItem(dphps.tDPH.nID);
	if(!pdphps)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoSch->UpdateExecute(&dphps)))
	{
		return g_nErrRtn;
	}

	EditItem(dphps.tDPH.nID, dphps);
	InitHash(dphps.tDPH.nID);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSch::DelPoSch(UINT32 nID)
{
	PDB_PO_SCH pdphps = FindItem(nID);
	if(!pdphps)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoSch->DeleteExecute(pdphps->tDPH.nID)))
	{
		return g_nErrRtn;
	}

	DeleteItem(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSch::ApplyPoSch(DB_PO_SCH&	dphps)
{
	if(FindItem(dphps.tDPH.nID))
	{
		return EditPoSch(dphps);
	}
	return AddPoSch(dphps);
}
//---------------------------------------------------------------------------

String					CManagePoSch::GetName(UINT32 nID)
{
	PDB_PO_SCH pdphps = FindItem(nID);
	if(!pdphps)	return "";
	return pdphps->tDPH.strName;
}
//---------------------------------------------------------------------------

UINT32					CManagePoSch::GetCurPolIDBySch(UINT64 nPolicyType)
{
	/*UINT32 nCurTime = GetCurrentDateTimeInt();

	TListID tIDList;
	TListIDItor begin, end;
	t_ManagePoSchUnit->GetItemIDList(tIDList);

	begin = tIDList.begin();
	end   = tIDList.end();

	for(; begin != end; begin++)
	{
		PDB_PO_SCH_UNIT pdphpsu = t_ManagePoSchUnit->FindItem(*begin);
		if(!pdphpsu && pdphpsu->nPolicyType != nPolicyType
			&& pdphpsu->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
			continue;

		if(pdphpsu->nStDate <= nCurTime && pdphpsu->nEdDate >= nCurTime)
			return pdphpsu->nNextPolID;
	}*/

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoSch::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoSchItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSch::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SCH pdphps = FindItem(nID);
	if(!pdphps)	return -1;

	SetPkt(pdphps, SendToken);

	{
		SendToken.TokenAdd_32(pdphps->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdphps->tDPH.tSubIDMap.begin();	end = pdphps->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoSchPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSch::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SCH pdphps = FindItem(nID);
	if(!pdphps)	return -1;

	SetPkt(pdphps, SendToken);

	{
		SendToken.TokenAdd_32(pdphps->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdphps->tDPH.tSubIDMap.begin();	end = pdphps->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoSchPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSch::SetPkt(PDB_PO_SCH pdphps, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdphps->tDPH);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSch::GetPkt(MemToken& RecvToken, DB_PO_SCH& dphps)
{
	if (!RecvToken.TokenDel_DPH(dphps.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



