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
#include "as_file.h"
#include "as_string.h"

#define CMD_FILE_FORMAT		"/usr/bin/file"

char *strndup(char *str, int len)
{
	char  *new_str;

	if (str == NULL || len < 0)
	{
		return NULL;
	}

	if ( (new_str = (char *) malloc(len + 1)) == NULL)
	{
		return NULL;
	}

	strncpy(new_str, str, len);
	new_str[len] = '\0';

	return new_str;
}



/* Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(src) + MIN(siz, strlen(initial dst)).
 * If retval >= siz, truncation occurred.
 */
int strlcat(char *dst, char *src, int siz)
{
	char  *d = dst;
	char  *s = src;
	int   n = siz;
	int   dlen;

	if (dst == NULL || src == NULL || siz <= 0) {
		return -1;
	}

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0') {
		d++;
	}

	dlen = d - dst;
	n = siz - dlen;

	if (n == 0) {
		return (dlen + strlen(s));
	}

	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}

	*d = '\0';
	return (dlen + (s - src));  /* count does not include NUL */
}

void remove_char_in_str(string &strRemove, char cRem)
{
	int i=0, nLen = 0;
	char *pt = NULL;
	pt = (char *)strRemove.c_str();
	nLen = (int)strRemove.length();
	if(pt == NULL || nLen < 1)
		return;
	for(i=0; i<nLen; i++)
	{
		if(pt[i] == cRem)
		{
			strRemove.erase(strRemove.begin()+i);
			i--;
			nLen--;
		}
	}
}

LPSTR DelLastChar(String& strInput, char cDelChar)
{
	LPSTR pcStr = NULL;
	INT32 nLastIndex = strInput.length()-1;
	if(nLastIndex < 0)
		return (LPSTR)strInput.c_str();
	pcStr = (LPSTR)strInput.c_str();
	if(pcStr[nLastIndex] == cDelChar)
	{
		pcStr[nLastIndex] = '\0';
	}
	return pcStr;
}



int count_rule_data(char *pString, char cDelimiter, int *pnCount)
{
	int nCount = 0;
	int nLen, i;

	if(pString == NULL || pnCount == NULL)
	{
		return -1;
	}

	nLen = (int)strlen(pString);
	if(nLen < 1)
	{
		return -2;
	}

	nCount = 1;
	for (i=0; i<nLen; i++)
	{
		if(*(pString + i) == cDelimiter)
			nCount++;
	}
	*pnCount = nCount;
	return 0;
}

void replace_char_in_str(char *str, char s, char d)
{
	char  *pt;

	pt = str;

	while (*pt++) {
		if (*pt == s) {
			*pt = d;
		}
	}
}

INT32  conver_utf8(char *pSrcData, int nSrcLen, char **ppDstData)
{
	iconv_t it;
	INT32 nRetVal = 0;
	char *pDstData = NULL;
	char *pSrc = NULL;
	char *pDst = NULL;

	size_t nSrcStrLen = (size_t)nSrcLen;
	size_t nDstStrLen = nSrcLen*4+1;

	if(pSrcData == NULL || ppDstData == NULL || nSrcStrLen < 1)
		return -1;

	it = iconv_open(CHARSET_UTF8, CHARSET_EUCKR);
	if (it == (iconv_t)(-1))
	{
		return -2;
	}

	do{
		pDstData = (char *)malloc(nDstStrLen + 1);
		if(pDstData == NULL)
		{
			nRetVal = -4;
			break;
		}
		memset(pDstData, 0, nDstStrLen + 1);

		pSrc = pSrcData;
		pDst = pDstData;

		if(iconv(it, &pSrc, &nSrcStrLen, &pDst, &nDstStrLen) == (size_t)(-1))
		{
			nRetVal = -5;
			break;
		}

		nRetVal = 0;
	}while(FALSE);

	iconv_close(it);

	if(nRetVal == 0)
	{
		*ppDstData = pDstData;
	}
	else
	{
		safe_free(pDstData);
	}
	return nRetVal;
}

