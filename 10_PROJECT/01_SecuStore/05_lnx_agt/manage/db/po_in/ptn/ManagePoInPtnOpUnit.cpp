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
#include "ManagePoInPtnOpUnit.h"

//---------------------------------------------------------------------------

CManagePoInPtnOpUnit*	t_ManagePoInPtnOpUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInPtnOpUnit::CManagePoInPtnOpUnit()
{
	t_DBMgrPoInPtnOpUnit	= new CDBMgrPoInPtnOpUnit();
}
//---------------------------------------------------------------------------

CManagePoInPtnOpUnit::~CManagePoInPtnOpUnit()
{
	if(t_DBMgrPoInPtnOpUnit)	{	delete t_DBMgrPoInPtnOpUnit;	t_DBMgrPoInPtnOpUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInPtnOpUnit::LoadDBMS()
{
	TListDBPoInPtnOpUnit tDBPoInPtnOpUnitList;
    TListDBPoInPtnOpUnitItor begin, end;
	if(SetER(t_DBMgrPoInPtnOpUnit->LoadExecute(&tDBPoInPtnOpUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInPtnOpUnitList.begin();	end = tDBPoInPtnOpUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOpUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_PTN_OP_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_ptn_op_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%llu,"
							"%s,%u,",							
							GetHdrHashInfo(pdata).c_str(),			
							pdata->nSchTime, 
							pdata->strScanPath.c_str(), pdata->nFileGatherType);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOpUnit::AddPoInPtnOpUnit(DB_PO_IN_PTN_OP_UNIT&	data)
{
	if(SetER(t_DBMgrPoInPtnOpUnit->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOpUnit::EditPoInPtnOpUnit(DB_PO_IN_PTN_OP_UNIT&	data)
{
	PDB_PO_IN_PTN_OP_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInPtnOpUnit->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOpUnit::DelPoInPtnOpUnit(UINT32 nID)
{
	PDB_PO_IN_PTN_OP_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInPtnOpUnit->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOpUnit::ApplyPoInPtnOpUnit(DB_PO_IN_PTN_OP_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInPtnOpUnit(data);
	}
	return AddPoInPtnOpUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInPtnOpUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_PTN_OP_UNIT pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOpUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInPtnOpUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOpUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_OP_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOpUnit::SetPkt(PDB_PO_IN_PTN_OP_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_64(pdata->nSchTime);

	SendToken.TokenAdd_String(pdata->strScanPath);
	SendToken.TokenAdd_32(pdata->nFileGatherType);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnOpUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_PTN_OP_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))							goto	INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nSchTime))						goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strScanPath) < 0)			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nFileGatherType))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



