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
	CStringW(void) { };
	~CStringW(void) { };

public:
	CStringW(const CStringW& obj)			{m_str = obj.m_str;}
	CStringW(const wchar_t* p)			    {if (NULL != p) m_str = p;}
	void operator=(const CStringW& obj)	{m_str = obj.m_str;}
	void operator=(const wchar_t* p)		{if (NULL != p) m_str = p;}
	void operator=(const wchar_t p)		{if (0 != p) m_str = &p;}
	void operator=(wchar_t* p)		        {if (NULL != p) m_str = p;}
	
	CStringW& operator+=(const wchar_t* p)	{if (NULL != p) m_str += p; return (*this);}
	CStringW& operator+=(const wchar_t p)	{if (0 != p) m_str += &p; return (*this);}
	friend CStringW operator+(const CStringW& str1, const wchar_t* psz2 )

	{CStringW r; r.m_str=str1.m_str; if (NULL != psz2) {r.m_str = r.m_str + psz2;} return r;}
	bool operator==(const wchar_t* p)			{return (m_str.compare(p) == 0);}
	operator const wchar_t*()					{return m_str.c_str();}
	operator const wchar_t*()	const			{return m_str.c_str();}
	int GetLength(void)						{return m_str.length();}
	bool IsEmpty()							{return(GetLength()==0);}
	int Find(const wchar_t* pszSub, int iStart=0)	{if (NULL == pszSub) return -1; return m_str.find(pszSub, iStart);}
	int Find(const wchar_t ch, int iStart=0)	{return m_str.find(ch, iStart);}
	wchar_t GetAt( int nIndex )				{return m_str.at(nIndex);	}
	wchar_t* GetLPTSTR()				{return (wchar_t*)(m_str.c_str());	}
	void Empty( )					{ m_str.clear(); }
	
	CStringW(const wchar_t* p, int nLength)
	{
		m_str.clear();
		m_str.insert(0, p, nLength);
	}

	CStringW Mid(int iFirst, int nCount)
	{
		CStringW r; 
		try
		{
			r.m_str = m_str.substr(iFirst, nCount); 
		}
		catch(...)
		{
		}
		return r;
	}

	CStringW Mid(int iFirst)
	{
		CStringW r; 
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

	int Insert(int iIndex, const wchar_t* psz)
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

	int Insert(int iIndex, wchar_t ch)
	{
		try
		{
			m_str.insert(iIndex, 1, ch); 
		}
		catch(...)
		{
			m_str += ch;
		}
		return m_str.length();
	}

	CStringW Left( int nCount ) const
	{
		CStringW r; 
		try
		{
			size_t l = 0;

			if(nCount > m_str.length())	return r;

			for (;;)
			{
				if(l == nCount)	break;

				r += m_str[l];
				l += 1;
			}
		}
		catch(...)
		{
		}
		return r;
	}



	CStringW Right( int nCount ) const
	{
		CStringW r; 
		try
		{
			size_t l = 0;
			l = m_str.length();

			if(nCount > m_str.length())	return r;

			for (;;)
			{
				if(!nCount)		break;

				r += m_str[l - (nCount)];
				nCount -= 1;
			}
		}
		catch(...)
		{
		}
		return r;
	}
	
	CStringW& Trim()
	{
		size_t l = 0;
		l = m_str.length();

		for (;;)
		{
			if (0 == l) break;

			if (m_str[l-1] != L'\t' && m_str[l-1] != L' ')
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
			if (m_str[0] != L'\t' && m_str[0] != L' ')
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

	CStringW& TrimRight(wchar_t chTarget)
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

	CStringW& TrimRight(const wchar_t* pszTargets)
	{
		size_t l = 0;

		if (NULL == pszTargets) return (*this);

		l = m_str.length();

		for (;;)
		{
			if (0 == l) break;

			if (NULL == wcschr(pszTargets, m_str[l-1]))
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

	CStringW& TrimLeft(wchar_t chTarget)
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

	CStringW& TrimLeft(const wchar_t* pszTargets)
	{
		if (NULL == pszTargets) return (*this);

		for (;;)
		{
			if (NULL == wcschr(pszTargets, m_str[0]))
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

	int Replace(const wchar_t* pszOld, const wchar_t* pszNew)
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
					l = wcslen(pszOld);
					if (0 == l) return 0;
				}
				m_str.replace(offset, l, pszNew);
				offset += l;
				r++;
			}
		}
		return r;
	}

	void Format(const wchar_t* pszFormat, ...)
	{
		int		len	= 0;
		va_list	vl;

		va_start(vl, pszFormat);

		// 포맷의 길이를 구한다.
		len = vswprintf(NULL, 0, pszFormat, vl) + 1;

		// 버퍼의 크기를 조정한다.
		m_str.resize(len, '\0');

		// 포맷을 적용한다.
		va_start(vl, pszFormat);
		if (vswprintf(&m_str[0], len, pszFormat, vl) > len)
		{
			// Error
		}
		va_end(vl);
	}

	int Compare( const wchar_t* lpsz ) const
	{
		return wcscmp(m_str.c_str(), lpsz);
	}

	int CompareNoCase( const wchar_t* lpsz ) const
	{
		wchar_t* lpOr = (wchar_t*)m_str.c_str();

		while(*lpOr && *lpsz)
		{
			wchar_t wchOr = towlower(*lpOr);
			wchar_t wchTa = towlower(*lpsz);

			if(wchOr > wchTa)	return -1;
			if(wchOr < wchTa)	return 1;

			lpOr += 1; lpsz += 1;
		}

		if(*lpOr)	return -1;
		if(*lpsz)	return 1;
		return 0;
	}

	void MakeLower( )
	{
		int nOrLen = m_str.size();

		int nIdx = 0;
		while(nIdx < nOrLen)
		{
			wchar_t chOr = m_str.at(nIdx);
			chOr = towlower(chOr);

			m_str.replace(nIdx, 1, &chOr);

			nIdx += 1;
		}
		return;
	}

	bool operator<(const CStringW& obj)	const
	{
		int nOrLen = m_str.size();
		int nTaLen = obj.m_str.size();

		int nIdx = 0;
		while(nIdx < nOrLen && nIdx < nTaLen)
		{
			wchar_t chOr = m_str.at(nIdx);
			wchar_t chTa = obj.m_str.at(nIdx);

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
	std::wstring m_str;
};

#endif //ASI_LNX_STRINGW_H_938EF200_F60B_452A_BD30_A10E8507EDCC