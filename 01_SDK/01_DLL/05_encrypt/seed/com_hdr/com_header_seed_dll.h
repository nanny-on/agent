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
 * * 
 */


#ifndef COM_HEADER_SEED_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_SEED_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


//////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

typedef INT32 (*ASISEED_SetKeyType)(BYTE* lpKey);
typedef INT32 (*ASISEED_MakeKeyType)(BYTE* lpKey);
typedef INT32 (*ASISEED_EncType)(BYTE* lpData, INT32 nLen, BYTE** lpOutBuf, INT32& nOutLen);
typedef INT32 (*ASISEED_DecType)(BYTE* lpData, BYTE** lpOutBuf, INT32& nOutLen);
typedef INT32 (*ASISEED_EncDataType)(BYTE* lpData, INT32 nLen, BYTE** lpOutBuf, INT32& nOutLen);
typedef INT32 (*ASISEED_DecDataType)(BYTE* lpData, BYTE** lpOutBuf, INT32& nOutLen);
typedef INT32 (*ASISEED_FreeBufferType)(BYTE* lpBuff);

//////////////////////////////////////////////////////////////////////////

#endif //COM_HEADER_SEED_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
