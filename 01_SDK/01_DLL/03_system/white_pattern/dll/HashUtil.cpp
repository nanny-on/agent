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
 */

#include "stdafx.h"
#include "com_struct.h"
#include "WhiteListDef.h"
#include "HashUtil.h"

CHashUtil::CHashUtil()
{
	m_ppHashBucket	= NULL;
	m_pWhiteTbl = NULL;
	m_pPathList	= NULL;
	m_nPathListCount = 0;
}
//---------------------------------------------------------------------------------

CHashUtil::~CHashUtil()
{
}
//---------------------------------------------------------------------------------

INT32 CHashUtil::AddReservedPathWithLock(char *acPath)
{
	PRESEVED_PATH_LIST pNewEntry = NULL;

	if(acPath == NULL || acPath[0] == 0)
	{
		return -1;
	}

	pNewEntry = (PRESEVED_PATH_LIST)malloc(sizeof(RESEVED_PATH_LIST));
	if(pNewEntry==NULL)
	{
		return -2;
	}
	memset(pNewEntry, 0, sizeof(RESEVED_PATH_LIST));
	
	strncpy(pNewEntry->acFilePath, acPath, MAX_PATH-1);
	pNewEntry->acFilePath[MAX_PATH-1] = 0;
	pNewEntry->nSize = (INT32)strlen(acPath);

	pthread_mutex_lock(&m_mutex);
	if(m_nPathListCount < 1)
		pNewEntry->pNext = NULL;
	else
		pNewEntry->pNext = m_pPathList;
	m_pPathList = pNewEntry;
	m_nPathListCount++;
	pthread_mutex_unlock(&m_mutex);
	
	return 0;
}

INT32 CHashUtil::GetReservedPathCountWithLock()
{
	INT32 nPathListCount = 0;
	pthread_mutex_lock(&m_mutex);
	nPathListCount = m_nPathListCount;
	pthread_mutex_unlock(&m_mutex);
	return nPathListCount;
}

INT32 CHashUtil::GetReservedPathWithLock( PRESEVED_PATH_LIST pList, INT32 nPathListCount )
{
	PRESEVED_PATH_LIST pHashEntry = NULL; 
	PRESEVED_PATH_LIST pNextEntry = NULL; 
	INT32 nCount = 0;
	
	if(pList == NULL || nPathListCount < 1)
		return -1;

	pthread_mutex_lock(&m_mutex);
	pHashEntry = m_pPathList;
	while( pHashEntry )
	{
		if(nCount >= nPathListCount)
			break;
		strncpy(pList[nCount].acFilePath, pHashEntry->acFilePath, MAX_PATH-1);
		pList[nCount].acFilePath[MAX_PATH-1] = 0;
		pList[nCount].nSize = pHashEntry->nSize;
		nCount++;
		pNextEntry = pHashEntry->pNext;
		pHashEntry = pNextEntry;
	}
	pthread_mutex_unlock(&m_mutex);
	return 0;
}

VOID CHashUtil::ClearReservedPathWithLock()
{
	PRESEVED_PATH_LIST pHashEntry = NULL; 
	PRESEVED_PATH_LIST pNextEntry = NULL; 
	
	pthread_mutex_lock(&m_mutex);
	pHashEntry = m_pPathList;
	while( pHashEntry ) 
	{
		pNextEntry = pHashEntry->pNext;
		safe_free(pHashEntry);
		pHashEntry = pNextEntry;
	}
	m_nPathListCount = 0;
	m_pPathList = NULL;
	pthread_mutex_unlock(&m_mutex);    
}

/*
*	hash() - Hash function returns a hash number for a given key.
*	tptr: Pointer to a hash table
*	key: The key to create a hash number for
*/
BOOL CHashUtil::get_hash(const char *pKey, const int nMask, int *pnHash)
{
	int nHashValue = 0;
	char acHashValue[8] = {0,};
	if(pKey == NULL || pKey[0] == 0 || nMask == 0 || pnHash == NULL)
	{
		return FALSE;
	}
	strncpy(acHashValue, pKey, 4);
	acHashValue[4] = 0;
	nHashValue = strtoul(acHashValue, NULL, 16);
	nHashValue = nHashValue % nMask;
	*pnHash = nHashValue;
	return TRUE;
}

