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
#include "com_struct.h"
#include "as_registry.h"
#include "as_parse.h"
#include "as_sha256.h"
#include "as_zip.h"

#define ZIP_FILE 1
#define UPDATE_FILE 2
#define UNZIP_FILE 3

#define VERSION_FILE "version.nfo"
#define UPDATE_MODULE "update_module.tar"
#define UPDATE_SCRIPTS "update.sh" 

void print_usage()
{
	printf("Examples : \n");
	printf("\tupdate -z -v vesion -d zip_dir [-f zip_file_name]\n");
	printf("\t# create a zip file from zip_dir. (default zip file name : ssa_lnx_x64_XXXXXXXX.zip)\n");
	printf("\tupdate -u -f zip_file_name [-d ext_dir]\n");
	printf("\t# update by a zip file to ext_dir. (default ext dir : /usr/local/ashin/nanny)\n");
	printf("\tupdate -e -f zip_file_name [-d ext_dir]\n");
	printf("\t# extract by a zip file to ext_dir. (default ext dir : /usr/local/ashin/nanny/inven/_update)\n");
}

INT32 remove_file_all(char *pDirPath)
{
	char acFilePath[MAX_PATH] = {0,};
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	if(pDirPath == NULL)
		return -1;

	if(DirectoryExists(pDirPath) == FALSE)
	{
		return -2;
	}

	dp = opendir(pDirPath);
	if (dp == NULL)
	{
		return -3;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
		{
			continue;
		}

		if(DT_DIR == dirp->d_type)
		{
			continue;
		}
		snprintf(acFilePath, MAX_PATH-1, "%s/%s", pDirPath, (char *)dirp->d_name);
		unlink(acFilePath);
		Sleep(10);
	}

	closedir(dp);
	return 0;
}

int set_opt(int argc, char *argv[], char *pcFileName, int nFileMax, char *pcPath, int nPathMax, char *pcVersion, int nVersionMax)
{
	int c;
	extern char *optarg;
	int nRetVal = 0;
	if(pcFileName == NULL || nFileMax < 1 || pcPath == NULL || nPathMax < 1 || pcVersion == NULL || nVersionMax < 1)
		return 0;

	while ((c = getopt(argc, argv, "d:f:v:uezh")) != EOF)
	{
		switch ((char)c)
		{
		case 'd':
			if(optarg != NULL)
				strncpy(pcPath, optarg, nPathMax-1);
			break;
		case 'f':
			if(optarg != NULL)
				strncpy(pcFileName, optarg, nFileMax-1);
			break;
		case 'v':
			if(optarg != NULL)
				strncpy(pcVersion, optarg, nVersionMax-1);
			break;
		case 'u':
			nRetVal = UPDATE_FILE;
			break;
		case 'e':
			nRetVal = UNZIP_FILE;
			break;
		case 'z':
			nRetVal = ZIP_FILE;
			break;
		case 'h':
			print_usage();
			nRetVal = 0;
			break;
		}
	}
	return nRetVal;
}

int get_zip_path(char *pZipPath, int nMaxLen, char *pDirPath, char *pZipFileName)
{
	char acUpdatePath[MAX_PATH] = {0,};
	char acTempPath[MAX_PATH] = {0,};
	char acCurrentTime[MAX_TIME_STR] = {0,};
	int i = 0;
	int nRet = 0;
	if(pZipPath == NULL || nMaxLen < 1 || pDirPath == NULL)
		return -1;

	snprintf(acUpdatePath, MAX_PATH-1, "%s/udt", pDirPath);
	if(DirectoryExists(acUpdatePath) == FALSE)
	{
		if(ForceDirectories(acUpdatePath) == FALSE)
		{
			return -2;
		}
	}

	if(pZipFileName == NULL || pZipFileName[0] == 0)
	{
		GetCurrentDateTime(0, acCurrentTime);
		nRet = -3;
		for(i=1; i<33;)
		{
			snprintf(acTempPath, MAX_PATH-1, "%s/ssa_lnx_x64_%s_%02d.zip", acUpdatePath, acCurrentTime, i);
			if(is_file(acTempPath) != 0)
			{
				strncpy(pZipPath, acTempPath, nMaxLen-1);
				nRet = 0;
				break;
			}
			i += 2;
		}
	}
	else
	{
		snprintf(pZipPath, CHAR_MAX_SIZE-1, "%s/%s", acUpdatePath, pZipFileName);
		nRet = 0;
	}
	return nRet;
}


