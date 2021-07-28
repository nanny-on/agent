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
#include "ManageHostPolicy.h"

//---------------------------------------------------------------------------

CManageHostPolicy*	t_ManageHostPolicy = NULL;

//---------------------------------------------------------------------------

CManageHostPolicy::CManageHostPolicy()
{
	t_DBMgrHostPolicy	= new CDBMgrHostPolicy();
}
//---------------------------------------------------------------------------

CManageHostPolicy::~CManageHostPolicy()
{
	if(t_DBMgrHostPolicy)		{	delete t_DBMgrHostPolicy;		t_DBMgrHostPolicy = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageHostPolicy::LoadDBMS()
{
	TListDBHostPolicy tDBHostPolicyList;
    TListDBHostPolicyItor begin, end;
	if(SetER(t_DBMgrHostPolicy->LoadExecute(&tDBHostPolicyList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBHostPolicyList.begin();	end = tDBHostPolicyList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
		AddKeyID(begin->nHostID, begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostPolicy::AddHostPolicy(DB_HOST_POLICY&	dhp)
{
	if(SetER(t_DBMgrHostPolicy->InsertExecute(&dhp)))
    {
    	return g_nErrRtn;
    }

    AddItem(dhp.nID, dhp);
	AddKeyID(dhp.nHostID, dhp.nID);
    
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostPolicy::EditHostPolicy(DB_HOST_POLICY&	dhp)
{
	PDB_HOST_POLICY pdhp = FindItem(dhp.nID);
	if(!pdhp)	return ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND;

    if(SetER(t_DBMgrHostPolicy->UpdateExecute(&dhp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dhp.nID, dhp);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostPolicy::DelHostPolicy(UINT32 nID)
{
	PDB_HOST_POLICY pdhp = FindItem(nID);
	if(!pdhp)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrHostPolicy->DeleteExecute(pdhp->nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyID(pdhp->nHostID);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

UINT32					CManageHostPolicy::GetHostPolicyID(UINT32 nHostID, UINT64 nPolicyType)
{
    PDB_HOST_POLICY pdhp = FirstItem();
    if(!pdhp)	return 0;

    return t_DeployPolicyUtil->GetPolicy(pdhp->tDP, nPolicyType);
}
//---------------------------------------------------------------------------

INT32					CManageHostPolicy::GetHostIDMap(TMapID& tIDMap, UINT32 nPkgID, UINT64 nPolicyType)
{
	TMapDBHostPolicyItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {	
		UINT32 nChkPkgID = t_DeployPolicyUtil->GetPolicy(begin->second.tDP, nPolicyType);
        if(nChkPkgID != nPkgID)	continue;

        tIDMap[begin->second.nHostID] = begin->second.nID;
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostPolicy::GetHostIDList(TListID& tIDList, UINT32 nPkgID, UINT64 nPolicyType)
{
    TMapDBHostPolicyItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
        UINT32 nChkPkgID = t_DeployPolicyUtil->GetPolicy(begin->second.tDP, nPolicyType);
        if(nChkPkgID != nPkgID)	continue;

        tIDList.push_back(begin->second.nHostID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32				CManageHostPolicy::CheckHostPolicyStatus(TListID& tHIDList, UINT32 nPkgID, UINT64 nPolicyType, TListID& tApplyIDList, TListID& tReleaseIDList)
{
	TListID tCurHIDList;
    GetHostIDList(tCurHIDList, nPkgID, nPolicyType);

    {
    	TListIDItor begin, end;
        begin = tHIDList.begin();	end = tHIDList.end();
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
         	if(IsExistsIDFromList(tHIDList, *begin))	continue;

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

INT32				CManageHostPolicy::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());
    SendToken.TokenAdd_32(SS_POLICY_INDEX_TOTAL_NUMBER);

	TMapDBHostPolicyItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32				CManageHostPolicy::SetPkt(PDB_HOST_POLICY pdhp, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdhp->nID);
    SendToken.TokenAdd_32(pdhp->nHostID);    
	SendToken.TokenAdd_32(pdhp->nRegDate);

	PDB_POLICY pDP = &(pdhp->tDP);
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
