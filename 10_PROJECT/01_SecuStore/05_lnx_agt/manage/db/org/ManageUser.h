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

#ifndef ManageUserH
#define ManageUserH
//---------------------------------------------------------------------------

class CManageUser : public CManageBase<DB_USER>
{

public:
	INT32			LoadDBMS();

public:
	INT32			InitHash();
	INT32			InitHash(UINT32 nID);

private:
	TMapID			m_tNonGroupIDMap;

public:
	INT32			AddUser(DB_USER& du);
    INT32			EditUser(DB_USER& du);
	INT32			EditUser_GroupID(UINT32 nID, UINT32 nGroupID);
	INT32			DelUser(UINT32 nID);
	INT32			ApplyUser(DB_USER& du);

public:
	UINT32			GetGroupID(UINT32 nUserID);
	UINT32			GetOwnerAdminID(UINT32 nUserID);
	String			GetNameForEvt(UINT32 nUserID);

public:
	INT32			AddNonGroupUserID(UINT32 nID);
	INT32			DelNonGroupUserID(UINT32 nID);
	INT32			ClearNonGroupUserID();
	INT32			IsNonGroupUserID(UINT32 nID);
	INT32			GetNonGroupUserIDList(TListID& tIDList);

public:
	INT32			ClearLinkInformation(DB_USER& du);

	PDB_USER		FindUserAccountID(String strAccountID);

public:
	INT32			SetPkt(MemToken& SendToken);
	INT32			SetPktMgr(MemToken& SendToken);
	INT32			SetPkt(PDB_USER pdu, MemToken& SendToken);
	INT32			GetPkt(MemToken& RecvToken, DB_USER& du);

public:
	void 			PrintDbgInfo();

public:
	CManageUser();
    ~CManageUser();

};

extern CManageUser*	t_ManageUser;

#endif
