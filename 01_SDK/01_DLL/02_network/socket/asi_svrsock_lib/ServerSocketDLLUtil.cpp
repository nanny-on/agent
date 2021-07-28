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

//---------------------------------------------------------------------------


#include "stdafx.h"
#include "com_struct.h"
#include "ServerSocketDLLUtil.h"

CSvrSocketUtil*	t_LinkSvrUtil;

//---------------------------------------------------------------------------

CSvrSocketUtil::CSvrSocketUtil()
{
    m_hServerSocketDll  = NULL;
	m_pEvtObject = NULL;
	m_nSvrSockType = SERVER_SOCKET_TYPE_EVENT;
	ClrSvrSockFuncAddr();
}
//---------------------------------------------------------------------------

CSvrSocketUtil::~CSvrSocketUtil()
{
	FreeLibraryExt();
}
//---------------------------------------------------------------------------

void    CSvrSocketUtil::ClrSvrSockFuncAddr()
{
	m_fStartServer_EVT = NULL;
	m_fStopServer_EVT = NULL;
	m_fFreeServer_EVT = NULL;
	m_fSendData_EVT = NULL;
	m_fStartRecv_EVT = NULL;
	m_fGetSocketAddressUINT_EVT = NULL;
	m_fGetSocketAddress_EVT = NULL;
	m_fIsExistsSessionID_EVT = NULL;
	m_fCloseSocket_EVT = NULL;	
	m_fEditSessionKey_EVT = NULL;
	m_fEditSessionID_EVT = NULL;
	m_fGetPkt_EVT = NULL;
	m_fIsExistsPkt_EVT = NULL;
	m_fEditLogFileName_EVT = NULL;
	m_fFreeMemory_EVT = NULL;
}


void    CSvrSocketUtil::FreeLibraryExt()
{
    if(m_hServerSocketDll)
    {
		if(m_pEvtObject)
		{
			if(m_fStopServer_EVT)
				m_fStopServer_EVT(m_pEvtObject, 1);
			if(m_fFreeServer_EVT)
            	m_fFreeServer_EVT(m_pEvtObject);
			m_pEvtObject = NULL;
		}

        FreeLibrary(m_hServerSocketDll);
		ClrSvrSockFuncAddr();
        m_hServerSocketDll = NULL;
    }
}
//---------------------------------------------------------------------------

INT32   CSvrSocketUtil::LoadLibraryExt(LPCTSTR szDLLPath)
{
    INT32 nRtn = 0;

	if(szDLLPath == NULL || szDLLPath[0] == 0)
		return -1;

    m_hServerSocketDll = LoadLibrary(szDLLPath);
    if(m_hServerSocketDll == NULL)
    {
        return -2;
    }
    
   	nRtn = LoadDll_Evt();
	if(nRtn != 0)
	{
		if(m_hServerSocketDll != NULL)
		{
			FreeLibrary(m_hServerSocketDll);
			ClrSvrSockFuncAddr();
			m_hServerSocketDll = NULL;
		}
	}
	return nRtn;    
}
//---------------------------------------------------------------------------


