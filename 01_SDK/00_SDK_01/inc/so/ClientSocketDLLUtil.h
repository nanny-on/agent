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

//---------------------------------------------------------------------------

#ifndef ClientSocketUtil_H__
#define ClientSocketUtil_H__

#include "com_header_socket_dll.h"
//---------------------------------------------------------------------------

class CCltSocketUtil
{
private:
    HINSTANCE m_hClientSocketDll;
    PVOID m_pObject;
    INT32 m_nInitalize;
	NewClient_CLDType 			m_fNewClient_CLD;
    FreeClient_CLDType          m_fFreeClient_CLD;
    StartClient_CLDType         m_fStartClient_CLD;
    Send_CLDType                m_fSend_CLD;
    CloseSocket_CLDType         m_fCloseSocket_CLD;
    EditSessionKey_CLDType      m_fEditSessionKey_CLD;
    StopClient_CLDType          m_fStopClient_CLD;
	IsExistPkt_CLDType			m_fIsExistPkt_CLD;
	ClearAllPkt_CLDType			m_fClearAllPkt_CLD;
    GetPkt_CLDType              m_fGetPkt_CLD;
    EditNotifyMsgValueType      m_fEditNotifyMsgValue_CLD;
	GetLocalAddress_CLDType		m_fGetLocalAddress_CLD;
    FreeMemory_CLDType          m_fFreeMemory_CLD;
	char m_acClassName[MAX_TYPE_LEN];
private:
	void  ClearFuncAddr();

public:
	INT32 StartClient_CLD(PSOCK_INIT_DATA pSID, PSOCK_NOTIFY_MSG pSNM, char *pClassName);
    INT32 Send_CLD(UINT16 nType, UINT16 nCode, UINT32 length , PVOID data);
    INT32 Send_CLD(UINT16 nType, UINT16 nCode, PVOID data, UINT32 length);
    INT32 CloseSocket_CLD(INT32 nFlag);
    INT32 EditSessionKey_CLD(UINT32 nSessionKey);
    INT32 StopClient_CLD();
    INT32 FreeClient_CLD();
	INT32 IsExistPkt_CLD();
	INT32 ClearAllPkt_CLD();
    INT32 GetPkt_CLD(PKT_DATA& pkt_data);
    INT32 EditNotifyMsgValue_CLD(SOCK_NOTIFY_MSG notify_msg_value);
	INT32 GetLocalAddress_CLD(UINT32& nLocalAddress);
    INT32 FreeMemory_CLD(void* lpvoid);
	INT32 LoadLibraryExt(LPCTSTR strDLLPath, PSOCK_INIT_DATA pSID, PSOCK_NOTIFY_MSG pSNM, char *pClassName);
    INT32 LoadLibraryExt(LPCTSTR strDLLPath);
    void  FreeLibraryExt();
	void  SetInitialize(INT32 nFlag);
	INT32 IsInitialize();

public:
    String  strHost;
    UINT32  dwPort;
public:
    CCltSocketUtil();
    ~CCltSocketUtil();
};



extern CCltSocketUtil*	t_LgnCltUtil;
extern CCltSocketUtil*	t_MgrCltUtil;
extern CCltSocketUtil*	t_UdtCltUtil;

#endif /*ClientSocketUtil_H__*/
