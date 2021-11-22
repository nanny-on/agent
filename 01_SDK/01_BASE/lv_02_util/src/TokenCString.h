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

//---------------------------------------------------------------------------

#ifndef TokenCStringH
#define TokenCStringH
//---------------------------------------------------------------------------

class CTokenCString
{
private:
    char*   m_pcBuffer;
    INT32   m_nStart;
    INT32   m_nEnd;
    INT32   m_nPos;
    INT32   m_nReversPlay;
    char    m_cTok;
public:
    void    	SetBuffer(const char *pcInput, INT32 nLen, char pTok = '\f', INT32 nReversPlay = 0);
    CString		NextToken();
	CString		NextToken_Line();
	CString		NextToken_Query();
    UINT32		NextToken_UInt();
	UINT64		NextToken_UInt64();
    INT32		NextToken_Int();
    BOOL		NextToken_Bool();
    INT16		NextToken_Short();
	UINT8		NextToken_Byte();
	CString		NextToken_Len(INT32 nLen = 15);
	CString		NextTokenByTokPos(INT32 nTokPos);
    BOOL		IsNextToken();
	INT32		IsNext_Line();

public:
    CString		RemainValue();
	INT32		EditToken(char pTok = '\a');
	UINT32		GetRemainTokenCount();
	CString		ReverseCStr(CString strInput);

public:
    CTokenCString();
    CTokenCString(const char *pcInput, INT32 nLen, char pTok = '\f', INT32 nReversPlay = 0);
	CTokenCString(BYTE* lpInput, INT32 nLen, char pTok = '\f', INT32 nReversPlay = 0);
    ~CTokenCString();
};
extern CTokenCString*		t_TokenCString;

#endif
