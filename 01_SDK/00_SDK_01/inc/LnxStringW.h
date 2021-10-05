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

#ifndef ASI_LNX_STRINGW_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define ASI_LNX_STRINGW_H_938EF200_F60B_452A_BD30_A10E8507EDCC

#include <string>
#include <vector>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

class CStringW
{
public:
	CStringW(void);
	~CStringW(void);

public:
	CStringW(const CStringW& obj);
	CStringW(const wchar_t* p);
	CStringW(const wchar_t p);
	CStringW(const wchar_t* p, int nLength);

	void operator=(const CStringW& obj);
	void operator=(const wchar_t* p);
	void operator=(const wchar_t p);
	void operator=(wchar_t* p);
	wchar_t operator[](int nIdx) const;
	
	CStringW& operator+=(const wchar_t* p);
	CStringW& operator+=(const wchar_t p);
	friend CStringW operator+(const CStringW& str1, const wchar_t* psz2 );
	bool operator==(const wchar_t* p);
	operator const wchar_t*();
	operator const wchar_t*()	const;
	int GetLength(void);
	bool IsEmpty();
	int Find(const wchar_t* pszSub, int iStart=0);
	int Find(const wchar_t ch, int iStart=0);
	int ReverseFind(const wchar_t* pszSub, int iStart=0);
	int ReverseFind(const wchar_t ch, int iStart=0);
	wchar_t GetAt( int nIndex );
	wchar_t* GetLPTSTR();
	void Empty( );
	
	CStringW Mid(int iFirst, int nCount);
	CStringW Mid(int iFirst);
	int Delete(int iIndex, int nCount = 1);
	int Remove(wchar_t ch );
	int Insert(int iIndex, const wchar_t* psz);
	int Insert(int iIndex, wchar_t ch);
	CStringW Left( int nCount ) const;
	CStringW Right( int nCount ) const;
	CStringW& Trim();
	CStringW& TrimRight(wchar_t chTarget);
	CStringW& TrimRight(const wchar_t* pszTargets);
	CStringW& TrimLeft(wchar_t chTarget);
	CStringW& TrimLeft(const wchar_t* pszTargets);
	int Replace(const wchar_t* pszOld, const wchar_t* pszNew);
	int Replace(wchar_t pszOld, wchar_t pszNew);
	void Format(const wchar_t* pszFormat, ...);
	int Compare( const wchar_t* lpsz ) const;
	int CompareNoCase( const wchar_t* lpsz ) const;
	CStringW&  MakeLower( );
	CStringW&  MakeUpper( );
	bool operator<(const CStringW& obj)	const;
	
private:
	INT32 IsString(wchar_t* szBuf);
	void InitNaming();

protected:
	wchar_t m_szClsName[30];
	std::wstring m_str;
};

#endif //ASI_LNX_STRINGW_H_938EF200_F60B_452A_BD30_A10E8507EDCC
