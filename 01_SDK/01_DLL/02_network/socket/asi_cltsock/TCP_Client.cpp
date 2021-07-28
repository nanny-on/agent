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

// SoketClient.cpp: implementation of the CTCP_Client class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "TCP_Client.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTCP_Client::CTCP_Client()
{
	m_nSock                 = -1;
	m_pSSL                 = NULL;
	m_bConnected           = FALSE;
	m_bContinue            = TRUE;
	m_IsRunThreadRecv      = 0;
	m_IsRunThreadSend      = 0;
	m_nLastConnectTryTime  = 0;
	m_nLastChkRecvPktSize  = 0;
	m_nConnectTryPause     = 0;
	m_tRecvThread          = 0;
	m_tSendThread          = 0;

	memset(&m_InitData, 0, sizeof(m_InitData));
	memset(&m_NotifyMsg, 0, sizeof(m_NotifyMsg));

	m_InitData.nRemainDebugLog = 1;
	m_InitData.m_nFileLogRetention = 5;
	
	memset(&m_SA, 0, sizeof(SOCKADDR_IN));

	pthread_mutex_init(&m_pkt_mutex, NULL);
	pthread_mutex_init(&m_pkt_send_mutex, NULL);
}
//---------------------------------------------------------------------------------------------------------------------------

CTCP_Client::~CTCP_Client()
{
	CloseSocket(1);
	RemovePktSendWithSndPktLock();
	pthread_mutex_destroy(&m_pkt_mutex);
	pthread_mutex_destroy(&m_pkt_send_mutex);
}
//---------------------------------------------------------------------------------------------------------------------------

