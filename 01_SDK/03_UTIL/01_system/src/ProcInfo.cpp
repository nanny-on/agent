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

// ProcInfo.cpp: implementation of the CProcInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "ProcInfo.h"

#define ACCOUNT_GROUP "BUILTIN"
#define ACCOUNT_LOCAL "NT AUTHORITY"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcInfo::CProcInfo()
{
	memset(m_szHostName, 0, CHAR_MAX_SIZE);
}
//---------------------------------------------------------------------

CProcInfo::~CProcInfo()
{

}
//---------------------------------------------------------------------

BOOL	CProcInfo::GetProcessUserName(HANDLE hProcess, CHAR *acUserName, CHAR *acDomainName)
{
	BOOL bRet = TRUE;
	return bRet;
}
//---------------------------------------------------------------------

INT32		CProcInfo::GetProcessFullPath(DWORD dwProcessId, CHAR *acProcFullPath, INT32 nLen)
{
	return FALSE;	
}
//---------------------------------------------------------------------

void	CProcInfo::SetProcessPath(CHAR *szFileName, INT32 nLen)
{
}

//---------------------------------------------------------------------
BOOL	  CProcInfo::IsDir(CHAR *path)
{
	INT32 ret;
	struct stat stStat;
	
	ret = stat(path, &stStat);
	if (ret < 0)
		return FALSE;
	
	return S_ISDIR(stStat.st_mode);
}
//---------------------------------------------------------------------
