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
#include "ManagePoFaProc.h"

//---------------------------------------------------------------------------

CManagePoFaProc*	t_ManagePoFaProc = NULL;

//---------------------------------------------------------------------------

CManagePoFaProc::CManagePoFaProc()
{
	t_DBMgrPoFaProc	= new CDBMgrPoFaProc();
}
//---------------------------------------------------------------------------

CManagePoFaProc::~CManagePoFaProc()
{
	if(t_DBMgrPoFaProc)	{	delete t_DBMgrPoFaProc;	t_DBMgrPoFaProc = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaProc::LoadDBMS()
{
	TListDBPoFaProc tDBPoFaProcList;
    TListDBPoFaProcItor begin, end;
	if(SetER(t_DBMgrPoFaProc->LoadExecute(&tDBPoFaProcList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaProcList.begin();	end = tDBPoFaProcList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::InitHash()
{
	t_ManagePoFaProcPkg->InitPkg();
	
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

INT32					CManagePoFaProc::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FA_PROC pdpfp 			= NULL;
	{
		if( (pdpfp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_proc by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,", 
							GetHdrHashInfo(pdpfp).c_str());

		{
			TMapIDItor begin, end;
			begin = pdpfp->tDPH.tSubIDMap.begin();	end = pdpfp->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoFaProcPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
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

INT32					CManagePoFaProc::AddPoFaProc(DB_PO_FA_PROC&	dpfp)
{
	if(SetER(t_DBMgrPoFaProc->InsertExecute(&dpfp)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfp.tDPH.nID, dpfp);
	InitHash(dpfp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::EditPoFaProc(DB_PO_FA_PROC&	dpfp)
{
	PDB_PO_FA_PROC pdpfp = FindItem(dpfp.tDPH.nID);
	if(!pdpfp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaProc->UpdateExecute(&dpfp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfp.tDPH.nID, dpfp);
	InitHash(dpfp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::DelPoFaProc(UINT32 nID)
{
	PDB_PO_FA_PROC pdpfp = FindItem(nID);
	if(!pdpfp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaProc->DeleteExecute(pdpfp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::ApplyPoFaProc(DB_PO_FA_PROC&	dpfp)
{
	if(FindItem(dpfp.tDPH.nID))
	{
		return EditPoFaProc(dpfp);
	}
	return AddPoFaProc(dpfp);
}
//---------------------------------------------------------------------------

String					CManagePoFaProc::GetName(UINT32 nID)
{
	PDB_PO_FA_PROC pdpfp = FindItem(nID);
    if(!pdpfp)	return "";
    return pdpfp->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::GetHostSysTypePkgListID(TMapID tSubIDMap, TListID& tPkgIDList)
{
	TMapIDItor begin, end;
	begin = tSubIDMap.begin();	end = tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_PROC_PKG pdpfpp = t_ManagePoFaProcPkg->FindItem(begin->first);
		if(!pdpfpp)	continue;

		PDB_PO_FA_PROC_UNIT pdpfpu = t_ManagePoFaProcUnit->FindItem(pdpfpp->tDPH.nUnitID);
		if(!pdpfpu)	continue;

		if(t_EnvInfo->m_nHostSysType & pdpfpu->tDPH.nTargetOS)
			tPkgIDList.push_back(pdpfpp->tDPH.nID);
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaProcItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_PROC pdpfp = FindItem(nID);
	if(!pdpfp)	return -1;

	SetPkt(pdpfp, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpfp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfp->tDPH.tSubIDMap.begin();	end = pdpfp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaProcPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_PROC pdpfp = FindItem(nID);
	if(!pdpfp)	return -1;

	SetPkt(pdpfp, SendToken);

	{
		SendToken.TokenAdd_32(pdpfp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfp->tDPH.tSubIDMap.begin();	end = pdpfp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaProcPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::SetPktHost_EPS(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_PROC pdpfp = FindItem(nID);
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
			t_ManagePoFaProcPkg->SetPktHost(*begin, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::SetPkt(PDB_PO_FA_PROC pdpfp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfp->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProc::GetPkt(MemToken& RecvToken, DB_PO_FA_PROC& dpfp)
{
	if (!RecvToken.TokenDel_DPH(dpfp.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



