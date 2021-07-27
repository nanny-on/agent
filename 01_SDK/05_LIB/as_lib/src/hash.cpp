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
#include "as_sha1.h"
#include "as_sha256.h"
#include "as_file.h"
#include "as_hash.h"

//-----------------------------------------------------------------
INT32 SHAString(INT32 nLenType, LPCSTR lpString, INT32 nInLen, LPTSTR lpBuffer, INT32 nOutLen)
{
	INT32 nRetVal = 0;
	if(lpString == NULL || nInLen < 1 || lpBuffer == NULL || nOutLen < 1)
	{
		return -1;
	}

	if(nLenType == ASIHASH_SHA_LEN_TYPE_128)
	{
		get_sha1_hash((char *)lpString, nInLen, lpBuffer, nOutLen);
	}
	else if(nLenType == ASIHASH_SHA_LEN_TYPE_256)
	{
		nRetVal = get_sha256_hash((char *)lpString, nInLen, lpBuffer, nOutLen);
	}
	return nRetVal;
}
//-----------------------------------------------------------------

INT32 SHAByte(INT32 nLenType, LPCSTR lpString, INT32 nInLen, BYTE* lpBuffer, INT32 nOutLen)
{
	INT32 nRetVal = 0;
	if(lpString == NULL || nInLen < 1 || lpBuffer == NULL || nOutLen < 1)
	{
		return -1;
	}

	if(nLenType == ASIHASH_SHA_LEN_TYPE_128)
	{
		nRetVal = get_sha1_digest((char *)lpString, nInLen, lpBuffer, nOutLen);
	}
	else if(nLenType == ASIHASH_SHA_LEN_TYPE_256)
	{
		nRetVal = get_sha256_digest((char *)lpString, nInLen, lpBuffer, nOutLen);
	}

	return nRetVal;
}
//-----------------------------------------------------------------

INT32 	SHAFileLen(INT32 nLenType, LPCSTR lpFileName)
{
	INT32 nFileSize = 0;
	if(lpFileName == NULL)
		return 0;
	if(get_file_size(lpFileName, (UINT32 *)&nFileSize) != 0)
		return 0;

	return nFileSize;
}
//-----------------------------------------------------------------

INT32 	SHAFile(INT32 nLenType, LPCSTR lpFileName, LPTSTR lpBuffer, INT32 nOutLen)
{
	INT32 nRetVal = 0;

	if(lpFileName == NULL || lpBuffer == NULL || nOutLen < 1)
	{
		return -1;
	}

	if(nLenType == ASIHASH_SHA_LEN_TYPE_128)
	{
		nRetVal = get_sha1_hash_from_file((char *)lpFileName, lpBuffer, nOutLen);
	}
	else if(nLenType == ASIHASH_SHA_LEN_TYPE_256)
	{
		nRetVal = get_sha256_hash_from_file((char *)lpFileName, lpBuffer, nOutLen);
	}
	return nRetVal;
}

