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

#ifndef _THREAD_PO_IN_PTN_FILE_H__
#define _THREAD_PO_IN_PTN_FILE_H__

/////////////////////////////////////////////////////////////////////////////
// CThreadPoInPtnFile thread

#define ASI_FANOTIFY_BUFF_MAX	8192

#define FAN_OPEN_EXEC		0x00001000	/* File was opened for exec */
#define FAN_OPEN_EXEC_PERM	0x00040000	/* File open/exec in perm check */

typedef struct _asi_chk_ptn_file
{
	char acFile[MAX_FILE_NAME];
	char acPath[MAX_PATH];
	char acProcPath[MAX_PATH];
	char acFilePath[MAX_PATH];
	ASI_WENG_WL_EX stAWWE;
} ASI_CHK_PTN_FILE, *PASI_CHK_PTN_FILE;

class CThreadPoInPtnFile : public CThreadBase
{
public:
	CThreadPoInPtnFile();
	virtual ~CThreadPoInPtnFile();

private:
	UINT32						m_nOrderID;
	TMapMemFindOrderInfo		m_tOrderIDMap;
	INT32						m_nFaNotifyFd;
	UINT32						m_nFaNotifyCount;
	UINT32						m_nFaNotifyIndex;
	pthread_mutex_t 			m_FaNotifyMutex;
	TMapFaNotifyPath			m_tFaNofifyPathMap;
	CASIWEngDLLUtil				m_tWEDLLUtil;
	CASIFIDLLUtil				m_tFIDLLUtil;
	TMapStrID					m_tFileTypeMap;

private:
	BOOL						AddFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath);
	BOOL						ModFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath);
	BOOL						GetFaNotifyPath(PFANOTIFY_PATH pFaNotifyPath);
	BOOL						DelFaNotifyPath(INT32 nIndex);
	BOOL						IsFaNotifyPath(char *pPath);
	VOID						ClearFaNotifyPath();

	BOOL						InitFaNotifyFd();
	BOOL						GetFaNotifyFd(INT32 &nNotifyFd);
	VOID						CloseFaNotifyFd();

	INT32						AddFaNotifyMark(PFANOTIFY_PATH pFaNotifyPath);
	INT32						RemoveFaNotifyMark(PFANOTIFY_PATH pFaNotifyPath);

	BOOL						GetProcPathFromPid(INT32 nPid, char *pcBuffer, INT32 nSize);
	BOOL						GetFilePathFromFd(INT32 nFd, PASI_CHK_PTN_FILE pChkPtnFile);
	INT32						AnalyzeExecEvent(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_PTN_FILE pChkPtnFile);
	INT32						AnalyzeFaNotifyEvent(INT32 nNotifyFd, PVOID pMetaData, PASI_CHK_PTN_FILE pChkPtnFile);
	INT32						CheckFaNotifyFile(INT32 nNotifyFd, char *pcBuffer, PASI_CHK_PTN_FILE pChkPtnFile);
// Operations
public:
	UINT32						GetFaNotifyPathCount();
	void						ClearWatchFaNotify();
	INT32						AddWatchFaNotify(INT32 nOrderID, char *pPath);
	INT32						RemoveWatchFaNotify(char *pPath);
	INT32						LoadWhitePattern();
	VOID						UnloadWhitePattern();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadPoInPtnFile)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

public:
	UINT32		GetNextOrderID()					{	return ++m_nOrderID;			};
	INT32		AddOrderID(PVOID pPFOI);
	INT32		IsEndWork()							{	return m_nPause;	};

// Implementation
protected:

};

extern	CThreadPoInPtnFile*	t_ThreadPoInPtnFile;

/////////////////////////////////////////////////////////////////////////////

#endif //_THREAD_PO_IN_PTN_FILE_H__
