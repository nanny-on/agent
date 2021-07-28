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
#ifndef StrEncryptH
#define StrEncryptH


#define PASS_HASH_READ_BUFF_SIZE 			1024
#define PASS_HASH_LENGTH 					SHA_DIGEST_LENGTH*2

class CStrEncrypt
{
public:
    static void ScrembleEncrypt(unsigned long key, unsigned char *SrcData, int SrcLen);
    static void ScrembleDecrypt(unsigned long key, unsigned char *SrcData, int SrcLen);
public:
	static void ScrembleEncrypt_Win(unsigned long key, unsigned char *SrcData, int SrcLen);
    static void ScrembleDecrypt_Win(unsigned long key, unsigned char *SrcData, int SrcLen);
public:
	CStrEncrypt();
	~CStrEncrypt();
};

#endif

