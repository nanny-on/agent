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

// ProcUtil.cpp: implementation of the CProcUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "FileUtil.h"
#include "ProcUtil.h"

//---------------------------------------------------------------------------


CProcUtil*	t_ProcUtil = NULL;

//---------------------------------------------------------------------------


CProcUtil::CProcUtil()
{

}
//---------------------------------------------------------------------------

CProcUtil::~CProcUtil()
{

}
//---------------------------------------------------------------------------

BOOL CProcUtil::ProcessStop(LPCSTR lpProcName)
{
	INT32 nPid = 0;
	if(lpProcName == NULL)
		return FALSE;

	nPid = GetProcessID(lpProcName);
	if(nPid == 0)
		return FALSE;

	if(kill(nPid, SIGKILL) == -1)
		return FALSE;

	return TRUE;
}
//---------------------------------------------------------------------------

BOOL CProcUtil::ProcessStart(LPCSTR lpExeFileName, LPCSTR lpCommand, BOOL bWait, BOOL bChkChild, UINT32* pProcessID)
{
	char szCmd[CHAR_MAX_SIZE] = {0,};

	if(lpExeFileName == NULL || lpExeFileName[0] == 0)
	{
		WriteLogE("[ProcessStart] invalid input data");
		return FALSE;
	}

	if(lpCommand == NULL || lpCommand[0] == 0)
		snprintf(szCmd, CHAR_MAX_SIZE-1, "%s\n", lpExeFileName);
	else	
		snprintf(szCmd, CHAR_MAX_SIZE-1, "%s %s\n", lpExeFileName, lpCommand);
	szCmd[CHAR_MAX_SIZE-1] = 0;

	if(system(szCmd) == -1)
	{
		WriteLogE("[ProcessStart] fail to start %s (%d)", szCmd, errno);
		return FALSE;
	}

	return TRUE;
}
//---------------------------------------------------------------------------

BOOL CProcUtil::UpdateProcStart(LPCSTR lpExePathName, LPCSTR lpExeFileName, LPCSTR lpZipPathName, LPCSTR lpUpdatePathName)
{
	pid_t nChildPid = 0;
	INT32 nStatus = 0;
	INT32 nRetVal = 0;

	if(lpExePathName == NULL || lpExeFileName == NULL || lpZipPathName == NULL || lpUpdatePathName == NULL)
	{
		WriteLogE("[UpdateProcStart] invalid input data");
		return FALSE;
	}

	nChildPid = fork();
	if(nChildPid==-1)
	{
		WriteLogE("[UpdateProcStart] fail to fork (%d)", errno);
		return FALSE;
	}

	if(nChildPid==0)
	{
		nRetVal = execl(lpExePathName, lpExeFileName, "-u", "-f", lpZipPathName, "-d", lpUpdatePathName, NULL);
		if(nChildPid==-1)
		{
			WriteLogE("[UpdateProcStart] fail to exec (%d)", errno);
			return FALSE;
		}
		exit(0);
	}
	else if(nChildPid==0)
	{
		waitpid(nChildPid,&nStatus,WNOHANG);
	}
	return TRUE;
}


DWORD	CProcUtil::GetChildProcessID(DWORD dwProcessID, String strProcessName)
{
	return 0;
}

INT32 CProcUtil::GetProcessID(LPCSTR lpProcName)
{
	INT32 nPid = 0;
	DIR *pDp = NULL;
	struct dirent *pDirp = NULL;
	INT32 i, nLength;
	BOOL bIsProc = TRUE;
	FILE* fp = NULL;
	char acFileName[MAX_PATH] = {0,};
	char acFilePath[MAX_PATH] = {0,};
	char acProcName[MAX_PROC_PATH] = {0,};
	if(lpProcName == NULL)
		return 0;

	pDp = opendir("/proc");
	if (pDp == NULL)
	{
		WriteLogE("[GetProcessID] fail to open proc dir (%d)", errno);
		return 0;
	}

	while((pDirp = readdir(pDp)) != NULL)
	{
		strncpy(acFileName, pDirp->d_name, MAX_PATH-1);
		acFileName[MAX_PATH-1] = 0;
		if(DT_DIR == pDirp->d_type)
		{
			nLength = (INT32)strlen(acFileName);
			bIsProc = TRUE;
			for(i = 0; i < nLength; i++)
			{
				if(acFileName[i] > '9' || acFileName[i] < '0')
				{
					bIsProc = FALSE;
					break;
				}
			}
			if(bIsProc==TRUE)
			{
				snprintf(acFilePath, MAX_PATH-1, "/proc/%s/stat", acFileName);
				fp = fopen(acFilePath, "r");
				if(fp!=NULL)
				{
					fscanf(fp, "%d%s", &i, acProcName);
					nLength = (INT32)strlen(acProcName);
					if(nLength > 0 || nLength < MAX_PROC_PATH-1)
					{
						acProcName[nLength - 1] = 0;
						if(!_stricmp(lpProcName, &acProcName[1]))
							nPid = atoi(acFileName);
					}
					fclose(fp);
				}
			}
		}
	}
	closedir(pDp);
	return nPid;
}

