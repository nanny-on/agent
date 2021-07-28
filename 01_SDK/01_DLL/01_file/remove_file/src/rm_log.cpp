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
#include "rm_log.h"

typedef list<DeleteFileList_t*> DEL_FILE_LIST;
typedef DEL_FILE_LIST::iterator delItor;

void find_entry(DEL_FILE_LIST& FindList, int maxChkDay);

INT32 RemoveFileByDT(const char* lpFindPath, unsigned int nChkType, unsigned int nDay)
{
	time_t  tCurrentTime, tChkTime;
	char acDelFilePath[CHAR_MAX_SIZE] = {0,};
	int nChkDay, nChkHour, nChkMin, nChkSec;
	INT32 nRemoveCnt = 0;
	double  dDiffTime = 0;
	struct stat stFileStat;
	DIR *pDir = NULL;
	struct dirent *pDirEnt = NULL;

	if(nDay == 0)
	{
		return 0;
	}

	if(lpFindPath == NULL)
		return -11;
	
	if (lstat(lpFindPath, &stFileStat) != 0)
	{
		return -12;
	}

	if (!S_ISDIR(stFileStat.st_mode))
	{
		return 0;
	}

	if ( (pDir = opendir(lpFindPath)) == NULL)
	{
		return -13;
	}

	while((pDirEnt = readdir(pDir)) != NULL)
	{
		if (!_stricmp(pDirEnt->d_name, ".") || !_stricmp(pDirEnt->d_name,".."))
		{
			continue;
		}

		if(DT_REG != pDirEnt->d_type)
		{
			continue;
		}

		snprintf(acDelFilePath, CHAR_MAX_SIZE-1, "%s/%s", lpFindPath, pDirEnt->d_name);
		acDelFilePath[CHAR_MAX_SIZE-1] = 0;

		if (lstat(acDelFilePath, &stFileStat) != 0)
		{
			closedir(pDir);
			return -14;
		}

		if(nChkType == FILE_DT_CHK_TYPE_WRITE)
		{
			tChkTime = stFileStat.st_mtime;
		}
		else if(nChkType == FILE_DT_CHK_TYPE_ACCESS)
		{
			tChkTime = stFileStat.st_atime;
		}
		else
		{
			tChkTime = stFileStat.st_ctime;
		}
		time(&tCurrentTime);
		dDiffTime = difftime(tCurrentTime, tChkTime);
		nChkDay = dDiffTime / (60 * 60 * 24);
		if (nChkDay < nDay)
		{
			continue;
		}
		if(unlink(acDelFilePath) == 0)
			nRemoveCnt++;
	}
	closedir(pDir);
	
	return nRemoveCnt;
}

int FindFileByLastDay(const char* lpFindPath, unsigned int nChkType, unsigned int nDay, DEL_FILE_LIST& DelFileList, time_t& tOldestDay)
{
	DIR  *pDir = NULL;
	struct dirent *pDirEnt = NULL;
	char acDelFilePath[CHAR_MAX_SIZE] = {0,};
	struct stat stFileStat;
	time_t  tChkTime = 0;
	UINT32 nChkDay = 0;

	if(nChkType != FILE_DT_CHK_TYPE_WRITE && nChkType != FILE_DT_CHK_TYPE_ACCESS)
	{
		return 0;
	}

	if(lpFindPath == NULL)
		return -1;

	if ( (pDir = opendir(lpFindPath)) == NULL)
	{
		return -2;
	}

	while((pDirEnt = readdir(pDir)) != NULL)
	{
		if (strcmp(pDirEnt->d_name, ".") == 0 || strcmp(pDirEnt->d_name,"..") == 0)
		{
			continue;
		}
		snprintf(acDelFilePath, CHAR_MAX_SIZE-1, "%s/%s", lpFindPath, pDirEnt->d_name);
		acDelFilePath[CHAR_MAX_SIZE-1] = 0;

		if(DT_DIR == pDirEnt->d_type)
		{
			FindFileByLastDay(acDelFilePath, nChkType, nDay, DelFileList, tOldestDay);
		}
		else if(DT_REG == pDirEnt->d_type)
		{
			if (lstat(acDelFilePath, &stFileStat) != 0)
			{
				closedir(pDir);
				return -3;
			}
			if(nChkType == FILE_DT_CHK_TYPE_WRITE)
			{
				tChkTime = stFileStat.st_mtime;
			}
			else if(nChkType == FILE_DT_CHK_TYPE_ACCESS)
			{
				tChkTime = stFileStat.st_atime;
			}

			nChkDay = tChkTime/(60*60*24);
			if(tOldestDay == 0)
				tOldestDay = nChkDay;
			else if(tOldestDay > nChkDay)
				tOldestDay = nChkDay;

			DeleteFileList_t* pFileInfoList = (DeleteFileList_t*)malloc(sizeof(DeleteFileList_t));
			if(pFileInfoList != NULL)
			{
				pFileInfoList->nChkDay = nChkDay;
				snprintf(pFileInfoList->acFilePath, CHAR_MAX_SIZE-1, "%s", lpFindPath);
				snprintf(pFileInfoList->acFileName, CHAR_MAX_SIZE-1, "%s", pDirEnt->d_name);
				DelFileList.push_back(pFileInfoList);
			}
		}
	}
	closedir(pDir);
	return 0;
}

int RemoveFileByLastDay(const char* lpFindPath, unsigned int nChkType, unsigned int nDay)
{
	struct stat stFindStat;
	DeleteFileList_t* pInfo = NULL;

	if(lpFindPath == NULL)
	{
		return -1;
	}
	if (lstat(lpFindPath, &stFindStat) != 0)
	{
		return -2;
	}

	time_t tOldestDay = 0;
	DEL_FILE_LIST stDelFileList;

	if (S_ISDIR(stFindStat.st_mode)) 
	{
		FindFileByLastDay(lpFindPath, nChkType, nDay, stDelFileList, tOldestDay);
		find_entry(stDelFileList, (UINT32)tOldestDay + nDay);
	}

	delItor begin, end;	
	begin = stDelFileList.begin(); end = stDelFileList.end();
	for(;begin != end; begin++)	
	{	
		pInfo = *begin;
		safe_free(pInfo);
	}
	stDelFileList.clear();	
	
	return 0;
}

void find_entry(DEL_FILE_LIST& FindList, int maxChkDay)
{
	DeleteFileList_t *pList = NULL;
	char szFullPath[CHAR_MAX_SIZE] = {0,};
	
	delItor begin, end;
	begin = FindList.begin(); end = FindList.end();
	for(;begin != end; begin++)
	{
		pList = *begin;
		if(pList == NULL)
		{
			continue;
		}
		if (pList->nChkDay < maxChkDay)
		{
			snprintf(szFullPath, CHAR_MAX_SIZE-1, "%s/%s", pList->acFilePath,pList->acFileName);
			szFullPath[CHAR_MAX_SIZE-1] = 0;
			unlink(szFullPath);
		}
	}
}