/*
*  InitHash() - Initialize a new hash table.
*  tptr: Pointer to the hash table to initialize
*  buckets: The number of initial buckets to create
*/
INT32 CHashUtil::InitHash(char *acLogMsg)
{
	m_ppHashBucket = (PWHITE_HASH_NODE *)malloc(MAX_HASH_SIZE * sizeof(PWHITE_HASH_NODE));
	if(m_ppHashBucket == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[InitHash] fail to allocate bucket memory (%d)", errno);
		return -1;
	}
	memset(m_ppHashBucket, 0, MAX_HASH_SIZE * sizeof(PWHITE_HASH_NODE));

	m_pWhiteTbl = (PWHITE_HASH_TABLE)malloc(sizeof(WHITE_HASH_TABLE));
	if(m_pWhiteTbl == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[InitHash] fail to allocate table memory (%d)", errno);
		safe_free(m_ppHashBucket);
		return -2;
	}
	memset(m_pWhiteTbl, 0, sizeof(WHITE_HASH_TABLE));

	m_pWhiteTbl->ppBucket = m_ppHashBucket;
	m_pWhiteTbl->nSize = MAX_HASH_SIZE;
	m_pWhiteTbl->nMask = MAX_HASH_SIZE-1;
	m_pWhiteTbl->nTotalCnt = 0;

	pthread_mutex_init(&m_mutex, NULL);
	return 0;
}

VOID CHashUtil::remove_hash_node(PWHITE_HASH_NODE pRemNode)
{
	PWHITE_HASH_NODE pHashNode = NULL;

	if ( pRemNode != NULL )
	{
		pHashNode = pRemNode->pNext;
		if ( pHashNode )
			pHashNode->pPrev = pRemNode->pPrev;
		if ( pRemNode->pPrev )
			*pRemNode->pPrev = pHashNode;
		pRemNode->pNext = NULL;
		pRemNode->pPrev = NULL;
	}
}

VOID CHashUtil::clear_hash_node(PWHITE_HASH_NODE pStartNode)
{
	PWHITE_HASH_NODE pHashNode = NULL, pNextHashNode = NULL;

	if(pStartNode != NULL)
	{
		pHashNode = pStartNode;
		while ( pHashNode )
		{
			pNextHashNode = pHashNode->pNext;
			safe_free(pHashNode);
			pHashNode = pNextHashNode;
		}
	}
}

VOID CHashUtil::clear_hash_table()
{
	INT32 i;
	if(m_pWhiteTbl != NULL)
	{
		for (i=0; i<MAX_HASH_SIZE; i++)
		{
			if(m_pWhiteTbl->ppBucket[i] != NULL)
			{
				clear_hash_node(m_pWhiteTbl->ppBucket[i]);
				m_pWhiteTbl->ppBucket[i] = NULL;
			}
		}
		m_pWhiteTbl->nTotalCnt = 0;
	}
}

INT32 CHashUtil::add_node_to_table(PWHITE_HASH_TABLE pHashTable, PWHITE_HASH_NODE pNewNode, char *acLogMsg)
{
	PWHITE_HASH_NODE *ppHashNode, pFirstNode = NULL;
	INT32 nHashVal = 0;
	if (pHashTable == NULL || pNewNode == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[add_node_to_table] invalid input data");
		return -1;
	}

	if(get_hash(pNewNode->stWhiteList.acWhiteHash, pHashTable->nMask, &nHashVal) == FALSE)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[add_node_to_table] fail to get hash value (%s)", pNewNode->stWhiteList.acWhiteHash);
		return -2;
	}

	pFirstNode = pHashTable->ppBucket[nHashVal];
	while ( pFirstNode != NULL )
	{
		if (!_stricmp(pFirstNode->stWhiteList.acWhiteHash, pNewNode->stWhiteList.acWhiteHash))
		{
			return 1;
		}
		pFirstNode = pFirstNode->pNext;
	}

	ppHashNode = &pHashTable->ppBucket[nHashVal];

	pFirstNode = *ppHashNode;
	if ( pFirstNode )
		pFirstNode->pPrev = &pNewNode->pNext;

	pNewNode->pNext = pFirstNode;
	pNewNode->pPrev = ppHashNode;
	*ppHashNode = pNewNode;

	return 0;
}

