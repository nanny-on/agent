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
#include "ManageUser.h"

//---------------------------------------------------------------------------

CManageUser*	t_ManageUser = NULL;

//---------------------------------------------------------------------------

CManageUser::CManageUser()
{
	t_DBMgrUser	= new CDBMgrUser();
}
//---------------------------------------------------------------------------

CManageUser::~CManageUser()
{
	if(t_DBMgrUser)		{	delete t_DBMgrUser;		t_DBMgrUser = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageUser::LoadDBMS()
{
	TListDBUser tDBUserList;
    TListDBUserItor begin, end;
	if(SetER(t_DBMgrUser->LoadExecute(&tDBUserList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBUserList.begin();	end = tDBUserList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
		AddSKeyID(begin->strUniCode, begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageUser::InitHash()
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

INT32					CManageUser::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_USER pDU 			= NULL;
	{
		if( (pDU = FindItem(nID)) == NULL)
		{
			WriteLogE("not find user by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%u,%u,%u,"
							"%u,%u,%s,%s,%s,"
							"%s,%s,%s,"
							"%s,%s,%s,%s,%s,%s,"
							"%s,%u,", 
							pDU->nUsedFlag, pDU->nRegDate, pDU->nInsType, 
							pDU->nGroupID, pDU->nUGroupID, pDU->strUniCode.c_str(), pDU->strAccountID.c_str(), pDU->strAccountPW.c_str(),
							pDU->strGCode.c_str(), pDU->strName.c_str(), pDU->strCoID.c_str(),
							pDU->strCoRank.c_str(), pDU->strCoRespon.c_str(), pDU->strCoEMail.c_str(), pDU->strCoPhone.c_str(), pDU->strUserHPhone.c_str(), pDU->strSrcGroupName.c_str(),
							pDU->strDescr.c_str(), pDU->nExtOption);	
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pDU->strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageUser::AddUser(DB_USER&	du)
{
	if(SetER(t_DBMgrUser->InsertExecute(&du)))
    {
    	return g_nErrRtn;
    }

	AddItem(du.nID, du); 
	AddSKeyID(du.strUniCode, du.nID); 
	InitHash(du.nID);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageUser::EditUser(DB_USER&	du)
{
    if(t_DBMgrUser->UpdateExecute(&du))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(du.nID, du);
	InitHash(du.nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageUser::EditUser_GroupID(UINT32 nID, UINT32 nGroupID)
{
	PDB_USER pdu = FindItem(nID);
	if(!pdu)
		return ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND;

//	UINT32 nOldGroupID 	= pdu->nGroupID; 
	pdu->nGroupID		= nGroupID;

	if(t_DBMgrUser->UpdateExecute(pdu))
		return ERR_DBMS_UPDATE_FAIL;

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageUser::DelUser(UINT32 nID)
{
	PDB_USER pdu = FindItem(nID);
	if(!pdu)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrUser->DeleteExecute(pdu->nID)))
	{
		return g_nErrRtn;
	}

    DelSKeyID(pdu->strUniCode);
	DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageUser::ApplyUser(DB_USER& du)
{
	if(FindItem(du.nID))
	{
		return EditUser(du);
	}
	return AddUser(du);
}
//---------------------------------------------------------------------------

INT32					CManageUser::ClearLinkInformation(DB_USER& du)
{
	t_ManageUserPolicy->DelUserPolicy(t_ManageUserPolicy->FindKeyID(du.nID));

	return 0;
}
//---------------------------------------------------------------------------

PDB_USER				CManageUser::FindUserAccountID(String strAccountID)
{
	TMapDBUserItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.strAccountID.compare(strAccountID))	continue;
		return &(begin->second);
	}
	return NULL;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

UINT32					CManageUser::GetGroupID(UINT32 nUserID)
{
	PDB_USER pdu = FindItem(nUserID);
	if(!pdu)	return 0;

	return pdu->nGroupID;
}
//---------------------------------------------------------------------------

UINT32					CManageUser::GetOwnerAdminID(UINT32 nUserID)
{
	return 0;
}
//---------------------------------------------------------------------------

String					CManageUser::GetNameForEvt(UINT32 nUserID)
{
	PDB_USER pdu = FindItem(nUserID);
	if(!pdu)	return "";

	return pdu->strName;
}
//---------------------------------------------------------------------------

INT32			CManageUser::AddNonGroupUserID(UINT32 nID)
{
	m_tNonGroupIDMap[nID] = 0;
	return 0;
}
//---------------------------------------------------------------------------

INT32			CManageUser::DelNonGroupUserID(UINT32 nID)
{
	m_tNonGroupIDMap.erase(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32			CManageUser::ClearNonGroupUserID()
{
	m_tNonGroupIDMap.clear();
	return 0;
}
//---------------------------------------------------------------------------

INT32			CManageUser::IsNonGroupUserID(UINT32 nID)
{
	TMapIDItor find = m_tNonGroupIDMap.find(nID);
	if(find == m_tNonGroupIDMap.end())	return 0;
	return 1;
}
//---------------------------------------------------------------------------


INT32			CManageUser::GetNonGroupUserIDList(TListID& tIDList)
{
	ConvertMapToList(m_tNonGroupIDMap, tIDList);
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CManageUser::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBUserItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageUser::SetPktMgr(MemToken& SendToken)
{
	PDB_USER pdu = FirstItem();
	if(!pdu)
	{
		SendToken.TokenAdd_32(0);
		return 0;
	}

	{
		SendToken.TokenAdd_32(1);
		SendToken.TokenAdd_32(pdu->nID);
		SendToken.TokenAdd_String(pdu->strHash);

		SendToken.TokenSet_Block();
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageUser::SetPkt(PDB_USER pdu, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdu->nID);
	SendToken.TokenAdd_32(pdu->nRegDate);
	SendToken.TokenAdd_32(pdu->nUsedFlag);
	SendToken.TokenAdd_32(pdu->nInsType);

	SendToken.TokenAdd_32(pdu->nGroupID);
	SendToken.TokenAdd_32(pdu->nUGroupID);
	SendToken.TokenAdd_String(pdu->strUniCode);
	SendToken.TokenAdd_String(pdu->strAccountID);
	SendToken.TokenAdd_String(pdu->strAccountPW);

	SendToken.TokenAdd_String(pdu->strName);
	SendToken.TokenAdd_String(pdu->strGCode);
	SendToken.TokenAdd_String(pdu->strCoID);
	SendToken.TokenAdd_String(pdu->strCoRank);
	SendToken.TokenAdd_String(pdu->strCoRespon);
	SendToken.TokenAdd_String(pdu->strCoEMail);
	SendToken.TokenAdd_String(pdu->strCoPhone);
	SendToken.TokenAdd_String(pdu->strUserHPhone);
	SendToken.TokenAdd_String(pdu->strDescr);

	SendToken.TokenAdd_32(pdu->nExtOption);

	SendToken.TokenAdd_String(pdu->strSrcGroupName);

	SendToken.TokenSet_Block();
 	return 0;
}
//---------------------------------------------------------------------------

INT32	CManageUser::GetPkt(MemToken& RecvToken, DB_USER& du)
{
	if (!RecvToken.TokenDel_32(du.nID))							goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(du.nRegDate))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(du.nUsedFlag))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(du.nInsType))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(du.nGroupID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(du.nUGroupID))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strUniCode) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strAccountID) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strAccountPW) < 0)		goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(du.strName) < 0)				goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strGCode) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strCoID) < 0)				goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strCoRank) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strCoRespon) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strCoEMail) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strCoPhone) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strUserHPhone) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(du.strDescr) < 0)			goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(du.nExtOption))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(du.strSrcGroupName) < 0)			goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
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

void 		CManageUser::PrintDbgInfo()
{
	WriteLogN("remain manage item cnt [%s] : %u", t_DBMgrUser->m_strDBTName.c_str(), m_tMap.size());
    return;
}
//---------------------------------------------------------------------------





