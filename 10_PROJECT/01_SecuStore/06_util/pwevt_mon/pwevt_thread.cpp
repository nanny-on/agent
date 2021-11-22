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
 * License along with this program. If not, see <https://www.gnu.org/licenses/>.   
 *  
 * 
 */

#include "stdafx.h"
#include "com_struct.h"
#include "pwevt_thread.h"

/////////////////////////////////////////////////////////////////////////////
// CMonThreadTimer

CMonThreadTimer*	t_MonThreadTimer = NULL;

CMonThreadTimer::CMonThreadTimer()
{
}

CMonThreadTimer::~CMonThreadTimer()
{
}

BOOL CMonThreadTimer::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CMonThreadTimer::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CMonThreadTimer message handlers

int CMonThreadTimer::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nRunFlag = 1;
	while(GetContinue())
	{
		if(!m_nPause)
			t_MonTimerUtil.ChkTimer();
		
		Sleep(500);
	}
	return 0;
}