INT32	CSvrSocketUtil::LoadDll_Evt()
{

	m_fStartServer_EVT = (StartServer_EVTType)GetProcAddress(m_hServerSocketDll, "StartServer_EVT");
    if(!m_fStartServer_EVT)
    {
        return -10;
    }
    			
	m_fStopServer_EVT = (StopServer_EVTType)GetProcAddress(m_hServerSocketDll, "StopServer_EVT");
    if(!m_fStopServer_EVT)
    {
        return -11;
    }

    m_fFreeServer_EVT = (FreeServer_EVTType)GetProcAddress(m_hServerSocketDll, "FreeServer_EVT");
    if(!m_fFreeServer_EVT)
    {
        return -12;
    }

    m_fGetSocketAddress_EVT = (GetSocketAddress_EVTType)GetProcAddress(m_hServerSocketDll, "GetSocketAddress_EVT");
    if(!m_fGetSocketAddress_EVT)
    {
        return -13;
    }

    m_fGetPkt_EVT = (GetPkt_EVTType)GetProcAddress(m_hServerSocketDll, "GetPkt_EVT");
    if(!m_fGetPkt_EVT)
    {
        return -14;
    }

    m_fIsExistsPkt_EVT = (IsExistsPkt_EVTType)GetProcAddress(m_hServerSocketDll, "IsExistsPkt_EVT");
    if(!m_fIsExistsPkt_EVT)
    {
        return -15;
    }
	
    m_fEditSessionID_EVT = (EditSessionID_EVTType)GetProcAddress(m_hServerSocketDll, "EditSessionID_EVT");
    if(!m_fEditSessionID_EVT)
    {
        return -16;
    }

    m_fIsExistsSessionID_EVT = (IsExistsSessionID_EVTType)GetProcAddress(m_hServerSocketDll, "IsExistsSessionID_EVT");
    if(!m_fIsExistsSessionID_EVT)
    {
        return -17;
    }

    m_fSendData_EVT = (SendData_EVTType)GetProcAddress(m_hServerSocketDll, "SendData_EVT");
    if(!m_fSendData_EVT)
    {
        return -18;
    }

    m_fEditSessionKey_EVT = (EditSessionKey_EVTType)GetProcAddress(m_hServerSocketDll, "EditSessionKey_EVT");
    if(!m_fEditSessionKey_EVT)
    {
        return -19;
    }

    m_fEditLogFileName_EVT = (EditLogFileName_EVTType)GetProcAddress(m_hServerSocketDll, "EditLogFileName_EVT");
    if(!m_fEditLogFileName_EVT)
    {
        return -20;
    }

    m_fCloseSocket_EVT = (CloseSocket_EVTType)GetProcAddress(m_hServerSocketDll, "CloseSocket_EVT");
    if(!m_fCloseSocket_EVT)
    {
        return -21;
    }

    m_fFreeMemory_EVT = (FreeMemory_EVTType)GetProcAddress(m_hServerSocketDll, "FreeMemory_EVT");
    if(!m_fFreeMemory_EVT)
    {
        return -22;
    }

    m_fGetSocketAddressUINT_EVT = (GetSocketAddressUINT_EVTType)GetProcAddress(m_hServerSocketDll, "GetSocketAddressUINT_EVT");
    if(!m_fGetSocketAddressUINT_EVT)
    {
        return -23;
    }

    m_fIsRunSvr_EVT = (FreeServer_EVTType)GetProcAddress(m_hServerSocketDll, "IsRunSvr_EVT");
    if(!m_fIsRunSvr_EVT)
    {
        return -24;
    }


    return 0;
}
//---------------------------------------------------------------------------

UINT32	CSvrSocketUtil::GetIPByID(INT32 nSessionID)
{
	UINT32 nAddress = 0;
	GetSocketAddressUINT(nSessionID, nAddress);
	return nAddress;
}
//---------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
INT32	CSvrSocketUtil::StartServer(PSOCK_INIT_DATA pSID, PSOCK_NOTIFY_MSG pSNM)
{
	INT32 nRtn = 0;

	if(pSID == NULL || m_fStartServer_EVT == NULL)
		return -1;
	if(m_pEvtObject == NULL)
	{
		nRtn = m_fStartServer_EVT(&m_pEvtObject, pSID, sizeof(SOCK_INIT_DATA), pSNM, sizeof(SOCK_NOTIFY_MSG));
	    if(nRtn != 0 || m_pEvtObject == NULL)
	    {
			return nRtn;
	    }
	}	
    return 0;
}
//---------------------------------------------------------------------------
INT32	CSvrSocketUtil::StopServer(INT32 nDeleteRemainPkt)
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fStartServer_EVT == NULL)
		return -1;	
	nRtn = 	m_fStopServer_EVT(m_pEvtObject, nDeleteRemainPkt);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::FreeServer()
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fFreeServer_EVT == NULL)
		return -1;		
	nRtn = m_fFreeServer_EVT(m_pEvtObject);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::IsRunSvr()
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fIsRunSvr_EVT == NULL)
		return -1;		
	nRtn = m_fIsRunSvr_EVT(m_pEvtObject);
}


