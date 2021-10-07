

#include "stdafx.h"
#include <LnxString.h>

CString::CString(void) 
{
	InitNaming();
}

CString::~CString(void) 
{

}

void CString::InitNaming()
{
	memset(m_szClsName, 0, 30);
	strcpy(m_szClsName, ASHIN_STRING_CLS_NAME);
}
//----------------------------------------------------------------------------------

CString::CString(const CString& obj)
{
	InitNaming();
	m_str = obj.m_str;
}
//----------------------------------------------------------------------------------

CString::CString(const char* p)			    
{
	InitNaming();

	if (NULL != p) 
		m_str = p;
}
//----------------------------------------------------------------------------------

CString::CString(const char p)
{
	InitNaming();

	if (0 != p) 
	{
		char pBuf[2] = {0, };
		pBuf[0] = p;

		m_str = pBuf;
	}
}
//----------------------------------------------------------------------------------

CString::CString(const char* p, int nLength)
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

char CString::operator[](int nIdx) const
{		
	return m_str.c_str()[nIdx];
}
//----------------------------------------------------------------------------------

void CString::operator=(const CString& obj)	
{
	InitNaming();

	m_str = obj.m_str;
}
//----------------------------------------------------------------------------------

void CString::operator=(const char* p)		
{
	InitNaming();

	if (NULL != p) 
		m_str = p;
}
//----------------------------------------------------------------------------------

void CString::operator=(const char p)		
{
	InitNaming();

	if (0 != p) 
		m_str = &p;
}
//----------------------------------------------------------------------------------

void CString::operator=(char* p)		   
{
	InitNaming();

	if (NULL != p) 
		m_str = p;
}
//----------------------------------------------------------------------------------

CString& CString::operator+=(const char* p)	
{
	if (NULL != p) 
		m_str += p; 
	return (*this);
}
//----------------------------------------------------------------------------------

CString& CString::operator+=(const char p)	
{
	if (0 != p) 
	{
		m_str.push_back(p);
	}
	return (*this);
}
//----------------------------------------------------------------------------------

bool CString::operator==(const char* p)			
{
	return (m_str.compare(p) == 0);
}
//----------------------------------------------------------------------------------

CString::operator char*() 
{
	return (char*)m_str.c_str(); 
}
//----------------------------------------------------------------------------------

CString::operator const char*()					
{
	return m_str.c_str();
}
//----------------------------------------------------------------------------------

CString::operator const char*()	const			
{
	return m_str.c_str();
}
//----------------------------------------------------------------------------------

bool CString::operator<(const CString& obj)	const
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
//----------------------------------------------------------------------------------

