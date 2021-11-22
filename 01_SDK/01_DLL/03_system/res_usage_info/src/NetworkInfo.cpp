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

// NetWorkInfo.cpp: implementation of the CNetWorkInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "NetworkInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetWorkInfo::CNetWorkInfo()
{
}

CNetWorkInfo::~CNetWorkInfo()
{
}

bool CNetWorkInfo::AddressToIP4(char *pAddress16, char *pAddress, int nMaxLen)
{
	char *pStr = NULL;
	char acSub[16] = {0,};
    struct sockaddr_in sa;
	if(pAddress16 == NULL || pAddress == NULL || nMaxLen < 1)
		return false;
	memcpy(acSub, pAddress16, 8);
	sa.sin_addr.s_addr = HexStringToInt(acSub);
	pStr = inet_ntoa(sa.sin_addr);
	if(pStr != NULL)
		strncpy(pAddress, pStr, nMaxLen-1);
    return true;
}

bool CNetWorkInfo::AddressToIp6(char *pAddress16, char *pAddress, int nMaxLen)
{
	int i = 0;
	char acTmp[64] = {0,};
	char acTmp2[4] = {0,};
    struct sockaddr_in6 sa;
    struct in6_addr ip;
	
	if(pAddress16 == NULL || pAddress == NULL || nMaxLen < 1)
		return false;

    for (i=0; i<4; i++)
	{
        acTmp[0+8*i] = pAddress16[6+8*i];
        acTmp[1+8*i] = pAddress16[7+8*i];
        acTmp[2+8*i] = pAddress16[4+8*i];
        acTmp[3+8*i] = pAddress16[5+8*i];
        acTmp[4+8*i] = pAddress16[2+8*i];
        acTmp[5+8*i] = pAddress16[3+8*i];
        acTmp[6+8*i] = pAddress16[0+8*i];
        acTmp[7+8*i] = pAddress16[1+8*i];
    }
    for(i=0; i<16; i++)
	{
		memcpy(acTmp2, &acTmp[i*2], 2);
        ip.s6_addr[i] = HexStringToInt(acTmp2); 
    }
    sa.sin6_family = AF_INET6; 
    sa.sin6_addr = ip;
    inet_ntop(AF_INET6, &(sa.sin6_addr), pAddress, INET6_ADDRSTRLEN);
    return true;
}

bool CNetWorkInfo::SplitConnetion(char *pString, const char *pToken, PLISTCELL *ppList, int *pnListCount)
{
    PLISTCELL pList = NULL;
	char *pBuffer = NULL;
	char *pData = NULL;	
	int nLength = 0;
	int i, nCount = 0;

	if(pString == NULL || pToken == NULL || ppList == NULL || pnListCount == NULL)
		return false;

    nLength = strlen(pString);
	if(nLength < 1)
		return false;
	
	pBuffer = (char *)malloc(nLength+1);
	if(pBuffer == NULL)
		return false;
    memset(pBuffer, 0, nLength+1);

	nCount = 0;
	for(i=0; i<nLength; i++)
	{
		if(pString[i] == pToken[0])
		{
			if(pString[i+1] != pToken[0] && nCount != 0)
			{
				pBuffer[nCount++] = pString[i];
			}
		}
		else
		{
			pBuffer[nCount++] = pString[i];
		}
	}

	nCount = count_token(pBuffer, (int)pToken[0]);
	if(nCount < 1)
	{
		safe_free(pBuffer);
		return false;
	}
	pList = (PLISTCELL)malloc(sizeof(LISTCELL)*nCount);
	if(pList == NULL)
	{
		free(pBuffer);
		return false;
	}
	memset(pList, 0, sizeof(LISTCELL)*nCount);

	pData = pBuffer;
	for(i=0; i<nCount; i++)
	{
		pData = copy_token(pList[i].acValue, TOKEN_MAX, pData, (int)pToken[0]);
		if(pData == NULL)
			break;
	}
	safe_free(pBuffer);
	*pnListCount = nCount;
	*ppList = pList;
	
    return true;
}