int create_version_file(char *pcRootDir, char *pcVersion)
{
	FILE *fp = NULL;
	char szVersionFile[MAX_PATH] = {0,};
	if(pcRootDir == NULL || pcVersion == NULL || pcVersion[0] == 0)
		return -1;

	snprintf(szVersionFile, MAX_PATH-1, "%s/%s", pcRootDir, VERSION_FILE);

	if(is_file(szVersionFile) == 0) 
	{
		unlink(szVersionFile);
	}
	fp = fopen(szVersionFile, "wt");
	if(fp == NULL)
	{
		printf("fail to open %s (%d)\n", szVersionFile, errno);
		return -3;
	}
	fwrite(pcVersion, 1, strlen(pcVersion), fp);
	fclose(fp);
	return 0;
}


int get_version_info(char *pcRootDir, char *pcVersion, int nVerMax)
{
	FILE *fp = NULL;
	char szVersionFile[MAX_PATH] = {0,};
	char szVersion[MAX_PATH] = {0,};
	if(pcRootDir == NULL || pcVersion == NULL || nVerMax < 1)
		return -1;

	snprintf(szVersionFile, MAX_PATH-1, "%s/%s", pcRootDir, VERSION_FILE);
	if(is_file(szVersionFile) != 0) 
	{
		return -2;
	}
	fp = fopen(szVersionFile, "rt");
	if(fp == NULL)
	{
		return -3;
	}
	if(fread(szVersion, 1, MAX_PATH-1, fp) == 0)
	{
		fclose(fp);
		return -4;
	}
	fclose(fp);
	if(strrchr(szVersion, '.') == NULL)
		return -5;
	strncpy(pcVersion, szVersion, nVerMax-1);
	return 0;
}

int set_version_info(char *pcVersion)
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	String strVersion;
	if(pcVersion == NULL)
	{
		return -1;
	}

	strVersion = SPrintf("%s", pcVersion);
	if(strVersion.empty())
	{
		return -2;
	}

	if(RegCreateKeyEx(HKEY_LOCAL_MACHINE, STR_REG_DEFAULT_SVC_LOCAL_PATH, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp) != 0)
	{
		return -3;
	}
	
	if(RegSetValueEx(hSubKey, "host_bin_ver", 0, REG_SZ, (PBYTE)strVersion.c_str(), strVersion.length()+1) != 0)
	{
		RegCloseKey(hSubKey);
		return -4;
	}
	RegCloseKey(hSubKey);
	return 0;
}

int get_ext_path(char *pExtPath, int nMaxLen, char *pDstDir, char *pZipFileName)
{
	char acExtractPath[MAX_PATH] = {0,};
	int i = 0;
	int nRet = 0;
	if(pExtPath == NULL || nMaxLen < 1)
		return -1;
	
	if(pDstDir == NULL || pDstDir[0] == 0)
	{
		char acRootPath[MAX_PATH] = {0,};
		nRet = get_nanny_agent_root(acRootPath, MAX_PATH);
		if(nRet != 0)
		{
			nRet -= 10;
			return nRet;
		}
		snprintf(acExtractPath, CHAR_MAX_SIZE-1, "%s/nanny/inven/_update", acRootPath);
		if(DirectoryExists(acExtractPath) == FALSE)
		{
			if(ForceDirectories(acExtractPath) == FALSE)
			{
				return -2;
			}
		}
		else
		{
			remove_file_all(acExtractPath);
		}
	}
	else
	{
		snprintf(acExtractPath, CHAR_MAX_SIZE-1, "%s", pDstDir);
		if(DirectoryExists(acExtractPath) == FALSE)
		{
			if(ForceDirectories(acExtractPath) == FALSE)
			{
				return -3;
			}
		}
		else
		{
			remove_file_all(acExtractPath);
		}
	}

	strncpy(pExtPath, acExtractPath, nMaxLen-1);
	return 0;
}

