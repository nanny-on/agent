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

// ProcUtil.h: interface for the CProcUtil class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PROC_UTIL_H__
#define _PROC_UTIL_H__

typedef struct _process_info
{
	String		pszProcessName;
	String		strProcessPath;
	UINT32		nProcessID;
}PROCESS_INFO,*PPROCESS_INFO;

typedef map<UINT32, PROCESS_INFO>	TMapEnumProcess;
typedef TMapEnumProcess::iterator	TMapEnumProcessItor;

class CProcUtil  
{
private:
	BOOL Token(LPTSTR strInput, char pTok, LPTSTR pszRtn, BOOL bReverse = FALSE);

public:
	INT32	GetProcessID(LPCSTR lpProcName);
	BOOL	ProcessStart(LPCSTR lpExeFileName, LPCSTR lpCommand, BOOL bWait = FALSE, BOOL bChkChild = FALSE, UINT32* pProcessID = NULL);
	BOOL	UpdateProcStart(LPCSTR lpExePathName, LPCSTR lpExeFileName, LPCSTR lpZipPathName, LPCSTR lpUpdatePathName);
	BOOL	ProcessStop(LPCSTR lpProcName);
	BOOL	ProcessExist(LPCSTR lpProcName);
	BOOL	ProcessExist(UINT32 nProcID);
	BOOL	GatherProcessName(TMapEnumProcess& tProcessNameMap);
	BOOL	GatherProcessNameNT(TMapEnumProcess& tProcessNameMap, TMapID& tProcessIDMap);	
	UINT32	GetProcessIDByName(String strProcName);
	UINT32	GetProcessIDByName(TMapEnumProcess& tProcessNameMap, String strProcName, String strProcPath, UINT32* tProcIDList);
	UINT32	GetProcessIDByNameEx(TMapEnumProcess& tProcessNameMap, String strProcName, String strProcPath);
	INT32	GetProcessNameByID(DWORD dwPID, LPTSTR lpProcName, INT32 nLen = CHAR_MAX_SIZE);
	DWORD	GetChildProcessID(DWORD dwProcessID, String strProcessName = "");
	DWORD	GetParentProcess(DWORD dwProcessID, String& strProcessName);
	INT32	GetEnumProcessID(String strProcessName, TListID& tProcIDList);
	DWORD	GetProcessSessionID(UINT32 nProcID);

public:
	BOOL	ProcessStartAsUser(LPCSTR lpExeName, LPCSTR lpCommand, BOOL bWait = FALSE, BOOL bHide = FALSE);

public:
	CProcUtil();
	virtual ~CProcUtil();

};

extern CProcUtil*	t_ProcUtil;

#endif //_PROC_UTIL_H__
