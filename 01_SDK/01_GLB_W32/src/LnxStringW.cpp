

#include "stdafx.h"
#include <LnxStringW.h>


CStringW::CStringW(void) 
{
	InitNaming();
}

CStringW::~CStringW(void) 
{

}
//----------------------------------------------------------------------------------

void CStringW::InitNaming()
{
	memset(m_szClsName, 0, 30*sizeof(wchar_t));
	wcscpy(m_szClsName, ASHIN_STRING_CLS_NAME_W);
}
//----------------------------------------------------------------------------------

CStringW::CStringW(const CStringW& obj)			
{
	InitNaming();

	m_str = obj.m_str;
}
//----------------------------------------------------------------------------------

CStringW::CStringW(const wchar_t* p)
{
	InitNaming();

	if (NULL != p) 
		m_str = p;
}
//----------------------------------------------------------------------------------

CStringW::CStringW(const wchar_t p)
{
	InitNaming();

	if (0 != p) 
		m_str = p;
}
//----------------------------------------------------------------------------------

CStringW::CStringW(const wchar_t* p, int nLength)
{
	InitNaming();

	m_str.clear();
	m_str.insert(0, p, nLength);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

wchar_t CStringW::operator[](int nIdx) const
{		
	return m_str.c_str()[nIdx];
}
//----------------------------------------------------------------------------------

void CStringW::operator=(const CStringW& obj)	
{
	InitNaming();

	m_str = obj.m_str;
}
//----------------------------------------------------------------------------------

void CStringW::operator=(const wchar_t* p)	
{
	InitNaming();

	if (NULL != p) 
		m_str = p;
}
//----------------------------------------------------------------------------------

void CStringW::operator=(const wchar_t p)	
{
	InitNaming();

	if (0 != p) 
	{
		wchar_t pBuf[2] = {0, };
		pBuf[0] = p;

		m_str = pBuf;
	}
}

void CStringW::operator=(wchar_t* p)		   
{
	InitNaming();

	if (NULL != p) 
		m_str = p;
}
//----------------------------------------------------------------------------------
	
CStringW& CStringW::operator+=(const wchar_t* p)	
{
	if (NULL != p) 
		m_str += p; 
	return (*this);
}
//----------------------------------------------------------------------------------

CStringW& CStringW::operator+=(const wchar_t p)	
{
	if (0 != p)
	{
		m_str.push_back(p);
	}
	return (*this);
}
//----------------------------------------------------------------------------------

bool CStringW::operator==(const wchar_t* p)			
{
	return (m_str.compare(p) == 0);
}
//----------------------------------------------------------------------------------

CStringW::operator const wchar_t*()					
{
	return m_str.c_str();
}
//----------------------------------------------------------------------------------

CStringW::operator const wchar_t*()	const			
{
	return m_str.c_str();
}
//----------------------------------------------------------------------------------

bool CStringW::operator<(const CStringW& obj)	const
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
//----------------------------------------------------------------------------------

CStringW operator+(const CStringW& str1, const wchar_t* psz2 )
{
	CStringW r; 
	r.m_str=str1.m_str; 
	if (NULL != psz2) 
	{
		r.m_str = r.m_str + psz2;
	} 
	return r;
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

int CStringW::GetLength(void)						
{	
	return wcslen(m_str.c_str());
	//return m_str.length();
}
//----------------------------------------------------------------------------------

bool CStringW::IsEmpty()							
{
	return(GetLength()==0);
}
//----------------------------------------------------------------------------------

int CStringW::Find(const wchar_t* pszSub, int iStart)
{
	if (NULL == pszSub) 
		return -1; 
	return m_str.find(pszSub, iStart);
}
//----------------------------------------------------------------------------------

int CStringW::Find(const wchar_t ch, int iStart)	
{
	return m_str.find(ch, iStart);
}
//----------------------------------------------------------------------------------

int CStringW::ReverseFind(const wchar_t* pszSub, int iStart)	
{
	if (NULL == pszSub) 
		return -1; 
	return m_str.rfind(pszSub, iStart);
}
//----------------------------------------------------------------------------------

int CStringW::ReverseFind(const wchar_t ch, int iStart)	
{
	return m_str.rfind(ch, iStart);
}
//----------------------------------------------------------------------------------

wchar_t CStringW::GetAt( int nIndex )				
{
	return m_str.at(nIndex);	
}
//----------------------------------------------------------------------------------

wchar_t* CStringW::GetLPTSTR()				
{
	return (wchar_t*)(m_str.c_str());	
}
//----------------------------------------------------------------------------------

void CStringW::Empty( )				
{ 
	m_str.clear(); 
}
//----------------------------------------------------------------------------------
	
CStringW CStringW::Mid(int iFirst, int nCount)
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
//----------------------------------------------------------------------------------

CStringW CStringW::Mid(int iFirst)
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
//----------------------------------------------------------------------------------

int CStringW::Delete(int iIndex, int nCount)
{
	try
	{
		m_str.erase(iIndex, nCount); 
	}
	catch(...)
	{
	}
	return GetLength();
}
//----------------------------------------------------------------------------------

int CStringW::Remove(wchar_t ch )
{
	std::wstring str;
	int nOrLen = m_str.size();

	int nRmNum = 0;
	int nIdx = 0;
	while(nIdx < nOrLen)
	{
		wchar_t chOr = m_str.at(nIdx);
		if(chOr != ch)	
		{
			str += chOr;
		}
		else
		{
			nRmNum += 1;
		}

		nIdx += 1;
	}

	m_str = str;
	return nRmNum;
}
//----------------------------------------------------------------------------------

int CStringW::Insert(int iIndex, const wchar_t* psz)
{
	try
	{
		if (NULL != psz) m_str.insert(iIndex, psz); 
	}
	catch(...)
	{
		m_str += psz;
	}

	return GetLength();
}
//----------------------------------------------------------------------------------

int CStringW::Insert(int iIndex, wchar_t ch)
{
	try
	{
		m_str.insert(iIndex, 1, ch); 
	}
	catch(...)
	{
		m_str += ch;
	}
	return GetLength();
}
//----------------------------------------------------------------------------------

CStringW CStringW::Left( int nCount ) const
{
	CStringW r; 
	try
	{
		size_t l = 0;

		if(nCount > m_str.length())	return r;

		for (;;)
		{
			if(l == (size_t)nCount)	break;

			r += m_str[l];
			l += 1;
		}
	}
	catch(...)
	{
	}
	return r;
}
//----------------------------------------------------------------------------------

CStringW CStringW::Right( int nCount ) const
{
	CStringW r; 
	try
	{
		size_t l = 0;
		l = m_str.length();

		if(nCount > l)	return r;

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
//----------------------------------------------------------------------------------
	
CStringW& CStringW::Trim()
{
	size_t l = 0;
	l = GetLength();

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
//----------------------------------------------------------------------------------

CStringW& CStringW::TrimRight(wchar_t chTarget)
{
	size_t l = 0;
	l = GetLength();

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
//----------------------------------------------------------------------------------

CStringW& CStringW::TrimRight(const wchar_t* pszTargets)
{
	size_t l = 0;

	if (NULL == pszTargets) return (*this);

	l = GetLength();

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
//----------------------------------------------------------------------------------

CStringW& CStringW::TrimLeft(wchar_t chTarget)
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
//----------------------------------------------------------------------------------

CStringW& CStringW::TrimLeft(const wchar_t* pszTargets)
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
//----------------------------------------------------------------------------------

int CStringW::Replace(const wchar_t* pszOld, const wchar_t* pszNew)
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
//----------------------------------------------------------------------------------

int CStringW::Replace(wchar_t pszOld, wchar_t pszNew)
{
	wchar_t pszBufO[2] = {0, };
	wchar_t pszBufN[2] = {0, };

	pszBufO[0] = pszOld;
	pszBufN[0] = pszNew;
	
	return Replace((const wchar_t*)pszBufO, (const wchar_t*)pszBufN);
}
//----------------------------------------------------------------------------------

INT32 CStringW::IsString(wchar_t* szBuf)
{
	INT32 nMaxLen = wcslen(ASHIN_STRING_CLS_NAME_W);
	wchar_t* szOrName = ASHIN_STRING_CLS_NAME_W;

	INT32 nChkIdx = 0;
	INT32 nValid = 0;

	while(nChkIdx < nMaxLen)
	{
		wchar_t chChk = szBuf[nChkIdx];
		if(!chChk)
		{
			break;
		}
		if(szOrName[nChkIdx] != szBuf[nChkIdx])
		{
			break;
		}

		nChkIdx += 1;
	}	

	if(nChkIdx == nMaxLen)	nValid = 1;

	return nValid;	
// 	INT32 nMaxLen = 10;
// 	INT32 nChkIdx = 0;
// 	INT32 nValid = 0;
// 
// 	while(nChkIdx < nMaxLen)
// 	{
// 		wchar_t chChk = szBuf[nChkIdx];
// 		if(!chChk)
// 		{
// 			break;
// 		}
// 		if(chChk < L' ' || L'~' < chChk)
// 		{
// 			nValid = 1;
// 			break;
// 		}
// 
// 		nChkIdx += 1;
// 	}	
// 	return nValid;	
}
//----------------------------------------------------------------------------------

void CStringW::Format(const wchar_t* pszFormat, ...)
{
	int		len	= 0;
	va_list	vl;

	va_start(vl, pszFormat);

	
	{
		va_list_tag_ex vt;

		CStringW strFmtOpt = L"";

		int nFmtLen = wcslen(pszFormat);
		int nChkIdx = 0;
		int nFindFmt = 0;
		while(nChkIdx < nFmtLen)
		{
			wchar_t chChk = pszFormat[nChkIdx];
			if(chChk == L'%')
			{
				if(nFindFmt)	nFindFmt = 0;
				else
				{
					memcpy(&vt, vl, sizeof(vt));//= arg_ptr[0];
					nFindFmt = 1;
				}
			}
			else if(nFindFmt == 1)
			{
				if((chChk == L's' || chChk == L'S') && nFindFmt == 1)
				{
					wchar_t* pszValue = va_arg(vl, wchar_t*);
					if(IsString(pszValue))
					{
						CStringW* strDay = (CStringW*)pszValue;
						pszValue = (wchar_t*)(strDay->GetLPTSTR());

						if(vt.gp_offset == vt.fp_offset)
							memcpy(vt.overflow_arg_area, &pszValue, sizeof(wchar_t*));
						else
							memcpy(vt.reg_save_area + vt.gp_offset, &pszValue, sizeof(wchar_t*));
					}
					nFindFmt = 0;
				}
				else if(chChk == L'd' || chChk == L'u')
				{
					if(strFmtOpt == L"I64")
						va_arg(vl, UINT64);
					else
						va_arg(vl, int);

					nFindFmt = 0;
				}
				else if(chChk == L'x' || chChk == L'X')
				{
					if(strFmtOpt == L"02")		va_arg(vl, BYTE);
					else if(strFmtOpt == L"04")	va_arg(vl, short);
					else						va_arg(vl, int);
				}
				else if(chChk == L'f' || chChk == L'u')
				{
					va_arg(vl, double);
					nFindFmt = 0;
				}
				else if(chChk == L'c' || chChk == L'C')
				{
					va_arg(vl, wchar_t);
					nFindFmt = 0;
				}
				else
				{
					strFmtOpt += chChk;
				}

				if(!nFindFmt)	strFmtOpt = L"";
			}

			nChkIdx += 1;
		}
	}


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
//----------------------------------------------------------------------------------

int CStringW::Compare( const wchar_t* lpsz ) const
{
	return wcscmp(m_str.c_str(), lpsz);
}
//----------------------------------------------------------------------------------

int CStringW::CompareNoCase( const wchar_t* lpsz ) const
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
//----------------------------------------------------------------------------------

CStringW&  CStringW::MakeLower( )
{
	int nOrLen = m_str.size();

	int nIdx = 0;
	while(nIdx < nOrLen)
	{
		wchar_t chOr = m_str.at(nIdx);
		chOr = towlower(chOr);

		//m_str.replace(nIdx, 1, chOr);
		m_str[nIdx] = chOr;

		nIdx += 1;
	}
	return (*this);
}
//----------------------------------------------------------------------------------

CStringW&  CStringW::MakeUpper( )
{
	int nOrLen = m_str.size();

	int nIdx = 0;
	while(nIdx < nOrLen)
	{
		wchar_t chOr = m_str.at(nIdx);
		chOr = towupper(chOr);

		//m_str.replace(nIdx, 1, chOr);
		m_str[nIdx] = chOr;

		nIdx += 1;
	}
	return (*this);
}
//----------------------------------------------------------------------------------