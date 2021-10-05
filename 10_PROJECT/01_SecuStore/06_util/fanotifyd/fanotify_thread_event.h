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

#ifndef _FANOTIFY_THREAD_EVENT_H__
#define _FANOTIFY_THREAD_EVENT_H__

/////////////////////////////////////////////////////////////////////////////
// CThreadFaNotifyEvent thread
class CThreadFaNotifyEvent : public CThreadBase
{
public:
	CThreadFaNotifyEvent();
	virtual ~CThreadFaNotifyEvent();

private:
	INT32						m_nFaNotifyFd;
	INT32						m_nClientFd;
	BOOL						m_bIsConnected;
	UINT32						m_nFaNotifyCount;
	UINT32						m_nFaNotifyIndex;
	UINT32						m_nSendTime;
	pthread_mutex_t 			m_EventMutex;
	pthread_mutex_t 			m_SockMutex;
	TMapFaNotifyPath			m_tFaNotifyEventMap;
	HWND						m_pNotifyWnd;

private:
	INT32						InitNotifyEvent(INT32 &nNotifyFd, INT32 &nClientFd, INT32 nTestMode);
	VOID						UninitNotifyEvent(INT32 nTestMode);

	BOOL						AddFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath);
	BOOL						ModFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath);
	BOOL						GetFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath);
	BOOL						DelFaNotifyPath(INT32 nIndex);
	BOOL						IsFaNotifyPath(char *pPath);
	VOID						ClearFaNotifyPath();

	UINT32						GetFaNotifyPathCount();
	void						ClearWatchFaNotify();
	INT32						AddWatchFaNotify(INT32 nOrderID, char *pPath);
	INT32						RemoveWatchFaNotify(char *pPath);
	INT32						CheckWhitePatternFile();

	INT32						InitSockEnv(INT32 &nClientFd);
	VOID						UninitSockEnv();
	BOOL						IsSockConnected();
	VOID						GetClientFd(INT32 &nClientFd);
	INT32						SockRecv(INT32 nFd, PVOID pRecvData, INT32 nReqSize);
	INT32						SockWrite(INT32 nFd, PVOID pWriteData, INT32 nReqSize);

	BOOL						InitFaNotifyFd(INT32 &nNotifyFd);
	BOOL						GetFaNotifyFd(INT32 &nNotifyFd);
	VOID						CloseFaNotifyFd();

	INT32						AddFaNotifyMark(PFANOTIFY_PATH pFaNotifyPath);
	INT32						RemoveFaNotifyMark(PFANOTIFY_PATH pFaNotifyPath);

	BOOL						ParseFilePath(PASI_CHK_INFO pInfo);
	BOOL						GetProcPathFromPid(INT32 nPid, PASI_CHK_INFO pProcInfo);
	BOOL						GetFilePathFromFd(INT32 nFd, PASI_CHK_INFO pFileInfo);
	INT32						SendEventToServer(INT32 nNotifyFd, INT32 nClientFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc);
	INT32						AnalyzeExecEvent2(INT32 nClientFd, PASI_CHK_FILE_PROC pChkFileProc, INT32 nCount);
	INT32						AnalyzeFaNotifyEvent(INT32 nNotifyFd, INT32 nClientFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc);
	INT32						CheckFaNotifyFile(INT32 nNotifyFd, INT32 nClientFd, char *pcBuffer, PASI_CHK_FILE_PROC pChkFileProc);
// Operations
public:
	INT32						SetNotifyWnd(HWND pNotifyWnd);
	UINT32						GetSendTime();
	INT32						SendAliveMessage();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadFaNotifyEvent)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

public:
	INT32		AddOrderID(PVOID pPFOI);
	INT32		IsEndWork()							{	return m_nPause;	};

// Implementation
protected:

};

extern	CThreadFaNotifyEvent*	t_ThreadFaNotifyEvent;

/////////////////////////////////////////////////////////////////////////////

#endif //_THREAD_PO_IN_PTN_FILE_H__
