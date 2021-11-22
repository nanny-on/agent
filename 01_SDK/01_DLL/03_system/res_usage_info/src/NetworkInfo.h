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

// NetWorkInfo.h: interface for the CNetWorkInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NETWORK_INFO_H__
#define _NETWORK_INFO_H__


#define CONNECTION_TCP "tcp"
#define CONNECTION_TCP6 "tcp6"
#define CONNECTION_UDP "udp"
#define CONNECTION_UDP6 "udp6"

#define TCP_SOURCE_PATH "/proc/net/tcp"
#define TCP6_SOURCE_PATH "/proc/net/tcp6"
#define UDP_SOURCE_PATH "/proc/net/udp"
#define UDP6_SOURCE_PATH "/proc/net/udp6"

#define TOKEN_MAX 1024

#define MAX_BUFF_SIZE 20480

typedef struct _net_conn_info {
	int nLocalPort;
	int nForeignPort;
	int nState;
	int nUserId;
    int nInode;
    int nReserved;
    char acType[MAX_TYPE_LEN];
    char acLocalAddress[MAX_NETADDR_LEN];
    char acForeignAddress[MAX_NETADDR_LEN];
} NET_CONN_INFO, *PNET_CONN_INFO;

typedef struct _list_cell {
    char acValue[TOKEN_MAX];
}LISTCELL, *PLISTCELL;


class CNetWorkInfo  
{
public:
	CNetWorkInfo();
	virtual ~CNetWorkInfo();

private:
	bool AddressToIP4(char *pAddress16, char *pAddress, int nMaxLen);
	bool AddressToIp6(char *pAddress16, char *pAddress, int nMaxLen);
	bool SplitConnetion(char *pString, const char *pToken, PLISTCELL *ppList, int *pnListCount);
	bool TranslateNetworkAddress(char *pAddress16, const char *pType, char *pAddress, int nMaxLen, int *pnPort);
	bool GetNetConnInfo(char *pLocal, char *pForeign, char *pState, char *pUserId, char *pInode, const char *pType, PNET_CONN_INFO pNetConnInfo);
	bool ParseRecordToConnection(char *pRecord, const char *pType, PNET_CONN_INFO pNetConnInfo);
	bool FindConnections(const char *pcType, PNET_CONN_INFO *ppNetInfo, int *pnNetCount);	

public :
	void Get_TCPUDP_Info(TListResInfoNetStat & tListNetStat);
};

#endif // _NETWORK_INFO_H__