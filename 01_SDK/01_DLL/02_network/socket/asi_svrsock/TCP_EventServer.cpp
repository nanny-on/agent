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

#include "stdafx.h"
#include "com_struct.h"
#include "TCP_EventServer.h"


typedef void (*pfnAuth_Console_Callback)(PPKT_DATA pkt_data);
static pfnAuth_Console_Callback LoginAuth = NULL;

CTCP_EventServer::CTCP_EventServer()
{
	memset(&m_tListenAddr, 0x00, sizeof(SOCKADDR_IN));
	memset(&m_tCliAddr, 0x00, sizeof(SOCKADDR_IN));

	m_nContinueServer         = 0;
	m_nRunServer              = 0;
	m_nTotalConnectNum        = -1;
	m_tAutoDisConLastChkTime  = 0;
	m_nGlobalID               = -10;
	m_tListenSocket           = -1;

	m_nMaxEvent               =  DEFAULT_SESSION_COUNT;
	m_pEpollEvents            = NULL;

	memset(m_LogFilePath, 0x00, MAX_PATH);
	memset(m_LogFileName, 0x00, MAX_PATH);

	pthread_mutex_init(&m_mutex, NULL);
	pthread_mutex_init(&m_pkt_mutex, NULL);
}
//---------------------------------------------------------------------------------------------------------------------------

