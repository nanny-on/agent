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

// Server_DLL.cpp : Defines the entry poINT32 for the DLL application.
//

#include "stdafx.h"
#include "com_struct.h"
#include "Server_DLL.h"
#include "TCP_EventServer.h"


INT32	StartServer_EVT(ASI_SVR_SOCKET* lpObject, PSOCK_INIT_DATA pSID, UINT32 nSIDSize, PSOCK_NOTIFY_MSG pSNM, UINT32 nSNMSize)
{
	CTCP_EventServer* pTCP_EventSvr = NULL;
	SOCK_NOTIFY_MSG tSNM;
	SOCK_INIT_DATA tSID;
	INT32 nRetVal = 0;

	if(lpObject == NULL || pSID == NULL)
		return -1001;

	memcpy(&tSID, pSID, sizeof(tSID));

	if(pSNM)
		memcpy(&tSNM, pSNM, sizeof(tSNM));
	else
		memset(&tSNM, 0, sizeof(tSNM));
	
	pTCP_EventSvr = new CTCP_EventServer();
	if(pTCP_EventSvr == NULL)
		return -1002;
	
	*lpObject = (ASI_SVR_SOCKET)pTCP_EventSvr;
	
	if(pTCP_EventSvr->IsRunSvr())
		return 0;
	
	nRetVal = pTCP_EventSvr->StartServer(tSID.szOpenAddress, tSID.nOpenPort, tSNM, tSID);
	if(nRetVal != 0)
	{
		delete pTCP_EventSvr;
		*lpObject = NULL;
	}
	return nRetVal;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	StopServer_EVT(ASI_SVR_SOCKET lpObject, INT32 nDeleteRemainPkt)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;
		
		if(!TCP_EventSvr || !TCP_EventSvr->IsRunSvr())
			return 0;

		TCP_EventSvr->StopServer(nDeleteRemainPkt);
		return 0;
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	FreeServer_EVT(ASI_SVR_SOCKET  lpObject)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;
		
		if(TCP_EventSvr->IsRunSvr())
			return 0;

		delete TCP_EventSvr;
		return 0;
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}

INT32	IsRunSvr_EVT(ASI_SVR_SOCKET  lpObject)
{
	CTCP_EventServer* pTCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		pTCP_EventSvr = (CTCP_EventServer*)lpObject;
		return pTCP_EventSvr->IsRunSvr();
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------

INT32	SendData_EVT(ASI_SVR_SOCKET lpObject, INT32 sessionid, UINT16 type, UINT16 code, UINT32 len, PVOID data)
{
	CTCP_EventServer* pTCP_EventSvr = NULL;
	if(!lpObject)
	{
		return -1000;
	}
	try
	{
		pTCP_EventSvr = (CTCP_EventServer*)lpObject;
		if(!pTCP_EventSvr->IsRunSvr())
		{
			return -2;
		}

		return pTCP_EventSvr->SendData(sessionid, type, code, len, data);
	}
	catch(...)
	{		
		return -100;
	}
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------

INT32	GetSocketAddressUINT_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID, UINT32& nAddress)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		if(!TCP_EventSvr->IsRunSvr())
			return -2;

		return TCP_EventSvr->GetSocketAddressUINTWithLock(nSessionID, nAddress);
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	GetSocketAddress_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID, LPSTR lpAddress)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		if(!TCP_EventSvr->IsRunSvr())
			return -2;

		return TCP_EventSvr->GetSocketAddressWithLock(nSessionID, lpAddress);
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	GetPkt_EVT(ASI_SVR_SOCKET     lpObject, PKT_DATA& pkt_data)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		if(!TCP_EventSvr->IsRunSvr())
			return -2;

		return TCP_EventSvr->GetPktWithPktLock(pkt_data);
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	IsExistsPkt_EVT(ASI_SVR_SOCKET       lpObject)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		if(!TCP_EventSvr->IsRunSvr())
			return -2;

		return TCP_EventSvr->IsExistsPktWithPktLock();
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	IsExistsSessionID_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		if(!TCP_EventSvr->IsRunSvr())
			return -2;

		return TCP_EventSvr->IsExistsSessionIDWithLock(nSessionID);
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	GetConnectInfo_EVT(ASI_SVR_SOCKET lpObject, INT32& nCurrentConnectNum, INT32& nTotalConnectNum)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		if(!TCP_EventSvr->IsRunSvr())
			return -2;

		return TCP_EventSvr->GetConnectInfoWithLock(nCurrentConnectNum, nTotalConnectNum);
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	CloseSocket_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID, INT32 nRemainEvt)	
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		if(!TCP_EventSvr->IsRunSvr())
			return -2;

		//TCP_EventSvr->WriteLog("owner object request close socket : %d", nSessionID);

		return TCP_EventSvr->CloseSocket(nSessionID);
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	EditSessionKey_EVT(ASI_SVR_SOCKET lpObject, INT32 nSessionID, UINT32 nSessionKey)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		if(!TCP_EventSvr->IsRunSvr())
			return -2;

		return TCP_EventSvr->EditSessionKeyWithLock(nSessionID, nSessionKey);
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	EditSessionID_EVT(ASI_SVR_SOCKET lpObject, INT32 nOldID, INT32 nNewID)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		if(!TCP_EventSvr->IsRunSvr())
			return -2;

		return TCP_EventSvr->EditSessionIDWithLock(nOldID, nNewID);
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32	FreeMemory_EVT(ASI_SVR_SOCKET lpObject, void* lpvoid)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		return TCP_EventSvr->FreeMemory(lpvoid);;
	}
	catch(...)
	{
		return -100;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------

INT32 EditLogFileName_EVT(ASI_SVR_SOCKET lpObject, LPCTSTR lpLogFilePath, LPCTSTR lpLogFileName)
{
	CTCP_EventServer* TCP_EventSvr;
	if(!lpObject)	return -1000;
	try
	{
		TCP_EventSvr = (CTCP_EventServer*)lpObject;

		TCP_EventSvr->EditLogFileName_Ext(lpLogFilePath, lpLogFileName);
	}
	catch(...)
	{
		;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------------------


