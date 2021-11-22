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

// Client_DLL.cpp : Defines the entry poINT32 for the DLL application.
//

#include "stdafx.h"
#include "com_struct.h"
#include "Client_DLL.h"
#include "TCP_Client.h"

int g_nNewCount = 0;

INT32 NewClient_CLD(PVOID *ppObject)
{
	CTCP_Client* pTCP_Client = NULL;
	pTCP_Client = (CTCP_Client*)new CTCP_Client();
	if(pTCP_Client == NULL)
	{
		return -1;
	}
	*ppObject = pTCP_Client;
	g_nNewCount++;
	return 0;	
}

INT32 FreeClient_CLD(PVOID pObject)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;
	delete pTCP_Client;
	g_nNewCount--;
	return 0;
}

INT32 StartClient_CLD(PVOID pObject, PSOCK_INIT_DATA pSID, UINT32 dwSIDSize, PSOCK_NOTIFY_MSG pSNM, UINT32 dwSNMSize, char *pClassName)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	SOCK_NOTIFY_MSG tSNM;
	SOCK_INIT_DATA tSID;
	
	if(pTCP_Client == NULL || pSID == NULL || pClassName == NULL)
		return -1;

	if(pSNM)
		memcpy(&tSNM, pSNM, sizeof(tSNM));
	else
		memset(&tSNM, 0, sizeof(tSNM));

	memcpy(&tSID, pSID, sizeof(tSID));	
	return pTCP_Client->StartClient(pSID->szOpenAddress, pSID->nOpenPort, tSNM, tSID, pClassName);
}
//------------------------------------------------------------------------------------------------------------------------

INT32 Send_CLD(PVOID pObject, UINT16 nType, UINT16 nCode, UINT32 length , PVOID data)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;
/*
	if(pTCP_Client->IsRunClient() == 0)
		return 0;
*/		
	return pTCP_Client->Send(nType, nCode, length, data);
}
//------------------------------------------------------------------------------------------------------------------------

INT32 CloseSocket_CLD(PVOID pObject, INT32 nFlag)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;
	if(pTCP_Client->IsRunClient() == 0)
		return 0;
	
	return pTCP_Client->CloseSocket(nFlag);
}
//------------------------------------------------------------------------------------------------------------------------

INT32 SetConnectTryFlag_CLD(PVOID pObject, INT32 nFlag)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;
	if(pTCP_Client->IsRunClient() == 0)
		return 0;
	
	return pTCP_Client->SetConnectTryFlag(nFlag);
}
//------------------------------------------------------------------------------------------------------------------------

INT32 EditSessionKey_CLD(PVOID pObject, UINT32 nSessionKey)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;
	if(pTCP_Client->IsRunClient() == 0)
		return 0;
	
	return pTCP_Client->EditSessionKey(nSessionKey);
}
//------------------------------------------------------------------------------------------------------------------------

INT32 StopClient_CLD(PVOID pObject)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;

	if(pTCP_Client->IsRunClient() == 0)
		return 0;
	
	return pTCP_Client->StopClient();
}
//------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------

INT32 GetPkt_CLD(PVOID pObject, PKT_DATA& pkt_data)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;

	return pTCP_Client->GetPktWithPktLock(pkt_data);
}
//------------------------------------------------------------------------------------------------------------------------

INT32 IsExistPkt_CLD(PVOID pObject)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;

	return pTCP_Client->IsExistPktWithPktLock();
}

INT32 ClearAllPkt_CLD(PVOID pObject)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;

	pTCP_Client->ClearAllPkt();
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------

INT32 EditNotifyMsgValue_CLD(PVOID pObject, SOCK_NOTIFY_MSG notify_msg_value)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;
	return pTCP_Client->EditNotifyMsgValue(notify_msg_value);
}
//------------------------------------------------------------------------------------------------------------------------

INT32 FreeMemory_CLD(PVOID pObject, void* pVoid)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;
	return pTCP_Client->FreeMemory(pVoid);
}
//------------------------------------------------------------------------------------------------------------------------

INT32 GetLocalAddress_CLD(PVOID pObject, UINT32& nIPAddress)
{
	CTCP_Client* pTCP_Client = (CTCP_Client*)pObject;
	if(pTCP_Client == NULL)
		return -1;

	return pTCP_Client->GetLocalAddress(nIPAddress);
}
//------------------------------------------------------------------------------------------------------------------------
