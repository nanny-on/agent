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

#include "stdafx.h"
#include "com_struct.h"
#include "TokenQuery.h"

//---------------------------------------------------------------------------


CTokenQuery::CTokenQuery()
{
    lpBuffer    = NULL;
    n_Start     = n_End = n_Pos = n_ReversPlay = 0;
    p_Tok       = ',';
}
//-----------------------------------------------------------------

CTokenQuery::CTokenQuery(LPCTSTR lpInput, INT32 nLen, char pTok, INT32 nReversPlay)
{
    lpBuffer    = NULL;
    n_Start     = n_End = n_Pos = n_ReversPlay = 0;
    p_Tok       = ',';

    SetBuffer(lpInput, nLen, pTok, nReversPlay);
}
//-----------------------------------------------------------------

CTokenQuery::~CTokenQuery()
{
    if(lpBuffer)
    {
        delete[] lpBuffer;
    }
    lpBuffer = NULL;
}
//-----------------------------------------------------------------

void    CTokenQuery::SetBuffer(LPCTSTR lpInput, INT32 nLen, char pTok, INT32 nReversPlay)
{
    if(lpBuffer)
        delete[] lpBuffer;

    if(!lpInput || nLen < 1)
        return;

    lpBuffer = new char[nLen];
    memcpy(lpBuffer, lpInput, nLen);
    n_Start = 0;
    n_End = nLen;
    p_Tok = pTok;
    n_ReversPlay = nReversPlay;
    if(n_ReversPlay)
        n_Pos = n_End - 1;
    else
        n_Pos = n_Start;
    return;
}
//-----------------------------------------------------------------

bool    CTokenQuery::IsNextToken()
{
    if(n_Pos >= n_End || n_Pos < n_Start)
        return false;
    return true;
}
//-----------------------------------------------------------------

CString  CTokenQuery::NextToken()
{
    if(n_Pos >= n_End || n_Pos < n_Start)
        return "";

    CString  strRtn;
    INT32    nContinue = 1;

    if(n_ReversPlay)
    {
        for(n_Pos; n_Pos >= n_Start && nContinue; n_Pos--)
        {
            if(lpBuffer[n_Pos] == p_Tok)
            {
                nContinue = 0;
            }
            else
            {
                strRtn += lpBuffer[n_Pos];
            }
        }
        strRtn = ReverseString(strRtn);
    }
    else
    {
        for(n_Pos; n_Pos < n_End && nContinue; n_Pos++)
        {
            if(lpBuffer[n_Pos] == p_Tok)
            {
				if(strRtn.IsEmpty())
				{
					continue;
				}
				
				nContinue = 0;
            }
            else
            {
                strRtn += lpBuffer[n_Pos];
            }
        }
    }
    return CString(strRtn);
}
//-----------------------------------------------------------------

CString  CTokenQuery::NextToken(CHAR chTokOpt)
{
    if(n_Pos >= n_End || n_Pos < n_Start)
        return "";

    CString  strRtn;
    INT32    nContinue = 1;

    if(n_ReversPlay)
    {
        for(n_Pos; n_Pos >= n_Start && nContinue; n_Pos--)
        {
            if(lpBuffer[n_Pos] == p_Tok || lpBuffer[n_Pos] == chTokOpt)
            {
                nContinue = 0;
            }
            else
            {
                strRtn += lpBuffer[n_Pos];
            }
        }
        strRtn = ReverseString(strRtn);
    }
    else
    {
        for(n_Pos; n_Pos < n_End && nContinue; n_Pos++)
        {
            if(lpBuffer[n_Pos] == p_Tok || lpBuffer[n_Pos] == chTokOpt)
            {
				if(strRtn.IsEmpty())
				{
					continue;
				}
				
				nContinue = 0;
            }
            else
            {
                strRtn += lpBuffer[n_Pos];
            }
        }
    }
    return CString(strRtn);
}
//-----------------------------------------------------------------

