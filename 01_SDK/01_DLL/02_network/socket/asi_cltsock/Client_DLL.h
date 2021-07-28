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


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CLIENT_DLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CLIENT_DLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


//#include "socket_define_header.h"
#include "com_header_socket_dll.h"

// This class is exported from the Client_DLL.so

#define __declspec(dllexport)

extern "C" INT32 __declspec(dllexport)NewClient_CLD(PVOID *ppObject);
extern "C" INT32 __declspec(dllexport)FreeClient_CLD(PVOID pObject);
extern "C" INT32 __declspec(dllexport)StartClient_CLD(PVOID pObject, PSOCK_INIT_DATA pSID, UINT32 dwSIDSize, PSOCK_NOTIFY_MSG pSNM, UINT32 dwSNMSize, char *pClassName);
extern "C" INT32 __declspec(dllexport)Send_CLD(PVOID pObject, UINT16 nType, UINT16 nCode, UINT32 length , PVOID data);
extern "C" INT32 __declspec(dllexport)CloseSocket_CLD(PVOID pObject, INT32 nFlag);
extern "C" INT32 __declspec(dllexport)SetConnectTryFlag_CLD(PVOID pObject, INT32 nFlag);
extern "C" INT32 __declspec(dllexport)EditSessionKey_CLD(PVOID pObject, UINT32 nSessionKey);
extern "C" INT32 __declspec(dllexport)StopClient_CLD(PVOID pObject);
extern "C" INT32 __declspec(dllexport)GetPkt_CLD(ASI_CLT_SOCKET  lpObject, PKT_DATA& pkt_data);
extern "C" INT32 __declspec(dllexport)IsExistPkt_CLD(PVOID pObject);
extern "C" INT32 __declspec(dllexport)ClearAllPkt_CLD(PVOID pObject);
extern "C" INT32 __declspec(dllexport)EditNotifyMsgValue_CLD(PVOID pObject, SOCK_NOTIFY_MSG notify_msg_value);
extern "C" INT32 __declspec(dllexport)GetLocalAddress_CLD(PVOID pObject, UINT32& nIPAddress);
extern "C" INT32 __declspec(dllexport)FreeMemory_CLD(PVOID pObject, void* pVoid);
