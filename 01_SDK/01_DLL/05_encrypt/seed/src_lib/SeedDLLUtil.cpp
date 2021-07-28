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

// SeedDLLUtil.cpp: implementation of the CSeedDLLUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "SeedDLLUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeedDLLUtil*	t_SeedDLLUtil;

//--------------------------------------------------------------------------------------------

CSeedDLLUtil::CSeedDLLUtil()
{
	m_hDll = NULL;

	f_Set_Key		= 0;
	f_Make_Key		= 0;
	f_Encrypt_Data	= 0;
	f_Decrypt_Data	= 0;
	f_Free_Buffer	= 0;
}
//--------------------------------------------------------------------------------------------

CSeedDLLUtil::~CSeedDLLUtil()
{
	FreeLibraryExt();
}
//--------------------------------------------------------------------------------------------

INT32			CSeedDLLUtil::FreeLibraryExt()
{
	if(m_hDll)
	{
		FreeLibrary(m_hDll);
	}
	return 0;
}
//--------------------------------------------------------------------------------------------

INT32			CSeedDLLUtil::LoadLibraryExt(LPCTSTR lpDllPath)
{
	m_hDll = LoadLibrary(lpDllPath);
	if(!m_hDll)
		return -1;
	
	if( (f_Set_Key = (Set_KeyType)GetProcAddress(m_hDll, "Set_Key")) == NULL)
	{
		FreeLibraryExt();
		return -2;
	}
	
	if( (f_Make_Key = (Make_KeyType)GetProcAddress(m_hDll, "Make_Key")) == NULL)
	{
		FreeLibraryExt();
		return -3;
	}

	if( (f_Encrypt_Data = (Encrypt_DataType)GetProcAddress(m_hDll, "Encrypt_Data")) == NULL)
	{
		FreeLibraryExt();
		return -4;
	}

	if( (f_Decrypt_Data = (Decrypt_DataType)GetProcAddress(m_hDll, "Decrypt_Data")) == NULL)
	{
		FreeLibraryExt();
		return -5;
	}

	if( (f_Free_Buffer = (Free_BufferType)GetProcAddress(m_hDll, "Free_Buffer")) == NULL)
	{
		FreeLibraryExt();
		return -6;
	}
	
	return 0;
}
//--------------------------------------------------------------------------------------------
void CSeedDLLUtil::CopyIntToMem(PVOID lpMemory, UINT32 nValue)
{
	memcpy(lpMemory, &nValue, 4);
}
//--------------------------------------------------------------------------------------------

INT32			CSeedDLLUtil::SetKey()
{
	BYTE pDefaultKey[16] = {0, };
	CopyIntToMem(pDefaultKey,		SEED_DEFAULT_KEY_1);	
	CopyIntToMem(pDefaultKey + 4,	SEED_DEFAULT_KEY_2);	
	CopyIntToMem(pDefaultKey + 8,	SEED_DEFAULT_KEY_3);	
	CopyIntToMem(pDefaultKey + 12,	SEED_DEFAULT_KEY_4);	
	return SetKey(pDefaultKey);
}

//--------------------------------------------------------------------------------------------

INT32			CSeedDLLUtil::SetKey(BYTE* lpKey)
{
	if(!f_Set_Key)	return -1;
	return f_Set_Key(lpKey);
}
//--------------------------------------------------------------------------------------------

INT32			CSeedDLLUtil::MakeKey(BYTE* lpKey)
{
	if(!f_Make_Key)	return -1;
	return f_Make_Key(lpKey);
}
//--------------------------------------------------------------------------------------------

INT32			CSeedDLLUtil::EncData(BYTE* lpData, INT32 nLen, BYTE** lpOutBuf, INT32& nOutLen)
{
	if(!f_Encrypt_Data)	return -1;
	return f_Encrypt_Data(lpData, nLen, lpOutBuf, nOutLen);
}
//--------------------------------------------------------------------------------------------

INT32			CSeedDLLUtil::DecData(BYTE* lpData, BYTE** lpOutBuf, INT32& nOutLen)
{
	if(!f_Decrypt_Data)	return -1;
	return f_Decrypt_Data(lpData, lpOutBuf, nOutLen);
}
//--------------------------------------------------------------------------------------------

INT32			CSeedDLLUtil::FreeBuffer(BYTE* lpData)
{
	if(!f_Free_Buffer)	return -1;
	
	return f_Free_Buffer(lpData);
}
//--------------------------------------------------------------------------------------------

