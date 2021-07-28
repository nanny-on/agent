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
#include "FileAnalyze.h"
#include "AriaEx.h"
#include "WhitePatternUtil.h"

CWhitePatternUtil::CWhitePatternUtil()
{

}
//------------------------------------------------------------------------------------------------------

CWhitePatternUtil::~CWhitePatternUtil()
{

}
//------------------------------------------------------------------------------------------------------

INT32 CWhitePatternUtil::get_signiture(char *szPath, PASI_WENG_WL_EX pWhiteList, char *acLogMsg)
{
	INT32 nRetVal = 0;
	CFileAnalyze cFileaAalyze;
	PFILE_INFO pFileData = NULL;
	PELF_DATA pElfData = NULL;
	DWORD dwLen = 0;
	char szHashVal[SHA512_BLOCK_SIZE+HASH_PADDING_LEN] = {0,};

	if(NULL == szPath || NULL == pWhiteList)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[get_signiture] invalid input data");
		return -1;
	}

	do{
		pFileData = (PFILE_INFO)malloc(sizeof(FILE_INFO));
		if(pFileData == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[get_signiture] fail to allocate file data memory (%d)", errno);
			nRetVal = -2;
			break;
		}
		memset(pFileData, 0, sizeof(FILE_INFO));
		pElfData = (PELF_DATA)malloc(sizeof(ELF_DATA)+HASH_PADDING_LEN);
		if(pElfData == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[get_signiture] fail to allocate elf data memory (%d)", errno);
			nRetVal = -3;
			break;
		}
		memset(pElfData, 0, sizeof(ELF_DATA)+HASH_PADDING_LEN);
		nRetVal = cFileaAalyze.GetFileData(szPath, pFileData, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}
		
		if(pFileData->dwFileSize <= 0)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[get_signiture] invalid file size (%d)", pFileData->dwFileSize);
			nRetVal = -4;
			break;
		}

		nRetVal = cFileaAalyze.GetElfData(szPath, pElfData, acLogMsg);
		if(nRetVal == 0) //elf
		{
			nRetVal = SHAString(ASIHASH_SHA_LEN_TYPE_512, (LPCSTR)pElfData, sizeof(ELF_DATA), szHashVal, SHA512_BLOCK_SIZE+1);
			if(nRetVal < 0)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[get_signiture] fail to get sha string (%d)", nRetVal);
				nRetVal = -5;
				break;
			}
			pWhiteList->dwMasic = pElfData->stElfHeader.dwMachine;
		}
		else if(nRetVal == 1) //scripts
		{
			pWhiteList->dwMasic = 0xFF;
			nRetVal = SHAFile(ASIHASH_SHA_LEN_TYPE_512, szPath, szHashVal, SHA512_BLOCK_SIZE+1);
		}
		else
		{
			nRetVal -= 20;
			break;
		}
		strncpy(pWhiteList->acFilePath, szPath, MAX_PATH-1);
		pWhiteList->acFilePath[MAX_PATH-1] = 0;
		//File Length 저장
		pWhiteList->dwFileSize = pFileData->dwFileSize;
		//File Write Time 저장
		pWhiteList->dwWriteTime = pFileData->dwWriteTime;
		//File Access Time 저장
		pWhiteList->dwAccessTime = pFileData->dwAccessTime;
		//hash value 저장
		strncpy(pWhiteList->acWhiteHash, szHashVal, SHA512_BLOCK_SIZE);
		pWhiteList->acWhiteHash[SHA512_BLOCK_SIZE] = 0;
		nRetVal = 0;
	}while(FALSE);

	safe_free(pFileData);
	safe_free(pElfData);

	return nRetVal;
}

//----------------------------------
// file add function
//----------------------------------
INT32 CWhitePatternUtil::we_fadd(char *szPath, char *acLogMsg)
{
	PASI_WENG_WL_EX pWhiteListEx = NULL;
	INT32 nRetVal = 0;
	if(szPath == NULL || szPath[0] == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_fadd] invalid input data");
		return -1;
	}
	pWhiteListEx = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(NULL == pWhiteListEx)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_fadd] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pWhiteListEx, 0, sizeof(ASI_WENG_WL_EX));
	do{
		nRetVal = get_signiture(szPath, pWhiteListEx, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 1000;
			break;
		}

		nRetVal = m_tHashUtil.AddHashNodeWithLock(pWhiteListEx, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 2000;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	safe_free(pWhiteListEx);
	return nRetVal;
}

//----------------------------------
// file del function
//----------------------------------
INT32 CWhitePatternUtil::we_fdel(char *szPath, char *acLogMsg)
{
	PASI_WENG_WL_EX pWhiteListEx = NULL;
	INT32 nRetVal = 0;
	if(szPath == NULL || szPath[0] == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_fdel] invalid input data");
		return -1;
	}

	pWhiteListEx = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(NULL == pWhiteListEx)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_fdel] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pWhiteListEx, 0, sizeof(ASI_WENG_WL_EX));
	do{
		nRetVal = get_signiture(szPath, pWhiteListEx, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}

		if(pWhiteListEx->acWhiteHash[0] == 0)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[we_fdel] invalid hash data");
			nRetVal = -3;
			break;
		}
		nRetVal = m_tHashUtil.DelHashNodeWithLock(pWhiteListEx, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 20;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	safe_free(pWhiteListEx);
	return nRetVal;
}

void CWhitePatternUtil::we_fclear()
{
	m_tHashUtil.ClearHashNodeWithLock();
}


