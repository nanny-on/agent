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
#include "ManagePoFaOp.h"

//---------------------------------------------------------------------------

CManagePoFaOp*	t_ManagePoFaOp = NULL;

//---------------------------------------------------------------------------

CManagePoFaOp::CManagePoFaOp()
{
	t_DBMgrPoFaOp	= new CDBMgrPoFaOp();
}
//---------------------------------------------------------------------------

CManagePoFaOp::~CManagePoFaOp()
{
	if(t_DBMgrPoFaOp)	{	delete t_DBMgrPoFaOp;	t_DBMgrPoFaOp = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaOp::LoadDBMS()
{
	TListDBPoFaOp tDBPoFaOpList;
    TListDBPoFaOpItor begin, end;
	if(SetER(t_DBMgrPoFaOp->LoadExecute(&tDBPoFaOpList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaOpList.begin();	end = tDBPoFaOpList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::InitHash()
{
	t_ManagePoFaOpPkg->InitPkg();
	
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

INT32					CManagePoFaOp::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FA_OP pdpfp 			= NULL;
	{
		if( (pdpfp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_op by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,%u,%u,%u,%u,%u,%u,%u,"
							"%u,%u,%u,%u,"
							"%u,%u,%u,", 
							GetHdrHashInfo(pdpfp).c_str(), pdpfp->nSchTime, pdpfp->nDelCount, pdpfp->nLimitSize,
							pdpfp->nLimitCnt, pdpfp->nChkFDTType, pdpfp->nChkFDTValue, pdpfp->nDelMethod,
							pdpfp->nLnxSchTime, pdpfp->nLnxDelCount, pdpfp->nLnxLimitSize, pdpfp->nLnxLimitCnt,
							pdpfp->nLnxChkFDTType, pdpfp->nLnxChkFDTValue, pdpfp->nLnxDelMethod
							);

		{
			TMapIDItor begin, end;
			begin = pdpfp->tDPH.tSubIDMap.begin();	end = pdpfp->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				t_ManagePoFaOpPkg->GetHash(begin->first, EmptyStr(m_strHashValue));
				strOrgValue += m_strHashValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfp->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::AddPoFaOp(DB_PO_FA_OP&	dpfp)
{
	if(SetER(t_DBMgrPoFaOp->InsertExecute(&dpfp)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfp.tDPH.nID, dpfp);
	InitHash(dpfp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::EditPoFaOp(DB_PO_FA_OP&	dpfp)
{
	PDB_PO_FA_OP pdpfp = FindItem(dpfp.tDPH.nID);
	if(!pdpfp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaOp->UpdateExecute(&dpfp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfp.tDPH.nID, dpfp);
	InitHash(dpfp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::DelPoFaOp(UINT32 nID)
{
	PDB_PO_FA_OP pdpfp = FindItem(nID);
	if(!pdpfp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaOp->DeleteExecute(pdpfp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::ClearPoFaOp()
{
	if(SetER(t_DBMgrPoFaOp->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::ApplyPoFaOp(DB_PO_FA_OP&	dpfp)
{
	if(FindItem(dpfp.tDPH.nID))
	{
		return EditPoFaOp(dpfp);
	}
	return AddPoFaOp(dpfp);
}
//---------------------------------------------------------------------------

String					CManagePoFaOp::GetName(UINT32 nID)
{
	PDB_PO_FA_OP pdpfp = FindItem(nID);
    if(!pdpfp)	return "";
    return pdpfp->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::GetHostSysTypePkgListID(TMapID tSubIDMap, TListID& tPkgIDList)
{
	TMapIDItor begin, end;
	begin = tSubIDMap.begin();	end = tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_OP_PKG pdpfop = t_ManagePoFaOpPkg->FindItem(begin->first);
		if(!pdpfop)	continue;

		PDB_PO_FA_OP_UNIT pdpfou = t_ManagePoFaOpUnit->FindItem(pdpfop->tDPH.nUnitID);
		if(!pdpfou)	continue;

		if(t_EnvInfo->m_nHostSysType & pdpfou->tDPH.nTargetOS)
			tPkgIDList.push_back(pdpfop->tDPH.nID);
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaOpItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP pdpfp = FindItem(nID);
	if(!pdpfp)	return -1;

	SetPkt(pdpfp, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpfp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfp->tDPH.tSubIDMap.begin();	end = pdpfp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaOpPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP pdpfp = FindItem(nID);
	if(!pdpfp)	return -1;
	
	SetPkt(pdpfp, SendToken);

	{		
		SendToken.TokenAdd_32(pdpfp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfp->tDPH.tSubIDMap.begin();	end = pdpfp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaOpPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::SetPktHost_EPS(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP pdpfp = FindItem(nID);
	if(!pdpfp)	return -1;

	SetPkt(pdpfp, SendToken);

	{
		TListID tPkgIDList;
		GetHostSysTypePkgListID(pdpfp->tDPH.tSubIDMap, tPkgIDList);	// OSType check

		SendToken.TokenAdd_32(tPkgIDList.size());		

		TListIDItor begin, end;
		begin = tPkgIDList.begin();	end = tPkgIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaOpPkg->SetPktHost(*begin, SendToken);
		}
	}

	return 0;
}


INT32					CManagePoFaOp::SetInotifyFaOp(UINT32 nID)
{
	INT32 i;
	PDB_PO_FA_OP pdpfp = FindItem(nID);
	if(!pdpfp)
		return -1;

	TListID tPkgIDList;
	GetHostSysTypePkgListID(pdpfp->tDPH.tSubIDMap, tPkgIDList);

	if(t_ThreadPoFaOp != NULL)
	{
		t_ThreadPoFaOp->ClearWatchNotify();
	}

	TListIDItor begin, end;
	begin = tPkgIDList.begin();	end = tPkgIDList.end();
	for(begin; begin != end; begin++)
	{
		t_ManagePoFaOpPkg->SetInotifyPkg(*begin);
	}

	return 0;
}


//---------------------------------------------------------------------------

INT32					CManagePoFaOp::SetPkt(PDB_PO_FA_OP pdpfp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfp->tDPH);

	SendToken.TokenAdd_32(pdpfp->nSchTime);
	SendToken.TokenAdd_32(pdpfp->nDelCount);
	SendToken.TokenAdd_32(pdpfp->nChkFDTType);
	SendToken.TokenAdd_32(pdpfp->nChkFDTValue);

	SendToken.TokenAdd_32(pdpfp->nLimitSize);
	SendToken.TokenAdd_32(pdpfp->nLimitCnt);
	SendToken.TokenAdd_32(pdpfp->nDelMethod);

	// linux
	SendToken.TokenAdd_32(pdpfp->nLnxSchTime);
	SendToken.TokenAdd_32(pdpfp->nLnxDelCount);
	SendToken.TokenAdd_32(pdpfp->nLnxLimitSize);
	SendToken.TokenAdd_32(pdpfp->nLnxLimitCnt);
	SendToken.TokenAdd_32(pdpfp->nLnxChkFDTType);
	SendToken.TokenAdd_32(pdpfp->nLnxChkFDTValue);
	SendToken.TokenAdd_32(pdpfp->nLnxDelMethod);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOp::GetPkt(MemToken& RecvToken, DB_PO_FA_OP& dpfp)
{
	if (!RecvToken.TokenDel_DPH(dpfp.tDPH))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfp.nSchTime))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nDelCount))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nChkFDTType))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nChkFDTValue))		goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfp.nLimitSize))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nLimitCnt))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nDelMethod))		goto	INVALID_PKT;

	// linux
	if (!RecvToken.TokenDel_32(dpfp.nLnxSchTime))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nLnxDelCount))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nLnxLimitSize))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nLnxLimitCnt))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nLnxChkFDTType))	goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nLnxChkFDTValue))	goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfp.nLnxDelMethod))		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



