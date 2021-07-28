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
#include "ManageVulnFileProtect.h"

//---------------------------------------------------------------------------

CManageVulnFileProtect*	t_ManageVulnFileProtect = NULL;

//---------------------------------------------------------------------------

CManageVulnFileProtect::CManageVulnFileProtect()
{
}
//---------------------------------------------------------------------------

CManageVulnFileProtect::~CManageVulnFileProtect()
{
}
//---------------------------------------------------------------------------

INT32		CManageVulnFileProtect::LoadDBMS()
{
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageVulnFileProtect::AddVulnFileProtect(DB_VULN_FILE_PROTECT&	data)
{
	AddItem(data.nID, data);  
	AddKeyIDList(data.nSVulnID, data.nID);
	
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageVulnFileProtect::EditVulnFileProtect(DB_VULN_FILE_PROTECT&	data)
{
	PDB_VULN_FILE_PROTECT pdata = FindItem(data.nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	{
		DelKeyIDList(pdata->nSVulnID, pdata->nID);
		EditItem(data.nID, data);
		AddKeyIDList(data.nSVulnID, data.nID);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageVulnFileProtect::DelVulnFileProtect(UINT32 nID)
{
	PDB_VULN_FILE_PROTECT pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	DelKeyIDList(pdata->nSVulnID, nID);
	DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageVulnFileProtect::ApplyVulnFileProtect(DB_VULN_FILE_PROTECT&	data)
{
	if(FindItem(data.nID))
	{
		return EditVulnFileProtect(data);
	}
	return AddVulnFileProtect(data);
}
//---------------------------------------------------------------------------

String					CManageVulnFileProtect::GetName(UINT32 nID)
{
	PDB_VULN_FILE_PROTECT pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManageVulnFileProtect::GetPkt(MemToken& RecvToken, DB_VULN_FILE_PROTECT& data)
{
	if(!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nUsedFlag))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nRegDate))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nAdminID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nExtOption))					goto INVALID_PKT;

	if( RecvToken.TokenDel_String(data.strName) < 0)			goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strDescr) < 0)			goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(data.nSVulnID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nSVulnSubID))				goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(data.nLockType))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nCustomID))					goto INVALID_PKT;

	if( RecvToken.TokenDel_String(data.strLockPath) < 0)		goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nLockAnyPerm))				goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strExProcPath) < 0)		goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nExProcPerm))				goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------


INT32		CManageVulnFileProtect::SetPkt(MemToken& SendToken)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	SendToken.TokenAdd_32(tIDList.size());

	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_VULN_FILE_PROTECT pdata = FindItem(*begin);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}
	return tIDList.size();
}
//---------------------------------------------------------------------------

INT32		CManageVulnFileProtect::SetPkt(PDB_VULN_FILE_PROTECT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedFlag);
	SendToken.TokenAdd_32(pdata->nRegDate);
	SendToken.TokenAdd_32(pdata->nUsedMode);
	SendToken.TokenAdd_32(pdata->nAdminID);
	SendToken.TokenAdd_32(pdata->nExtOption);

	SendToken.TokenAdd_String(pdata->strName);
	SendToken.TokenAdd_String(pdata->strDescr);

	SendToken.TokenAdd_32(pdata->nSVulnID);
	SendToken.TokenAdd_32(pdata->nSVulnSubID);

	SendToken.TokenAdd_32(pdata->nLockType);
	SendToken.TokenAdd_32(pdata->nCustomID);

	SendToken.TokenAdd_String(pdata->strLockPath);
	SendToken.TokenAdd_32(pdata->nLockAnyPerm);
	SendToken.TokenAdd_String(pdata->strExProcPath);
	SendToken.TokenAdd_32(pdata->nExProcPerm);

	SendToken.TokenSet_Block();
 	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageVulnFileProtect::MakeObject(UINT32 nID, POBJECT_ENTRY* pObjList, UINT32& nObjRulNum, UINT32 nType)
{
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

void 		CManageVulnFileProtect::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][site_sw] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------



