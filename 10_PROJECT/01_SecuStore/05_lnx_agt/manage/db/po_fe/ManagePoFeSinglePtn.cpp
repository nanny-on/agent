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
#include "ManagePoFeSinglePtn.h"

//---------------------------------------------------------------------------

CManagePoFeSinglePtn*	t_ManagePoFeSinglePtn = NULL;

//---------------------------------------------------------------------------

CManagePoFeSinglePtn::CManagePoFeSinglePtn()
{
	t_DBMgrPoFeSinglePtn	= new CDBMgrPoFeSinglePtn();
}
//---------------------------------------------------------------------------

CManagePoFeSinglePtn::~CManagePoFeSinglePtn()
{
	if(t_DBMgrPoFeSinglePtn)	{	delete t_DBMgrPoFeSinglePtn;	t_DBMgrPoFeSinglePtn = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFeSinglePtn::LoadDBMS()
{
	DB_PO_FE_SINGLE_PTN dpfsp;
	TListDBPoFeSinglePtn tDBPoFeSinglePtnList;
    TListDBPoFeSinglePtnItor begin, end;
	if(SetER(t_DBMgrPoFeSinglePtn->LoadExecute(&tDBPoFeSinglePtnList)))
    {
		if(tDBPoFeSinglePtnList.size() == 0)
		{
			memset(&dpfsp, 0, sizeof(DB_PO_FE_SINGLE_PTN));
			dpfsp.tDPH.nID = 1;
			dpfsp.tDPH.nUsedFlag = USED_FLAG_TYPE_USED;
			dpfsp.tDPH.nRegDate = GetCurrentDateTimeInt();
			AddPoFeSinglePtn(dpfsp);
		}
    	return g_nErrRtn;
    }

    begin = tDBPoFeSinglePtnList.begin();	end = tDBPoFeSinglePtnList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
	}

	if(tDBPoFeSinglePtnList.size() == 0)
	{
		memset(&dpfsp, 0, sizeof(DB_PO_FE_SINGLE_PTN));
		dpfsp.tDPH.nID = 1;
		dpfsp.tDPH.nUsedFlag = USED_FLAG_TYPE_USED;
		dpfsp.tDPH.nRegDate = GetCurrentDateTimeInt();
		AddPoFeSinglePtn(dpfsp);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::InitHash()
{
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFeSinglePtn::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::AddPoFeSinglePtn(DB_PO_FE_SINGLE_PTN&	dpfsp)
{
	if(SetER(t_DBMgrPoFeSinglePtn->InsertExecute(&dpfsp)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfsp.tDPH.nID, dpfsp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::EditPoFeSinglePtn(DB_PO_FE_SINGLE_PTN&	dpfsp)
{
	PDB_PO_FE_SINGLE_PTN pdpfsp = FindItem(dpfsp.tDPH.nID);
	if(!pdpfsp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFeSinglePtn->UpdateExecute(&dpfsp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfsp.tDPH.nID, dpfsp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::DelPoFeSinglePtn(UINT32 nID)
{
	PDB_PO_FE_SINGLE_PTN pdpfsp = FindItem(nID);
	if(!pdpfsp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFeSinglePtn->DeleteExecute(pdpfsp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::ApplyPoFeSinglePtn(DB_PO_FE_SINGLE_PTN&	dpfsp)
{
	if(FindItem(dpfsp.tDPH.nID))
	{
		return EditPoFeSinglePtn(dpfsp);
	}
	return AddPoFeSinglePtn(dpfsp);
}
//---------------------------------------------------------------------------

String					CManagePoFeSinglePtn::GetName(UINT32 nID)
{
	PDB_PO_FE_SINGLE_PTN pdpfsp = FindItem(nID);
    if(!pdpfsp)	return "";
    return pdpfsp->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFeSinglePtnItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_SINGLE_PTN pdpfsp = FindItem(nID);
	if(!pdpfsp)	return -1;

	return SetPkt(pdpfsp, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::SetPkt(PDB_PO_FE_SINGLE_PTN pdpfsp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfsp->tDPH);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::GetPkt(MemToken& RecvToken, DB_PO_FE_SINGLE_PTN& dpfsp)
{
	if (!RecvToken.TokenDel_DPH(dpfsp.tDPH))		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_SINGLE_PTN pdpfsp = FindItem(nID);
	if(!pdpfsp)	return -1;

	SetPktHost(pdpfsp, SendToken);

	t_ManagePoFeSinglePtnUnit->SetPkt(SendToken);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtn::SetPktHost(PDB_PO_FE_SINGLE_PTN pdpfsp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfsp->tDPH);

	SendToken.TokenSet_Block();

	return 0;
}
//---------------------------------------------------------------------------



