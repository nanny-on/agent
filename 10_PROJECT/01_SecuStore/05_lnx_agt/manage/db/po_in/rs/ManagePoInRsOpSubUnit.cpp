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
#include "ManagePoInRsOpSubUnit.h"

//---------------------------------------------------------------------------

CManagePoInRsOpSubUnit*	t_ManagePoInRsOpSubUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInRsOpSubUnit::CManagePoInRsOpSubUnit()
{
	t_DBMgrPoInRsOpSubUnit	= new CDBMgrPoInRsOpSubUnit();
}
//---------------------------------------------------------------------------

CManagePoInRsOpSubUnit::~CManagePoInRsOpSubUnit()
{
	if(t_DBMgrPoInRsOpSubUnit)	{	delete t_DBMgrPoInRsOpSubUnit;	t_DBMgrPoInRsOpSubUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInRsOpSubUnit::LoadDBMS()
{
	TListDBPoInRsOpSubUnit tDBPoInRsOpSubUnitList;
    TListDBPoInRsOpSubUnitItor begin, end;
/*
	if(SetER(t_DBMgrPoInRsOpSubUnit->LoadExecute(&tDBPoInRsOpSubUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInRsOpSubUnitList.begin();	end = tDBPoInRsOpSubUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
*/
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpSubUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_RS_OP_SUB_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_rs_op_sub_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		m_strHashValue = SPrintf("%s,"
								"%s,%s,%u,", 
								GetHdrHashInfo(pdata).c_str(),
								pdata->strSbType.c_str(), pdata->strSbProc.c_str(), pdata->nAcMode);

		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpSubUnit::AddPoInRsOpSubUnit(DB_PO_IN_RS_OP_SUB_UNIT&	data)
{
	if(SetER(t_DBMgrPoInRsOpSubUnit->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpSubUnit::EditPoInRsOpSubUnit(DB_PO_IN_RS_OP_SUB_UNIT&	data)
{
	PDB_PO_IN_RS_OP_SUB_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInRsOpSubUnit->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpSubUnit::DelPoInRsOpSubUnit(UINT32 nID)
{
	PDB_PO_IN_RS_OP_SUB_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInRsOpSubUnit->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpSubUnit::ApplyPoInRsOpSubUnit(DB_PO_IN_RS_OP_SUB_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInRsOpSubUnit(data);
	}
	return AddPoInRsOpSubUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInRsOpSubUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_RS_OP_SUB_UNIT pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpSubUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInRsOpSubUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpSubUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP_SUB_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpSubUnit::SetPkt(PDB_PO_IN_RS_OP_SUB_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_String(pdata->strSbType);
	SendToken.TokenAdd_String(pdata->strSbProc);
	SendToken.TokenAdd_32(pdata->nAcMode);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpSubUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_RS_OP_SUB_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strSbType) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strSbProc) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nAcMode))				goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