INT32 CWhitePatternUtil::we_dbmake(char *acEnginePath, char *acDbName, char *acLogMsg)
{
	INT32 nRetVal = 0;
	FILE *fp = NULL;
	PWHITE_FILE_HDR pFileHdr = NULL;
	PCHAR pcPatternFile = NULL;
	PCHAR pcTempFile = NULL;
	INT32 nListCount = 0;
	time_t tTime = 0;

	if(acEnginePath == NULL || acEnginePath[0] == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_dbmake] invalid input data");
		return -1;
	}

	pcPatternFile = (PCHAR)malloc(MAX_PATH);
	if(pcPatternFile == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_dbmake] fail to allocate memory (%d)", errno);
		return -2;
	}

	pcTempFile = (PCHAR)malloc(MAX_PATH);
	if(pcTempFile == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_dbmake] fail to allocate memory (%d)", errno);
		safe_free(pcPatternFile);
		return -3;
	}

	pFileHdr = (PWHITE_FILE_HDR)malloc(sizeof(WHITE_FILE_HDR));
	if(pFileHdr == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_dbmake] fail to allocate memory (%d)", errno);
		safe_free(pcPatternFile);
		safe_free(pcTempFile);
		return -4;
	}

	do{
		memset(pcPatternFile, 0, MAX_PATH);
		memset(pcTempFile, 0, MAX_PATH);
		memset(pFileHdr, 0, sizeof(WHITE_FILE_HDR));

		time(&tTime);
		pFileHdr->dwCreateTime = (DWORD)tTime;
		pFileHdr->dwWriteTime = (DWORD)tTime;
		pFileHdr->dwCategory = AS_LOCAL_WHITE;

		if(acDbName == NULL || acDbName[0] == 0)
		{
			strncpy(pFileHdr->acDbName, WDB_FILE_NAME, MAX_QQBUFF-1);
			pFileHdr->acDbName[MAX_QQBUFF-1] = 0;
		}
		else
		{
			strncpy(pFileHdr->acDbName, acDbName, MAX_QQBUFF-1);
			pFileHdr->acDbName[MAX_QQBUFF-1] = 0;
			nRetVal = (INT32)strlen(pFileHdr->acDbName);
			if(nRetVal > 4)
			{
				if(!_stricmp(&pFileHdr->acDbName[nRetVal-4], ".wdb"))
				{
					pFileHdr->acDbName[nRetVal-4] = 0;
				}
			}
			nRetVal = 0;
		}
		snprintf(pcPatternFile, MAX_PATH-1, "%s/%s.wdb", acEnginePath, pFileHdr->acDbName);
		pcPatternFile[MAX_PATH-1] = 0;
		snprintf(pcTempFile, MAX_PATH-1, "%s/%s.tmp", acEnginePath, pFileHdr->acDbName);
		pcTempFile[MAX_PATH-1] = 0;

		if(is_file(pcTempFile) == 0)
		{
			unlink(pcTempFile);
		}

		fp = fopen(pcTempFile, "w");
		if(fp == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[we_dbmake] fail to open file %s (%d) ", pcTempFile, errno);
			nRetVal = -5;
			break;
		}

		nRetVal = m_tHashUtil.PrintFileHeader(fp, pFileHdr, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}

		nRetVal = m_tHashUtil.PrintAllHashNodeWithLock(fp, AS_LOCAL_WHITE, NULL, &nListCount, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 20;
			break;
		}

		if(nListCount < 1)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[we_dbmake] invalid white list count (%d)", nListCount);
			nRetVal = -6;
			break;
		}

		nRetVal = 0;
	}while(FALSE);

	if(fp != NULL)
	{
		fclose(fp);
	}

	if(nRetVal == 0)
	{
		nRetVal = AriaEnCryptFile(ASI_ARIA, pcTempFile, pcPatternFile, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 30;
		}
	}
	if(is_file(pcTempFile) == 0)
	{
		unlink(pcTempFile);
	}
	safe_free(pcTempFile);
	safe_free(pcPatternFile);
	safe_free(pFileHdr);
	return nRetVal;
}

INT32 CWhitePatternUtil::we_pattern_load(char *acEnginePath, char *acLogMsg)
{
	FILE *fp = NULL;
	INT32 nRetVal = 0;
	INT32 nLen = 0;
	INT32 nCount = 0;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	char szPattenFile[MAX_PATH] = {0,};

	if(acEnginePath == NULL || acEnginePath[0] == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_pattern_load] invalid input data");
		return -1;
	}

	nLen = (INT32)strlen(acEnginePath);
	if(nLen < 1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_pattern_load] invalid input data");
		return -1;
	}

	if((nLen > 2) && (acEnginePath[nLen-1] == '/'))
	{
		acEnginePath[nLen-1] = 0;
	}

	dp = opendir(acEnginePath);
	if (dp == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_pattern_load] fail to open %s (%d)", acEnginePath, errno);
		return -3;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
			continue;
		if(DT_DIR == dirp->d_type)
			continue;

		nLen = (INT32)strlen(dirp->d_name);
		if(nLen < 5)
			continue;
		if(_stricmp(&dirp->d_name[nLen-4], ".wdb"))
			continue;

		snprintf(szPattenFile, MAX_PATH-1, "%s/%s", acEnginePath, dirp->d_name);
		szPattenFile[MAX_PATH-1] = 0;

		nCount = 0;

		nRetVal = asi_load_white_list(szPattenFile, NULL, &nCount, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}
		nRetVal = 0;
	}

	closedir(dp);

	return nRetVal;
}


INT32 CWhitePatternUtil::scan_white_path(FILE *fp, char *acPath, DWORD dwDepth, BOOL bIsSaveReserved, char *acLogMsg)
{
	INT32 nRetVal = 0;
	INT32 nLength = 0;
	PASI_WENG_WL_EX pWhiteListEx = NULL;
	PCHAR pcPathName = NULL;
	PCHAR pcWhiteInfo = NULL;
	DWORD dwDirDepth = 0;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;

	dwDirDepth = dwDepth + 1;

	if(acPath == NULL || acPath[0] == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[scan_white_path] invalid input data (dp:%d)", dwDirDepth);
		return -1;
	}

	if(bIsSaveReserved == TRUE && dwDirDepth > MAX_RESERVED_DEPTH)
	{
		nRetVal = m_tHashUtil.AddReservedPathWithLock(acPath);
		if(nRetVal < 0)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[scan_white_path] fail to add reserved path (%s) (dp:%d) (ret:%d)", acPath, dwDirDepth, nRetVal);
			nRetVal -= 10;
			return nRetVal;
		}
		return 0;
	}

	if(fp == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[scan_white_path] invalid input data (%d)", dwDirDepth);
		return -2;
	}

	pcPathName = (PCHAR)malloc(MAX_PATH);
	if(pcPathName == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[scan_white_path] fail to allocate path memory (dp:%d) (ret:%d)", dwDirDepth, errno);
		return -3;
	}
	memset(pcPathName, 0, MAX_PATH);

	pWhiteListEx = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(pWhiteListEx == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[scan_white_path] fail to allocate white memory (dp:%d) (ret:%d)", dwDirDepth, errno);
		safe_free(pcPathName);
		return -4;
	}

	pcWhiteInfo = (PCHAR)malloc(MAX_BUFF);
	if(pcWhiteInfo == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[scan_white_path] fail to allocate info memory (dp:%d) (ret:%d)", dwDirDepth, errno);
		safe_free(pcPathName);
		safe_free(pWhiteListEx);
		return -5;
	}
	memset(pcWhiteInfo, 0, MAX_BUFF);

	dp = opendir(acPath);
	if (dp == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[scan_white_path] fail to open %s (dp:%d) (ret:%d)", acPath, dwDirDepth, errno);
		safe_free(pcPathName);
		safe_free(pWhiteListEx);
		safe_free(pcWhiteInfo);
		return -6;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
			continue;

		snprintf(pcPathName, MAX_PATH-1, "%s/%s", acPath, dirp->d_name);
		pcPathName[MAX_PATH-1] = 0;
		if(DT_DIR == dirp->d_type)
		{
			nRetVal = scan_white_path(fp, pcPathName, dwDirDepth, bIsSaveReserved, acLogMsg);
			if(nRetVal < 0)
			{
				nRetVal -= 20;
				break;
			}
			continue;
		}
		else if(DT_REG == dirp->d_type)
		{
			memset(pWhiteListEx, 0, sizeof(ASI_WENG_WL_EX));

			if(get_signiture(pcPathName, pWhiteListEx, acLogMsg) < 0)
			{
				if(acLogMsg)
					acLogMsg[0] = 0;
				continue;
			}
			if(m_tHashUtil.AddHashNodeWithLock(pWhiteListEx, acLogMsg) < 0)
			{
				if(acLogMsg)
					acLogMsg[0] = 0;
				continue;
			}
			//hash | path | tag | product | company | desc | sign company | category | size | magic | response | role | create time | access time | write time | version high | version low
			snprintf(pcWhiteInfo, MAX_BUFF-1, FMT_WHITE_LIST, pWhiteListEx->acWhiteHash, RULE_SEP, pWhiteListEx->acFilePath, RULE_SEP,pWhiteListEx->acTagName[0]?pWhiteListEx->acTagName:"NULL", RULE_SEP, 
				"NULL", RULE_SEP, "NULL", RULE_SEP, "NULL", RULE_SEP, "NULL", RULE_SEP,
				pWhiteListEx->dwCategory, RULE_SEP, pWhiteListEx->dwFileSize, RULE_SEP, pWhiteListEx->dwMasic, RULE_SEP, pWhiteListEx->dwResponse, RULE_SEP, 
				pWhiteListEx->dwRole, RULE_SEP, 0, RULE_SEP, pWhiteListEx->dwAccessTime, RULE_SEP, pWhiteListEx->dwWriteTime, RULE_SEP, 
				0, RULE_SEP, 0);
			pcWhiteInfo[MAX_BUFF-1] = 0;
			nLength = (INT32)strlen(pcWhiteInfo);
			fprintf(fp, FMT_WHITE_LIST_HDR, nLength, RULE_SEP, pcWhiteInfo);

		}
		nRetVal = 0;
	}

	closedir(dp);
	safe_free(pWhiteListEx);
	safe_free(pcPathName);
	safe_free(pcWhiteInfo);
	return nRetVal;
}


