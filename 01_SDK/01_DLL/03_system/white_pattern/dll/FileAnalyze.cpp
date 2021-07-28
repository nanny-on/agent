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
 */

#include "stdafx.h"
#include "com_struct.h"
#include "ElfFile.h"
#include "FileAnalyze.h"

CFileAnalyze::CFileAnalyze(void)
{
}

CFileAnalyze::~CFileAnalyze(void)
{
}

INT32 CFileAnalyze::GetFileData(char *szPath, PFILE_INFO pFileInfo, char *acLogMsg)
{
	struct stat st;
	char acBaseName[MAX_QQBUFF] = {0,};

	if(szPath == NULL || szPath[0] == 0 || pFileInfo == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetFileData] invalid input data");
		return -1;
	}

	if(stat(szPath, &st) < 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetFileData] fail to stat %s (%d)", szPath, errno);
		return -2;
	}

	pFileInfo->dwFileSize = (DWORD)st.st_size;
	pFileInfo->dwWriteTime = (DWORD)st.st_mtime;
	pFileInfo->dwAccessTime = (DWORD)st.st_atime;

	if(get_basename(szPath, acBaseName, MAX_QQBUFF-1) != NULL)
	{
		strncpy(pFileInfo->acFileName, acBaseName, MAX_QQBUFF-1);
		pFileInfo->acFileName[MAX_QQBUFF-1] = 0;
	}
	return 0;
}

INT32 CFileAnalyze::GetElfData(char *szPath, PELF_DATA pElfData, char *acLogMsg)
{
	INT32 nRetVal = 0;
	CElfFileInfo *pElfFileInfo = NULL;

	if(szPath == NULL || szPath[0] == 0 || pElfData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetElfData] invalid input data");
		return -1;
	}

	pElfFileInfo = new CElfFileInfo;
	if(pElfFileInfo == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetElfData] fail to allocate memory (%d)", errno);
		return -2;
	}

	nRetVal = pElfFileInfo->AnalysisElfFile(szPath, pElfData, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
	}
	delete pElfFileInfo;
	return nRetVal;
}