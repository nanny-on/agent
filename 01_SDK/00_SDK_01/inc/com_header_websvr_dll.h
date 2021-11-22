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

#ifndef	ASI_DLL_DEFINE_WSVR_H_91287755_D7A6_4F36_991E_AE92458D87CA
#define	ASI_DLL_DEFINE_WSVR_H_91287755_D7A6_4F36_991E_AE92458D87CA

//-----------------------------------------------------------------------------------------------

typedef struct _asi_web_svr_info
{
	_asi_web_svr_info()
	{
		nWebID			= 0;
		nSvrPort		= 0;
		nBWRxLimited	= 0;
		nBWTxLimited	= 0;
		nRemainLog		= 0;

		ZeroMemory(szSvrName, MAX_PATH);
		ZeroMemory(szHomePath, MAX_PATH);
		ZeroMemory(szDefaultPage, MAX_PATH);
		ZeroMemory(szLogPath, MAX_PATH);
		ZeroMemory(szLogFile, MAX_PATH);
	}
	UINT32			nWebID;
	CHAR			szSvrName[MAX_PATH];
	UINT16			nSvrPort;
	CHAR			szHomePath[MAX_PATH];
	CHAR			szDefaultPage[MAX_PATH];
	UINT32			nBWRxLimited;
	UINT32			nBWTxLimited;
	UINT32			nRemainLog;
	CHAR			szLogPath[MAX_PATH];
	CHAR			szLogFile[MAX_PATH];
}ASI_WEB_SVR_INFO, *PASI_WEB_SVR_INFO;

#define ASI_WEB_SVR_ENGINE	PVOID
//-----------------------------------------------------------------------------------------------

typedef INT32	( *ASIWSVR_StartWebServerType)(ASI_WEB_SVR_INFO* pAWSI, UINT32 nSize, ASI_WEB_SVR_ENGINE* lpCtx);
typedef INT32	( *ASIWSVR_StopWebServerType)(ASI_WEB_SVR_ENGINE lpCtx);
//-----------------------------------------------------------------------------------------------

#endif