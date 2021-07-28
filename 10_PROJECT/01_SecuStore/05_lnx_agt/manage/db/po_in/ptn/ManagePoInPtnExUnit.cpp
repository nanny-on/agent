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
#include "ManagePoInPtnExUnit.h"

//---------------------------------------------------------------------------

CManagePoInPtnExUnit*	t_ManagePoInPtnExUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInPtnExUnit::CManagePoInPtnExUnit()
{
	t_DBMgrPoInPtnExUnit	= new CDBMgrPoInPtnExUnit();
}
//---------------------------------------------------------------------------

CManagePoInPtnExUnit::~CManagePoInPtnExUnit()
{
	if(t_DBMgrPoInPtnExUnit)	{	delete t_DBMgrPoInPtnExUnit;	t_DBMgrPoInPtnExUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInPtnExUnit::LoadDBMS()
{
	TListDBPoInPtnExUnit tDBPoInPtnExUnitList;
    TListDBPoInPtnExUnitItor begin, end;
	if(SetER(t_DBMgrPoInPtnExUnit->LoadExecute(&tDBPoInPtnExUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInPtnExUnitList.begin();	end = tDBPoInPtnExUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_PTN_EX_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_ptn_ex_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,"
							"%u,%u,"
							"%s,"
							"%s,"
							"%s,%s,"
							"%s,"
							"%s,%s,"
							"%s,%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nBlockMode, 
							pdata->nUnitType, pdata->nFltChkType,
							pdata->strUserName.c_str(),
							pdata->tDFI.strPath.c_str(), 
							pdata->tDFI.strPublisher.c_str(), pdata->tDFI.strDescription.c_str(), 
							pdata->tDFI.strHash.c_str(),
							pdata->tDFI.strSignPubName.c_str(),pdata->tDFI.strSignPubSN.c_str(), 
							pdata->tDFI.strSignCntName.c_str(), pdata->tDFI.strSignCntSN.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExUnit::AddPoInPtnExUnit(DB_PO_IN_PTN_EX_UNIT&	data)
{
	if(SetER(t_DBMgrPoInPtnExUnit->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExUnit::EditPoInPtnExUnit(DB_PO_IN_PTN_EX_UNIT&	data)
{
	PDB_PO_IN_PTN_EX_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInPtnExUnit->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExUnit::DelPoInPtnExUnit(UINT32 nID)
{
	PDB_PO_IN_PTN_EX_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInPtnExUnit->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExUnit::ApplyPoInPtnExUnit(DB_PO_IN_PTN_EX_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInPtnExUnit(data);
	}
	return AddPoInPtnExUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInPtnExUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_PTN_EX_UNIT pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInPtnExUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_EX_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExUnit::SetPkt(PDB_PO_IN_PTN_EX_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nBlockMode);

	SendToken.TokenAdd_32(pdata->nUnitType);
	SendToken.TokenAdd_32(pdata->nFltChkType);

	SendToken.TokenAdd_String(pdata->strUserName);

	SendToken.TokenAdd_String(pdata->tDFI.strPath);
	SendToken.TokenAdd_String(pdata->tDFI.strPublisher);
	SendToken.TokenAdd_String(pdata->tDFI.strDescription);
	SendToken.TokenAdd_String(pdata->tDFI.strHash);
	SendToken.TokenAdd_String(pdata->tDFI.strSignPubName);
	SendToken.TokenAdd_String(pdata->tDFI.strSignPubSN);
	SendToken.TokenAdd_String(pdata->tDFI.strSignCntName);	
	SendToken.TokenAdd_String(pdata->tDFI.strSignCntSN);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnExUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_PTN_EX_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))							goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nBlockMode))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nUnitType))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nFltChkType))					goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strUserName) < 0)			goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.tDFI.strPath) < 0)			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDFI.strPublisher) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDFI.strDescription) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDFI.strHash) < 0)			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDFI.strSignPubName) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDFI.strSignPubSN) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDFI.strSignCntName) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tDFI.strSignCntSN) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