INT32  make_utf8_file(char *pRootPathData)
{
	INT32 nPathLen = 0;
	INT32 nRetVal = 0;
	char *pUtf8 = NULL;
	char acFilePath[MAX_PATH] = {0,};
	FILE *fp = NULL;
	if(pRootPathData == NULL)
		return -1;

	snprintf(acFilePath, MAX_PATH-1, "%s/pem/utf_test", pRootPathData);
	if(DirectoryExists(acFilePath) == FALSE)
		CreateDirectory(acFilePath, NULL);

	snprintf(acFilePath, MAX_PATH-1, "%s/pem/utf_test/ÇÑ±Û»ùÇÃ.utf", pRootPathData);
	nPathLen = strlen(acFilePath);

	if(is_file(acFilePath) == REG_FILE)
	{
		return 0;
	}

	do{
		nRetVal = conver_utf8(acFilePath, nPathLen+2, &pUtf8);
		if(nRetVal != 0 || pUtf8 == NULL)
		{
			nRetVal = -2;
			break;
		}
		fp = fopen(pUtf8, "wt");
		if(fp == NULL)
		{
			nRetVal = -3;
			break;
		}
		fprintf(fp, "utf-8 file name test");
		fclose(fp);
		nRetVal = 0;
	}while(FALSE);

	safe_free(pUtf8);
	return nRetVal;
}


INT32 is_utf8_file(char *pRootPathData)
{
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	INT32 nRetVal = 0;
	INT32 nLen = 0;
	FILE* pPipe = NULL;
	char *pBuff = NULL;
	char acFilePath[MAX_PATH] = {0,};
	char acFileName[MAX_FILE_NAME] = {0,};
	FILE* fp = NULL;

	if(pRootPathData == NULL)
		return -1;

	nRetVal = is_file((char *)CMD_FILE_FORMAT);
	if(nRetVal != 0)
	{
		return -2;
	}
	snprintf(acFilePath, MAX_PATH-1, "%s/pem/utf_test", pRootPathData);

	dp = opendir(acFilePath);
	if (dp == NULL)
	{
		nRetVal = make_utf8_file(pRootPathData);
		if(nRetVal != 0)
		{
			nRetVal -= 100;
			return nRetVal;
		}
		Sleep(100);
		dp = opendir(acFilePath);
		if (dp == NULL)
		{
			return -3;
		}
	}

	nRetVal = -5;
	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
		{
			continue;
		}
		if(DT_REG == dirp->d_type)
		{
			strncpy(acFileName, dirp->d_name, MAX_FILE_NAME-1);
			nLen = strlen(acFileName);
			if(nLen < 4)
				continue;
			if(_stricmp(&acFileName[nLen-4], ".utf"))
				continue;
			snprintf(acFilePath, MAX_PATH-1, "%s/pem/utf_test/chech_utf.txt", pRootPathData);

			fp = fopen(acFilePath, "wt");
			if(fp == NULL)
			{
				nRetVal = -4;
				break;
			}
			fprintf(fp, "%s", acFileName);
			fclose(fp);
			nRetVal = 0;
			break;
		}
	}
	closedir(dp);

	if(nRetVal != 0)
	{
		return nRetVal;
	}

	do{
		pBuff = (char *)malloc(CHAR_MAX_SIZE);
		if(pBuff == NULL)
		{
			nRetVal = -6;
			break;
		}
		memset(pBuff, 0, CHAR_MAX_SIZE);

		snprintf(pBuff, CHAR_MAX_SIZE-1, "%s %s", CMD_FILE_FORMAT, acFilePath);

		pPipe = popen(pBuff, "r");
		if(pPipe == NULL)
		{
			nRetVal = -7;
			break;
		}
		memset(pBuff, 0, CHAR_MAX_SIZE);

		if(fgets(pBuff, CHAR_MAX_SIZE-1, pPipe) == NULL)
		{
			nRetVal = -8;
			break;
		}
		pBuff[CHAR_MAX_SIZE-1] = 0;

		nLen = strlen(pBuff);
		if(nLen < 3)
		{
			nRetVal = -9;
			break;
		}

		if(strstr(pBuff, "UTF-8") == NULL)
		{
			nRetVal = 0;
			break;
		}
		nRetVal = 1;
	}while(FALSE);

	if(pPipe != NULL)
		pclose(pPipe);
	safe_free(pBuff);
	unlink(acFilePath);

	return nRetVal;
}

