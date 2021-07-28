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
#include "ManageHostHw.h"

//---------------------------------------------------------------------------

CManageHostHw*	t_ManageHostHw = NULL;

//---------------------------------------------------------------------------

CManageHostHw::CManageHostHw()
{
	t_DBMgrHostHw	= new CDBMgrHostHw();
}
//---------------------------------------------------------------------------

CManageHostHw::~CManageHostHw()
{
	if(t_DBMgrHostHw)		{	delete t_DBMgrHostHw;		t_DBMgrHostHw = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageHostHw::LoadDBMS()
{
	DB_HOST_HW dhhw;
	TListDBHostHw tDBHostHwList;
    TListDBHostHwItor begin, end;
	
	if(SetER(t_DBMgrHostHw->LoadExecute(&tDBHostHwList)))
    {
		if(tDBHostHwList.size() == 0)
		{
			memset(&dhhw, 0, sizeof(DB_HOST_HW));
			AddHostHw(dhhw);
		}
    	return g_nErrRtn;
    }

    begin = tDBHostHwList.begin();	end = tDBHostHwList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
    }

	if(tDBHostHwList.size() == 0)
	{
		memset(&dhhw, 0, sizeof(DB_HOST_HW));
		AddHostHw(dhhw);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostHw::AddHostHw(DB_HOST_HW&	dhhw)
{
	if(SetER(t_DBMgrHostHw->InsertExecute(&dhhw)))
    {
    	return g_nErrRtn;
    }

    AddItem(dhhw.nID, dhhw);   

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostHw::EditHostHw(DB_HOST_HW&	dhhw)
{
    if(t_DBMgrHostHw->UpdateExecute(&dhhw))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(dhhw.nID, dhhw);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostHw::DelHostHw(UINT32 nID)
{
	PDB_HOST_HW pdhhw = FindItem(nID);
	if(!pdhhw)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrHostHw->DeleteExecute(pdhhw->nID)))
    {
    	return g_nErrRtn;
    }
                             
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32	CManageHostHw::GetPkt(MemToken& RecvToken, DB_HOST_HW& dhhw)
{	
	if (!RecvToken.TokenDel_32(dhhw.nPCType))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dhhw.strCPUName) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhhw.nMemSize))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhhw.nHDDSize))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhhw.nLanNum))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dhhw.strBiosDate) < 0)		goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManageHostHw::SetPkt(PDB_HOST_HW pdhhw, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdhhw->nPCType);
	SendToken.TokenAdd_String(pdhhw->strCPUName);
	SendToken.TokenAdd_32(pdhhw->nMemSize);
	SendToken.TokenAdd_32(pdhhw->nHDDSize);
	SendToken.TokenAdd_32(pdhhw->nLanNum);
	SendToken.TokenAdd_String(pdhhw->strBiosDate);

	SendToken.TokenSet_Block();
    
 	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManageHostHw::GetPktHost(MemToken& RecvToken, DB_HOST_HW& dhhw)
{	
	if (!RecvToken.TokenDel_32(dhhw.nPCType))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dhhw.strCPUName) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhhw.nMemSize))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhhw.nHDDSize))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dhhw.nLanNum))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dhhw.strBiosDate) < 0)		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------

INT32		CManageHostHw::SetPktHost(PDB_HOST_HW pdhhw, MemToken& SendToken)
{	
	SendToken.TokenAdd_32(pdhhw->nPCType);
	SendToken.TokenAdd_String(pdhhw->strCPUName);
	SendToken.TokenAdd_32(pdhhw->nMemSize);
	SendToken.TokenAdd_32(pdhhw->nHDDSize);
	SendToken.TokenAdd_32(pdhhw->nLanNum);
	SendToken.TokenAdd_String(pdhhw->strBiosDate);

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





