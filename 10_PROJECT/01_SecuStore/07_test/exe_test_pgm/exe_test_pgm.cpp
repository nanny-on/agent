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

BOOL is_digit(int c)
{
	if(c >= 0x30 && c <= 0x39)
	{
		return TRUE;
	}
	return FALSE;
}

INT32 is_valid_num(char *str)
{
	INT32 i, nLen = 0;

	if (str == NULL || str[0] == '\0')
	{
		return -1;
	}

	nLen = strlen(str);
	if(nLen < 1)
	{
		return -2;
	}
	for (i=0; i<nLen; i++)
	{
		if (is_digit((int) str[i]) == FALSE)
		{
			return -3;
		}
	}
	return 0;
}

INT32 is_dir(char *pcPath)
{
	struct stat stStat;
	if(pcPath == NULL || pcPath[0] == 0)
		return -1;

	if(stat(pcPath, &stStat) < 0)
		return -2;
	if (!S_ISDIR(stStat.st_mode))
		return -3;
	return 0;
}

void get_current_date_time(char *pcBuf)
{
	time_t t;
	struct tm *dt = NULL;
	t = time(NULL);
	dt = localtime(&t);
	if(dt == NULL)
		return;
	if(pcBuf != NULL)
		snprintf(pcBuf, MAX_TIME_STR-1, "%.4d_%.2d_%.2d %.2d:%.2d:%.2d", dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);
}


INT32 execute_pgm(char *acFullPath, char *acFile)
{
	pid_t pid = -1;
	INT32 status = 0;
	INT32 nRetVal = 0;
	char acTime[MAX_TIME_STR] = {0,};
	if(acFullPath == NULL || acFullPath[0] == 0 || acFile == NULL || acFile[0] == 0)
		return -1;

	pid = fork();
	if (pid < 0)
	{
		return -2;
	}
	else if (pid == 0)
	{
		nRetVal = execlp(acFullPath, acFile, NULL);
		if(nRetVal < 0)
		{
			get_current_date_time(acTime);
			fprintf(stderr, "[%s] deny to execute %s\n", acTime, acFullPath);
		}
		exit(127);
	}
	else
	{
		while(waitpid(pid, &status, 0) < 0)
		{
			if(errno != EINTR)
			{
				nRetVal = -3;
				break;
			}
		}
	}

	return nRetVal;
}

void print_usage()
{
	fprintf(stdout, "Examples : \n");
	fprintf(stdout, "\texe_test_pgm -d path -c file_count [-s sleep(ms)]\n");
}

INT32 set_opt(INT32 argc, char *argv[], char *pcPath, INT32 nPathMax, INT32 &nFileCount, INT32 &nSleep)
{
	INT32 c;
	extern char *optarg;
	INT32 nLen = 0;
	char acValue[MAX_TYPE_LEN] = {0,};
	if(pcPath == NULL || nPathMax < 2)
	{
		fprintf(stderr, "invalid input data\n");
		return -1;
	}

	while ((c = getopt(argc, argv, "d:c:s:h")) != EOF)
	{
		switch ((char)c)
		{
		case 'd':
			if(optarg == NULL)
			{
				fprintf(stderr, "invalid input path\n");
				return -2;
			}
			strncpy(pcPath, optarg, nPathMax-1);
			pcPath[nPathMax-1] = 0;
			nLen = (INT32)strlen(pcPath);
			if(nLen > 2 && pcPath[nLen-1] == '/')
			{
				pcPath[nLen-1] = 0;
			}
			break;
		case 'c':
			if(optarg == NULL)
			{
				fprintf(stderr, "invalid input file count\n");
				return -3;
			}
			strncpy(acValue, optarg, MAX_TYPE_LEN-1);
			acValue[MAX_TYPE_LEN-1] = 0;
			if(is_valid_num(acValue) < 0)
			{
				fprintf(stderr, "invalid input file count %s\n", acValue);
				return -4;
			}
			nFileCount = atoi(acValue);
			break;
		case 's':
			if(optarg == NULL)
			{
				fprintf(stderr, "invalid input sleep msec\n");
				return -5;
			}
			strncpy(acValue, optarg, MAX_TYPE_LEN-1);
			acValue[MAX_TYPE_LEN-1] = 0;
			if(is_valid_num(acValue) < 0)
			{
				fprintf(stderr, "invalid input sleep msec %s\n", acValue);
				return -6;
			}
			nSleep = atoi(acValue);
			break;
		case 'h':
			print_usage();
			return 1;
		}
	}
	return 0;
}

INT32 check_valid_value(char *pcPath, INT32 nFileCount, INT32 nSleep)
{
	if(pcPath == NULL || pcPath[0] == 0 || nFileCount < 1 || nSleep < 1)
	{
		fprintf(stderr, "invalid input data\n");
		return -1;
	}

	if(is_dir(pcPath) < 0)
	{
		fprintf(stderr, "invalid input path %s\n", pcPath);
		return -2;
	}
	if(nFileCount < 1 || nFileCount > 999)
	{
		fprintf(stderr, "invalid input file count %d (1~999)\n", nFileCount);
		return -3;
	}
	if(nSleep < 1 || nSleep > 9999)
	{
		fprintf(stderr, "invalid input sleep %d (1~9999)ms\n", nSleep);
		return -4;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	INT32 nRetVal = 0;
	INT32 nFileMax = 0;
	INT32 nCount = 0;
	INT32 nSleep = 990;
	DIR *pDir = NULL;
	struct dirent *pDirEnt = NULL;
	char acPath[MAX_PATH] = {0,};
	char acFullPath[MAX_PATH] = {0,};
	char acFileName[MAX_FILE_NAME] = {0,};
 
	if(argc < 5)
	{
		print_usage();
		exit(1);
	}

	nRetVal = set_opt(argc, argv, acPath, MAX_PATH, nFileMax, nSleep);
	if(nRetVal != 0)
	{
		exit(2);
	}

	nRetVal = check_valid_value(acPath, nFileMax, nSleep);
	if(nRetVal != 0)
	{
		exit(3);
	}

	pDir = opendir(acPath);
	if (pDir == NULL)
	{
		fprintf(stderr, "fail to open dir %s (%d)\n", acPath, errno);
		exit(4);
	}

	while((pDirEnt = readdir(pDir)) != NULL)
	{
		if(!_stricmp(pDirEnt->d_name, ".") || !_stricmp(pDirEnt->d_name, ".."))
		{
			continue;
		}
		if(DT_REG == pDirEnt->d_type)
		{
			strncpy(acFileName, pDirEnt->d_name, MAX_FILE_NAME-1);
			acFileName[MAX_FILE_NAME-1] = 0;
			snprintf(acFullPath, MAX_PATH-1, "%s/%s", acPath, pDirEnt->d_name);
			acFullPath[MAX_PATH-1] = 0; 
			nRetVal = execute_pgm(acFullPath, acFileName);
			if(nRetVal != 0)
			{
				fprintf(stderr, "fail to execute %s (%d) (%d)\n", acFullPath, nRetVal, errno);
				exit(5);
			}
			nCount++;
			if(nCount >= nFileMax)
			{
				break;
			}
			Sleep(nSleep);
		}
	}
	closedir(pDir);

	fprintf(stdout, "success to execute %s's %d files\n", acPath, nCount);

	return 0;
}


