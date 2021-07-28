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
#include "CompMemToken.h"
//---------------------------------------------------------------------------

                                                    
CompMemToken::CompMemToken()
{
    m_pBuf = NULL ;
    m_nPos = 0;
    m_nBufLen = 0 ;
    m_bAutoMem      = FALSE ;
    m_bDataReceived = FALSE ;
}

CompMemToken::CompMemToken(INT32 aInitialSize )
{
    if( aInitialSize < 10 )
        aInitialSize = 10 ;

    m_nPos = 0 ;
    m_nBufLen = aInitialSize ;
	if(m_nBufLen > 0)
	{
    	m_pBuf = (BYTE *)malloc(aInitialSize);
	}

    m_bAutoMem = TRUE;
    m_bDataReceived = FALSE;
}


CompMemToken::CompMemToken(void *aSrc, INT32 aLen)
{
    m_pBuf = (BYTE *)aSrc;
    m_nPos = 0 ;
    m_nBufLen       = aLen ;
    m_bAutoMem      = FALSE ;
    m_bDataReceived = FALSE ;
}

CompMemToken::~CompMemToken()
{
    if( m_bAutoMem && !m_bDataReceived )
    {
        safe_free(m_pBuf); 
    }
    m_nPos = 0;
    m_nBufLen = 0;
}

void CompMemToken::Set(void *aSrc, INT32 aLen)
{
    if( m_bAutoMem && !m_bDataReceived )
    {
        safe_free(m_pBuf);
    }

    m_pBuf = (BYTE *)aSrc;
    m_nPos          = 0;
    m_nBufLen       = aLen;
    m_bAutoMem      = FALSE;
    m_bDataReceived = FALSE;
}

void CompMemToken::Set(INT32 aInitialSize)
{
    if( m_bAutoMem && !m_bDataReceived )
    {
        safe_free(m_pBuf);
    }
    
    m_nPos          = 0 ;
    m_nBufLen       = aInitialSize ;
	if(m_nBufLen > 0)
	{
    	m_pBuf = (BYTE *)malloc(aInitialSize);
	}
    m_bAutoMem      = TRUE;
    m_bDataReceived = FALSE;
}

void	CompMemToken::Clear()
{
	if(m_bAutoMem)
	{
		m_nPos = 0 ;
		if(m_pBuf != NULL)
			memset(m_pBuf, 0, m_nBufLen);
	}
}

// 입력된 바이트 만큼을 넣는다
BOOL CompMemToken::_TokenAdd(void *aSrc, INT32 aLen)
{
    BYTE *pBuf = NULL;
    
    if( m_pBuf == NULL || aSrc == NULL || aLen < 1)
        return FALSE ;

    if( m_nPos + aLen > m_nBufLen )
    {
        if( m_bAutoMem )
        {
            while( m_nPos + aLen > m_nBufLen )
            {
                pBuf = (BYTE *)malloc(m_nBufLen*2);
				if(pBuf == NULL)
					return FALSE;
                memcpy(pBuf, m_pBuf, m_nBufLen) ;
                safe_free(m_pBuf);
                
                m_pBuf = pBuf;
                m_nBufLen *= 2;
                continue ;
            }
        }
        else
        {
            return FALSE ;
        }
    }

    memcpy(&m_pBuf[m_nPos], aSrc, aLen) ;
    m_nPos += aLen ;
    return TRUE ;    
}

// 입력된 바이트 만큼을 뺀다
BOOL CompMemToken::_TokenDel(void *aDest, INT32 aLen)
{
    if( m_pBuf == NULL || aDest == NULL || aLen < 1)
        return FALSE ;

    if( m_nPos + aLen > m_nBufLen )
        return FALSE ;

    memcpy(aDest, &m_pBuf[m_nPos], aLen);
    m_nPos += aLen ;
    return TRUE ;    
}

BOOL CompMemToken::TokenAdd_16(INT16 aSrc)
{
	INT16 a = ntohs(aSrc);
	if(!_TokenAdd(&a, 2))
		return FALSE;
	return TRUE;
}

BOOL CompMemToken::TokenAdd_32(INT32 aSrc)
{
	INT32 a = ntohl(aSrc);
	if(!_TokenAdd(&a, 4))
		return FALSE;
	return TRUE;
}

BOOL CompMemToken::TokenAdd_64(INT64 aSrc)
{
	if(!_TokenAdd(&aSrc, 8))
		return FALSE;
	return TRUE;
}

BOOL CompMemToken::TokenAdd_String(String aSrc)
{
    INT16 nLen = aSrc.length() ;
    if( !TokenAdd_16( nLen ) )
        return FALSE ;
    if( !_TokenAdd((PVOID)aSrc.c_str(), nLen) )
        return FALSE ;

    return TRUE ;
}

