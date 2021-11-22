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

INT32 check_dir(char *pcPath)
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

INT32 check_file(char *pcPath)
{
	struct stat stStat;
	if(pcPath == NULL || pcPath[0] == 0)
		return -1;

	if(stat(pcPath, &stStat) < 0)
		return -2;
	if (!S_ISREG(stStat.st_mode))
		return -3;
	return 0;
}

INT32 create_dir(char *pcPath)
{
	if(pcPath == NULL || pcPath[0] == 0)
		return -1;

	if(check_dir(pcPath) == 0)
		return 0;

	if(mkdir(pcPath, 0755) != 0)
		return -2;

	return 0;
}

INT32 clear_dir(char *pcPath)
{
	INT32 nRetVal = 0;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	char acFilePath[MAX_PATH] = {0,};
	if(pcPath == NULL || pcPath[0] == 0)
	{
		return -1;
	}

	dp = opendir(pcPath);
	if (dp == NULL)
	{
		return -2;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
			continue;
		if(DT_REG == dirp->d_type)
		{
			snprintf(acFilePath, MAX_PATH-1, "%s/%s", pcPath, dirp->d_name);
			acFilePath[MAX_PATH-1] = 0;
			unlink(acFilePath);
		}
	}
	closedir(dp);

	return nRetVal;
}

