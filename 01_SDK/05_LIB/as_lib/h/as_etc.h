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
 * 
 */


#ifndef _AS_ETC_H__
#define	_AS_ETC_H__


INT32	SetER(INT32 nRtn);
String	GetGUID();
String GetOTPKey();
String GetOTPStr(UINT32 nTimeStamp, String strDefaultKey);
DWORD CoCreateGuid(GUID* pguid);
UINT32	htonl_ext(UINT32 nValue);
UINT16	htonl16_ext(UINT16 nValue);
UINT64	htonl64_ext(UINT64 nValue);
DWORD	GetFileSizeExt(LPCTSTR lpFileName);

PVOID	MallocExt(UINT32 nSize);


extern 	INT32	g_nErrRtn;

#endif /*_AS_ETC_H__*/