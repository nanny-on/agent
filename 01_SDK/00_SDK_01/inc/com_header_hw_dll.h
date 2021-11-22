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



#ifndef     COM_HEADER_HW_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define     COM_HEADER_HW_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//------------------------------------------------------------------------------

typedef	INT32 (*ASIHW_GetCpuNameType)(LPTSTR lpName, UINT32 nBufLen);
//--------------------------------------------------------------------------

typedef INT32 (*ASIHW_GetHdSizeType)(UINT64& nSize);
typedef INT32 (*ASIHW_GetHdInfoType)(INT32 nDrive, UINT64& nTotal, UINT64& nAvail);
typedef INT32 (*ASIHW_GetHdNumberType)(UINT32 nNumber);
//--------------------------------------------------------------------------

typedef INT32 (*ASIHW_GetMemSizeType)(UINT64& nSize);
//--------------------------------------------------------------------------

typedef INT32 (*ASIHW_GetBIOSDTType)(LPTSTR lpDate, UINT32 nBufLen);
//--------------------------------------------------------------------------


#endif      //COM_HEADER_TS_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
