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
#include "ManagePoCtlProc.h"

//---------------------------------------------------------------------------

CManagePoCtlProc*	t_ManagePoCtlProc = NULL;

//---------------------------------------------------------------------------

CManagePoCtlProc::CManagePoCtlProc()
{
	t_DBMgrPoCtlProc	= new CDBMgrPoCtlProc();
}
//---------------------------------------------------------------------------

CManagePoCtlProc::~CManagePoCtlProc()
{
	if(t_DBMgrPoCtlProc)	{	delete t_DBMgrPoCtlProc;	t_DBMgrPoCtlProc = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoCtlProc::LoadDBMS()
{
	TListDBPoCtlProc tDBPoCtlProcList;
    TListDBPoCtlProcItor begin, end;
	if(SetER(t_DBMgrPoCtlProc->LoadExecute(&tDBPoCtlProcList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoCtlProcList.begin();	end = tDBPoCtlProcList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::InitHash()
{
	t_ManagePoCtlProcPkg->InitPkg();
	
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

INT32					CManagePoCtlProc::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_CTL_PROC pdpcp 			= NULL;
	{
		if( (pdpcp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_ctl_proc by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,%u,", 
							GetHdrHashInfo(pdpcp).c_str(),
							pdpcp->nBlockMode);

		{
			TMapIDItor begin, end;
			begin = pdpcp->tDPH.tSubIDMap.begin();	end = pdpcp->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoCtlProcPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpcp->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::AddPoCtlProc(DB_PO_CTL_PROC&	dpcp)
{
	if(SetER(t_DBMgrPoCtlProc->InsertExecute(&dpcp)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpcp.tDPH.nID, dpcp);
	InitHash(dpcp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::EditPoCtlProc(DB_PO_CTL_PROC&	dpcp)
{
	PDB_PO_CTL_PROC pdpcp = FindItem(dpcp.tDPH.nID);
	if(!pdpcp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoCtlProc->UpdateExecute(&dpcp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpcp.tDPH.nID, dpcp);
	InitHash(dpcp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::DelPoCtlProc(UINT32 nID)
{
	PDB_PO_CTL_PROC pdpcp = FindItem(nID);
	if(!pdpcp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoCtlProc->DeleteExecute(pdpcp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::ApplyPoCtlProc(DB_PO_CTL_PROC&	dpcp)
{
	if(FindItem(dpcp.tDPH.nID))
	{
		return EditPoCtlProc(dpcp);
	}
	return AddPoCtlProc(dpcp);
}
//---------------------------------------------------------------------------

String					CManagePoCtlProc::GetName(UINT32 nID)
{
	PDB_PO_CTL_PROC pdpcp = FindItem(nID);
    if(!pdpcp)	return "";
    return pdpcp->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::GetHostSysTypePkgListID(TMapID tSubIDMap, TListID& tPkgIDList)
{
	TMapIDItor begin, end;
	begin = tSubIDMap.begin();	end = tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_CTL_PROC_PKG pdpcpp = t_ManagePoCtlProcPkg->FindItem(begin->first);
		if(!pdpcpp)	continue;

		PDB_PO_CTL_PROC_UNIT pdpppu = t_ManagePoCtlProcUnit->FindItem(pdpcpp->tDPH.nUnitID);
		if(!pdpppu)	continue;

		if(t_EnvInfo->m_nHostSysType & pdpppu->tDPH.nTargetOS)
			tPkgIDList.push_back(pdpcpp->tDPH.nID);
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoCtlProcItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PROC pdpcp = FindItem(nID);
	if(!pdpcp)	return -1;

	SetPkt(pdpcp, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpcp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpcp->tDPH.tSubIDMap.begin();	end = pdpcp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoCtlProcPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PROC pdpcp = FindItem(nID);
	if(!pdpcp)	return -1;

	SetPkt(pdpcp, SendToken);

	{
		SendToken.TokenAdd_32(pdpcp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpcp->tDPH.tSubIDMap.begin();	end = pdpcp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoCtlProcPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::SetPktHost_EPS(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PROC pdpcp = FindItem(nID);
	if(!pdpcp)	return -1;

	SetPkt(pdpcp, SendToken);

	{
		TListID tPkgIDList;
		GetHostSysTypePkgListID(pdpcp->tDPH.tSubIDMap, tPkgIDList);	// OSType check

		SendToken.TokenAdd_32(tPkgIDList.size());		

		TListIDItor begin, end;
		begin = tPkgIDList.begin();	end = tPkgIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoCtlProcPkg->SetPktHost(*begin, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::SetPkt(PDB_PO_CTL_PROC pdpcp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpcp->tDPH);
	SendToken.TokenAdd_32(pdpcp->nBlockMode);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProc::GetPkt(MemToken& RecvToken, DB_PO_CTL_PROC& dpcp)
{
	if (!RecvToken.TokenDel_DPH(dpcp.tDPH))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpcp.nBlockMode))		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



