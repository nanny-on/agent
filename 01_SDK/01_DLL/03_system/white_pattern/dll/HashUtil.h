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

#ifndef _HASH_UTIL_H__
#define _HASH_UTIL_H__

#define MAX_HASH_SIZE 0x100

#define STAG_ASWL	"[WLHDR]"
#define ETAG_ASWL	"[/WLHDR]"


typedef struct white_hash_node
{
	ASI_WENG_WL_EX stWhiteList;
	struct white_hash_node **pPrev;		
	struct white_hash_node *pNext;		
} WHITE_HASH_NODE, *PWHITE_HASH_NODE;

typedef struct white_hash_tbl
{
	INT32 nSize;
	INT32 nMask;
	INT32 nTotalCnt;
	INT32 nReserved;
	WHITE_HASH_NODE **ppBucket;
} WHITE_HASH_TABLE, *PWHITE_HASH_TABLE;

typedef struct reserv_path_list
{
	INT32 nSize;
	INT32 nReserved;
	char acFilePath[MAX_PATH];
	struct reserv_path_list *pNext;
} RESEVED_PATH_LIST, *PRESEVED_PATH_LIST;

class CHashUtil
{
private:
	PWHITE_HASH_NODE	*m_ppHashBucket;
	PWHITE_HASH_TABLE	m_pWhiteTbl;
	pthread_mutex_t		m_mutex;
	INT32 				m_nPathListCount;
	PRESEVED_PATH_LIST  m_pPathList;

public:
	INT32 AddReservedPathWithLock(char *acPath);
	INT32 GetReservedPathCountWithLock();
	INT32 GetReservedPathWithLock( PRESEVED_PATH_LIST pList, INT32 nPathListCount );
	VOID ClearReservedPathWithLock();

	INT32 InitHash(char *acLogMsg = NULL);
	VOID UninitHashWithLock();
	INT32 GetHashNodeCountWithLock(INT32 *pnCount, char *acLogMsg = NULL);
	INT32 GetHashNodeCountWithParseRule(PWHITE_PARSE_RULE pParseRule, INT32 *pnCount, char *acLogMsg = NULL);;
	INT32 GetAllHashNodeWithLock(PASI_WENG_WL_EX pWhiteList, INT32 nTotalCount, char *acLogMsg = NULL);
	INT32 GetAllHashNodeWithParseRule(PWHITE_PARSE_RULE pParseRule, PASI_WENG_WL_EX pWhiteList, INT32 nTotalCount, char *acLogMsg = NULL);
	BOOL GetHashNodeWithLock(PASI_WENG_WL_EX pWhiteList, char *acLogMsg = NULL);
	INT32 AddHashNodeWithLock(PASI_WENG_WL_EX pWhiteList, char *acLogMsg = NULL);
	INT32 ModHashNodeWithLock(PASI_WENG_WL_EX pWhiteList, char *acLogMsg = NULL);
	INT32 DelHashNodeWithLock(PASI_WENG_WL_EX pWhiteList, char *acLogMsg = NULL);
	VOID ClearHashNodeWithLock(VOID);
	BOOL CheckWhiteListWithRule(PASI_WENG_WL_EX pWhiteList, PWHITE_PARSE_RULE pParseRule);
	INT32 PrintFileHeader(FILE *fp, PWHITE_FILE_HDR pFileHdr, char *acLogMsg = NULL);
	INT32 PrintAllHashNodeWithLock(FILE *fp, DWORD dwCategory, PWHITE_PARSE_RULE pParseRule, INT32 *pnCount, char *acLogMsg = NULL);

private:
	BOOL get_hash(const char *pKey, const int nMask, int *pnHash);
	VOID remove_hash_node(PWHITE_HASH_NODE pRemNode);
	VOID clear_hash_node(PWHITE_HASH_NODE pStartNode);
	VOID clear_hash_table();
	INT32 add_node_to_table(PWHITE_HASH_TABLE pHashTable, PWHITE_HASH_NODE pNewNode, char *acLogMsg = NULL);
	PWHITE_HASH_NODE find_node_from_table(PWHITE_HASH_TABLE pHashTable, PASI_WENG_WL_EX pWhiteList);
	INT32 mod_node_to_table(PWHITE_HASH_TABLE pHashTable, PASI_WENG_WL_EX pWhiteList);

public:
	CHashUtil();
	~CHashUtil();
};

#endif /*_HASH_UTIL_H__*/