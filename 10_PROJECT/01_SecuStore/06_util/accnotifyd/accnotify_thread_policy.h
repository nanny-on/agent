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
 * License along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *  * 
 */

#ifndef _ACC_NOTIFY_THREAD_POLICY_H__

#define _ACC_NOTIFY_THREAD_POLICY_H__

class CThreadAccNotifyPolicy : public CThreadBase
{
public:
	CThreadAccNotifyPolicy();
	virtual ~CThreadAccNotifyPolicy();

private:
	HWND				m_pNotifyWnd;
	INT32				m_nServerFd;
	INT32				m_nClientFd;
	INT32				m_nCheckThread;
	pthread_mutex_t 	m_SockMutex;

private:
	INT32				CheckThread(INT32 nClientFd);
	INT32				InitUnixSock(INT32 &nSrvFd);
	VOID				UninitUnixSock();
	INT32				GetCheckThreadState();
	VOID				SetCheckThreadState(INT32 nState);
	INT32				GetClientFd();
	VOID				SetClientFd(INT32 nFd);
	INT32				CheckSockEvent(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	INT32				SockRecv(INT32 nFd, PVOID pRecvData, INT32 nReqSize);
	INT32				SockWrite(INT32 nFd, PVOID pWriteData, INT32 nReqSize);
// Operations
public:
	INT32				SendExitThreadCmd();
	INT32				SetNotifyWnd(HWND pNotifyWnd);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadAccNotifyPolicy)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

public:
	INT32		IsEndWork()							{	return m_nPause;	};

// Implementation
protected:

};

extern	CThreadAccNotifyPolicy*	t_ThreadAccNotifyPolicy;

/////////////////////////////////////////////////////////////////////////////

#endif //_ACC_NOTIFY_THREAD_POLICY_H__