CTCP_EventServer::~CTCP_EventServer()
{
	StopServer(1);
	pthread_mutex_destroy(&m_mutex);
	pthread_mutex_destroy(&m_pkt_mutex);
}
//---------------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::Bind(UINT16 nOpenPort)
{
	INT32 nFlags = 0;
	INT32 nUsedFlag = 1;
	INT32 nRetVal = 0;
	
	m_tListenAddr.sin_family = AF_INET;
	m_tListenAddr.sin_port = htons(nOpenPort);
	m_tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	m_tListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_tListenSocket == -1)
	{
		return -1;
	}
	do{
		if (m_init_data.nExtSockFlag != 1)
		{
			if ( (nFlags = fcntl(m_tListenSocket, F_GETFL, 0)) < 0)
			{
				nRetVal = -2;
				break;
			}

			if (fcntl(m_tListenSocket, F_SETFL, nFlags | O_NONBLOCK) < 0)
			{
				nRetVal = -3;
				break;
			}
		}

		
		if (setsockopt(m_tListenSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &nUsedFlag, sizeof(nUsedFlag)) == -1)
		{
			nRetVal = -4;
			break;
		}

		if (bind(m_tListenSocket, (struct sockaddr *) &m_tListenAddr, sizeof(m_tListenAddr)) == -1)
		{
			nRetVal = -5;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	if(nRetVal != 0)
	{
		close(m_tListenSocket);
		m_tListenSocket = -1;
	}
	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::Listen(UINT32 backlog)
{
	if(m_tListenSocket == -1)
	{
		return -1;
	}
	if (listen(m_tListenSocket, backlog) == -1)
	{
		close(m_tListenSocket);
		m_tListenSocket = -1;
		return -2;
	}

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::InitEpoll(DWORD dwMaxSession)
{
	struct epoll_event  tEvent;

	if(m_tListenSocket == -1)
	{
		return -1;
	}
	memset(&tEvent, 0, sizeof(tEvent));
	m_nMaxEvent = (int)dwMaxSession;
	if(m_nMaxEvent < 1)
	{
		return -2;
	}

	m_nEpollFd = epoll_create(m_nMaxEvent);
	if (m_nEpollFd == -1)
	{
		return -3;
	}

	m_pEpollEvents = (struct epoll_event *) calloc(m_nMaxEvent, sizeof(struct epoll_event));
	if (m_pEpollEvents == NULL)
	{
		close(m_nEpollFd);
		m_nEpollFd = -1;
		return -4;
	}

	tEvent.events	= EPOLLIN | EPOLLRDHUP;
	tEvent.data.fd  = m_tListenSocket;

	if (epoll_ctl(m_nEpollFd, EPOLL_CTL_ADD, m_tListenSocket, &tEvent) == -1)
	{
		close(m_nEpollFd);
		m_nEpollFd = -1;
		return -5;
	}

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------
void *CTCP_EventServer::ExtCliThreadSub(LPVOID lParam)
{
	int nCliSocket = 0;
	int nCloseSocket = 0;
	int nRet = 0;
	int nTimeOut = 30;
	INT32 nClntSessionID = 0;
	SOCKADDR_IN tCliAddr;
	EVT_SESSION* pevt_session = NULL;
	CTCP_EventServer* tTCP_EventSvr = (CTCP_EventServer *) lParam;     
	nCliSocket = tTCP_EventSvr->m_tCliSocket;
	tCliAddr = tTCP_EventSvr->m_tCliAddr;

	while (tTCP_EventSvr->m_nContinueServer) 
	{
		nCloseSocket = 0;
		nClntSessionID = tTCP_EventSvr->t_Sock2SessionMap[nCliSocket];
		pevt_session = tTCP_EventSvr->GetEvtSession(nClntSessionID);
		if(pevt_session)
		{
			nRet = tTCP_EventSvr->Recv(pevt_session, nCloseSocket, nTimeOut);
			if (nRet < 0)
				Sleep(100);
		}
		else
		{
			Sleep(100);
		}
		
		if (nCloseSocket == 1)
		{
			if (tTCP_EventSvr->m_init_data.nRemainSockEvt) 
			{
				SOCK_EVT_PROP sock_evt_prop;
				sock_evt_prop.nSessionID  = nClntSessionID;
				sock_evt_prop.nEventID    = ASI_SOCKET_EVENT_DISCONNECT;
				sock_evt_prop.nAddress    = tCliAddr.sin_addr.s_addr;
				tTCP_EventSvr->AddEvtWithPktLock(sock_evt_prop);
			}

			tTCP_EventSvr->DelSocketEventWithLock(nClntSessionID, nCliSocket);
			break;
		}
	}
	sleep(1);

	return (void*)0x00;
}

void *CTCP_EventServer::ExtAcceptThreadSub(LPVOID lParam)
{
	INT32 flags;
	INT32 nSockFd = 0, nListenFd;
	INT32 nAddrLen = 0;
	pthread_t thID = 0;
	INT32 nNewSessionID = 0;
	INT32 nClntSessionID = 0;
	CHAR szResult[32] = {0,};
	SOCK_EVT_PROP sock_evt_prop;	
	CTCP_EventServer* tTCP_EventSvr = (CTCP_EventServer *) lParam;

	if(tTCP_EventSvr == NULL)
	{
		return (void*)0x0;
	}
	pthread_detach(pthread_self());


	tTCP_EventSvr->m_nRunServer	+= 1;
	
	while (tTCP_EventSvr->m_nContinueServer)
	{
		nAddrLen = sizeof(tTCP_EventSvr->m_tCliAddr);

		if (tTCP_EventSvr->m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
		{
			tTCP_EventSvr->m_tCliSocket = accept(tTCP_EventSvr->m_tListenSocket, (struct sockaddr *)&tTCP_EventSvr->m_tCliAddr, (socklen_t*)&nAddrLen);
		}
		else
		{
			tTCP_EventSvr->m_tCliSocket = accept4(tTCP_EventSvr->m_tListenSocket, (struct sockaddr *)&tTCP_EventSvr->m_tCliAddr, (socklen_t*)&nAddrLen, SOCK_NONBLOCK);
		}
		if(nSockFd == -1)
		{
			sleep(1);
			continue;
		}

		if((nNewSessionID = tTCP_EventSvr->AddSocketEventWithLock(tTCP_EventSvr->m_tCliSocket, tTCP_EventSvr->m_tCliAddr)) == 0)
		{
			tTCP_EventSvr->CloseSingleSocket(tTCP_EventSvr->m_tCliSocket);
			tTCP_EventSvr->m_tCliSocket  = -1;
			sleep(1);
			continue;
		}

		if (tTCP_EventSvr->m_init_data.nRemainSockEvt) 
		{
			sock_evt_prop.nSessionID  = nNewSessionID;
			sock_evt_prop.nEventID    = ASI_SOCKET_EVENT_CONNECT;
			sock_evt_prop.nAddress    = tTCP_EventSvr->m_tCliAddr.sin_addr.s_addr;
			tTCP_EventSvr->AddEvtWithPktLock(sock_evt_prop);
		}

		memset(szResult, 0x00, sizeof(szResult));
		snprintf(szResult, sizeof(szResult)-1, "%d.%d.%d.%d", (int)((unsigned char*)&tTCP_EventSvr->m_tCliAddr.sin_addr.s_addr)[0], (int)((unsigned char*)&tTCP_EventSvr->m_tCliAddr.sin_addr.s_addr)[1], (int)((unsigned char*)&tTCP_EventSvr->m_tCliAddr.sin_addr.s_addr)[2], (int)((unsigned char*)&tTCP_EventSvr->m_tCliAddr.sin_addr.s_addr)[3]);

		if (pthread_create(&thID, NULL, ExtCliThreadSub, lParam) != 0)
		{
			nClntSessionID = tTCP_EventSvr->t_Sock2SessionMap[tTCP_EventSvr->m_tCliSocket];	
			tTCP_EventSvr->DelSocketEventWithLock(nClntSessionID, tTCP_EventSvr->m_tCliSocket);
			sleep(1);
			continue;
		}
		pthread_detach(thID);	
		sleep(2);

	}

	tTCP_EventSvr->m_nRunServer -= 1;

	return (void*)0x0;

}

void *CTCP_EventServer::MultiAcceptThreadSub(LPVOID lParam)
{
	CTCP_EventServer*	tTCP_EventSvr = (CTCP_EventServer *) lParam;
	SOCKET				sNewConnectSock = -1;
	PEVT_SESSION		pEvtSession = NULL;
	SOCKADDR_IN			tNewConnectAddr;
	TMapEvtSessionItor	begin, end;
	INT32				nNewSessionID = 0;
	INT32				nNewConnectAddrLen = sizeof(tNewConnectAddr);
	int					i, nEvtCnt;
	int                 nCloseSocket = 0;
	char				szClntIP[20] = {0,};
	struct epoll_event	tEvent;
	INT32				nRet = 0;
	INT32				nClntSessionID = 0;
	INT32				nTimeOut = 30;
	INT32				nSessionCount = 0;
	EVT_SESSION*		pevt_session = NULL;

	if(tTCP_EventSvr == NULL)
	{
		return (void*)0x0;
	}
	pthread_detach(pthread_self());

	tTCP_EventSvr->m_nRunServer	+= 1;
	
	while (tTCP_EventSvr->m_nContinueServer)
	{
		nEvtCnt = epoll_wait(tTCP_EventSvr->m_nEpollFd, tTCP_EventSvr->m_pEpollEvents, tTCP_EventSvr->m_nMaxEvent, 10);
		if (nEvtCnt == 0)
		{
			continue;
		}
		if ( nEvtCnt  < 0)
		{
			break;
		}

		for (i = 0; i < nEvtCnt; i++)
		{
			if (tTCP_EventSvr->m_pEpollEvents[i].data.fd == tTCP_EventSvr->m_tListenSocket)
			{
				while (1)
				{
					if (tTCP_EventSvr->m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
					{
						nNewConnectAddrLen = sizeof(tNewConnectAddr);
						sNewConnectSock = accept(tTCP_EventSvr->m_tListenSocket,(SOCKADDR *)&tNewConnectAddr,(socklen_t *)&nNewConnectAddrLen);
						if(sNewConnectSock != -1)
						{
							strncpy(szClntIP, inet_ntoa(tNewConnectAddr.sin_addr), 19);
							szClntIP[19] = 0;
						}
					} 
					else 
					{
						nNewConnectAddrLen = sizeof(tNewConnectAddr);
						sNewConnectSock = accept4(tTCP_EventSvr->m_tListenSocket,(SOCKADDR *)&tNewConnectAddr,(socklen_t *)&nNewConnectAddrLen, SOCK_NONBLOCK);
						if(sNewConnectSock != -1)
						{
							strncpy(szClntIP, inet_ntoa(tNewConnectAddr.sin_addr), 19);
							szClntIP[19] = 0;
						}
					}
					
					if (sNewConnectSock == -1)
					{	
						if (errno == EINTR)
						{
							continue;
						}
						else if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							break;
						}
						else
						{ 
							break;
						}
					}
					else 
					{
						tEvent.events = EPOLLIN | EPOLLRDHUP;

						if((nNewSessionID = tTCP_EventSvr->AddSocketEventWithLock(sNewConnectSock, tNewConnectAddr)) == 0)
						{
							tTCP_EventSvr->CloseSingleSocket(sNewConnectSock);
							continue;
						}
						tEvent.data.fd = sNewConnectSock;
						
						if(epoll_ctl(tTCP_EventSvr->m_nEpollFd, EPOLL_CTL_ADD, sNewConnectSock, &tEvent) == -1)
						{
							tTCP_EventSvr->DelSocketEventWithLock(nClntSessionID, sNewConnectSock);
							continue;
						}
						nSessionCount++;
					
						if (tTCP_EventSvr->m_init_data.nRemainSockEvt) 
						{
							SOCK_EVT_PROP sock_evt_prop;
							sock_evt_prop.nSessionID  = nNewSessionID;
							sock_evt_prop.nEventID    = ASI_SOCKET_EVENT_CONNECT;
							sock_evt_prop.nAddress    = tNewConnectAddr.sin_addr.s_addr;
							tTCP_EventSvr->AddEvtWithPktLock(sock_evt_prop);
						}
					}
				
				}
			}
			else 
			{
				nClntSessionID = tTCP_EventSvr->t_Sock2SessionMap[tTCP_EventSvr->m_pEpollEvents[i].data.fd];	

				if (tTCP_EventSvr->m_pEpollEvents[i].events & EPOLLERR || tTCP_EventSvr->m_pEpollEvents[i].events & EPOLLRDHUP || (!(tTCP_EventSvr->m_pEpollEvents[i].events & EPOLLIN)))
				{
					if(nSessionCount > 0)
						nSessionCount--;

					if (tTCP_EventSvr->m_init_data.nRemainSockEvt)
					{
						SOCK_EVT_PROP sock_evt_prop;
						sock_evt_prop.nSessionID  = nClntSessionID;
						sock_evt_prop.nEventID    = ASI_SOCKET_EVENT_DISCONNECT;
						sock_evt_prop.nAddress    = tTCP_EventSvr->t_EvtSessionMap[nClntSessionID].t_SockAddr.sin_addr.s_addr;
						tTCP_EventSvr->AddEvtWithPktLock(sock_evt_prop);
					}
					
					if(epoll_ctl(tTCP_EventSvr->m_nEpollFd, EPOLL_CTL_DEL, tTCP_EventSvr->m_pEpollEvents[i].data.fd, NULL) == -1)
					{
						sleep(1);
					}	
					tTCP_EventSvr->DelSocketEventWithLock(nClntSessionID, tTCP_EventSvr->m_pEpollEvents[i].data.fd);
					tTCP_EventSvr->m_pEpollEvents[i].data.fd = -1;
					continue;
				} 
				if (tTCP_EventSvr->m_pEpollEvents[i].events & EPOLLHUP || tTCP_EventSvr->m_pEpollEvents[i].events & EPOLLIN) 
				{
					nCloseSocket = 0;

					pevt_session = tTCP_EventSvr->GetEvtSession(nClntSessionID);
					if(pevt_session)
					{
						nRet = tTCP_EventSvr->Recv(pevt_session, nCloseSocket, nTimeOut);
						if (nRet < 0)
							Sleep(10);
					}
					else
					{
						Sleep(10);
					}	
					
					if (nCloseSocket == 1)
					{
						if(nSessionCount > 0)
							nSessionCount--;
				
						if (tTCP_EventSvr->m_init_data.nRemainSockEvt) 
						{
							SOCK_EVT_PROP sock_evt_prop;
							sock_evt_prop.nSessionID  = nClntSessionID;
							sock_evt_prop.nEventID    = ASI_SOCKET_EVENT_DISCONNECT;
							sock_evt_prop.nAddress    = tTCP_EventSvr->t_EvtSessionMap[nClntSessionID].t_SockAddr.sin_addr.s_addr;
							tTCP_EventSvr->AddEvtWithPktLock(sock_evt_prop);
						}
						if(epoll_ctl(tTCP_EventSvr->m_nEpollFd, EPOLL_CTL_DEL, tTCP_EventSvr->m_pEpollEvents[i].data.fd, NULL) == -1)
						{
							sleep(1);
						}
						tTCP_EventSvr->DelSocketEventWithLock(nClntSessionID, tTCP_EventSvr->m_pEpollEvents[i].data.fd);
						tTCP_EventSvr->m_pEpollEvents[i].data.fd = -1;
					}
				}
			}
		}
	}

	tTCP_EventSvr->m_nRunServer -= 1;
	
	return (void*)0x0;
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::CloseSingleSocket(SOCKET s)
{
	int nKeepAlive = 0;
	linger li = {0, 0};
	if(s == -1)
	{
		return 0;
	}
	if(s == m_tListenSocket)
	{
		close(s);
		m_tListenSocket = -1;
	}
	else
	{
		if(m_init_data.nOnKeepAlive)
		{
			setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &nKeepAlive, sizeof(nKeepAlive));
		}
		setsockopt(s, SOL_SOCKET, SO_LINGER, (char*)&li, sizeof(li)); 
		shutdown(s, 2);					
		close(s);
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::MakeSendBuf(UINT32 nSessionKey, UINT16 type, UINT16 code, UINT32 length, PVOID lpBuf, PVOID	*lpOutput)
{
	PKT_DATA_HDR_ALL	pkt_hdr_all;
	PKT_DATA_HDR_FIRST  pkt_hdr_first;
	PKT_DATA_HDR_SECOND pkt_hdr_second;
	PKT_DATA_HDR_SHA256	pkt_hdr_sha256;
	PKT_DATA_HDR_SSL	pkt_hdr_ssl;
	BYTE	acDstHash[40] = {0,};
	INT32 nTotalLen = 0;
	PBYTE lpPlanText = NULL;
	PBYTE lpEncText = NULL;
	UINT32 nEncLen = 0;
	CSecurityUtil tSecuUtil;

	if(lpOutput == NULL)
	{
		return -1;
	}

	if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SEED_SHA2)
	{
		tSecuUtil.SetMode(ASI_BUFFER_ENC_TYPE_SEED);
		tSecuUtil.SetSessionKey(nSessionKey);
		
		UINT32 nEncLen = tSecuUtil.Encrypt_Len(length);
		pkt_hdr_sha256.type		= ntohs(type);
		pkt_hdr_sha256.code		= ntohs(code);
		pkt_hdr_sha256.length	= ntohl(nEncLen);
		
		if(lpBuf)	nTotalLen = nEncLen + PKT_DATA_HDR_SHA256_SIZE;
		else		nTotalLen = PKT_DATA_HDR_SHA256_SIZE;
		
		*lpOutput = (PBYTE)malloc(sizeof(BYTE) * nTotalLen + 1);
		if(*lpOutput == NULL)
		{
			return -1;
		}
		memset(*lpOutput, 0, sizeof(BYTE) * nTotalLen + 1);
		
		if(lpBuf && length)
		{
			lpEncText = (PBYTE)*lpOutput + PKT_DATA_HDR_SHA256_SIZE;
			
			tSecuUtil.Encrypt((PBYTE)lpBuf, length, &lpEncText, nEncLen);
			tSecuUtil.GetSha256(lpEncText, (DWORD)nEncLen, acDstHash, sizeof(acDstHash));
			memcpy(pkt_hdr_sha256.hash, acDstHash, 32);
		}
		memcpy(*lpOutput, &pkt_hdr_sha256, PKT_DATA_HDR_SHA256_SIZE);
	}	
	else if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
	{
		tSecuUtil.SetMode(ASI_BUFFER_ENC_TYPE_TLS1);
		tSecuUtil.SetSessionKey(nSessionKey);
		
		UINT32 nEncLen = tSecuUtil.Encrypt_Len(length);		
		pkt_hdr_ssl.type	= ntohs(type);
		pkt_hdr_ssl.code	= ntohs(code);
		pkt_hdr_ssl.length	= ntohl(nEncLen);
		
		if(lpBuf)		
			nTotalLen = nEncLen + sizeof(pkt_hdr_ssl);
		else
			nTotalLen = sizeof(pkt_hdr_ssl);
		
		*lpOutput = (PBYTE)malloc(sizeof(BYTE) * nTotalLen + 1);
		if(*lpOutput == NULL)
		{
			return -1;
		}
		
		memset(*lpOutput, 0x00, sizeof(BYTE) * nTotalLen + 1);
		
		memcpy(*lpOutput, &pkt_hdr_ssl, sizeof(pkt_hdr_ssl));
		
		if(lpBuf && length)
		{
			lpEncText = (PBYTE)*lpOutput + PKT_DATA_HDR_SSL_SIZE;			
			tSecuUtil.Encrypt((PBYTE)lpBuf, length, &lpEncText, nEncLen);
		}
	}

	return nTotalLen;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_EventServer::SendCoreWithLock(PEVT_SESSION pevt_session, PPKT_DATA pkt_data, INT32& nCloseSocket, INT32 nTimeOut)
{
	INT32 nSendedSize = 0;
	DWORD nSendSize = 0;
	INT32 nOnceSendSize = 0;
	INT32 nRetVal = 0;
	INT32 nFlag = 0;
	INT32 nMakeBufSize = 0;
	PVOID lpOutput = NULL;
	int nAgainCnt = 0;
	SOCKET s = 0;
	time_t nStartTime = 0;
	time_t nEndTime = 0;
	
	pthread_mutex_lock(&m_mutex);
	nMakeBufSize = pkt_data->hdr.length;
	lpOutput = pkt_data->body.data;
	s = pevt_session->t_Socket;
	nStartTime = time(NULL);
	while(nSendedSize < nMakeBufSize)
	{
		if( (nMakeBufSize - nSendedSize) > m_init_data.nPktPagingSize)
			nOnceSendSize = m_init_data.nPktPagingSize;
		else
			nOnceSendSize = nMakeBufSize - nSendedSize;
		
		nSendSize = send(s, ((char*)lpOutput) + nSendedSize, nOnceSendSize, nFlag);
		if(nSendSize == -1)
		{
			if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nEndTime = time(NULL);
				if(difftime(nEndTime, nStartTime) > nTimeOut)
				{
					nCloseSocket = 1;
					nRetVal = -2;
					break;
				}	
				Sleep(10);
				continue;
			}
			else
			{
				nCloseSocket = 1;
				nRetVal = -3;
				break;	
			}
		}
		else if(nSendSize == 0)
		{
			nCloseSocket = 1;
			nRetVal = -4;
			break;
		}
		else
		{			
			nSendedSize += nSendSize;
			nStartTime = time(NULL);
			nRetVal = 0;
		}
	}

	pthread_mutex_unlock(&m_mutex);

	safe_free(lpOutput);

	return nRetVal;		
}
//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_EventServer::SendCoreSSLWithLock(PEVT_SESSION pevt_session, PPKT_DATA pkt_data, INT32& nCloseSocket, INT32 nTimeOut)
{
	INT32 nSendLen = 0;
	INT32 nBufferSize = 0;
	INT32 nSendedLen = 0;
	INT32 nSSLErrNo = 0;
	INT32 nOnceSendSize = 0;
	INT32 nMakeBufSize = 0;
	PVOID lpHeader = NULL;
	SSL	 *tSSL = NULL;
	
	INT32 nRtn = 0;
	time_t nStartTime = 0;
	time_t nEndTime = 0;
	

	pthread_mutex_lock(&m_mutex);

	tSSL = pevt_session->t_SSL;
	nMakeBufSize = pkt_data->hdr.length;
	lpHeader = pkt_data->body.data;
	
	nBufferSize = nMakeBufSize;
	nStartTime = time(NULL);
	while(nBufferSize > nSendedLen && !nRtn)
	{
		nSendLen = m_tSSLSockUtil.SSL_SendBuf(tSSL, ((char*)lpHeader) + nSendedLen, nBufferSize - nSendedLen);
		if(nSendLen == -1)
		{
			if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nEndTime = time(NULL);
				if(difftime(nEndTime, nStartTime) > nTimeOut)
				{
					nCloseSocket = 1;
					nRtn = -2;
					break;
				}			
				Sleep(10);
				continue;
			}
			else
			{
				nCloseSocket = 1;
				nRtn = -3;
				break;
			}
		}
		else if(nSendLen == 0)
		{
			nCloseSocket = 1;
			nRtn = -4;
			break;
		}
		else
		{
			nSendedLen += nSendLen;
			nStartTime = time(NULL);
			nRtn = 0;
		}
	}
	
	safe_free(lpHeader);
	pthread_mutex_unlock(&m_mutex);
	return nRtn;
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::SendData(INT32 sessionid, UINT16 type, UINT16 code, UINT32 length, PVOID data)
{
	INT32 nTimeOut = 30;
	PEVT_SESSION pevt_session = GetEvtSession(sessionid);
	if(pevt_session == NULL)
	{
		return -1;
	}

	if(length > MAKE_BUFFER_MAX_SIZE || length == 0)
	{
		return -10;
	}	
	PVOID lpOutput = NULL;
	INT32 nMakeBufSize = MakeSendBuf(pevt_session->n_SessionKey, type, code, length, data, &lpOutput);
	if(nMakeBufSize < 1)
	{
		return -1;
	}
	PKT_DATA pkt_data;
	pkt_data.hdr.length = nMakeBufSize;
	pkt_data.body.data	= (PBYTE)lpOutput;		

	INT32 nRet = 0;
	INT32 nCloseSocket = 0;

	if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
	{
		nRet = SendCoreSSLWithLock(pevt_session, &pkt_data, nCloseSocket, nTimeOut);
	}
	else
	{
		nRet = SendCoreWithLock(pevt_session, &pkt_data, nCloseSocket, nTimeOut);
	}
	if( nRet != 0 )
	{
		if(nCloseSocket)
		{
			pevt_session->n_CloseReserved = 1;
		}
	}
	return nRet;
}
//---------------------------------------------------------------------------------------------------------------------------

void	CTCP_EventServer::StopServer(INT32 nDeleteRemainPkt)
{
	if (!m_nContinueServer && !m_nRunServer)
	{
		return;
	}

	m_nContinueServer = 0;

	while (m_nRunServer)
	{
		usleep(10000);
	}

	if(nDeleteRemainPkt)
		RemovePktWithPktLock();

	ClearSocketEventWithLock();

	if(m_tListenSocket != -1)
	{
		close(m_tListenSocket);
		m_tListenSocket = -1;
	}
	safe_free(m_pEpollEvents);
	if(m_nEpollFd != -1)
	{
		close(m_nEpollFd);
		m_nEpollFd = -1;
	}
	return;
}
//---------------------------------------------------------------------------------------------------------------------------

void	CTCP_EventServer::EditLogFileName_Ext(LPCTSTR lpLogFilePath, LPCTSTR lpLogFileName)
{	
	snprintf(m_LogFilePath, MAX_PATH-1, "%s", lpLogFilePath);
	m_LogFilePath[MAX_PATH-1] = 0;
	snprintf(m_LogFileName, MAX_PATH-1, "%s", lpLogFileName);
	m_LogFileName[MAX_PATH-1] = 0;
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::StartServer(LPCTSTR lpOpenAddress, UINT16 nOpenPort, SOCK_NOTIFY_MSG tSNM, SOCK_INIT_DATA tSID)
{
	INT32 nRtn = 0;
	INT32 nErrRtn = 0;
	DWORD dwSessionCount = 10;

	if(lpOpenAddress == NULL || lpOpenAddress[0] == 0 || nOpenPort > 65536 )
		return -10;
	
	snprintf(m_LogFilePath, MAX_PATH-1, "%s", tSID.szLogFilePath);
	m_LogFilePath[MAX_PATH-1] = 0;
	snprintf(m_LogFileName, MAX_PATH-1, "%s", tSID.szLogFileName);
	m_LogFileName[MAX_PATH-1] = 0;

	if(tSNM.m_hNotifyOwner)
		LoginAuth = (pfnAuth_Console_Callback)tSNM.m_hNotifyOwner;

	memcpy(&m_init_data, &tSID, sizeof(m_init_data));

	if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
	{		
		if(m_tSSLSockUtil.Init_SSL())
		{
			return -20;
		}

		if( (nRtn = m_tSSLSockUtil.Init_PEM(m_init_data.szPemFileCertPath, m_init_data.szPemFileKeyPath, nErrRtn)) != 0)
		{
			return -30;
		}
	}

	if( Bind(nOpenPort) )
	{
		return -40;
	}

	if(Listen())
	{
		return -50;
	}


	if (m_init_data.nExtSockFlag != 1)
	{
		if (InitEpoll(dwSessionCount))
		{
			return -60;
		}

		m_nContinueServer = 1;

		if (pthread_create(&m_hMultiAcceptThread, NULL, MultiAcceptThreadSub, (void *) this) != 0)
		{
			m_nContinueServer = 0;
			return -70;
		}
	}
	else
	{
		m_nContinueServer = 1;
		if (pthread_create(&m_hMultiAcceptThread, NULL, ExtAcceptThreadSub, (void *) this) != 0)
		{
			m_nContinueServer = 0;
			return -80;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------

bool		CTCP_EventServer::AddPktWithPktLock(PKT_DATA& pkt_data)
{
	pthread_mutex_lock(&m_pkt_mutex);
	m_PktQueue.push(pkt_data);
	pthread_mutex_unlock(&m_pkt_mutex);
	return true;
}

INT32		CTCP_EventServer::FreeMemory(PVOID pBuff)
{
	safe_free(pBuff);
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::GetPktWithPktLock(PKT_DATA& pkt_data)
{
	INT32 nResult = 0;
	
	pthread_mutex_lock(&m_pkt_mutex);
	nResult = m_PktQueue.size();
	if(nResult > 0)
	{
		pkt_data = m_PktQueue.front();
		m_PktQueue.pop();
	}
	pthread_mutex_unlock(&m_pkt_mutex);
	return nResult;
}
//---------------------------------------------------------------------------------------------------------------------

UINT32		CTCP_EventServer::IsExistsPktWithPktLock()
{
	UINT32 nResult = 0;
	pthread_mutex_lock(&m_pkt_mutex);
	nResult = m_PktQueue.size();
	pthread_mutex_unlock(&m_pkt_mutex);
	return nResult;	
}
//---------------------------------------------------------------------------------------------------------------------

INT32		CTCP_EventServer::RemovePktWithPktLock()
{
	PKT_DATA pkt_data;
	pthread_mutex_lock(&m_pkt_mutex);
	while(!m_PktQueue.empty())
	{
		pkt_data = m_PktQueue.front();
		m_PktQueue.pop();
		safe_free(pkt_data.body.data);
	}
	pthread_mutex_unlock(&m_pkt_mutex);
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------

bool		CTCP_EventServer::AddEvtWithPktLock(SOCK_EVT_PROP& sock_evt_prop)
{
	PKT_DATA pkt_data;

	pthread_mutex_lock(&m_pkt_mutex);
	pkt_data.nIsSockEvent = 1;
	pkt_data.nSessionID	= sock_evt_prop.nSessionID;
	pkt_data.sock_evt = sock_evt_prop;
	m_PktQueue.push(pkt_data);
	pthread_mutex_unlock(&m_pkt_mutex);
	return true;
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::AddSocketEventWithLock(SOCKET tSocket, SOCKADDR_IN tSockAddr, INT32 nChkSSL)
{
	char szClntIP[20] = {0,};
	EVT_SESSION  stEvtEession;
	int nFlags = 0;
	INT32 nRetVal = 0;
	int nTime = 1000;
	int nIntval = 1000;
	int nProbeCnt = 10;

	memset(&stEvtEession, 0, sizeof(stEvtEession));
	
	stEvtEession.nSessionID = m_nGlobalID--;
	if(m_nGlobalID > -10)
		m_nGlobalID = -10;

	stEvtEession.t_Socket		= tSocket;
	stEvtEession.t_SockAddr		= tSockAddr;
	stEvtEession.n_SessionKey	= m_init_data.nSessionKey;
	stEvtEession.n_CloseReserved	= 0;
	stEvtEession.t_SSL			= NULL;
	stEvtEession.t_RegTime		= time(NULL);
	stEvtEession.p_BodyData      = NULL;

	InitSessionRecvInfo(&stEvtEession);

	pthread_mutex_lock(&m_mutex);
	
	if (nChkSSL && m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
	{
		if ( (stEvtEession.t_SSL = m_tSSLSockUtil.GetSSL_Accept(stEvtEession.t_Socket, nRetVal)) == NULL)
		{
			INET_NTOA(stEvtEession.t_SockAddr.sin_addr, szClntIP, sizeof(szClntIP));
			pthread_mutex_unlock(&m_mutex);
			return 0;
		}		
		else
		{
			m_tSSLSockUtil.SetShutDown_Flag(stEvtEession.t_SSL, 0);

			nFlags = fcntl(tSocket, F_GETFL, 0);
			fcntl(tSocket, F_SETFL, nFlags | O_NONBLOCK);
		}
	}
	if (nChkSSL && m_init_data.nOnKeepAlive)
	{
		SetSockKeepAlive(tSocket, nTime, nIntval, nProbeCnt);
	}		

	if (nChkSSL)
		m_nTotalConnectNum++;
	
	t_EvtSessionMap[stEvtEession.nSessionID] = stEvtEession;
	t_Sock2SessionMap[tSocket] = stEvtEession.nSessionID;
	nRetVal = stEvtEession.nSessionID;
	pthread_mutex_unlock(&m_mutex);
	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------

INT32			CTCP_EventServer::AddSocketEventListen(SOCKET tSocket, SOCKADDR_IN tSockAddr)
{
	EVT_SESSION stEvtSession;
	INT32 nRetVal = 0;
	memset(&stEvtSession, 0, sizeof(stEvtSession));
	pthread_mutex_lock(&m_mutex);
	stEvtSession.nSessionID	= -1;
	stEvtSession.t_Socket = tSocket;
	stEvtSession.t_SockAddr = tSockAddr;
	stEvtSession.n_SessionKey = m_init_data.nSessionKey;
	stEvtSession.n_CloseReserved = 0;
	stEvtSession.t_SSL = 0;	
	stEvtSession.t_RegTime = time(NULL);	
	t_EvtSessionMap[stEvtSession.nSessionID] = stEvtSession;
	t_Sock2SessionMap[tSocket] = -1;
	nRetVal = stEvtSession.nSessionID;
	pthread_mutex_unlock(&m_mutex);
	
	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::DelSocketEventWithLock(INT32 nSessionID, SOCKET sCloseSock)
{	
	INT32 nRetVal = 0;
	SOCKET s = 0;

	pthread_mutex_lock(&m_mutex);
	s = t_EvtSessionMap[nSessionID].t_Socket;
	if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL && t_EvtSessionMap[nSessionID].t_SSL)
	{
		m_tSSLSockUtil.ShutDown_SSL(t_EvtSessionMap[nSessionID].t_SSL);
		m_tSSLSockUtil.Free_SSL(t_EvtSessionMap[nSessionID].t_SSL);
		t_EvtSessionMap[nSessionID].t_SSL = NULL;
	}

	if(sCloseSock != s)
	{
		CloseSingleSocket(sCloseSock);
	}
	
	if (t_EvtSessionMap[nSessionID].n_SessionKey != 0 && s != -1)
	{
		CloseSingleSocket(s);
		t_Sock2SessionMap.erase(s);
		t_EvtSessionMap[nSessionID].t_Socket = -1;
	}

	safe_free(t_EvtSessionMap[nSessionID].p_BodyData);
	t_EvtSessionMap.erase(nSessionID);
	nRetVal = t_EvtSessionMap.size();
	pthread_mutex_unlock(&m_mutex);

	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------

void CTCP_EventServer::ClearSocketEventWithLock()
{
	TMapEvtSessionItor begin, end;
	pthread_mutex_lock(&m_mutex);
	begin = t_EvtSessionMap.begin();
	end = t_EvtSessionMap.end();
	for(begin; begin != end; begin++)
	{
		if((*begin).second.t_Socket != -1)
		{
			CloseSingleSocket((*begin).second.t_Socket);
			(*begin).second.t_Socket = -1;
		}	

		if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL && (*begin).second.t_SSL)
		{
			m_tSSLSockUtil.Free_SSL((*begin).second.t_SSL);
			(*begin).second.t_SSL = NULL;
		}
	}

	t_EvtSessionMap.clear();
	t_Sock2SessionMap.clear();
	pthread_mutex_unlock(&m_mutex);
}
//---------------------------------------------------------------------------------------------------------------------

PEVT_SESSION CTCP_EventServer::GetEvtSession(INT32 nSessionID)
{
	TMapEvtSessionItor find = t_EvtSessionMap.find(nSessionID);
	if(find == t_EvtSessionMap.end())
	{
		return NULL;
	}
	return &t_EvtSessionMap[nSessionID];
}
//---------------------------------------------------------------------------------------------------------------------

void CTCP_EventServer::InitSessionRecvInfo(PEVT_SESSION pEvtSession)
{
	if(pEvtSession != NULL)
	{
		memset(pEvtSession->p_HeaderData, 0x00, 1024);
		safe_free(pEvtSession->p_BodyData);
		pEvtSession->n_PktRecvStep    = 0;
		pEvtSession->n_HeaderRemain   = m_init_data.nPktHdrSize;
		pEvtSession->n_HeaderDataPtr  = 0;
		pEvtSession->n_BodyLength     = 0;
		pEvtSession->n_BodyRemain     = 0;
		pEvtSession->n_BodyDataPtr    = 0;
	}
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_EventServer::Recv(PEVT_SESSION pEvtSession, INT32& nCloseSocket, INT32 nTimeOut)
{	
	INT32 nSessionID = 0;
	SOCKET sock	= 0;
	UINT32 nSessionKey	= 0;
	INT32 nRecvedSize = 0;
	INT32 nRecvSize = 0;
	INT32 nOnceRecvSize = 0;
	INT32 nBodySize = 0;
	INT32 nRetVal = 0;
	char *pHeader = NULL;
	char *pRecvData = NULL;
	time_t nStartTime = 0;
	time_t nEndTime = 0;
	PKT_DATA_HDR_SHA256		pkt_hdr_sha256;
	PKT_DATA_HDR_SSL		pkt_hdr_ssl;
	PBYTE lpPlanText = NULL;
	UINT32 nPlanLen = 0;
	CSecurityUtil tSecuUtil;
	PKT_DATA pkt_data;
	SSL *tSSL = NULL;

	if(pEvtSession == NULL)
		return -1;

	pHeader = (char	*)malloc(1024);
	if(pHeader == NULL)
		return -2;
	memset(pHeader, 0, 1024);
	
	nSessionID	= pEvtSession->nSessionID;
	sock		= pEvtSession->t_Socket;
	tSSL		= pEvtSession->t_SSL;
	nSessionKey	= pEvtSession->n_SessionKey;
	nCloseSocket = 0;

	nStartTime = time(NULL);
	nRetVal = 0;
	while(nRecvedSize != m_init_data.nPktHdrSize && m_nContinueServer)
	{
		nOnceRecvSize = m_init_data.nPktHdrSize - nRecvedSize;			
		if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
		{
			nRecvSize = m_tSSLSockUtil.SSL_RecvBuf(tSSL, &pHeader[nRecvedSize], nOnceRecvSize);
		}
		else
		{
			nRecvSize = recv(sock, &pHeader[nRecvedSize], nOnceRecvSize, 0);
		}		

		if (nRecvSize < 0)
		{
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nEndTime = time(NULL);
				if(difftime(nEndTime, nStartTime) > nTimeOut)
				{
					nCloseSocket = 1;
					nRetVal = -3;
					break;
				}
				Sleep(10);
			}
			else
			{
				nCloseSocket = 1;
				nRetVal = -4;
				break;
			}
		}
		else if (nRecvSize == 0)
		{
			nCloseSocket = 1;
			nRetVal = -5;
			break;
		}
		else
		{
			nRecvedSize += nRecvSize;
			nStartTime = time(NULL);
			nRetVal = 0;
		}
	}
	if(m_nContinueServer == FALSE)
	{
		nRetVal = -10;
	}
	if(nRetVal != 0)
	{
		safe_free(pHeader);
		return nRetVal;
	}

	if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SEED_SHA2)
	{
		memcpy(&pkt_hdr_sha256, pHeader, sizeof(pkt_hdr_sha256));
		nBodySize = ntohl(pkt_hdr_sha256.length);
	}
	else if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
	{
		memcpy(&pkt_hdr_ssl, pHeader, sizeof(pkt_hdr_ssl));
		nBodySize = ntohl(pkt_hdr_ssl.length);
	}
	safe_free(pHeader);

	if(nBodySize < 1)
	{
		return -6;
	}
	pRecvData = (char *)malloc(nBodySize + 1);
	if(pRecvData == NULL)
		return -7;
	memset(pRecvData, 0, nBodySize + 1);
	
	nTimeOut = 30;
	nStartTime = time(NULL);
	nRecvedSize = 0;
	nRetVal = 0;
	while(nRecvedSize != nBodySize && m_nContinueServer)
	{
		if( nBodySize - nRecvedSize > m_init_data.nPktPagingSize)
			nOnceRecvSize = m_init_data.nPktPagingSize;
		else
			nOnceRecvSize = nBodySize - nRecvedSize;
		
		if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
		{
			nRecvSize = m_tSSLSockUtil.SSL_RecvBuf(pEvtSession->t_SSL, &pRecvData[nRecvedSize], nOnceRecvSize);
		}
		else
		{
			nRecvSize = recv(sock, &pRecvData[nRecvedSize], nOnceRecvSize, 0);
		}
		
		if (nRecvSize < 0)
		{
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nEndTime = time(NULL);
				if(difftime(nEndTime, nStartTime) > nTimeOut)
				{
					nCloseSocket = 1;
					nRetVal = -8;
					break;
				}			
				Sleep(10);
			}
			else
			{
				nCloseSocket = 1;
				nRetVal = -9;
				break;
			}
		}
		else if(nRecvSize == 0)
		{
			nCloseSocket = 1;
			nRetVal = -10;
			break;
		}
		else
		{
			nRecvedSize += nRecvSize;
			nStartTime = time(NULL);
			nRetVal = 0;			
		}
	}
	
	if(m_nContinueServer == 0)
	{
		nRetVal = -11;
	}
	if(nRetVal != 0)
	{
		safe_free(pRecvData);
		return nRetVal;
	}

	if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SEED_SHA2)
	{
		tSecuUtil.SetMode(ASI_BUFFER_ENC_TYPE_SEED);
		tSecuUtil.SetSessionKey(nSessionKey);
		
		if(tSecuUtil.CompareSha256((PBYTE)pRecvData, nBodySize, pkt_hdr_sha256.hash))
		{
			char pszChkHash[257] = {0, };
			char pszCurHash[257] = {0, };
			
			tSecuUtil.ConvertSha256(pkt_hdr_sha256.hash, pszChkHash, 256);
			tSecuUtil.ConvertSha256Last(pszCurHash, 256);
			
			safe_free(pRecvData);
			return -12;
		}
		
		tSecuUtil.Decrypt((PBYTE)pRecvData, nBodySize, &lpPlanText, nPlanLen);	
		
		pkt_data.hdr.length = nPlanLen;
		pkt_data.hdr.type	= ntohs(pkt_hdr_sha256.type);
		pkt_data.hdr.code	= ntohs(pkt_hdr_sha256.code);
		pkt_data.body.data = (PBYTE)lpPlanText;
	}
	else if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
	{
		tSecuUtil.SetMode(ASI_BUFFER_ENC_TYPE_TLS1);
		tSecuUtil.SetSessionKey(nSessionKey);

		tSecuUtil.Decrypt((PBYTE)pRecvData, nBodySize, &lpPlanText, nPlanLen);	
		
		pkt_data.hdr.length = nPlanLen;
		pkt_data.hdr.type	= ntohs(pkt_hdr_ssl.type);
		pkt_data.hdr.code	= ntohs(pkt_hdr_ssl.code);		
		pkt_data.body.data	= (PBYTE)lpPlanText;

	}

	safe_free(pRecvData);

	pkt_data.nIsSockEvent = 0;
	pkt_data.nSessionID = nSessionID;

	AddPktWithPktLock(pkt_data);
	
	return 0;
}


//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_EventServer::GetSocketAddressUINTWithLock(INT32 nSessionID, UINT32& nAddress)
{
	INT32 nRetVal = 0;
	PEVT_SESSION pEvtSession = NULL;

	pthread_mutex_lock(&m_mutex);
	do{
		pEvtSession = GetEvtSession(nSessionID);
		if(pEvtSession == NULL)
		{
			nRetVal = -1;
			break;
		}
		nAddress = pEvtSession->t_SockAddr.sin_addr.s_addr;
		nRetVal = 0;
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);	
	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_EventServer::GetSocketAddressWithLock(INT32 nSessionID, OUT LPSTR lpAddress)
{
	PEVT_SESSION pEvtSession = NULL;
	char  szClntIP[20] = {0,};
	INT32 nRetVal = 0;
	
	pthread_mutex_lock(&m_mutex);
	do{
	    TMapEvtSessionItor find = t_EvtSessionMap.find(nSessionID);
		if(find == t_EvtSessionMap.end())
		{
	        nRetVal = -1;
			break;
		}
		pEvtSession = &find->second;

		INET_NTOA(pEvtSession->t_SockAddr.sin_addr, szClntIP, sizeof(szClntIP));
		strncpy(lpAddress, szClntIP, 19);
		nRetVal = 0;
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);
	
	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------


INT32	CTCP_EventServer::IsExistsSessionIDWithLock(INT32 nSessionID)
{
	INT32 nRetVal = 0;
	pthread_mutex_lock(&m_mutex);
	do{
		TMapEvtSessionItor find = t_EvtSessionMap.find(nSessionID);
		if(find == t_EvtSessionMap.end())
		{
			nRetVal = 0;
			break;
		}
		nRetVal = 1;
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);
	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_EventServer::GetConnectInfoWithLock(INT32& nCurrentConnectNum, INT32& nTotalConnectNum)
{
	pthread_mutex_lock(&m_mutex);
	nCurrentConnectNum = t_EvtSessionMap.size();
	nTotalConnectNum = m_nTotalConnectNum;
	pthread_mutex_unlock(&m_mutex);
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------

void		CTCP_EventServer::AutoDisConnectByTimeOutWithLock()
{
	TListID tSocketIDList;
	time_t tCurTime;
	TMapEvtSessionItor begin, end;
	TListIDItor	begin2, end2;
	
	if(m_init_data.nChkAutoDisConRunTime == 0)
		return;
	
	tCurTime = time(NULL);
	if(difftime(tCurTime, m_tAutoDisConLastChkTime) < m_init_data.nChkAutoDisConRunTime)
		return;
	
	pthread_mutex_lock(&m_mutex);
	begin = t_EvtSessionMap.begin();	end = t_EvtSessionMap.end();
	for(begin; begin != end; begin++)
	{
		if(difftime(tCurTime, begin->second.t_RegTime) >= m_init_data.nChkAutoDisConRunTime)	
			tSocketIDList.push_back(begin->first);
	}

	begin2 = tSocketIDList.begin();	end2 = tSocketIDList.end();
	for(begin2; begin2 != end2; begin2++)
	{
		CloseSocket(*begin2);
	}	

	m_tAutoDisConLastChkTime = tCurTime;

	pthread_mutex_unlock(&m_mutex);
}
//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_EventServer::CloseSocket(INT32 nSessionID, INT32 nRemainEvt)	
{
	SOCKET s = t_EvtSessionMap[nSessionID].t_Socket;

	if (t_EvtSessionMap[nSessionID].n_SessionKey == 0)
	{
		return 0;                                       
	}  

	if(m_init_data.nPktEncType == ASI_PKT_ENC_TYPE_SSL && t_EvtSessionMap[nSessionID].t_SSL)
	{
		m_tSSLSockUtil.ShutDown_SSL(t_EvtSessionMap[nSessionID].t_SSL);
		m_tSSLSockUtil.Free_SSL(t_EvtSessionMap[nSessionID].t_SSL);
		t_EvtSessionMap[nSessionID].t_SSL = NULL;
	}
	
	if(s != -1)
	{
		epoll_ctl(m_nEpollFd, EPOLL_CTL_DEL, s, NULL);
		CloseSingleSocket(s);
		t_Sock2SessionMap.erase(s);
		t_EvtSessionMap[nSessionID].t_Socket = -1;
	}

	safe_free(t_EvtSessionMap[nSessionID].p_BodyData);
	t_EvtSessionMap.erase(nSessionID);			

	return t_EvtSessionMap.size();	
}
//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_EventServer::EditSessionIDWithLock(INT32 nOldID, INT32 nNewID)
{
	INT32 nRetVal = 0;
	EVT_SESSION evt_session;
	pthread_mutex_lock(&m_mutex);
	do{
	   	TMapEvtSessionItor find = t_EvtSessionMap.find(nOldID);
		if(find == t_EvtSessionMap.end())
		{
	        nRetVal = -1;
			break;
		}
		
		evt_session = find->second;
		evt_session.nSessionID = nNewID;
		t_EvtSessionMap[nNewID] = evt_session;
		t_EvtSessionMap.erase(nOldID);
		t_Sock2SessionMap[evt_session.t_Socket] = nNewID;
		nRetVal = 0;
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);
	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_EventServer::EditSessionKeyWithLock(INT32 nSessionID, UINT32 nSessionKey)
{
	INT32 nRetVal = 0;
	PEVT_SESSION pEvtSession = NULL;
	pthread_mutex_lock(&m_mutex);
	do{
		TMapEvtSessionItor find = t_EvtSessionMap.find(nSessionID);
		if(find == t_EvtSessionMap.end())
		{
			nRetVal = -1; 
			break;
		}

		pEvtSession = &find->second;
		pEvtSession->n_SessionKey = nSessionKey;
		nRetVal = 0;
	}while(FALSE);
	pthread_mutex_unlock(&m_mutex);
	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_EventServer::IsRunSvr()	
{
	return m_nRunServer;
}
//---------------------------------------------------------------------------------------------------------------------
char *CTCP_EventServer::INET_NTOA(struct in_addr in, char *buf, int len)
{
	in_addr_t      addr;
	unsigned char  *addr_ptr;

	if (buf == NULL || len <= 0)
		return NULL;

	addr = in.s_addr;
	addr_ptr = (unsigned char *)&addr;

	snprintf(buf, len, "%d.%d.%d.%d", addr_ptr[0] & 0xff, addr_ptr[1] & 0xff, addr_ptr[2] & 0xff,addr_ptr[3] & 0xff);

	return buf;
}

//---------------------------------------------------------------------------------------------------------------------
void CTCP_EventServer::SetSockKeepAlive(SOCKET tSocket, int nTime, int nIntval, int nProbeCnt)
{
	int  nKeepAliveOn = 1;
	setsockopt(tSocket, SOL_TCP, TCP_KEEPIDLE, (void *) &nTime, sizeof(nTime));
	setsockopt(tSocket, SOL_TCP, TCP_KEEPINTVL, (void *) &nIntval, sizeof(nIntval));
	setsockopt(tSocket, SOL_TCP, TCP_KEEPCNT, (void *) &nProbeCnt, sizeof(nProbeCnt));
	setsockopt(tSocket, SOL_SOCKET, SO_KEEPALIVE, (void *) &nKeepAliveOn, sizeof(nKeepAliveOn));
}

