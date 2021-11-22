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

#ifndef _AS_STR_TOKEN_H_
#define _AS_STR_TOKEN_H_

typedef struct str_token {
	int   alloc_size;		/* 실제 문자열을 저장할 수 있는 공간 */
	int   data_len;			/* NULL 문자를 제외한 실제 문자열 길이 */
	char  data[0];
} str_token_t;


void add_token_str(str_token_t **token, char *str, char *sep);
void add_token_int(str_token_t **token, int num, char *sep);
int count_token(char *pSrc, int ch);
char *copy_token(char *pDst, int nDstMax, char *pSrc, int ch);

int HexStringToInt(char *pHexString);
int StringToInt(char *pString);
UINT64 StringToUInt64(const char *pString);

#endif /* _AS_STR_TOKEN_H_ */


