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

#ifndef ManageHostPolicyH
#define ManageHostPolicyH
//---------------------------------------------------------------------------

class CManageHostPolicy : public CManageBase<DB_HOST_POLICY>
{
public:
	INT32					LoadDBMS();

public:
	INT32					AddHostPolicy(DB_HOST_POLICY&	dhp);
    INT32					EditHostPolicy(DB_HOST_POLICY&	dhp);
    INT32					DelHostPolicy(UINT32 nID);
	PDB_HOST_POLICY			FindHostPolicy_HostID(UINT32 nHostID);

public:
	UINT32					GetHostPolicyID(UINT32 nHostID, UINT64 nPolicyType);

public:
	INT32					GetHostIDMap(TMapID& tIDMap, UINT32 nPkgID, UINT64 nPolicyType);
    INT32					GetHostIDList(TListID& tIDList, UINT32 nPkgID, UINT64 nPolicyType);
    INT32					CheckHostPolicyStatus(TListID& tHIDList, UINT32 nPkgID, UINT64 nPolicyType, TListID& tApplyIDList, TListID& tReleaseIDList);  

public:
	INT32				SetPkt(MemToken& SendToken);
    INT32				SetPkt(PDB_HOST_POLICY pdhp, MemToken& SendToken);


public:
	CManageHostPolicy();
    ~CManageHostPolicy();

};

extern CManageHostPolicy*	t_ManageHostPolicy;

#endif
