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

#include "stdafx.h"
#include "com_struct.h"
#include "network.h"

class CNetInfo
{
public:
	CNetInfo(const char* pszInfName, const char* pszMac, const char* pszIP, const char* pszSubnet)
	{
		strncpy(szInfName, pszInfName, 15);
		strncpy(szMac, pszMac, 31);
		strncpy(szIP, pszIP,15);
		strncpy(szSubnet, pszSubnet,31);
	}
	~CNetInfo()
	{
	}
	char szInfName[16];
	char szMac[32];
	char szIP[16];
	char szSubnet[32];
};

typedef list<CNetInfo*> NetInfoList;
typedef NetInfoList::iterator netitor;

class CNetInfoList
{
public:
	CNetInfoList()
	{
	}
	~CNetInfoList()
	{
		ClearAll();
	}
	void ClearAll()
	{
        netitor begin, end;
		begin = m_NetList.begin(); end = m_NetList.end();
		for(;begin != end; begin++)
		{
			CNetInfo* pInfo = *begin;
			if(pInfo != NULL)
				delete pInfo;
	    }
		m_NetList.clear();
	}

	int AddNetInfo(const char* pszInfName, const char* pszMac, const char* pszIP, const char* pszSubnet)
	{
		if(strcmp(pszInfName, "lo") && strcmp(pszMac, "0.0.0.0.0.0") && FindNet(pszInfName, pszMac) == NULL)
		{
			CNetInfo* pInfo = new CNetInfo(pszInfName, pszMac, pszIP, pszSubnet);
			m_NetList.push_back(pInfo);
		}
		return m_NetList.size();
	}

	CNetInfo* FindNet(const char* pszInfName, const char* pszMac)
	{
        netitor begin, end;
		begin = m_NetList.begin(); end = m_NetList.end();
		for(;begin != end; begin++)
		{
			if(strcmp((*begin)->szInfName, pszInfName) == 0 && strcmp((*begin)->szMac, pszMac) == 0 )
			{
				return (*begin);
			}

		}
		return NULL;
	}
    CNetInfo* RemoveNet(const char* pszInfName, const char* pszMac)
	{
		netitor begin, end;
		begin = m_NetList.begin(); end = m_NetList.end();
		for(;begin != end; begin++)
		{
			if(strcmp((*begin)->szInfName, pszInfName) == 0 && strcmp((*begin)->szMac, pszMac) == 0 )
			{
				CNetInfo* pInfo = *begin;
				m_NetList.remove(pInfo);
				return pInfo;
			}
		}
		return NULL;
	}

	void PrintAll()
	{
        netitor begin, end;
        begin = m_NetList.begin(); end = m_NetList.end();
        for(;begin != end; begin++)
        {
            CNetInfo* pInfo = *begin;
			if(pInfo)
				printf("Interface: %s, Mac: %s, IP: %s\n", pInfo->szInfName, pInfo->szMac, pInfo->szIP);
        }
	}

	int GetCount()
	{
		return m_NetList.size();
	}

	CNetInfo* GetAt(int nIndex)
	{
        netitor begin, end;
		int n = 0;
        begin = m_NetList.begin(); end = m_NetList.end();
        for(;begin != end; begin++)
        {
	        CNetInfo* pInfo = *begin;
			if(n == nIndex)
				return pInfo;
			n++;
        }
		return NULL;
	}

protected:
	NetInfoList m_NetList;
};

void convrt_mac(const char *pData, char *pCvrtStr, int nSize)
{
	char acBuf[128] = {0,};
	char acTempBuf[8] = {0,};
	char *pcStp	= NULL;
	int nTemp=0;
	int nLen=0;
	if(pData == NULL || pCvrtStr == NULL || nSize < 1)
		return;
	pcStp = strtok( (char *)pData , ":" );
	do
	{
		memset(acTempBuf, 0, 8);
		sscanf(pcStp, "%x", &nTemp);
		snprintf(acTempBuf, 7, "%02X:", nTemp);
		nLen = strlen(acTempBuf);
		if(nLen + strlen(acBuf) > 127)
			break;
		strncat( acBuf, acTempBuf, nLen);
	} while( (pcStp = strtok( NULL , ":" )) != NULL );
	//acBuf[127] = '\0';
	nLen = strlen(acBuf);
	if(acBuf[nLen] == ':');
	acBuf[nLen-1] = '\0';

	strncpy( pCvrtStr, acBuf, nSize);
}