PWHITE_HASH_NODE CHashUtil::find_node_from_table(PWHITE_HASH_TABLE pHashTable, PASI_WENG_WL_EX pWhiteList)
{
	PWHITE_HASH_NODE pNode = NULL;
	INT32 nHashVal = 0;

	if (pWhiteList == NULL || pHashTable == NULL)
	{
		return NULL;
	}

	if(get_hash(pWhiteList->acWhiteHash, pHashTable->nMask, &nHashVal) == FALSE)
	{
		return NULL;
	}

	pNode = pHashTable->ppBucket[nHashVal];

	while ( pNode != NULL )
	{
		if (!_stricmp(pNode->stWhiteList.acWhiteHash, pWhiteList->acWhiteHash))
			return pNode;
		pNode = pNode->pNext;
	}
	return NULL;
}

INT32 CHashUtil::mod_node_to_table(PWHITE_HASH_TABLE pHashTable, PASI_WENG_WL_EX pWhiteList)
{
	PWHITE_HASH_NODE pNode = NULL;
	INT32 nHashVal = 0;

	if (pWhiteList == NULL || pHashTable == NULL)
	{
		return -1;
	}

	if(get_hash(pWhiteList->acWhiteHash, pHashTable->nMask, &nHashVal) == FALSE)
	{
		return -2;
	}

	pNode = pHashTable->ppBucket[nHashVal];
	while ( pNode != NULL )
	{
		if (!_stricmp(pNode->stWhiteList.acWhiteHash, pWhiteList->acWhiteHash))
		{
			memcpy(&pNode->stWhiteList, pWhiteList, sizeof(ASI_WENG_WL_EX));
			return 0;
		}
		pNode = pNode->pNext;
	}
	return -3;
}


VOID CHashUtil::UninitHashWithLock()
{
	pthread_mutex_lock(&m_mutex);
	clear_hash_table();
	safe_free(m_ppHashBucket);
	safe_free(m_pWhiteTbl);
	pthread_mutex_unlock(&m_mutex);
	Sleep(10);
	pthread_mutex_destroy(&m_mutex);
}

INT32 CHashUtil::GetHashNodeCountWithLock(INT32 *pnCount, char *acLogMsg)
{
	if (pnCount == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetHashNodeCountWithLock] invalid input data");
		return -1;
	}
	*pnCount = 0;
	pthread_mutex_lock(&m_mutex);
	if(m_pWhiteTbl != NULL)
	{
		*pnCount = m_pWhiteTbl->nTotalCnt;
	}
	pthread_mutex_unlock(&m_mutex);

	return 0;
}

INT32 CHashUtil::GetHashNodeCountWithParseRule(PWHITE_PARSE_RULE pParseRule, INT32 *pnCount, char *acLogMsg)
{
	INT32 i;
	INT32 nCount = 0;
	PWHITE_HASH_NODE pHashNode = NULL;
	PWHITE_HASH_NODE pNextNode = NULL;

	if (pParseRule == NULL || pnCount == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetHashNodeCountWithParseRule] invalid input data");
		return -1;
	}

	pthread_mutex_lock(&m_mutex);
	if(m_pWhiteTbl != NULL)
	{
		for (i=0; i<MAX_HASH_SIZE; i++)
		{
			pHashNode = m_pWhiteTbl->ppBucket[i];
			while ( pHashNode != NULL )
			{
				pNextNode = pHashNode->pNext;
				if(CheckWhiteListWithRule(&pHashNode->stWhiteList, pParseRule) == TRUE)
				{
					nCount++;
				}
				pHashNode = pNextNode;
			}
		}
	}
	pthread_mutex_unlock(&m_mutex);
	*pnCount = nCount;
	return 0;
}


