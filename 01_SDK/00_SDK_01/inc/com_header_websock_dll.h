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

#ifndef     COM_HEADER_ASI_WEBSOCK_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define     COM_HEADER_ASI_WEBSOCK_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define		ASI_WEBSOCK_MSG_TYPE_CONNECT			1
#define		ASI_WEBSOCK_MSG_TYPE_DISCONNECT			2
#define		ASI_WEBSOCK_MSG_TYPE_RECV_MSG			10
#define		ASI_WEBSOCK_MSG_TYPE_SEND_MSG			20
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

typedef struct _asi_websock_init
{
	_asi_websock_init()
	{
		nRemainLog		= 1;

		ZeroMemory(szLogPath, MAX_PATH);
		ZeroMemory(szLogFile, MAX_PATH);

		hNotifyMsg		= NULL;
		nNotifyMsgID	= 0;
	}

	CHAR		szLogPath[MAX_PATH];
	CHAR		szLogFile[MAX_PATH];
	INT32		nRemainLog;

	HWND		hNotifyMsg;
	UINT32		nNotifyMsgID;

}ASI_WEBSOCK_INIT, *PASI_WEBSOCK_INIT;
//--------------------------------------------------------------------------

typedef struct _asi_websock_msg
{
	_asi_websock_msg()
	{
		nSvrPort	= 0;
		nCliPort	= 0;

		nMsgType	= 0;
		nMsgCode	= 0;

		lpMsgValue	= NULL;
		nMsgLength	= 0;
	}


	UINT32		nSvrPort;
	UINT32		nCliPort;

	UINT32		nMsgType;
	UINT32		nMsgCode;

	LPCTSTR		lpMsgValue;
	UINT32		nMsgLength;
}ASI_WEBSOCK_MSG, *PASI_WEBSOCK_MSG;
//--------------------------------------------------------------------------

typedef	INT32 (*fn_ASIWEBSOCK_InitType)(PVOID* lpObject, PASI_WEBSOCK_INIT pAWSI, UINT32 nSize);
typedef	INT32 (*fn_ASIWEBSOCK_FreeType)(PVOID lpObject);

typedef	INT32 (*fn_ASIWEBSOCK_StartSvrType)(PVOID lpObject, UINT32 nPort);
typedef	INT32 (*fn_ASIWEBSOCK_StopSvrType)(PVOID lpObject, UINT32 nPort);
typedef	INT32 (*fn_ASIWEBSOCK_IsRunSvrType)(PVOID lpObject, UINT32 nPort, UINT32& nRunMode);
typedef	INT32 (*fn_ASIWEBSOCK_ClearSvrType)(PVOID lpObject);

typedef	INT32 (*fn_ASIWEBSOCK_RecvMsgType)(PVOID lpObject, PASI_WEBSOCK_MSG pAWM);
typedef	INT32 (*fn_ASIWEBSOCK_SendMsgType)(PVOID lpObject, PASI_WEBSOCK_MSG pAWM);
//--------------------------------------------------------------------------

#endif      //COM_HEADER_ASI_WEBSOCK_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
