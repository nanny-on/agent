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

#ifndef ASI_LNX_STRING_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define ASI_LNX_STRING_H_938EF200_F60B_452A_BD30_A10E8507EDCC

#include <string>
#include <vector>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

class CString
{
public:
	CString(void);
	~CString(void);

public:
	CString(const CString& obj);
	CString(const char* p);
	CString(const char p);
	CString(const char* p, int nLength);

	void operator=(const CString& obj);
	void operator=(const char* p);
	void operator=(const char p);
	void operator=(char* p);
	char operator[](int nIdx) const;

	CString& operator+=(const char* p);
	CString& operator+=(const char p);
	friend CString operator+(const CString& str1, const char* psz2 );

	bool operator==(const char* p);
	explicit operator char*();
	operator const char*();
	operator const char*() const;
	int GetLength(void);
	bool IsEmpty();
	int Find(const char* pszSub, int iStart=0);
	int Find(const char ch, int iStart=0);
	int ReverseFind(const char* pszSub, int iStart=0);
	int ReverseFind(const char ch, int iStart=0);
	char GetAt( int nIndex );
	char* GetLPTSTR();
	void Empty( );

	CString Mid(int iFirst, int nCount);
	CString Mid(int iFirst);	
	int Delete(int iIndex, int nCount = 1);
	int Remove(char ch );
	int Insert(int iIndex, const char* psz);
	int Insert(int iIndex, char ch);
	CString Left( int nCount ) const;
	CString Right( int nCount ) const;
	CString& Trim();
	CString& TrimRight(char chTarget);
	CString& TrimRight(const char* pszTargets);
	CString& TrimLeft(char chTarget);
	CString& TrimLeft(const char* pszTargets);
	int Replace(const char* pszOld, const char* pszNew);
	int Replace(char pszOld, char pszNew);
	void Format(const char* pszFormat, ...);
	int Compare( LPCTSTR lpsz ) const;
	int CompareNoCase( LPCTSTR lpsz ) const;
	CString& MakeLower( );
	CString& MakeUpper( );
	CString& ConvertWinToUnix();
	bool operator<(const CString& obj)	const;

private:
	INT32 IsString(char* szBuf);
	void InitNaming();

protected:
	char m_szClsName[30];
	std::string m_str;

};


#endif //ASI_LNX_STRING_H_938EF200_F60B_452A_BD30_A10E8507EDCC
