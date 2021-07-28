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
#include "ManagePoInDevOExUnit.h"

//---------------------------------------------------------------------------

CManagePoInDevOExUnit*	t_ManagePoInDevOExUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInDevOExUnit::CManagePoInDevOExUnit()
{
	t_DBMgrPoInDevOExUnit	= new CDBMgrPoInDevOExUnit();
}
//---------------------------------------------------------------------------

CManagePoInDevOExUnit::~CManagePoInDevOExUnit()
{
	if(t_DBMgrPoInDevOExUnit)	{	delete t_DBMgrPoInDevOExUnit;	t_DBMgrPoInDevOExUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInDevOExUnit::LoadDBMS()
{
	TListDBPoInDevOExUnit tDBPoInDevOExUnitList;
	TListDBPoInDevOExUnitItor begin, end;
	if(SetER(t_DBMgrPoInDevOExUnit->LoadExecute(&tDBPoInDevOExUnitList)))
	{
		return g_nErrRtn;
	}

	begin = tDBPoInDevOExUnitList.begin();	end = tDBPoInDevOExUnitList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOExUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_DEVO_EX_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_devo_ex_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%u,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nBlockType, pdata->nExPtn);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOExUnit::AddPoInDevOExUnit(DB_PO_IN_DEVO_EX_UNIT&	data)
{
	if(SetER(t_DBMgrPoInDevOExUnit->InsertExecute(&data)))
	{
		return g_nErrRtn;
	}

	AddItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOExUnit::EditPoInDevOExUnit(DB_PO_IN_DEVO_EX_UNIT&	data)
{
	PDB_PO_IN_DEVO_EX_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInDevOExUnit->UpdateExecute(&data)))
	{
		return g_nErrRtn;
	}

	EditItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOExUnit::DelPoInDevOExUnit(UINT32 nID)
{
	PDB_PO_IN_DEVO_EX_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInDevOExUnit->DeleteExecute(pdata->tDPH.nID)))
	{
		return g_nErrRtn;
	}

	DeleteItem(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOExUnit::ApplyPoInDevOExUnit(DB_PO_IN_DEVO_EX_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInDevOExUnit(data);
	}
	return AddPoInDevOExUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInDevOExUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_DEVO_EX_UNIT pdata = FindItem(nID);
	if(!pdata)	return "";
	return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
INT32					CManagePoInDevOExUnit::IsPolType(UINT32 nDvType)
{
	TMapDBPoInDevOExUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.tDPH.nUsedMode != STATUS_USED_MODE_ON)	continue;
		if((begin->second.nExPtn & nDvType) && begin->second.nBlockType == 0 /*허용*/)	return 0;
		if((begin->second.nExPtn & nDvType) && begin->second.nBlockType == 1 /*차단*/)	return 1;
	}
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInDevOExUnit::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInDevOExUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOExUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_DEVO_EX_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOExUnit::SetPkt(PDB_PO_IN_DEVO_EX_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nBlockType);
	SendToken.TokenAdd_32(pdata->nExPtn);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOExUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_DEVO_EX_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nBlockType))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nExPtn))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



