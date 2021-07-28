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

#ifndef ManageInfoProcReputationH
#define ManageInfoProcReputationH
//---------------------------------------------------------------------------

class CManageInfoProcReputation : public CManageBase<DB_INFO_PROC_REPUTATION>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddInfoProcReputation(DB_INFO_PROC_REPUTATION&	data);
    INT32					EditInfoProcReputation(DB_INFO_PROC_REPUTATION&	data);
	INT32					DelInfoProcReputation(UINT32 nID);
	INT32					ApplyInfoProcReputation(DB_INFO_PROC_REPUTATION&	data);

public:
	UINT64					IsInvalidRight(UINT64 nPackage, UINT64 nPolicy, UINT64 nControl);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_INFO_PROC_REPUTATION pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_INFO_PROC_REPUTATION& data);

public:
	CManageInfoProcReputation();
    ~CManageInfoProcReputation();

};

extern CManageInfoProcReputation*	t_ManageInfoProcReputation;

#endif