CString  CTokenQuery::NextTokenValue()
{
	if(n_Pos >= n_End || n_Pos < n_Start)
		return "";

	CString strRtn;
	char acChar[4] = {0,};
	INT32 nContinue = 1;
	INT32	nChkFlag = 0;

	if(n_ReversPlay)
	{
		for(n_Pos; n_Pos >= n_Start && nContinue; n_Pos--)
		{
			if(lpBuffer[n_Pos] == '\'')
			{
				nChkFlag ^= 1;
				acChar[0] = lpBuffer[n_Pos];
				strRtn += acChar;
			}
			else if(!nChkFlag && lpBuffer[n_Pos] == p_Tok)
			{
				nContinue = 0;
			}
			else
			{
				acChar[0] = lpBuffer[n_Pos];
				strRtn += acChar;
			}
		}
		strRtn = ReverseString(strRtn);
	}
	else
	{
		for(n_Pos; n_Pos < n_End && nContinue; n_Pos++)
		{
			if(lpBuffer[n_Pos] == '\'')
			{
				nChkFlag ^= 1;
				acChar[0] = lpBuffer[n_Pos];
				strRtn += acChar;
			}
			else if(!nChkFlag && lpBuffer[n_Pos] == p_Tok)
			{
				if(strRtn.IsEmpty())
				{
					continue;
				}

				nContinue = 0;
			}
			else
			{
				acChar[0] = lpBuffer[n_Pos];
				strRtn += acChar;
			}
		}
	}
	return strRtn;
}
//-----------------------------------------------------------------

CString		CTokenQuery::NextTokenTableName()
{
	if(n_Pos >= n_End || n_Pos < n_Start)
		return "";

	CString strRtn;
	char acChar[4] = {0,};
	INT32 nContinue = 1;

	if(n_ReversPlay)
	{
		while(n_Pos >= n_Start && nContinue)
		{
			CHAR chChk = lpBuffer[n_Pos];

			if((chChk == ' ' || chChk == '\t'))
			{
				nContinue = 0;
				n_Pos--;
			}
			else if(chChk == '(')
			{
				nContinue = 0;
			}
			else
			{
				acChar[0] = chChk;
				strRtn += acChar;
				n_Pos -= 1;
			}
		}
		strRtn = ReverseString(strRtn);
	}
	else
	{
		while(n_Pos < n_End && nContinue)
		{
			CHAR chChk = lpBuffer[n_Pos];

			if((chChk == ' ' || chChk == '\t'))
			{
				n_Pos += 1;
				if(strRtn.IsEmpty())
				{					
					continue;
				}
				nContinue = 0;
			}
			else if(chChk == '(')
			{
				nContinue = 0;
			}
			else
			{
				acChar[0] = chChk;
				strRtn += acChar;
				n_Pos += 1;
			}
		}
	}
	return strRtn;
}
//-----------------------------------------------------------------

CString		CTokenQuery::NextToken_SelectValue()
{
	char acChar[4] = {0,};
	if(n_Pos >= n_End || n_Pos < n_Start)
		return "";

	CString strRtn;
	INT32 nContinue = 1;
	INT32	nValueTokFind = 0;

	{
		for(n_Pos; n_Pos < n_End && nContinue; n_Pos++)
		{
			CHAR chChkValue = lpBuffer[n_Pos];
			if(chChkValue == ' ')
			{
				if(strRtn.IsEmpty())
				{
					continue;
				}
				nContinue = 0;
			}
			else if(chChkValue == ',')
			{
				nValueTokFind += 1;
			}
			else
			{
				acChar[0]= chChkValue;
				strRtn += acChar;
			}
		}
	}
	return strRtn;
}
//-----------------------------------------------------------------

INT32		CTokenQuery::IsNextToken_SelectValue()
{
	if(n_Pos >= n_End || n_Pos < n_Start)
		return 0;

	INT32 nIdx = n_Pos;
	while(nIdx < n_End)
	{
		if(lpBuffer[nIdx] == ' ')
		{
			nIdx++;
		}
	}	
	
	CString strRemain = &(lpBuffer[nIdx]);

	strRemain.MakeLower();
	if(strRemain.Find("from ") == 0)	return 0;

	return 1;
}
//-----------------------------------------------------------------

CString		CTokenQuery::GetToken_SelTable()
{
	CString strFrom = NextToken();
	if(strFrom.CompareNoCase("from"))	return "";
	return NextToken();
}
//-----------------------------------------------------------------