CString operator+(const CString& str1, const char* psz2 )	
{
	CString r; 
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

int CString::GetLength(void)						
{
	//return m_str.length();
	return strlen(m_str.c_str());
}
//----------------------------------------------------------------------------------

bool CString::IsEmpty()
{
	return(GetLength()==0);
}
//----------------------------------------------------------------------------------

int CString::Find(const char* pszSub, int iStart)	
{
	if (NULL == pszSub) 
		return -1; 
	return m_str.find(pszSub, iStart);
}
//----------------------------------------------------------------------------------

int CString::Find(const char ch, int iStart)	
{
	return m_str.find(ch, iStart);
}
//----------------------------------------------------------------------------------

int CString::ReverseFind(const char* pszSub, int iStart)	
{
	if (NULL == pszSub) 
		return -1; 
	return m_str.rfind(pszSub, iStart);
}
//----------------------------------------------------------------------------------

int CString::ReverseFind(const char ch, int iStart)	
{
	return m_str.rfind(ch, iStart);
}
//----------------------------------------------------------------------------------

char CString::GetAt( int nIndex )
{
	return m_str.at(nIndex);	
}
//----------------------------------------------------------------------------------

char* CString::GetLPTSTR()				
{
	return (char*)(m_str.c_str());	
}
//----------------------------------------------------------------------------------

void CString::Empty( )					
{ 
	m_str.clear(); 
}
//----------------------------------------------------------------------------------

CString CString::Mid(int iFirst, int nCount)
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
//----------------------------------------------------------------------------------

CString CString::Mid(int iFirst)
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
//----------------------------------------------------------------------------------
	
int CString::Delete(int iIndex, int nCount)
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

int CString::Remove(char ch )
{
	std::string str;
	int nOrLen = m_str.size();

	int nRmNum = 0;
	int nIdx = 0;
	while(nIdx < nOrLen)
	{
		char chOr = m_str.at(nIdx);
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

int CString::Insert(int iIndex, const char* psz)
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

int CString::Insert(int iIndex, char ch)
{
	try
	{
		m_str.insert(iIndex, 1, ch); 
	}
	catch(...)
	{
		m_str += ch;
	}
	return (int)GetLength();
}
//----------------------------------------------------------------------------------

CString CString::Left( int nCount ) const
{
	CString r; 
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

CString CString::Right( int nCount ) const
{
	CString r; 
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

CString& CString::Trim()
{
	size_t l = 0;
	l = GetLength();

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
//----------------------------------------------------------------------------------

CString& CString::TrimRight(char chTarget)
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

CString& CString::TrimRight(const char* pszTargets)
{
	size_t l = 0;

	if (NULL == pszTargets) return (*this);

	l = GetLength();

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
//----------------------------------------------------------------------------------

CString& CString::TrimLeft(char chTarget)
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

CString& CString::TrimLeft(const char* pszTargets)
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
//----------------------------------------------------------------------------------

int CString::Replace(const char* pszOld, const char* pszNew)
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
//----------------------------------------------------------------------------------

int CString::Replace(char pszOld, char pszNew)
{
	char pszBufO[2] = {0, };
	char pszBufN[2] = {0, };

	pszBufO[0] = pszOld;
	pszBufN[0] = pszNew;

	return Replace((const char*)pszBufO, (const char*)pszBufN);	
}
//----------------------------------------------------------------------------------

INT32 CString::IsString(char* szBuf)
{
	INT32 nMaxLen = strlen(ASHIN_STRING_CLS_NAME);
	char* szOrName = ASHIN_STRING_CLS_NAME;

	INT32 nChkIdx = 0;
	INT32 nValid = 0;

	while(nChkIdx < nMaxLen)
	{
		char chChk = szBuf[nChkIdx];
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
// 		char chChk = szBuf[nChkIdx];
// 		if(!chChk)
// 		{
// 			break;
// 		}
// 		if(chChk < ' ' || '~' < chChk)
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

void CString::Format(const char* pszFormat, ...)
{
	int		len	= 0;
	va_list	vl;

	va_start(vl, pszFormat);

	{
		va_list_tag_ex vt;	

		CString strFmtOpt = "";

		int nFmtLen = strlen(pszFormat);
		int nChkIdx = 0;
		int nFindFmt = 0;
		while(nChkIdx < nFmtLen)
		{
			char chChk = pszFormat[nChkIdx];
			if(chChk == '%')
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
				if((chChk == 's' || chChk == 'S') && nFindFmt == 1)
				{
					char* pszValue = va_arg(vl, char*);					
					if(IsString(pszValue))
					{
						CString* strDay = (CString*)pszValue;
						pszValue = (char*)(strDay->GetLPTSTR());

						if(vt.gp_offset == vt.fp_offset)
							memcpy(vt.overflow_arg_area, &pszValue, sizeof(char*));
						else
							memcpy(vt.reg_save_area + vt.gp_offset, &pszValue, sizeof(char*));
					}
					nFindFmt = 0;
				}
				else if(chChk == 'd' || chChk == 'u')
				{
					if(strFmtOpt == "I64")
						va_arg(vl, UINT64);
					else
						va_arg(vl, int);

					nFindFmt = 0;
				}
				else if(chChk == 'x' || chChk == 'X')
				{
					if(strFmtOpt == "02")		va_arg(vl, BYTE);
					else if(strFmtOpt == "04")	va_arg(vl, short);
					else						va_arg(vl, int);
				}
				else if(chChk == 'f' || chChk == 'u')
				{
					va_arg(vl, double);
					nFindFmt = 0;
				}
				else if(chChk == 'c' || chChk == 'C')
				{
					va_arg(vl, char);
					nFindFmt = 0;
				}
				else
				{
					strFmtOpt += chChk;
				}

				if(!nFindFmt)	strFmtOpt = "";
			}

			nChkIdx += 1;
		}
	}


	va_start(vl, pszFormat);
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
//----------------------------------------------------------------------------------

int CString::Compare( LPCTSTR lpsz ) const
{
	return strcmp(m_str.c_str(), lpsz);
}
//----------------------------------------------------------------------------------

int CString::CompareNoCase( LPCTSTR lpsz ) const
{
	return _stricmp(m_str.c_str(), lpsz);
}
//----------------------------------------------------------------------------------

CString& CString::MakeLower( )
{
	int nOrLen = m_str.size();
		
	int nIdx = 0;
	while(nIdx < nOrLen)
	{
		char chOr = m_str.at(nIdx);
		chOr = TOLOWER(chOr);

		//m_str.replace(nIdx, 1, chOr);
		m_str[nIdx] = chOr;
			
		nIdx += 1;
	}
	return (*this);
}
//----------------------------------------------------------------------------------

CString& CString::MakeUpper( )
{
	int nOrLen = m_str.size();

	int nIdx = 0;
	while(nIdx < nOrLen)
	{
		char chOr = m_str.at(nIdx);
		chOr = TOUPPER(chOr);

		//m_str.replace(nIdx, 1, chOr);
		m_str[nIdx] = chOr;

		nIdx += 1;
	}
	return (*this);
}


CString& CString::ConvertWinToUnix()
{
	int nOrLen = m_str.size();
	int nIdx = 0;
	char chOr = 0;
	if(nOrLen < 1)
		return (*this);
	chOr = m_str.at(0);
	if(chOr != '\\')
		return (*this);
	while(nIdx < nOrLen)
	{
		chOr = m_str.at(nIdx);
		if(chOr == '\\')
			chOr = '/';
		m_str[nIdx] = chOr;
		nIdx += 1;
	}
	return (*this);
}
//----------------------------------------------------------------------------------