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

// SSLSockUtil.h: interface for the CSSLSockUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSLSOCKUTIL_H__B1ADBD0E_5803_41F0_B70E_BF6D3CDC6DB8__INCLUDED_)
#define AFX_SSLSOCKUTIL_H__B1ADBD0E_5803_41F0_B70E_BF6D3CDC6DB8__INCLUDED_


#include <openssl/rsa.h>       /* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "MutexExt.h"

//------------------------------------------------------------------------------------------
#define		SSL_PACKET_BUFFER_MAX_SIZE			10240
#define		SSL_ACCEPT_TRY_MAX_NUM				200
#define		SSL_MUTEX_USED_FLAG_SITE			0
#define		SSL_MUTEX_USED_FLAG_GLOBAL			1
//------------------------------------------------------------------------------------------

class CSSLSockUtil  
{
private:
	const SSL_METHOD* m_meth;
	SSL_CTX*		m_ctx;
	INT32			m_nMutexUsedFlag;
	pthread_mutex_t m_send_mutex;
	pthread_mutex_t m_recv_mutex;

public:
	INT32			Init_SSL(INT32 nIsSvr = 1);
	INT32			Init_PEM(LPCTSTR lpCertPath, LPCTSTR lpKeyPath, INT32& nErrRtn);
	INT32			Set_HandShakeCallBack(PVOID lpCallBack);
	INT32			Free_SSL();
	void			SetMutexUsedFlag(INT32 nUsedFlag)	{	m_nMutexUsedFlag = nUsedFlag;	}

public:
	SSL*     		GetSSL_Accept(SOCKET s, INT32& nErrRtn);
	SSL*			GetSSL_Connect(SOCKET s, INT32& nErrRtn);
	SSL*			GetSSL_Connect_SMS(SOCKET s, INT32& nErrRtn);
	void			Free_SSL(SSL* ssl);
	void			ShutDown_SSL(SSL* ssl);
	INT32			GetShutDown_Flag(SSL* ssl);
	void			SetShutDown_Flag(SSL* ssl, INT32 nMode);

public:
	INT32			SSL_SendBuf(SSL* ssl, PVOID lpBuf, INT32 nLen);
	INT32			SSL_RecvBuf(SSL* ssl, PVOID lpBuf, INT32 nLen);
	
public:
	CSSLSockUtil();
	virtual ~CSSLSockUtil();

};

#endif // !defined(AFX_SSLSOCKUTIL_H__B1ADBD0E_5803_41F0_B70E_BF6D3CDC6DB8__INCLUDED_)