INT32 CWhitePatternUtil::we_pattern_create(char *acWhitePath, char *acEnginePath, char *acDbName, char *acLogMsg)
{
	INT32 nRetVal = 0;
	FILE *fp = NULL;
	DWORD dwDepth = 0;
	DWORD i, dwReservedCount = 0;
	time_t tTime = 0;
	PRESEVED_PATH_LIST pReservedList = NULL;
	PWHITE_FILE_HDR pFileHdr = NULL;
	PCHAR pcTempFile = NULL;
	PCHAR pcPatternFile = NULL;

	if(acWhitePath == NULL || acEnginePath == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_pattern_create] invalid input data");
		return -1;
	}

	pcTempFile = (PCHAR)malloc(MAX_PATH);
	if(pcTempFile == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_pattern_create] fail to allocate memory (%d)", errno);
		return -2;
	}

	pcPatternFile = (PCHAR)malloc(MAX_PATH);
	if(pcPatternFile == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_pattern_create] fail to allocate memory (%d)", errno);
		safe_free(pcTempFile);
		return -3;
	}
	
	pFileHdr = (PWHITE_FILE_HDR)malloc(sizeof(WHITE_FILE_HDR));
	if(pFileHdr == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[we_pattern_create] fail to allocate memory (%d)", errno);
		safe_free(pcTempFile);
		safe_free(pcPatternFile);
		return -4;
	}

	do{
		memset(pcTempFile, 0, MAX_PATH);
		memset(pcPatternFile, 0, MAX_PATH);
		memset(pFileHdr, 0, sizeof(WHITE_FILE_HDR));

		time(&tTime);
		pFileHdr->dwCreateTime = (DWORD)tTime;
		pFileHdr->dwWriteTime = (DWORD)tTime;
		pFileHdr->dwCategory = AS_LOCAL_WHITE;

		if(acDbName == NULL || acDbName[0] == 0)
		{
			strncpy(pFileHdr->acDbName, WDB_FILE_NAME, MAX_QQBUFF-1);
			pFileHdr->acDbName[MAX_QQBUFF-1] = 0;
		}
		else
		{
			strncpy(pFileHdr->acDbName, acDbName, MAX_QQBUFF-1);
			pFileHdr->acDbName[MAX_QQBUFF-1] = 0;
			nRetVal = (INT32)strlen(pFileHdr->acDbName);
			if(nRetVal > 4)
			{
				if(!_stricmp(&pFileHdr->acDbName[nRetVal-4], ".wdb"))
				{
					pFileHdr->acDbName[nRetVal-4] = 0;
				}
			}
			nRetVal = 0;
		}

		snprintf(pcPatternFile, MAX_PATH-1, "%s/%s.wdb", acEnginePath, pFileHdr->acDbName);
		pcPatternFile[MAX_PATH-1] = 0;
		snprintf(pcTempFile, MAX_PATH-1, "%s/%s.tmp", acEnginePath, pFileHdr->acDbName);
		pcTempFile[MAX_PATH-1] = 0;

		if(is_file(pcTempFile) == 0)
		{
			unlink(pcTempFile);
		}

		fp = fopen(pcTempFile, "w");
		if(fp == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[we_pattern_create] fail to open file %s (%d) ", pcTempFile, errno);
			nRetVal = -5;
			break;
		}

		nRetVal = m_tHashUtil.PrintFileHeader(fp, pFileHdr, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}

		nRetVal = scan_white_path(fp, acWhitePath, dwDepth, 0, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 20;
			break;
		}
		dwReservedCount = m_tHashUtil.GetReservedPathCountWithLock();
		if(dwReservedCount != 0)
		{
			pReservedList = (PRESEVED_PATH_LIST)malloc(sizeof(RESEVED_PATH_LIST)*dwReservedCount);
			if(NULL == pReservedList)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[we_pattern_create] fail to allocate memory (%d)", errno);
				nRetVal = -6;
				m_tHashUtil.ClearReservedPathWithLock();
				break;
			}
			memset(pReservedList, 0, sizeof(RESEVED_PATH_LIST)*dwReservedCount);
			nRetVal = m_tHashUtil.GetReservedPathWithLock(pReservedList, dwReservedCount);
			if(nRetVal < 0)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[we_pattern_create] fail to get reserved list");
				nRetVal -= 30;
				safe_free(pReservedList);
				m_tHashUtil.ClearReservedPathWithLock();
				break;
			}
			for(i=0;i<dwReservedCount; i++)
			{
				dwDepth = 0;
				nRetVal = scan_white_path(fp, pReservedList[i].acFilePath, dwDepth, FALSE, acLogMsg);
				if(nRetVal < 0)
				{
					nRetVal -= 40;
					break;
				}
				nRetVal = 0;
			}
			safe_free(pReservedList);
			m_tHashUtil.ClearReservedPathWithLock();
			if(nRetVal < 0)
			{
				break;
			}
		}

		nRetVal = 0;
	}while(FALSE);

	if(fp != NULL)
	{
		fclose(fp);
	}
	if(nRetVal == 0)
	{
		nRetVal = AriaEnCryptFile(ASI_ARIA, pcTempFile, pcPatternFile, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 50;
		}
	}
	if(is_file(pcTempFile) == 0)
	{
		unlink(pcTempFile);
	}

	safe_free(pFileHdr);
	safe_free(pcTempFile);
	safe_free(pcPatternFile);
	return nRetVal;
}

