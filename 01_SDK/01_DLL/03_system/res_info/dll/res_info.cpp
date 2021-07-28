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

#include "stdafx.h"
#include "com_struct.h"
#include "res_info.h"

#define MINIMUM_COUNT	2
#define CHECK_DELAY		1

static pthread_t g_tid = 0;
static int g_nItemID = 0;
static double g_dBandWidth = 0;
static int g_nCpuUsage = 0;
static double g_dMaxNetSpeed = 1;
static FILE* fp_cmd = NULL;
static int g_nExit = 0;

void __attribute__ ((constructor)) res_init(void)
{
}

void __attribute__ ((destructor)) res_fini(void)
{
	if(fp_cmd)
	{
		g_nExit = 1;
		while(fp_cmd)
			sleep(1);
	}
}

static void* MoniteringThread(void* pParam)
{
	char szInfName[16] = {0,};
    struct ifaddrs* ifaddr, *ifa;
    if(getifaddrs(&ifaddr) == -1)
		return 0;

    int nID = 0;
    for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_addr == NULL)
            continue;
        int family = ifa->ifa_addr->sa_family;
        if((family == AF_INET || family == AF_INET6) && strcmp(ifa->ifa_name, "lo"))
        {
            if(nID == g_nItemID)
            {
                strncpy(szInfName, ifa->ifa_name, sizeof(szInfName)-1);
                break;
            }
            nID++;
        }
    }
    if(strlen(szInfName) == 0)
    {
        return NULL;
    }
	
	{
		FILE* fp_ethtool = NULL;
		char szTemp[256] = {0,};
		snprintf(szTemp, 255, "ethtool %s", szInfName);
		fp_ethtool = popen(szTemp, "r");
		if(fp_ethtool)
		{
			while(fgets(szTemp, sizeof(szTemp) - 1, fp_ethtool))
			{
				const char* pSpeed = strstr(szTemp, "Speed:");
				if(pSpeed)
				{
					char dum[256] = {0,};
					int n = 1;
					sscanf(pSpeed+6, "%d%s", &n, dum);
					g_dMaxNetSpeed = n;
		            if(dum[0] == 'k' || dum[0] == 'K')
						g_dMaxNetSpeed = n*1000;
		            if(dum[0] == 'M' || dum[0] == 'm')
						g_dMaxNetSpeed = n*1000*1000;
					if(dum[0] == 'G' || dum[0] == 'g')
						g_dMaxNetSpeed = n*1000*1000*1000;
					g_dMaxNetSpeed = g_dMaxNetSpeed/8.0;
				}
			}
			pclose(fp_ethtool);
		}
	}

	char buf[256] = {0,};
	char dummy[256] = {0,};
    int nSent = 0, nRecv = 0;
    char dstat_cmd[32] = {0,};
    if(fp_cmd == NULL)
    {
        snprintf(dstat_cmd, sizeof(dstat_cmd)-1, "dstat -N %s", szInfName);
        if((fp_cmd = popen(dstat_cmd, "r")) == NULL)
        {
	        return NULL;
        }
    }

    int nRowCount = 0;
    int nRecvColIndex = -1;
    int nSendColIndex = -1;
	int nIdleColIndex = -1;
    int nRecvBytes = 0;
    int nSendBytes = 0;
	int nIdle = 0;
	char* pret = NULL;
    while(!g_nExit)
    {
		pret = fgets(buf, sizeof(buf), fp_cmd);
		if(pret == NULL || pret == (char*)(-1))
			break;

        if(nRecvColIndex == -1 || nSendColIndex == -1 || nIdleColIndex == -1)
        {
            const char* precv = strstr(buf, "recv");
            const char* psend = strstr(buf, "send");
			const char* pidle = strstr(buf, "idl");
            if(precv == NULL)
                continue;
            nRecvColIndex = precv - buf - 1;
            nSendColIndex = psend - buf - 1;
			nIdleColIndex = pidle - buf;
        }
        else
        {
            nRowCount++;
            int nTemp = 0;
            sscanf(&buf[nRecvColIndex], "%d%s", &nTemp, dummy);
			if(dummy[0] == 'k') nTemp = nTemp*1000;
			if(dummy[0] == 'M') nTemp = nTemp*1000*1000;
			//printf("recv - nTemp: %d dummy: '%s'\n", nTemp, dummy);
            nRecvBytes += nTemp;
            nTemp = 0;
            sscanf(&buf[nSendColIndex], "%d%s", &nTemp, dummy);
            if(dummy[0] == 'k') nTemp = nTemp*1000;
            if(dummy[0] == 'M') nTemp = nTemp*1000*1000;
			//printf("send - nTemp: %d dummy: '%s'\n", nTemp, dummy);
            nSendBytes += nTemp;
            nTemp = 0;
            sscanf(&buf[nIdleColIndex], "%d %s", &nTemp, dummy);
            nIdle += nTemp;
            if(nRowCount >= MINIMUM_COUNT && nRowCount)
            {
				g_dBandWidth = (double)(nSendBytes + nRecvBytes)/nRowCount;
				g_nCpuUsage = 100 - nIdle/nRowCount;
				//printf("CPU: %d Send: %d Recv: %d\n", g_nCpuUsage, nSendBytes, nRecvBytes);
				nIdle = 0; nSendBytes = 0; nRecvBytes = 0; nRowCount = 0;
			}
        }
    }

    pclose(fp_cmd);
	fp_cmd = NULL;

	return NULL;
}

