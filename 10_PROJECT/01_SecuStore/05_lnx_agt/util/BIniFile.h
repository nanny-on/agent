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

#if !defined(AFX_BINIFILE_H__695DB7A2_39EB_4476_A340_43842C207239__INCLUDED_)
#define AFX_BINIFILE_H__695DB7A2_39EB_4476_A340_43842C207239__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBIniFile  
{
public:
	inline LPCTSTR GetPathName() { return m_szFileName; };
	BOOL GetProfileStruct(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPVOID lpStruct, UINT uSizeStruct);
	UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpKeyName, INT nDefault);
	DWORD GetProfileString(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCSTR lpReturnedString, DWORD nSize);
	DWORD GetProfileSection(LPCTSTR lpszSection, LPCSTR lpReturnedString, DWORD nSize);
	DWORD GetProfileSectionNames(LPCSTR lpszReturnBuffer, DWORD nSize);
	BOOL WriteProfileStruct(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPVOID lpStruct, UINT uSizeStruct);
	BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPCTSTR lpString);
	BOOL WriteProfileSection(LPCTSTR lpszSection, LPCTSTR lpString);
	void SetFileName(LPCTSTR lpFileName);
	BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpKeyName, int nVal);

	CBIniFile(LPCTSTR lpFileName);
	CBIniFile();
	virtual ~CBIniFile();

	DWORD GetProfileStringW(LPCWSTR lpszSection, LPCWSTR lpKeyName, LPCWSTR lpDefault, LPWSTR lpReturnedString, DWORD nSize, LPWSTR lpFileName);

private:
	CHAR m_szFileName[MAX_PATH];
};

#endif // !defined(AFX_BINIFILE_H__695DB7A2_39EB_4476_A340_43842C207239__INCLUDED_)
