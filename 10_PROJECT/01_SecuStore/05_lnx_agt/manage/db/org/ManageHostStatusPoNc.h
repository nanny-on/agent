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

#ifndef ManageHostStatusPoNcH
#define ManageHostStatusPoNcH
//---------------------------------------------------------------------------

class CManageHostStatusPoNc : public CManageBase<DB_HOST_STATUS_PO_NC>
{

public:
	INT32			LoadDBMS();

public:
	INT32				AddHostStatusPoNc(DB_HOST_STATUS_PO_NC& data);
    INT32				EditHostStatusPoNc(DB_HOST_STATUS_PO_NC& data);
	INT32				DelHostStatusPoNc(UINT32 nID);
	INT32				ApplyHostStatusPoNc(DB_HOST_STATUS_PO_NC& data);

	INT32				ClearDrvAndRunStatus(UINT32 nID);
    
public:
	INT32			GetPkt(MemToken& RecvToken, DB_HOST_STATUS_PO_NC& data);
    INT32			SetPkt(PDB_HOST_STATUS_PO_NC pdata, MemToken& SendToken);

public:
	void 			RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManageHostStatusPoNc();
    ~CManageHostStatusPoNc();

};

extern CManageHostStatusPoNc*	t_ManageHostStatusPoNc;

#endif
