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

#ifndef _SERVER_DLL_H__

#define _SERVER_DLL_H__

#include "com_header_socket_dll.h"

#define __declspec(dllexport)

extern "C" INT32	__declspec(dllexport)StartServer_EVT(ASI_SVR_SOCKET* lpObject, PSOCK_INIT_DATA pSID, UINT32 nSIDSize, PSOCK_NOTIFY_MSG pSNM, UINT32 nSNMSize);
extern "C" INT32	__declspec(dllexport)StopServer_EVT(ASI_SVR_SOCKET lpObject, INT32 nDeleteRemainPkt);
extern "C" INT32	__declspec(dllexport)FreeServer_EVT(ASI_SVR_SOCKET  lpObject);
extern "C" INT32	__declspec(dllexport)IsRunSvr_EVT(ASI_SVR_SOCKET  lpObject);
extern "C" INT32	__declspec(dllexport)SendData_EVT(ASI_SVR_SOCKET lpObject, INT32 sessionid, UINT16 type, UINT16 code, UINT32 len, PVOID data);
extern "C" INT32	__declspec(dllexport)StartRecv_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID);
extern "C" INT32	__declspec(dllexport)GetSocketAddressUINT_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID, UINT32& nAddress);
extern "C" INT32	__declspec(dllexport)GetSocketAddress_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID, LPSTR lpAddress);
extern "C" INT32	__declspec(dllexport)IsExistsSessionID_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID);
extern "C" INT32	__declspec(dllexport)GetConnectInfo_EVT(ASI_SVR_SOCKET lpObject, INT32& nCurrentConnectNum, INT32& nTotalConnectNum);
extern "C" INT32	__declspec(dllexport)CloseSocket_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID, INT32 nRemainEvt);	
extern "C" INT32	__declspec(dllexport)EditSessionKey_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID, UINT32 nSessionKey);
extern "C" INT32	__declspec(dllexport)EditSessionID_EVT(ASI_SVR_SOCKET lpObject, INT32 nOldID, INT32 nNewID);
extern "C" INT32	__declspec(dllexport)GetPkt_EVT(ASI_SVR_SOCKET   lpObject, PKT_DATA& pkt_data);
extern "C" INT32	__declspec(dllexport)IsExistsPkt_EVT(ASI_SVR_SOCKET     lpObject);
extern "C" INT32	__declspec(dllexport)EditLogFileName_EVT(ASI_SVR_SOCKET      lpObject, LPCTSTR lpLogFilePath, LPCTSTR lpLogFileName);
extern "C" INT32	__declspec(dllexport)FreeMemory_EVT(ASI_SVR_SOCKET lpObject, void* lpvoid);

#endif /*_SERVER_DLL_H__*/