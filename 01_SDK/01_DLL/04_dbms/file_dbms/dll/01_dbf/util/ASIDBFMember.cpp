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
 */

#include "stdafx.h"
#include "com_struct.h"
#include "ASIDBFMember.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASIDBFMember::CASIDBFMember()
{	
	m_tCurSelTable			= NULL;
	m_nCurSelDirection		= 0;
	m_pCurItemBuff			= NULL;

	m_nSelContinue	= 0;
	m_nSelColNum	= 0;

	m_nLastInsID			= 0;
	
	m_nFastItemLen			= 0;

	m_nFastFileMode			= 0;
	m_pFastFileBuff			= NULL;
	m_nFastFileBuffLen		= 0;
	m_nFastSFilePoint		= 0;
	m_nFastEFilePoint		= 0;
	m_nFastCFilePoint		= 0;
	m_nFastFileReadNum		= 0;

//	m_nASyncUsedMode		= 0;
//	m_tThreadSelect			= NULL;
}
//---------------------------------------------------------------------

CASIDBFMember::~CASIDBFMember()
{
	//StopThread();
	ClearMember();

	ClearFastFileBuff();
}
//---------------------------------------------------------------------

// CThreadSelect*		CASIDBFMember::GetThreadSel(PVOID pOwner)
// {
// 	if(!m_tThreadSelect)
// 	{
// 		m_tThreadSelect = new CThreadSelect();
// 		m_tThreadSelect->SetOwnerClass(pOwner);
// 		m_tThreadSelect->CreateThreadExt("select thread");
// 	}
// 	return m_tThreadSelect;
// }
// //---------------------------------------------------------------------
// 
// INT32				CASIDBFMember::StopThread()
// {
// 	if(m_tThreadSelect)
// 	{
// 		m_tThreadSelect->SetContinue(0);
// 		StopThread_Common(m_tThreadSelect);
// 		delete m_tThreadSelect;
// 		m_tThreadSelect = NULL;
// 	}
// 	return 0;
// }
// //---------------------------------------------------------------------
// 
// INT32		CASIDBFMember::StopThread_Common(CThreadBase* tThreadObject, UINT32 nWaitTime)
// {
// 	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
// 	if(hEvent == INVALID_HANDLE_VALUE)		return -1;
// 	INT32 nLWaitTime = nWaitTime * 100;
// 
// 	while(WaitForSingleObject(hEvent, 10) != WAIT_OBJECT_0)
// 	{
// 		if(tThreadObject->IsRunThread() == 0)	break;
// 		if(!nLWaitTime)							break;
// 
// 		--nLWaitTime;
// 	}
// 	CloseHandle(hEvent);
// 	TerminateThread(tThreadObject->m_hThread, 0);
// 	if(!nLWaitTime)
// 	{
// 		WriteLogN("thread terminate fail : over wait time [%d]", nWaitTime);
// 		return -10;
// 	}
// 	return 0;
// }
// //--------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

INT32		CASIDBFMember::ClearMember()
{
	if(m_pCurItemBuff)
	{
		free(m_pCurItemBuff);
		m_pCurItemBuff = NULL;
	}
	return 0;
}
//---------------------------------------------------------------------

INT32		CASIDBFMember::ClearFastFileBuff()
{
	m_tMutexFastFileBuff.Lock();
	if(m_pFastFileBuff)
	{
		free(m_pFastFileBuff);
		m_pFastFileBuff = NULL;
	}

	m_nFastFileMode = 0;
	m_nFastSFilePoint = 0;
	m_nFastEFilePoint = 0;
	m_nFastCFilePoint = 0;
	m_nFastFileBuffLen = 0;
	m_tMutexFastFileBuff.UnLock();
	return 0;
}
//---------------------------------------------------------------------

