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

#ifndef _ACCNOTIFY_THREAD_BASE_H__
#define _ACCNOTIFY_THREAD_BASE_H__

// ThreadBase.h : header file
enum T_STATE 
{
	STATE_RUN = 0,
	STATE_SUSPEND,
	STATE_RESUME,
	STATE_EXIT
};


/////////////////////////////////////////////////////////////////////////////
// CThreadBase thread

class CThreadBase
{
public:
	CThreadBase();           // protected constructor used by dynamic creation
	virtual ~CThreadBase();
	VOID ResumeThread();
	VOID TerminateThread();
	VOID SuspendThread();
	int ExitInstance();
	BOOL CreateThreadExt(String strName, pthread_t *pnThreadID, BOOL bCreateSuspended = FALSE);
	int WaitToExit(DWORD dwMilliSecs);
	int WaitToResume(DWORD dwMilliSecs);
	
	HANDLE		GetHandle() 						{	return m_hThread;	};
	String		GetThreadName() 					{	return m_strThreadName; 	};
	INT32		SetThreadName(String strName)		{	m_strThreadName = strName;	};
	void		SetPause()							{	m_nPause = 1;	m_nState = 	STATE_SUSPEND;	};
	void		SetResume()							{	m_nPause = 0;	m_nState = 	STATE_RESUME;	};
	UINT32		GetContinue()						{	return m_nContinue;			};
	void		SetContinue(UINT32 nContinue)		{	m_nContinue = nContinue;	};
	INT32		IsRunThread()						{	return m_nRunFlag;			};
	void		SetOwnerClass(PVOID pOwnerClass)	{	m_pOwnerClass = pOwnerClass;};

	T_STATE		m_nState;
	
protected:
	DWORD 		m_dwThreadID;
	HANDLE		m_hThread;
	UINT32		m_nContinue;
	UINT32		m_nPause;
	UINT32		m_nRunFlag;
	BOOL		m_bInit;
	String		m_strThreadName;
	pthread_cond_t m_ExitCond;
	pthread_cond_t m_ResumeCond;
	pthread_mutex_t m_ExitMutex;
	pthread_mutex_t m_ResumeMutex;
	PVOID		m_pOwnerClass;

	virtual BOOL InitInstance() { return 1; }
	virtual int Run() { return 1; }
	virtual int Finalize() { return 1; }
	static PVOID ThreadWorker(LPVOID lParam);
};

#endif // _ACCNOTIFY_THREAD_BASE_H__
