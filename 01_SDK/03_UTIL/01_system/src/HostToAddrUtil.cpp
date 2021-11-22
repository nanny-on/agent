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

//---------------------------------------------------------------------------


#include "stdafx.h"
#include "com_struct.h"
#include "HostToAddrUtil.h"

CHostToAddrUtil*	t_HostToAddrUtil = NULL;

//--------------------------------------------------------------------

CHostToAddrUtil::CHostToAddrUtil()
{
	m_bInitWinSock = FALSE;
	InitWinSock();
}
//---------------------------------------------------------------------------

CHostToAddrUtil::~CHostToAddrUtil()
{
	UnInitWinSock();
}
//---------------------------------------------------------------------------

void CHostToAddrUtil::InitWinSock()
{
	m_bInitWinSock = TRUE;

}
//---------------------------------------------------------------------------

void CHostToAddrUtil::UnInitWinSock()
{
	if (m_bInitWinSock)
	{
//		WSACleanup();
		m_bInitWinSock = FALSE;
	}
}
//---------------------------------------------------------------------------

LPCTSTR		CHostToAddrUtil::ConvertHostToAddr(LPCTSTR lpHost, LPTSTR lpAddr, INT32 nAddrLen)
{
	if (!m_bInitWinSock)
		return LPCTSTR(lpAddr);

    memset(lpAddr, 0, nAddrLen);
	strncpy(lpAddr, lpHost, nAddrLen);

	struct hostent * host_addr = gethostbyname(lpHost);
    if (host_addr==NULL)
    	return (LPCTSTR)lpAddr;

	struct in_addr ia;
	ia.s_addr = *((int*)*host_addr->h_addr_list);
	
	memset(lpAddr, 0, nAddrLen);
    strncpy(lpAddr, inet_ntoa(ia), nAddrLen -1);

	return (LPCTSTR)lpAddr;
}
//---------------------------------------------------------------------------

BOOL		CHostToAddrUtil::IsIdlePort(UINT32 nPort, DWORD& dwWsaError)
{
	int nSocket = 0;
	sockaddr_in addr;
	BOOL bRet = FALSE;

	if (!m_bInitWinSock)
		return FALSE;

	nSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(nSocket < 0)
		return FALSE;

	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_family		= AF_INET;
	addr.sin_port		= htons(nPort);

	if (0 == bind(nSocket, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)))
		bRet = TRUE;
	else
		bRet = FALSE;

	close(nSocket);

	return bRet;
}