INT32		CTokenQuery::GetToken_DirectionInfo(CString& strKey, UINT32& nDirection)
{
	char acChar[4] = {0,};
	CString strWhere = NextToken();
	if(strWhere.CompareNoCase("where"))		return 0;

	CString strCondition = &(lpBuffer[n_Pos]);
	strCondition.MakeLower();

	INT32 nIdx = strCondition.Find(" order by ");
	INT32 nLen = strCondition.GetLength();

	if(nIdx == -1)	return 0;

	nIdx += strlen_ext(" order by ");
	while(nIdx < nLen)
	{
		CHAR chCell = lpBuffer[n_Pos + nIdx];
		if(chCell == ' ' || chCell == ';')
		{
			if(strKey.IsEmpty())
			{
				continue;
			}
			break;
		}
		else
		{
			acChar[0] = chCell;
			strKey += acChar;
		}
		nIdx++;
	}

	CString strDirection;
	while(nIdx < nLen)
	{
		CHAR chCell = lpBuffer[n_Pos + nIdx];
		if(chCell == ' ' || chCell == ';')
		{
			if(strDirection.IsEmpty())
			{
				continue;
			}
			break;
		}
		else
		{
			acChar[0] = chCell;
			strDirection += acChar;
		}
		nIdx++;
	}

	if(strDirection.Compare("desc") == 0)		nDirection = 1;
	if(strDirection.Compare("asc") == 0)		nDirection = 0;

	return 0;

}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

INT32		CTokenQuery::IsNextToken_WhereCdt(CString strCmd)
{
	if(strCmd.IsEmpty())	return 0;

	CString strCmdLow = strCmd;		strCmdLow.MakeLower();
	LPCTSTR lpCmdLow = strCmdLow;

	INT32 nFindIdx = 0, nWaitAnd = 0;

	INT32 nIdx = 0;
	while(nIdx < strCmd.GetLength())
	{
		CHAR chChk = lpCmdLow[nIdx++];
		if(nFindIdx != 8 && (chChk == ' ' || chChk == '\t'))
		{
			nFindIdx = 1;
			continue;
		}
		else if(chChk == ';')	return 0;
		else if(chChk == '\'')	return 1;
		else if(chChk == '=')	return 1;
		else if(chChk == '<')	return 1;
		else if(chChk == '>')	return 1;
		else if(!nWaitAnd)
		{			
			if(nFindIdx == 1 && chChk == 'b')		nFindIdx += 1;
			if(nFindIdx == 2 && chChk == 'e')		nFindIdx += 1;
			if(nFindIdx == 3 && chChk == 't')		nFindIdx += 1;
			if(nFindIdx == 4 && chChk == 'w')		nFindIdx += 1;
			if(nFindIdx == 5 && chChk == 'e')		nFindIdx += 1;
			if(nFindIdx == 6 && chChk == 'e')		nFindIdx += 1;
			if(nFindIdx == 7 && chChk == 'n')		nFindIdx += 1;
			if(nFindIdx == 8 && chChk == ' ')		return 1;
			if(nFindIdx == 8 && chChk == '\t')		return 1;
		}
	}		
	return 0;
}
//-----------------------------------------------------------------

