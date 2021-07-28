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
#include "ManagePoPowerUnit.h"

//---------------------------------------------------------------------------

CManagePoPowerUnit*	t_ManagePoPowerUnit = NULL;

//---------------------------------------------------------------------------

CManagePoPowerUnit::CManagePoPowerUnit()
{
	t_DBMgrPoPowerUnit	= new CDBMgrPoPowerUnit();
}
//---------------------------------------------------------------------------

CManagePoPowerUnit::~CManagePoPowerUnit()
{
	if(t_DBMgrPoPowerUnit)	{	delete t_DBMgrPoPowerUnit;	t_DBMgrPoPowerUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoPowerUnit::LoadDBMS()
{
	TListDBPoPowerUnit tDBPoPowerUnitList;
    TListDBPoPowerUnitItor begin, end;
	if(SetER(t_DBMgrPoPowerUnit->LoadExecute(&tDBPoPowerUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoPowerUnitList.begin();	end = tDBPoPowerUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_POWER_UNIT pdppu 			= NULL;
	{
		if( (pdppu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_power_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%u,%u,%u,%u,%u",
							GetHdrHashInfo(pdppu).c_str(),
							pdppu->nScPeriodType, pdppu->nScStartTime, pdppu->nScEndTime, 
							pdppu->nCtlMode, pdppu->nCtlWaitTime, pdppu->nNotifyInfoID);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerUnit::AddPoPowerUnit(DB_PO_POWER_UNIT&	dppu)
{
	if(SetER(t_DBMgrPoPowerUnit->InsertExecute(&dppu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dppu.tDPH.nID, dppu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerUnit::EditPoPowerUnit(DB_PO_POWER_UNIT&	dppu)
{
	PDB_PO_POWER_UNIT pdppu = FindItem(dppu.tDPH.nID);
	if(!pdppu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoPowerUnit->UpdateExecute(&dppu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dppu.tDPH.nID, dppu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerUnit::DelPoPowerUnit(UINT32 nID)
{
	PDB_PO_POWER_UNIT pdppu = FindItem(nID);
	if(!pdppu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoPowerUnit->DeleteExecute(pdppu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerUnit::ApplyPoPowerUnit(DB_PO_POWER_UNIT&	dppu)
{
	if(FindItem(dppu.tDPH.nID))
	{
		return EditPoPowerUnit(dppu);
	}
	return AddPoPowerUnit(dppu);
}
//---------------------------------------------------------------------------

String					CManagePoPowerUnit::GetName(UINT32 nID)
{
	PDB_PO_POWER_UNIT pdppu = FindItem(nID);
    if(!pdppu)	return "";
    return pdppu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoPowerUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoPowerUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_POWER_UNIT pdppu = FindItem(nID);
	if(!pdppu)	return -1;

	return SetPkt(pdppu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerUnit::SetPkt(PDB_PO_POWER_UNIT pdppu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdppu->tDPH);

	SendToken.TokenAdd_32(pdppu->nScPeriodType);
	SendToken.TokenAdd_32(pdppu->nScStartTime);
	SendToken.TokenAdd_32(pdppu->nScEndTime);
	SendToken.TokenAdd_32(pdppu->nCtlMode);
	SendToken.TokenAdd_32(pdppu->nCtlWaitTime);
	SendToken.TokenAdd_32(pdppu->nNotifyInfoID);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPowerUnit::GetPkt(MemToken& RecvToken, DB_PO_POWER_UNIT& dppu)
{
	if (!RecvToken.TokenDel_DPH(dppu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dppu.nScPeriodType))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dppu.nScStartTime))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dppu.nScEndTime))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dppu.nCtlMode))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dppu.nCtlWaitTime))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dppu.nNotifyInfoID))			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



