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

#ifndef ManageEnvTrustSubjectH
#define ManageEnvTrustSubjectH
//---------------------------------------------------------------------------

class CManageEnvTrustSubject : public CManagePoBase<DB_ENV_TRUST_SUBJECT>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddEnvTrustSubject(DB_ENV_TRUST_SUBJECT&	dets);
    INT32					EditEnvTrustSubject(DB_ENV_TRUST_SUBJECT&	dets);
	INT32					DelEnvTrustSubject(UINT32 nID);
	INT32					ApplyEnvTrustSubject(DB_ENV_TRUST_SUBJECT&	dets);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_ENV_TRUST_SUBJECT pdets, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_ENV_TRUST_SUBJECT& dets);

public:
	INT32					SetPktDbg(MemToken& SendToken);

public:
	CManageEnvTrustSubject();
    ~CManageEnvTrustSubject();

};

extern CManageEnvTrustSubject*	t_ManageEnvTrustSubject;

#endif
