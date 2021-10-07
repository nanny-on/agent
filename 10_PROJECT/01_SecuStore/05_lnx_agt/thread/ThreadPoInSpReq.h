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

#ifndef _THREAD_PO_IN_SP_REQ_H__
#define _THREAD_PO_IN_SP_REQ_H__

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInSpReq thread

class CThreadPoInSpReq : public CThreadBase
{
public:
	CThreadPoInSpReq();
	virtual ~CThreadPoInSpReq();
private:
	char m_acFiFoPath[MAX_PATH];
	INT32 InitEnv();
	INT32 FreeEnv();
	INT32 Recv(INT32 nFd, PVOID pRecvData, INT32 nReqSize);

public:
	INT32 SendExitThreadCmd();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
};

extern	CThreadPoInSpReq*	t_ThreadPoInSpReq;

#endif // _THREAD_PO_IN_SP_REQ_H__
