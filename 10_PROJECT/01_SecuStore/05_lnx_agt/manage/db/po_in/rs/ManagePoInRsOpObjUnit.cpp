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
#include "ManagePoInRsOpObjUnit.h"

//---------------------------------------------------------------------------

CManagePoInRsOpObjUnit*	t_ManagePoInRsOpObjUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInRsOpObjUnit::CManagePoInRsOpObjUnit()
{
	t_DBMgrPoInRsOpObjUnit	= new CDBMgrPoInRsOpObjUnit();
}
//---------------------------------------------------------------------------

CManagePoInRsOpObjUnit::~CManagePoInRsOpObjUnit()
{
	if(t_DBMgrPoInRsOpObjUnit)	{	delete t_DBMgrPoInRsOpObjUnit;	t_DBMgrPoInRsOpObjUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInRsOpObjUnit::LoadDBMS()
{
	TListDBPoInRsOpObjUnit tDBPoInRsOpObjUnitList;
    TListDBPoInRsOpObjUnitItor begin, end;
/*
	if(SetER(t_DBMgrPoInRsOpObjUnit->LoadExecute(&tDBPoInRsOpObjUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInRsOpObjUnitList.begin();	end = tDBPoInRsOpObjUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
*/
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpObjUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_RS_OP_OBJ_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_rs_op_obj_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		m_strHashValue = SPrintf("%s,"
								"%s,%s,%s,", 
								GetHdrHashInfo(pdata).c_str(),
								pdata->strChkType.c_str(), pdata->strChkPath.c_str(), pdata->strChkExt.c_str());

		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpObjUnit::AddPoInRsOpObjUnit(DB_PO_IN_RS_OP_OBJ_UNIT&	data)
{
	if(SetER(t_DBMgrPoInRsOpObjUnit->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpObjUnit::EditPoInRsOpObjUnit(DB_PO_IN_RS_OP_OBJ_UNIT&	data)
{
	PDB_PO_IN_RS_OP_OBJ_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInRsOpObjUnit->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpObjUnit::DelPoInRsOpObjUnit(UINT32 nID)
{
	PDB_PO_IN_RS_OP_OBJ_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInRsOpObjUnit->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpObjUnit::ApplyPoInRsOpObjUnit(DB_PO_IN_RS_OP_OBJ_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInRsOpObjUnit(data);
	}
	return AddPoInRsOpObjUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInRsOpObjUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_RS_OP_OBJ_UNIT pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpObjUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInRsOpObjUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpObjUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP_OBJ_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpObjUnit::SetPkt(PDB_PO_IN_RS_OP_OBJ_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_String(pdata->strChkType);
	SendToken.TokenAdd_String(pdata->strChkPath);
	SendToken.TokenAdd_String(pdata->strChkExt);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpObjUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_RS_OP_OBJ_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strChkType) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strChkPath) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strChkExt) < 0)		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