bool CNetWorkInfo::TranslateNetworkAddress(char *pAddress16, const char *pType, char *pAddress, int nMaxLen, int *pnPort)
{
	char acAddr[MAX_NETADDR_LEN] = {0,};
	char acBin[8] = {0,};
	int nPort = 0;
	if(pAddress16 == NULL || pType == NULL || pAddress == NULL || nMaxLen < 1 || pnPort == NULL)
		return false;

    if (!strcmp(pType, CONNECTION_TCP) || !strcmp(pType, CONNECTION_UDP))
	{
		if(AddressToIP4(pAddress16, acAddr, MAX_NETADDR_LEN) == false)
			return false;
		strncpy(pAddress, acAddr, nMaxLen-1);
		memcpy(acBin, &pAddress16[9], 4);
		nPort = HexStringToInt(acBin);
		*pnPort = nPort;
        return true;
    }
	else if (!strcmp(pType, CONNECTION_TCP6) || !strcmp(pType, CONNECTION_UDP6))
	{
		if(AddressToIp6(pAddress16, acAddr, MAX_NETADDR_LEN) == false)
			return false;
		strncpy(pAddress, acAddr, nMaxLen-1);
		memcpy(acBin, &pAddress16[33], 4);
		nPort = HexStringToInt(acBin);
		*pnPort = nPort;
        return true;
    }
	return false;
}


bool CNetWorkInfo::GetNetConnInfo(char *pLocal, char *pForeign, char *pState, char *pUserId, char *pInode, const char *pType, PNET_CONN_INFO pNetConnInfo)
{
	int nLocalPort = 0;
	int nForeignPort = 0;
	char acLocalAddress[MAX_NETADDR_LEN] = {0,};
	char acForeignAddress[MAX_NETADDR_LEN] = {0,};

	if(pNetConnInfo == NULL)
		return false;

	memset(pNetConnInfo, 0, sizeof(NET_CONN_INFO));

    if(TranslateNetworkAddress(pLocal, pType, acLocalAddress, MAX_NETADDR_LEN, &nLocalPort) == true)
    {
		strncpy(pNetConnInfo->acLocalAddress, acLocalAddress, MAX_NETADDR_LEN-1);
		pNetConnInfo->nLocalPort = nLocalPort;
    }
    if(TranslateNetworkAddress(pForeign, pType, acForeignAddress, MAX_NETADDR_LEN, &nForeignPort) == true)
    {
		strncpy(pNetConnInfo->acForeignAddress, acForeignAddress, MAX_NETADDR_LEN-1);
		pNetConnInfo->nForeignPort = nForeignPort;
    }

	if(pState != NULL)
	{
		pNetConnInfo->nState = HexStringToInt(pState);
	}
    
	if(pUserId != NULL)
	{
		pNetConnInfo->nUserId = StringToInt(pUserId);
	}
    
	if(pInode != NULL)
	{
		pNetConnInfo->nInode = StringToInt(pInode);
	}
    
	if(pType != NULL)
		strncpy(pNetConnInfo->acType, pType, MAX_TYPE_LEN-1);

    return true;
}

bool CNetWorkInfo::ParseRecordToConnection(char *pRecord, const char *pType, PNET_CONN_INFO pNetConnInfo)
{
	PLISTCELL pList = NULL;
	bool bRetVal = true;
	int nListCount = 0;
	
	if(pRecord == NULL || pNetConnInfo == NULL)
		return false;
	do{
		bRetVal = SplitConnetion(pRecord, " ", &pList, &nListCount);
		if(bRetVal == false || pList == NULL || nListCount < 10)
		{
			return false;
		}
	    bRetVal = GetNetConnInfo(pList[1].acValue, pList[2].acValue, pList[3].acValue, pList[7].acValue, pList[9].acValue, pType, pNetConnInfo);
	}while(false);
	safe_free(pList);
	return bRetVal;
}


