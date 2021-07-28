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

#ifndef ManageEnvSocketH
#define ManageEnvSocketH
//---------------------------------------------------------------------------

class CManageEnvSocket : public CManagePoBase<DB_ENV_SOCKET>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddEnvSocket(DB_ENV_SOCKET&	des);
    INT32					EditEnvSocket(DB_ENV_SOCKET&	des);
    INT32					DelEnvSocket(UINT32 nID);
	INT32					ApplyEnvSocket(DB_ENV_SOCKET&	des);

public:
	String					GetName(UINT32 nID);    
	INT32					SetSocketErrCode(UINT32 nSocketType, UINT32 nErrCode = 0);
	INT32					GetSocketErrCode(UINT32 nSocketType);
	INT32					DelSocketErrCode(UINT32 nSocketType);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_ENV_SOCKET pdes, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_ENV_SOCKET& des);

public:
	CManageEnvSocket();
    ~CManageEnvSocket();

};

extern CManageEnvSocket*	t_ManageEnvSocket;

#endif
