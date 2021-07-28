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

#ifndef	ASI_DLL_DEFINE_SOCKET_H_91287755_D7A6_4F36_991E_AE92458D87CA
#define	ASI_DLL_DEFINE_SOCKET_H_91287755_D7A6_4F36_991E_AE92458D87CA

//***************************************************************************************//
#ifndef DEFINE_PKT_HEADER
#define	DEFINE_PKT_HEADER


typedef struct _pkt_data_hdr_first
{
	_pkt_data_hdr_first()
	{
		length = 0;
		memset(hash, 0, 16);
	}
	UINT32		length;
	BYTE		hash[16];
}PKT_DATA_HDR_FIRST, *PPKT_DATA_HDR_FIRST;

#define			PKT_DATA_HDR_FIRST_SIZE	sizeof(PKT_DATA_HDR_FIRST)
//-----------------------------------------------------------------------------------------------

typedef struct _pkt_data_hdr_second
{	
	_pkt_data_hdr_second()
	{
		type = 0;
		code = 0;
	}
    UINT16		type ;
    UINT16		code ;
} PKT_DATA_HDR_SECOND, *PPKT_DATA_HDR_SECOND ;

#define			PKT_DATA_HDR_SECOND_SIZE	sizeof(PKT_DATA_HDR_SECOND)
//-----------------------------------------------------------------------------------------------

typedef struct _pkt_data_hdr_all
{
	_pkt_data_hdr_all()
	{
		type = 0;
		code = 0;
		length = 0;
		memset(hash, 0, 16);
	}
	UINT32		length;
	BYTE		hash[16];
    UINT16		type ;
    UINT16		code ;
}PKT_DATA_HDR_ALL, *PPKT_DATA_HDR_ALL;

#define			PKT_DATA_HDR_ALL_SIZE		sizeof(PKT_DATA_HDR_ALL)


typedef struct _pkt_data_hdr
{
	UINT32		length;
	UINT16		type;
	UINT16		code;
}PKT_DATA_HDR, *PPKT_DATA_HDR;

#define			PKT_DATA_HDR_SIZE				sizeof(PKT_DATA_HDR)
//-----------------------------------------------------------------------------------------------

typedef struct _pkt_data_hdr_sha256
{
	_pkt_data_hdr_sha256()
	{
		type = 0;
		code = 0;
		length = 0;
		memset(hash, 0, 32);
	}
	UINT32		length;
	BYTE		hash[32];
    UINT16		type ;
    UINT16		code ;
}PKT_DATA_HDR_SHA256, *PPKT_DATA_HDR_SHA256;

#define			PKT_DATA_HDR_SHA256_SIZE		sizeof(PKT_DATA_HDR_SHA256)
//-----------------------------------------------------------------------------------------------

typedef struct _pkt_data_hdr_ssl
{
	_pkt_data_hdr_ssl()
	{
		type = 0;
		code = 0;
		length = 0;
		memset(hash, 0, 16);
	}
	UINT32		length;
	BYTE		hash[16];
    UINT16		type ;
    UINT16		code ;
}PKT_DATA_HDR_SSL, *PPKT_DATA_HDR_SSL;

#define			PKT_DATA_HDR_SSL_SIZE		sizeof(PKT_DATA_HDR_SSL)
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

typedef struct _pkt_data_body
{
	_pkt_data_body()
	{
		data = NULL;
	}
	BYTE		*data;
}PKT_DATA_BODY, *PPKT_DATA_BODY;
//-----------------------------------------------------------------------------------------------

typedef struct _sock_evt_prop
{    
	_sock_evt_prop()
	{
		nErrorCode		= 0;
		nSessionID		= 0;
		nAddress		= 0;
	}
	INT32               nSessionID ;
    UINT32              nAddress ;
    INT32               nEventID ;
	INT32				nErrorCode ;
}SOCK_EVT_PROP, *PSOCK_EVT_PROP ;
//-----------------------------------------------------------------------------------------------

