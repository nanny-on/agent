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
#include "ManageHostVulnRstUnit.h"

//---------------------------------------------------------------------------

CManageHostVulnRstUnit*	t_ManageHostVulnRstUnit = NULL;

//---------------------------------------------------------------------------

CManageHostVulnRstUnit::CManageHostVulnRstUnit()
{
	t_DBMgrHostVulnRstUnit	= new CDBMgrHostVulnRstUnit();
}
//---------------------------------------------------------------------------

CManageHostVulnRstUnit::~CManageHostVulnRstUnit()
{
	if(t_DBMgrHostVulnRstUnit)		{	delete t_DBMgrHostVulnRstUnit;		t_DBMgrHostVulnRstUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRstUnit::LoadDBMS()
{
	TListDBHostVulnRstUnit tDBHostVulnRstUnitList;
    TListDBHostVulnRstUnitItor begin, end;
	if(SetER(t_DBMgrHostVulnRstUnit->LoadExecute(&tDBHostVulnRstUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBHostVulnRstUnitList.begin();	end = tDBHostVulnRstUnitList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
		AddKeyTypeID(begin->nVulnRstID, begin->tDHVRUS.nVulnID, begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostVulnRstUnit::AddHostVulnRstUnit(DB_HOST_VULN_RST_UNIT&	data)
{
	if(SetER(t_DBMgrHostVulnRstUnit->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    
	AddKeyTypeID(data.nVulnRstID, data.tDHVRUS.nVulnID, data.nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostVulnRstUnit::EditHostVulnRstUnit(DB_HOST_VULN_RST_UNIT&	data)
{
    if(t_DBMgrHostVulnRstUnit->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostVulnRstUnit::DelHostVulnRstUnit(UINT32 nID)
{
	PDB_HOST_VULN_RST_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrHostVulnRstUnit->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyTypeID(pdata->nVulnRstID, pdata->tDHVRUS.nVulnID);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostVulnRstUnit::ClearHostVulnRstUnit(UINT32 nVulnRstID)
{
	TListID tIDList;
	GetKeyTypeID_IDList(nVulnRstID, tIDList);

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		DelHostVulnRstUnit(*begin);
	}

	DelKeyTypeID(nVulnRstID);
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManageHostVulnRstUnit::GetPkt_Link(MemToken& RecvToken, DB_HOST_VULN_RST_UNIT& data)
{
	if (!RecvToken.TokenDel_32(data.nID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedFlag))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nVulnRstID))			goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.tDHVRUS.nVulnID))		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.tDHVRUS.nDScore))		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.tDHVRUS.nRst))			goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strRstInfo) < 0)	goto INVALID_PKT;

	RecvToken.TokenSkip_Block(); 
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRstUnit::SetPkt_Link(MemToken& SendToken)
{
	TListID tIDList;
	GetItemIDList(tIDList);
	
	SendToken.TokenAdd_32(tIDList.size());
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_HOST_VULN_RST_UNIT pdata = FindItem(*begin);
		if(!pdata)  	continue;

		SetPkt_Link(pdata, SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRstUnit::SetPkt_Link(TListID& tIDList, MemToken& SendToken)
{
	SendToken.TokenAdd_32(tIDList.size());
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{	
		PDB_HOST_VULN_RST_UNIT pdata = FindItem(*begin);
		if(!pdata)  	continue;

		SetPkt_Link(pdata, SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRstUnit::SetPkt_Link(PDB_HOST_VULN_RST_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedFlag);
	SendToken.TokenAdd_32(pdata->nRegDate);

	SendToken.TokenAdd_32(pdata->nVulnRstID);

	SendToken.TokenAdd_32(pdata->tDHVRUS.nVulnID);
	SendToken.TokenAdd_32(pdata->tDHVRUS.nDScore);
	SendToken.TokenAdd_32(pdata->tDHVRUS.nRst);

	SendToken.TokenAdd_String(pdata->strRstInfo);

	SendToken.TokenSet_Block();    
 	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------- 
void 		CManageHostVulnRstUnit::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][host_vuln_rst] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





