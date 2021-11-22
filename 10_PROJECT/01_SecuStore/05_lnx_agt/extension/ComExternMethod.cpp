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
 * 
 */


#include "stdafx.h"
#include "com_struct.h"
#include "ComExternMethod.h"


//------------------------------------------------------------------------------
INT32			HexToMap(String strHexValue, TMapID& tValueMap, INT32 nMaxLen)
{
	INT32 nIdx = 0;
	CTokenString Token(strHexValue.c_str(), strHexValue.length());
	while(Token.IsNextToken())
	{
		String strBlock = Token.NextToken_Len(8);
		UINT32 nValue = HexToInt(strBlock.c_str());
		
		tValueMap[nIdx] = nValue;
		nIdx += 1;
		if(nMaxLen)		nMaxLen -= 1;
	}

	while(nIdx < nMaxLen)
	{
		tValueMap[nIdx] = 0;
		nIdx += 1;
	}
	return 0;
}
//------------------------------------------------------------------------------

INT32			MapToHex(TMapID& tValueMap, String& strHexValue, INT32 nMaxLen)
{
	INT32 nIdx = 0;
	while(nIdx < nMaxLen)
	{
		UINT32 nValue = 0;
		TMapIDItor find = tValueMap.find(nIdx);
		if(find != tValueMap.end())
		{
			nValue = find->second;
		}
		strHexValue += IntToHex(nValue);
		nIdx += 1;
	}
	return 0;
}
//------------------------------------------------------------------------------

INT32			HexToMap_64(String strHexValue, TMapID64& tValueMap, INT32 nMaxLen)
{
	INT32 nIdx = 0;
	CTokenString Token(strHexValue.c_str(), strHexValue.length());
	while(Token.IsNextToken())
	{
		String strBlock = Token.NextToken_Len(16);
		UINT64 nValue = HexToInt64(strBlock.c_str());

		tValueMap[nIdx] = nValue;
		nIdx += 1;
		if(nMaxLen)		nMaxLen -= 1;
	}

	while(nIdx < nMaxLen)
	{
		tValueMap[nIdx] = 0;
		nIdx += 1;
	}
	return 0;
}
//------------------------------------------------------------------------------

INT32			MapToHex_64(TMapID64& tValueMap, String& strHexValue, INT32 nMaxLen)
{
	INT32 nIdx = 0;
	while(nIdx < nMaxLen)
	{
		UINT64 nValue = 0;
		TMapID64Itor find = tValueMap.find(nIdx);
		if(find != tValueMap.end())
		{
			nValue = find->second;
		}
		strHexValue += Int64ToHex(nValue);
		nIdx += 1;
	}
	return 0;
}
//------------------------------------------------------------------------------

INT32			MapIDStrToString(TMapIDStr& tIDMapStr, String& strMapList, INT32 nMaxLen, CHAR chBlockTok)
{
	INT32 nIdx = 0;
	TMapIDStrItor begin, end;
	begin = tIDMapStr.begin();	end = tIDMapStr.end();
	for(begin; begin != end && nIdx < nMaxLen; begin++)
	{
		String strCell = SPrintf("%s%c", begin->second.c_str(), chBlockTok);
		strMapList += strCell;
		nIdx += 1;
	}

	while(nIdx < (nMaxLen - 1))
	{
		strMapList += SPrintf("%c", chBlockTok);
		nIdx += 1;
	}
	return 0;
}
//------------------------------------------------------------------------------

INT32			StringToMapIDStr(String strMapList, TMapIDStr& tIDMapStr, INT32 nMaxLen, CHAR chBlockTok)
{
	INT32 nIdx = 0;
	CTokenString Token(strMapList.c_str(), strMapList.length(), chBlockTok);
	while(Token.IsNextToken() && nIdx < nMaxLen)
	{
		String strCell = Token.NextToken();

		tIDMapStr[nIdx] = strCell;

		nIdx += 1;
	}

	while(nIdx < (nMaxLen - 1))
	{
		String strEmptyValue = "";
		tIDMapStr[nIdx] = strEmptyValue;

		nIdx += 1;
	}

	return tIDMapStr.size();
}