CString		CTokenQuery::NextToken_WhereCdt(CString& strCmd, CString& strName, CString& strValue, INT32& nType)
{
	char acChar[4] = {0,};
	CString strCmdLow = strCmd;
	strCmdLow.MakeLower();

	LPCTSTR lpCmdLow = strCmdLow;
	LPCTSTR lpCmd = strCmd;

	CString strKeyValue;
	strName = "", strValue = "", nType = 0;

	INT32 nIdx = 0, nAndIdx = 0, nBtIdx = 0;
	INT32 nWaitValue = 0, nWaitSpace = 0, nWaitAnd = 0, nEndCmd = 0;
	INT32 nFindCdt = 0, nIgnoreSpace = 0;
	INT32 nValueCompType = 0;


	while(nIdx < strCmdLow.GetLength())
	{
		CHAR chChk = lpCmdLow[nIdx];
		CHAR chChkOr = lpCmd[nIdx++];

		if(nEndCmd)
		{
			continue;
		}

		if(chChk == ';' && nWaitSpace && !nFindCdt)
		{
			nEndCmd = 1;
			continue;
		}

		strKeyValue += chChkOr;

		if(chChk == '=' && nWaitValue == 0 && nValueCompType == 0)
		{
			nType = ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL;

			nValueCompType = 1;
			nWaitValue = 1;
			nWaitSpace = 1;
			nAndIdx = nBtIdx = 0;
			nIgnoreSpace = 1;
			continue;
		}
		else if(chChk == '=' && nValueCompType == 1 && nWaitValue == 1 && nType == ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_LARGE)
		{
			nType = ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_LARGE;

			nValueCompType = 0;

			nWaitValue = 1;
			nWaitSpace = 1;
			nAndIdx = nBtIdx = 0;
			nIgnoreSpace = 1;
			continue;
		}
		else if(chChk == '=' && nValueCompType == 1 && nWaitValue == 1 && nType == ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_SMALL)
		{
			nType = ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_SMALL;

			nValueCompType = 0;
			
			nWaitValue = 1;
			nWaitSpace = 1;
			nAndIdx = nBtIdx = 0;
			nIgnoreSpace = 1;
			continue;
		}
		else if(chChk == '<' && nWaitValue == 0 && nValueCompType == 0)
		{
			nType = ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_SMALL;

			nValueCompType = 1;

			nWaitValue = 1;
			nWaitSpace = 1;
			nAndIdx = nBtIdx = 0;
			nIgnoreSpace = 1;
			continue;
		}
		else if(chChk == '>' && nWaitValue == 0 && nValueCompType == 0)
		{
			nType = ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_LARGE;

			nValueCompType = 1;

			nWaitValue = 1;
			nWaitSpace = 1;
			nAndIdx = nBtIdx = 0;
			nIgnoreSpace = 1;
			continue;
		}
		else if(chChk == '>' && nWaitValue == 1 && nValueCompType == 1 && nType == ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_SMALL)
		{
			nType = ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_NOT_EQUAL;

			nValueCompType = 0;

			nWaitValue = 1;
			nWaitSpace = 1;
			nAndIdx = nBtIdx = 0;
			nIgnoreSpace = 1;
			continue;
		}
		else if(nWaitSpace && chChk == '\'')
		{	
			nValueCompType = 0;

			nFindCdt ^= 1;
			nAndIdx = 0;
		}
		else if(nIgnoreSpace && (chChk == ' ' || chChk == '\t'))
		{
			if(nWaitValue)
			{
				acChar[0] = chChkOr;
				strValue += acChar;
			}
			else
			{
				acChar[0] = chChkOr;
				strName += acChar;
			}

			continue;
		}
		else if((chChk == ' ' || chChk == '\t') && !nFindCdt && nWaitValue && nWaitSpace && (nWaitAnd ? (nWaitAnd == 2 ? TRUE : FALSE) : TRUE) && strValue.IsEmpty() == FALSE)
		{
			nAndIdx = 0;
			break;
		}
		else
		{
			nIgnoreSpace = 0;

			if(nBtIdx != 8 && (chChk == ' ' || chChk == '\t'))			nBtIdx += 1;
			if(nBtIdx == 1 && chChk == 'b')			nBtIdx += 1;
			if(nBtIdx == 2 && chChk == 'e')			nBtIdx += 1;
			if(nBtIdx == 3 && chChk == 't')			nBtIdx += 1;
			if(nBtIdx == 4 && chChk == 'w')			nBtIdx += 1;
			if(nBtIdx == 5 && chChk == 'e')			nBtIdx += 1;
			if(nBtIdx == 6 && chChk == 'e')			nBtIdx += 1;
			if(nBtIdx == 7 && chChk == 'n')			nBtIdx += 1;
			if(nBtIdx == 8 && (chChk == ' ' || chChk == '\t'))	
			{
				nIgnoreSpace = 1;
				nWaitValue = 1;
				nWaitAnd = 1;

				strValue = strName.Right(8);
				strName.Delete(strName.GetLength() - 8, 8);

				nType = ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_BETWEEN;
				nBtIdx = 0;
			}

			if(nAndIdx != 4 && (chChk == ' ' || chChk == '\t'))		nAndIdx = 1;
			if(nAndIdx == 1 && chChk == 'a')		nAndIdx += 1;
			if(nAndIdx == 2 && chChk == 'n')		nAndIdx += 1;
			if(nAndIdx == 3 && chChk == 'd')		nAndIdx += 1;
			if(nAndIdx == 4 && (chChk == ' ' || chChk == '\t'))
			{				
				nAndIdx = 0;

				if(!nWaitValue && !nWaitAnd)
				{
					strKeyValue = "";
					strName = "";
					continue;
				}
				nIgnoreSpace = 1;
				nWaitSpace = 1;
				if(nWaitAnd)	nWaitAnd = 2;
			}
		}

		if(nWaitValue)
		{
			acChar[0] = chChkOr;
			strValue += acChar;
		}
		else
		{
			acChar[0] = chChkOr;
			strName += acChar;
		}
	}

	strName.Trim();
	strValue.Trim();

	if(nIdx && nIdx != strCmdLow.GetLength())	strCmd = &(lpCmd[nIdx -1]);
	else		strCmd = "";

	return strKeyValue;
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

CString  CTokenQuery::RemainValue()
{
	if(n_Pos >= n_End || n_Pos < n_Start)
		return "";

	CString strRtn;
	char acChar[4] = {0,};
	bool bContinue = true;

	if(n_ReversPlay)
	{
		for(n_Pos; n_Pos >= n_Start && bContinue; n_Pos--)
		{
			acChar[0] = lpBuffer[n_Pos];
			strRtn += acChar;
		}
		strRtn = ReverseString((LPCTSTR)strRtn);
	}
	else
	{
		for(n_Pos; n_Pos < n_End && bContinue; n_Pos++)
		{
			acChar[0] = lpBuffer[n_Pos];
			strRtn += acChar;
		}
	}
	return strRtn;
}
//-----------------------------------------------------------------

UINT32  CTokenQuery::NextToken_UInt()
{
    try
    {
        CString strNextValue = NextToken();
        if(strNextValue.GetLength())
        {
            return (UINT32)StrToInt64(strNextValue);
        }
    }
    catch(...)
    {
    }
    return 0;
}
//-----------------------------------------------------------------

UINT64		CTokenQuery::NextToken_UInt64()
{
	try
	{
		CString strNextValue = NextToken();
		if(strNextValue.GetLength())
		{
			return (UINT64)StrToInt64(strNextValue);
		}
	}
	catch(...)
	{
	}
	return 0;
}
//-----------------------------------------------------------------

INT32   CTokenQuery::NextToken_Int()
{
    try
    {
        CString strNextValue = NextToken();
        if(strNextValue.GetLength())
        {
            return (UINT)StrToInt64(strNextValue);
        }
    }
    catch(...)
    {
    }
    return 0;
}
//-----------------------------------------------------------------

bool    CTokenQuery::NextToken_Bool()
{
    return (NextToken_Int() == 0 ? false : true);
}
//-----------------------------------------------------------------

INT16   CTokenQuery::NextToken_Short()
{
    return (INT16)NextToken_Int();
}
//-----------------------------------------------------------------

BYTE    CTokenQuery::NextToken_Byte()
{
    return (BYTE)NextToken_Short();
}
//-----------------------------------------------------------------

CString	CTokenQuery::ReverseString(LPCTSTR lpInput, INT32 nLen)
{
	if(!nLen)	nLen = strlen_ext(lpInput);

	INT32 nSIdx = 0;
	INT32 nEIdx = (nLen - 1);

	while(nSIdx < nEIdx && nSIdx != nEIdx)
	{
		char chTemp = lpInput[nSIdx];
		((LPTSTR)lpInput)[nSIdx] = lpInput[nEIdx];
		((LPTSTR)lpInput)[nEIdx] = chTemp;

		nSIdx++;	nEIdx--;
	}
	return lpInput;
}
//-----------------------------------------------------------------