INT32	CSvrSocketUtil::SendData(INT32 nSessionID, UINT16 type, UINT16 code, UINT32 length, PVOID data)
{
    INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fSendData_EVT == NULL)
		return -1;		
	nRtn = m_fSendData_EVT(m_pEvtObject, nSessionID, type, code, length, data);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::StartRecv(INT32 nSessionID)
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fStartRecv_EVT == NULL)
		return -1;
	nRtn = m_fStartRecv_EVT(m_pEvtObject, nSessionID);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::GetSocketAddressUINT(INT32 nSessionID, UINT32& nAddress)
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fGetSocketAddressUINT_EVT == NULL)
		return -1;
	nRtn = m_fGetSocketAddressUINT_EVT(m_pEvtObject, nSessionID, nAddress);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::GetSocketAddress(INT32 nSessionID, OUT LPTSTR pszAddress)
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fGetSocketAddress_EVT == NULL)
		return -1;
	nRtn = m_fGetSocketAddress_EVT(m_pEvtObject, nSessionID, pszAddress);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::EditSessionKey(INT32 nSessionID, UINT32 nSessionKey)
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fGetSocketAddress_EVT == NULL)
		return -1;

	nRtn = m_fEditSessionKey_EVT(m_pEvtObject, nSessionID, nSessionKey);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::EditSessionID(INT32 nOldID, INT32 nNewID)
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fEditSessionID_EVT == NULL)
		return -1;
	nRtn = m_fEditSessionID_EVT(m_pEvtObject, nOldID, nNewID);
    return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::IsExistsSessionID(INT32 nSessionID)
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fIsExistsSessionID_EVT == NULL)
		return -1;

	nRtn = m_fIsExistsSessionID_EVT(m_pEvtObject, nSessionID);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::GetPkt(PKT_DATA& pkt_data)
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fGetPkt_EVT == NULL)
		return -1;
	nRtn = m_fGetPkt_EVT(m_pEvtObject, pkt_data);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::IsExistsPkt()
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fIsExistsPkt_EVT == NULL)
		return -1;

	nRtn = m_fIsExistsPkt_EVT(m_pEvtObject);
	return nRtn;
}
//---------------------------------------------------------------------------


INT32	CSvrSocketUtil::FreeMemory(void* lpVoid)
{
	INT32 nRtn = 0;
	if(m_fFreeMemory_EVT == NULL)
		return -1;

	nRtn = m_fFreeMemory_EVT(m_pEvtObject, lpVoid);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::CloseSocket(INT32 nSessionID, INT32 nRemainEvt)
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fCloseSocket_EVT == NULL)
		return -1;

	nRtn = m_fCloseSocket_EVT(m_pEvtObject, nSessionID, nRemainEvt);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::EditLogFileName(LPCTSTR lpLogFilePath, LPCTSTR lpLogFileName)
{
	INT32 nRtn = 0;
	if(m_pEvtObject == NULL || m_fEditLogFileName_EVT == NULL)
		return -1;

	nRtn = m_fEditLogFileName_EVT(m_pEvtObject, lpLogFilePath, lpLogFileName);
	return nRtn;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::SendFileBuffer(INT32 nSessionID, UINT16 type, UINT16 code, UINT32 length, PVOID lpBuf)
{
	return -1;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::SendFileName(INT32 nSessionID, UINT16 type, UINT16 code, LPCTSTR lpFileName, INT32 nOnceBlockSize)
{
	return -1;
}
//---------------------------------------------------------------------------

INT32	CSvrSocketUtil::SendFileHandle(INT32 nSessionID, UINT16 type, UINT16 code, LPCTSTR lpFileName, INT32 nAutoChangeMethod)	
{
	return -1;
}
//---------------------------------------------------------------------------

