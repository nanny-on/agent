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
#include "ManagePoFaOpSchUnit.h"

//---------------------------------------------------------------------------

CManagePoFaOpSchUnit*	t_ManagePoFaOpSchUnit = NULL;

//---------------------------------------------------------------------------

CManagePoFaOpSchUnit::CManagePoFaOpSchUnit()
{
	t_DBMgrPoFaOpSchUnit	= new CDBMgrPoFaOpSchUnit();
}
//---------------------------------------------------------------------------

CManagePoFaOpSchUnit::~CManagePoFaOpSchUnit()
{
	if(t_DBMgrPoFaOpSchUnit)	{	delete t_DBMgrPoFaOpSchUnit;	t_DBMgrPoFaOpSchUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaOpSchUnit::LoadDBMS()
{
	TListDBPoFaOpSchUnit tDBPoFaOpSchUnitList;
    TListDBPoFaOpSchUnitItor begin, end;
	if(SetER(t_DBMgrPoFaOpSchUnit->LoadExecute(&tDBPoFaOpSchUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaOpSchUnitList.begin();	end = tDBPoFaOpSchUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSchUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_OP_SCH_UNIT pdpfosu 			= NULL;
	{
		if( (pdpfosu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_op_sch_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		m_strHashValue = SPrintf("%s,"
							"%u,%u,%u,%u,%u,", 
							GetHdrHashInfo(pdpfosu).c_str(),
							pdpfosu->nSchMon, pdpfosu->nSchDay, pdpfosu->nSchWeek, 
							pdpfosu->nSchHour, pdpfosu->nSchMin);

		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSchUnit::AddPoFaOpSchUnit(DB_PO_FA_OP_SCH_UNIT&	dpfosu)
{
	if(SetER(t_DBMgrPoFaOpSchUnit->InsertExecute(&dpfosu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfosu.tDPH.nID, dpfosu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSchUnit::EditPoFaOpSchUnit(DB_PO_FA_OP_SCH_UNIT&	dpfosu)
{
	PDB_PO_FA_OP_SCH_UNIT pdpfosu = FindItem(dpfosu.tDPH.nID);
	if(!pdpfosu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaOpSchUnit->UpdateExecute(&dpfosu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfosu.tDPH.nID, dpfosu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSchUnit::DelPoFaOpSchUnit(UINT32 nID)
{
	PDB_PO_FA_OP_SCH_UNIT pdpfosu = FindItem(nID);
	if(!pdpfosu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaOpSchUnit->DeleteExecute(pdpfosu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSchUnit::ClearPoFaOpSchUnit()
{
	if(SetER(t_DBMgrPoFaOpSchUnit->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSchUnit::ApplyPoFaOpSchUnit(DB_PO_FA_OP_SCH_UNIT&	dpfosu)
{
	if(FindItem(dpfosu.tDPH.nID))
	{
		return EditPoFaOpSchUnit(dpfosu);
	}
	return AddPoFaOpSchUnit(dpfosu);
}
//---------------------------------------------------------------------------

String					CManagePoFaOpSchUnit::GetName(UINT32 nID)
{
	PDB_PO_FA_OP_SCH_UNIT pdpfosu = FindItem(nID);
    if(!pdpfosu)	return "";
    return pdpfosu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSchUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaOpSchUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSchUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_SCH_UNIT pdpfosu = FindItem(nID);
	if(!pdpfosu)	return -1;

	return SetPkt(pdpfosu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSchUnit::SetPkt(PDB_PO_FA_OP_SCH_UNIT pdpfosu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfosu->tDPH);

	SendToken.TokenAdd_32(pdpfosu->nSchMon);
	SendToken.TokenAdd_32(pdpfosu->nSchDay);
	SendToken.TokenAdd_32(pdpfosu->nSchWeek);
	SendToken.TokenAdd_32(pdpfosu->nSchHour);
	SendToken.TokenAdd_32(pdpfosu->nSchMin);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSchUnit::GetPkt(MemToken& RecvToken, DB_PO_FA_OP_SCH_UNIT& dpfosu)
{
	if (!RecvToken.TokenDel_DPH(dpfosu.tDPH))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfosu.nSchMon))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfosu.nSchDay))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfosu.nSchWeek))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfosu.nSchHour))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfosu.nSchMin))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



