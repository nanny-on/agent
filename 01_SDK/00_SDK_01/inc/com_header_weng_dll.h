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

#ifndef     COM_HEADER_WENG_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define     COM_HEADER_WENG_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//------------------------------------------------------------------------------

#define		HASH_PADDING_LEN	8
//#define		SHA512_BLOCK_SIZE	128
//------------------------------------------------------------------------------

#define		AS_WHITE_FILE				0x0000
#define		AS_ELF_FILE					0x0001
#define		AS_INVALID_FILE				0x0004
//------------------------------------------------------------------------------

#define AS_PARSE_FILE_NAME	0x80	//패턴파일명
#define AS_PARSE_FILE_PATH	0x08	//파일경로룰
#define AS_PARSE_TAG_NAME	0x04	//패턴 태그명
#define AS_PARSE_SYSTEM		0x02	//시스템 구분
#define AS_PARSE_CATEGORY	0x01	//리스트범주

#define AS_WILD_NONE	0  /* wildcard 사용 안함 */
#define AS_WILD_ALL		1  /* '*' 형식 */
#define AS_WILD_PRE		2  /* '*data' 형식 */
#define AS_WILD_POST	3  /* 'data*' 형식 */

#define AS_LOCAL_WHITE	1
#define AS_GLOBAL_WHITE	2
#define AS_GLOBAL_BLACK	4

#define	ASI_ARIA			"asi!aria*asi"

typedef struct _WHITE_PARSE_DATA
{
	DWORD dwParseFlag;
	DWORD dwCategory;
	DWORD dwMasic;
	char acTagRule[MAX_QBUFF];
	char acProductRule[MAX_QBUFF];
	char acCompanyRule[MAX_QBUFF];
	char acSignCompanyRule[MAX_QBUFF];
	char acWdbRule[MAX_QBUFF];
	char acPathRule[MAX_HBUFF];
} WHITE_PARSE_DATA, *PWHITE_PARSE_DATA;

typedef struct _PARSE_RULE_NAME
{
	char acName[MAX_QQBUFF];
} PARSE_RULE_NAME, *PPARSE_RULE_NAME;

typedef struct _PARSE_RULE_PATH {
	DWORD dwWildType;				/* WILDTYPE_xxx */
	DWORD dwWildLen;				/* WILDCARD 사용 문자열 길이 */
	char acWildStr[MAX_TYPE_LEN];	/* wildcard 사용 문자열 (wildcard 제외) */
	char acDirPath[MAX_HBUFF];	
} PARSE_RULE_PATH, *PPARSE_RULE_PATH;

typedef struct _WHITE_PARSE_RULE
{
	DWORD dwParseFlag;
	DWORD dwCategory;
	DWORD dwMasic;
	DWORD dwTagCount;
	DWORD dwProductCount;
	DWORD dwComapanyCount;
	DWORD dwSignCount;
	DWORD dwWdbCount;
	DWORD dwPathCount;
	PPARSE_RULE_NAME pTagRule;
	PPARSE_RULE_NAME pProductRule;
	PPARSE_RULE_NAME pCompanyRule;
	PPARSE_RULE_NAME pSignRule;
	PPARSE_RULE_NAME pWdbRule;
	PPARSE_RULE_PATH pPathRule;
} WHITE_PARSE_RULE, *PWHITE_PARSE_RULE;

typedef struct _WHITE_FILE_HDR
{
	DWORD dwCategory;				//4
	DWORD dwCreateTime;				//8
	DWORD dwWriteTime;				//12
	DWORD dwFileVersionLS;			//16
	DWORD dwFileVersionMS;			//20
	DWORD dwSdkVersionLS;			//24
	DWORD dwSdkVersionMS;			//28
	DWORD dwReserved;				//32
	char acDbName[MAX_QQBUFF];		//96
} WHITE_FILE_HDR, *PWHITE_FILE_HDR;