DWORD	CProcUtil::GetParentProcess(DWORD dwProcessID, String& strProcessName)
{
	proc_info_t stProcInfo;
	memset(&stProcInfo, 0, sizeof(stProcInfo));

	if(get_proc_info_by_pid((INT32)dwProcessID, &stProcInfo) != 0)
		return 0;

	strProcessName = stProcInfo.name;
	return (UINT32)stProcInfo.ppid;
}
//---------------------------------------------------------------------------

INT32	CProcUtil::GetEnumProcessID(String strProcessName, TListID& tProcIDList)
{
	return 0;
}
//---------------------------------------------------------------------------

DWORD CProcUtil::GetProcessSessionID(UINT32 nProcID) 
{
	return 0;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


BOOL CProcUtil::ProcessStartAsUser(LPCSTR lpExeName, LPCSTR lpCommand, BOOL bWait, BOOL bHide)
{
	BOOL bResult = FALSE;
	return bResult;
}
//---------------------------------------------------------------------------

BOOL CProcUtil::ProcessExist(LPCSTR lpProcName)
{
	BOOL bContinue  = TRUE;
	return !bContinue;
}
//---------------------------------------------------------------------------

BOOL	CProcUtil::ProcessExist(UINT32 nProcID)
{
	BOOL bContinue  = TRUE;
	return !bContinue;
}
//---------------------------------------------------------------------------

BOOL	CProcUtil::GatherProcessName(TMapEnumProcess& tProcessNameMap)
{
	return TRUE;
}
//---------------------------------------------------------------------------

BOOL	CProcUtil::GatherProcessNameNT(TMapEnumProcess& tProcessNameMap, TMapID& tProcessIDMap)
{
    return TRUE;
}
//---------------------------------------------------------------------------


UINT32	CProcUtil::GetProcessIDByNameEx(TMapEnumProcess& tProcessNameMap, String strProcName, String strProcPath)
{
	return 0;
}
//---------------------------------------------------------------------------

UINT32 CProcUtil::GetProcessIDByName(String strProcName)
{
	return 0;
}
//---------------------------------------------------------------------------

UINT32 CProcUtil::GetProcessIDByName(TMapEnumProcess& tProcessNameMap, String strProcName, String strProcPath, UINT32* tProcIDList)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32	CProcUtil::GetProcessNameByID(DWORD dwPID, LPTSTR lpProcName, INT32 nLen)
{
	return 0;

}
//---------------------------------------------------------------------------

BOOL CProcUtil::Token(LPTSTR strInput, char pTok, LPTSTR pszRtn, BOOL bReverse)
{
	int nPos = 0;
	CHAR strResult[CHAR_MAX_SIZE] = {0, };
	BOOL bFind = FALSE;

	if(bReverse)
	{			
		INT32 nLen = (INT32)strlen_ext(strInput);
		INT32 nTIndex = 0;
		INT32 nSIndex = 0;

		for(INT32 i = nLen - 1; i > -1; i--)
		{
			if(strInput[i] == pTok)
			{
				strInput[i] = 0;
				break;
			}
			else
			{
				pszRtn[nSIndex++] = strInput[i];
			}
		}		
		ReverseLPTSTR(pszRtn);
	}
	else
	{
		INT32 nLen = (INT32)strlen_ext(strInput);
		INT32 nTIndex = 0;
		INT32 nSIndex = 0;

		for(INT32 i = 0; i < nLen; i++)
		{
			if(strInput[i] == pTok && !bFind)
			{
				bFind = TRUE;
				continue;
			}

			if(bFind)
			{				
				strResult[nSIndex]	= strInput[i];
				nSIndex++;
			}
			else
			{
				pszRtn[nTIndex] = strInput[i];
				nTIndex++;
			}
		}

		ZeroMemoryExt(strInput);
		strncpy(strInput, strResult, nLen);		
	}
	return TRUE;
}


