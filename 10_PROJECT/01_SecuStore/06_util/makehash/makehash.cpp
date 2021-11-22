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
#include "as_sha256.h"
#include "makehash.h"

#define OUTPUT_FILENAME		"file_hash.info"

#define DLL_FILE_EXT		".so"

void PrintUsage()
{
	printf("usage : makehash directory\n");
	printf("ex    : makehash /usr/local/ashin/nanny\n");
}

int MakeModuleHashFromDir(const char* pcRootDir, char *pcCurDir, FILE* fRes)
{
	DIR *dp;
	struct dirent *dirp;
	struct stat st;
	size_t nRet = 0;

	int nRootLen = 0;
	int nFileLen = 0;
	char acFileName[MAX_PATH] = {0,};
	char acFullFilePath[CHAR_MAX_SIZE] = {0,};
	char *pRelPath = NULL;
	FILE_HASH_INFO fhi;
	CHAR szFileHash[CHAR_MAX_SIZE] = {0, };
	CHAR szFileExt[8] = {0,};
	
	if(pcRootDir == NULL || pcCurDir == NULL || fRes == NULL)
	{
		printf("invalid input data\n");
		return -1;
	}
	
	nRootLen = strlen(pcRootDir);
	if(nRootLen == 0 || nRootLen > CHAR_MAX_SIZE-1)
		
		return -1;

	dp = opendir(pcCurDir);
	if (dp == NULL)
	{
		printf("can't open '%s' directory (%d)\n", pcCurDir, errno);
		return -1;
	}

	int nRtn = 0;
	while((dirp = readdir(dp)) != NULL)
	{
		strncpy(acFileName, dirp->d_name, MAX_PATH-1);
		acFileName[MAX_PATH-1] = 0;

		snprintf(acFullFilePath, CHAR_MAX_SIZE-1, "%s/%s", pcCurDir, acFileName);
		acFullFilePath[CHAR_MAX_SIZE-1] = 0;
		
		if(DT_DIR == dirp->d_type)
		{
			if(acFileName[0] != '.')
			{
				nRtn = MakeModuleHashFromDir(pcRootDir, acFullFilePath, fRes);
				if(nRtn == -1)
					return -1;
			}
		}
		else if(DT_REG == dirp->d_type)
		{
			memset(&fhi, 0, sizeof(FILE_HASH_INFO));
			pRelPath = &acFullFilePath[nRootLen+1];
			
			nFileLen = strlen(acFileName);
			if(nFileLen > 3)
			{
				strncpy(szFileExt, &acFileName[nFileLen-3], 3);
				szFileExt[3] = 0;
				
				if(!_stricmp(acFileName, NANNY_AGENT_IDENT) || !_stricmp(szFileExt, DLL_FILE_EXT))
				{
				    nRtn = get_sha256_hash_from_file(acFullFilePath, szFileHash, CHAR_MAX_SIZE);
					if(nRtn == 0)
					{
						strncpy(fhi.szFileName, pRelPath, MAX_PATH-1);
						fhi.szFileName[MAX_PATH-1] = 0;
						
						strncpy(fhi.szFileHash, szFileHash, MAX_PATH-1);
						fhi.szFileHash[MAX_PATH-1] = 0;
						printf("----------------------------------------------------------------------------------------------\n");
						printf("file: %s\nshort name: %s\nhash: %s\n", acFullFilePath, fhi.szFileName, fhi.szFileHash);
						if((nRet = fwrite(&fhi, 1, sizeof(fhi), fRes)) != sizeof(fhi))
						{
							printf("failed to write %s hash (ret:%d). (errno:%d)\n", acFullFilePath, nRet, errno);
							return -1;
						}
					}
					else
					{
						printf("cannot get hash of [%s]\n", acFullFilePath);
					}
				}
			}
		}
	}

	closedir(dp);
	return 0;
}

int main(int argc, char* argv[])
{
	int ret;
	struct stat st;
	char szRootPath[CHAR_MAX_SIZE] = {0,};
	char szHashFile[CHAR_MAX_SIZE] = {0,};
	int nLen = 0;
	
	if(argc < 2)
	{
		PrintUsage();
		return 0;
	}

	strncpy(szRootPath, argv[1], CHAR_MAX_SIZE-1);
	szRootPath[CHAR_MAX_SIZE-1] = 0;
	
	nLen = strlen(szRootPath);
	if(nLen == 1 && szRootPath[nLen - 1] == '/')
	{
		printf("select to other path\n");
		return 0;
	}
	else if(nLen > 1 && szRootPath[nLen - 1] == '/')
		szRootPath[nLen - 1] = '\0';

	ret = stat(szRootPath, &st);
	if (ret < 0)
	{
		PrintUsage();
		return 0;
	}
	if(!(st.st_mode & S_IFDIR))
	{
		PrintUsage();
		return 0;
	}

	snprintf(szHashFile, CHAR_MAX_SIZE-1, "%s/dll/%s", szRootPath, OUTPUT_FILENAME);

	FILE* fRes = fopen(szHashFile, "w");
	if(fRes == NULL)
	{
		printf("cannot open file [%s]. %d\n", szHashFile, errno);
		return 0;
	}

	if(MakeModuleHashFromDir(szRootPath, szRootPath, fRes) == 0)
		printf("----------------------------------------------------------------------------------------------\n");

	fclose(fRes);

	return 1;
}
