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

#ifndef ManageHostPatchH
#define ManageHostPatchH
//---------------------------------------------------------------------------

class CManageHostPatch : public CManageBase<DB_HOST_PATCH>
{

public:
	INT32				LoadDBMS();

public:
	INT32				AddHostPatch(DB_HOST_PATCH& data);
    INT32				EditHostPatch(DB_HOST_PATCH& data);
    INT32				DelHostPatch(UINT32 nID);

public:
	PDB_HOST_PATCH		FindChkKey(PDB_HOST_PATCH pdata);

public:
	String				GetKey(PDB_HOST_PATCH pdata);
	INT32				IsExistLocalPatch(TListDBHostPatch* tPatchInfoList, PDB_HOST_PATCH pdata);

public:
	INT32				GetPkt(MemToken& RecvToken, DB_HOST_PATCH& data);
	INT32				SetPkt(MemToken& SendToken);
	INT32				SetPkt(TListDBHostPatch& tDBHostPatchList, MemToken& SendToken);
	INT32				SetPkt(PDB_HOST_PATCH pdata, MemToken& SendToken);	

public:
	void 				RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManageHostPatch();
    ~CManageHostPatch();

};

extern CManageHostPatch*	t_ManageHostPatch;

#endif
