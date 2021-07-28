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

#ifndef ManageHostStatusPoH
#define ManageHostStatusPoH
//---------------------------------------------------------------------------

class CManageHostStatusPo : public CManageBase<DB_HOST_STATUS_PO>
{

public:
	INT32			LoadDBMS();

public:
	INT32				AddHostStatusPo(DB_HOST_STATUS_PO& dhsp);
    INT32				EditHostStatusPo(DB_HOST_STATUS_PO& dhsp);
	INT32				DelHostStatusPo(UINT32 nID);
	INT32				ApplyHostStatusPo(DB_HOST_STATUS_PO& dhsp);

	INT32				ClearDrvAndRunStatus(UINT32 nID);
    
public:
	INT32			GetPkt(MemToken& RecvToken, DB_HOST_STATUS_PO& dhsp);
    INT32			SetPkt(PDB_HOST_STATUS_PO pdhsp, MemToken& SendToken);

public:
	void 			RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManageHostStatusPo();
    ~CManageHostStatusPo();

};

extern CManageHostStatusPo*	t_ManageHostStatusPo;

#endif
