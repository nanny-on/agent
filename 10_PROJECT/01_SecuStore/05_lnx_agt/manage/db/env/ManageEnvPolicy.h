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

#ifndef ManageEnvPolicyH
#define ManageEnvPolicyH
//---------------------------------------------------------------------------

class CManageEnvPolicy : public CManagePoBase<DB_ENV_POLICY>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddEnvPolicy(DB_ENV_POLICY&	dep);
    INT32					EditEnvPolicy(DB_ENV_POLICY&	dep);
    INT32					DelEnvPolicy(UINT32 nID);

public:
	String					GetName(UINT32 nID);    
	UINT32					GetCurPolicyType();

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_ENV_POLICY pdep, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_ENV_POLICY& dep);

public:
	CManageEnvPolicy();
    ~CManageEnvPolicy();

};

extern CManageEnvPolicy*	t_ManageEnvPolicy;

#endif
