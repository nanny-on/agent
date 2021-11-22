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

#ifndef _WHITE_PATTERN_UTIL__
#define _WHITE_PATTERN_UTIL__

#include "WhiteListDef.h"
#include "HashUtil.h"

#define MAX_RESERVED_DEPTH 7

class CWhitePatternUtil
{
private:
	INT32 asi_load_list_from_file_v1(FILE *fp, PWHITE_FILE_HDR pFileHdr, PWHITE_PARSE_RULE pParseRule, INT32 *pnCount, char *acLogMsg = NULL);
	INT32 asi_parse_list_v1(char *pBuffer, PWHITE_FILE_HDR pFileHdr, PASI_WENG_WL_EX pWhiteList, char *acLogMsg = NULL);

public:
	CHashUtil m_tHashUtil;

public:
	INT32 we_pattern_load(char *acEnginePath, char *acLogMsg = NULL);
	INT32 we_pattern_create(char *acWhitePath, char *acEnginePath, char *acDbName, char *acLogMsg = NULL);
	INT32 scan_white_path(FILE *fp, char *acPath, DWORD dwDepth, BOOL bIsSaveReserved, char *acLogMsg = NULL);
	INT32 get_signiture(char *szPath, PASI_WENG_WL_EX pWhiteList, char *acLogMsg = NULL);
	DWORD we_pkload(char *acEnginePath, char *acLogMsg = NULL);
	INT32 we_fadd(char *szPath, char *acLogMsg = NULL);
	INT32 we_fdel(char *szPath, char *acLogMsg = NULL);
	void we_fclear();
	INT32 we_dbmake(char *acEnginePath, char *acDbName, char *acLogMsg = NULL);
	INT32 asi_get_white_list(char *szPath, PASI_WENG_WL_EX pWhiteList, DWORD *pdwWhiteType, char *acLogMsg = NULL);
	INT32 asi_load_white_list(char *szEngineFile, PWHITE_PARSE_DATA pPasrseData, INT32 *pnCount, char *acLogMsg = NULL);
	INT32 asi_add_white_list(char *szPath, PASI_WENG_WL_EX pWhiteList, char *acLogMsg = NULL);
	INT32 asi_mod_white_list(PASI_WENG_WL_EX pWhiteList, char *acLogMsg = NULL);
	INT32 asi_del_white_list(char *szPath,  PASI_WENG_WL_EX pWhiteList, char *acLogMsg = NULL);
	INT32 asi_get_white_list_count(PWHITE_PARSE_DATA pPasrseData, INT32 *pnListCount, char *acLogMsg = NULL);
	INT32 asi_get_all_white_list(PWHITE_PARSE_DATA pPasrseData, INT32 nTotalCount, PASI_WENG_WL_EX pWhiteList, char *acLogMsg = NULL);
	INT32 asi_get_wdb_count(char *szEnginePath, INT32 *pnFileCount, char *acLogMsg = NULL);
	INT32 asi_get_all_wdb_hdr(char *szEnginePath, INT32 nTotalCount, PWHITE_FILE_HDR pWhiteHdr, char *acLogMsg = NULL);
	INT32 asi_get_wdb_hdr(FILE *fp, PWHITE_FILE_HDR pFileHdr, char *acLogMsg = NULL);
	INT32 asi_convert_wdb(char *szEnginePath, DWORD dwFileCount, PVOID *pWhiteHdr, DWORD dwHdrSize, char *acLogMsg = NULL);
	INT32 asi_make_wdb(char *szEngineFile, DWORD dwCategory, PWHITE_PARSE_DATA pPasrseData, INT32 *pnListCount, char *acLogMsg = NULL);
	BOOL asi_get_wildstr_info(char *szPath, PPARSE_RULE_PATH pWildInfo);
	INT32 asi_get_parse_rule(PWHITE_PARSE_DATA pPasrseData, PWHITE_PARSE_RULE pParseRule, char *acLogMsg = NULL);
	INT32 asi_get_parse_name(char *szData, PPARSE_RULE_NAME pParseName, INT32 nTotalCount);
	INT32 asi_get_parse_path(char *szData, PPARSE_RULE_PATH pParsePath, INT32 nTotalCount);

public:
	CWhitePatternUtil();
	~CWhitePatternUtil();

};

#endif /*_WHITE_PATTERN__*/