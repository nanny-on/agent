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
 * License along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *  * 
 */

#ifndef _ACCNOTIFY_THREAD_SERVER_SOCKET_H__
#define _ACCNOTIFY_THREAD_SERVER_SOCKET_H__

/////////////////////////////////////////////////////////////////////////////
class CThreadAccServerSocket : public CThreadBase
{
public:
	CThreadAccServerSocket();
	virtual ~CThreadAccServerSocket();

private:
	static VOID					SetOpMode(INT32 nMode);
	static INT32				CheckThread(INT32 nClientFd);
	static INT32				InitUnixSock(INT32 &nSrvFd);
	static VOID					UninitUnixSock();
	static INT32				GetCheckThreadState();
	static VOID					SetCheckThreadState(INT32 nState);
	static INT32				GetClientFd();
	static VOID					SetClientFd(INT32 nFd);
	static INT32				CheckSockEvent(INT32 nClientFd, PASI_ACC_NOTIFY_POLICY pAccNotiPol);
	static INT32				SockRecv(INT32 nFd, PVOID pRecvData, INT32 nReqSize);
	static INT32				SockWrite(INT32 nFd, PVOID pWriteData, INT32 nReqSize);

	static VOID					SetPolicyUse(INT32 nPolUse);

	static VOID					SetPolicyInfo(PASI_POLICY_INFO pPolicyInfo);
	static VOID					ClearAllInfo();
	static VOID					ClearPolicyInfo();

	static VOID					AddAllowExPathStr(CString strPath, CString strHash);
	static VOID					AddDenyExPathStr(CString strPath, CString strHash);
	static VOID					ClearExPathStr();

	static VOID					AddAllowSpPathStr(CString strPath, CString strHash);
	static VOID					AddDenySpPathStr(CString strPath, CString strHash);
	static VOID					ClearSpPathStr();

	static VOID					AddWlPathStr(CString strPath, CString strHash);
	static VOID					ClearWlPathStr();

	static VOID					AddBlPathStr(CString strPath, CString strHash);
	static VOID					ClearBlPathStr();

	static VOID					AddCrPathStr(CString strPath, CString strHash);
	static VOID					ClearCrPathStr();
	static UINT32				GetSockContinue();
// Operations
public:
	INT32						SetNotifyWnd(HWND pNotifyWnd);
	static INT32				SendExitThreadCmd();

	static VOID					GetPolicyUse(INT32 &nPolUse);

	static VOID					GetPolicyInfo(PASI_POLICY_INFO pPolicyInfo);

	static INT32				GetDenyExPathCount();
	static INT32				IsDenyExPathStr(CString strPath);
	static INT32				FindDenyExPathStr(CString strPath, CString &strHash);
	static INT32				GetAllowExPathCount();
	static INT32				IsAllowExPathStr(CString strPath);
	static INT32				FindAllowExPathStr(CString strPath, CString &strHash);

	static INT32				GetDenySpPathCount();
	static INT32				IsDenySpPathStr(CString strPath);
	static INT32				FindDenySpPathStr(CString strPath, CString &strHash);
	static INT32				GetAllowSpPathCount();
	static INT32				IsAllowSpPathStr(CString strPath);
	static INT32				FindAllowSpPathStr(CString strPath, CString &strHash);

	static INT32				GetWlPathCount();
	static INT32				IsWlPathStr(CString strPath);
	static INT32				FindWlPathStr(CString strPath, CString &strHash);

	static INT32				GetBlPathCount();
	static INT32				IsBlPathStr(CString strPath);
	static INT32				FindBlPathStr(CString strPath, CString &strHash);

	static INT32				GetCrPathCount();
	static INT32				IsCrPathStr(CString strPath);
	static INT32				FindCrPathStr(CString strPath, CString &strHash);

	static VOID					GetOpMode(INT32 &nMode);
	static void					SetSockContinue(UINT32 nContinue);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadAccServerSocket)
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

extern	CThreadAccServerSocket*	t_ThreadAccServerSocket;

/////////////////////////////////////////////////////////////////////////////

#endif //_ACCNOTIFY_THREAD_SERVER_SOCKET_H__