bool CNetWorkInfo::FindConnections(const char *pcType, PNET_CONN_INFO *ppNetInfo, int *pnNetCount)
{
	PNET_CONN_INFO pConnections = NULL;
	PLISTCELL pList = NULL;
	int i, nCount = 0;
	int nNetCount = 0;
	FILE *fp = NULL;
    char acSourcePath[MAX_PATH] = {0,};
    char *pcBuffer = NULL;
	int nBufSize = MAX_BUFF_SIZE;
	bool bRetVal = true;	
	if(pcType == NULL || ppNetInfo == NULL || pnNetCount == NULL)
		return false;

    if (!_stricmp(pcType, CONNECTION_TCP))
	{
        strncpy(acSourcePath, TCP_SOURCE_PATH, MAX_PATH-1);
    }
	else if (!_stricmp(pcType, CONNECTION_TCP6))
	{
        strncpy(acSourcePath, TCP6_SOURCE_PATH, MAX_PATH-1);
    }
	else if (!_stricmp(pcType, CONNECTION_UDP))
	{
        strncpy(acSourcePath, UDP_SOURCE_PATH, MAX_PATH-1);
    }
	else if (_stricmp(pcType, CONNECTION_UDP6))
	{
        strncpy(acSourcePath, UDP6_SOURCE_PATH, MAX_PATH-1);
    }
	else
		return false;

	do{
		pcBuffer = (char *)malloc(nBufSize);
		if(pcBuffer == NULL)
		{
			bRetVal = false;
			break;
		}
	    memset(pcBuffer, 0, nBufSize);
		
		fp = fopen(acSourcePath, "r");
		if(fp == NULL)
		{
			bRetVal = false;
			break;
		}
	    fread(pcBuffer, nBufSize-1, 1, fp);
		fclose(fp);

		bRetVal = SplitConnetion(pcBuffer, "\n", &pList, &nCount);
		if(bRetVal == false)
		{
			bRetVal = false;
			break;
		}

		if(pList == NULL || nCount < 1)
		{
			bRetVal = false;
			break;
		}

		pConnections = (PNET_CONN_INFO)malloc((nCount-1)*sizeof(NET_CONN_INFO));
		if(pConnections == NULL)
		{
			bRetVal = false;
			break;
		}
		memset(pConnections, 0, (nCount-1)*sizeof(NET_CONN_INFO));

		nNetCount = 0;
		for(i=1; i<nCount; i++)
		{
			if(ParseRecordToConnection(pList[i].acValue, pcType, &pConnections[nNetCount]) == true)
			{
				nNetCount++;
			}
		}
		if(nNetCount == 0)
		{
			safe_free(pConnections);
			bRetVal = false;
			break;
		}
		*ppNetInfo = pConnections;
		*pnNetCount = nNetCount;
		bRetVal = true;
	}while(false);

	safe_free(pcBuffer);
	safe_free(pList);
    return bRetVal;
}


void CNetWorkInfo::Get_TCPUDP_Info(TListResInfoNetStat & tListNetStat)
{
	bool bRetVal = false;
	INT32 i = 0;
	PNET_CONN_INFO pNetInfo = NULL;
	int nNetCount = 0;
	ASIRES_INFO_NET_STAT tns;

    bRetVal = FindConnections("tcp", &pNetInfo, &nNetCount);
	if(bRetVal == true && pNetInfo != NULL && nNetCount > 0)
	{
		for (i = 0; i < (int)nNetCount; i++)
		{
			memset(&tns, 0, sizeof(ASIRES_INFO_NET_STAT));
			
			tns.nProtocol = RESDLL_TCP;				
			
			tns.nLocalPort = pNetInfo[i].nLocalPort;
			tns.nRemotePort = pNetInfo[i].nForeignPort;
			
			strncpy(tns.szLocalAddr, pNetInfo[i].acLocalAddress, MAX_NETADDR_LEN-1);
			strncpy(tns.szRemoteAddr, pNetInfo[i].acForeignAddress, MAX_NETADDR_LEN-1);			
					
			tns.nStatus = pNetInfo[i].nState;		
			
			tListNetStat.push_back(tns);	
		}
	}	
	safe_free(pNetInfo);
	nNetCount = 0;

    bRetVal = FindConnections("udp", &pNetInfo, &nNetCount);
	if(bRetVal == true && pNetInfo != NULL && nNetCount > 0)
	{
		for (i = 0; i < (int)nNetCount; i++)
		{
			memset(&tns, NULL, sizeof(ASIRES_INFO_NET_STAT));

			tns.nProtocol = RESDLL_UDP;
			tns.nLocalPort = pNetInfo[i].nLocalPort;
			tns.nRemotePort = pNetInfo[i].nForeignPort;
			strncpy(tns.szLocalAddr, pNetInfo[i].acLocalAddress, MAX_NETADDR_LEN-1);
			strncpy(tns.szRemoteAddr, pNetInfo[i].acForeignAddress, MAX_NETADDR_LEN-1);			
			tns.nStatus = -1;
			tListNetStat.push_back(tns);		
		}	
	}
	safe_free(pNetInfo);
	nNetCount = 0;	
}

