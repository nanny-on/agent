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

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#include "stdafx.h"
#include "com_struct.h"
#include "FileToken.h"
//---------------------------------------------------------------------------

                                                    
FileToken::FileToken()
{
    m_nFd = -1;
    m_nPos = 0 ;
    m_nFileLen = 0 ;    
	m_nOpenFile = 0;
}

FileToken::FileToken(String strFileName)
{
	DWORD dwSize = 0;
	m_nFd = open(strFileName.c_str(), O_RDWR | O_CREAT, 0666);
	if(m_nFd == -1)
	{
		return;
	}
	m_nOpenFile = 1;
	if(GetFileSizeWithFd(m_nFd, &dwSize) == 0)
	{
		m_nPos = 0 ;
		m_nFileLen = dwSize;	
	}
}

FileToken::FileToken(INT32 nFd)
{
	DWORD dwSize = 0;
	m_nFd = nFd;
	m_nOpenFile = 0;

	if(GetFileSizeWithFd(m_nFd, &dwSize) != 0)
	{
		m_nPos = 0 ;
		m_nFileLen = dwSize;	
	}
}

FileToken::~FileToken()
{
	if(m_nFd != -1 && m_nOpenFile)
	{
		close(m_nFd);
		m_nFd = -1;
		m_nOpenFile = 0;
	}
    m_nPos = 0 ;
    m_nFileLen = 0 ;
}

void	FileToken::Clear()
{
	if(m_nFd != -1)
	{
		close(m_nFd);
		m_nFd = -1;
	}
    m_nPos = 0 ;
    m_nFileLen = 0 ;
}

// 입력된 바이트 만큼을 넣는다
BOOL FileToken::_TokenAdd(void *aSrc, INT32 aLen)
{
	PBYTE lpBuffer = (PBYTE)aSrc;
    INT32 nOnceWrited = 0;
	INT32 nWritedSize = 0;
	INT32 nTryNum = 10;

    if(m_nFd == -1 || aSrc == NULL || aLen < 1)
        return FALSE;
	
    while(nWritedSize < aLen && nTryNum)
    {
		nOnceWrited = write(m_nFd, &lpBuffer[nWritedSize], aLen - nWritedSize);
		if(nOnceWrited == -1)
		{
			if(errno == EINTR || errno == EAGAIN)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return FALSE;
			}
		}
		if(nOnceWrited > 0)
		{
			nWritedSize += nOnceWrited;
			nTryNum -= 1;
		}
    }

    m_nPos += aLen;
	if(nTryNum == 0)
		return FALSE;
    return TRUE ;    
}

// 입력된 바이트 만큼을 뺀다
BOOL FileToken::_TokenDel(void *aDest, INT32 aLen)
{
	PBYTE pbBuffer = NULL;
	PBYTE pbDest = (PBYTE)aDest;
	INT32 nDestSize = aLen;
	INT32 nReadSize = 0;
	INT32 nBuffSize = 0;
	INT32 nTotalReadSize = 0;
	INT32 nOnceReadSize = 0;
	INT32 nTryNum = 10;
	
    if(m_nFd == -1 || aDest == NULL || aLen < 1)
        return FALSE;

	if(aLen > CHAR_MAX_SIZE)
		nBuffSize = CHAR_MAX_SIZE;
	else
		nBuffSize = aLen;

	pbBuffer = (PBYTE)malloc(nBuffSize+1);
	if(pbBuffer == NULL)
		return FALSE;
	
    while(nTotalReadSize < nDestSize && nTryNum)
    {
    	memset(pbBuffer, 0, nBuffSize+1);
        nOnceReadSize = nDestSize - nTotalReadSize;
		if(nOnceReadSize > nBuffSize)
			nOnceReadSize = nBuffSize;
        nReadSize = read(m_nFd, pbBuffer, nOnceReadSize);
		if(nReadSize == -1)
		{
			if(errno == EINTR || errno == EAGAIN)
			{
				Sleep(10);
				continue;
			}
			else
			{
				safe_free(pbBuffer);
				return FALSE;
			}
		}		
		
		if(nReadSize > 0 || nReadSize < nBuffSize+1)
		{
			memcpy(&pbDest[nTotalReadSize], pbBuffer, nReadSize);
			nTotalReadSize += nReadSize;
			nTryNum -= 1;
		}
    }

	m_nPos += aLen;
	safe_free(pbBuffer);
	if(nTryNum == 0)
		return FALSE;
    return TRUE;    
}


