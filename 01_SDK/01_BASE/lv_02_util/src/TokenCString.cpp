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

//---------------------------------------------------------------------

#include "stdafx.h"
#include "com_struct.h"
#include "TokenCString.h"

//---------------------------------------------------------------------------

CTokenCString* t_TokenCString = NULL;

//---------------------------------------------------------------------------
CTokenCString::CTokenCString()
{
    m_pcBuffer = NULL;
    m_nStart = 0;
	m_nEnd = 0;
	m_nPos = 0;
	m_nReversPlay = 0;
    m_cTok = ',';
}
//-------------------------------------------------------------------------

CTokenCString::CTokenCString(BYTE* pbInput, INT32 nLen, char pTok, INT32 nReversPlay)
{
    m_pcBuffer = NULL;
    m_nStart = 0;
	m_nEnd = 0;
	m_nPos = 0;
	m_nReversPlay = 0;
    m_cTok = ',';
    SetBuffer((const char *)pbInput, nLen, pTok, nReversPlay);
}
//-------------------------------------------------------------------------

CTokenCString::CTokenCString(const char *pcInput, INT32 nLen, char pTok, INT32 nReversPlay)
{
    m_pcBuffer = NULL;
    m_nStart = 0;
	m_nEnd = 0;
	m_nPos = 0;
	m_nReversPlay = 0;
    m_cTok = ',';

    SetBuffer(pcInput, nLen, pTok, nReversPlay);
}
//-------------------------------------------------------------------------

CTokenCString::~CTokenCString()
{
	safe_free(m_pcBuffer);
}
//-------------------------------------------------------------------------

void    CTokenCString::SetBuffer(const char *pcInput, INT32 nLen, char cTok, INT32 nReversPlay)
{
    if(pcInput == NULL || nLen < 1)
        return;

	safe_free(m_pcBuffer);

    m_pcBuffer = (char *)malloc(nLen+1);
	if(m_pcBuffer == NULL)
		return;
	memset(m_pcBuffer, 0, nLen+1);
    strncpy(m_pcBuffer, pcInput, nLen);
	m_pcBuffer[nLen] = 0;
    m_nStart = 0;
    m_nEnd = nLen;
    m_cTok = cTok;
    m_nReversPlay = nReversPlay;
    if(m_nReversPlay)
        m_nPos = m_nEnd - 1;
    else
        m_nPos = m_nStart;
}
//-------------------------------------------------------------------------

BOOL    CTokenCString::IsNextToken()
{
    if(m_nPos >= m_nEnd || m_nPos < m_nStart)
        return FALSE;
    return TRUE;
}
//-------------------------------------------------------------------------

INT32	CTokenCString::IsNext_Line()
{
    if(m_nPos >= m_nEnd || m_nPos < m_nStart)
        return 0;
    return 1;
}
//-------------------------------------------------------------------------

CString  CTokenCString::NextToken()
{
    CString  strRtn;
    BOOL bContinue = TRUE;

    if(m_nPos >= m_nEnd || m_nPos < m_nStart || m_pcBuffer == NULL)
        return "";

    if(m_nReversPlay)
    {
        for(m_nPos; m_nPos >= m_nStart && bContinue; m_nPos--)
        {
            if(m_pcBuffer[m_nPos] == m_cTok)
            {
                bContinue = FALSE;
            }
            else
            {
                strRtn += m_pcBuffer[m_nPos];
            }
        }
        strRtn = ReverseCStr(strRtn);
    }
    else
    {
        for(m_nPos; m_nPos < m_nEnd && bContinue; m_nPos++)
        {
            if(m_pcBuffer[m_nPos] == m_cTok)
            {
                bContinue = FALSE;
            }
            else
            {
                strRtn += m_pcBuffer[m_nPos];
            }
        }
    }
    return strRtn;
}
//-------------------------------------------------------------------------

CString		CTokenCString::NextToken_Query()
{
	CString  strRtn;
	BOOL bContinue = TRUE;
	BOOL bTokenFine = FALSE;

	if(m_nPos >= m_nEnd || m_nPos < m_nStart || m_pcBuffer == NULL)
        return "";

    for(m_nPos; m_nPos < m_nEnd && bContinue; m_nPos++)
    {
    	if(bTokenFine == FALSE)
        {
            if(m_pcBuffer[m_nPos] == m_cTok)
            {
                bTokenFine = TRUE;
            }
            else
            {
                strRtn += m_pcBuffer[m_nPos];
            }
        }
        else
        {
            if(m_pcBuffer[m_nPos] != '\n' && m_pcBuffer[m_nPos] != '\r')
            {
                bContinue = FALSE;
                m_nPos--;
            }
        }
    }
    return strRtn;
}
//-------------------------------------------------------------------------

CString		CTokenCString::NextToken_Line()
{
    CString  strRtn;
    BOOL    bContinue = TRUE;
    BOOL	bTokenFine = FALSE;
	char	cFindChar = 0;

	if(m_nPos >= m_nEnd || m_nPos < m_nStart || m_pcBuffer == NULL)
        return "";

    for(m_nPos; m_nPos < m_nEnd && bContinue; m_nPos++)
    {
    	if(bTokenFine == FALSE)
        {
            if(m_pcBuffer[m_nPos] == '\n' || m_pcBuffer[m_nPos] == '\r')
            {
                bTokenFine = TRUE;
				cFindChar = m_pcBuffer[m_nPos];
            }
            else
            {
                strRtn += m_pcBuffer[m_nPos];
            }
        }
        else
        {
			if(m_pcBuffer[m_nPos] == cFindChar)
			{
				bContinue = FALSE;
                m_nPos--;
			}
            else if(m_pcBuffer[m_nPos] != '\n' && m_pcBuffer[m_nPos] != '\r')
            {
                bContinue = false;
                m_nPos--;
            }
        }
    }
    return strRtn;
}
//-------------------------------------------------------------------------