CNetInfoList g_netlist;

int GetInterfaceCount()
{
	struct ifaddrs * ifaddr, *ifa;
	int family;
	int sock;
	struct ifreq ifr;
	struct sockaddr_in* sin;
	char host[NI_MAXHOST] = {0,};
	char acMac[32] = {0,};
	char acMask[32] = {0,};
	char* pMac = NULL;
	
	g_netlist.ClearAll();

	if(getifaddrs(&ifaddr) == -1)
	{
		return 0;
	}

	for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if(ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;
		if(family == AF_INET || family == AF_INET6)
		{
			getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if(sock >= 0)
			{
				strncpy(ifr.ifr_name, ifa->ifa_name, sizeof(ifr.ifr_name)-1);
				if(ioctl(sock, SIOCGIFHWADDR, &ifr) >= 0)
				{
					pMac = ether_ntoa((struct ether_addr*)(ifr.ifr_hwaddr.sa_data));
					if(pMac != NULL)
					{
						convrt_mac(pMac, acMac, sizeof(acMac)-1);
						if(ioctl(sock, SIOCGIFNETMASK, &ifr) >= 0) // netmask
						{
							sin = (struct sockaddr_in*)&ifr.ifr_addr;
							if(sin != NULL)
								strncpy(acMask, inet_ntoa(sin->sin_addr), sizeof(acMask)-1);
						}
						g_netlist.AddNetInfo(ifa->ifa_name, acMac, host, acMask);
					}
				}
				close(sock);
			}
		}

	}
	freeifaddrs(ifaddr);
	return g_netlist.GetCount();
}

int GetIPAddr(int nIndex, char* buf, int bufLen)
{
	int nCount = 0;
	CNetInfo* pInfo = NULL;
	if(nIndex < 0 || buf == NULL || bufLen < 1)
		return 0;

	nCount = g_netlist.GetCount();
	if(nCount == 0 || nCount < nIndex)
		return 0;
		
	pInfo = g_netlist.GetAt(nIndex);
	if(pInfo == NULL)
	{
		return 0;
	}
	strncpy(buf, pInfo->szIP, bufLen-1);
	return 1;
}

int GetMacAddress(int nIndex, char* buf, int bufLen)
{
	int nCount = 0;
	CNetInfo* pInfo = NULL;

	if(nIndex < 0 || buf == NULL || bufLen < 1)
		return 0;

	nCount = g_netlist.GetCount();
	if(nCount == 0 || nCount < nIndex)
		return 0;

	pInfo = g_netlist.GetAt(nIndex);
	if(pInfo == NULL)
	{
		return 0;
	}
    strncpy(buf, pInfo->szMac, bufLen-1);
    return 1;
}

int GetInterfaceSubNet(int SelectedInterface, char* buf, int bufLen)
{
	if(buf == NULL || bufLen < 1)
		return 0;

    if(g_netlist.GetCount() > 0)
    {
        CNetInfo* pInfo = g_netlist.GetAt(SelectedInterface);
        if(pInfo)
        {
            strncpy(buf, pInfo->szSubnet, bufLen-1);
            return 1;
        }
    }
	return 0;
}

int GetNetWorkGroup(char* buf, int nBufLen)
{
	return 0;
}

int GetHostByNameAddress(char*  buf, int nBufLen)
{
	return 0;
}

int GetSubNetByAddress(const char* lpAddress, char* lpSubNet, int nLen)
{
	return 0;
}