BOOL CompMemToken::TokenAdd_StringW(StringW aSrc)
{
	INT16 nLen = aSrc.length() * sizeof(wchar_t) ;
	if( !TokenAdd_16( nLen ) )
		return FALSE ;
	if( !_TokenAdd( (PVOID)aSrc.c_str(), nLen) )
		return FALSE ;

	return TRUE ;
}

BOOL CompMemToken::TokenAdd_PVOID(PVOID aSrc, INT32 nLen)
{
    if( !TokenAdd_32( nLen ) )
        return FALSE ;
    if( !_TokenAdd( aSrc, nLen) )
        return FALSE ;

    return TRUE ;
}

BOOL CompMemToken::TokenDel_16(INT16& aSrc)
{
	INT16 a = 0;
	if(!_TokenDel(&a, 2))
		return FALSE;

	aSrc = ntohs(a);
	return TRUE;
}

BOOL CompMemToken::TokenDel_16(UINT16& aSrc)
{
	INT16 a = 0;
	if(!_TokenDel(&a, 2))
		return FALSE;

	aSrc = ntohs(a);
	return TRUE;
}

BOOL CompMemToken::TokenDel_32(INT32& aSrc)
{
	INT32 a = 0;
	if(!_TokenDel(&a, 4))
		return FALSE;

	aSrc = ntohl(a);
	return TRUE;
}

BOOL CompMemToken::TokenDel_32(UINT32& aSrc)
{
	INT32 a = 0;
	if(!_TokenDel(&a, 4))
		return FALSE;

	aSrc = ntohl(a);
	return TRUE;
}

BOOL CompMemToken::TokenDel_64(INT64& aSrc)
{
	if(!_TokenDel(&aSrc, 8))
		return FALSE;
	return TRUE;
}

BOOL CompMemToken::TokenDel_64(UINT64& aSrc)
{
	if(!_TokenDel(&aSrc, 8))
		return FALSE;
	return TRUE;
}

INT16 CompMemToken::TokenDel_String(String& aSrc)
{
    INT16 nLen = 0;

    if( !TokenDel_16( nLen ) )
        return -1;

	if(nLen < 1)
		return -2;
	
	char* pszBuf = (char*)malloc(nLen + 1);
	if(pszBuf == NULL)
		return -3;
	
	memset(pszBuf, 0, nLen+1);

    if( !_TokenDel( (PVOID)pszBuf, nLen) )
	{
		safe_free(pszBuf);
        return -4;
	}

//	aSrc = String(pszBuf);
	aSrc = pszBuf;
	safe_free(pszBuf);

    return nLen;
}

INT16 CompMemToken::TokenDel_StringW(StringW& aSrc)
{
	INT16 nLen = 0;

	if( !TokenDel_16( nLen ) )
		return -1 ;

	if(nLen < 2)
		return -2;

	char* pszBuf = (char*)malloc(nLen + 2);
	if(pszBuf == NULL)
		return -3;
	
	memset(pszBuf, 0, nLen+2);

	if( !_TokenDel( (PVOID)pszBuf, nLen) )
	{
		safe_free(pszBuf);
		return -4;
	}

//	aSrc = StringW((PWCHAR)pszBuf);
	safe_free(pszBuf);

	return nLen;
}

INT32 CompMemToken::TokenDel_PVOID(PVOID aSrc, INT32 nSrcLen)
{
    INT32 nLen = 0;

	if(aSrc == NULL || nSrcLen < 1)
		return -1;
	
    if( !TokenDel_32( nLen ) )
        return -2;

	if(nLen < 1)
		return -3;

	char* pszBuf = (char*)malloc(nLen + 1);
	if(pszBuf == NULL)
		return -4;
		
	memset(pszBuf, 0, nLen+1);

    if( !_TokenDel( (PVOID)pszBuf, nLen) )
	{
		safe_free(pszBuf);
        return -5;
	}

	memcpy(aSrc, pszBuf, nSrcLen);
	safe_free(pszBuf);

    return nLen;
}

PVOID CompMemToken::GetData()
{
    return m_pBuf;
}

INT32 CompMemToken::GetLength()
{
    return m_nPos;
}  

PVOID   CompMemToken::GetRData()
{
	if(m_pBuf == NULL || m_nPos < 0)
		return NULL;
    return (PVOID)&m_pBuf[m_nPos];
}

INT32   CompMemToken::GetRLength()
{
    return m_nBufLen - m_nPos;
}
 
BOOL CompMemToken::IsMoreToken()
{
    if(m_nPos == m_nBufLen)
        return FALSE;
    return TRUE;
}