int main(int argc, char* argv[])
{
	char acDirPath[MAX_PATH] = {0,};
	char acVersion[MAX_PATH] = {0,};
	char acZipFileName[MAX_PATH] = {0,};
	char acZipPath[CHAR_MAX_SIZE] = {0,};
	int nZipFlag = 0;
	int i, nRetVal = 0;

	nZipFlag = set_opt(argc, argv, acZipFileName, MAX_PATH, acDirPath, MAX_PATH, acVersion, MAX_PATH);
	if(nZipFlag == 0)
	{
		print_usage();
		return 2;
	}

	if(nZipFlag == ZIP_FILE)
	{
		FILE *fp = NULL;
		char szFileHash[MAX_PATH] = {0,};
		char szHashPath[MAX_PATH] = {0,};
		char szModFile[MAX_PATH] = {0,};
		if(acDirPath[0] == 0 || acVersion[0] == 0)
		{
			print_usage();
			return 3;
		}
		nRetVal = get_zip_path(acZipPath, CHAR_MAX_SIZE, acDirPath, acZipFileName);
		if(nRetVal < 0)
		{
			printf("fail to get zip file name (%d)\n", nRetVal);
			return 4;
		}

		nRetVal = create_version_file(acDirPath, acVersion);
		if(nRetVal != 0)
		{
			printf("fail to create %s/version_info (%d)\n",acDirPath,  nRetVal);
			return 5;
		}

		snprintf(szModFile, MAX_PATH-1, "%s/%s", acDirPath, UPDATE_MODULE);
		if(is_file(szModFile) != 0)
		{
			printf("fail to find %s/update_module.tar (%d)\n", acDirPath, errno);
			return 6;
		}

		nRetVal = as_zip_file(acZipPath, acDirPath, AS_ZIP_UNZIP_PASS);
		if(nRetVal != 0)
		{
			printf("fail to create a %s (%d)\n", acZipPath, nRetVal);
			return 7;
		}

		printf("success to zip a %s\n", acZipPath);
		nRetVal = get_sha256_hash_from_file(acZipPath, szFileHash, MAX_PATH);
		if(nRetVal != 0)
		{
			printf("fail to get hash value from %s (%d)\n", acZipPath, nRetVal);
			return 8;
		}
		snprintf(szHashPath, MAX_PATH-1, "%s.txt", acZipPath);
		printf("success to hash a %s\n", acZipPath);

		fp = fopen(szHashPath, "wt");
		if(fp == NULL)
		{
			printf("fail to open %s (%d)\n", szHashPath, errno);
			return 9;
		}
		fwrite(szFileHash, 1, strlen(szFileHash), fp);
		fclose(fp);

		printf("success to create a %s from %s\n", acZipPath, acDirPath);
	}
	else if(nZipFlag == UPDATE_FILE)
	{
		char acShPath[MAX_PATH] = {0,};

		memset(acVersion, 0, MAX_PATH);
		if(acZipFileName[0] == 0)
		{
			print_usage();
			return 11;
		}

		nRetVal = get_ext_path(acZipPath, CHAR_MAX_SIZE, acDirPath, acZipFileName);
		if(nRetVal < 0)
		{
			printf("fail to get zip file name (%d)\n", nRetVal);
			return 12;
		}

		nRetVal = as_unzip_file(acZipFileName, acZipPath, AS_ZIP_UNZIP_PASS);
		if(nRetVal != 0)
		{
			printf("fail to extract a %s to %s (%d)\n", acZipFileName, acZipPath, nRetVal);
			return 13;
		}

		nRetVal = get_version_info(acZipPath, acVersion, MAX_PATH);
		if(nRetVal != 0)
		{
			printf("fail to get version (%s/%s) (%d)\n", acZipPath,VERSION_FILE, nRetVal);
			return 14;
		}
		nRetVal = set_version_info(acVersion);
		if(nRetVal != 0)
		{
			printf("fail to set version (%s) (%d)\n", acVersion, nRetVal);
			return 15;
		}

		snprintf(acShPath, MAX_PATH-1, "%s/%s", acZipPath, UPDATE_SCRIPTS);
		acShPath[MAX_PATH-1] = 0;

		if(chmod(acShPath, 0755) == -1)
		{
			printf("fail to chmod %s (%d)\n", acShPath, acZipPath, errno);
			return 16;
		}

//		unlink(acZipFileName);

		if(system(acShPath) == -1)
		{
			printf("fail to start %s (%d)\n", acShPath, errno);
			return 17;
		}
		
		printf("success to update product from %s\n", acZipFileName);
	}
	else if(nZipFlag == UNZIP_FILE)
	{
		if(acZipFileName[0] == 0)
		{
			print_usage();
			return 21;
		}

		nRetVal = get_ext_path(acZipPath, CHAR_MAX_SIZE, acDirPath, acZipFileName);
		if(nRetVal < 0)
		{
			printf("fail to get zip file name (%d)\n", nRetVal);
			return 22;
		}

		nRetVal = as_unzip_file(acZipFileName, acZipPath, AS_ZIP_UNZIP_PASS);
		if(nRetVal != 0)
		{
			printf("fail to extract a %s to %s (%d)\n", acZipFileName, acZipPath, nRetVal);
			return 23;
		}
		printf("success to unzip from %s to %s\n", acZipFileName, acZipPath);
	}
	else
	{
		print_usage();
		return 3;
	}
	
	return 0;
}
