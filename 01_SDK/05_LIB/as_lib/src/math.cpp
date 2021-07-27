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

#include "stdafx.h"
#include "as_etc.h"
#include "as_string.h"

//---------------------------------------------------------------------------------------------------------------------------

INT32	StrToIntExt(LPCTSTR lpValue)
{
	if(!lpValue)
		return 0;
	if(strlen(lpValue) == 0)
		return 0;
	
	return atoi(lpValue);
}
//---------------------------------------------------------------------------

INT64	StrToInt64(LPCTSTR lpValue)
{
	if(!lpValue)
		return 0;
	if(strlen(lpValue) == 0)
		return 0;

	return (INT64)atoll(lpValue);
}
//---------------------------------------------------------------------------

UINT64	StrToUInt64(LPCTSTR lpValue)
{
	if(!lpValue)
		return 0;
	if(strlen(lpValue) == 0)
		return 0;

	return (UINT64)strtoull(lpValue, NULL, 10);
}
//---------------------------------------------------------------------------

String UInt64ToStr(UINT64 nValue)
{
	String strValue;
	strValue = SPrintf("%llu", nValue);
	return strValue;
}
//---------------------------------------------------------------------------

String	BinToHex(BYTE nValue)
{
	String strValue;
	CHAR szHex[3] = {0, };
	sprintf_ext(3, szHex, "%02X", nValue);
	strValue = szHex;
	return strValue;
}
//---------------------------------------------------------------------------

String	IntToHex(UINT32 nValue)
{    
	String strValue;
	CHAR szHex[9] = {0, };
	nValue = htonl_ext(nValue);

	for(INT32 nIdx = 0; nIdx < 4; nIdx++)
	{
		String strHex = BinToHex(((PBYTE)&nValue)[nIdx]);
		strcat_ext(szHex, strHex.c_str(), 9);
	}
	strValue = szHex;
	return strValue;
}
//---------------------------------------------------------------------------

String	Int16ToHex(UINT16 nValue)
{
	String strValue;
	CHAR szHex[5] = {0, };
	nValue = htonl16_ext(nValue);

	for(INT32 nIdx = 0; nIdx < 2; nIdx++)
	{
		String strHex = BinToHex(((PBYTE)&nValue)[nIdx]);
		strcat_ext(szHex, strHex.c_str(), 5);
	}
	strValue = szHex;
	return strValue;
}
//---------------------------------------------------------------------------

String	Int64ToHex(UINT64 nValue)
{
	String strValue;
	CHAR szHex[17] = {0, };
	nValue = htonl64_ext(nValue);

	for(INT32 nIdx = 0; nIdx < 8; nIdx++)
	{
		String strHex = BinToHex(((PBYTE)&nValue)[nIdx]);
		strcat_ext(szHex, strHex.c_str(), 17);
	}
	strValue = szHex;
	return strValue;
}
//---------------------------------------------------------------------------

BYTE	HexToBin(LPCTSTR lpHex)
{
	BYTE bBin = 0;
	bBin = (BYTE)strtol(lpHex, NULL, 16);
	return bBin;
}
//---------------------------------------------------------------------------

UINT32	HexToInt(LPCTSTR lpHex)
{
	UINT32 nRtn = 0;
	INT32 nPosHex = 0;
	INT32 nHexLen = strlen(lpHex);
	INT32 nIndx = 0;

	while(nPosHex < nHexLen && nIndx < 4)
	{
		CHAR szHexCell[3] = {0, };		
		memcpy(szHexCell, lpHex + nPosHex, 2);	

		((PBYTE)&nRtn)[nIndx] = HexToBin(szHexCell);

		nIndx += 1;
		nPosHex += 2;
	}
	return htonl_ext(nRtn);
}
//---------------------------------------------------------------------------

UINT16	HexToInt16(LPCTSTR lpHex)
{
	UINT16 nRtn = 0;
	INT32 nPosHex = 0;
	INT32 nHexLen = strlen(lpHex);
	INT32 nIndx = 0;

	while(nPosHex < nHexLen && nIndx < 2)
	{
		CHAR szHexCell[3] = {0, };		
		memcpy(szHexCell, lpHex + nPosHex, 2);	

		((PBYTE)&nRtn)[nIndx] = HexToBin(szHexCell);

		nIndx += 1;
		nPosHex += 2;
	}
	return htonl16_ext(nRtn);
}
//---------------------------------------------------------------------------

UINT64	HexToInt64(LPCTSTR lpHex)
{
	UINT64 nRtn = 0;
	INT32 nPosHex = 0;
	INT32 nHexLen = strlen(lpHex);
	INT32 nIndx = 0;

	while(nPosHex < nHexLen && nIndx < 8)
	{
		CHAR szHexCell[3] = {0, };		
		memcpy(szHexCell, lpHex + nPosHex, 2);	

		((PBYTE)&nRtn)[nIndx] = HexToBin(szHexCell);

		nIndx += 1;
		nPosHex += 2;
	}
	return htonl64_ext(nRtn);
}
//---------------------------------------------------------------------------

