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

// FindProcess.cpp: implementation of the CFindProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "ResInfo.h"
#include "ProcessInfo.h"


CFindProcess::CFindProcess()
{
	m_TicksPerSec = sysconf(_SC_CLK_TCK);
}
//------------------------------------------------------------------------------------

CFindProcess::~CFindProcess()
{

}
//------------------------------------------------------------------------------------

INT32	CFindProcess::GetResourceData(TListResInfoProcInfo & tList_ProcessInfo)
{
	Find_Process(&tList_ProcessInfo);
	return 0;
}

INT32	CFindProcess::GetProcessCount()
{
	INT32 nCount = 0;
	DIR *pDir = NULL;
	struct dirent *pDrp = NULL;
	char acDirName[MAX_FILE_NAME] = {0, };

	pDir = opendir(PROC);
	/* Open /proc directory */
	if (pDir == NULL)
	{
		return 0;
	}

	while ((pDrp = readdir(pDir)) != NULL)
	{
		if(!_stricmp(pDrp->d_name, ".") || !_stricmp(pDrp->d_name, ".."))
		{
			continue;
		}

		strncpy(acDirName, pDrp->d_name, MAX_FILE_NAME-1);
		acDirName[MAX_FILE_NAME-1] = 0;
		
		if (is_valid_num(acDirName) == 0)
		{
			nCount++;
		}
	}

	closedir(pDir);

	return nCount;
}
//------------------------------------------------------------------------------------


INT32 CFindProcess::ReadProcStat(INT32 nPid, pid_stats *pPidStats)
{
	INT32 fd = 0;
	INT32 nRetVal = 0;
	char acFileName[MAX_PATH] = {0,}; 
	char *pcBuffer = NULL;

	if(pPidStats == NULL)
		return -1;

	pcBuffer = (char *)malloc(CHAR_MAX_SIZE+1);
	if(pcBuffer == NULL)
	{
		return -2;
	}
	memset(pcBuffer, 0, CHAR_MAX_SIZE+1);
	
	do{
		snprintf(acFileName, MAX_PATH-1, PID_STAT, nPid);
		acFileName[MAX_PATH-1] = 0;

		fd = open(acFileName, O_RDONLY);
		if (fd < 0)
		{
			nRetVal = -3;
			break;
		}
		nRetVal = read(fd, pcBuffer, CHAR_MAX_SIZE);
		close(fd);
		if (nRetVal <= 0 || nRetVal > CHAR_MAX_SIZE)
		{
			nRetVal = -4;
			break;
		}
		pcBuffer[nRetVal] = 0;

		memset(acFileName, 0, MAX_PATH);

		nRetVal = sscanf(pcBuffer,
				"%d %s %*s %d %d %d" //6
				" %d %d %u %llu %llu" //11
				" %llu %llu %llu %llu %lld %lld" //17
				" %*d %*d %u %*u %u %llu %llu"  //24
				" %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u" //37
				" %*u %u %u %u %llu %llu %lld\n", //44
				&pPidStats->pid, acFileName, &pPidStats->ppid, &pPidStats->pgrp, &pPidStats->session,
				&pPidStats->tty, &pPidStats->tgid, &pPidStats->flags, &pPidStats->minflt, &pPidStats->cminflt,
				&pPidStats->majflt, &pPidStats->cmajflt, &pPidStats->utime,  &pPidStats->stime, &pPidStats->cutime, &pPidStats->cstime,
				&pPidStats->threads, &pPidStats->start_time, &pPidStats->vsz, &pPidStats->rss, &pPidStats->processor,
				&pPidStats->priority, &pPidStats->policy, &pPidStats->blkio_swapin_delays, &pPidStats->gtime, &pPidStats->cgtime);

		if (nRetVal < 23)
		{
			nRetVal = -5;
			break;
		}

		if (nRetVal< 25)
		{
			pPidStats->gtime = pPidStats->cgtime = 0;
		}

		nRetVal = (int)strlen(acFileName);
		if(nRetVal < 1)
		{
			nRetVal = -6;
			break;
		}

		if(acFileName[nRetVal-1] == ')')
			acFileName[nRetVal-1] = 0;
		if(acFileName[0] == '(')
			strncpy(pPidStats->comm, &acFileName[1], MAX_COMM_LEN-1);
		else
			strncpy(pPidStats->comm, acFileName, MAX_COMM_LEN-1);

		pPidStats->comm[MAX_COMM_LEN-1] = 0;
			
		/* Convert to kB */
		pPidStats->vsz >>= 10;
		pPidStats->rss = pPidStats->rss/KBYTES;
		nRetVal = 0;
	}while(FALSE);

	safe_free(pcBuffer);
	return nRetVal;
}

