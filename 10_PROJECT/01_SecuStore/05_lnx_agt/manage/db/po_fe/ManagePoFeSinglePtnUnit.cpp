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
#include "ManagePoFeSinglePtnUnit.h"

//---------------------------------------------------------------------------

CManagePoFeSinglePtnUnit*	t_ManagePoFeSinglePtnUnit = NULL;

//---------------------------------------------------------------------------

CManagePoFeSinglePtnUnit::CManagePoFeSinglePtnUnit()
{
	t_DBMgrPoFeSinglePtnUnit	= new CDBMgrPoFeSinglePtnUnit();
}
//---------------------------------------------------------------------------

CManagePoFeSinglePtnUnit::~CManagePoFeSinglePtnUnit()
{
	if(t_DBMgrPoFeSinglePtnUnit)	{	delete t_DBMgrPoFeSinglePtnUnit;	t_DBMgrPoFeSinglePtnUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFeSinglePtnUnit::LoadDBMS()
{
	TListDBPoFeSinglePtnUnit tDBPoFeSinglePtnUnitList;
    TListDBPoFeSinglePtnUnitItor begin, end;
	if(SetER(t_DBMgrPoFeSinglePtnUnit->LoadExecute(&tDBPoFeSinglePtnUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFeSinglePtnUnitList.begin();	end = tDBPoFeSinglePtnUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtnUnit::InitHash()
{	
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoFeSinglePtnUnit::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtnUnit::AddPoFeSinglePtnUnit(DB_PO_FE_SINGLE_PTN_UNIT&	dpfspu)
{
	if(SetER(t_DBMgrPoFeSinglePtnUnit->InsertExecute(&dpfspu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfspu.tDPH.nID, dpfspu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtnUnit::EditPoFeSinglePtnUnit(DB_PO_FE_SINGLE_PTN_UNIT&	dpfspu)
{
	PDB_PO_FE_SINGLE_PTN_UNIT pdpfspu = FindItem(dpfspu.tDPH.nID);
	if(!pdpfspu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFeSinglePtnUnit->UpdateExecute(&dpfspu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfspu.tDPH.nID, dpfspu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtnUnit::DelPoFeSinglePtnUnit(UINT32 nID)
{
	PDB_PO_FE_SINGLE_PTN_UNIT pdpfspu = FindItem(nID);
	if(!pdpfspu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFeSinglePtnUnit->DeleteExecute(pdpfspu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtnUnit::ApplyPoFeSinglePtnUnit(DB_PO_FE_SINGLE_PTN_UNIT&	dpfspu)
{
	if(FindItem(dpfspu.tDPH.nID))
	{
		return EditPoFeSinglePtnUnit(dpfspu);
	}
	return AddPoFeSinglePtnUnit(dpfspu);
}
//---------------------------------------------------------------------------

String					CManagePoFeSinglePtnUnit::GetName(UINT32 nID)
{
	PDB_PO_FE_SINGLE_PTN_UNIT pdpfspu = FindItem(nID);
    if(!pdpfspu)	return "";
    return pdpfspu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtnUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFeSinglePtnUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtnUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_SINGLE_PTN_UNIT pdpfspu = FindItem(nID);
	if(!pdpfspu)	return -1;

	return SetPkt(pdpfspu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtnUnit::SetPkt(PDB_PO_FE_SINGLE_PTN_UNIT pdpfspu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfspu->tDPH);

	SendToken.TokenAdd_32(pdpfspu->nTargetType);
	SendToken.TokenAdd_32(pdpfspu->nTargetID);
	SendToken.TokenAdd_32(pdpfspu->nBlockType);
	SendToken.TokenAdd_String(pdpfspu->strFeKey);
	SendToken.TokenAdd_32(pdpfspu->nLimitTime);
	SendToken.TokenAdd_32(pdpfspu->nPeType);
	SendToken.TokenAdd_32(pdpfspu->nPeBit);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeSinglePtnUnit::GetPkt(MemToken& RecvToken, DB_PO_FE_SINGLE_PTN_UNIT& dpfspu)
{
	if (!RecvToken.TokenDel_DPH(dpfspu.tDPH))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfspu.nTargetType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfspu.nTargetID))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfspu.nBlockType))				goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfspu.strFeKey) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfspu.nLimitTime))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfspu.nPeType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfspu.nPeBit))					goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