void		CTCP_Client::FreeResource()
{
	if (m_nSock != -1)
	{
		close(m_nSock);
		m_nSock = -1;
	}
}
//---------------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::StartClient(LPCTSTR lpAddress, UINT32 dwPort, SOCK_NOTIFY_MSG notify_msg_value, SOCK_INIT_DATA init_data, char *pClassName)
{
	INT32 nRetVal = 0;
	UINT32 dwEncType = ASI_PKT_ENC_TYPE_SEED_SHA2;

	if(pClassName)
		strncpy(m_acClassName, pClassName, MAX_TYPE_LEN-1);
	else
		strncpy(m_acClassName, "unknown", MAX_TYPE_LEN-1);

	if(lpAddress == NULL || dwPort == 0 || dwPort > 65536)
	{
		WriteLog("[Error] [%s] [StartClient] invalid input data (-1)", m_acClassName);
		return -1;
	}

	
	memcpy(&m_InitData, &init_data, sizeof(m_InitData));
	memcpy(&m_NotifyMsg, &notify_msg_value, sizeof(m_NotifyMsg));

	if (m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SEED_SHA2)
	{
		dwEncType = ASI_BUFFER_ENC_TYPE_SEED;
	}
	else
	{
		dwEncType = ASI_BUFFER_ENC_TYPE_TLS1;
	}

	m_tSecuUtilSend.SetMode(dwEncType);
	m_tSecuUtilSend.SetSessionKey(m_InitData.nSessionKey);

	m_tSecuUtilRecv.SetMode(dwEncType);
	m_tSecuUtilRecv.SetSessionKey(m_InitData.nSessionKey);

	if (m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SSL && m_tSSLSockUtil.Init_SSL(0))			
	{
		WriteLog("[Error] [%s] [StartClient] fail to init ssl (-2) (%d)", m_acClassName, errno);
		return -2;
	}

	m_SA.sin_family = AF_INET;
	m_SA.sin_port = ntohs((short)dwPort);
	m_SA.sin_addr.s_addr = inet_addr(lpAddress);

	do{
		nRetVal = Connect();
		if (nRetVal != 0)
		{
			nRetVal -= 10;
			break;
		}
		m_IsRunThreadRecv = 1;
		m_bContinue = TRUE;
		if (pthread_create(&m_tRecvThread, NULL, RecvThread, (void *)this) != 0)
		{
			WriteLog("[Error] [%s] [StartClient] fail to create recv thread (-3) (%d)", m_acClassName, errno);
			nRetVal = -3;
			break;
		}

		if (m_InitData.nPktPoolFlag & ASI_PKT_POOL_FLAG_SEND)
		{
			m_IsRunThreadSend = 1;
			if (pthread_create(&m_tSendThread, NULL, SendThread, (void *) this) != 0)
			{
				WriteLog("[Error] [%s] [StartClient] fail to create send thread (-4) (%d)", m_acClassName, errno);
				nRetVal = -4;
				break;
			}
		}
		nRetVal = 0;

		WriteLog("[Info] [%s] [StartClient] success to connect %s:%d", m_acClassName, lpAddress, dwPort);

	}while(FALSE);
	if(nRetVal != 0)
	{
		FreeResource();
		StopClient();
		m_bContinue = FALSE;
	}

	return nRetVal;	
}
//---------------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::StopClient()
{
	m_bContinue = FALSE;

	while (this->m_IsRunThreadRecv == 2 || this->m_IsRunThreadSend == 2)
	{
		usleep(10000);
	}

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::CloseSocket(INT32 nFlag)
{
	SOCK_EVT_PROP sock_evt_prop;
	if (m_bConnected)
	{
		if(m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SSL && m_pSSL != NULL)
		{
			m_tSSLSockUtil.ShutDown_SSL(m_pSSL);
		}

		m_nLastConnectTryTime = time(NULL);

		FreeResource();
		
		if (m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SSL && m_pSSL != NULL)
		{
			m_tSSLSockUtil.Free_SSL(m_pSSL);
			m_pSSL = NULL;
		}

		if (m_InitData.nRemainSockEvt)
		{
			sock_evt_prop.nEventID	= ASI_SOCKET_EVENT_DISCONNECT;
			AddEvtWithPktLock(sock_evt_prop);
			WriteLog("[Info] [%s] [RecvThread] add socket event disconnect", m_acClassName);
			Sleep(500);
		}

		RemovePktWithPktLock();
		m_bConnected = FALSE;

	}

	if(nFlag)
		StopClient();

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::MakeSendBuf(UINT16 type, UINT16 code, UINT32 length, PVOID lpBuf, PVOID	*lpOutput)
{
	PKT_DATA_HDR_SHA256	pkt_hdr_sha256;
	PKT_DATA_HDR_SSL	pkt_hdr_ssl;

	INT32 nTotalLen = 0;
	UINT32 nEncLen = 0;
	PBYTE lpPlanText = NULL;
	PBYTE lpEncText = NULL;
	BYTE acDstHash[40] = {0,};

	if(lpOutput == NULL)
		return -1;

	
	if (m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SEED_SHA2)
	{
		nEncLen = m_tSecuUtilSend.Encrypt_Len(length);
		pkt_hdr_sha256.type = ntohs(type);
		pkt_hdr_sha256.code	= ntohs(code);
		pkt_hdr_sha256.length = ntohl(nEncLen);

		if (lpBuf)
		{
			nTotalLen = nEncLen + PKT_DATA_HDR_SHA256_SIZE;
		}
		else
			nTotalLen = PKT_DATA_HDR_SHA256_SIZE;

		*lpOutput = (PBYTE)malloc(sizeof(BYTE) * nTotalLen + 1);
		if(*lpOutput == NULL)
			return -2;
		
		memset(*lpOutput, 0, sizeof(BYTE) * nTotalLen + 1);

		if (lpBuf && length > 0)
		{
			lpEncText = (PBYTE)*lpOutput + PKT_DATA_HDR_SHA256_SIZE;
			m_tSecuUtilSend.Encrypt((PBYTE)lpBuf, length, &lpEncText, nEncLen);
			m_tSecuUtilSend.GetSha256(lpEncText, nEncLen, acDstHash, sizeof(acDstHash));
			memcpy(pkt_hdr_sha256.hash, acDstHash, 32);
		}
		memcpy(*lpOutput, &pkt_hdr_sha256, PKT_DATA_HDR_SHA256_SIZE);
	}	
	else if (m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
	{
		nEncLen = m_tSecuUtilSend.Encrypt_Len(length);
		pkt_hdr_ssl.type	= ntohs(type);
		pkt_hdr_ssl.code	= ntohs(code);
		pkt_hdr_ssl.length	= ntohl(nEncLen);
		
		if (lpBuf)
			nTotalLen = nEncLen + sizeof(pkt_hdr_ssl);
		else
			nTotalLen = sizeof(pkt_hdr_ssl);
		
		*lpOutput = (BYTE*)malloc(sizeof(BYTE) * nTotalLen + 1);
		if(*lpOutput == NULL)
			return -3;

		memset(*lpOutput, 0x00, sizeof(BYTE) * nTotalLen + 1);
		
		if (lpBuf && length)
		{
			lpEncText = (PBYTE)*lpOutput + PKT_DATA_HDR_SSL_SIZE;		
			m_tSecuUtilSend.Encrypt((PBYTE)lpBuf, length, &lpEncText, nEncLen);
		}
		memcpy(*lpOutput, &pkt_hdr_ssl, sizeof(pkt_hdr_ssl));
	}

	return nTotalLen;
}
//---------------------------------------------------------------------------------------------------------------------

void *CTCP_Client::SendThread(LPVOID lParam)
{
	PKT_DATA  pkt_data;
	INT32 nTimeOut = 30;

	CTCP_Client *pTcpClient = (CTCP_Client *)lParam;
	if(pTcpClient == NULL)
	{
		return (void *)NULL;
	}

	pthread_detach(pthread_self());

	pTcpClient->m_IsRunThreadSend = 2;
	
	pTcpClient->WriteLog("[Info] [%s] [SendThread] send thread start (%d)", pTcpClient->m_acClassName, pTcpClient->m_bContinue);

	while (pTcpClient->m_bContinue)
	{
		if (pTcpClient->m_bConnected == FALSE)	
		{		
//			pTcpClient->RemovePktSendWithSndPktLock();
//			pTcpClient->WriteLog("[Info] [%s] [SendThread] clear send buffer (%d)", pTcpClient->m_acClassName, pTcpClient->m_bContinue);
			Sleep(10);
			continue;
		}
				
		while (pTcpClient->GetPktSendWithSndPktLock(pkt_data))
		{	
			if(pTcpClient->m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
				pTcpClient->SendCoreSSLWithSndPktLock(&pkt_data, nTimeOut);
			else
				pTcpClient->SendCoreWithSndPktLock(&pkt_data, nTimeOut);
		}
		Sleep(10);
	}	

	pTcpClient->WriteLog("[Info] [%s] [SendThread] send thread end", pTcpClient->m_acClassName);

	pTcpClient->m_IsRunThreadSend = 0;
	return (void *)NULL;
}
//---------------------------------------------------------------------------------------------------------------------------

INT32	CTCP_Client::Send(UINT16 wType, UINT16 wCode, UINT32 dwLength, PVOID pData)
{
	PKT_DATA stPktData;
	PVOID lpOutput = NULL;
	INT32 nMakeBufSize = 0;
	INT32 nRtn = 0;
	INT32 nTimeOut = 30;
	BOOL bConnected = TRUE;
	UINT32	nPktEncType = 0;
	if (dwLength > MAKE_BUFFER_MAX_SIZE || dwLength == 0)
	{
		WriteLog("[Error] [%s] [Send] invalid send length : [type:%d][code:%d][length:%d] (-2)", m_acClassName, wType, wCode, dwLength);
		return -2;
	}	

	nMakeBufSize = MakeSendBuf(wType, wCode, dwLength, pData, &lpOutput);
	if (nMakeBufSize < 1)
	{
		WriteLog("[Error] [%s] [Send] fail to make buf size is [%d] (-3) (%d)", m_acClassName, nMakeBufSize, errno);
		return -3;
	}

	memset(&stPktData, 0, sizeof(stPktData));
	stPktData.hdr.type	= wType;
	stPktData.hdr.code	= wCode;
	stPktData.hdr.length = nMakeBufSize;
	stPktData.body.data	= (BYTE*)lpOutput;		

	if((m_InitData.nPktPoolFlag & ASI_PKT_POOL_FLAG_SEND) != 0)
	{
		this->AddPktSendWithSndPktLock(stPktData);
		return 0;
	}

	pthread_mutex_lock(&m_pkt_send_mutex);
	bConnected = m_bConnected;
	nPktEncType = m_InitData.nPktEncType;
	pthread_mutex_unlock(&m_pkt_send_mutex);

	if (bConnected == FALSE)
	{
		WriteLog("[Info] [%s] [Send] close to thread (-1)", m_acClassName);
		safe_free(stPktData.body.data);
		return -4;
	}
	
	if (nPktEncType == ASI_PKT_ENC_TYPE_SSL)
		nRtn = SendCoreSSLWithSndPktLock(&stPktData, nTimeOut);
	else
		nRtn = SendCoreWithSndPktLock(&stPktData, nTimeOut);
	
	return nRtn;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_Client::SendCoreWithSndPktLock(PPKT_DATA pPktData, INT32 nTimeOut)
{
	INT32 nSendedSize = 0;
	INT32 nSendSize = 0;
	INT32 nOnceSendSize = 0;
	INT32 nRtn = 0;
	INT32 nFlag = 0;
	time_t	nStartTime = time(NULL);
	time_t	nEndTime;
	INT32 nWouldBlockCnt = 0;
	INT32 nMakeBufSize = 0;
	char *pcOutput = NULL;
	INT32 nStatePos = 0;
	
	if(pPktData == NULL)
	{
		WriteLog("[Error] [%s] [SendCoreWithSndPktLock] invalid input data (-1)", m_acClassName);
		return -1;
	}
	nMakeBufSize = pPktData->hdr.length;
	pcOutput = (char *)pPktData->body.data;
	if(nMakeBufSize < 1 || pcOutput == NULL)
	{
		WriteLog("[Error] [%s] [SendCoreWithSndPktLock] invalid input data (-2)", m_acClassName);
		return -2;
	}

	pthread_mutex_lock(&m_pkt_send_mutex);
	while(nSendedSize < nMakeBufSize && nRtn == 0)
	{
		if( (nMakeBufSize - nSendedSize) > m_InitData.nPktPagingSize )
			nOnceSendSize = m_InitData.nPktPagingSize;
		else
			nOnceSendSize = nMakeBufSize - nSendedSize;
		
		nSendSize = send(m_nSock, &pcOutput[nSendedSize], nOnceSendSize, nFlag);
		if(nSendSize == -1)
		{
			if(errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nEndTime = time(NULL);
				if(difftime(nEndTime, nStartTime) < nTimeOut)
				{					
					Sleep(10);
					continue;
				}
				else
				{
					WriteLog("[Info] [%s] [SendCoreWithSndPktLock] send time out. socket close (-3) (%d)", m_acClassName, nTimeOut);
					nRtn = -3;
					CloseSocket();
					break;
				}
			}
			else
			{
				WriteLog("[Error] [%s] [SendCoreWithSndPktLock] fail to send. socket close (-4) (%d)", m_acClassName, errno);
				CloseSocket();
				nRtn = -4;
				break;
			}
		}
		else if(nSendSize == 0)
		{
			WriteLog("[Info] [%s] [SendCoreWithSndPktLock] detect session close. socket close (-5)", m_acClassName);
			CloseSocket();
			nRtn = -5;
			break;
		}
		else
		{			
			nSendedSize += nSendSize;
			nStartTime = time(NULL);
		}
	}
	pthread_mutex_unlock(&m_pkt_send_mutex);
	safe_free(pPktData->body.data);
	return nRtn;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	CTCP_Client::SendCoreSSLWithSndPktLock(PPKT_DATA pPktData, INT32 nTimeOut)
{
	INT32 nSendLen = 0;
	INT32 nBufferSize = 0;
	INT32 nSendedLen = 0;
	INT32 nMakeBufSize = 0;
	char *pcHeader = NULL;
	INT32 nRtn = 0;
	time_t	nStartTime = 0;
	time_t	nEndTime = 0;

	if(pPktData == NULL)
	{
		WriteLog("[Error] [%s] [SendCoreSSLWithSndPktLock] invalid input data (-1)", m_acClassName);
		return -1;
	}

	nMakeBufSize = pPktData->hdr.length;
	pcHeader = (char *)pPktData->body.data;
	if(nMakeBufSize < 1 || pcHeader == NULL)
	{
		WriteLog("[Error] [%s] [SendCoreSSLWithSndPktLock] invalid input data (-2)", m_acClassName);
		return -2;
	}

	pthread_mutex_lock(&m_pkt_send_mutex);
	nBufferSize = nMakeBufSize;
	nStartTime = time(NULL);
	while(nBufferSize != nSendedLen && !nRtn)
	{
		nSendLen = m_tSSLSockUtil.SSL_SendBuf(m_pSSL, &pcHeader[nSendedLen], nBufferSize - nSendedLen);
		if(nSendLen == -1)
		{
			if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nEndTime = time(NULL);
				if(difftime(nEndTime, nStartTime) > nTimeOut)
				{
					WriteLog("[Info] [%s] [SendCoreSSLWithSndPktLock] send time out. socket close (-3) (%d)", m_acClassName, nTimeOut);
					CloseSocket();
					nRtn = -3;
					break;
				}			
				Sleep(10);
				continue;
			}
			else
			{
				WriteLog("[Error] [%s] [SendCoreSSLWithSndPktLock] fail to send. socket close (-4) (%d)", m_acClassName, errno);
				CloseSocket();
				nRtn = -4;
				break;
			}
		}
		else if(nSendLen == 0)
		{
			WriteLog("[Info] [%s] [SendCoreSSLWithSndPktLock] detect session close. socket close (-5)", m_acClassName);
			CloseSocket();
			nRtn = -5;
			break;
		}
		else
		{
			nSendedLen += nSendLen;
			nStartTime = time(NULL);
			nRtn = 0;
		}
	}
	pthread_mutex_unlock(&m_pkt_send_mutex);
	safe_free(pcHeader);
	return nRtn;
}

INT32 CTCP_Client::Recv(INT32 nTimeOut)
{
	UINT32 nRecvedSize = 0;
	INT32 nRecvSize = 0;
	INT32 nOnceRecvSize = 0;
	UINT32 nBodySize = 0;

	char *pRecvHdr = NULL;
	char *pRecvData = NULL;

	time_t nStartTime = 0;
	time_t nEndTime = 0;

	PKT_DATA_HDR_FIRST		pkt_hdr_first;
	PKT_DATA_HDR_SECOND		pkt_hdr_second;
	PKT_DATA_HDR_ALL		pkt_hdr_all;
	PKT_DATA_HDR_SHA256		pkt_hdr_sha256;
	PKT_DATA_HDR_SSL		pkt_hdr_ssl;

	PBYTE lpPlanText = NULL;
	UINT32 nPlanLen = 0;

	PKT_DATA stPktData;

	nStartTime = time(NULL);

	if(m_InitData.nPktHdrSize > 1023)
	{
		WriteLog("[Error] [%s] [Recv] invalid hdr size (-1) (%d)", m_acClassName, m_InitData.nPktHdrSize);
		return -1;
	}
	
	pRecvHdr = (char *)malloc(1024);
	if(pRecvHdr == NULL)
	{
		WriteLog("[Error] [%s] [Recv] fail to allocate memory (-2) (%d)", m_acClassName, errno);
		return -2;
	}
	memset(pRecvHdr, 0, 1024);

	while (nRecvedSize != m_InitData.nPktHdrSize && m_bContinue)
	{
		nOnceRecvSize = m_InitData.nPktHdrSize - nRecvedSize;			
		
		if (m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
		{
			nRecvSize = m_tSSLSockUtil.SSL_RecvBuf(m_pSSL, &pRecvHdr[nRecvedSize], nOnceRecvSize);
		}
		else
		{
			nRecvSize = recv(m_nSock, &pRecvHdr[nRecvedSize], nOnceRecvSize, 0);
		}		
		
		if (nRecvSize == -1)
		{
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nEndTime = time(NULL);
				if(difftime(nEndTime, nStartTime) > nTimeOut)
				{
					WriteLog("[Info] [%s] [Recv] recv header time out. socket close (-3) (%d)", m_acClassName, nTimeOut);
					CloseSocket();
					safe_free(pRecvHdr);
					return -3;
				}
				Sleep(10);
				continue;
			}
			else
			{
				WriteLog("[Error] [%s] [Recv] fail to recv header. socket close (-4) (%d)", m_acClassName, errno);
				CloseSocket();
				safe_free(pRecvHdr);
				return -4;
			}
		}
		else if (nRecvSize == 0)	//	socket close
		{
			WriteLog("[Info] [%s] [Recv] detect session close. socket close (-5)", m_acClassName);
			safe_free(pRecvHdr);
			CloseSocket();
			return -5;
		}
		else
		{
			nRecvedSize += nRecvSize;
			nStartTime = time(NULL);
		}
	}

	if(m_bContinue == FALSE)
	{
		WriteLog("[Info] [%s] [Recv] close to thread (-6)", m_acClassName);
		safe_free(pRecvHdr);
		return -6;
	}

	if(m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SEED_SHA2)
	{
		memcpy(&pkt_hdr_sha256, pRecvHdr, sizeof(pkt_hdr_sha256));
		nBodySize = ntohl(pkt_hdr_sha256.length);
	}
	else
	{
		memcpy(&pkt_hdr_ssl, pRecvHdr, sizeof(pkt_hdr_ssl));
		nBodySize = ntohl(pkt_hdr_ssl.length);
	}
	safe_free(pRecvHdr);

	pRecvData = (char*)malloc(nBodySize + 1);
	if(pRecvData == NULL)
	{
		WriteLog("[Error] [%s] [Recv] fail to allocate memory (-6) (%d) (%d)", m_acClassName, nBodySize, errno);
		safe_free(pRecvHdr);
		return -7;
	}
	memset(pRecvData, 0, nBodySize + 1);

	nTimeOut = 60;
	nRecvedSize = 0;
	nStartTime = time(NULL);
	while (nRecvedSize != nBodySize && m_bContinue)
	{
		if (nBodySize - nRecvedSize > m_InitData.nPktPagingSize)
			nOnceRecvSize = m_InitData.nPktPagingSize;
		else
			nOnceRecvSize = nBodySize - nRecvedSize;		
		
		if (m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
		{
			nRecvSize = m_tSSLSockUtil.SSL_RecvBuf(m_pSSL, &pRecvData[nRecvedSize], nOnceRecvSize);
		}
		else
		{
			nRecvSize = recv(m_nSock, &pRecvData[nRecvedSize], nOnceRecvSize, 0);
		}
		
		if(nRecvSize == -1)
		{
			if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				nEndTime = time(NULL);
				if(difftime(nEndTime, nStartTime) > nTimeOut)
				{
					WriteLog("[Info] [%s] [Recv] recv body time out. socket close (-8) (%d)", m_acClassName, nTimeOut);
					safe_free(pRecvData);
					CloseSocket();
					return -8;
				}
				Sleep(10);
				continue;
			}
			else
			{
				WriteLog("[Error] [%s] [Recv] fail to recv body. socket close (-9) (%d)", m_acClassName, errno);
				safe_free(pRecvData);
				CloseSocket();
				return -9;
			}
		}
		else if(nRecvSize == 0)
		{
			WriteLog("[Info] [%s] [Recv] detect session close. socket close (-10)", m_acClassName);
			safe_free(pRecvData);
			CloseSocket();
			return -10;
		}
		else
		{
			nRecvedSize += nRecvSize;			
			nStartTime = time(NULL);
		}
	}
	
	if(m_bContinue == FALSE)
	{
		WriteLog("[Info] [%s] [Recv] close to thread (-10)", m_acClassName);
		safe_free(pRecvData);
		return -10;
	}
	
	if(m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SEED_SHA2)
	{
		if(m_tSecuUtilRecv.CompareSha256((BYTE*)pRecvData, nBodySize, pkt_hdr_sha256.hash))
		{
			char pszChkHash[257] = {0, };
			char pszCurHash[257] = {0, };
			
			m_tSecuUtilRecv.ConvertSha256(pkt_hdr_sha256.hash, pszChkHash, sizeof(pszChkHash));
			m_tSecuUtilRecv.ConvertSha256Last(pszCurHash, sizeof(pszCurHash));
			WriteLog("[Error] [%s] [Recv] invalid hash value : [chk:%s]--[cur:%s] (-11)", m_acClassName, pszChkHash, pszCurHash);
			safe_free(pRecvData);
			return -11;
		}
	}
	m_tSecuUtilRecv.Decrypt((BYTE*)pRecvData, nBodySize, &lpPlanText, nPlanLen);	

	stPktData.hdr.length = nPlanLen;
	stPktData.hdr.type	= ntohs(pkt_hdr_sha256.type);
	stPktData.hdr.code	= ntohs(pkt_hdr_sha256.code);
	stPktData.body.data = (BYTE*)lpPlanText;

	safe_free(pRecvData);

	AddPktWithPktLock(stPktData);
	return 0;
}

//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::Connect()
{
	INT32 nRtn = 0;
	if(m_bConnected)
		return 0;

	m_nSock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_nSock == -1)
	{
		WriteLog("[Error] [%s] [Connect] fail to create socket (-1) (%d)", m_acClassName, errno);
		return -1;
	}

	
	if(m_InitData.nPktEncType == ASI_PKT_ENC_TYPE_SSL)
	{
		nRtn = Connect_SSL();
		if(nRtn != 0)
		{
			return nRtn;
		}
	}
	else
	{
		nRtn = Connect_Normal();
		if(nRtn != 0)
		{
			return nRtn;
		}
	}
	SetSockKeepAlive(m_nSock, 5, 5, 5);
	m_bConnected = TRUE;
	return 0;	
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::Connect_Normal()
{
	socklen_t       nLen;
	int             nRetVal = 0;
	int             nFlags, nError;
	fd_set          tReadSet, tWriteSet;
	struct timeval  tTimeVal;

	if(m_nSock == -1)
	{
		WriteLog("[Error] [%s] [Connect_Normal] invalid socket (-1)", m_acClassName);
		return -1;
	}

	do{

		nFlags = fcntl(m_nSock, F_GETFL, 0);
		if (nFlags < 0)
		{
			WriteLog("[Error] [%s] [Connect_Normal] fail to get fcntl (-2) (%d)", m_acClassName, errno);
			nRetVal = -2;
			break;
		}
		
		if (fcntl(m_nSock, F_SETFL, nFlags | O_NONBLOCK) < 0)
		{	
			WriteLog("[Error] [%s] [Connect_Normal] fail to set fcntl (-3) (%d)", m_acClassName, errno);
			nRetVal = -3;
			break;
		}

		nRetVal = connect(m_nSock, (SOCKADDR *) &m_SA, sizeof(m_SA));
		if (nRetVal < 0)
		{
			if (errno == EINPROGRESS)
			{
				FD_ZERO(&tReadSet);
				FD_SET(m_nSock, &tReadSet);
				tWriteSet = tReadSet;
				tTimeVal.tv_sec = 15; 
				tTimeVal.tv_usec = 0;
				nRetVal = select(m_nSock + 1, &tReadSet, &tWriteSet, NULL, &tTimeVal);
				
				if (nRetVal == 0)
				{
					WriteLog("[Error] [%s] [Connect_Normal] fail to set select option (-4) (%d)", m_acClassName, errno); 	
					nRetVal = -4;
					break;
				}	
				
				nLen = sizeof(int);
				
				getsockopt(m_nSock, SOL_SOCKET, SO_ERROR, &nError, &nLen); 
				
				if (nError) 
				{
//					WriteLog("[Error] [Connect_Normal] fail to get sock option (-5) (%d)", errno); 	
					nRetVal = -5;
					break;
				
				}
			}
			else
			{
				WriteLog("[Error] [%s] [Connect_Normal] fail to connect (-7) (%d)", m_acClassName, errno); 	
				nRetVal = -7;
				break;
			}
		}
		nRetVal = 0;
 	}while(FALSE);

	if(nRetVal == 0)
	{
		nRetVal = fcntl(m_nSock, F_SETFL, nFlags);
		if (nRetVal < 0)
		{
			WriteLog("[Error] [%s] [Connect_Normal] fail to set fcntl (-8) (%d)", m_acClassName, errno);
 			nRetVal = -8;
		}
 	}
	
	if(nRetVal != 0)
	{
		if(m_nSock != -1)
		{
			close(m_nSock);
			m_nSock = -1;
		}
	}
	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::Connect_SSL()
{	
	INT32 nReVal = 0;

	nReVal = Connect_Normal();
	if (nReVal != 0 || m_nSock == -1)
	{
		nReVal -= 100;
		return nReVal;
	}

	m_pSSL = m_tSSLSockUtil.GetSSL_Connect(m_nSock, nReVal);
	
	if(m_pSSL == NULL)
	{
		WriteLog("[Error] [%s] [Connect_SSL] fail to connect ssl (-2) (%d) (%d)", m_acClassName, nReVal, errno);
		close(m_nSock);
		m_nSock = -1;
		return -2;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------

void CTCP_Client::SetSockKeepAlive(SOCKET tSocket, int nTime, int nIntval, int nProbeCnt)
{
	int  nKeepAliveOn = 1;

	setsockopt(tSocket, SOL_TCP, TCP_KEEPIDLE, (void *) &nTime, sizeof(nTime));
	setsockopt(tSocket, SOL_TCP, TCP_KEEPINTVL, (void *) &nIntval, sizeof(nIntval));
	setsockopt(tSocket, SOL_TCP, TCP_KEEPCNT, (void *) &nProbeCnt, sizeof(nProbeCnt));
	setsockopt(tSocket, SOL_SOCKET, SO_KEEPALIVE, (void *) &nKeepAliveOn, sizeof(nKeepAliveOn));
}
//---------------------------------------------------------------------------------------------------------------------

void *CTCP_Client::RecvThread(LPVOID lParam)
{
	INT32           nRc = 0;
	fd_set          tReadFds;
	struct timeval  tTimeVal;
	time_t nCurrentTime = 0;
	SOCK_EVT_PROP sock_evt_prop;
	CTCP_Client *pTcpClient = (CTCP_Client *)lParam;

	if(pTcpClient == NULL)
	{
		return (void *)NULL;
	}

	pthread_detach(pthread_self());

	pTcpClient->WriteLog("[Info] [%s] [RecvThread] recv thread start (%d)", pTcpClient->m_acClassName, pTcpClient->m_bContinue);

	pTcpClient->m_IsRunThreadRecv = 2;	

	while (pTcpClient->m_bContinue)
	{
		if (pTcpClient->m_bConnected == FALSE)	
		{
			if (pTcpClient->m_InitData.nOpTypeClt == ASI_CLT_TCP_EVENT_OP_TYPE_DEFAULT)
			{
				pTcpClient->WriteLog("[Info] [%s] [RecvThread] op type default and break", pTcpClient->m_acClassName);
				break;
			}

			nCurrentTime = time(NULL);
			if (pTcpClient->m_nLastConnectTryTime == 0 || difftime(nCurrentTime, pTcpClient->m_nLastConnectTryTime) > 2 && 
					pTcpClient->m_nConnectTryPause == 0)
			{
				pTcpClient->m_nLastConnectTryTime = time(NULL);
				if(pTcpClient->Connect() == 0)
				{
					if(pTcpClient->m_InitData.nRemainSockEvt)
					{
						pTcpClient->WriteLog("[Info] [%s] [RecvThread] add socket event connect", pTcpClient->m_acClassName);
						sock_evt_prop.nEventID = ASI_SOCKET_EVENT_CONNECT;
						pTcpClient->AddEvtWithPktLock(sock_evt_prop);
					}
				}
			}
			Sleep(10);
			continue;
		}

		tTimeVal.tv_sec   = 0;
		tTimeVal.tv_usec  = 10000;

		FD_ZERO(&tReadFds);
		FD_SET(pTcpClient->m_nSock, &tReadFds);

		if (select(pTcpClient->m_nSock + 1, &tReadFds, NULL, NULL, &tTimeVal) < 0)
		{
			Sleep(100);
			continue;
		}

		if (FD_ISSET(pTcpClient->m_nSock, &tReadFds))
		{
			pTcpClient->Recv();
		}
	}	

	pTcpClient->WriteLog("[Info] [%s] [RecvThread] recv thread end", pTcpClient->m_acClassName);

	pTcpClient->m_IsRunThreadRecv = 0;
	return (void *)NULL;
}
//---------------------------------------------------------------------------------------------------------------------------

bool		CTCP_Client::AddPktWithPktLock(PKT_DATA& pkt_data)
{
	INT32 nChkValue = 0;
	pthread_mutex_lock(&m_pkt_mutex);
	m_PktQueue.push(pkt_data);		
	nChkValue = m_PktQueue.size() - m_nLastChkRecvPktSize;
	if( abs(nChkValue) == 500)
	{
		m_nLastChkRecvPktSize = m_PktQueue.size();
	}
	pthread_mutex_unlock(&m_pkt_mutex);
	return true;
}
//---------------------------------------------------------------------------------------------------------------------

INT32		CTCP_Client::GetPktWithPktLock(PKT_DATA& pkt_data)
{
	INT32 nResult = 0;
	INT32 nChkValue = 0;
	pthread_mutex_lock(&m_pkt_mutex);
	if( ( nResult = m_PktQueue.size()) > 0)
	{
		pkt_data = m_PktQueue.front();
		m_PktQueue.pop();

		nChkValue = m_PktQueue.size() - m_nLastChkRecvPktSize;
		if( m_nLastChkRecvPktSize != 0 && abs(nChkValue) == 10)
		{
			m_nLastChkRecvPktSize = m_PktQueue.size();
		}
	}
	pthread_mutex_unlock(&m_pkt_mutex);
	return nResult;
}
//-------------------------------------------------------------------------------------------------------------------

UINT32		CTCP_Client::IsExistPktWithPktLock()
{
	UINT32 nResult = 0;
	pthread_mutex_lock(&m_pkt_mutex);
	nResult = m_PktQueue.size();
	pthread_mutex_unlock(&m_pkt_mutex);
	return nResult;	
}
//---------------------------------------------------------------------------------------------------------------------

INT32		CTCP_Client::RemovePktWithPktLock()
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


void	CTCP_Client::ClearAllPkt()
{
	RemovePktSendWithSndPktLock();
}

//---------------------------------------------------------------------------------------------------------------------

bool		CTCP_Client::AddEvtWithPktLock(SOCK_EVT_PROP& sock_evt_prop)
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
//---------------------------------------------------------------------------------------------------------------------------

bool			CTCP_Client::AddPktSendWithSndPktLock(PKT_DATA& pkt_data)
{
	pthread_mutex_lock(&m_pkt_send_mutex);
	m_PktSendQueue.push(pkt_data);				
	pthread_mutex_unlock(&m_pkt_send_mutex);
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------

UINT32			CTCP_Client::IsEmptyPktSendWithSndPktLock()
{
	UINT32 nResult = 0;
	pthread_mutex_lock(&m_pkt_send_mutex);
	nResult = m_PktSendQueue.size();
	pthread_mutex_unlock(&m_pkt_send_mutex);
	return nResult;	
}
//---------------------------------------------------------------------------------------------------------------------------

INT32			CTCP_Client::RemovePktSendWithSndPktLock()	
{
	PKT_DATA pkt_data;
	pthread_mutex_lock(&m_pkt_send_mutex);
	while(!m_PktSendQueue.empty())
	{
		pkt_data = m_PktSendQueue.front();
		m_PktSendQueue.pop();
		safe_free(pkt_data.body.data);			
	}
	pthread_mutex_unlock(&m_pkt_send_mutex);
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------

INT32			CTCP_Client::GetPktSendWithSndPktLock(PKT_DATA& pkt_data)
{
	INT32 nRetVal = 0;
	pthread_mutex_lock(&m_pkt_send_mutex);
	nRetVal = m_PktSendQueue.size();
	if(nRetVal > 0)
	{
		pkt_data = m_PktSendQueue.front();
		m_PktSendQueue.pop();			
	}
	pthread_mutex_unlock(&m_pkt_send_mutex);
	return nRetVal;
}
//---------------------------------------------------------------------------------------------------------------------------

void CTCP_Client::WriteLog(char* fmt,...)
{
	FILE *fp = NULL;
	va_list  args;
	char acSaveFile[MAX_PATH] = {0, };
	char acBuf[CHAR_MAX_SIZE] = {0, };
	char acTimeBuf[MAX_TIME_STR] = {0, };

	pthread_mutex_lock(&m_pkt_mutex);
	do{
		if (m_InitData.nRemainDebugLog == 0)
			break;

		if(m_InitData.szLogFilePath[0] == 0)
		{
			if(get_nanny_agent_root(acSaveFile, MAX_PATH-1) != 0)
				break;
			snprintf(m_InitData.szLogFilePath, MAX_PATH-1, "%s/nanny/log", acSaveFile);
		}

		if(m_InitData.szLogFileName[0] == 0)
		{
			snprintf(m_InitData.szLogFileName, MAX_PATH-1, "/nanny_agt_clt_sock_");
		}

		GetCurrentDateTime(0, acTimeBuf);

		snprintf(acSaveFile, MAX_PATH-1, "%s%s%s.txt", m_InitData.szLogFilePath, m_InitData.szLogFileName, acTimeBuf);
		acSaveFile[MAX_PATH-1] = 0;

		if(is_file(acSaveFile) != 0)
		{
			ClearOldLogFile(m_InitData.szLogFilePath, m_InitData.szLogFileName, m_InitData.m_nFileLogRetention);
		}

		fp = fopen(acSaveFile, "a");
		if (fp == NULL)
		{
			break;
		}
		GetCurrentDateTime(1, acTimeBuf);
		va_start(args,fmt);
		vsnprintf(acBuf, CHAR_MAX_SIZE-1, fmt, args);		
		va_end(args);
		fprintf(fp, "%s\t%s\n", acTimeBuf, acBuf);
		fclose(fp);
	}while(FALSE);
	pthread_mutex_unlock(&m_pkt_mutex);
}
//---------------------------------------------------------------------------------------------------------------------------


INT32 CTCP_Client::IsRunClient()
{
	return m_bConnected;
}
//---------------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::EditSessionKey(UINT32 nSessionKey)
{
	m_tSecuUtilSend.SetSessionKey(nSessionKey);
	m_tSecuUtilRecv.SetSessionKey(nSessionKey);
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::EditNotifyMsgValue(SOCK_NOTIFY_MSG notify_msg_value)
{
	memcpy(&m_NotifyMsg, &notify_msg_value, sizeof(m_NotifyMsg));
	return 0;
}

//---------------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::SetConnectTryFlag(INT32 nFlag)
{
	m_nConnectTryPause = nFlag;
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------

INT32 CTCP_Client::GetLocalAddress(UINT32& nIPAddress)
{
	SOCKADDR_IN	sa;
	socklen_t nSALen = sizeof(sa);

	if (! m_bConnected || m_nSock == -1)	
		return -1;
	getsockname(m_nSock, (SOCKADDR *) &sa, &nSALen);
	nIPAddress = sa.sin_addr.s_addr;

	return 0;
}

INT32 CTCP_Client::FreeMemory(PVOID pBuff)
{
	safe_free(pBuff);
	return 0;
}

//---------------------------------------------------------------------------------------------------------------------
