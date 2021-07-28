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

#ifndef     _THREAD_EXECUTE_H__
#define     _THREAD_EXECUTE_H__
/////////////////////////////////////////////////////////////////////////////
// CThreadExecute thread

class CThreadExecute : public CThreadBase
{
public:
	CThreadExecute();           // protected constructor used by dynamic creation
	virtual ~CThreadExecute();

private:
	CMutexExt				m_tMutex;
	TMapMemWorkExecute		m_tWorkMap;
	UINT32					m_nLastID;

private:
//	CSystemInfo				m_tSysInfo;
//	CApplyPatchUtil			m_tAPUtil;
//	CASICabDLLUtil			m_tASICabDLLUtil;

public:
	INT32					AddWork(PMEM_WORK_EXECUTE pdata);
	INT32					EditWork(UINT32 nID, MEM_WORK_EXECUTE& data);
	INT32					DelWork(UINT32 nID);
	INT32					GetWork(UINT32 nID, MEM_WORK_EXECUTE& data);
	INT32					IsNextWork();
	INT32					IsRemainWork(UINT32 nType, UINT32 nSkipID = 0);
	INT32					IsExistWork(MEM_WORK_EXECUTE& data);

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadExecute)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

protected:
	INT32	ExecuteWork(PMEM_WORK_EXECUTE pdata);
	INT32	ExecuteWorkForCab(PMEM_WORK_EXECUTE pdata);
	INT32	ExecuteProcDesktop(PMEM_WORK_EXECUTE pdata);
};

extern	CThreadExecute*	t_ThreadExecute;

#endif // _THREAD_EXECUTE_H__
