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

#ifndef TokenQueryH
#define TokenQueryH
//---------------------------------------------------------------------------

#include <string>
using namespace std;

class CTokenQuery
{
private:
    char*   lpBuffer;
    INT32   n_Start;
    INT32   n_End;
    INT32   n_Pos;
    INT32   n_ReversPlay;
    char    p_Tok;
public:
    void    	SetBuffer(LPCTSTR lpInput, INT32 nLen, char pTok = ' ', INT32 nReversPlay = 0);
	CString		NextToken();
	CString		NextToken(CHAR chTokOpt);
	CString		NextTokenValue();
	CString		NextTokenTableName();
	bool		IsNextToken();
	
	CString		NextToken_SelectValue();
	INT32		IsNextToken_SelectValue();
	CString		GetToken_SelTable();
	INT32		GetToken_DirectionInfo(CString& strKey, UINT32& nDirection);

	INT32		IsNextToken_WhereCdt(CString strCmd);
	CString		NextToken_WhereCdt(CString& strCmd, CString& strName, CString& strValue, INT32& nType);

    UINT32		NextToken_UInt();
	UINT64		NextToken_UInt64();
    INT32		NextToken_Int();
    bool		NextToken_Bool();
    INT16		NextToken_Short();
    BYTE		NextToken_Byte();
public:
	CString		RemainValue();    

public:
	CString		ReverseString(LPCTSTR lpInput, INT32 nLen = 0);

public:
    CTokenQuery();
    CTokenQuery(LPCTSTR lpInput, INT32 nLen, char pTok = ' ', INT32 nReversPlay = 0);
    ~CTokenQuery();
};

#endif