INT32 CFindProcess::ReadProcCmdLine(INT32 nPid, char *acProcPath, int nMaxLen)
{
	FILE *fp = NULL;
	char acFileName[MAX_PATH] = {0,}; 
	INT32 i, nLen = 0;

	if(nPid < 0 || nMaxLen < 1)
		return -1;
	
	snprintf(acFileName, MAX_PATH-1, PID_CMDLINE, nPid);
	fp = fopen(acFileName, "r");
	if (fp == NULL)
	{
		return -2;
	}

	memset(acProcPath, 0, nMaxLen);

	nLen = fread(acProcPath, 1, nMaxLen - 1, fp);
	fclose(fp);

	if(nLen < 1 || nLen > nMaxLen-1)
	{
		acProcPath[0] = 0;
		return -3;
	}

	for (i = 0; i < nLen; i++)
	{
		if (acProcPath[i] == '\0')
		{
			acProcPath[i] = ' ';
		}
	}
	return 0;
}


INT32 CFindProcess::ReadProcFd(INT32 nPid)
{
	DIR *pDir = NULL;
	struct dirent *pDrp = NULL;
	INT32 nCount = 0;
	char acFileName[MAX_PATH] = {0,}; 

	if(nPid < 0)
		return 0;

	snprintf(acFileName, MAX_PATH-1, PID_FD, nPid);

	pDir = opendir(acFileName);
	if (pDir == NULL)
	{
		return 0;
	}

	while ((pDrp = readdir(pDir)) != NULL)
	{
		if (is_valid_num(pDrp->d_name) == 0)
		{
			nCount++;
		}	
	}

	closedir(pDir);
	return nCount;
}

INT32 CFindProcess::GetTotalMeminfo(unsigned     long long *pddwTotal)
{
	INT32 nRetVal = 0;
	FILE *fp = NULL;
	unsigned long long ddwTotalMem = 0;
	char acLine[MAX_QBUFF] = {0,};

	if(pddwTotal == NULL)
		return -1;

	fp = fopen(MEMINFO, "r");
	if (fp == NULL)
	{
		return -2;
	}
	while (fgets(acLine, MAX_QBUFF-1, fp) != NULL)
	{
		if (!_strnicmp(acLine, "MemTotal:", 9))
		{
			/* Read the total amount of memory in kB */
			sscanf(&acLine[9], "%llu", &ddwTotalMem);
			break;
		}
	}
	fclose(fp);
	
	if(ddwTotalMem == 0)
		return -3;
	
	*pddwTotal = ddwTotalMem;
	return 0;
}


INT32 CFindProcess::ReadUptime(UINT64 *pddwTime)
{
	FILE *fp = NULL;
	INT32 nRetVal = 0;
	char acLine[MAX_QBUFF] = {0,};
	ULONG dwUpSec = 0, dwUpCent = 0;
	UINT64 ddwTime = 0;

	if(pddwTime == NULL || m_TicksPerSec < 1)
		return -1;

	do{
		fp = fopen(UPTIME, "r");
		if (fp == NULL)
		{
			nRetVal = -2;
			break;
		}
		if (fgets(acLine, MAX_QBUFF-1, fp) == NULL)
		{
			nRetVal = -3;
			break;
		}
		
		if (sscanf(acLine, "%lu.%lu", &dwUpSec, &dwUpCent) != 2)
		{
			nRetVal = -4;
			break;
		}
		*pddwTime = UINT64((dwUpSec*m_TicksPerSec)+dwUpCent);
		nRetVal = 0;
	}while(FALSE);
	if (fp != NULL)
	{
		fclose(fp);
	}
	return nRetVal;
}

