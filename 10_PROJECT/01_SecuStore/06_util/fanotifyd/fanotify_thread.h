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

#ifndef _FANOTIFY_THREAD_H__
#define _FANOTIFY_THREAD_H__

#include "fanotify_timer.h"
/////////////////////////////////////////////////////////////////////////////
// CThreadTimer thread

class CMonThreadTimer : public CThreadBase
{
public:
	CMonThreadTimer();           // protected constructor used by dynamic creation
	virtual ~CMonThreadTimer();

public:
	CMonTimerUtil		t_MonTimerUtil;

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreadTimer)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:

};

extern	CMonThreadTimer*	t_MonThreadTimer;

#endif /*_FANOTIFY_THREAD_H__*/