BOOL FileToken::TokenAdd_16(INT16 aSrc)
{
	INT16 a = ntohs(aSrc);
	if(!_TokenAdd(&a, 2))
		return FALSE;
	return TRUE;
}

BOOL FileToken::TokenAdd_32(INT32 aSrc)
{
	INT32 a = ntohl(aSrc);
	if(!_TokenAdd(&a, 4))
		return FALSE;
	return TRUE;
}

BOOL FileToken::TokenAdd_64(INT64 aSrc)
{
	if(!_TokenAdd(&aSrc, 8))
		return FALSE;
	return TRUE;
}

BOOL FileToken::TokenAdd_Char(char *acData)
{
	INT16 nLen = 0;
	
	if(acData == NULL)
		return FALSE;


	nLen = strlen(acData);
	if(nLen < 1)
	{
		nLen = 4;
		if( !TokenAdd_16( nLen ) )
			return FALSE ;
		if( !_TokenAdd((PVOID)"null", nLen) )
			return FALSE ;

	}
	else
	{
		if( !TokenAdd_16( nLen ) )
			return FALSE ;
		if( !_TokenAdd((PVOID)acData, nLen) )
			return FALSE ;
	}
	return TRUE ;
}

BOOL FileToken::TokenAdd_String(String aSrc)
{
	INT16 nLen = aSrc.length();
	if(nLen < 1)
	{
		nLen = 4;
		if( !TokenAdd_16( nLen ) )
			return FALSE ;
		if( !_TokenAdd((PVOID)"null", nLen) )
			return FALSE ;
		
	}
	else
	{
		if( !TokenAdd_16( nLen ) )
			return FALSE ;
		if( !_TokenAdd((PVOID)aSrc.c_str(), nLen) )
			return FALSE ;
	}
	return TRUE ;
}

BOOL FileToken::TokenAdd_StringW(StringW aSrc)
{
	INT16 nLen = aSrc.length() * sizeof(wchar_t);
	if(nLen < 2)
	{
		nLen = 2;
		if( !TokenAdd_16( nLen ) )
			return FALSE ;
		if( !_TokenAdd((PVOID)L"n", nLen) )
			return FALSE ;
		
	}
	else
	{
		if( !TokenAdd_16( nLen ) )
			return FALSE ;
		if( !_TokenAdd((PVOID)aSrc.c_str(), nLen))
			return FALSE ;
	}
	return TRUE ;
}


BOOL FileToken::TokenAdd_PVOID(PVOID aSrc, INT32 nLen)
{
	if(aSrc == NULL || nLen < 1)
		return FALSE;

    if( !TokenAdd_32( nLen ) )
        return FALSE ;
    if( !_TokenAdd( aSrc, nLen) )
        return FALSE ;

    return TRUE ;
}

BOOL FileToken::TokenDel_16(INT16& aSrc)
{
	INT16 a = 0;
	if(!_TokenDel(&a, 2))
		return FALSE;

	aSrc = ntohs(a);
	return TRUE;
}

BOOL FileToken::TokenDel_16(UINT16& aSrc)
{
	INT16 a = 0;
	if(!_TokenDel(&a, 2))
		return FALSE;

	aSrc = ntohs(a);
	return TRUE;
}

BOOL FileToken::TokenDel_32(INT32& aSrc)
{
	INT32 a = 0;
	if(!_TokenDel(&a, 4))
		return FALSE;

	aSrc = ntohl(a);
	return TRUE;
}