typedef struct _asi_weng_wl_ex
{
	DWORD dwCategory;				//4
	DWORD dwFileSize;				//8
	DWORD dwMasic;					//12
	DWORD dwResponse;				//16
	DWORD dwRole;					//20
	DWORD dwCreateTime;				//24
	DWORD dwAccessTime;				//28
	DWORD dwWriteTime;				//32
	DWORD dwFileVersionLS;
	DWORD dwFileVersionMS;
	char acTagName[MAX_QQBUFF];		//104
	char acProductName[MAX_QQBUFF];	//168
	char acCompanyName[MAX_QQBUFF];	//232
	char acDescName[MAX_QQBUFF];	//296
	char acSignCompanyName[MAX_QQBUFF];	//360
	char acWhiteHash[SHA512_BLOCK_SIZE+HASH_PADDING_LEN];	//496
	char acFilePath[MAX_PATH];	//756
	char acDbName[MAX_QQBUFF];	//820
} ASI_WENG_WL_EX, *PASI_WENG_WL_EX;

typedef list<ASI_WENG_WL_EX>				TListWEngWLEx;
typedef TListWEngWLEx::iterator				TListWEngWLExItor;
typedef TListWEngWLEx::reverse_iterator		TListWEngWLExRItor;

typedef map<String, ASI_WENG_WL_EX>			TMapWEngWLEx;
typedef TMapWEngWLEx::iterator				TMapWEngWLExItor;
typedef TMapWEngWLEx::reverse_iterator		TMapWEngWLExRItor;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

typedef	INT32	(*ASIWENG_InitType)(PVOID* lpObject);
typedef	INT32	(*ASIWENG_FreeType)(PVOID lpObject);
typedef	INT32	(*ASIWENG_InitDBType)(PVOID lpObject, char *acLogMsg);
typedef	INT32	(*ASIWENG_LoadDBType)(PVOID lpObject, char *acEnginePath, char *acLogMsg);
typedef	INT32	(*ASIWENG_CreateDBType)(PVOID lpObject, char *acWhitePath, char *acEnginePath, char *acDbName, char *acLogMsg);
typedef	INT32	(*ASIWENG_AddFileType)(PVOID lpObject, char *szPath, char *acLogMsg);
typedef	INT32	(*ASIWENG_DelFileType)(PVOID lpObject, char *szPath, char *acLogMsg);
typedef	INT32	(*ASIWENG_ClearFileType)(PVOID lpObject);
typedef	INT32	(*ASIWENG_MakeDBType)(PVOID lpObject, char *acEnginePath, char *acDbName, char *acLogMsg);
typedef	INT32	(*ASIWENG_UnInitDBType)(PVOID lpObject);
typedef	INT32	(*ASIWENG_AddWLType)(PVOID lpObject, char *szPath, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg);
typedef	INT32	(*ASIWENG_ModWLType)(PVOID lpObject, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg);
typedef	INT32	(*ASIWENG_DelWLType)(PVOID lpObject, char *szPath,  PVOID pWhiteList, DWORD dwListSize, char *acLogMsg);
typedef	INT32	(*ASIWENG_GetWLType)(PVOID lpObject, char *szPath, PVOID pWhiteList, DWORD dwListSize, PDWORD pdwWFileType, char *acLogMsg);
typedef	INT32	(*ASIWENG_GetWLCntType)(PVOID lpObject, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg);
typedef	INT32	(*ASIWENG_GetAllWLType)(PVOID lpObject, PVOID pPasrseData, DWORD dwDataSize, INT32 nTotalCount, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg);
typedef	INT32	(*ASIWENG_GetWLDBCntType)(PVOID lpObject, char *szEnginePath, INT32 *pnFileCount, char *acLogMsg);
typedef	INT32	(*ASIWENG_GetAllWLDBHdrType)(PVOID lpObject, char *szEnginePath, INT32 nFileCount, PVOID pWhiteHdr, DWORD dwHdrSize, char *acLogMsg);
typedef	INT32	(*ASIWENG_GetWLDBHdrType)(PVOID lpObject, char *acEngineFile, PVOID pWhiteHdr, DWORD dwHdrSize, char *acLogMsg);
typedef	INT32	(*ASIWENG_MakeWLDBType)(PVOID lpObject, char *szEngineFile, DWORD dwCategory, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg);
typedef	INT32	(*ASIWENG_LoadWLType)(PVOID lpObject, char *szEngineFile, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg);

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


#endif      //COM_HEADER_WENG_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