String	SPrintf(const char* fmt,...)
{
	String strBuff;
	char acBuf[2*CHAR_MAX_SIZE] = {0, };
	va_list args;
	va_start(args,fmt);
    vsnprintf(acBuf, 2*CHAR_MAX_SIZE-1, fmt, args);
    va_end(args);
	acBuf[2*CHAR_MAX_SIZE-1] = 0;
	strBuff = acBuf;
	return strBuff;
}


StringW	WPrintf(const wchar_t* fmt,...)
{
	StringW wstrBuff;
	wchar_t wcBuf[2*CHAR_MAX_SIZE] = {0, };
	va_list args;
	va_start(args,fmt);
    vswprintf(wcBuf, 2*CHAR_MAX_SIZE-1, fmt, args);
    va_end(args);
	wcBuf[2*CHAR_MAX_SIZE-1] = 0;
	wstrBuff = wcBuf;
	return wstrBuff;
}

String _strlwr(const char *str)
{
    int i=0;
    int len=0;
	String strRet = "";
	char acTempData[CHAR_MAX_SIZE] = {0,};
	if(str == NULL)
	{
		return strRet;
	}
    len=strlen(str);
	if(len < 1)
	{
		return strRet;
	}
	else if(len > CHAR_MAX_SIZE-1)
	{
		len = CHAR_MAX_SIZE -1;
	}

    strncpy(acTempData, str, len);
	acTempData[len] = 0;

    for(i=0;i<len;i++)
	{
        acTempData[i]=(char)_tolower(acTempData[i]);
    }
    strRet = acTempData;
	return strRet;
}

String _strupr(const char *str)
{
	int i=0;
	int len=0;
	String strRet = "";
	char acTempData[CHAR_MAX_SIZE] = {0,};
	if(str == NULL)
	{
		return strRet;
	}
	len=strlen(str);
	if(len < 1)
	{
		return strRet;
	}
	else if(len > CHAR_MAX_SIZE-1)
	{
		len = CHAR_MAX_SIZE -1;
	}

	strncpy(acTempData, str, len);
	acTempData[len] = 0;

	for(i=0;i<len;i++)
	{
		acTempData[i]=(char)_toupper(acTempData[i]);
	}
	strRet = acTempData;
	return strRet;
}
//-------------------------------------------------------------------------

LPTSTR	FormatString(LPTSTR lpBuf, LPTSTR fmt,...)
{
	va_list args;

	memset(lpBuf, 0, CHAR_MAX_SIZE);

	va_start(args,fmt);
	vsnprintf(lpBuf, CHAR_MAX_SIZE - 1, fmt, args);
	va_end(args);

	return lpBuf;
}

String	ConvertAnsiString(StringW strInputW)
{
	String strBuff;
	char pcBuf[CHAR_MAX_SIZE] = {0,};
	int nLast = 0;
	nLast = wcstombs(pcBuf,strInputW.c_str(), 100);
	if(nLast == -1)
		return "";
	else if(nLast > CHAR_MAX_SIZE-1)
		nLast = CHAR_MAX_SIZE-1;
	pcBuf[nLast] = '\0';
	strBuff = pcBuf;
	return strBuff;
}

StringW	ConvertWideString(String strInput)
{
	wchar_t wcBuf[CHAR_MAX_SIZE] = {0,};
	
	int nLast = 0;
	nLast = mbstowcs((wchar_t*)wcBuf, (char *)strInput.c_str(), 100);
	if(nLast == -1)
		return StringW(L"");
	else if(nLast > CHAR_MAX_SIZE-1)
		nLast = CHAR_MAX_SIZE-1;
	wcBuf[nLast] = L'\0';
	return StringW(wcBuf);
}

//-------------------------------------------------------------------------

void	ReverseLPTSTR(LPTSTR lpInput)
{
	INT32 nLen = strlen(lpInput);
	CHAR chSwap;

	for(INT32 i = 0; i < nLen / 2; i++)
	{
		chSwap = lpInput[i];
		lpInput[i] = lpInput[nLen - (i + 1)];
		lpInput[nLen - (i + 1)] = chSwap;
	}
	return;
}
//-------------------------------------------------------------------------

