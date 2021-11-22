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

#ifndef ManageUserPolicyH
#define ManageUserPolicyH
//---------------------------------------------------------------------------

class CManageUserPolicy : public CManageBase<DB_USER_POLICY>
{
public:
	INT32					LoadDBMS();
    INT32					InitUserPolicy();

public:
	INT32					AddUserPolicy(DB_USER_POLICY&	dup);
    INT32					EditUserPolicy(DB_USER_POLICY&	dup);
    INT32					DelUserPolicy(UINT32 nID);
	PDB_USER_POLICY			FindUserPolicy_UserID(UINT32 nUserID);

public:
	UINT32					GetUserPolicyID(UINT32 nUserID, UINT64 nPolicyType);

public:
	INT32					GetUserIDMap(TMapID& tIDMap, UINT32 nPkgID, UINT64 nPolicyType);
    INT32					GetUserIDList(TListID& tIDList, UINT32 nPkgID, UINT64 nPolicyType);
    INT32					CheckUserPolicyStatus(TListID& tHIDList, UINT32 nPkgID, UINT64 nPolicyType, TListID& tApplyIDList, TListID& tReleaseIDList);  

public:
	INT32				SetPkt(MemToken& SendToken);
    INT32				SetPkt(PDB_USER_POLICY pdup, MemToken& SendToken);


public:
	CManageUserPolicy();
    ~CManageUserPolicy();

};

extern CManageUserPolicy*	t_ManageUserPolicy;

#endif
