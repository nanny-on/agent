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
#include "ManageUserPolicy.h"

//---------------------------------------------------------------------------

CManageUserPolicy*	t_ManageUserPolicy = NULL;

//---------------------------------------------------------------------------

CManageUserPolicy::CManageUserPolicy()
{
	t_DBMgrUserPolicy	= new CDBMgrUserPolicy();
}
//---------------------------------------------------------------------------

CManageUserPolicy::~CManageUserPolicy()
{
	if(t_DBMgrUserPolicy)		{	delete t_DBMgrUserPolicy;		t_DBMgrUserPolicy = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageUserPolicy::LoadDBMS()
{
	TListDBUserPolicy tDBUserPolicyList;
    TListDBUserPolicyItor begin, end;
	if(SetER(t_DBMgrUserPolicy->LoadExecute(&tDBUserPolicyList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBUserPolicyList.begin();	end = tDBUserPolicyList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
		AddKeyID(begin->nUserID, begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageUserPolicy::AddUserPolicy(DB_USER_POLICY&	dup)
{
	if(SetER(t_DBMgrUserPolicy->InsertExecute(&dup)))
    {
    	return g_nErrRtn;
    }

    AddItem(dup.nID, dup);
    AddKeyID(dup.nUserID, dup.nID);
    
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageUserPolicy::EditUserPolicy(DB_USER_POLICY&	dup)
{
	PDB_USER_POLICY pdup = FindItem(dup.nID);
	if(!pdup)	return ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND;

    if(SetER(t_DBMgrUserPolicy->UpdateExecute(&dup)))
    {
    	return g_nErrRtn;
    }

    EditItem(dup.nID, dup);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageUserPolicy::DelUserPolicy(UINT32 nID)
{
	PDB_USER_POLICY pdup = FindItem(nID);
	if(!pdup)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrUserPolicy->DeleteExecute(pdup->nID)))
    {
    	return g_nErrRtn;
    }

    DelKeyID(pdup->nUserID);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

UINT32					CManageUserPolicy::GetUserPolicyID(UINT32 nUserID, UINT64 nPolicyType)
{
	PDB_USER pdp = t_ManageUser->FindItem(nUserID);
    if(!pdp)	return 0;

    PDB_USER_POLICY pdup = FindKeyItem(pdp->nID);
    if(!pdup)	return 0;

    return t_DeployPolicyUtil->GetPolicy(pdup->tDP, nPolicyType);
}
//---------------------------------------------------------------------------

INT32					CManageUserPolicy::GetUserIDMap(TMapID& tIDMap, UINT32 nPkgID, UINT64 nPolicyType)
{
	TMapDBUserPolicyItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
        UINT32 nChkPkgID = t_DeployPolicyUtil->GetPolicy(begin->second.tDP, nPolicyType);
        if(nChkPkgID != nPkgID)	continue;

        tIDMap[begin->second.nUserID] = begin->second.nID;
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageUserPolicy::GetUserIDList(TListID& tIDList, UINT32 nPkgID, UINT64 nPolicyType)
{
    TMapDBUserPolicyItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
        UINT32 nChkPkgID = t_DeployPolicyUtil->GetPolicy(begin->second.tDP, nPolicyType);
        if(nChkPkgID != nPkgID)	continue;

        tIDList.push_back(begin->second.nUserID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32				CManageUserPolicy::CheckUserPolicyStatus(TListID& tPIDList, UINT32 nPkgID, UINT64 nPolicyType, TListID& tApplyIDList, TListID& tReleaseIDList)
{
	TListID tCurHIDList;
    GetUserIDList(tCurHIDList, nPkgID, nPolicyType);

    {
    	TListIDItor begin, end;
        begin = tPIDList.begin();	end = tPIDList.end();
        for(begin; begin != end; begin++)
        {
            if(IsExistsIDFromList(tCurHIDList, *begin))	continue;

            tApplyIDList.push_back(*begin);
        }
    }

    {
    	TListIDItor begin, end;
        begin = tCurHIDList.begin();	end = tCurHIDList.end();
        for(begin; begin != end; begin++)
        {
         	if(IsExistsIDFromList(tPIDList, *begin))	continue;

            tReleaseIDList.push_back(*begin);
        }
    }
    return 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32				CManageUserPolicy::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());
    SendToken.TokenAdd_32(SS_POLICY_INDEX_TOTAL_NUMBER);

	TMapDBUserPolicyItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32				CManageUserPolicy::SetPkt(PDB_USER_POLICY pdup, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdup->nID);
    SendToken.TokenAdd_32(pdup->nUserID);    
	SendToken.TokenAdd_32(pdup->nRegDate);

	PDB_POLICY pDP = &(pdup->tDP);
	t_ManageBaseUtil->SetPktPolicy(pDP, SendToken);

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
