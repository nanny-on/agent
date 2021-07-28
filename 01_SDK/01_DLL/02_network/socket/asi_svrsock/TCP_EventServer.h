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

// TCP_EventServer.h: INT32erface for the CTCP_EventServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETSERVER_H__AB901800_C635_4111_B7A5_90B10E0DF4DF__INCLUDED_)
#define AFX_SOCKETSERVER_H__AB901800_C635_4111_B7A5_90B10E0DF4DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <queue>
#include <map>
#include "MutexExt.h"
#include "SSLSockUtil.h"
#include "SecurityUtil.h"
//#include "socket_define_header.h"
#include "com_header_socket_dll.h"

#define DEFAULT_SESSION_COUNT	10

//----------------------------------------------------------------------------------------------

typedef struct _evt_session
{	
	INT32			nSessionID;
	UINT32			n_SessionKey;
	SOCKET			t_Socket;
	SOCKADDR_IN		t_SockAddr;
	INT32			n_CloseReserved;
	SSL				*t_SSL;
	time_t			t_RegTime;

	int				n_PktRecvStep;		/* 0 - not recved, 1 - header recved, 2 - header, body recved */

	int				n_HeaderRemain;
	int				n_HeaderDataPtr;
	BYTE			p_HeaderData[1024];

	int				n_BodyLength;
	int				n_BodyRemain;
	int				n_BodyDataPtr;
	PBYTE           p_BodyData;

}EVT_SESSION, *PEVT_SESSION;
//----------------------------------------------------------------------------------------------

typedef map<INT32,	EVT_SESSION>	TMapEvtSession;
typedef TMapEvtSession::iterator	TMapEvtSessionItor;
//----------------------------------------------------------------------------------------------
typedef map<INT32, INT32>			TMapSockSIDPair;		// socket fd, session id
typedef TMapSockSIDPair::iterator	TMapSockSIDPairItor;
//----------------------------------------------------------------------------------------------
typedef queue<PKT_DATA>				TQueuePktData; 
//----------------------------------------------------------------------------------------------

class CTCP_EventServer  
{
public:
	CTCP_EventServer();
	virtual ~CTCP_EventServer();

private:
	INT32 Bind(UINT16 nOpenPort);
	INT32 Listen(UINT32 backlog = 5);	
	INT32 InitEpoll(DWORD dwMaxSession);
	INT32 MultiAcceptThread();

private:	
	SOCK_NOTIFY_MSG	  m_notify_msg_value;
	SOCK_INIT_DATA	m_init_data;

	TMapEvtSession	t_EvtSessionMap;
	TMapSockSIDPair	t_Sock2SessionMap;
	INT32			m_nGlobalID;
	time_t			m_tAutoDisConLastChkTime;
private:
	pthread_mutex_t m_pkt_mutex;
	TQueuePktData	m_PktQueue;
	bool			AddPktWithPktLock(PKT_DATA& pkt_data);
	INT32			RemovePktWithPktLock();
	bool			AddEvtWithPktLock(SOCK_EVT_PROP& sock_evt_prop);

public:
	INT32			GetPktWithPktLock(PKT_DATA& pkt_data);
	UINT32			IsExistsPktWithPktLock();
	INT32			FreeMemory(PVOID pBuff);
	

public:	
	INT32	StartServer(LPCTSTR lpOpenAddress, UINT16 nOpenPort, SOCK_NOTIFY_MSG tSNM, SOCK_INIT_DATA tSID);
	void	StopServer(INT32 nDeleteRemainPkt);
	INT32	SendData(INT32 sessionid, UINT16 type, UINT16 code, UINT32 len, PVOID data);
	INT32	GetSocketAddressUINTWithLock(INT32 nSessionID, UINT32& nAddress);
	INT32	GetSocketAddressWithLock(INT32 nSessionID, LPSTR lpAddress);
	INT32	IsExistsSessionIDWithLock(INT32 nSessionID);
	INT32	GetConnectInfoWithLock(INT32& nCurrentConnectNum, INT32& nTotalConnectNum);
	INT32	CloseSocket(INT32 nSessionID, INT32 nRemainEvt = 1);	
	INT32	EditSessionKeyWithLock(INT32 nSessionID, UINT32 nSessionKey);
	INT32	EditSessionIDWithLock(INT32 nOldID, INT32 nNewID);
	INT32	IsRunSvr();	
	void	EditLogFileName_Ext(LPCTSTR lpLogFilePath, LPCTSTR lpLogFileName);
	INT32	GetPort() { return ntohs(m_tListenAddr.sin_port); }
	
private:
	INT32	MakeSendBuf(UINT32 session_key, UINT16 type, UINT16 code, UINT32 length, PVOID lpBuf, PVOID	*lpOutput);
	INT32	CloseSingleSocket(SOCKET s);
	
public:
	INT32	SendCoreWithLock(PEVT_SESSION pevt_session, PPKT_DATA pkt_data, INT32& nCloseSockett, INT32 nTimeOut);
	INT32	SendCoreSSLWithLock(PEVT_SESSION pevt_session, PPKT_DATA pkt_data, INT32& nCloseSockett, INT32 nTimeOut);
	
protected:	
	SOCKET			m_tListenSocket;
	SOCKET			m_tCliSocket;
	SOCKADDR_IN		m_tCliAddr;
	SOCKADDR_IN		m_tListenAddr;

private:	
	void			InitSessionRecvInfo(PEVT_SESSION pevt_session);
	INT32 			Recv(PEVT_SESSION pevt_session, INT32& nCloseSocket, INT32 nTimeOut);
	PEVT_SESSION	GetEvtSession(INT32 nSessionID);
	void			ClearSocketEventWithLock();
	INT32			AddSocketEventWithLock(SOCKET tSocket, SOCKADDR_IN tSockAddr, INT32 nChkSSL = 1);
	INT32			AddSocketEventListen(SOCKET tSocket, SOCKADDR_IN tSockAddr);
	INT32			DelSocketEventWithLock(INT32 nSessionID, SOCKET sCloseSock);
	void			AutoDisConnectByTimeOutWithLock();
	char			*INET_NTOA(struct in_addr in, char *buf, int len);
	void			SetSockKeepAlive(SOCKET tSocket, int nTime, int nIntval, int nProbeCnt);

	INT32			m_nRunServer;
	INT32			m_nContinueServer;
	char			m_LogFilePath[MAX_PATH];
	char			m_LogFileName[MAX_PATH];

	int					m_nEpollFd;					/* epoll fd */
	int					m_nMaxEvent;				/* epoll로 감시할 최대 이벤트 개수 */
	struct epoll_event	*m_pEpollEvents;			/* epoll 이벤트를 저장할 버퍼 */
	
private:
	INT32			m_nTotalConnectNum;
	pthread_t		m_hMultiAcceptThread;
	pthread_mutex_t m_mutex;
	static void	*MultiAcceptThreadSub(LPVOID lParam);	
	static void	*ExtAcceptThreadSub(LPVOID lParam);	
	static void	*ExtCliThreadSub(LPVOID lParam);	
	CSSLSockUtil	m_tSSLSockUtil;
};

#endif // !defined(AFX_SOCKETSERVER_H__AB901800_C635_4111_B7A5_90B10E0DF4DF__INCLUDED_)
