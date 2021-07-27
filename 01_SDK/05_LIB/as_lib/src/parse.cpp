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

int is_digit(int c)
{
	if(c >= 0x30 && c <= 0x39)
	{
		return true;
	}
	return false;
}

int is_space(int c)
{
	if(c >= 0x09 && c <= 0x0d)
	{
		return true;
	}
	if(c == 0x20)
	{
		return true;
	}
	return false;
}

/* 문자열을 왼쪽에서 오른쪽으로 검색하여 공백문자가 있을 경우
 * 건너 뜀
 * ex) char *pt = "   hello world !!!"
 *  => pt == 'hello world !!!'
 */
char *skip_space(char *str)
{
	while (is_space((int)*str)) {
		str++;
	}

	return str;
}


/* 문자열에 포함된 공백문자를 모두 제거
 * ex) " hello world !!! "
 *  => "helloworld!!!"
 */
void strip_space(char *str)
{
	char  *t, *q;

	t = q = str;
	while (*t) {
		if (! is_space((int) *t)) {
			*q++ = *t;
		}
		t++;
	}
	*q = '\0';
}


/* 문자열을 왼쪽에서 오른쪽으로 검색하여 공백문자가 있을 경우
 * 해당 위치의 공백문자를 '\0'로 대치
 * ex) "hello world !!! "
 *  => "hello"
 */
void trim_space(char *str)
{
	while (*str) {
		if (is_space((int)*str)) {
			*str = '\0';
			break;
		} else {
			str++;
		}
	}
}


/* 문자열의 왼쪽 부분에 공백이 있을 경우 문자열을 제외한
 * 공백 부분만 제거
 * ex) " 	hello world !!! "
 *  => "hello world !!! "
 */
void trim_left_white_space(char *str)
{
	int   i, len;
	char  *pt;

	if (strlen(str) == 0 || ! is_space((int)*str)) {
		return;
	}

	pt = str;

	while (*pt != '\0') {
		if (is_space((int) *pt)) {
			pt++;
		} else {
			break;
		}
	}

	len = strlen(pt);

	for (i = 0; i < len; i++) {
		*(str + i) = *(pt + i);
	}

	*(str + i) = '\0';
}


/* 문자열의 오른쪽 부분에 공백이 있을 경우 문자열을 제외한
 * 공백 부분만 제거
 * ex) " hello world !!!	 "
 *  => " hello world !!!"
 */
void trim_right_white_space(char *str)
{
	int   len, done = 0;
	char  *pt;

	len = strlen(str);

	if (len == 0) {
		return;
	}

	for (pt = str + len - 1; pt != str - 1; pt--) {
		if (is_space((int) *pt)) {
			continue;
		} else {
			*(pt + 1) = '\0';
			done = 1;
			break;
		}
	}

	if (done == 0) {
		*str = '\0';
	}
}


void trim_white_space(char *str)
{
	trim_left_white_space(str);
	trim_right_white_space(str);
}


void trim_line_feed(char *str)
{
	while (*str) {
		if (*str == 0x0a || *str == 0x0d) {
			*str = '\0';
			break;
		} else {
			str++;
		}
	}
}


int is_valid_num(char *str)
{
	int i, len;

	if (str == NULL || str[0] == '\0') {
		return -1;
	}

	len = strlen(str);

	for (i = 0; i < len; i++) {
		if (! is_digit((int) str[i])) {
			return -1;
		}
	}

	return 0;
}


char *str_ntoken(char *str, const char *delim, char **nstr)
{
	char  *cp;

	cp     = NULL;
	*nstr  = NULL;

	if (str == NULL || delim == NULL || strlen(str) == 0) {
		return NULL;
	}


	if ( (cp = strstr(str, delim)) != NULL) {

		*cp++ = 0x00;

		if (*cp == 0x00) {
			*nstr = NULL;
		} else {
			*nstr = cp;
		}

		return str;

	} else {
		*nstr = NULL;
		return str;
	}
}

char *copy_token(char *dest, char *src, int ch)
{
	char *ptr = NULL;
	int nSrcLen = 0;
	int nLen = 0;

	if(dest == NULL || src == NULL)
		return NULL;

	nSrcLen = strlen(src);
	if(nSrcLen < 1)
		return NULL;

	ptr = strchr(src, ch);
	if ( ptr )
	{
		nLen = (int)(ptr - src);
		if(nLen > nSrcLen || nLen < 1)
			return NULL;

		strncpy(dest, src, nLen);
		dest[nLen] = '\0';

		trim_white_space(dest);

		return ptr + 1;
	}
	else
	{
		strncpy(dest, src, nSrcLen);
		dest[nSrcLen] = 0;
		trim_white_space(dest);
	}

	return NULL;
}


