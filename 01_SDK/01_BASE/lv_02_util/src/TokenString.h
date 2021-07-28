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

//---------------------------------------------------------------------------

#ifndef TokenStringH
#define TokenStringH
//---------------------------------------------------------------------------

class CTokenString
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
    String		NextToken();
	String		NextToken_Line();
	String		NextToken_Query();    
    UINT32		NextToken_UInt();
	UINT64		NextToken_UInt64();
    INT32		NextToken_Int();
    BOOL		NextToken_Bool();
    INT16		NextToken_Short();
	UINT8		NextToken_Byte();
	String		NextToken_Len(INT32 nLen = 15);
	String		NextTokenByTokPos(INT32 nTokPos);
    BOOL		IsNextToken();
	INT32		IsNext_Line();

public:
    String		RemainValue();    
	INT32		EditToken(char pTok = '\a');
	UINT32		GetRemainTokenCount();

public:
    CTokenString();
    CTokenString(const char *pcInput, INT32 nLen, char pTok = '\f', INT32 nReversPlay = 0);
	CTokenString(BYTE* lpInput, INT32 nLen, char pTok = '\f', INT32 nReversPlay = 0);
    ~CTokenString();
};
extern CTokenString*		t_TokenString;

#endif