INT32 CHashUtil::GetAllHashNodeWithParseRule(PWHITE_PARSE_RULE pParseRule, PASI_WENG_WL_EX pWhiteList, INT32 nTotalCount, char *acLogMsg)
{
	INT32 i, nCount = 0;
	PWHITE_HASH_NODE pHashNode = NULL;
	PWHITE_HASH_NODE pNextNode = NULL;

	if (pWhiteList == NULL || nTotalCount < 1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetAllHashNodeWithLock] invalid input data");
		return -1;
	}

	pthread_mutex_lock(&m_mutex);
	if(m_pWhiteTbl == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetAllHashNodeWithLock] invalid white table");
		pthread_mutex_unlock(&m_mutex);
		return -2;
	}

	for (i=0; i<MAX_HASH_SIZE; i++)
	{
		pHashNode = m_pWhiteTbl->ppBucket[i];
		while ( pHashNode != NULL )
		{
			pNextNode = pHashNode->pNext;
			if(CheckWhiteListWithRule(&pHashNode->stWhiteList, pParseRule) == TRUE)
			{
				memcpy(&pWhiteList[nCount], &pHashNode->stWhiteList, sizeof(ASI_WENG_WL_EX));
				nCount++;
				if(nCount >= nTotalCount)
					break;
			}
			pHashNode = pNextNode;
		}
		if(nCount >= nTotalCount)
			break;
	}
	pthread_mutex_unlock(&m_mutex);

	return 0;
}

INT32 CHashUtil::GetAllHashNodeWithLock(PASI_WENG_WL_EX pWhiteList, INT32 nTotalCount, char *acLogMsg)
{
	INT32 i, nCount = 0;
	PWHITE_HASH_NODE pHashNode = NULL;
	PWHITE_HASH_NODE pNextNode = NULL;

	if (pWhiteList == NULL || nTotalCount < 1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetAllHashNodeWithLock] invalid input data");
		return -1;
	}

	pthread_mutex_lock(&m_mutex);
	if(m_pWhiteTbl != NULL)
	{
		for (i=0; i<MAX_HASH_SIZE; i++)
		{
			pHashNode = m_pWhiteTbl->ppBucket[i];
			while ( pHashNode != NULL )
			{
				pNextNode = pHashNode->pNext;

				memcpy(&pWhiteList[nCount], &pHashNode->stWhiteList, sizeof(ASI_WENG_WL_EX));

				pHashNode = pNextNode;
				nCount++;
				if(nCount >= nTotalCount)
					break;
			}
			if(nCount >= nTotalCount)
				break;
		}
	}
	pthread_mutex_unlock(&m_mutex);

	return 0;
}

BOOL CHashUtil::GetHashNodeWithLock(PASI_WENG_WL_EX pWhiteList, char *acLogMsg)
{
	BOOL bRetVal = TRUE;
	PWHITE_HASH_NODE pFoundHashNode = NULL;

	if (pWhiteList == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetHashNodeWithLock] invalid input data");
		return FALSE;
	}
	
	pthread_mutex_lock(&m_mutex);
	do{
		if(m_pWhiteTbl == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[GetHashNodeWithLock] invalid white table memory");
			bRetVal = FALSE;
			break;
		}
		pFoundHashNode = find_node_from_table(m_pWhiteTbl, pWhiteList);
		if(pFoundHashNode == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[GetHashNodeWithLock] fail to find hash node (%s)", pWhiteList->acWhiteHash);
			bRetVal = FALSE;
			break;
		}
		memcpy(pWhiteList, &pFoundHashNode->stWhiteList, sizeof(ASI_WENG_WL_EX));
		bRetVal = TRUE;
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);
	
	return bRetVal;
}

