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

#ifndef ManageHostSysH
#define ManageHostSysH
//---------------------------------------------------------------------------

class CManageHostSys : public CManageBase<DB_HOST_SYS>
{
public:
	INT32			LoadDBMS();

public:
	INT32			AddHostSys(DB_HOST_SYS& dhs);
    INT32			EditHostSys(DB_HOST_SYS& dhs);
    INT32			DelHostSys(UINT32 nID);

public:
	INT32			GetPkt(MemToken& RecvToken, DB_HOST_SYS& dhs, UINT32 nPktType);
	INT32			SetPkt(MemToken& SendToken);
    INT32			SetPkt(PDB_HOST_SYS pdhs, MemToken& SendToken);

	// 20200724 add jhjung
public:
	INT32			SetHostSysType(UINT64 nOsID);

public:
	CManageHostSys();
    ~CManageHostSys();

};

extern CManageHostSys*	t_ManageHostSys;

#endif
