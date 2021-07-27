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

#ifndef ServerSocketUtilH
#define ServerSocketUtilH
#include "com_header_socket_dll.h"
//---------------------------------------------------------------------------

class CSvrSocketUtil
{
public:
private:
    HINSTANCE       m_hServerSocketDll;
	PVOID			m_pEvtObject;
	UINT32			m_nSvrSockType;

private:
	StartServer_EVTType				m_fStartServer_EVT;
	StopServer_EVTType				m_fStopServer_EVT;
	FreeServer_EVTType				m_fFreeServer_EVT;
	IsRunSvr_EVTType				m_fIsRunSvr_EVT;
	SendData_EVTType				m_fSendData_EVT;
	StartRecv_EVTType				m_fStartRecv_EVT;
	GetSocketAddressUINT_EVTType	m_fGetSocketAddressUINT_EVT;
	GetSocketAddress_EVTType		m_fGetSocketAddress_EVT;
	IsExistsSessionID_EVTType		m_fIsExistsSessionID_EVT;
	CloseSocket_EVTType				m_fCloseSocket_EVT;	
	EditSessionKey_EVTType			m_fEditSessionKey_EVT;
	EditSessionID_EVTType			m_fEditSessionID_EVT;
	GetPkt_EVTType					m_fGetPkt_EVT;
	IsExistsPkt_EVTType				m_fIsExistsPkt_EVT;
	EditLogFileName_EVTType			m_fEditLogFileName_EVT;
	FreeMemory_EVTType				m_fFreeMemory_EVT;
    
public:
	INT32	StartServer(PSOCK_INIT_DATA pSID, PSOCK_NOTIFY_MSG pSNM);
	INT32	StopServer(INT32 nDeleteRemainPkt);
	INT32	FreeServer();
	INT32	IsRunSvr();
	INT32	SendData(INT32 nSessionID, UINT16 type, UINT16 code, UINT32 len, PVOID data);
	INT32	SendData(INT32 nSessionID, UINT16 type, UINT16 code, PVOID data, UINT32 len)	{return SendData(nSessionID, type, code, len, data);	};
	INT32	SendDataRtn(PPKT_DATA pkt_data, UINT32 len, PVOID data)							{return SendData(pkt_data->nSessionID, pkt_data->hdr.type, pkt_data->hdr.code, len, data);	};
	INT32	SendDataRtn(PPKT_DATA pkt_data, PVOID data, UINT32 len)							{return SendData(pkt_data->nSessionID, pkt_data->hdr.type, pkt_data->hdr.code, len, data);	};
	INT32	StartRecv(INT32 nSessionID);
	INT32	GetSocketAddressUINT(INT32 nSessionID, UINT32& nAddress);
	INT32	GetSocketAddress(INT32 nSessionID, LPTSTR lpAddress);
	INT32	EditSessionKey(INT32 nSessionID, UINT32 nSessionKey);
	INT32	EditSessionID(INT32 nOldID, INT32 nNewID);
	INT32	IsExistsSessionID(INT32 nSessionID);
	INT32	GetPkt(PKT_DATA& pkt_data);
	INT32	IsExistsPkt();
	INT32	FreeMemory(void* lpvoid);
	INT32	CloseSocket(INT32 nSessionID, INT32 nRemainEvt = 1);	
	INT32	EditLogFileName(LPCTSTR lpLogFilePath, LPCTSTR lpLogFileName);
	INT32	SendFileBuffer(INT32 nSessionID, UINT16 type, UINT16 code, UINT32 length, PVOID lpBuf);
	INT32	SendFileName(INT32 nSessionID, UINT16 type, UINT16 code, LPCTSTR lpFileName, INT32 nOnceBlockSize);
	INT32	SendFileHandle(INT32 nSessionID, UINT16 type, UINT16 code, LPCTSTR lpFileName, INT32 nAutoChangeMethod);	
	
private:
	INT32	LoadDll_Evt();	
	void    ClrSvrSockFuncAddr();

public:
	UINT32	GetIPByID(INT32 nSessionID);

public:
	void	SetSvrSockMode(UINT32 nSvrSockMode = SERVER_SOCKET_TYPE_EVENT)	{	m_nSvrSockType = nSvrSockMode;	};
	INT32   LoadLibraryExt(LPCTSTR szDLLPath);
	void    FreeLibraryExt();
	
public:
    CSvrSocketUtil();
    ~CSvrSocketUtil();
};
//---------------------------------------------------------------------------

extern CSvrSocketUtil*	t_LinkSvrUtil;

#endif
