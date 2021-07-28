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
#include "ManagePoPmExUnit.h"

//---------------------------------------------------------------------------

CManagePoPmExUnit*	t_ManagePoPmExUnit = NULL;

//---------------------------------------------------------------------------

CManagePoPmExUnit::CManagePoPmExUnit()
{
	t_DBMgrPoPmExUnit	= new CDBMgrPoPmExUnit();
}
//---------------------------------------------------------------------------

CManagePoPmExUnit::~CManagePoPmExUnit()
{
	if(t_DBMgrPoPmExUnit)	{	delete t_DBMgrPoPmExUnit;	t_DBMgrPoPmExUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoPmExUnit::LoadDBMS()
{
	TListDBPoPmExUnit tDBPoPmExUnitList;
    TListDBPoPmExUnitItor begin, end;
	if(SetER(t_DBMgrPoPmExUnit->LoadExecute(&tDBPoPmExUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoPmExUnitList.begin();	end = tDBPoPmExUnitList.end();
    for(begin; begin != end; begin++)
	{
		HexToMap(begin->strPatchID, begin->tPatchIDMap, 0);
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmExUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_PM_EX_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_pm_ex_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nExMode, pdata->strPatchID.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmExUnit::AddPoPmExUnit(DB_PO_PM_EX_UNIT&	data)
{
	if(SetER(t_DBMgrPoPmExUnit->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	data.tPatchIDMap.clear();
	HexToMap(data.strPatchID, data.tPatchIDMap, 0);
    AddItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmExUnit::EditPoPmExUnit(DB_PO_PM_EX_UNIT&	data)
{
	PDB_PO_PM_EX_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoPmExUnit->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

	data.tPatchIDMap.clear();
	HexToMap(data.strPatchID, data.tPatchIDMap, 0);
	EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmExUnit::DelPoPmExUnit(UINT32 nID)
{
	PDB_PO_PM_EX_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoPmExUnit->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmExUnit::ApplyPoPmExUnit(DB_PO_PM_EX_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoPmExUnit(data);
	}
	return AddPoPmExUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoPmExUnit::GetName(UINT32 nID)
{
	PDB_PO_PM_EX_UNIT pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoPmExUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoPmExUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmExUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_PM_EX_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoPmExUnit::SetPkt(PDB_PO_PM_EX_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nExMode);
	SendToken.TokenAdd_String(pdata->strPatchID);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmExUnit::GetPkt(MemToken& RecvToken, DB_PO_PM_EX_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))							goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nExMode))						goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strPatchID) < 0)			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



