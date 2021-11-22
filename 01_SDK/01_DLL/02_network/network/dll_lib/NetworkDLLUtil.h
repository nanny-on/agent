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

// NetworkDLLUtil.h: interface for the CNetworkDLLUtil class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NETWORK_DLL_UTIL_H__
#define _NETWORK_DLL_UTIL_H__

typedef INT32   ( *GetInterfaceCountType)();
typedef INT32   ( *GetIPAddrType)(INT32 nIndex, LPSTR buf, INT32 bufLen);
typedef INT32   ( *GetMacAddressType)(INT32 nIndex, LPSTR buf, INT32 bufLen);
typedef INT32   ( *GetInterfaceSubNetType)(INT32 nIndex, LPSTR buf, INT32 bufLen);
typedef INT32	( *GetNetWorkGroupType)(LPSTR buf, INT32 nBufLen);
typedef INT32	( *GetHostByNameAddressType)(LPSTR buf, INT32 nBufLen); 
typedef INT32	( *GetSubNetByAddressType)(LPTSTR lpAddress, LPSTR lpSubNet, INT32 nLen);

class CNetworkDLLUtil  
{

private:
	HINSTANCE		m_hDLL;

public:
	INT32		LoadLibraryExt(LPCSTR lpDLLName);

private:
	GetInterfaceCountType		f_GetInterfaceCount;
	GetIPAddrType				f_GetIPAddr;
	GetMacAddressType			f_GetMacAddress;
	GetInterfaceSubNetType		f_GetInterfaceSubNet;
	GetNetWorkGroupType			f_GetNetWorkGroup;
	GetHostByNameAddressType	f_GetHostByNameAddress;
	GetSubNetByAddressType		f_GetSubNetByAddress;

public:
	INT32   GetInterfaceCount();
	INT32   GetIPAddr(INT32 nIndex, LPSTR buf, INT32 bufLen);
	INT32   GetMacAddress(INT32 nIndex, LPSTR buf, INT32 bufLen);
	INT32   GetInterfaceSubNet(INT32 nIndex, LPSTR buf, INT32 bufLen);
	INT32	GetNetWorkGroup(LPSTR buf, INT32 nBufLen);
	INT32	GetHostByNameAddress(LPSTR buf, INT32 nBufLen);
	INT32	GetSubNetByAddress(LPTSTR lpAddress, LPSTR lpSubNet, INT32 nLen);

public:
	CNetworkDLLUtil();
	virtual ~CNetworkDLLUtil();

};

extern	CNetworkDLLUtil*	t_NetworkDLLUtil;

#endif // !_NETWORK_DLL_UTIL_H__