INT32 CWhitePatternUtil::asi_parse_list_v1(char *pBuffer, PWHITE_FILE_HDR pFileHdr, PASI_WENG_WL_EX pWhiteList, char *acLogMsg)
{
	char *pRule = NULL;
	PCHAR pToken = NULL;
	INT32 nRetVal = 0;
	if(pBuffer == NULL || pFileHdr == NULL || pWhiteList == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] invalid input data");
		return -1;
	}
	pToken = (PCHAR)malloc(MAX_BUFF);
	if(pToken == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pToken, 0, MAX_BUFF);
	pRule = pBuffer;
	do{
		//Hash
		pRule = copy_token(pToken, pRule, RULE_SEP);
		if(pRule == NULL || pToken[0] == 0)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] invalid hash (%s)", pFileHdr->acDbName);
			nRetVal = -3;
			break;
		}
		strncpy(pWhiteList->acWhiteHash, pToken, SHA512_BLOCK_SIZE);
		pWhiteList->acWhiteHash[SHA512_BLOCK_SIZE] = 0;

		//File Path
		pRule = copy_token(pToken, pRule, RULE_SEP);
		if(pRule == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get file path (%s)", pFileHdr->acDbName);
			nRetVal = -4;
			break;
		}
		if(pToken[0] != 0)
		{
			strncpy(pWhiteList->acFilePath, pToken, MAX_PATH-1);
			pWhiteList->acFilePath[MAX_PATH-1] = 0;
		}

		//Tag
		pRule = copy_token(pToken, pRule, RULE_SEP);
		if(pRule == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get tag (%s)", pFileHdr->acDbName);
			nRetVal = -5;
			break;
		}
		if(_stricmp(pToken, "NULL"))
		{
			strncpy(pWhiteList->acTagName, pToken, MAX_QQBUFF-1);
			pWhiteList->acTagName[MAX_QQBUFF-1] = 0;
		}

		//Product
		pRule = copy_token(pToken, pRule, RULE_SEP);
		if(pRule == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get product (%s)", pFileHdr->acDbName);
			nRetVal = -6;
			break;
		}

		//Company
		pRule = copy_token(pToken, pRule, RULE_SEP);
		if(pRule == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get company (%s)", pFileHdr->acDbName);
			nRetVal = -7;
			break;
		}

		//Desc
		pRule = copy_token(pToken, pRule, RULE_SEP);
		if(pRule == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get desc (%s)", pFileHdr->acDbName);
			nRetVal = -8;
			break;
		}

		//Sign Com
		pRule = copy_token(pToken, pRule, RULE_SEP);
		if(pRule == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get sign (%s)", pFileHdr->acDbName);
			nRetVal = -9;
			break;
		}

		//Category
		pRule = copy_token(pToken, pRule, RULE_SEP);
		if ((pRule == NULL) || (is_valid_num(pToken) == -1))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get category (%s)", pFileHdr->acDbName);
			nRetVal = -10;
			break;
		}
		pWhiteList->dwCategory = (DWORD)atoi(pToken);


		//File Size
		if (((pRule = copy_token(pToken, pRule, RULE_SEP)) == NULL) || (is_valid_num(pToken) == -1))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get file size (%s)", pFileHdr->acDbName);
			nRetVal = -11;
			break;
		}
		pWhiteList->dwFileSize = (DWORD)atoi(pToken);

		//File Masic
		if (((pRule = copy_token(pToken, pRule, RULE_SEP)) == NULL) || (is_valid_num(pToken) == -1))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get file magic (%s)", pFileHdr->acDbName);
			nRetVal = -12;
			break;
		}
		pWhiteList->dwMasic = (DWORD)atoi(pToken);

		//File Response
		if (((pRule = copy_token(pToken, pRule, RULE_SEP)) == NULL) || (is_valid_num(pToken) == -1))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get file response (%s)", pFileHdr->acDbName);
			nRetVal = -13;
			break;
		}
		pWhiteList->dwResponse = (DWORD)atoi(pToken);

		//File Role
		if (((pRule = copy_token(pToken, pRule, RULE_SEP)) == NULL) || (is_valid_num(pToken) == -1))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get file role (%s)", pFileHdr->acDbName);
			nRetVal = -14;
			break;
		}
		pWhiteList->dwRole = (DWORD)atoi(pToken);

		//File Create Time
		pRule = copy_token(pToken, pRule, RULE_SEP);
		if(pRule == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get create time (%s)", pFileHdr->acDbName);
			nRetVal = -15;
			break;
		}

		//File Access Time
		if (((pRule = copy_token(pToken, pRule, RULE_SEP)) == NULL) || (is_valid_num(pToken) == -1))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get file access time (%s)", pFileHdr->acDbName);
			nRetVal = -16;
			break;
		}
		pWhiteList->dwAccessTime = (DWORD)atoi(pToken);

		//File Write Time
		copy_token(pToken, pRule, RULE_SEP);
		if (is_valid_num(pToken) == -1)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_parse_list_v1] fail to get file write time (%s)", pFileHdr->acDbName);
			nRetVal = -17;
			break;
		}
		pWhiteList->dwWriteTime = (DWORD)atoi(pToken);
		//db name
		strncpy(pWhiteList->acDbName, pFileHdr->acDbName, MAX_QQBUFF-1);
		pWhiteList->acDbName[MAX_QQBUFF-1] = 0;
		nRetVal = 0;
	}while(FALSE);
	safe_free(pToken);
	return nRetVal;
}

INT32 CWhitePatternUtil::asi_load_list_from_file_v1(FILE *fp, PWHITE_FILE_HDR pFileHdr, PWHITE_PARSE_RULE pParseRule, INT32 *pnCount, char *acLogMsg)
{
	PASI_WENG_WL_EX pWhiteList = NULL;
	PCHAR pcBuffer = NULL;
	INT32 nRetVal = 0;
	INT32 nCount = 0;

	if(fp == NULL || pFileHdr == NULL || pnCount == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_load_list_from_file_v1] invalid input data");
		return -1;
	}

	pcBuffer = (PCHAR)malloc(MAX_BUFF);
	if(pcBuffer == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_load_list_from_file_v1] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pcBuffer, 0, MAX_BUFF);

	pWhiteList = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(pWhiteList == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_load_list_from_file_v1] fail to allocate memory (%d)", errno);
		safe_free(pcBuffer);
		return -3;
	}

	while(fgets_data_file_v1(pcBuffer, MAX_BUFF-1, fp) != NULL)
	{
		pcBuffer[MAX_BUFF-1] = 0;
		memset(pWhiteList, 0, sizeof(ASI_WENG_WL_EX));
		nRetVal = asi_parse_list_v1(pcBuffer, pFileHdr, pWhiteList, acLogMsg);
		if(nRetVal < 0)
		{
			break;
		}
		if(m_tHashUtil.CheckWhiteListWithRule(pWhiteList, pParseRule) == FALSE)
		{
			continue;
		}
		nRetVal = m_tHashUtil.AddHashNodeWithLock(pWhiteList, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}
		nCount++;
		nRetVal = 0;
	}
	if(nRetVal == 0)
	{
		*pnCount = nCount;
	}
	safe_free(pWhiteList);
	safe_free(pcBuffer);
	return nRetVal;
}


