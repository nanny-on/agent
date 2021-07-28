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

#if !defined(AFX_THREAD_PO_FA_OP_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_)
#define AFX_THREAD_PO_FA_OP_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CThreadPoFaOp thread

class CThreadPoFaOp : public CThreadBase
{
public:
	CThreadPoFaOp();           // protected constructor used by dynamic creation
	virtual ~CThreadPoFaOp();

private:
	UINT32						m_nOrderID;
	TMapMemFindOrderInfo		m_tOrderIDMap;
	INT32						m_nNotifyFd;
	UINT32						m_nNotifyCount;
	pthread_mutex_t 			m_NotifyMutex;
	TMapNotifyPath				m_tNofifyPathMap;

private:
	BOOL						IsNotifyPath(INT32 nWatchd);
	BOOL						IsNotifyPathWithPath(char *pPath);
	BOOL						InsertNotifyPath(PNOTIFY_PATH pNotifyPath);
	BOOL						ModNotifyPath(PNOTIFY_PATH pNotifyPath);
	BOOL						DeleteNotifyPath(INT32 nWatchd);
	BOOL						GetNotifyPath(PNOTIFY_PATH pNotifyPath);
	void						ClearNotifyPath();
	BOOL						InitNotifyFd();
	BOOL						CloseNotifyFd();
	INT32						AddDirWatchNotify(PNOTIFY_PATH pNotifyPath);
	INT32						AddSubDirWatchNotify(INT32 nOrderID, INT32 nExtOption, char *pPath);
	INT32						DelWatchNotify(char *pPath);
	INT32						AnalyzeEvent(PVOID pEvent);
	INT32						CheckNotifyFile();
// Operations
public:
	void						ClearWatchNotify();
	INT32						AddWatchNotify(INT32 nOrderID, INT32 nExtOption, char *pPath);
	UINT32						GetNotifyPathCount();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadPoFaOp)
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

extern	CThreadPoFaOp*	t_ThreadPoFaOp;

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_THREAD_PO_FA_OP_H__83ABD8B0_065D_4303_A05E_165A4583F052__INCLUDED_)
