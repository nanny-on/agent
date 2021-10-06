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

#ifndef _ACCNOTIFY_THREAD_EVENT_H__
#define _ACCNOTIFY_THREAD_EVENT_H__

#define ASI_ACCNOTIFY_PATH "/bin"

/////////////////////////////////////////////////////////////////////////////
class CThreadAccNotifyEvent : public CThreadBase
{
public:
	CThreadAccNotifyEvent();
	virtual ~CThreadAccNotifyEvent();

private:
	INT32						m_nAccNotifyFd;
	INT32						m_nShmId;
	char *						m_pString;
	BOOL						m_bIsConnected;
	UINT32						m_nAccNotifyCount;
	UINT32						m_nAccNotifyIndex;
	UINT32						m_nSendTime;
	pthread_mutex_t 			m_EventMutex;
	TMapFaNotifyPath			m_tAccNotifyEventMap;
	HWND						m_pNotifyWnd;
	char						m_acSrcPath[MAX_PATH];
private:
	INT32						InitAccNotifyEvent(INT32 &nNotifyFd, INT32 nTestMode);
	VOID						UninitAccNotifyEvent(INT32 nTestMode);

	BOOL						AddAccNotifyPath(PFANOTIFY_PATH pAccNotifyPath);
	BOOL						ModAccNotifyPath(PFANOTIFY_PATH pAccNotifyPath);
	BOOL						GetAccNotifyPath(PFANOTIFY_PATH pAccNotifyPath);
	BOOL						DelAccNotifyPath(INT32 nIndex);
	BOOL						IsAccNotifyPath(char *pPath);
	VOID						ClearAccNotifyPath();

	UINT32						GetAccNotifyPathCount();
	void						ClearWatchAccNotify();
	INT32						AddWatchAccNotify(INT32 nOrderID, INT32 nNotifyFd, char *pPath);
	INT32						RemoveWatchAccNotify(char *pPath);
	INT32						CheckWhitePatternFile();

	INT32						InitShmEnv();
	VOID						UninitShmEnv();
	BOOL						IsSockConnected();
	INT32						ShmRecv(PVOID pRecvData, INT32 nReqSize);
	INT32						ShmWrite(PVOID pWriteData, INT32 nReqSize);

	BOOL						InitAccNotifyFd(INT32 &nNotifyFd);
	BOOL						GetAccNotifyFd(INT32 &nNotifyFd);
	VOID						CloseAccNotifyFd();

	INT32						AddAccNotifyMark(INT32 nNotifyFd, PFANOTIFY_PATH pAccNotifyPath);
	INT32						RemoveAccNotifyMark(PFANOTIFY_PATH pAccNotifyPath);

	BOOL						ParseFilePath(PASI_CHK_INFO pInfo);
	BOOL						GetProcPathFromPid(INT32 nPid, PASI_CHK_INFO pProcInfo);
	INT32						ChkInPtn(PASI_CHK_FILE_PROC pChkFileProc, INT32& nBlockMode, INT32& nIsWarning, INT32& nPolicyType);
	VOID						SetRetValValue(ASI_RET_INFO *pstRetInfo, INT32 nAcVal, INT32 nBlockMode, INT32 nIsWarning, INT32 nPolicyType);
	INT32						AnalyzeAccEvent(PASI_CHK_FILE_PROC pChkFileProc);BOOL						GetFilePathFromFd(INT32 nFd, PASI_CHK_INFO pFileInfo);
	INT32						SendEventToServer(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc);
	INT32						SendReqPoliyToServer(PASI_CHK_FILE_PROC pChkFileProc);
	INT32						AnalyzeAccNotifyEvent(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc);
	INT32						CheckAccNotifyFile(INT32 nNotifyFd, char *pcBuffer, PASI_CHK_FILE_PROC pChkFileProc);
	INT32						AnalyzeAccNotifyEvent2(PASI_CHK_FILE_PROC pChkFileProc, INT32 nCount);
// Operations
public:
	INT32						SetNotifyWnd(HWND pNotifyWnd);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadAccNotifyEvent)
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

extern	CThreadAccNotifyEvent*	t_ThreadAccNotifyEvent;

/////////////////////////////////////////////////////////////////////////////

#endif //_THREAD_PO_IN_PTN_FILE_H__