void remove_tail_crlf(char *szString)
{
	int i = 0;
	if(szString)
	{
		i = (int)strlen(szString);
		while (i > 0)
		{
			if ((szString[i-1] == '\n') || (szString[i-1] == '\r'))
				szString[i-1] = 0;
			else
				break;
			i--;
		}
	}
}

char *get_basename(char *pSrc, char *pDest, int nSize)
{
	char *basep, *s_ptr, *d_ptr;

	if (pSrc == NULL || pDest == NULL || nSize < 2)
		return NULL;

	if (*pSrc == '\0')
	{
		*pDest = '\0';
		return pDest;
	}

	basep = s_ptr = pSrc;

	while (*s_ptr)
	{
		if (*s_ptr++ == '/')
		{
			if (*s_ptr != '/' && *s_ptr != '\0')
				basep = s_ptr;
		}
	}

	s_ptr = basep;
	d_ptr = pDest;

	*d_ptr++ = *s_ptr++;   /* this is needed for '/////' => '/'. */
	nSize--;

	while (*s_ptr != '\0' && *s_ptr != '/' && nSize > 1)
	{
		*d_ptr++ = *s_ptr++;
		nSize--;
	}

	*d_ptr = '\0';

	return pDest;
}


char *get_dirname(char *src, char *dest, int size)
{
    char *ptr;

    if (src == NULL || dest == NULL || size < 2)
        return NULL;

    if (src != dest)
    {
        strncpy(dest, src, size - 1);
        dest[size - 1] = '\0';
    }

    if (*dest == '\0')
    {
        *dest = '.';
        *(dest + 1) = '\0';

        return (dest);
    }

    ptr = dest + strlen(dest);
    while (ptr != dest && *--ptr == '/')  /* trim trailing slash */
        ;

    if (ptr == dest && *ptr == '/')
    {
        *(ptr + 1) = '\0';
        return (dest);  /* return '/' */
    }

    while (ptr != dest)
    {
        if (*--ptr == '/')
        {
            while (*ptr == '/' && ptr != dest)
                ptr--;
            *++ptr = '\0';
            return (dest);
        }
    }

   	*dest = '.';
   	*(dest + 1) = '\0';
    return (dest);
}

BOOL split_filename(char *pPath, char *pFile, int size)
{
    char *ptr;

    if (pPath == NULL || pFile == NULL || size < 2)
        return FALSE;

    if (pPath[0] == '\0')
    {
		return FALSE;
    }

    ptr = pPath + strlen(pPath);
    while (ptr != pPath && *--ptr == '/')  /* trim trailing slash */
        ;

    if (ptr == pPath && *ptr == '/')
    {
		return FALSE;
    }

    while (ptr != pPath)
    {
        if (*--ptr == '/')
        {
            while (*ptr == '/' && ptr != pPath)
                ptr--;
			++ptr;
            ptr[0] = '\0';
			++ptr;
			if(ptr[0] != 0)
			{
				strncpy(pFile, ptr, size-1);
				return TRUE;
			}
			else
				return FALSE;
        }
    }
	return FALSE;
}


char *get_extname(char *pFileName, char *pExtName, int nExtSize)
{
	char *ptr = NULL;
	if (pFileName == NULL || pFileName[0] == 0 || pExtName == NULL || nExtSize < 2)
	{
		return NULL;
	}
	ptr = strrchr(pFileName, '.');
	if(ptr == NULL)
	{
		return NULL;
	}
	if(ptr[1] == 0)
	{
		return NULL;
	}

	strncpy(pExtName, ptr, nExtSize-1);
	pExtName[nExtSize-1] = 0;
	return pExtName;
}

char *remove_extname(char *pSrcName, char *pDestName, int nDestSize)
{
	char *ptr = NULL;

	if (pSrcName == NULL || pSrcName[0] == 0 || pDestName == NULL || nDestSize < 2)
	{
		return NULL;
	}

    if (pSrcName != pDestName)
    {
        strncpy(pDestName, pSrcName, nDestSize - 1);
        pDestName[nDestSize - 1] = '\0';
    }

	ptr = strrchr(pDestName, '.');
	if(ptr == NULL)
	{
		return NULL;
	}

    while (ptr != pDestName && *--ptr == '.')
        ;

    if (ptr == pDestName && *ptr == '.')
    {
        return NULL;
    }
    *++ptr = '\0';
	return pDestName;
}

