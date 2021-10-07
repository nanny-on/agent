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

#ifndef _AS_HASH_H_
#define _AS_HASH_H_

#define ASIHASH_SHA_LEN_TYPE_128		0
#define ASIHASH_SHA_LEN_TYPE_256		1
#define ASIHASH_SHA_LEN_TYPE_512		2

#define ASIHASH_SHA_TYPE_128_LEN		40
#define ASIHASH_SHA_TYPE_256_LEN		64
#define ASIHASH_SHA_TYPE_512_LEN		128

extern INT32 SHAString(INT32 nLenType, LPCSTR lpString, INT32 nInLen, LPTSTR lpBuffer, INT32 nOutLen);
extern INT32 SHAByte(INT32 nLenType, LPCSTR lpString, INT32 nInLen, BYTE* lpBuffer, INT32 nOutLen);
extern INT32 SHAFileLen(INT32 nLenType, LPCSTR lpFileName);
extern INT32 SHAFile(INT32 nLenType, LPCSTR lpFileName, LPTSTR lpBuffer, INT32 nOutLen);

#endif /* _AS_HASH_H_ */


