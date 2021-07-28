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

// SoketClient.h: interface for the CSoketClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOKETCLIENT_H__E321B9EE_B26C_463E_9E89_AAC50BB597F8__INCLUDED_)
#define AFX_SOKETCLIENT_H__E321B9EE_B26C_463E_9E89_AAC50BB597F8__INCLUDED_

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
#include "MutexExt.h"
#include "SSLSockUtil.h"
#include "SecurityUtil.h"
#include "com_header_socket_dll.h"


#define DEFAULT_RECVBUF_SIZE			8192
typedef queue<PKT_DATA>					TQueuePktData; 
typedef queue<PKT_DATA>					TQueuePktDataSend;

class CTCP_Client  
{

protected:
	SOCKADDR_IN			m_SA;
	SOCKET				m_nSock;
	time_t				m_nLastConnectTryTime;
	BOOL				m_bContinue;
	BOOL				m_bConnected;
	INT32				m_nConnectTryPause;
	UINT32				m_IsRunThreadRecv;
	UINT32				m_IsRunThreadSend;
	SOCK_INIT_DATA		m_InitData;
	SOCK_NOTIFY_MSG		m_NotifyMsg;

private:
	SSL*			m_pSSL;
	CSSLSockUtil	m_tSSLSockUtil;
	pthread_mutex_t m_pkt_mutex;
	TQueuePktData	m_PktQueue;

	CSecurityUtil	m_tSecuUtilRecv;
	pthread_t		m_tRecvThread;
	INT32			m_nLastChkRecvPktSize;

	CSecurityUtil	m_tSecuUtilSend;
	pthread_t		m_tSendThread;
	pthread_mutex_t 	m_pkt_send_mutex;
	TQueuePktDataSend	m_PktSendQueue;

private:
	static void *RecvThread(LPVOID lParam);
	static void *SendThread(LPVOID lParam);
	void	FreeResource();
	INT32	MakeSendBuf(UINT16 type, UINT16 code, UINT32 length, PVOID lpBuf, PVOID *lpOutput);
	INT32	Connect();
	INT32	Connect_Normal();
	INT32	Connect_SSL();
	void	SetSockKeepAlive(SOCKET tSocket, int nTime, int nIntval, int nProbeCnt);
	INT32	Recv(INT32 nTimeOut = 30);

	bool	AddPktWithPktLock(PKT_DATA& pkt_data);
	INT32	RemovePktWithPktLock();

	bool	AddEvtWithPktLock(SOCK_EVT_PROP& sock_evt_prop);

	bool	AddPktSendWithSndPktLock(PKT_DATA& pkt_data);
	UINT32	IsEmptyPktSendWithSndPktLock();
	INT32	RemovePktSendWithSndPktLock();
	INT32	GetPktSendWithSndPktLock(PKT_DATA& pkt_data);


public:
	char	m_acClassName[MAX_TYPE_LEN];

public:

	INT32	StartClient(LPCTSTR lpAddress, UINT32 dwPort, SOCK_NOTIFY_MSG notify_msg_value, SOCK_INIT_DATA init_data, char *pClassName = NULL);
	INT32	Send(UINT16 nType, UINT16 nCode, UINT32 length , PVOID data);
	INT32	CloseSocket(INT32 nFlag = 0);
	INT32	EditSessionKey(UINT32 nSessionKey);
	INT32	StopClient();
	INT32	IsRunClient();
	INT32	SendCoreWithSndPktLock(PPKT_DATA pPktData, INT32 nTimeOut);
	INT32	SendCoreSSLWithSndPktLock(PPKT_DATA pPktData, INT32 nTimeOut);
	void	WriteLog(char* fmt,...);
	UINT32	IsExistPktWithPktLock();	
    void	ClearAllPkt();

	INT32	GetPktWithPktLock(PKT_DATA& pkt_data);
	INT32	EditNotifyMsgValue(SOCK_NOTIFY_MSG notify_msg_value);
	INT32	SetConnectTryFlag(INT32 nFlag);
	INT32	GetLocalAddress(UINT32& nIPAddress);
	INT32	FreeMemory(PVOID pBuff);

public:
	CTCP_Client();
	virtual ~CTCP_Client();

};

#endif // !defined(AFX_SOKETCLIENT_H__E321B9EE_B26C_463E_9E89_AAC50BB597F8__INCLUDED_)