String		ReverseStr(String strInput)
{
	String strRtn = "";
	INT32 nLen = strInput.length();
	LPCTSTR lpInput = strInput.c_str();

	for(INT32 i = nLen; i > 0; i--)
	{
		strRtn += lpInput[i-1];
	}
	return strRtn;
}

//-------------------------------------------------------------------------

LPTSTR	ConvertLPTSTR(String& strValue)
{
	return (LPTSTR)strValue.c_str();
}
//-------------------------------------------------------------------------

String&	EmptyStr(String& strValue)
{
	strValue.empty();
	return strValue;
}
//-------------------------------------------------------------------------

LPTSTR	FindReverChar(LPCTSTR lpSrc, CHAR chFind)
{
	LPTSTR lpFind = (LPTSTR)strrchr(lpSrc, chFind);
	if(!lpFind)		return (LPTSTR)lpSrc;

	if(lpFind[0] == chFind)		return &(lpFind[1]);
	return lpFind;
}
//-------------------------------------------------------------------------

CHAR GetToken(String& strValue, CHAR chDefTok, CHAR chFindTok)
{
	if(strValue.find(chFindTok) != string::npos)	return chFindTok;
	return chDefTok;
}

//-------------------------------------------------------------------------

String ConvertIP(UINT32 nIP)
{
	String strResult;

	if(!nIP)	return strResult;

	BYTE btIP[4] = {0, };
	memcpy(&btIP[0], (BYTE*)&nIP, 1);
	memcpy(&btIP[1], (BYTE*)(&nIP)+1, 1);
	memcpy(&btIP[2], (BYTE*)(&nIP)+2, 1);
	memcpy(&btIP[3], (BYTE*)(&nIP)+3, 1);

	strResult = SPrintf("%u.%u.%u.%u", btIP[0], btIP[1], btIP[2], btIP[3]);

	return strResult;
}
//-------------------------------------------------------------------------

UINT32	ConvertHost(LPCTSTR lpAddress)
{
	if(lpAddress == NULL || lpAddress[0] == 0)
		return (UINT32)INADDR_NONE;
	if(strchr(lpAddress, '.') == NULL)
		return (UINT32)INADDR_NONE;

	return inet_addr(lpAddress);
}
//-------------------------------------------------------------------------

String ConvertDateTime(UINT32 nTime, UINT32 nFormat)
{
	if(!nTime)	return "";

	time_t t = (time_t)nTime;
	struct tm *dt;
	dt = gmtime(&t);

	if(!dt)		return "";

	String strRtn;

	switch(nFormat)
	{
	case 1: strRtn = SPrintf("%.4d-%.2d-%.2d %.2d:%.2d:%.2d", dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);			break;
	case 2:	strRtn = SPrintf("%.4d-%.2d-%.2d %.2d:%.2d", dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday, dt->tm_hour, dt->tm_min);							break;
	default:strRtn = SPrintf("%.4d-%.2d-%.2d", dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday);																break;
	}

	return strRtn;
}
//-------------------------------------------------------------------------

INT32 StrMatchSpec(LPCTSTR pszTarget, LPCTSTR pszSpec)
{
	char *cp = NULL, *mp = NULL;
	while (*pszTarget) {
		if (*pszSpec == '*')
		{
			if (!(*(++pszSpec)))
			{
				return 1;
			}
			mp = (LPTSTR)pszSpec;
			cp = (LPTSTR)(pszTarget+1);
		}
		else if (((*pszSpec == *pszTarget) && (*pszSpec != '#')) || (*pszSpec == '?') || ((*pszSpec == '#') && isdigit(*pszTarget)))
		{
			pszSpec++;
			pszTarget++;
		}
		else
		{
			if (mp)
			{
				pszSpec = mp;
				pszTarget = cp++;
			}
			else
			{
				return 0;
			}
		}
	}

	while (*pszSpec == '*') {
		pszSpec++;
	}
	return !(*pszSpec);
}
//-------------------------------------------------------------

