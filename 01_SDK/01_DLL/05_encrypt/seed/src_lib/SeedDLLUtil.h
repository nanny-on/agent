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
 * * 
 */

// SeedDLLUtil.h: interface for the CSeedDLLUtil class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SEED_DLL_UTIL_H__
#define _SEED_DLL_UTIL_H__

typedef INT32 ( *Set_KeyType)		(BYTE* lpKey);
typedef INT32 ( *Make_KeyType)		(BYTE* lpKey);
typedef INT32 ( *Encrypt_DataType)	(BYTE* lpData, INT32 nLen, BYTE** lpOutBuf, INT32& nOutLen);
typedef INT32 ( *Decrypt_DataType)	(BYTE* lpData, BYTE** lpOutBuf, INT32& nOutLen);
typedef INT32 ( *Free_BufferType)	(BYTE* lpBuff);

#define SEED_DEFAULT_KEY_1	0xFA123679
#define SEED_DEFAULT_KEY_2	0xEA1B9C1F
#define SEED_DEFAULT_KEY_3	0x9A469328
#define SEED_DEFAULT_KEY_4	0x59372675

class CSeedDLLUtil * 
{
private:
	HINSTANCE			m_hDll;
	Set_KeyType			f_Set_Key;
	Make_KeyType		f_Make_Key;
	Encrypt_DataType	f_Encrypt_Data;
	Decrypt_DataType	f_Decrypt_Data;
	Free_BufferType		f_Free_Buffer;

private:
	void			CopyIntToMem(PVOID lpMemory, UINT32 nValue);

public:
	INT32			LoadLibraryExt(LPCTSTR lpDllPath);
	INT32			FreeLibraryExt();

public:
	INT32 * SetKey();
	INT32			SetKey(BYTE* lpKey);
	INT32			MakeKey(BYTE* lpKey);
	INT32			EncData(BYTE* lpData, INT32 nLen, BYTE** lpOutBuf, INT32& nOutLen);
	INT32			DecData(BYTE* lpData, BYTE** lpOutBuf, INT32& nOutLen);
	INT32			FreeBuffer(BYTE* lpBuff);

public:
	CSeedDLLUtil();
	virtual ~CSeedDLLUtil();

};

extern CSeedDLLUtil*	t_SeedDLLUtil;

#endif // _SEED_DLL_UTIL_H__