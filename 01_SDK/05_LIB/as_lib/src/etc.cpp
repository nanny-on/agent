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
#include "as_string.h"
#include "as_file.h"

INT32		g_nErrRtn;

//---------------------------------------------------------------------------

INT32	SetER(INT32 nRtn)
{
	g_nErrRtn = nRtn;
    return nRtn;
}
//---------------------------------------------------------------------------

DWORD CoCreateGuid(GUID* pguid)
{
    uuid_t uuid;
    uuid_generate(uuid);

    char* puuid = (char*)&uuid;
    memcpy(pguid->Data4, &puuid[8], 8);
    memcpy(&pguid->Data3, &puuid[6], 2);
    memcpy(&pguid->Data2, &puuid[4], 2);
    memcpy(&pguid->Data1, puuid, 4);
	return 0;
}

String   GetGUID()
{
	String strRtn;
	GUID stGuid;
	GUID stGuidNull;
	memset(&stGuid, 0, sizeof(GUID));
	memset(&stGuidNull, 0, sizeof(GUID));
	CoCreateGuid(&stGuid);
	if(memcmp(&stGuid, &stGuidNull, sizeof(GUID)) == 0)
	{
		return "";
	}
	strRtn = SPrintf("%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", stGuid.Data1, stGuid.Data2, stGuid.Data3,
		stGuid.Data4[0], stGuid.Data4[1], stGuid.Data4[2], stGuid.Data4[3],
		stGuid.Data4[4], stGuid.Data4[5], stGuid.Data4[6], stGuid.Data4[7]);

	return strRtn;
}
//---------------------------------------------------------------------------

String	GetOTPKey()
{
	String strGUID;
	GUID stGuid;
	GUID stGuidNull;
	memset(&stGuid, 0, sizeof(GUID));
	memset(&stGuidNull, 0, sizeof(GUID));
	CoCreateGuid(&stGuid);
	if(memcmp(&stGuid, &stGuidNull, sizeof(GUID)) == 0)
	{
		return "";
	}

	strGUID = SPrintf("%08lX%04X%04X%02X%02X", stGuid.Data1, stGuid.Data2, stGuid.Data3,
		stGuid.Data4[0], stGuid.Data4[1]);

	return strGUID;
}
//-------------------------------------------------------------------------------------------

String GetOTPStr(UINT32 nTimeStamp, String strDefaultKey)
{
	INT32 nLen = strDefaultKey.length();
	INT32 nChkIdx = 0;
	String strTemp = strDefaultKey;
	String strRtn;
	while(nChkIdx < nLen)
	{
		CHAR szCur = strDefaultKey[nChkIdx];
		szCur += (BYTE)(nTimeStamp >> (nChkIdx % 32));
		if((nLen - nChkIdx))	szCur *= (nLen - nChkIdx);
		else					szCur += (nLen + nChkIdx);

		if(szCur == 0)
		{
			szCur = strDefaultKey[nChkIdx];
		}

		strTemp += SPrintf("%c", szCur);
		nChkIdx += 1;
	}

	nLen = strTemp.length();
	nChkIdx = 0;
	while(nChkIdx < (nLen / 2))
	{
		CHAR szCur = (strTemp[nChkIdx] + strTemp[nLen - nChkIdx - 1]);
		strRtn += SPrintf("%c", szCur);

		nChkIdx += 1;
	}
	return strRtn;
}
//-------------------------------------------------------------------------------------------

UINT32 htonl_ext(UINT32 nValue)
{
	UINT32 nRtn = 0;

	((BYTE*)&nRtn)[0] = BYTE(nValue >> 24);
	((BYTE*)&nRtn)[1] = BYTE(nValue >> 16);
	((BYTE*)&nRtn)[2] = BYTE(nValue >> 8);
	((BYTE*)&nRtn)[3] = BYTE(nValue);

	return nRtn;
}
//---------------------------------------------------------------------------

UINT16 htonl16_ext(UINT16 nValue)
{
	UINT16 nRtn = 0;

	((BYTE*)&nRtn)[0] = BYTE(nValue >> 8);
	((BYTE*)&nRtn)[1] = BYTE(nValue);

	return nRtn;
}
//---------------------------------------------------------------------------

UINT64	htonl64_ext(UINT64 nValue)
{
	UINT64 nRtn = 0;

	((BYTE*)&nRtn)[0] = BYTE(nValue >> 56);
	((BYTE*)&nRtn)[1] = BYTE(nValue >> 48);
	((BYTE*)&nRtn)[2] = BYTE(nValue >> 40);
	((BYTE*)&nRtn)[3] = BYTE(nValue >> 32);
	((BYTE*)&nRtn)[4] = BYTE(nValue >> 24);
	((BYTE*)&nRtn)[5] = BYTE(nValue >> 16);
	((BYTE*)&nRtn)[6] = BYTE(nValue >> 8);
	((BYTE*)&nRtn)[7] = BYTE(nValue);

	return nRtn;
}
//---------------------------------------------------------------------------

DWORD	GetFileSizeExt(LPCTSTR lpFileName)
{
	DWORD dwSize = 0;
	if(lpFileName == NULL)
		return 0;
	if(get_file_size(lpFileName, &dwSize) < 0)
		return 0;
	return dwSize;
}
//---------------------------------------------------------------------------

PVOID	MallocExt(UINT32 nSize)
{
	INT32 nWaitLimited = 10;

	PVOID pBuf = (PVOID)malloc(nSize + 1) ;
	if(!pBuf)
	{
		while(!pBuf && errno == ENOMEM && nWaitLimited--)
		{
			Sleep(100);
			pBuf = (PVOID)malloc(nSize + 1) ;
		}

		if(!pBuf)
		{
			return pBuf;
		}
	}

	memset(pBuf, 0, nSize + 1);
	return pBuf;
}
//---------------------------------------------------------------------------