//------------------------------------------------------------------------------

String			MapStrToStr(TMapStr& tStrMap, CHAR chCellTok, CHAR chBlockTok)
{
	String strValue;
	TMapStrItor begin, end;
	begin = tStrMap.begin();	end = tStrMap.end();
	for(begin; begin != end; begin++)
	{
		String strCell = SPrintf("%s%c%s%c", begin->first.c_str(), chCellTok, begin->second.c_str(), chBlockTok);
		strValue += strCell;
	}
	return strValue;
}
//------------------------------------------------------------------------------

INT32			StrToMapStr(String strMapList, TMapStr& tStrMap, CHAR chCellTok, CHAR chBlockTok)
{
	CTokenString Token(strMapList.c_str(), strMapList.length(), chBlockTok);
	while(Token.IsNextToken())
	{
		String strCell = Token.NextToken();

		CTokenString TokenCel(strCell.c_str(), strCell.length(), chCellTok);

		String strKey = TokenCel.NextToken();
		String strValue = TokenCel.RemainValue();

		tStrMap[strKey] = strValue;
	}
	return tStrMap.size();
}
//------------------------------------------------------------------------------

String			MapIDStrToStr(TMapIDStr& tIDMapStr, CHAR chCellTok, CHAR chBlockTok)
{
	String strValue;
	TMapIDStrItor begin, end;
	begin = tIDMapStr.begin();	end = tIDMapStr.end();
	for(begin; begin != end; begin++)
	{
		String strCell = SPrintf("%u%c%s%c", begin->first, chCellTok, begin->second.c_str(), chBlockTok);
		strValue += strCell;
	}
	return strValue;
}
//------------------------------------------------------------------------------

INT32			StrToMapIDStr(String strMapList, TMapIDStr& tIDMapStr, CHAR chCellTok)
{
	INT32 nIdx = 1;
	CTokenString Token(strMapList.c_str(), strMapList.length(), chCellTok);
	while(Token.IsNextToken())
	{
		String strValue = Token.NextToken();
		tIDMapStr[nIdx++] = strValue;
	}
	return tIDMapStr.size();
}

String			ListStrToStr(TListStr& tStrList, CHAR chBlockTok)
{
	String strValue;
	TListStrItor begin, end;
	begin = tStrList.begin();	end = tStrList.end();
	for(begin; begin != end; begin++)
	{
		String strCell;
		strCell = SPrintf("%s%c", (*begin).c_str(), chBlockTok);
		strValue += strCell;
	}
	return strValue;
}

INT32			StrToListStr(String strListList, TListStr& tStrList, CHAR chBlockTok)
{
	CTokenString Token(strListList.c_str(), strListList.length(), chBlockTok);
	while(Token.IsNextToken())
	{
		String strKey = Token.NextToken();

		tStrList.push_back(strKey);
	}
	return tStrList.size();
}

String			MapIDToStr_Str(TMapIDStr& tIDMapStr, CHAR chCellTok, CHAR chBlockTok)
{
	String strValue;
	TMapIDStrItor begin, end;
	begin = tIDMapStr.begin();	end = tIDMapStr.end();
	for(begin; begin != end; begin++)
	{
		String strCell;
		strCell = SPrintf("%u%c%s%c", begin->first, chCellTok, begin->second.c_str(), chBlockTok);
		strValue += strCell;
	}
	return strValue;
}

INT32			StrToMapID_Str(String strMapList, TMapIDStr& tIDMapStr, CHAR chCellTok, CHAR chBlockTok)
{
	CTokenString Token(strMapList.c_str(), strMapList.length(), chBlockTok);
	while(Token.IsNextToken())
	{
		String strCell = Token.NextToken();

		CTokenString TokenCel(strCell.c_str(), strCell.length(), chCellTok);

		UINT32 nKey = TokenCel.NextToken_UInt();
		String strValue = TokenCel.RemainValue();

		tIDMapStr[nKey] = strValue;
	}
	return tIDMapStr.size();
}

