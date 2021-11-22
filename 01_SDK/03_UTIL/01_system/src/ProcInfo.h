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

// ProcInfo.h: INT32erface for the CProcInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCINFO_H__2B72C57A_A03B_41C1_BA90_BC1F7E6AB43A__INCLUDED_)
#define AFX_PROCINFO_H__2B72C57A_A03B_41C1_BA90_BC1F7E6AB43A__INCLUDED_

class CProcInfo  
{
public:	
	CHAR 	m_szHostName[CHAR_MAX_SIZE];

public:
	BOOL	GetProcessUserName(HANDLE hProcess, CHAR *acUserName, CHAR *acDomainName);
	INT32	GetProcessFullPath(DWORD dwProcessId, CHAR *acProcFullPath, INT32 nLen);
	void	SetProcessPath(CHAR *szFileName, INT32 nLen);
	BOOL	IsDir(CHAR *path);

public:
	CProcInfo();
	virtual ~CProcInfo();

};

#endif // !defined(AFX_PROCINFO_H__2B72C57A_A03B_41C1_BA90_BC1F7E6AB43A__INCLUDED_)