String	BinToHexLen(PBYTE lpBin, INT32 nBinLen)
{
	String strRtn = "";

	INT32 nIdx = 0;
	while(nIdx < nBinLen)
	{
		strRtn += BinToHex(lpBin[nIdx++]);
	}
	return strRtn;
}
//---------------------------------------------------------------------------

PBYTE	HexToBinLen(LPCTSTR lpHex, PBYTE lpBin, INT32 nBinLen)
{
	INT32 nPosHex = 0;
	INT32 nHexLen = strlen(lpHex);
	INT32 nIndx = 0;

	while(nPosHex < nHexLen && nIndx < nBinLen)
	{
		CHAR szHexCell[3] = {0, };		
		memcpy(szHexCell, lpHex + nPosHex, 2);	

		lpBin[nIndx] = HexToBin(szHexCell);

		nIndx += 1;
		nPosHex += 2;
	}
	return lpBin;
}
//---------------------------------------------------------------------------

INT32	CompareText(LPCTSTR strA, LPCTSTR strB)
{
	return strcmp(strA, strB);
}
//---------------------------------------------------------------------------

INT32	CompareValue(INT32 nA, INT32 nB)
{
	if(nA == nB)		return 0;
	if(nA > nB)			return -1;
	return 1;
}
//---------------------------------------------------------------------------

INT32	CompareValue(UINT32 nA, UINT32 nB)
{
	if(nA == nB)		return 0;
	if(nA > nB)			return -1;
	return 1;
}
//---------------------------------------------------------------------------

INT32	CompareAddr(UINT32 nA, UINT32 nB)
{
	UINT32 nAA = htonl_ext(nA);
	UINT32 nBB = htonl_ext(nB);

	if(nAA == nBB)		return 0;
	if(nAA > nBB)		return -1;
	return 1;
}
//---------------------------------------------------------------------------

INT32	CompareValue(UINT64 nA, UINT64 nB)
{
	if(nA == nB)		return 0;
	if(nA > nB)			return -1;
	return 1;
}
//---------------------------------------------------------------------------

INT32	CompareVersion(String strVerA, String strVerB)
{
	INT32 nVerALen = strVerA.length();
	INT32 nVerBLen = strVerB.length();

	INT32 nVerAPos = 0;
	INT32 nVerBPos = 0;
	
	INT32 nRtn = 0;
	
	while(!nRtn)
	{
		String strVerAVal = "", strVerBVal = "";	
		while(nVerAPos < nVerALen)
		{
			CHAR chChkVal = strVerA.at(nVerAPos++);
			if(chChkVal != '.')		
			{
				strVerAVal += SPrintf("%c", chChkVal);
				continue;
			}
			break;
		}
		while(nVerBPos < nVerBLen)
		{
			CHAR chChkVal = strVerB.at(nVerBPos++);
			if(chChkVal != '.')		
			{
				strVerBVal += SPrintf("%c", chChkVal);
				continue;
			}
			break;
		}

		if(strVerAVal.empty() && strVerBVal.empty())
		{
			break;
		}
		else if(strVerAVal.empty())
		{
			nRtn = -1;
			break;
		}
		else if(strVerBVal.empty())
		{
			nRtn = 1;
			break;
		}

		INT32 nA = StrToIntExt(strVerAVal.c_str());
		INT32 nB = StrToIntExt(strVerBVal.c_str());

		nRtn = CompareValue(nA, nB);
	}

	return nRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

UINT64	GetSurplusBit64(UINT64 nMax, UINT64 nChk)
{
	UINT64 nRtn = 0;
	UINT64 nChkHex = 1;
	INT32 nIdx = 0;
	while(nIdx < 64)
	{
		if(((nMax & nChkHex) == 0) && (nChk & nChkHex))
		{
			nRtn += nChkHex;
		}

		nChkHex *= 2;
		nIdx += 1;
	}
	return nRtn;
}
//---------------------------------------------------------------------------

UINT32	GetSurplusBit32(UINT32 nMax, UINT32 nChk)
{
	UINT32 nRtn = 0;
	UINT32 nChkHex = 1;
	INT32 nIdx = 0;
	while(nIdx < 32)
	{
		if(((nMax & nChkHex) == 0) && (nChk & nChkHex))
		{
			nRtn += nChkHex;
		}

		nChkHex *= 2;
		nIdx += 1;
	}
	return nRtn;
}
//---------------------------------------------------------------------------

UINT16	GetSurplusBit16(UINT16 nMax, UINT16 nChk)
{
	UINT16 nRtn = 0;
	UINT16 nChkHex = 1;
	INT32 nIdx = 0;
	while(nIdx < 16)
	{
		if(((nMax & nChkHex) == 0) && (nChk & nChkHex))
		{
			nRtn += nChkHex;
		}

		nChkHex *= 2;
		nIdx += 1;
	}
	return nRtn;
}
//---------------------------------------------------------------------------

INT32 max(INT32 a, INT32 b)
{
	return a>b ? a : b;
}

INT32 min(INT32 a, INT32 b)
{
	return a<b ? a : b;
}