//------------------------------------------------------------------------------

String			MapIDToStr_ID(TMapID& tIDMap, CHAR chCellTok, CHAR chBlockTok)
{
	String strValue;
	TMapIDItor begin, end;
	begin = tIDMap.begin();	end = tIDMap.end();
	for(begin; begin != end; begin++)
	{
		String strCell;
		strCell= SPrintf("%u%c%u%c", begin->first, chCellTok, begin->second, chBlockTok);
		strValue += strCell;
	}
	return strValue;
}
//------------------------------------------------------------------------------

INT32			StrToMapID_ID(String strMapList, TMapID& tIDMap, CHAR chCellTok, CHAR chBlockTok)
{
	CTokenString Token(strMapList.c_str(), strMapList.length(), chBlockTok);
	while(Token.IsNextToken())
	{
		String strCell = Token.NextToken();

		CTokenString TokenCel(strCell.c_str(), strCell.length(), chCellTok);

		UINT32 nKey = TokenCel.NextToken_UInt();
		UINT32 nValue = TokenCel.NextToken_UInt();

		tIDMap[nKey] = nValue;
	}
	return tIDMap.size();
}


//------------------------------------------------------------------------------

INT32			MaptoMapStr_64(TMapID64& tIDMap, TMapStr& tStrMap)
{
	TMapID64Itor begin, end;
	begin = tIDMap.begin();		end = tIDMap.end();
	for(; begin != end; begin++)
	{
		tStrMap[UInt64ToStr(begin->first)] = UInt64ToStr(begin->second);
	}

	return tStrMap.size();
}
//------------------------------------------------------------------------------

INT32			MapStrToMap_64(TMapStr& tStrMap, TMapID64& tIDMap)
{
	TMapStrItor begin, end;
	begin = tStrMap.begin();		end = tStrMap.end();
	for(; begin != end; begin++)
	{
		tIDMap[StrToUInt64(begin->first.c_str())] = StrToUInt64(begin->second.c_str());
	}

	return tIDMap.size();
}
//------------------------------------------------------------------------------

INT32			HexToP32(String strHexValue, PINT32 pValue, INT32 nValNum)
{
	INT32 nIdx = 0;
	CTokenString Token(strHexValue.c_str(), strHexValue.length());
	while(Token.IsNextToken() && nIdx < nValNum)
	{
		String strBlock = Token.NextToken_Len(8);
		UINT32 nValue = HexToInt(strBlock.c_str());

		pValue[nIdx] = nValue;
		nIdx += 1;
	}
	return 0;
}
//------------------------------------------------------------------------------

INT32			P32ToHex(PINT32 pValue, INT32 nValNum, String& strHexValue)
{
	INT32 nIdx = 0;
	while(nIdx < nValNum)
	{
		UINT32 nValue = pValue[nIdx];		
		strHexValue += IntToHex(nValue);
		nIdx += 1;
	}
	return 0;
}
//------------------------------------------------------------------------------

INT32			RemoveSdeltemp()
{
	return 0;
}
//------------------------------------------------------------------------------
INT32			CheckDumpFile(String &strPath, INT32 nRestDumpCount)
{
	return 0;
}
//------------------------------------------------------------------------------

INT32			RemoveDumpFiles()
{
	return 0;
}
//------------------------------------------------------------------------------

INT32			RemoveInvenFiles()
{
	return 0;
}
//------------------------------------------------------------------------------

void			WriteLogA(char* fmt,...)
{
}

//------------------------------------------------------------------------------

void			DeletePrefixFiles(String strPath, LPCTSTR lpszPrefixed)
{
}
//------------------------------------------------------------------------------