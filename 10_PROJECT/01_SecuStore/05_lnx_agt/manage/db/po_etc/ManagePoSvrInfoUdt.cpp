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
#include "ManagePoSvrInfoUdt.h"

//---------------------------------------------------------------------------

CManagePoSvrInfoUdt*	t_ManagePoSvrInfoUdt = NULL;

//---------------------------------------------------------------------------

CManagePoSvrInfoUdt::CManagePoSvrInfoUdt()
{
	t_DBMgrPoSvrInfoUdt	= new CDBMgrPoSvrInfoUdt();
}
//---------------------------------------------------------------------------

CManagePoSvrInfoUdt::~CManagePoSvrInfoUdt()
{
	if(t_DBMgrPoSvrInfoUdt)	{	delete t_DBMgrPoSvrInfoUdt;	t_DBMgrPoSvrInfoUdt = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoSvrInfoUdt::LoadDBMS()
{
	TListDBPoSvrInfoUdt tDBPoSvrInfoUdtList;
    TListDBPoSvrInfoUdtItor begin, end;
	if(SetER(t_DBMgrPoSvrInfoUdt->LoadExecute(&tDBPoSvrInfoUdtList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoSvrInfoUdtList.begin();	end = tDBPoSvrInfoUdtList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoUdt::InitHash()
{
	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		InitHash(*begin);
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoUdt::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_SVR_INFO_UDT pdpsiu 			= NULL;
	{
		if( (pdpsiu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_svr_info_udt by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s\f%s\f", pdpsiu->strChkSvrInfoList.c_str(), pdpsiu->strDataSvrInfoList.c_str());
	}

	{
		pdpsiu->tDPH.strHash = strOrgValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoUdt::AddPoSvrInfoUdt(DB_PO_SVR_INFO_UDT&	dpsiu)
{
	if(SetER(t_DBMgrPoSvrInfoUdt->InsertExecute(&dpsiu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpsiu.tDPH.nID, dpsiu);
	InitHash(dpsiu.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoUdt::EditPoSvrInfoUdt(DB_PO_SVR_INFO_UDT&	dpsiu)
{
	PDB_PO_SVR_INFO_UDT pdpsiu = FindItem(dpsiu.tDPH.nID);
	if(!pdpsiu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoSvrInfoUdt->UpdateExecute(&dpsiu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpsiu.tDPH.nID, dpsiu);
	InitHash(dpsiu.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoUdt::DelPoSvrInfoUdt(UINT32 nID)
{
	PDB_PO_SVR_INFO_UDT pdpsiu = FindItem(nID);
	if(!pdpsiu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoSvrInfoUdt->DeleteExecute(pdpsiu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoUdt::ApplyPoSvrInfoUdt(DB_PO_SVR_INFO_UDT&	dpsiu)
{
	if(FindItem(dpsiu.tDPH.nID))
	{
		return EditPoSvrInfoUdt(dpsiu);
	}
	return AddPoSvrInfoUdt(dpsiu);
}
//---------------------------------------------------------------------------

String					CManagePoSvrInfoUdt::GetName(UINT32 nID)
{
	PDB_PO_SVR_INFO_UDT pdpsiu = FindItem(nID);
    if(!pdpsiu)	return "";
    return pdpsiu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoUdt::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoSvrInfoUdtItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoUdt::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SVR_INFO_UDT pdpsiu = FindItem(nID);
	if(!pdpsiu)	return -1;

	return SetPkt(pdpsiu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoUdt::SetPkt(PDB_PO_SVR_INFO_UDT pdpsiu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpsiu->tDPH);

	SendToken.TokenAdd_String(pdpsiu->strChkSvrInfoList);
	SendToken.TokenAdd_String(pdpsiu->strDataSvrInfoList);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSvrInfoUdt::GetPkt(MemToken& RecvToken, DB_PO_SVR_INFO_UDT& dpsiu)
{
	if (!RecvToken.TokenDel_DPH(dpsiu.tDPH))							goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dpsiu.strChkSvrInfoList) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpsiu.strDataSvrInfoList) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



