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
	CString(void) { };
	~CString(void) { };

public:
	CString(const CString& obj)			{m_str = obj.m_str;}
	CString(const char* p)			    {if (NULL != p) m_str = p;}
	CString(const char p)			    {if (0 != p) m_str = p;}

	void operator=(const CString& obj)	{m_str = obj.m_str;}
	void operator=(const char* p)		{if (NULL != p) m_str = p;}
	void operator=(const char p)		{char acChar[2] = {0,};acChar[0] = p; if (0 != p) m_str = acChar;}
	void operator=(char* p)		        {if (NULL != p) m_str = p;}

	CString& operator+=(const char* p)	{if (NULL != p) m_str += p; return (*this);}
	CString& operator+=(const char p)	{char acChar[2] = {0,}; acChar[0] = p; if (0 != p) m_str += acChar; return (*this);}
	friend CString operator+(const CString& str1, const char* psz2 )
	
	{CString r; r.m_str=str1.m_str; if (NULL != psz2) {r.m_str = r.m_str + psz2;} return r;}
	bool operator==(const char* p)			{return (m_str.compare(p) == 0);}
	operator const char*()					{return m_str.c_str();}
	operator const char*()	const			{return m_str.c_str();}
	int GetLength(void)						{return m_str.length();}
	bool IsEmpty()							{return(GetLength()==0);}
	int Find(const char* pszSub, int iStart=0)	{if (NULL == pszSub) return -1; return m_str.find(pszSub, iStart);}
	int Find(const char ch, int iStart=0)	{return m_str.find(ch, iStart);}
	int ReverseFind(const char* pszSub, int iStart=0)	{if (NULL == pszSub) return -1; return m_str.rfind(pszSub, iStart);}
	int ReverseFind(const char ch, int iStart=0)	{return m_str.rfind(ch, iStart);}
	char GetAt( int nIndex )				{return m_str.at(nIndex);	}
	char* GetLPTSTR()				{return (char*)(m_str.c_str());	}
	void Empty( )					{ m_str.clear(); }
	
	CString(const char* p, int nLength)
	{
		m_str.clear();
		m_str.insert(0, p, nLength);
	}

	CString Mid(int iFirst, int nCount)
	{
		CString r; 
		try
		{
			r.m_str = m_str.substr(iFirst, nCount); 
		}
		catch(...)
		{
		}
		return r;
	}

	CString Mid(int iFirst)
	{
		CString r; 
		try
		{
			r.m_str = m_str.substr(iFirst); 
		}
		catch(...)
		{
		}
		return r;
	}
	
	int Delete(int iIndex, int nCount = 1)
	{
		try
		{
			m_str.erase(iIndex, nCount); 
		}
		catch(...)
		{
		}
		return m_str.length();
	}

	int Insert(int iIndex, const char* psz)
	{
		try
		{
			if (NULL != psz) m_str.insert(iIndex, psz); 
		}
		catch(...)
		{
			m_str += psz;
		}

		return m_str.length();
	}

	int Insert(int iIndex, char ch)
	{
		char acChar[2] = {0,};
		acChar[0] = ch;
		try
		{
			m_str.insert(iIndex, 1, acChar[0]); 
		}
		catch(...)
		{
			m_str += acChar;
		}
		return (int)m_str.length();
	}

	CString Left( int nCount ) const
	{
		CString r; 
		try
		{
			size_t l = 0;
			char acChar[2] = {0,};
			if(nCount > (int)m_str.length())	return r;

			for (;;)
			{
				if(l == (size_t)nCount)	break;

				acChar[0] = m_str[l];
				r += acChar;
				l += 1;
			}
		}
		catch(...)
		{
		}
		return r;
	}



	CString Right( int nCount ) const
	{
		CString r; 
		try
		{
			size_t l = 0;
			char acChar[2] = {0,};
			l = m_str.length();

			if(nCount > (int)m_str.length())	return r;

			for (;;)
			{
				if(!nCount)		break;
				acChar[0] = m_str[l - (nCount)];
				r += acChar;
				nCount -= 1;
			}
		}
		catch(...)
		{
		}
		return r;
	}

	CString& Trim()
	{
		size_t l = 0;
		l = m_str.length();

		for (;;)
		{
			if (0 == l) break;

			if (m_str[l-1] != '\t' && m_str[l-1] != ' ')
			{
				break;
			}
			else
			{
				m_str.erase(l-1, 1);
				l--;
			}
		}

		for (;;)
		{
			if (m_str[0] != '\t' && m_str[0] != ' ')
			{
				break;
			}
			else
			{
				m_str.erase(0, 1);
			}
		}

		return (*this);
	}

	CString& TrimRight(char chTarget)
	{
		size_t l = 0;
		l = m_str.length();

		for (;;)
		{
			if (0 == l) break;

			if (m_str[l-1] != chTarget)
			{
				break;
			}
			else
			{
				m_str.erase(l-1, 1);
				l--;
			}
		}

		return (*this);
	}

	CString& TrimRight(const char* pszTargets)
	{
		size_t l = 0;

		if (NULL == pszTargets) return (*this);

		l = m_str.length();

		for (;;)
		{
			if (0 == l) break;

			if (NULL == strchr(pszTargets, m_str[l-1]))
			{
				break;
			}
			else
			{
				m_str.erase(l-1, 1);
				l--;
			}
		}

		return (*this);
	}

	CString& TrimLeft(char chTarget)
	{
		for (;;)
		{
			if (m_str[0] != chTarget)
			{
				break;
			}
			else
			{
				m_str.erase(0, 1);
			}
		}

		return (*this);
	}

	CString& TrimLeft(const char* pszTargets)
	{
		if (NULL == pszTargets) return (*this);

		for (;;)
		{
			if (NULL == strchr(pszTargets, m_str[0]))
			{
				break;
			}
			else
			{
				m_str.erase(0, 1);
			}
		}

		return (*this);
	}

	int Replace(const char* pszOld, const char* pszNew)
	{
		int r = 0;
		size_t l = 0;
		std::string::size_type offset = 0;

		if ((NULL == pszOld) || (NULL == pszNew)) return 0; 

		for (;;)
		{
			offset = m_str.find(pszOld, offset);

			if (std::string::npos == offset) 
			{
				break;
			}
			else
			{
				if (0 == l)
				{
					l = strlen(pszOld);
					if (0 == l) return 0;
				}
				m_str.replace(offset, l, pszNew);
				offset += l;
				r++;
			}
		}
		return r;
	}

	void Format(const char* pszFormat, ...)
	{
		int		len	= 0;
		va_list	vl;

		va_start(vl, pszFormat);

		// 포맷의 길이를 구한다.
		len = vsnprintf(NULL, 0, pszFormat, vl) + 1;

		// 버퍼의 크기를 조정한다.
		m_str.resize(len, '\0');

		// 포맷을 적용한다.
		va_start(vl, pszFormat);
		if (vsnprintf(&m_str[0], len, pszFormat, vl) > len)
		{
			// Error
		}
		va_end(vl);
	}

	int Compare( LPCTSTR lpsz ) const
	{
		return strcmp(m_str.c_str(), lpsz);
	}

	int CompareNoCase( LPCTSTR lpsz ) const
	{
		return _stricmp(m_str.c_str(), lpsz);
	}

	CString& MakeLower( )
	{
		int nOrLen = m_str.size();
		char acChar[2] = {0,};
		int nIdx = 0;
		while(nIdx < nOrLen)
		{
			char chOr = m_str.at(nIdx);
			acChar[0] = (char)_tolower(chOr);

			m_str.replace(nIdx, 1, acChar);
			
			nIdx += 1;
		}
		return (*this);
	}

	CString& MakeUpper( )
	{
		int nOrLen = m_str.size();
		char acChar[2] = {0,};
		int nIdx = 0;
		while(nIdx < nOrLen)
		{
			char chOr = m_str.at(nIdx);
			acChar[0] = (char)_toupper(chOr);

			m_str.replace(nIdx, 1, acChar);

			nIdx += 1;
		}
		return (*this);
	}

	bool operator<(const CString& obj)	const
	{
		int nOrLen = m_str.size();
		int nTaLen = obj.m_str.size();

		int nIdx = 0;
		while(nIdx < nOrLen && nIdx < nTaLen)
		{
			char chOr = m_str.at(nIdx);
			char chTa = obj.m_str.at(nIdx);

			if(chOr == chTa)
			{
				nIdx += 1;
				continue;
			}

			if(chOr < chTa) return true;
			return false;
		}

		if(nOrLen < nTaLen) return true;
		return false;
	}

protected:
	std::string m_str;
};

#endif //ASI_LNX_STRING_H_938EF200_F60B_452A_BD30_A10E8507EDCC