CString  CTokenCString::RemainValue()
{
    CString  strRtn;
    BOOL bContinue = TRUE;

    if(m_nPos >= m_nEnd || m_nPos < m_nStart || m_pcBuffer == NULL)
        return "";

    if(m_nReversPlay)
    {
        for(m_nPos; m_nPos >= m_nStart && bContinue; m_nPos--)
        {
            strRtn += m_pcBuffer[m_nPos];
        }
        strRtn = ReverseCStr(strRtn);
    }
    else
    {
        for(m_nPos; m_nPos < m_nEnd && bContinue; m_nPos++)
        {
            strRtn += m_pcBuffer[m_nPos];
        }
    }
    return strRtn;
}
//-------------------------------------------------------------------------

UINT32  CTokenCString::NextToken_UInt()
{
    CString strNextValue = NextToken();
    if(strNextValue.GetLength())
    {
        return (UINT32)StrToInt64(strNextValue);
    }
    return 0;
}
//-------------------------------------------------------------------------

INT32   CTokenCString::NextToken_Int()
{
    CString strNextValue = NextToken();
    if(strNextValue.GetLength())
    {
        return (UINT)StrToInt64(strNextValue);
    }
    return 0;
}
//-------------------------------------------------------------------------

UINT64		CTokenCString::NextToken_UInt64()
{
    CString strNextValue = NextToken();
    if(strNextValue.GetLength())
    {
        return (UINT64)StrToInt64(strNextValue);
    }
    return 0;
}
//-------------------------------------------------------------------------

BOOL    CTokenCString::NextToken_Bool()
{
    return (NextToken_Int() == 0 ? false : true);
}
//-------------------------------------------------------------------------

INT16   CTokenCString::NextToken_Short()
{
    return (INT16)NextToken_Int();
}
//-------------------------------------------------------------------------

UINT8    CTokenCString::NextToken_Byte()
{
    return (UINT8)NextToken_Short();
}
//-------------------------------------------------------------------------

INT32	CTokenCString::EditToken(char pTok)
{
	m_cTok = pTok;
	return 0;
}
//-------------------------------------------------------------------------

UINT32	CTokenCString::GetRemainTokenCount()
{
	UINT32 nRtn = 0;
	INT32 nPos = m_nPos;
	INT32 nStart = m_nStart;
	INT32 nEnd = m_nEnd;
	INT32 nLastFindPos = 0;

	if(m_nPos >= m_nEnd || m_nPos < m_nStart || m_pcBuffer == NULL)
        return 0;

	if(m_nReversPlay)
    {
        for(nPos; nPos >= nStart; nPos--)
        {
            if(m_pcBuffer[nPos] == m_cTok)
            {
                nRtn += 1;
				nLastFindPos = nPos;
            }
        }
    }
    else
    {
        for(nPos; nPos < nEnd; nPos++)
        {
            if(m_pcBuffer[nPos] == m_cTok)
            {
                nRtn += 1;
				nLastFindPos = nPos;
            }
        }
    }
	if(nLastFindPos != nPos)
		nRtn += 1;
	return nRtn;
}
//-------------------------------------------------------------------------

CString	CTokenCString::NextToken_Len(INT32 nLen)
{
    CString  strRtn;
	BOOL bContinue = TRUE;
	INT32 nTokenLen = nLen;

    if(m_nPos >= m_nEnd || m_nPos < m_nStart || m_pcBuffer == NULL || nTokenLen < 1)
        return "";

    if(m_nReversPlay)
    {
        for(m_nPos; m_nPos >= m_nStart && bContinue && nTokenLen; m_nPos--)
        {
            strRtn += m_pcBuffer[m_nPos];
			nTokenLen--;
        }
        strRtn = ReverseCStr(strRtn);
    }
    else
    {
        for(m_nPos; m_nPos < m_nEnd && bContinue && nTokenLen; m_nPos++)
        {
            strRtn += m_pcBuffer[m_nPos];
			nTokenLen--;
        }
    }
    return strRtn;
}
//-------------------------------------------------------------------------

CString  CTokenCString::NextTokenByTokPos(INT32 nTokPos)
{
	CString  strRtn;
	BOOL bContinue = TRUE;
	INT32 nPos = nTokPos;
	if(m_nPos >= m_nEnd || m_nPos < m_nStart || m_pcBuffer == NULL || nPos < 0)
		return "";

	if(m_nReversPlay)
	{
		for(m_nPos; m_nPos >= m_nStart && bContinue; m_nPos--)
		{
			if(m_pcBuffer[m_nPos] == m_cTok)
			{
				if(nPos)
				{
					strRtn += m_pcBuffer[m_nPos];
					nPos--;
				}
				else
					bContinue = FALSE;
			}
			else
			{
				strRtn += m_pcBuffer[m_nPos];
			}
		}
		strRtn = ReverseCStr(strRtn);
	}
	else
	{
		for(m_nPos; m_nPos < m_nEnd && bContinue; m_nPos++)
		{
			if(m_pcBuffer[m_nPos] == m_cTok)
			{
				if(nPos)
				{
					strRtn += m_pcBuffer[m_nPos];
					nPos--;
				}
				else
					bContinue = FALSE;
			}
			else
			{
				strRtn += m_pcBuffer[m_nPos];
			}
		}
	}
	return strRtn;
}
//-------------------------------------------------------------------------------