INT32 check_tmp_dir()
{
	char acPath[MAX_PATH] = "/tmp/tcfile";

	if(check_dir(acPath) == 0)
	{
		if(clear_dir(acPath) != 0)
			return -1;
	}
	else
	{
		if(mkdir(acPath, 0755) != 0)
			return -2;
	}

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

INT32 make_test_src(char *acPath, char *acName, INT32 nIndex)
{
	INT32 nRetVal = 0;
	FILE *fp = NULL;
	char acTime[MAX_TIME_STR] = {0,};
	char acSrcPath[MAX_PATH] = {0,};
	time_t t = 0;
	t = time(NULL);
	if(acPath == NULL || acName == NULL || nIndex < 0)
		return -1;
	snprintf(acSrcPath, MAX_PATH-1, "%s/%s_file_%03d.cpp", acPath, acName, nIndex);
	do{
		fp = fopen(acSrcPath, "wt");
		if(fp == NULL)
		{
			nRetVal = -2;
			break;
		}
		fprintf(fp, "#include <stdio.h>\n");
		fprintf(fp, "#include <time.h>\n\n");
		fprintf(fp, "#define MAX_TIME_STR 64+%d\n\n", nIndex);
		fprintf(fp, "#define CURRENT_TIME_VALUE %u\n\n", (UINT32)t);
		fprintf(fp, "void get_current_date_time_%d(char *pcBuf)\n", (int)t);
		fprintf(fp, "{\n");
		fprintf(fp, "\ttime_t t%d;\n", (int)t);
		fprintf(fp, "\tstruct tm *dt = NULL;\n");
		fprintf(fp, "\tt%d = time(NULL);\n", (int)t);
		fprintf(fp, "\tdt = localtime(&t%d);\n", (int)t);
		fprintf(fp, "\tif(dt == NULL)\n");
		fprintf(fp, "\t\treturn;\n");
		fprintf(fp, "\tif(pcBuf != NULL)\n");
		fprintf(fp, "\t\tsnprintf(pcBuf, MAX_TIME_STR-1, \"%c.4d_%c.2d_%c.2d %c.2d:%c.2d:%c.2d\", dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);\n", '%', '%', '%', '%', '%', '%');
		fprintf(fp, "}\n\n");
		fprintf(fp, "int main(int argc, char *agrv[])\n");
		fprintf(fp, "{\n");
		fprintf(fp, "\tchar acTime[MAX_TIME_STR] = {0,};\n");
		fprintf(fp, "\tget_current_date_time_%d(acTime);\n", (int)t);
		fprintf(fp, "\tfprintf(stdout, \"[%03d]", nIndex);
		get_current_date_time(acTime);
		fprintf(fp, "[%s]", acTime);
		fprintf(fp, "[%cs]", '%');
		fprintf(fp, "[%s_file_%000d] ", acName, nIndex);
		fprintf(fp, "success to execute %cs", '%');
		fprintf(fp, "\\n\", acTime, agrv[0]);\n");
		fprintf(fp, "\treturn CURRENT_TIME_VALUE%c30;\n", '%');
		fprintf(fp, "}\n");
		nRetVal = 0;
	}while(FALSE);
	if(fp != NULL)
	{
		fclose(fp);
	}
	return nRetVal;
}


INT32 build_test_pgm(char *acPath, char *acName, INT32 nIndex)
{
	pid_t pid = -1;
	INT32 status = 0;
	INT32 nRetVal = 0;
	char acSrcPath[MAX_PATH] = {0,};
	char acBinPath[MAX_PATH] = {0,};
	char acTime[MAX_TIME_STR] = {0,};
	if(acPath == NULL || acPath[0] == 0 || acName == NULL || acName[0] == 0)
		return -1;
	snprintf(acSrcPath, MAX_PATH-1, "%s/%s_file_%03d.cpp", acPath, acName, nIndex);
	snprintf(acBinPath, MAX_PATH-1, "%s/%s_file_%03d", acPath, acName, nIndex);
	if(check_file(acBinPath) == 0)
		unlink(acBinPath);

	pid = fork();
	if (pid < 0)
	{
		return -2;
	}
	else if (pid == 0)
	{
		nRetVal = execlp("/usr/bin/g++", "g++", "-g", "-o", acBinPath, acSrcPath, "-static-libgcc", "-static-libstdc++", NULL);
		if(nRetVal < 0)
		{
			get_current_date_time(acTime);
			fprintf(stderr, "[%s] fail to compile %s (%d)\n", acTime, acSrcPath, errno);
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
	unlink(acSrcPath);
	return nRetVal;
}

void trim_feed(char *str)
{
	while (*str) {
		if (*str == 0x0a || *str == 0x0d) {
			*str = '\0';
			break;
		} else {
			str++;
		}
	}
}

INT32 is_proc(char *acProcName)
{
	FILE *fp = NULL;
	char *pTok, *pLast;
	char acBuf[MAX_PATH] = { 0,};
	char acFileName[MAX_PATH] = {0,};
	char *pcPath = NULL;
	char acDelim[3] = { 0x09, 0x20, 0x00 }; /* tab, space */
	DIR *pDir = NULL;
	struct dirent *pDirEnt = NULL;

	if(acProcName == NULL)
		return -1;

	pcPath = (char *)malloc(MAX_PATH);
	if(pcPath == NULL)
	{
		return -2;
	}
	memset(pcPath, 0, MAX_PATH);

	if ( (pDir = opendir("/proc")) == NULL)
	{
		safe_free(pcPath);
		return -3;
	}

	while((pDirEnt = readdir(pDir)) != NULL)
	{
		if (pDirEnt->d_name[0] < '0' || pDirEnt->d_name[0] > '9')
		{
			continue;
		}
		strncpy(acFileName, pDirEnt->d_name, MAX_PATH-1);
		acFileName[MAX_PATH-1] = 0;
		snprintf(pcPath, MAX_PATH-1, "/proc/%s/status", acFileName);
		pcPath[MAX_PATH-1] = 0;

		if ( (fp = fopen(pcPath, "r")) == NULL)
		{
			continue;
		}

		while (fgets(acBuf, MAX_PATH-1, fp))
		{
			trim_feed(acBuf);
			pTok = strtok_r(acBuf, acDelim, &pLast);
			if (pTok == NULL)
			{
				memset(acBuf, 0, MAX_PATH);
				continue;
			}

			if (strcmp(pTok, "Name:") != 0)
			{
				memset(acBuf, 0, MAX_PATH);
				continue;
			}

			pTok = strtok_r(NULL, "", &pLast);
			if (pTok == NULL)
			{
				memset(acBuf, 0, MAX_PATH);
				continue;
			}

			if (strcmp(pTok, acProcName) != 0)
			{
				memset(acBuf, 0, MAX_PATH);
				break;
			}
			else
			{
				fclose(fp);
				closedir(pDir);
				safe_free(pcPath);
				return 1;
			}
		}

		fclose(fp);
	}

	closedir(pDir);
	safe_free(pcPath);

	return 0;
}

INT32 check_make_test_pgm(char *acPath, char *acName, INT32 nIndex)
{
	INT32 i, nRetVal = 0;
	char acBinPath[MAX_PATH] = {0,};
	char acCheckPath[MAX_PATH] = {0,};

	if(acPath == NULL || acPath[0] == 0 || acName == NULL || acName[0] == 0)
		return -1;

	snprintf(acBinPath, MAX_PATH-1, "%s/%s_file_%03d", acPath, acName, nIndex);
	snprintf(acCheckPath, MAX_PATH-1, "/tmp/tcfile/%s_file_%03d", acName, nIndex);
	for(i=0; i<20; i++)
	{
		if(check_file(acCheckPath) == 0)
		{
			unlink(acCheckPath);
			return 0;
		}
		Sleep(50);
	}
	unlink(acBinPath);
	return -2;
}

INT32 build_pgm(char *acPath, char *acName, INT32 nIndex)
{
	INT32 i, nRetVal = 0;
	INT32 nIsNanny = 0;
	char acTime[MAX_TIME_STR] = {0,};
	if(acPath == NULL || acPath[0] == 0 || acName == NULL || acName[0] == 0)
		return -1;

	nIsNanny = is_proc(NANNY_AGENT_IDENT);

	for(i=0; i<10; i++)
	{
		nRetVal = make_test_src(acPath, acName, nIndex);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			fprintf(stderr, "fail to make %s_file_%000d.cpp (%d) (%d)\n", acName, nIndex, nRetVal, errno);
			return nRetVal;
		}
		Sleep(100);
		nRetVal = build_test_pgm(acPath, acName, nIndex);
		if(nRetVal < 0)
		{
			nRetVal -= 20;
			fprintf(stderr, "fail to build %s_file_%000d (%d) (%d)\n", acName, nIndex, nRetVal, errno);
			return nRetVal;
		}
		Sleep(100);

		if(nIsNanny == 0)
		{
			return 0;
		}
		nRetVal = check_make_test_pgm(acPath, acName, nIndex);
		if(nRetVal == 0)
		{
			return 0;
		}
	}
	return -2;
}

void print_usage()
{
	fprintf(stdout, "Examples : \n");
	fprintf(stdout, "\tmake_test_pgm -d test_dir -n test_name -c file_count\n");
}

INT32 set_opt(INT32 argc, char *argv[], char *pcPath, INT32 nPathMax, char *pcName, INT32 nNameMax, INT32 &nFileCount)
{
	INT32 c;
	extern char *optarg;
	INT32 nLen = 0;
	char acValue[MAX_TYPE_LEN] = {0,};
	if(pcPath == NULL || nPathMax < 2 || pcName == NULL || nNameMax < 2)
	{
		fprintf(stderr, "invalid input data\n");
		return -1;
	}

	while ((c = getopt(argc, argv, "d:n:c:h")) != EOF)
	{
		switch ((char)c)
		{
		case 'd':
			if(optarg == NULL)
			{
				fprintf(stderr, "invalid input test path\n");
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
		case 'n':
			if(optarg == NULL)
			{
				fprintf(stderr, "invalid input test name\n");
				return -3;
			}
			strncpy(pcName, optarg, nNameMax-1);
			pcName[nNameMax-1] = 0;
			break;
		case 'c':
			if(optarg == NULL)
			{
				fprintf(stderr, "invalid input file count\n");
				return -4;
			}
			strncpy(acValue, optarg, MAX_TYPE_LEN-1);
			acValue[MAX_TYPE_LEN-1] = 0;
			if(is_valid_num(acValue) < 0)
			{
				fprintf(stderr, "invalid input file count %s\n", acValue);
				return -5;
			}
			nFileCount = atoi(acValue);
			break;
		case 'h':
			print_usage();
			return 1;
		}
	}
	return 0;
}

INT32 check_valid_value(char *pcPath, INT32 nFileCount)
{
	if(pcPath == NULL || pcPath[0] == 0 || nFileCount < 1)
	{
		fprintf(stderr, "invalid input data\n");
		return -1;
	}

	if(create_dir(pcPath) < 0)
	{
		fprintf(stderr, "invalid input path %s (%d)\n", pcPath, errno);
		return -2;
	}

	if(nFileCount < 1 || nFileCount > 999)
	{
		fprintf(stderr, "invalid input file count %d (1~999)\n", nFileCount);
		return -3;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	INT32 nRetVal = 0;
	INT32 nFileMax = 0;
	INT32 i = 0;
	char acTestPath[MAX_PATH] = {0,};
	char acTestName[MAX_FILE_NAME] = {0,};
	char acFullPath[MAX_PATH] = {0,};
 
	if(argc < 7)
	{
		print_usage();
		exit(1);
	}

	nRetVal = set_opt(argc, argv, acTestPath, MAX_PATH, acTestName, MAX_FILE_NAME, nFileMax);
	if(nRetVal != 0)
	{
		exit(2);
	}
	snprintf(acFullPath, MAX_PATH-1, "%s/%s", acTestPath, acTestName);

	nRetVal = check_valid_value(acFullPath, nFileMax);
	if(nRetVal != 0)
	{
		exit(3);
	}
	check_tmp_dir();
	for(i=1; i<nFileMax+1; i++)
	{
		nRetVal = build_pgm(acFullPath, acTestName, i);
		if(nRetVal < 0)
		{
			exit(4);
		}
	}
	fprintf(stdout, "success to make %s's %d test files\n", acTestPath, nFileMax);

	return 0;
}