INT32 CWhitePatternUtil::asi_load_white_list(char *szEngineFile, PWHITE_PARSE_DATA pPasrseData, INT32 *pnCount, char *acLogMsg)
{
	INT32 nRetVal = 0;
	FILE *fp = NULL;
	char szTempFile[MAX_PATH] = {0,};
	PWHITE_FILE_HDR pFileHdr = NULL;
	PWHITE_PARSE_RULE pParseRule = NULL;
	INT32 nCount = 0;

	if(szEngineFile == NULL || szEngineFile[0] == 0 || pnCount == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_load_white_list] invalid input data");
		return -1;
	}

	pParseRule = (PWHITE_PARSE_RULE)malloc(sizeof(WHITE_PARSE_RULE));
	if(pParseRule == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_load_white_list] fail to allocate parse memory (%d)", errno);
		return -2;
	}
	memset(pParseRule, 0, sizeof(WHITE_PARSE_RULE));

	do {

		pFileHdr = (PWHITE_FILE_HDR)malloc(sizeof(WHITE_FILE_HDR));
		if(pFileHdr == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_load_white_list] fail to allocate memory (%d)", errno);
			nRetVal = -3;
			break;
		}
		memset(pFileHdr, 0, sizeof(WHITE_FILE_HDR));

		nRetVal = asi_get_parse_rule(pPasrseData, pParseRule, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}

		snprintf(szTempFile, MAX_PATH-1, "%s.tmp", szEngineFile);
		szTempFile[MAX_PATH-1] = 0;

		nRetVal = AriaDeCryptFile(ASI_ARIA, szEngineFile, szTempFile, acLogMsg);
		if(nRetVal < 0)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_load_white_list] %s", acLogMsg);
			nRetVal -= 20;
			break;
		}

		fp = fopen(szTempFile, "r");
		if(fp == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_load_white_list] fail to open file %s (%d) ", szTempFile, errno);
			nRetVal = -4;
			break;
		}
		memset(pFileHdr, 0, sizeof(WHITE_FILE_HDR));
		nRetVal = asi_get_wdb_hdr(fp, pFileHdr, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 30;
			break;
		}
		nRetVal = asi_load_list_from_file_v1(fp, pFileHdr, pParseRule, &nCount, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 40;
			break;
		}

		*pnCount = nCount;
		nRetVal = 0;
	}while(FALSE);

	if(fp != NULL)
	{
		fclose(fp);
	}

	if(is_file(szTempFile) == 0)
	{
		unlink(szTempFile);
	}
	safe_free(pFileHdr);

	if(NULL != pParseRule)
	{
		safe_free(pParseRule->pPathRule);
		safe_free(pParseRule->pTagRule);
		safe_free(pParseRule->pWdbRule);
		safe_free(pParseRule);
	}
	return nRetVal;
}

INT32 CWhitePatternUtil::asi_get_white_list(char *szPath, PASI_WENG_WL_EX pWhiteList, DWORD *pdwWhiteType, char *acLogMsg)
{

	if(NULL == pWhiteList || NULL == pdwWhiteType)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_white_list] invalid input data");
		return -1;
	}
	do{
		if(NULL != szPath)
		{
			if(get_signiture(szPath, pWhiteList, acLogMsg) < 0)
			{
				*pdwWhiteType = AS_INVALID_FILE;
				break;
			}
		}
		if(pWhiteList->acWhiteHash[0] == 0)
		{
			*pdwWhiteType = AS_INVALID_FILE;
			break;
		}
		if(m_tHashUtil.GetHashNodeWithLock(pWhiteList, acLogMsg) == FALSE)
		{
			*pdwWhiteType = AS_ELF_FILE;
			break;
		}
		*pdwWhiteType = AS_WHITE_FILE;
	}while(FALSE);

	return 0;
}


INT32 CWhitePatternUtil::asi_add_white_list(char *szPath, PASI_WENG_WL_EX pWhiteList, char *acLogMsg)
{
	INT32 nRetVal = 0;

	if(NULL == pWhiteList)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_add_white_list] invalid input data");
		return -1;
	}
	do{
		if(NULL != szPath)
		{
			nRetVal = get_signiture(szPath, pWhiteList, acLogMsg);
			if(nRetVal < 0)
			{
				nRetVal -= 10;
				break;
			}
		}

		if(0 == pWhiteList->acWhiteHash[0])
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_add_white_list] invalid white hash");
			nRetVal = -2;
			break;
		}

		nRetVal = m_tHashUtil.AddHashNodeWithLock(pWhiteList, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 20;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}


INT32 CWhitePatternUtil::asi_mod_white_list(PASI_WENG_WL_EX pWhiteList, char *acLogMsg)
{
	INT32 nRetVal = 0;

	if(NULL == pWhiteList)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_mod_white_list] invalid input data");
		return -1;
	}
	do{
		if(0 == pWhiteList->acWhiteHash[0])
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_mod_white_list] invalid white hash");
			nRetVal = -2;
			break;
		}

		nRetVal = m_tHashUtil.ModHashNodeWithLock(pWhiteList, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}

INT32 CWhitePatternUtil::asi_del_white_list(char *szPath, PASI_WENG_WL_EX pWhiteList, char *acLogMsg)
{
	INT32 nRetVal = 0;

	if(NULL == pWhiteList)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_del_white_list] invalid input data");
		return -1;
	}
	do{
		if(NULL != szPath)
		{
			nRetVal = get_signiture(szPath, pWhiteList, acLogMsg);
			if(nRetVal < 0)
			{
				nRetVal -= 10;
				break;
			}
		}

		if(0 == pWhiteList->acWhiteHash[0])
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_del_white_list] invalid white hash");
			nRetVal = -2;
			break;
		}

		nRetVal = m_tHashUtil.DelHashNodeWithLock(pWhiteList, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 20;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}


INT32 CWhitePatternUtil::asi_get_white_list_count(PWHITE_PARSE_DATA pPasrseData, INT32 *pnListCount, char *acLogMsg)
{
	INT32 nRetVal = 0;
	PWHITE_PARSE_RULE pParseRule = NULL;
	INT32 nListCount = 0;

	if(pPasrseData == NULL || pnListCount == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_white_list_count] invalid input data");
		return -1;
	}

	pParseRule = (PWHITE_PARSE_RULE)malloc(sizeof(WHITE_PARSE_RULE));
	if(NULL == pParseRule)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_white_list_count] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pParseRule, 0, sizeof(WHITE_PARSE_RULE));
	do{
		nRetVal = asi_get_parse_rule(pPasrseData, pParseRule, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}
		//전체 리스트 수 조회
		if(pParseRule->dwParseFlag == 0)
		{
			nRetVal = m_tHashUtil.GetHashNodeCountWithLock(&nListCount, acLogMsg);
			if(nRetVal < 0)
			{
				nRetVal -= 20;
				break;
			}
		}
		else
		{
			nRetVal = m_tHashUtil.GetHashNodeCountWithParseRule(pParseRule, &nListCount, acLogMsg);
			if(nRetVal < 0)
			{
				nRetVal -= 30;
				break;
			}
		}
		*pnListCount = nListCount;
		nRetVal = 0;
	}while(FALSE);

	if(NULL != pParseRule)
	{
		safe_free(pParseRule->pPathRule);
		safe_free(pParseRule->pTagRule);
		safe_free(pParseRule->pWdbRule);
		safe_free(pParseRule);
	}
	return nRetVal;
}


