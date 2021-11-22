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


#ifndef COM_HEADER_HASH_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_HASH_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


//////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#define ASIHASH_SHA_LEN_TYPE_128		0
#define ASIHASH_SHA_LEN_TYPE_256		1
#define ASIHASH_SHA_LEN_TYPE_512		2

#define ASIHASH_SHA_TYPE_128_LEN		40
#define ASIHASH_SHA_TYPE_256_LEN		64
#define ASIHASH_SHA_TYPE_512_LEN		128

//---------------------------------------------------------------------------

typedef INT32 (*ASIHASH_SHAStringType)(INT32 nLenType, LPCTSTR lpString, INT32 nLen, LPTSTR lpBuffer);
typedef INT32 (*ASIHASH_SHAByteType)(INT32 nLenType, LPCTSTR lpString, INT32 nLen, BYTE* lpBuffer);
typedef INT32 (*ASIHASH_SHAFileLenType)(INT32 nLenType, LPCTSTR lpFileName);
typedef INT32 (*ASIHASH_SHAFileType)(INT32 nLenType, LPCTSTR lpFileName, LPTSTR lpBuffer, BYTE* lpFileBuff);

typedef INT32 (*ASIHASH_MD5StringType)(LPCTSTR lpString, INT32 nLen, LPTSTR lpBuffer);
typedef INT32 (*ASIHASH_MD5FileLenType)(LPCTSTR lpFileName);
typedef INT32 (*ASIHASH_MD5FileType)(LPCTSTR lpFileName, LPTSTR lpBuffer);

//---------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////

#endif //COM_HEADER_HASH_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
