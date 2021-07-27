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

#ifndef _COM_HEADER_OTP_DLL_H__
#define _COM_HEADER_OTP_DLL_H__

typedef	INT32 (*ASIOTP_GenOtpRandomKeyType)(LPTSTR lpKey, INT32 nKeyLen);
typedef	INT32 (*ASIOTP_GenOtpValueType)(LPCTSTR lpKey, UINT32 nTimeKey, UINT32& nCode);
typedef	INT32 (*ASIOTP_ChkOtpValueType)(LPCTSTR lpKey, UINT32 nTimeKey, UINT32 nCode, INT32 nTimeArea);
typedef	INT32 (*ASIOTP_GenOtpQRCodeType)(LPCTSTR lpKey, LPCTSTR lpName, LPCTSTR lpEmail, PBYTE* pQRCode, INT32& nQRSize);
typedef	INT32 (*ASIOTP_FreeBufferType)(PBYTE* pQRCode);

#endif /*_COM_HEADER_OTP_DLL_H__*/