INT32 CWhitePatternUtil::asi_get_all_white_list(PWHITE_PARSE_DATA pPasrseData, INT32 nTotalCount, PASI_WENG_WL_EX pWhiteList, char *acLogMsg)
{
	INT32 nRetVal = 0;
	PWHITE_PARSE_RULE pParseRule = NULL;

	if(pPasrseData == NULL || nTotalCount < 1 || pWhiteList == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_all_white_list] invalid input data");
		return -1;
	}

	pParseRule = (PWHITE_PARSE_RULE)malloc(sizeof(WHITE_PARSE_RULE));
	if(NULL == pParseRule)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_all_white_list] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pParseRule, 0, sizeof(WHITE_PARSE_RULE));
	do{
		nRetVal = asi_get_parse_rule(pPasrseData, pParseRule, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}
		if(pParseRule->dwParseFlag == 0)
		{
			nRetVal = m_tHashUtil.GetAllHashNodeWithLock(pWhiteList, nTotalCount, acLogMsg);
			if(nRetVal < 0)
			{
				nRetVal -= 20;
				break;
			}
		}
		else
		{
			nRetVal = m_tHashUtil.GetAllHashNodeWithParseRule(pParseRule, pWhiteList, nTotalCount, acLogMsg);
			if(nRetVal < 0)
			{
				nRetVal -= 30;
				break;
			}
		}
		nRetVal = 0;
	}while(FALSE);

	if(NULL != pParseRule)
	{
		safe_free(pParseRule->pPathRule);
		safe_free(pParseRule->pTagRule);
		safe_free(pParseRule->pWdbRule);
		safe_free(pParseRule);
	}
	return nRetVal;
}

INT32 CWhitePatternUtil::asi_get_wdb_count(char *szEnginePath, INT32 *pnFileCount, char *acLogMsg)
{
	INT32 nFileCount = 0;
	INT32 nLen = 0;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;

	if(szEnginePath == NULL || szEnginePath[0] == 0 || pnFileCount == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_count] invalid input data");
		return -1;
	}

	if(DirectoryExists(szEnginePath) == FALSE)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_count] invalid path (%s)", szEnginePath);
		return -2;
	}

	dp = opendir(szEnginePath);
	if (dp == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_count] fail to open %s (%d)", szEnginePath, errno);
		return -3;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
			continue;

		if(DT_DIR == dirp->d_type)
			continue;
		nLen = (INT32)strlen(dirp->d_name);
		if(nLen < 4)
			continue;
		if(_stricmp(&dirp->d_name[nLen-4], ".wdb"))
			continue;
		nFileCount++;
	}
	closedir(dp);
	*pnFileCount = nFileCount;
	return 0;
}

INT32 CWhitePatternUtil::asi_get_all_wdb_hdr(char *szEnginePath, INT32 nTotalCount, PWHITE_FILE_HDR pWhiteHdr, char *acLogMsg)
{
	INT32 nFileCount = 0;
	INT32 nLen = 0;
	INT32 nRetVal = 0;
	FILE *fp = NULL;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;

	char szWePath[MAX_PATH] = {0,};
	char szTempPath[MAX_PATH] = {0,};

	if(szEnginePath == NULL || szEnginePath[0] == 0 || nTotalCount < 1 || pWhiteHdr == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_all_wdb_hdr] invalid input data");
		return -1;
	}

	if(DirectoryExists(szEnginePath) == FALSE)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_all_wdb_hdr] invalid path (%s)", szEnginePath);
		return -2;
	}

	dp = opendir(szEnginePath);
	if (dp == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_all_wdb_hdr] fail to open %s (%d)", szEnginePath, errno);
		return -3;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
			continue;

		if(DT_DIR == dirp->d_type)
			continue;
		nLen = (INT32)strlen(dirp->d_name);
		if(nLen < 4)
			continue;
		if(_stricmp(&dirp->d_name[nLen-4], ".wdb"))
			continue;

		if(nFileCount >= nTotalCount)
		{
			nRetVal = 0;
			break;
		}

		snprintf(szWePath, MAX_PATH-1, "%s/%s", szEnginePath, dirp->d_name);
		szWePath[MAX_PATH-1] = 0;
		snprintf(szTempPath, MAX_PATH-1, "%s/%s.tmp", szEnginePath, dirp->d_name);
		szTempPath[MAX_PATH-1] = 0;

		nRetVal = AriaDeCryptFile(ASI_ARIA, szWePath, szTempPath, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}

		fp = fopen(szTempPath, "r");
		if(fp == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_all_wdb_hdr] fail to open file %s (%d) ", szTempPath, errno);
			nRetVal = -4;
			break;
		}

		nRetVal = asi_get_wdb_hdr(fp, &pWhiteHdr[nFileCount], acLogMsg);
		if(nRetVal < 0)
		{
			fclose(fp);
			nRetVal -= 20;
			break;
		}
		fclose(fp);
		if(is_file(szTempPath) == 0)
		{
			unlink(szTempPath);
		}
		nRetVal = 0;
		nFileCount++;
	}
	closedir(dp);

	return nRetVal;
}

INT32 CWhitePatternUtil::asi_get_wdb_hdr(FILE *fp, PWHITE_FILE_HDR pFileHdr, char *acLogMsg)
{
	INT32 nRetVal = 0;
	INT32 nLen = 0;
	char *pRule = NULL;
	char *pBuffer = NULL;
	char *pToken = NULL;

	if(fp == NULL || pFileHdr == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] invalid input data");
		return -1;
	}

	do{
		pBuffer = (char *)malloc(MAX_BUFF);
		if(pBuffer == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] fail to allocate memory (%d)", errno);
			nRetVal = -2;
			break;
		}
		memset(pBuffer, 0, MAX_BUFF);

		pToken = (char *)malloc(MAX_BUFF);
		if(pToken == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] fail to allocate memory (%d)", errno);
			nRetVal = -3;
			break;
		}
		memset(pToken, 0, MAX_BUFF);

		if(fgets_data_file(pBuffer,MAX_BUFF-1,fp) == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] fail to get start tag (%d)", errno);
			nRetVal = -4;
			break;
		}
		if (_stricmp(pBuffer, STAG_ASWL))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] invalid start tag (%s)", pBuffer);
			nRetVal = -5;
			break;
		}
		
		if(fgets_data_file(pBuffer,MAX_BUFF-1,fp) == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] fail to get file header (%d)", errno);
			nRetVal = -6;
			break;
		}
		pRule = pBuffer;
		//DB Name
		pRule = copy_token(pToken, pRule, RULE_SEP);
		if(pRule == NULL || pToken[0] == 0)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] fail to get db name (%s)", pBuffer);
			nRetVal = -7;
			break;
		}
		strncpy(pFileHdr->acDbName, pToken, MAX_QQBUFF-1);
		pFileHdr->acDbName[MAX_QQBUFF-1] = 0;

		//File Category
		if (((pRule = copy_token(pToken, pRule, RULE_SEP)) == NULL) || (is_valid_num(pToken) == -1))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] fail to get file category (%s)", pBuffer);
			nRetVal = -8;
			break;
		}
		pFileHdr->dwCategory = (DWORD)atoi(pToken);

		//File Create Time
		if (((pRule = copy_token(pToken, pRule, RULE_SEP)) == NULL) || (is_valid_num(pToken) == -1))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] fail to get file create time (%s)", pBuffer);
			nRetVal = -9;
			break;
		}
		pFileHdr->dwCreateTime = (DWORD)atoi(pToken);

		//File Write Time
		if (((pRule = copy_token(pToken, pRule, RULE_SEP)) == NULL) || (is_valid_num(pToken) == -1))
		{
			if(acLogMsg)snprintf
				(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] fail to get file write time (%s)", pBuffer);
			nRetVal = -10;
			break;
		}
		pFileHdr->dwWriteTime = (DWORD)atoi(pToken);

		pFileHdr->dwFileVersionMS = 0;
		pFileHdr->dwSdkVersionLS = 0;

		if(fgets_data_file(pBuffer,MAX_BUFF-1,fp) == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] fail to get end tag (%d)", errno);
			nRetVal = -12;
			break;
		}
		if (_stricmp(pBuffer, ETAG_ASWL))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_wdb_hdr] invalid end tag (%s)", pBuffer);
			nRetVal = -13;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	safe_free(pBuffer);
	safe_free(pToken);
	return nRetVal;
}


