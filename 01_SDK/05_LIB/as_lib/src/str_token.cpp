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
#include "as_util.h"
#include "as_str_token.h"

static str_token_t *alloc_token(int size);
static void token_append(str_token_t **token, char *str, int len);


void add_token_str(str_token_t **token, char *str, char *sep)
{
	token_append(token, str, strlen(str));

	if (sep != NULL) {
		token_append(token, sep, strlen(sep));
	}
}


void token_int(str_token_t **token, int num, char *sep)
{
	char  tmp[128] = { 0,};

	snprintf(tmp, sizeof(tmp), "%d", num);
	token_append(token, tmp, strlen(tmp));

	if (sep != NULL) {
		token_append(token, sep, strlen(sep));
	}
}


static str_token_t *alloc_token(int size)
{
	str_token_t  *token = NULL;

	token = (str_token_t *) malloc(sizeof(str_token_t) + size + 1);
	if(token != NULL)
	{
		token->alloc_size  = size;
		token->data_len    = 0;
		token->data[0]     = '\0';
	}
	return token;
}

int count_token(char *pSrc, int ch)
{
	int nCount = 1;
	int i, nLen = 0;

	if(pSrc == NULL)
		return 0;

	nLen = (int)strlen(pSrc);
	if(nLen < 1)
		return 0;

	for(i=0; i<nLen; i++)
	{
		if(pSrc[i] == (char)ch)
			nCount++;
	}
	return nCount;
}

char *copy_token(char *pDst, int nDstMax, char *pSrc, int ch)
{
	char *ptr = NULL;
	int nLen = 0;
	if(pDst == NULL || nDstMax < 1 || pSrc == NULL)
		return NULL;

	ptr = strchr(pSrc, ch);
	if ( ptr )
	{
		nLen = (int)(ptr - pSrc);
		if(nLen > nDstMax-1)
			nLen = nDstMax-1;
		strncpy(pDst, pSrc, nLen);
		pDst[nLen] = '\0';
		return ptr + 1;
	}

	strncpy(pDst, pSrc, nDstMax-1);
	pDst[nDstMax-1] = 0;
	return NULL;
}



int HexStringToInt(char *pHexString)
{
	if(pHexString == NULL)
		return 0;
    return (int)strtol(pHexString, NULL, 16);
}

int StringToInt(char *pString)
{
	if(pString == NULL)
		return 0;

    return (int)strtol(pString, NULL, 10);
}

UINT64 StringToUInt64(const char *pString)
{
	if(pString == NULL)
		return 0;

	return (UINT64)stoull(pString, NULL, 0);
}

static void token_append(str_token_t **token, char *str, int len)
{
	if (str == NULL) {
		len = 0;
	}

	if (*token == NULL) {
		*token = alloc_token(len << 1);
	}

	if ( ((*token)->data_len + len) > (*token)->alloc_size) {
		/* 너무 자주 realloc이 수행되지 않도록 미리 2배 할당 */
		(*token)->alloc_size = ((*token)->data_len + len) << 1;
		*token = (str_token_t *) realloc(*token, sizeof(str_token_t) + (*token)->alloc_size + 1);
	}

	if (str != NULL) {
		memcpy((*token)->data + (*token)->data_len, str, len);
	}

	(*token)->data_len += len;
	(*token)->data[(*token)->data_len] = '\0';
}


