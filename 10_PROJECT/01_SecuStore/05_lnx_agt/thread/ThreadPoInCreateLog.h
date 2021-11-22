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

#ifndef _THREAD_PO_IN_CREATE_LOG_H__
#define _THREAD_PO_IN_CREATE_LOG_H__

/////////////////////////////////////////////////////////////////////////////
class CThreadPoInCreateLog : public CThreadBase
{
public:
	CThreadPoInCreateLog();
	virtual ~CThreadPoInCreateLog();

private:
	UINT32						m_nTestTime;
	INT32						m_nTestCount;
	double						m_fTotalDiffTime;
	pthread_mutex_t 			m_EventMutex;
	CASIWEngDLLUtil				m_tWEDLLUtil;
	TListChkFileProc			m_tCreateFileList;
private:
	INT32						LoadWhitePattern();
	VOID						UnloadWhitePattern();

	INT32						GetCreateFileList(TListChkFileProc &tListCreateFile);
	INT32						GetCreateFileListCnt();
	VOID						ClearCreateFileList();
	INT32						AnalyzeCreateEvent(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32						CreateSendFile(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32						AnalyzeElfFile(PASI_CHK_PTN_FILE pChkPtnFile);
	INT32						CheckCreateNotifyFile(TListChkFileProc tListChkFileProc, PASI_CHK_PTN_FILE pChkPtnFile);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(t_ThreadPoInCreateLog)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

public:
	INT32		AddCreateFileList(PASI_CHK_FILE_PROC pCreateFile);
	INT32		AddOrderID(PVOID pPFOI);
	INT32		IsEndWork()							{	return m_nPause;	};

// Implementation
protected:

};

extern	CThreadPoInCreateLog*	t_ThreadPoInCreateLog;

/////////////////////////////////////////////////////////////////////////////

#endif //_THREAD_PO_IN_CREATE_LOG_H__