INT32 CHashUtil::AddHashNodeWithLock(PASI_WENG_WL_EX pWhiteList, char *acLogMsg)
{
	INT32 nRetVal = 0;
	PWHITE_HASH_NODE pNewHashNode = NULL;

	if (pWhiteList == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AddHashNodeWithLock] invalid input data");
		return -1;
	}

	if(pWhiteList->dwCategory == 0)
	{
		pWhiteList->dwCategory = AS_LOCAL_WHITE;
	}

	pNewHashNode = (PWHITE_HASH_NODE)malloc(sizeof(WHITE_HASH_NODE));
	if (pNewHashNode == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AddHashNodeWithLock] fail to allocate memory (%d)", errno);
		return -2;
	}
	
	memcpy(&pNewHashNode->stWhiteList, pWhiteList, sizeof(ASI_WENG_WL_EX));
	pNewHashNode->pNext = NULL;
	pNewHashNode->pPrev = NULL;
	
	pthread_mutex_lock(&m_mutex);
	do{
		if(m_pWhiteTbl == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AddHashNodeWithLock] invalid table address");
			nRetVal = -3;
			safe_free(pNewHashNode);
			break;
		}
		nRetVal = add_node_to_table(m_pWhiteTbl, pNewHashNode, acLogMsg);
		if(nRetVal != 0)
		{
			if(nRetVal < 0)
				nRetVal -= 10;
			else
				nRetVal = 0;
			safe_free(pNewHashNode);
			break;
		}
		m_pWhiteTbl->nTotalCnt++;
		nRetVal = 0;
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);

	return nRetVal;
}

INT32 CHashUtil::ModHashNodeWithLock(PASI_WENG_WL_EX pWhiteList, char *acLogMsg)
{
	INT32 nRetVal = 0;
	PWHITE_HASH_NODE pFoundHashNode = NULL;

	if (pWhiteList == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ModHashNodeWithLock] invalid input data");
		return -1;
	}

	pthread_mutex_lock(&m_mutex);
	do{
		if(m_pWhiteTbl == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[ModHashNodeWithLock] invalid table address");
			nRetVal = -2;
			break;
		}
		nRetVal = mod_node_to_table(m_pWhiteTbl, pWhiteList);
		if(nRetVal < 0)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[ModHashNodeWithLock] fail to mod table (%d)", nRetVal);
			nRetVal -= 10;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);

	return nRetVal;
}

INT32 CHashUtil::DelHashNodeWithLock(PASI_WENG_WL_EX pWhiteList, char *acLogMsg)
{
	INT32 nRetVal = 0;
	PWHITE_HASH_NODE pFoundHashNode = NULL;

	if (pWhiteList == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[DelHashNodeWithLock] invalid input data");
		return -1;
	}

	pthread_mutex_lock(&m_mutex);
	do{
		if(m_pWhiteTbl == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[DelHashNodeWithLock] invalid table address");
			nRetVal = -2;
			break;
		}
		pFoundHashNode = find_node_from_table(m_pWhiteTbl, pWhiteList);
		if(pFoundHashNode == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[DelHashNodeWithLock] fail to find hash node (%s)", pWhiteList->acWhiteHash);
			nRetVal = -3;
			break;
		}
		remove_hash_node(pFoundHashNode);
		memcpy(pWhiteList, &pFoundHashNode->stWhiteList, sizeof(ASI_WENG_WL_EX));
		safe_free(pFoundHashNode);
		if(m_pWhiteTbl->nTotalCnt > 0)
			m_pWhiteTbl->nTotalCnt--;
		nRetVal = 0;
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);

	return nRetVal;
}

VOID CHashUtil::ClearHashNodeWithLock()
{
	pthread_mutex_lock(&m_mutex);
	clear_hash_table();
	pthread_mutex_unlock(&m_mutex);
}


