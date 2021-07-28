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
#include "ManagePoHostRmInfo.h"

//---------------------------------------------------------------------------

CManagePoHostRmInfo*	t_ManagePoHostRmInfo = NULL;

//---------------------------------------------------------------------------

CManagePoHostRmInfo::CManagePoHostRmInfo()
{
	t_DBMgrPoHostRmInfo	= new CDBMgrPoHostRmInfo();
}
//---------------------------------------------------------------------------

CManagePoHostRmInfo::~CManagePoHostRmInfo()
{
	if(t_DBMgrPoHostRmInfo)	{	delete t_DBMgrPoHostRmInfo;	t_DBMgrPoHostRmInfo = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoHostRmInfo::LoadDBMS()
{
	TListDBPoHostRmInfo tDBPoHostRmInfoList;
    TListDBPoHostRmInfoItor begin, end;
	if(SetER(t_DBMgrPoHostRmInfo->LoadExecute(&tDBPoHostRmInfoList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoHostRmInfoList.begin();	end = tDBPoHostRmInfoList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRmInfo::InitHash()
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

INT32					CManagePoHostRmInfo::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_HOST_RM_INFO pdphri 			= NULL;
	{
		if( (pdphri = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_host_rm_info by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,%u,%s,", 
							GetHdrHashInfo(pdphri).c_str(),
							pdphri->nRmPwType, pdphri->strRmPw.c_str());
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdphri->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRmInfo::AddPoHostRmInfo(DB_PO_HOST_RM_INFO&	dphri)
{
	if(SetER(t_DBMgrPoHostRmInfo->InsertExecute(&dphri)))
    {
    	return g_nErrRtn;
    }

    AddItem(dphri.tDPH.nID, dphri);
	InitHash(dphri.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRmInfo::EditPoHostRmInfo(DB_PO_HOST_RM_INFO&	dphri)
{
	PDB_PO_HOST_RM_INFO pdphri = FindItem(dphri.tDPH.nID);
	if(!pdphri)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoHostRmInfo->UpdateExecute(&dphri)))
    {
    	return g_nErrRtn;
    }

    EditItem(dphri.tDPH.nID, dphri);
	InitHash(dphri.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRmInfo::DelPoHostRmInfo(UINT32 nID)
{
	PDB_PO_HOST_RM_INFO pdphri = FindItem(nID);
	if(!pdphri)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoHostRmInfo->DeleteExecute(pdphri->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRmInfo::ApplyPoHostRmInfo(DB_PO_HOST_RM_INFO&	dphri)
{
	if(FindItem(dphri.tDPH.nID))
	{
		return EditPoHostRmInfo(dphri);
	}
	return AddPoHostRmInfo(dphri);
}
//---------------------------------------------------------------------------

String					CManagePoHostRmInfo::GetName(UINT32 nID)
{
	PDB_PO_HOST_RM_INFO pdphri = FindItem(nID);
    if(!pdphri)	return "";
    return pdphri->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoHostRmInfo::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoHostRmInfoItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRmInfo::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_HOST_RM_INFO pdphri = FindItem(nID);
	if(!pdphri)	return -1;

	return SetPkt(pdphri, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRmInfo::SetPkt(PDB_PO_HOST_RM_INFO pdphri, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdphri->tDPH);
	
	SendToken.TokenAdd_32(pdphri->nRmPwType);
	SendToken.TokenAdd_String(pdphri->strRmPw);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRmInfo::GetPkt(MemToken& RecvToken, DB_PO_HOST_RM_INFO& dphri)
{
	if (!RecvToken.TokenDel_DPH(dphri.tDPH))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dphri.nRmPwType))			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dphri.strRmPw) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



