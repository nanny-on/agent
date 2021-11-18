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

#ifndef _ACCNOTIFY_THREAD_CLIENT_SOCKET_H__
#define _ACCNOTIFY_THREAD_CLIENT_SOCKET_H__

/////////////////////////////////////////////////////////////////////////////
class CThreadAccClientSocket : public CThreadBase
{
public:
	CThreadAccClientSocket();
	virtual ~CThreadAccClientSocket();

private:
	static VOID					SetOpMode(INT32 nMode);
	static INT32				InitShmEnv();
	static VOID					UninitShmEnv();
	static INT32				ShmWrite(PVOID pWriteData, INT32 nReqSize);
	
	static INT32				GetChkFileProcListCnt();
	static INT32				GetChkFileProcList(TListChkFileProc &tListChkFileProc);
	static VOID					ClearChkFileProcList();
	static INT32				SendEventToServer(TListChkFileProc tListChkFileProc);
// Operations
public:
	INT32						SetNotifyWnd(HWND pNotifyWnd);
	static INT32				AddChkFileProcList(PASI_CHK_FILE_PROC pChkFileProc);
	static VOID					GetOpMode(INT32 &nMode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadAccClientSocket)
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

extern	CThreadAccClientSocket*	t_ThreadAccClientSocket;

/////////////////////////////////////////////////////////////////////////////

#endif //_ACCNOTIFY_THREAD_CLIENT_SOCKET_H__
