/*
 * Copyright (C) 2021-2026 ASHINi Corp. 
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; either 
 * version 3 of the License, or any later version, with the additional
 * special exception to link portions of this program with the OpenSSL
 * library. See LICENSE for more details. 
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
#include "ManagePoHostNotifyUnit.h"

//---------------------------------------------------------------------------

CManagePoHostNotifyUnit*	t_ManagePoHostNotifyUnit = NULL;

//---------------------------------------------------------------------------

CManagePoHostNotifyUnit::CManagePoHostNotifyUnit()
{
	t_DBMgrPoHostNotifyUnit	= new CDBMgrPoHostNotifyUnit();
}
//---------------------------------------------------------------------------

CManagePoHostNotifyUnit::~CManagePoHostNotifyUnit()
{
	if(t_DBMgrPoHostNotifyUnit)	{	delete t_DBMgrPoHostNotifyUnit;	t_DBMgrPoHostNotifyUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoHostNotifyUnit::LoadDBMS()
{
	TListDBPoHostNotifyUnit tDBPoHostNotifyUnitList;
    TListDBPoHostNotifyUnitItor begin, end;
	if(SetER(t_DBMgrPoHostNotifyUnit->LoadExecute(&tDBPoHostNotifyUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoHostNotifyUnitList.begin();	end = tDBPoHostNotifyUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_HOST_NOTIFY_UNIT pdphnu 			= NULL;
	{
		if( (pdphnu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_host_notify_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strItemHash;
		strItemHash = SPrintf("%s,"
			"%u,%u,%u,%u,%u,"
			"%u,%s,%u,", 
			GetHdrHashInfo(pdphnu).c_str(),
			pdphnu->nSchTime,
			pdphnu->nPosType, pdphnu->nShowSize, pdphnu->nShowPos, pdphnu->nShowTime,
			pdphnu->nMsgFmtType, pdphnu->strMsgInfo.c_str(), pdphnu->nNotifySkipDay);

		strOrgValue += strItemHash;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyUnit::AddPoHostNotifyUnit(DB_PO_HOST_NOTIFY_UNIT&	dphnu)
{
	if(SetER(t_DBMgrPoHostNotifyUnit->InsertExecute(&dphnu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dphnu.tDPH.nID, dphnu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyUnit::EditPoHostNotifyUnit(DB_PO_HOST_NOTIFY_UNIT&	dphnu)
{
	PDB_PO_HOST_NOTIFY_UNIT pdppu = FindItem(dphnu.tDPH.nID);
	if(!pdppu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoHostNotifyUnit->UpdateExecute(&dphnu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dphnu.tDPH.nID, dphnu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyUnit::DelPoHostNotifyUnit(UINT32 nID)
{
	PDB_PO_HOST_NOTIFY_UNIT pdphnu = FindItem(nID);
	if(!pdphnu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoHostNotifyUnit->DeleteExecute(pdphnu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyUnit::ApplyPoHostNotifyUnit(DB_PO_HOST_NOTIFY_UNIT&	dphnu)
{
	if(FindItem(dphnu.tDPH.nID))
	{
		return EditPoHostNotifyUnit(dphnu);
	}
	return AddPoHostNotifyUnit(dphnu);
}
//---------------------------------------------------------------------------

String					CManagePoHostNotifyUnit::GetName(UINT32 nID)
{
	PDB_PO_HOST_NOTIFY_UNIT pdppu = FindItem(nID);
    if(!pdppu)	return "";
    return pdppu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoHostNotifyUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_HOST_NOTIFY_UNIT pdppu = FindItem(nID);
	if(!pdppu)	return -1;

	return SetPkt(pdppu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyUnit::SetPkt(PDB_PO_HOST_NOTIFY_UNIT pdphnu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdphnu->tDPH);

	SendToken.TokenAdd_64(pdphnu->nSchTime);
	SendToken.TokenAdd_32(pdphnu->nPosType);
	SendToken.TokenAdd_32(pdphnu->nShowSize);
	SendToken.TokenAdd_32(pdphnu->nShowPos);
	SendToken.TokenAdd_32(pdphnu->nShowTime);
	SendToken.TokenAdd_32(pdphnu->nMsgFmtType);
	SendToken.TokenAdd_String(pdphnu->strMsgInfo);
	SendToken.TokenAdd_32(pdphnu->nNotifySkipDay);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostNotifyUnit::GetPkt(MemToken& RecvToken, DB_PO_HOST_NOTIFY_UNIT& dphnu)
{
	if (!RecvToken.TokenDel_DPH(dphnu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_64(dphnu.nSchTime))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphnu.nPosType))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphnu.nShowSize))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphnu.nShowPos))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphnu.nShowTime))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphnu.nMsgFmtType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dphnu.strMsgInfo) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphnu.nNotifySkipDay))			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