BOOL FileToken::TokenDel_32(UINT32& aSrc)
{
	INT32 a = 0;
	if(!_TokenDel(&a, 4))
		return FALSE;

	aSrc = ntohl(a);
	return TRUE;
}

BOOL FileToken::TokenDel_64(INT64& aSrc)
{
	if(!_TokenDel(&aSrc, 8))
		return FALSE;
	return TRUE;
}

BOOL FileToken::TokenDel_64(UINT64& aSrc)
{
	if(!_TokenDel(&aSrc, 8))
		return FALSE;
	return TRUE;
}

INT16 FileToken::TokenDel_String(String& aSrc)
{
    INT16 i, nLen = 0;

    if( !TokenDel_16( nLen ) )
        return -1;

	if(nLen < 1)
		return -2;

	char* pszBuff = (char*)malloc(nLen + 2);
	if(pszBuff == NULL)
		return -3;
	
	memset(pszBuff, 0, nLen+2);

    if( !_TokenDel( (PVOID)pszBuff, nLen) )
	{
		safe_free(pszBuff);
        return -4;
	}
	
	if(nLen == 4 && !_stricmp(pszBuff, "null"))
	{
		safe_free(pszBuff);
		return 0;
	}
	pszBuff[nLen] = 0;

	aSrc = pszBuff;
	safe_free(pszBuff);

    return nLen;
}

INT16 FileToken::TokenDel_Char(char *acData, INT16 nMaxLen)
{
	INT16 i, nLen = 0;

	if(acData == NULL || nMaxLen < 1)
		return -1;

	if( !TokenDel_16( nLen ) )
		return -2;

	if(nLen < 1 || nLen > nMaxLen-1)
		return -3;

	char* pszBuff = (char*)malloc(nLen + 2);
	if(pszBuff == NULL)
		return -4;

	memset(pszBuff, 0, nLen+2);

	if( !_TokenDel( (PVOID)pszBuff, nLen) )
	{
		safe_free(pszBuff);
		return -5;
	}

	if(nLen == 4 && !_stricmp(pszBuff, "null"))
	{
		safe_free(pszBuff);
		return 0;
	}
	pszBuff[nLen] = 0;

	strncpy(acData, pszBuff, nMaxLen-1);

	safe_free(pszBuff);

	return nLen;
}

INT16 FileToken::TokenDel_StringW(StringW& aSrc)
{
	INT16 nLen = 0;

	if( !TokenDel_16( nLen ) )
		return -1;

	if(nLen < 1)
		return -2;

	CHAR* pszBuff = (char*)malloc(nLen + 2);
	if(pszBuff == NULL)
		return -2;
	
	memset(pszBuff, 0, nLen+2);

	if( !_TokenDel( (PVOID)pszBuff, nLen) )
	{
		safe_free(pszBuff);
		return -4;
	}

	if(nLen == 2 && pszBuff[0] == L'n')
	{
		safe_free(pszBuff);
		return 0;
	}	

	aSrc = StringW((PWCHAR)pszBuff);
	safe_free(pszBuff);

	return nLen;
}

INT32 FileToken::TokenDel_PVOID(PVOID aSrc, INT32 nSrcLen)
{
    INT32 nLen = 0;

    if( !TokenDel_32( nLen ) )
        return -1 ;

	if(nLen < 1)
		return -2;

	char* pszBuff = (char*)malloc(nLen + 1);
	if(pszBuff == NULL)
		return -4;
	
	memset(pszBuff, 0, nLen+1);

    if( !_TokenDel( (PVOID)pszBuff, nLen) )
	{
		safe_free(pszBuff);
        return -4 ;
	}

	memcpy(aSrc, pszBuff, nSrcLen);
	safe_free(pszBuff);

    return nLen;
}

INT32 FileToken::GetFD()
{
    return m_nFd;
}

INT32 FileToken::GetLength()
{
    return m_nPos;
}  
 
BOOL FileToken::IsMoreToken()
{
    if(m_nPos == m_nFileLen)
        return FALSE;
    return TRUE;
}
