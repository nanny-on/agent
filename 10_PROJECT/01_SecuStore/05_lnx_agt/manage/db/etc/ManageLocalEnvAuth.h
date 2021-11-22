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

#ifndef ManageLocalEnvAuthH
#define ManageLocalEnvAuthH
//---------------------------------------------------------------------------

class CManageLocalEnvAuth : public CManageBase<DB_LOCAL_ENV_AUTH>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddLocalEnvAuth(DB_LOCAL_ENV_AUTH&	dlea);
    INT32					EditLocalEnvAuth(DB_LOCAL_ENV_AUTH&	dlea);
	INT32					DelLocalEnvAuth(UINT32 nID);
	INT32					ApplyLocalEnvAuth(DB_LOCAL_ENV_AUTH&	dlea);

public:
	UINT64					IsInvalidRight(UINT64 nPackage, UINT64 nPolicy, UINT64 nControl);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_LOCAL_ENV_AUTH pdlea, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_LOCAL_ENV_AUTH& dlea);

public:
	CManageLocalEnvAuth();
    ~CManageLocalEnvAuth();

};

extern CManageLocalEnvAuth*	t_ManageLocalEnvAuth;

#endif
