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

// MainRun.cpp: implementation of the CMainRun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "MainDlg.h"

//--------------------------------------------------------------------

INT32		CMainDlg::StopThread_Common(CThreadBase* tThreadObject, UINT32 nWaitTime)
{
	INT32 nLWaitTime = 0;
	if(tThreadObject == NULL)
		return 0;
	
	if(m_SemExt.CreateEvent() == FALSE)
		return -1;
	
	nLWaitTime = nWaitTime * 10;

	while(m_SemExt.WaitForSingleObject(100) == WAIT_TIMEOUT)
	{
		if(tThreadObject->GetContinue() == 0)
			break;
		if(!nLWaitTime)
		--nLWaitTime;
	}
	m_SemExt.CloseHandle(); 
	if(!nLWaitTime)
	{
		WriteLogN("thread terminate fail : over wait time [%d]", nWaitTime);
		return -10;
	}
	return 0;
}
//--------------------------------------------------------------------

VOID CMainDlg::DelFileAfterBoot()
{
	if(t_LogicMgrPoFaDelFileAfterBoot)
		t_LogicMgrPoFaDelFileAfterBoot->DelFileAfterBoot();

}