typedef struct _pkt_data
{
	_pkt_data()
	{
		nSessionID = 0;
		nIsSockEvent = 0;
	}
	_pkt_data(UINT16 nType, UINT16 nCode)
	{
		nSessionID = 0;
        hdr.type = nType;
        hdr.code = nCode;
	}
	INT32			nSessionID;
	INT32			nIsSockEvent;
	PKT_DATA_HDR	hdr;
	PKT_DATA_BODY	body;
	SOCK_EVT_PROP	sock_evt;
}PKT_DATA, *PPKT_DATA;
#endif
//-----------------------------------------------------------------------------------------------

typedef struct _sock_notify_msg
{
	_sock_notify_msg()
	{
		m_hNotifyOwner		= NULL;
		m_nRecvMsg			= WM_USER + 1000;	 
		m_nDisconMsg		= WM_USER + 1001;
		m_nConnectMsg		= WM_USER + 1002;
		m_nRecvStatusMsg	= WM_USER + 1003;
		m_nSendStatusMsg	= WM_USER + 1004;
	}
	PVOID			m_hNotifyOwner;
	UINT32			m_nRecvMsg;
	UINT32			m_nDisconMsg;
	UINT32			m_nConnectMsg;
	UINT32			m_nRecvStatusMsg;
	UINT32			m_nSendStatusMsg;
}SOCK_NOTIFY_MSG, *PSOCK_NOTIFY_MSG;


//-----------------------------------------------------------------------------------------------

#define		ASI_PKT_ENC_TYPE_SEED_SHA2					1
#define		ASI_PKT_ENC_TYPE_SSL						10
//-----------------------------------------------------------------------------------------------

#define		ASI_SVR_RUN_TYPE_TCP_EVENT					0
#define		ASI_SVR_RUN_TYPE_TCP_IOCP					1
#define		ASI_SVR_RUN_TYPE_TCP_SNMP					2
#define		ASI_SVR_RUN_TYPE_UDP_SNMP					3

#define		ASI_CLT_RUN_TYPE_TCP_EVENT					0
//-----------------------------------------------------------------------------------------------

#define		ASI_SVR_TCP_IOCP_OP_TYPE_DEFAULT			0
#define		ASI_SVR_TCP_IOCP_OP_TYPE_EXTENSION			1

#define		ASI_CLT_TCP_EVENT_OP_TYPE_DEFAULT			0
#define		ASI_CLT_TCP_EVENT_OP_TYPE_AUTO_CONNECT		1
//-----------------------------------------------------------------------------------------------

#define		ASI_SVR_PKT_TRANS_START_TYPE_DEMAND			0
#define		ASI_SVR_PKT_TRANS_START_TYPE_AUTO_START		1

//-----------------------------------------------------------------------------------------------

#define		ASI_PKT_POOL_FLAG_NONE						0x0000
#define		ASI_PKT_POOL_FLAG_RECV						0x0001
#define		ASI_PKT_POOL_FLAG_SEND						0x0002
//-----------------------------------------------------------------------------------------------

#define		DEFAULT_PKT_PAGING_SIZE						4096//1024////4096//20480//8192//

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
#define		ASI_SOCKET_EVENT_CONNECT					1
#define		ASI_SOCKET_EVENT_DISCONNECT					0

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