INT32 CWhitePatternUtil::asi_get_parse_name(char *szData, PPARSE_RULE_NAME pParseName, INT32 nTotalCount)
{
	INT32 i=0;
	char *pRule = NULL;
	char szToken[MAX_QQBUFF] = {0,};
	if(szData == NULL || pParseName == NULL || nTotalCount < 1)
		return -1;

	pRule = szData;
	for(i=0; i<nTotalCount; i++)
	{
		pRule = copy_token(szToken, pRule, RULE_SEP);
		if(szToken[0] != 0)
		{
			strncpy(pParseName[i].acName, szToken, MAX_QQBUFF-1);
			pParseName[i].acName[MAX_QQBUFF-1] = 0;
		}
		else
		{
			break;
		}
		if(pRule == NULL)
		{
			break;
		}
	}
	return 0;
}


INT32 CWhitePatternUtil::asi_get_parse_path(char *szData, PPARSE_RULE_PATH pParsePath, INT32 nTotalCount)
{
	DWORD i=0;
	char *pRule = NULL;
	char szToken[MAX_QQBUFF] = {0,};
	if(szData == NULL || pParsePath == NULL || nTotalCount < 0)
		return -1;

	pRule = szData;
	for(i=0; i<nTotalCount; i++)
	{
		pRule = copy_token(szToken, pRule, RULE_SEP);
		if(szToken[0] != 0)
		{
			if(asi_get_wildstr_info(szToken, &pParsePath[i]) == FALSE)
			{
				break;
			}
		}
		else
		{
			break;
		}
		if(pRule == NULL)
		{
			break;
		}
	}
	return 0;
}

INT32 CWhitePatternUtil::asi_get_parse_rule(PWHITE_PARSE_DATA pPasrseData, PWHITE_PARSE_RULE pParseRule, char *acLogMsg)
{
	INT32 nCount = 0;
	INT32 nRetVal = 0;
	if(pPasrseData == NULL || pParseRule == NULL)
	{
		return 0;
	}
	if(pPasrseData->dwParseFlag == 0)
	{
		return 0;
	}
	pParseRule->dwParseFlag = pPasrseData->dwParseFlag;
	if(pPasrseData->dwParseFlag & AS_PARSE_CATEGORY)
	{
		pParseRule->dwCategory = pPasrseData->dwCategory;
	}
	if(pPasrseData->dwParseFlag & AS_PARSE_SYSTEM)
	{
		pParseRule->dwMasic = pPasrseData->dwMasic;
	}
	if(pPasrseData->dwParseFlag & AS_PARSE_TAG_NAME)
	{
		nCount = 0;
		nRetVal = count_rule_data(pPasrseData->acTagRule, RULE_SEP, &nCount);
		if(nRetVal == 0 && nCount > 1)
		{
			pParseRule->dwTagCount = (DWORD)nCount;
			pParseRule->pTagRule = (PPARSE_RULE_NAME)malloc(nCount * sizeof(PARSE_RULE_NAME));
			if(pParseRule->pTagRule == NULL)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_parse_rule] fail to allocate tag memory (%d)", errno);
				return -2;
			}
			memset(pParseRule->pTagRule, 0, nCount * sizeof(PARSE_RULE_NAME));
			nRetVal = asi_get_parse_name(pPasrseData->acTagRule, pParseRule->pTagRule, nCount);
			if(nRetVal < 0)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_parse_rule] fail to parse tag rule (cnt:%d) (ret:%d)", nCount, nRetVal);
				return -3;
			}
		}
	}
	//파일경로룰
	if(pPasrseData->dwParseFlag & AS_PARSE_FILE_PATH)
	{
		nCount = 0;
		nRetVal = count_rule_data(pPasrseData->acPathRule, RULE_SEP, &nCount);
		if(nRetVal == 0 && nCount > 1)
		{
			pParseRule->dwPathCount = (DWORD)nCount;
			pParseRule->pPathRule = (PPARSE_RULE_PATH)malloc(nCount * sizeof(PARSE_RULE_PATH));
			if(pParseRule->pPathRule == NULL)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_parse_rule] fail to allocate path memory (%d)", errno);
				return -4;
			}
			memset(pParseRule->pPathRule, 0, nCount * sizeof(PARSE_RULE_PATH));
			nRetVal = asi_get_parse_path(pPasrseData->acPathRule, pParseRule->pPathRule, nCount);
			if(nRetVal < 0)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_parse_rule] fail to parse path rule (cnt:%d) (ret:%d)", nCount, nRetVal);
				return -5;
			}
		}
	}
	if(pPasrseData->dwParseFlag & AS_PARSE_FILE_NAME)
	{
		nCount = 0;
		nRetVal = count_rule_data(pPasrseData->acWdbRule, RULE_SEP, &nCount);
		if(nRetVal == 0 && nCount > 1)
		{
			pParseRule->dwWdbCount = (DWORD)nCount;
			pParseRule->pWdbRule = (PPARSE_RULE_NAME)malloc(nCount * sizeof(PARSE_RULE_NAME));
			if(pParseRule->pWdbRule == NULL)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_parse_rule] fail to allocate wdb file memory (%d)", errno);
				return -6;
			}
			memset(pParseRule->pWdbRule, 0, nCount * sizeof(PARSE_RULE_NAME));
			nRetVal = asi_get_parse_name(pPasrseData->acWdbRule, pParseRule->pWdbRule, nCount);
			if(nRetVal < 0)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[asi_get_parse_rule] fail to parse wdb file rule (cnt:%d) (ret:%d)", nCount, nRetVal);
				return -7;
			}
		}
	}
	return 0;
}

