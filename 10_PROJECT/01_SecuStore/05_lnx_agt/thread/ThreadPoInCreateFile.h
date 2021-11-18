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

#ifndef _THREAD_PO_IN_CREATE_FILE_H__
#define _THREAD_PO_IN_CREATE_FILE_H__

/////////////////////////////////////////////////////////////////////////////
class CThreadPoInCreateFile : public CThreadBase
{
public:
	CThreadPoInCreateFile();
	virtual ~CThreadPoInCreateFile();

private:
	INT32						m_nCreateNotifyFd;
	UINT32						m_nCreateNotifyCount;
	UINT32						m_nCreateNotifyIndex;
	TMapFaNotifyPath			m_tCreateNotifyEventMap;
	pthread_mutex_t 			m_EventMutex;
private:
	INT32						InitCreateNotifyEvent(INT32 &nNotifyFd);
	VOID						UninitCreateNotifyEvent();

	BOOL						AddCreateNotifyPath(PFANOTIFY_PATH pCreateNotifyPath);
	BOOL						ModCreateNotifyPath(PFANOTIFY_PATH pCreateNotifyPath);
	BOOL						GetCreateNotifyPath(PFANOTIFY_PATH pCreateNotifyPath);
	BOOL						DelCreateNotifyPath(INT32 nIndex);
	BOOL						IsCreateNotifyPath(char *pPath);
	VOID						ClearCreateNotifyPath();

	UINT32						GetCreateNotifyPathCount();
	void						ClearWatchCreateNotify();
	INT32						AddWatchCreateNotify(INT32 nOrderID, INT32 nNotifyFd, char *pPath);
	INT32						RemoveWatchCreateNotify(char *pPath);
	INT32						CheckWhitePatternFile();

	BOOL						InitCreateNotifyFd(INT32 &nNotifyFd);
	BOOL						GetCreateNotifyFd(INT32 &nNotifyFd);
	VOID						CloseCreateNotifyFd();

	INT32						AddCreateNotifyMark(INT32 nNotifyFd, PFANOTIFY_PATH pCreateNotifyPath);
	INT32						RemoveCreateNotifyMark(PFANOTIFY_PATH pCreateNotifyPath);

	static BOOL					ParseFilePath(PASI_CHK_INFO pInfo);
	static BOOL					GetFilePathFromFd(INT32 nFd, PASI_CHK_INFO pFileInfo);
	static INT32				AnalyzeCreateFile(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc);
	static INT32				AnalyzeCreateNotifyEvent(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_FILE_PROC pChkFileProc);
	static INT32				CheckCreateNotifyFile(INT32 nNotifyFd, char *pcBuffer, PASI_CHK_FILE_PROC pChkFileProc);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadPoInCreateFile)
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

extern	CThreadPoInCreateFile*	t_ThreadPoInCreateFile;

/////////////////////////////////////////////////////////////////////////////

#endif //_THREAD_PO_IN_CREATE_FILE_H__