BOOL CHashUtil::CheckWhiteListWithRule(PASI_WENG_WL_EX pWhiteList, PWHITE_PARSE_RULE pParseRule)
{
	DWORD i = 0;
	DWORD dwPathLen = 0;
	char *ptr = NULL;
	char acFileName[MAX_QQBUFF] = {0,};
	//룰이 없는 경우 무조건 출력
	if(pWhiteList == NULL || pParseRule == NULL)
		return TRUE;
	//검사 Category에서 검사할 항목이 없는 경우 무조건 출력
	if(pParseRule->dwParseFlag == 0)
		return TRUE;

	//리스트범주
	if(pParseRule->dwParseFlag & AS_PARSE_CATEGORY)
	{
		if(pParseRule->dwCategory == pWhiteList->dwCategory)
		{
			return TRUE;
		}
	}
	//시스템 구분
	if(pParseRule->dwParseFlag & AS_PARSE_SYSTEM)
	{
		if(pParseRule->dwMasic == pWhiteList->dwMasic)
		{
			return TRUE;
		}
	}
	//패턴 태그명
	if(pParseRule->dwParseFlag & AS_PARSE_TAG_NAME)
	{
		for(i=0;i<pParseRule->dwTagCount; i++)
		{
			if(pParseRule->pTagRule[i].acName[0] != 0)
			{
				if(!_stricmp(pParseRule->pTagRule[i].acName, pWhiteList->acTagName))
				{
					return TRUE;
				}
			}
		}
	}
	//파일경로룰
	if(pParseRule->dwParseFlag & AS_PARSE_FILE_PATH)
	{
		for(i=0;i<pParseRule->dwPathCount; i++)
		{
			dwPathLen = (DWORD)strlen(pParseRule->pPathRule[i].acDirPath);
			if(0 == dwPathLen)
				continue;
			if(pParseRule->pPathRule[i].dwWildType == AS_WILD_ALL)
			{
				if (!_strnicmp(pWhiteList->acFilePath, pParseRule->pPathRule[i].acDirPath, dwPathLen))
				{
					return TRUE;
				}
			}
			else if(pParseRule->pPathRule[i].dwWildType == AS_WILD_PRE)
			{
				if (!_strnicmp(pWhiteList->acFilePath, pParseRule->pPathRule[i].acDirPath, dwPathLen))
				{
					if(get_basename(pWhiteList->acFilePath, acFileName, MAX_QQBUFF-1) != NULL)
					{
						dwPathLen = (DWORD)strlen(acFileName);
						if(dwPathLen > pParseRule->pPathRule[i].dwWildLen)
						{
							ptr = acFileName + dwPathLen - pParseRule->pPathRule[i].dwWildLen;
							if (!_stricmp(ptr, pParseRule->pPathRule[i].acWildStr))
							{
								return TRUE;
							}
						}
					}
				}
			}
			else if(pParseRule->pPathRule[i].dwWildType == AS_WILD_POST)
			{
				if (!_strnicmp(pWhiteList->acFilePath, pParseRule->pPathRule[i].acDirPath, dwPathLen))
				{
					if(get_basename(pWhiteList->acFilePath, acFileName, MAX_QQBUFF-1) != NULL)
					{
						if (!_strnicmp(acFileName, pParseRule->pPathRule[i].acWildStr, pParseRule->pPathRule[i].dwWildLen))
						{
							return TRUE;
						}
					}
				}
			}
			else
			{
				if(!_stricmp(pParseRule->pPathRule[i].acDirPath, pWhiteList->acFilePath))
				{
					return TRUE;
				}
			}
		}
	}
	//패턴파일명
	if(pParseRule->dwParseFlag & AS_PARSE_FILE_NAME)
	{
		for(i=0;i<pParseRule->dwWdbCount; i++)
		{
			if(pParseRule->pWdbRule[i].acName[0] != 0)
			{
				if(!_stricmp(pParseRule->pWdbRule[i].acName, pWhiteList->acDbName))
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

INT32 CHashUtil::PrintFileHeader(FILE *fp, PWHITE_FILE_HDR pFileHdr, char *acLogMsg)
{
	if (fp == NULL || pFileHdr == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[PrintFileHeader] invalid input data");
		return -1;
	}
	fprintf(fp, "%s\n", STAG_ASWL);
	//db name | category | create time | write time | major version | minor version
	fprintf(fp, FMT_FILE_HEADER, pFileHdr->acDbName, RULE_SEP, pFileHdr->dwCategory, RULE_SEP, pFileHdr->dwCreateTime, RULE_SEP,
		pFileHdr->dwWriteTime, RULE_SEP, pFileHdr->dwFileVersionMS, RULE_SEP, pFileHdr->dwFileVersionLS);
	fprintf(fp, "%s\n", ETAG_ASWL);
	return 0;
}

INT32 CHashUtil::PrintAllHashNodeWithLock(FILE *fp, DWORD dwCategory, PWHITE_PARSE_RULE pParseRule, INT32 *pnCount, char *acLogMsg)
{
	INT32 i, nNode = 0;
	INT32 nLength = 0;
	INT32 nCount = 0;
	PWHITE_HASH_NODE pHashNode = NULL;
	PWHITE_HASH_NODE pNextNode = NULL;
	PCHAR pcWhiteInfo = NULL;
	if (fp == NULL || pnCount == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[PrintAllHashNodeWithLock] invalid input data");
		return -1;
	}

	*pnCount = 0;
	pthread_mutex_lock(&m_mutex);
	if(m_pWhiteTbl == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[PrintAllHashNodeWithLock] invalid table address");
		pthread_mutex_unlock(&m_mutex);
		return -2;
	}
	if(m_pWhiteTbl->nTotalCnt < 1)
	{
		pthread_mutex_unlock(&m_mutex);
		return 0;
	}

	pcWhiteInfo = (PCHAR)malloc(MAX_BUFF);
	if(pcWhiteInfo == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[PrintAllHashNodeWithLock] fail to allocate memory (%d)", errno);
		pthread_mutex_unlock(&m_mutex);
		return -3;
	}
	memset(pcWhiteInfo, 0, MAX_BUFF);

	for (i=0; i<MAX_HASH_SIZE; i++)
	{
		pHashNode = m_pWhiteTbl->ppBucket[i];
		while ( pHashNode != NULL )
		{
			pNextNode = pHashNode->pNext;
			if(pHashNode->stWhiteList.acWhiteHash[0] == 0)
			{
				pHashNode = pNextNode;
				continue;
			}
			if(CheckWhiteListWithRule(&pHashNode->stWhiteList, pParseRule) == TRUE)
			{
				//hash | path | tag | product | company | desc | sign company | category | size | magic | response | role | create time | access time | write time | version high | version low
				snprintf(pcWhiteInfo, MAX_BUFF-1, FMT_WHITE_LIST, pHashNode->stWhiteList.acWhiteHash, RULE_SEP, pHashNode->stWhiteList.acFilePath, RULE_SEP, pHashNode->stWhiteList.acTagName[0]?pHashNode->stWhiteList.acTagName:"NULL", RULE_SEP, 
					"NULL", RULE_SEP, "NULL", RULE_SEP, "NULL", RULE_SEP, "NULL", RULE_SEP,
					dwCategory, RULE_SEP, pHashNode->stWhiteList.dwFileSize, RULE_SEP, pHashNode->stWhiteList.dwMasic, RULE_SEP, pHashNode->stWhiteList.dwResponse, RULE_SEP, 
					pHashNode->stWhiteList.dwRole, RULE_SEP, 0, RULE_SEP, pHashNode->stWhiteList.dwAccessTime, RULE_SEP, pHashNode->stWhiteList.dwWriteTime, RULE_SEP, 
					0, RULE_SEP, 0);
				pcWhiteInfo[MAX_BUFF-1] = 0;
				nLength = (INT32)strlen(pcWhiteInfo);
				fprintf(fp, FMT_WHITE_LIST_HDR, nLength, RULE_SEP, pcWhiteInfo);
				nCount++;
			}
			pHashNode = pNextNode;
			Sleep(1);
		}
	}
	pthread_mutex_unlock(&m_mutex);
	safe_free(pcWhiteInfo);
	*pnCount = nCount;
	return 0;
}