typedef struct _sock_init_data
{
	_sock_init_data()
	{
		ZeroMemory(szOpenAddress, MAX_PATH);
		nOpenPort				= 0;
		
		nSessionKey				= 0;
		nPktHdrSize				= 0;
		nPktEncType				= 0;
		nPktPagingSize			= DEFAULT_PKT_PAGING_SIZE;
		nPktTransMode			= ASI_SVR_PKT_TRANS_START_TYPE_AUTO_START;
		nPktPoolFlag			= ASI_PKT_POOL_FLAG_RECV | ASI_PKT_POOL_FLAG_SEND;		//작은연결 많은 데이트 통신..^^
		{
			nRunTypeSvr			= ASI_SVR_RUN_TYPE_TCP_EVENT;
			nRunTypeClt			= ASI_CLT_RUN_TYPE_TCP_EVENT;
		}
		{
			nOpTypeSvr			= ASI_SVR_TCP_IOCP_OP_TYPE_DEFAULT;
			nOpTypeClt			= ASI_CLT_TCP_EVENT_OP_TYPE_DEFAULT;
		}
		nExtSockFlag			= 0;
		nOnKeepAlive			= 0;
		nMaxComPortNum			= 32;		//MAX_COMPLETION_THREAD_COUNT
		nRemainSockEvt			= 1;
		nRemainDebugLog			= 1;
		nChkAutoDisConRunTime	= 0;
		nChkAutoDisConTime		= 0;

		ZeroMemory(szLogFileName, MAX_PATH);
		ZeroMemory(szLogFilePath, MAX_PATH);
		ZeroMemory(szPemFileCertPath, MAX_PATH);
		ZeroMemory(szPemFileKeyPath, MAX_PATH);
	}
	CHAR			szOpenAddress[MAX_PATH];		//  SERVER & CLIENT
	UINT32			nOpenPort;						//  SERVER & CLIENT

	UINT32			nSessionKey;			//	SERVER & CLIENT 
	UINT32			nPktHdrSize;			//	SERVER & CLIENT	
	UINT32			nPktEncType;			//	SERVER & CLIENT
	INT32			nPktPagingSize;			//	SERVER & CLIENT
	UINT32			nPktTransMode;			//	SERVER
	UINT32			nPktPoolFlag;			//  SERVER & CLIENT, 0 (Used Recv : Default), 0x01(Used Send), 0x02 (Not Used Recv)
	union
	{
		UINT32		nRunTypeSvr;			//  SERVER & CLIENT
		UINT32		nRunTypeClt;			//  SERVER & CLIENT
	};
	union
	{
		UINT32		nOpTypeSvr;				//  SERVER & CLIENT
		UINT32		nOpTypeClt;				//  SERVER & CLIENT
	};
	UINT32			nExtSockFlag;			//  SERVER & CLIENT
	UINT32			nOnKeepAlive;			//  SERVER & CLIENT
	UINT32			nMaxComPortNum;			//  SERVER
	UINT32			nRemainSockEvt;			//  SERVER & CLIENT
	UINT32			nRemainDebugLog;		//  SERVER & CLIENT
	UINT32			m_nFileLogRetention;	//  SERVER & CLIENT
	UINT32			nChkAutoDisConRunTime;	// SERVER
	UINT32			nChkAutoDisConTime;		// SERVER

	CHAR			szLogFileName[MAX_PATH];		//  SERVER & CLIENT
	CHAR			szLogFilePath[MAX_PATH];		//  SERVER & CLIENT
	CHAR			szPemFileCertPath[MAX_PATH];	//  SERVER
	CHAR			szPemFileKeyPath[MAX_PATH];		//  SERVER
}SOCK_INIT_DATA, *PSOCK_INIT_DATA;
//-----------------------------------------------------------------------------------------------


#define     DEFAULT_WAIT_THREAD_NUM         32
#define		DEFAULT_ONCE_READ_FILE_SIZE		4*1024 // 4Kbyte
#define		MAKE_BUFFER_MAX_SIZE			1024 * 1024 * 1024
#define		ONCE_PACKET_MAX_SIZE			MAXWORD

typedef		PVOID						ASI_SVR_SOCKET;
typedef		PVOID						ASI_CLT_SOCKET;

#define		DL_RECV_START				0x00000001
#define		DL_RECV_EXCEPTION			0x00000002
#define		DL_RECV_DEBUG_DETAIL		0x00000004
#define		DL_RECV_DEBUG_STEP			0x00000008
#define		DL_SEND_DEBUG_DETAIL		0x00000010
#define		DL_SEND_DEBUG_OP			0x00000020

#define		SERVER_SOCKET_TYPE_EVENT			0

//-----------------------------------------------------------------------------------------------

#define		ASI_SVRSOCK_DLL_NAME		"asi_svrsock.so"
#define		ASI_CLTSOCK_DLL_NAME		"asi_cltsock.so"

#define		ASI_SVRSOCK_DLL_VERSION		"1.0.0.1"
#define		ASI_CLTSOCK_DLL_VERSION		"1.0.0.1"

#define		ANY_OPEN_ADDRESS			"0.0.0.0"
//-----------------------------------------------------------------------------------------------

INT32	StartClient(ASI_SVR_SOCKET* lpObject, PSOCK_INIT_DATA pSID, PSOCK_NOTIFY_MSG pSNM);


typedef INT32 (*StartServer_EVTType)(ASI_SVR_SOCKET* lpObject, PSOCK_INIT_DATA pSID, UINT32 dwSIDSize, PSOCK_NOTIFY_MSG pSNM, UINT32 dwSNMSize);