INT32 GetCpuUsageTotal(UINT32* nUsage)
{
	if(g_tid == 0)
	{
		pthread_attr_t attr = {0,};
		pthread_attr_init(&attr);
		pthread_create(&g_tid, &attr, MoniteringThread, NULL);
	}

	*nUsage = g_nCpuUsage;

	return 0;
}

INT32 GetMemoryInfo(UINT64 *pddwTotalPhySize, UINT64 *pddwTotalPageSize, UINT64 *pddwAvailPhySize, UINT64 *pddwAvailPageSize)
{
	char szBuf[CHAR_MAX_SIZE] = {0,};
	FILE* fp = NULL;
	char* pStrMemTotal = NULL;
	char* pStrMemFree = NULL;
	char* pStrBuffers = NULL;
	char* pStrCached = NULL;
	char* pTok = NULL;

	if(pddwTotalPhySize == NULL || pddwAvailPhySize == NULL)
		return -1;

	*pddwTotalPhySize = 1;
	*pddwAvailPhySize = 0;
	
	fp = fopen("/proc/meminfo", "r");
	if(fp == NULL)
		return -2;

	if(fread(szBuf, 1, CHAR_MAX_SIZE-1, fp) == 0)
	{
		fclose(fp);
		return -3;
	}
	fclose(fp);

	pStrMemTotal = strstr(szBuf, "MemTotal:");
	pStrMemFree = strstr(szBuf, "MemFree:");
	pStrBuffers = strstr(szBuf, "Buffers:");
	pStrCached = strstr(szBuf, "Cached:");
	if(pStrMemTotal)
	{
		strtok(pStrMemTotal+1, " ");
		pTok = strtok(NULL, " ");
		if(pTok)
			*pddwTotalPhySize = strtoul(pTok, NULL, 10)*1024;
	}
	if(pStrMemFree)
	{
        strtok(pStrMemFree+1, " ");
        pTok = strtok(NULL, " ");
        if(pTok)
            *pddwAvailPhySize += strtoul(pTok, NULL, 10)*1024;
	}
	if(pStrBuffers)
	{
		strtok(pStrBuffers+1, " ");
		pTok = strtok(NULL, " ");
		if(pTok)
			*pddwAvailPhySize += strtoul(pTok, NULL, 10)*1024;
	}
	if(pStrCached)
	{
		strtok(pStrCached+1, " ");
		pTok = strtok(NULL, " ");
		if(pTok)
			*pddwAvailPhySize += strtoul(pTok, NULL, 10)*1024;
	}
			
	return 0;
}

INT32 GetHddInfo(UINT64* nTotalHddSize, UINT64* nUsageHddSize, LPCTSTR lpDriveName)
{
	char* MMOUNT = "/proc/mounts";
    char buf[256] = {0,};
    char devname[80] = {0,};
    char mountdir[80] = {0,};
    char fstype[12] = {0,};

	struct statfs fs;
    struct stat st;
    FILE *fp = fopen(MMOUNT, "r");
    if(!fp)
    {
	    return -1;
    }
															
    while(fgets(buf, 255, fp))
    {
	    char tmpdev[80] = {0,};
        char tmpdir[80] = {0,};
        sscanf(buf, "%79s%79s%11s", tmpdev, tmpdir, fstype);
        if(strncmp(lpDriveName, tmpdir, strlen(tmpdir)) == 0)
        {
	        if(stat(tmpdev, &st) == 0)
            {
	            if(S_ISBLK(st.st_mode) && strlen(mountdir) < strlen(tmpdir))
                {
	                strncpy(mountdir, tmpdir, sizeof(mountdir)-1);
                    strncpy(devname, tmpdev, sizeof(devname)-1);
                }
            }
        }
    }
	fclose(fp);

	if(statfs(mountdir, &fs))
    {
        return -2;
    }
    *nTotalHddSize = fs.f_blocks*fs.f_bsize;
    *nUsageHddSize = fs.f_bavail*fs.f_bsize;
	
	return 0;
}

INT32 GetTrafficBandWidht(INT32 nItemID, DOUBLE* dBandWidth)
{
	g_nItemID = nItemID;
    if(g_tid == 0)
    {
        pthread_attr_t attr = {0,};
        pthread_attr_init(&attr);
        pthread_create(&g_tid, &attr, MoniteringThread, NULL);
    }

	*dBandWidth = g_dMaxNetSpeed;

	return 0;
}

INT32 GetTrafficValue(INT32 nItemID, DOUBLE* dTrafficValue)
{
	double dTemp = 0;
	GetTrafficBandWidht(nItemID, &dTemp);
	if(g_dBandWidth > g_dMaxNetSpeed)
		g_dBandWidth = g_dMaxNetSpeed;
	*dTrafficValue = g_dBandWidth;

	return 0;
}
