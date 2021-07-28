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

#ifndef ManageLogEventH
#define ManageLogEventH
//---------------------------------------------------------------------------

class CManageLogEvent : public CManageBase<DB_LOG_EVENT>
{
public:
	INT32		LoadDBMS();

public:
	INT32					AddLogEvent(DB_LOG_EVENT&	dle);
    INT32					EditLogEvent(DB_LOG_EVENT&	dle);
    INT32					DelLogEvent(UINT32 nID);
	PDB_LOG_EVENT			FindLogEvent(UINT32 nID);

private:
	INT32					GetInitPktList(UINT32 nAdminID, TListPVOID& tInitPktList);

public:
	INT32					SetPktSync(TListPVOID& tIDList);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPktSyncMode(MemToken& SendToken, UINT32 nSyncMode);
	INT32					SetPktSync(MemToken& SendToken);
	INT32					SetPkt(MemToken& SendToken, UINT32 nAdminID);
	INT32					SetPkt(PDB_LOG_EVENT pdle, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_LOG_EVENT& dle);

public:
	CManageLogEvent();
    ~CManageLogEvent();

};

extern CManageLogEvent*	t_ManageLogEvent;

#endif
