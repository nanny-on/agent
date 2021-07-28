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

#ifndef ManageLogPatchH
#define ManageLogPatchH
//---------------------------------------------------------------------------

class CManageLogPatch : public CManageBase<DB_LOG_PATCH>
{
public:
	INT32					LoadDBMS();

public:
	INT32					AddLogPatch(DB_LOG_PATCH&	data);
    INT32					EditLogPatch(DB_LOG_PATCH&	data);
    INT32					DelLogPatch(UINT32 nID);

public:
	INT32					IsExistLocalPatch(TListDBLogPatch* tPatchInfoList, PDB_LOG_PATCH pdata);
	INT32					IsRestartSystem();
	INT32					GetApplyNum(UINT32 nID);


public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(TListDBLogPatch& tLogList, MemToken& SendToken);
	INT32					SetPkt(PDB_LOG_PATCH pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_LOG_PATCH& data);

	INT32					SetPkt_Link(MemToken& SendToken);
	INT32					SetPkt_Link(TListDBLogPatch& tLogList, MemToken& SendToken);
	INT32					SetPkt_Link(PDB_LOG_PATCH pdata, MemToken& SendToken);
	INT32					GetPkt_Link(MemToken& RecvToken, DB_LOG_PATCH& data);

public:
	CManageLogPatch();
    ~CManageLogPatch();

};

extern CManageLogPatch*	t_ManageLogPatch;

#endif