INT32 CWhitePatternUtil::asi_make_wdb(char *szEngineFile, DWORD dwCategory, PWHITE_PARSE_DATA pPasrseData, INT32 *pnListCount, char *acLogMsg)
{
	FILE *fp = NULL;
	PWHITE_PARSE_RULE pParseRule = NULL;
	PWHITE_FILE_HDR pFileHdr = NULL;
	INT32 nListCount = 0;
	INT32 nRetVal = 0;
	PCHAR pTempFile = NULL;
	time_t tTime = 0;

	if(szEngineFile == NULL || szEngineFile[0] == 0 || pPasrseData == NULL || pnListCount == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[asi_make_wdb] invalid input data");
		return -1;
	}

	do{
		pTempFile = (PCHAR)malloc(MAX_PATH);
		if(NULL == pTempFile)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_make_wdb] fail to allocate temp path memory (%d)", errno);
			nRetVal = -2;
			break;
		}
		memset(pTempFile, 0, MAX_PATH);

		pFileHdr = (PWHITE_FILE_HDR)malloc(sizeof(WHITE_FILE_HDR));
		if(NULL == pFileHdr)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_make_wdb] fail to allocate header memory (%d)", errno);
			nRetVal = -3;
			break;
		}
		memset(pFileHdr, 0, sizeof(WHITE_FILE_HDR));

		pParseRule = (PWHITE_PARSE_RULE)malloc(sizeof(WHITE_PARSE_RULE));
		if(NULL == pParseRule)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_make_wdb] fail to allocate rule memory (%d)", errno);
			nRetVal = -4;
			break;
		}
		memset(pParseRule, 0, sizeof(WHITE_PARSE_RULE));

		nRetVal = m_tHashUtil.GetHashNodeCountWithLock(&nListCount, acLogMsg);
		if(nRetVal < 0 || nListCount < 1)
		{
			nRetVal -= 10;
			break;
		}

		nRetVal = asi_get_parse_rule(pPasrseData, pParseRule, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 20;
			break;
		}
		snprintf(pTempFile, MAX_PATH-1, "%s.tmp", szEngineFile);
		pTempFile[MAX_PATH-1] = 0;

		if(get_basename(szEngineFile, pFileHdr->acDbName, MAX_QQBUFF-1) == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[asi_make_wdb] fail to get base name (%s)", szEngineFile);
			nRetVal = -5;
			break;
		}
		pFileHdr->acDbName[MAX_QQBUFF-1] = 0;
		pFileHdr->dwFileVersionMS = 0;
		pFileHdr->dwFileVersionLS = 0;
		time(&tTime);
		pFileHdr->dwCreateTime = (DWORD)tTime;
		pFileHdr->dwWriteTime = (DWORD)tTime;
		pFileHdr->dwCategory = dwCategory;

		fp = fopen(pTempFile, "wt");
		if(fp == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[we_dbmake] fail to open file %s (%d) ", pTempFile, errno);
			nRetVal = -6;
			break;
		}
		nRetVal = m_tHashUtil.PrintFileHeader(fp, pFileHdr, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 30;
			fclose(fp);
			break;

		}
		nListCount = 0;
		nRetVal = m_tHashUtil.PrintAllHashNodeWithLock(fp, dwCategory, pParseRule, &nListCount, acLogMsg);
		if(nRetVal < 0 || nListCount < 1)
		{
			nRetVal -= 40;
			fclose(fp);
			break;
		}
		fclose(fp);

		nRetVal = AriaEnCryptFile(ASI_ARIA, pTempFile, szEngineFile, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 50;
			break;
		}
		*pnListCount = nListCount;
		nRetVal = 0;
	}while(FALSE);

	if(is_file(pTempFile) == 0)
	{
		unlink(pTempFile);
	}
	if(NULL != pParseRule)
	{
		safe_free(pParseRule->pPathRule);
		safe_free(pParseRule->pTagRule);
		safe_free(pParseRule->pWdbRule);
		safe_free(pParseRule);
	}
	safe_free(pTempFile);
	safe_free(pFileHdr);
	return nRetVal;
}


BOOL CWhitePatternUtil::asi_get_wildstr_info(char *szPath, PPARSE_RULE_PATH pWildInfo)
{
	char *pStart = NULL;
	char *pLast = NULL;
	int nLen = 0;
	char acDirPath[PATH_LENGTH] = {0,};
	if (szPath == NULL || pWildInfo == NULL)
	{
		return FALSE;
	}

	if (szPath[0] == '\0')
	{
		return FALSE;
	}

	strncpy(acDirPath, szPath, PATH_LENGTH-1);
	acDirPath[PATH_LENGTH-1] = 0;

	pLast = acDirPath + strlen(acDirPath) - 1;

	pStart = pLast + 1;

	while (pStart > acDirPath)
	{
		pStart--;

		if (*pStart == '\\')
		{
			*pStart = '\0';
			pStart++;
			break;
		}
	}

	if (pStart == pLast)
	{
		if (*pStart != '*')  /* '*' 형식인 경우 */
		{
			return FALSE;
		}
		pWildInfo->dwWildType = AS_WILD_ALL;
		strncpy(pWildInfo->acDirPath, acDirPath, PATH_LENGTH-1);
		pWildInfo->acDirPath[PATH_LENGTH-1];
	}
	else if (*pStart == '*')  /* '*data' 형식인 경우 */
	{
		pWildInfo->dwWildType = AS_WILD_PRE;
		strncpy(pWildInfo->acWildStr, pStart + 1, WILDCARD_LENGTH-1);
		pWildInfo->acWildStr[WILDCARD_LENGTH-1] = '\0';
		pWildInfo->dwWildLen = (DWORD)strlen(pWildInfo->acWildStr);

		strncpy(pWildInfo->acDirPath, acDirPath, PATH_LENGTH-1);
		pWildInfo->acDirPath[PATH_LENGTH-1];
	}
	else if (*pLast == '*')  /* 'data*' 형식인 경우 */
	{
		pWildInfo->dwWildType = AS_WILD_POST;
		strncpy(pWildInfo->acWildStr, pStart, WILDCARD_LENGTH-1);
		pWildInfo->acWildStr[WILDCARD_LENGTH-1] = '\0';
		nLen = (int)strlen(pWildInfo->acWildStr);
		if(nLen < 1)
		{
			return FALSE;
		}
		if (pWildInfo->acWildStr[nLen - 1] == '*')
			pWildInfo->acWildStr[nLen - 1] = '\0';
		pWildInfo->dwWildLen = (DWORD)strlen(pWildInfo->acWildStr);
		strncpy(pWildInfo->acDirPath, acDirPath, PATH_LENGTH-1);
		pWildInfo->acDirPath[PATH_LENGTH-1];
	}
	else
	{
		pWildInfo->dwWildType = AS_WILD_NONE;
		strncpy(pWildInfo->acDirPath, szPath, PATH_LENGTH-1);
		pWildInfo->acDirPath[PATH_LENGTH-1];
	}
	return TRUE;
}