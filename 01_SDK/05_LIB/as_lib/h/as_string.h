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
 * 
 */

#ifndef _AS_STRING_H_
#define _AS_STRING_H_

#include "LnxString.h"

extern char *strndup(char *str, int len);
extern int strlcat(char *dst, char *src, int siz);
extern void replace_char_in_str(char *str, char s, char d);
extern void remove_char_in_str(String &strRemove, char cRem);
extern LPSTR DelLastChar(String& strInput, char cDelChar);
extern int count_rule_data(char *pString, char cDelimiter, int *pnCount);
extern String SPrintf(const char* fmt,...);
extern StringW	WPrintf(const wchar_t* fmt,...);
extern int count_rule_data(char *pString, char cDelimiter, int *pnCount);
extern String _strlwr(const char *str);
extern String _strupr(const char *str);
extern LPTSTR FormatString(LPTSTR lpBuf, LPTSTR fmt,...);
extern void ReverseLPTSTR(LPTSTR lpInput);
extern String ConvertAnsiString(StringW strInputW);
extern StringW ConvertWideString(String strInput);
extern String ReverseStr(String strInput);
extern LPTSTR ConvertLPTSTR(String& strValue);
extern String&	EmptyStr(String& strValue);
extern LPTSTR FindReverChar(LPCTSTR lpSrc, CHAR chFind);
extern CHAR GetToken(String& strValue, CHAR chDefTok, CHAR chFindTok);
extern String ConvertIP(UINT32 nIP);
extern UINT32 ConvertHost(LPCTSTR lpAddress);
extern String ConvertDateTime(UINT32 nTime, UINT32 nFormat);
extern INT32 StrMatchSpec(LPCTSTR pszTarget, LPCTSTR pszSpec);
extern LPTSTR strcat_ext(LPTSTR lpTarget, LPCTSTR lpSource, UINT32 nTarSize);
extern UINT32 strlen_ext(LPCTSTR lpValue);
extern INT32 GetLStrLen(UINT16 nSize);
extern void	sprintf_ext(UINT32 nTarSize, char *lpTarget, const char *fmt,...);
extern String QueryToMem(String& strValue);
extern String MemToQuery(String strValue);
extern String PathUnixToWin(String& strValue);
extern String PathWinToUnix(String strValue);
extern INT32 ConvertCharset(char *pSrcCharSet, char *pDstCharSet, char *pSrcData, int nSrcLen, char *pDstData, int nDstLen);
extern INT32 ConvertCharsetString(char *pSrcCharSet, char *pDstCharSet, String strSrcData, String &strDstData);
extern INT32 conver_utf8(char *pSrcData, int nSrcLen, char **ppDstData);
extern INT32 make_utf8_file(char *pRootPathData);
extern INT32 is_utf8_file(char *pRootPathData);
#endif /* _AS_STRING_H_ */