void CFindProcess::GetProcessCPUTime(ULONG dwStartTime, INT32 *pnTime, char *pcStartTime, int nMaxLen)
{
	UINT64 ddwUpTime = 0;
	UINT64 ddwRunning = 0;
	time_t tProcTime = 0;
	struct tm dt;
	char szTemp[32] = {0, };

	if(pnTime == NULL || pcStartTime == NULL || nMaxLen < 1)
		return;

	if(ReadUptime(&ddwUpTime) != 0)
		return;

	ddwRunning = (UINT64)(ddwUpTime - dwStartTime);

	tProcTime = (time_t)(time(NULL) - (ddwRunning / m_TicksPerSec));

	*pnTime = (INT32)tProcTime;
	
	localtime_r(&tProcTime, &dt);

	if (dt.tm_hour > 12)
    {
    	dt.tm_hour -= 12;
        snprintf(szTemp, 31, "PM %02d:%02d:%02d", dt.tm_hour, dt.tm_min, dt.tm_sec);
    }
    else
		snprintf(szTemp, 31, "AM %02d:%02d:%02d", dt.tm_hour, dt.tm_min, dt.tm_sec);

    snprintf(pcStartTime, nMaxLen-1, "%d-%02d-%02d %s", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, szTemp);

	
}


INT32 CFindProcess::GetProcessNameAndID(INT32 nPid, ASIRES_INFO_PROCESS_INFO &pi)
{
	INT32 nRetVal = 0;
	pid_stats *pStats = NULL;

	do
	{
		pStats = (pid_stats *)malloc(sizeof(pid_stats));
		if(pStats == NULL)
		{
			nRetVal = -1;
			break;
		}
		memset(pStats, 0, sizeof(pid_stats));

		nRetVal = ReadProcStat(nPid, pStats);
		if(nRetVal != 0)
		{
			nRetVal -= -1000;
			break;
		}

		pi.pbi.nPPID = (INT32)pStats->ppid;
		pi.pbi.nBasePri = (INT32)pStats->priority;
		pi.psi.nThreadCount = (INT32)pStats->threads;
		pi.psi.nHandleCount = ReadProcFd(nPid);
		strncpy(pi.pbi.szProcessName, pStats->comm , MAX_PROC_PATH-1);
		pi.pbi.szProcessName[MAX_PROC_PATH-1] = 0;
		
		nRetVal = ReadProcCmdLine(nPid, pStats->cmdline, MAX_CMDLINE_LEN);
		if(nRetVal == 0)
		{
			strncpy(pi.pbi.szProcessFullName, pStats->cmdline, MAX_PROC_PATH-1);
			pi.pbi.szProcessFullName[MAX_PROC_PATH-1] = 0;
		}
		else
		{
			strncpy(pi.pbi.szProcessFullName, pStats->comm, MAX_PROC_PATH-1);
			pi.pbi.szProcessFullName[MAX_PROC_PATH-1] = 0;
		}
		GetProcessUserName(pStats->uid, pi.pai.szUserName, MAX_QBUFF);
		GetProcessCPUTime(pStats->start_time, (INT32 *)&pi.pti.nStartTime, pi.pti.szStartTime, MAX_TIME_STR);
		nRetVal = 0;
	}while(FALSE);

	safe_free(pStats);
	return nRetVal;
}

INT32 CFindProcess::Find_Process(TListResInfoProcInfo * tList_ProcessInfo)
{
	ASIRES_INFO_PROCESS_INFO stProcInfo;
	INT32 nPid = 0;
	DIR *pDir = NULL;
	struct dirent *pDrp = NULL;
	char acDirName[MAX_FILE_NAME] = {0, };

	if(tList_ProcessInfo == NULL)
		return -1;

	pDir = opendir(PROC);
	if (pDir == NULL)
		return -2;

	while ((pDrp = readdir(pDir)) != NULL)
	{
		if(!_stricmp(pDrp->d_name, ".") || !_stricmp(pDrp->d_name, ".."))
		{
			continue;
		}

		strncpy(acDirName, pDrp->d_name, MAX_FILE_NAME-1);
		acDirName[MAX_FILE_NAME-1] = 0;
		if (is_valid_num(acDirName) != 0)
		{
			continue;
		}
		memset(&stProcInfo, 0, sizeof(ASIRES_INFO_PROCESS_INFO));
		
		nPid = stProcInfo.pbi.nPID = atoi(acDirName);
		
		if (GetProcessNameAndID(nPid, stProcInfo) == -1)
		{	
			continue;
		}
		tList_ProcessInfo->push_back(stProcInfo);
	}

	closedir(pDir);
	return 0;
}
//------------------------------------------------------------------------------------

VOID  CFindProcess::GetProcessUserName(UINT32 dwUid, char *pUserName, INT32 nMaxLen) 
{
	struct passwd *pPwdEnt = NULL;

	if(pUserName == NULL || nMaxLen < 1)
		return;

	pPwdEnt = getpwuid(dwUid);
	if(pPwdEnt != NULL)
	{
		strncpy(pUserName, pPwdEnt->pw_name, nMaxLen-1);
	}
}
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