LPTSTR   strcat_ext(LPTSTR lpTarget, LPCTSTR lpSource, UINT32 nTarSize)
{
	if(strlen_ext(lpSource) + strlen_ext(lpTarget) >= nTarSize)	return lpTarget;

	UINT32 nTarLen = strlen_ext(lpTarget);
	UINT32 nCpIdx = 0;
	while(nCpIdx < nTarSize)
	{
		if(lpSource[nCpIdx] == 0)	break;

		lpTarget[nCpIdx + nTarLen] = lpSource[nCpIdx];
		nCpIdx += 1;
	}
	return lpTarget;
}
//-------------------------------------------------------------------------

UINT32	strlen_ext(LPCTSTR lpValue)
{
	if(!lpValue)	return 0;
	UINT32 nLen = (UINT32)strlen(lpValue);
	return nLen;
}

INT32	GetLStrLen(UINT16 nSize)
{
	if(!nSize)	return 0;

	return (nSize * nSize);
}
//-------------------------------------------------------------------------

void	sprintf_ext(UINT32 nTarSize, char *lpTarget, const char *fmt,...)
{
	ZeroMemory(lpTarget, nTarSize);

	va_list args;
	va_start(args,fmt);
	vsnprintf(lpTarget, nTarSize, fmt, args);
	va_end(args);
	return;
}

//-------------------------------------------------------------------------

String	QueryToMem(String& strValue)
{
	replace_char_in_str((char *)strValue.c_str(), '\a', '\'');
	replace_char_in_str((char *)strValue.c_str(), '\f', '\\');
	return strValue;
}
//-------------------------------------------------------------------------

String	MemToQuery(String strValue)
{
	replace_char_in_str((char *)strValue.c_str(), '\'', '\a');
	replace_char_in_str((char *)strValue.c_str(), '\\', '\f');
	return strValue;
}
//-------------------------------------------------------------------------

String		PathUnixToWin(String& strValue)
{
	replace_char_in_str((char *)strValue.c_str(), '/', '\\');
	return strValue;
}
//-------------------------------------------------------------------------

String		PathWinToUnix(String strValue)
{
	replace_char_in_str((char *)strValue.c_str(), '\\', '/');
	return strValue;
}

INT32  ConvertCharsetString(char *pSrcCharSet, char *pDstCharSet, String strSrcData, String &strDstData)
{
	iconv_t it;
	INT32 nRetVal = 0;
	char *pSrcData = NULL;
	char *pDstData = NULL;
	char *pSrc = NULL;
	char *pDst = NULL;

	size_t nSrcStrLen = (size_t)strSrcData.length();
	size_t nDstStrLen = nSrcStrLen*4+1;

	if(pSrcCharSet == NULL || pDstCharSet == NULL || nSrcStrLen < 1)
		return -1;

	it = iconv_open(pDstCharSet, pSrcCharSet);
	if (it == (iconv_t)(-1))
	{
		nRetVal = errno;
		nRetVal -= 1000;
		return nRetVal;
	}

	do{
		pSrcData = (char *)malloc(nSrcStrLen + 2);
		if(pSrcData == NULL)
		{
			iconv_close(it);
			nRetVal = -3;
			break;
		}
		memset(pSrcData, 0, nSrcStrLen + 2);
		strncpy(pSrcData, strSrcData.c_str(), nSrcStrLen);
		pDstData = (char *)malloc(nDstStrLen + 1);
		if(pDstData == NULL)
		{
			iconv_close(it);
			nRetVal = -4;
			break;
		}
		memset(pDstData, 0, nDstStrLen + 1);

		pSrc = pSrcData;
		pDst = pDstData;

		if(iconv(it, &pSrc, &nSrcStrLen, &pDst, &nDstStrLen) == (size_t)(-1))
		{
			nRetVal = errno;
			iconv_close(it);
			nRetVal -= -2000;
			break;
		}
		iconv_close(it);

		if(pDstData[0] == 0)
		{
			nRetVal = -6;
			break;
		}
		strDstData = pDstData;
		nRetVal = 0;
	}while(FALSE);

	safe_free(pSrcData);
	safe_free(pDstData);

	return nRetVal;
}
