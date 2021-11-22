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

//---------------------------------------------------------------------------

#include "stdafx.h"
#include "com_struct.h"
#include "ManageWinSession.h"

//---------------------------------------------------------------------------

//CManageWinSession	t_ManageWinSession;

//---------------------------------------------------------------------------

CManageWinSession::CManageWinSession()
{
}
//---------------------------------------------------------------------------

CManageWinSession::~CManageWinSession()
{
	
}
//---------------------------------------------------------------------------

INT32		CManageWinSession::InitWinSession()
{
	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageWinSession::AddWinSession(UINT32 nID, UINT32 nAddEvt, UINT32 nLoginTime)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageWinSession::DelWinSession(UINT32 nID)
{
	return 0;		
}
//---------------------------------------------------------------------------

INT32		CManageWinSession::EdistrUserName(MEM_WIN_SESSION& tMWS)
{
	return 0;
}
//---------------------------------------------------------------------------

String		CManageWinSession::GetLogonUserName()
{
	String strUserName = "";
	return strUserName;
}
//---------------------------------------------------------------------------

String		CManageWinSession::GetLogonDomainName()
{
	String strDomainName = "";
/*
	CProcUtil tProcUtil;
	UINT32 nPID = tProcUtil.GetProcessIDByName("explorer.exe");
	if(!nPID)	return strDomainName;

	UINT32 nSID = tProcUtil.GetProcessSessionID(nPID);

	DWORD len;
	LPTSTR lpBuff = NULL;
	WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, nSID, WTSDomainName, &lpBuff, &len);
	{
		strDomainName = lpBuff;
		WTSFreeMemory(lpBuff);
		lpBuff = NULL;
	}
*/
	return strDomainName;
}
//---------------------------------------------------------------------------

INT32				CManageWinSession::GetSIDList(TListStr& tSIDList)
{
	TMapMemWinSessionItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		tSIDList.push_back(begin->second.strSID);
	}
	return tSIDList.size();
}
//---------------------------------------------------------------------------

INT32				CManageWinSession::IsWinSessionChange()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32				CManageWinSession::IsWinSessionChgHK()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32				CManageWinSession::AddWinSessionChange(UINT32 nMode)
{
	return 0;
}
//---------------------------------------------------------------------------

