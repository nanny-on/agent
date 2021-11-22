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

#ifndef MEM_WIN_SESSION_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_WIN_SESSION_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_win_session
{
	_mem_win_session()
	{
		nID					= 0;
		strUserName			= "";
		strWinStation		= "";
		strSID				= "";
		nLogonTime			= 0;
		nEndOpType			= 0;
	}

	UINT32			nID;	
	String			strUserName;
	String			strWinStation;
	String			strSID;
	UINT32			nLogonTime;
	UINT64			nEndOpType;
}MEM_WIN_SESSION, *PMEM_WIN_SESSION;

typedef list<MEM_WIN_SESSION>				TListMemWinSession;
typedef TListMemWinSession::iterator		TListMemWinSessionItor;

typedef map<UINT32, MEM_WIN_SESSION>		TMapMemWinSession;
typedef TMapMemWinSession::iterator			TMapMemWinSessionItor;

#endif //MEM_WIN_SESSION_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