typedef INT32 (*StopServer_EVTType)(ASI_SVR_SOCKET lpObject, INT32 nDeleteRemainPkt);

typedef INT32 (*FreeServer_EVTType)(ASI_SVR_SOCKET lpObject);

typedef INT32 (*IsRunSvr_EVTType)(ASI_SVR_SOCKET lpObject);

typedef INT32 (*SendData_EVTType)(ASI_SVR_SOCKET lpObject, INT32 sessionid, UINT16 type, UINT16 code, UINT32 len, PVOID data);

typedef INT32 (*StartRecv_EVTType)(ASI_SVR_SOCKET lpObject, INT32 nSessionID);

typedef INT32 (*GetSocketAddressUINT_EVTType)(ASI_SVR_SOCKET lpObject, INT32 nSessionID, UINT32& nAddress);

typedef INT32 (*GetSocketAddress_EVTType)(ASI_SVR_SOCKET lpObject, INT32 nSessionID, LPTSTR lpAddress);

typedef INT32 (*EditSessionKey_EVTType)(ASI_SVR_SOCKET lpObject, INT32 nSessionID, UINT32 nSessionKey);

typedef INT32 (*EditSessionID_EVTType)(ASI_SVR_SOCKET lpObject, INT32 nOldID, INT32 nNewID);

typedef INT32 (*IsExistsSessionID_EVTType)(ASI_SVR_SOCKET lpObject, INT32 nSessionID);

typedef INT32 (*GetPkt_EVTType)(ASI_SVR_SOCKET  lpObject, PKT_DATA& pkt_data);

typedef INT32 (*IsExistsPkt_EVTType)(ASI_SVR_SOCKET  lpObject);

typedef INT32 (*GetConnectInfo_EVTType)(ASI_SVR_SOCKET lpObject, INT32& nCurrentConnectNum, INT32& nTotalConnectNum);

typedef INT32 (*FreeMemory_EVTType)(ASI_SVR_SOCKET lpObject, void* lpvoid);

typedef INT32 (*CloseSocket_EVTType)(ASI_SVR_SOCKET lpObject, INT32 nSessionID, INT32 nRemainEvt);	

typedef INT32 (*EditLogFileName_EVTType)(ASI_SVR_SOCKET  lpObject, LPCTSTR lpLogFilePath, LPCTSTR lpLogFileName);

//-----------------------------------------------------------------------------------------------
typedef INT32 (*NewClient_CLDType)(PVOID *ppObject);
typedef INT32 (*FreeClient_CLDType)(PVOID pObject);
typedef INT32 (*StartClient_CLDType)(PVOID pObject, PSOCK_INIT_DATA pSID, UINT32 dwSIDSize, PSOCK_NOTIFY_MSG pSNM, UINT32 dwSNMSize, char *pClassName);
typedef INT32 (*Send_CLDType)(PVOID pObject, UINT16 nType, UINT16 nCode, UINT32 length , PVOID data);
typedef INT32 (*CloseSocket_CLDType)(PVOID pObject, INT32 nFlag);
typedef INT32 (*SetConnectTryFlag_CLDType)(PVOID pObject, INT32 nFlag);
typedef INT32 (*EditSessionKey_CLDType)(PVOID pObject, UINT32 nSessionKey);
typedef INT32 (*StopClient_CLDType)(PVOID pObject);
typedef INT32 (*IsExistPkt_CLDType)(PVOID pObject);
typedef INT32 (*GetPkt_CLDType)(PVOID pObject, PKT_DATA& pkt_data);
typedef INT32 (*LockPktMutex_CLDType)(PVOID  pObject);
typedef INT32 (*UnLockPktMutex_CLDType)(PVOID  pObject);
typedef INT32 (*GetPktUnLock_CLDType)(PVOID pObject, PKT_DATA& pkt_data);
typedef INT32 (*EditNotifyMsgValueType)(PVOID pObject, SOCK_NOTIFY_MSG notify_msg_value);
typedef INT32 (*GetLocalAddress_CLDType)(PVOID pObject, UINT32& nIPAddress);
typedef INT32 (*FreeMemory_CLDType)(PVOID pObject, void* pVoid);


#endif
