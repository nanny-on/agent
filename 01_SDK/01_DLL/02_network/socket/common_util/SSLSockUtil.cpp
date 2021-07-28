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

// SSLSockUtil.cpp: implementation of the CSSLSockUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "SSLSockUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef void (*SSL_HandShakeCallBackType)(const SSL*, int, int);

CSSLSockUtil::CSSLSockUtil()
{
	m_meth	= NULL;
	m_ctx	= NULL;
	m_nMutexUsedFlag = SSL_MUTEX_USED_FLAG_SITE;
	pthread_mutex_init(&m_send_mutex, NULL);
	pthread_mutex_init(&m_recv_mutex, NULL);	
}

CSSLSockUtil::~CSSLSockUtil()
{
	Free_SSL();
	pthread_mutex_destroy(&m_send_mutex);
	pthread_mutex_destroy(&m_recv_mutex);	
}

INT32 CSSLSockUtil::Init_SSL(INT32 nIsSvr)
{
	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();
	
	if(nIsSvr)
		m_meth = SSLv23_server_method();
	else
		m_meth = SSLv23_client_method();
	
	m_ctx = SSL_CTX_new (m_meth);
	if (!m_ctx)
	{
		return -1;
	}
	return 0;
}

INT32 CSSLSockUtil::Init_PEM(LPCTSTR lpCertPath, LPCTSTR lpKeyPath, INT32& nErrRtn)
{
 	if ((nErrRtn = SSL_CTX_use_certificate_file(m_ctx, lpCertPath, SSL_FILETYPE_PEM)) <= 0)
	{
 		return -3;
 	}

	if ((nErrRtn = SSL_CTX_use_PrivateKey_file(m_ctx, lpKeyPath, SSL_FILETYPE_PEM)) <= 0)
	{
		return -4;
	}
	
	if ((nErrRtn = SSL_CTX_check_private_key(m_ctx)) == 0)
	{
		return -5;
	}
	return 0;
}

INT32			CSSLSockUtil::Set_HandShakeCallBack(PVOID lpCallBack)
{
	SSL_CTX_set_info_callback(m_ctx,(SSL_HandShakeCallBackType)lpCallBack);
	return 0;
}

INT32			CSSLSockUtil::Free_SSL()
{	
	if(m_ctx)
	{
		SSL_CTX_free (m_ctx);
		m_ctx = NULL;
	}
	return 0;
}

SSL*     		CSSLSockUtil::GetSSL_Accept(SOCKET s, INT32& nErrRtn)
{
	SSL* ssl = NULL;
	INT32 nTryIndex = SSL_ACCEPT_TRY_MAX_NUM;
	
	if( (ssl = SSL_new(m_ctx)) == NULL)
	{
		nErrRtn = -1;
		return NULL;
	}
	
	SSL_set_cipher_list(ssl, "AES256-SHA256");

	SSL_set_fd (ssl, s);

	while( nTryIndex && (nErrRtn = SSL_accept (ssl)) < 0)
	{
		usleep(10000);
		nTryIndex--;
	}
	if(nTryIndex == 0)
	{		
		SSL_free(ssl);
		return NULL;
	}

	nErrRtn = SSL_ACCEPT_TRY_MAX_NUM - nTryIndex;
	return ssl;
}

SSL*     		CSSLSockUtil::GetSSL_Connect(SOCKET s, INT32& nErrRtn)
{
	SSL* ssl = NULL;
	
	if( (ssl = SSL_new(m_ctx)) == NULL)
	{
		nErrRtn = -1;
		return NULL;
	}

	SSL_set_cipher_list(ssl, "AES256-SHA256");
	SSL_set_fd (ssl, s);
	if(SSL_connect (ssl) == -1)
	{
		nErrRtn = -2;
		SSL_free(ssl);
		return NULL;
	}	
	return ssl;
}

SSL*     		CSSLSockUtil::GetSSL_Connect_SMS(SOCKET s, INT32& nErrRtn)
{
	SSL* ssl = NULL;
	
	if( (ssl = SSL_new(m_ctx)) == NULL)
	{
		nErrRtn = -1;
		return NULL;
	}

	SSL_set_fd (ssl, s);
	if(SSL_connect (ssl) < 0)
	{
		nErrRtn = -2;
		SSL_free(ssl);
		return NULL;
	}	
	return ssl;
}


void			CSSLSockUtil::Free_SSL(SSL* ssl)
{
	if(ssl)
	{
		SSL_free(ssl);	
	}
}

void			CSSLSockUtil::ShutDown_SSL(SSL* ssl)
{
	if(ssl)
	{
		SSL_set_quiet_shutdown(ssl, SSL_RECEIVED_SHUTDOWN);
		SSL_set_quiet_shutdown(ssl, SSL_SENT_SHUTDOWN);
	}
}


INT32			CSSLSockUtil::GetShutDown_Flag(SSL* ssl)
{
	INT32 nRtn = 0;
	if(ssl)
	{
		nRtn = SSL_get_shutdown(ssl);
	}
	return nRtn;
}

void			CSSLSockUtil::SetShutDown_Flag(SSL* ssl, INT32 nMode)
{
	SSL_set_shutdown(ssl, nMode);
}

INT32			CSSLSockUtil::SSL_SendBuf(SSL* ssl, PVOID lpBuf, INT32 nLen)
{
	INT32 nRtn = 0;
	pthread_mutex_lock(&m_send_mutex);
	nRtn = SSL_write(ssl, lpBuf, nLen);
	pthread_mutex_unlock(&m_send_mutex);
	return nRtn;
}

INT32			CSSLSockUtil::SSL_RecvBuf(SSL* ssl, PVOID lpBuf, INT32 nLen)
{	
	INT32 nRtn = 0;
	pthread_mutex_lock(&m_recv_mutex);
	nRtn = SSL_read(ssl, lpBuf, nLen);
	pthread_mutex_unlock(&m_recv_mutex);
	return nRtn;
}



