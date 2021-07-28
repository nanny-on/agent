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
#include "ManagePoSelfProtectUnit.h"

//---------------------------------------------------------------------------

CManagePoSelfProtectUnit*	t_ManagePoSelfProtectUnit = NULL;

//---------------------------------------------------------------------------

CManagePoSelfProtectUnit::CManagePoSelfProtectUnit()
{
	t_DBMgrPoSelfProtectUnit	= new CDBMgrPoSelfProtectUnit();
}
//---------------------------------------------------------------------------

CManagePoSelfProtectUnit::~CManagePoSelfProtectUnit()
{
	if(t_DBMgrPoSelfProtectUnit)	{	delete t_DBMgrPoSelfProtectUnit;	t_DBMgrPoSelfProtectUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoSelfProtectUnit::LoadDBMS()
{
	TListDBPoSelfProtectUnit tDBPoSelfProtectUnitList;
    TListDBPoSelfProtectUnitItor begin, end;
	if(SetER(t_DBMgrPoSelfProtectUnit->LoadExecute(&tDBPoSelfProtectUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoSelfProtectUnitList.begin();	end = tDBPoSelfProtectUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_SELF_PROTECT_UNIT pdpspu 			= NULL;
	{
		if( (pdpspu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_self_protect_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%s,", 
							GetHdrHashInfo(pdpspu).c_str(),
							pdpspu->nProtectType, 
							pdpspu->strProtectInfo.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectUnit::AddPoSelfProtectUnit(DB_PO_SELF_PROTECT_UNIT&	dpspu)
{
	if(SetER(t_DBMgrPoSelfProtectUnit->InsertExecute(&dpspu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpspu.tDPH.nID, dpspu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectUnit::EditPoSelfProtectUnit(DB_PO_SELF_PROTECT_UNIT&	dpspu)
{
	PDB_PO_SELF_PROTECT_UNIT pdpspu = FindItem(dpspu.tDPH.nID);
	if(!pdpspu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoSelfProtectUnit->UpdateExecute(&dpspu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpspu.tDPH.nID, dpspu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectUnit::DelPoSelfProtectUnit(UINT32 nID)
{
	PDB_PO_SELF_PROTECT_UNIT pdpspu = FindItem(nID);
	if(!pdpspu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoSelfProtectUnit->DeleteExecute(pdpspu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectUnit::ApplyPoSelfProtectUnit(DB_PO_SELF_PROTECT_UNIT&	dpspu)
{
	if(FindItem(dpspu.tDPH.nID))
	{
		return EditPoSelfProtectUnit(dpspu);
	}
	return AddPoSelfProtectUnit(dpspu);
}
//---------------------------------------------------------------------------

String					CManagePoSelfProtectUnit::GetName(UINT32 nID)
{
	PDB_PO_SELF_PROTECT_UNIT pdpspu = FindItem(nID);
    if(!pdpspu)	return "";
    return pdpspu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoSelfProtectUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SELF_PROTECT_UNIT pdpspu = FindItem(nID);
	if(!pdpspu)	return -1;

	return SetPkt(pdpspu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectUnit::SetPkt(PDB_PO_SELF_PROTECT_UNIT pdpspu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpspu->tDPH);

	SendToken.TokenAdd_32(pdpspu->nProtectType);
	SendToken.TokenAdd_String(pdpspu->strProtectInfo);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSelfProtectUnit::GetPkt(MemToken& RecvToken, DB_PO_SELF_PROTECT_UNIT& dpspu)
{
	if (!RecvToken.TokenDel_DPH(dpspu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpspu.nProtectType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpspu.strProtectInfo) < 0)	goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



