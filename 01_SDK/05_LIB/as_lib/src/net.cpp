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
#include "as_util.h"
#include "as_net.h"


/* thread-safe inet_ntoa */
char *inet_ntoa(struct in_addr in, char *buf, int len)
{
	in_addr_t      addr;
	unsigned char  *addr_ptr;

	if (buf == NULL || len <= 0)
		return NULL;

	addr = in.s_addr;
	addr_ptr = (unsigned char *)&addr;

	snprintf(buf, len, "%d.%d.%d.%d",
			addr_ptr[0] & 0xff,
			addr_ptr[1] & 0xff,
			addr_ptr[2] & 0xff,
			addr_ptr[3] & 0xff);

	return buf;
}


/* dns 서버를 이용해 url과 매칭되는 ip 주소를 얻는다 */
int get_ipaddr_by_url(char *url, char *ipaddr, int len)
{
	int             error;
	char            hent_buf[8192] = { 0,};
	struct hostent  hent, *hres;

	if (gethostbyname_r(url, &hent, hent_buf, sizeof(hent_buf), &hres, &error) != 0)
	{
		return -1;
	}

	if (hres == NULL)
	{
		return -2;
	}

	snprintf(ipaddr, len, "%s", inet_ntoa(*((struct in_addr *)hent.h_addr_list[0])));
	return 0;
}


/* 소켓 통신 시 사용하는 네트워크 인터페이스의
 * IP 주소를 얻는다 (xxx.xxx.xxx.xxx)
 */
void get_ipaddr_by_sock(int sock, char *buf, int buf_len)
{
	socklen_t           addr_len;
	struct sockaddr_in  addr;

	addr_len = sizeof(addr);
	getsockname(sock, (struct sockaddr *)&addr, &addr_len);

	inet_ntoa(addr.sin_addr, buf, buf_len);
}


/* 해당 네트워크 인터페이스의 IP 주소를 얻는다 */
int get_ipaddr_by_netiface(char *netiface, unsigned int *ipaddr)
{
	int           fd;
	struct ifreq  ifrq;

	memset(&ifrq, 0x00, sizeof(ifrq));

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		return -1;
	}

	snprintf(ifrq.ifr_name, sizeof(ifrq.ifr_name), "%s", netiface);

	if (ioctl(fd, SIOCGIFADDR, &ifrq) < 0)
	{
		close(fd);
		return -2;
	}

	/* copy ip addr */
	memcpy(ipaddr, &ifrq.ifr_addr.sa_data[2], 4);

	close(fd);
	return 0;
}


/* 해당 네트워크 인터페이스의 MAC 주소를 얻는다 */
int get_macaddr_by_netiface(char *netiface, unsigned char *macaddr)
{
	int  fd;
	struct ifreq  ifrq;

	memset(&ifrq, 0x00, sizeof(ifrq));

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		return -1;
	}

	snprintf(ifrq.ifr_name, sizeof(ifrq.ifr_name), "%s", netiface);

	if (ioctl(fd, SIOCGIFHWADDR, &ifrq) < 0)
	{
		close(fd);
		return -2;
	}

	/* copy mac addr */
	memcpy(macaddr, &ifrq.ifr_addr.sa_data[0], 6);

	close(fd);
	return 0;
}


/* 해당 IP 주소가 할당된 네트워크 인터페이스를 얻는다 */
int get_netiface_by_ipaddr(unsigned int ipaddr, char *iface, int iface_len)
{
	int            i;
	int            fd, len;
	int            get_info = 0;
	struct ifreq   *ifr;
	struct ifconf  ifcfg;

	len = sizeof(struct ifreq) * ASI_DFT_GET_NETIFACE_CNT;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		return -1;
	}

	memset(&ifcfg, 0x00, sizeof(ifcfg));

get_netiface_info:

	ifcfg.ifc_buf = (char *) malloc(len);
	if(ifcfg.ifc_buf == NULL)
	{
		return -2;
	}

	ifcfg.ifc_len = len;

	if (ioctl(fd, SIOCGIFCONF, (char *) &ifcfg) < 0)
	{
		safe_free(ifcfg.ifc_buf);

		if (errno != EINVAL)
		{
			close(fd);
			return -3;
		}
		else
		{
			len *= 2;
			goto get_netiface_info;
		}
	}

	ifr = ifcfg.ifc_req;

	for (i = 0; i < ifcfg.ifc_len; i += sizeof(struct ifreq))
	{
		if (ifr->ifr_addr.sa_family == AF_INET && strcmp(ifr->ifr_name, "lo") != 0)
		{
			if (memcmp(&ipaddr, &ifr->ifr_addr.sa_data[2], 4) == 0)
			{
				snprintf(iface, iface_len, "%s", ifr->ifr_name);
				get_info = 1;
				break;
			}
		}
		ifr++;
	}

	safe_free(ifcfg.ifc_buf);
	close(fd);

	if (get_info == 0)
	{
		return -4;
	}
	return 0;
}


/* 해당 MAC 주소가 할당된 네트워크 인터페이스를 얻는다 */
int get_netiface_by_macaddr(unsigned char *macaddr, char *iface, int iface_len)
{
	int  i;
	int  fd, len;
	int  get_info = 0;
	struct ifreq   *ifr;
	struct ifconf  ifcfg;

	len = sizeof(struct ifreq) * ASI_DFT_GET_NETIFACE_CNT;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		return -1;
	}

	memset(&ifcfg, 0x00, sizeof(ifcfg));

get_netiface_info:

	ifcfg.ifc_buf = (char *) malloc(len);
	if(ifcfg.ifc_buf == NULL)
	{
		return -2;
	}
	ifcfg.ifc_len = len;

	if (ioctl(fd, SIOCGIFCONF, (char *)&ifcfg) < 0)
	{
		safe_free(ifcfg.ifc_buf);
		if (errno != EINVAL)
		{
			close(fd);
			return -3;
		}
		else
		{
			len *= 2;
			goto get_netiface_info;
		}
	}

	ifr = ifcfg.ifc_req;

	for (i = 0; i < ifcfg.ifc_len; i += sizeof(struct ifreq), ifr++) {

		if (ifr->ifr_addr.sa_family == AF_INET && strcmp(ifr->ifr_name, "lo") != 0)
		{
			if (ioctl(fd, SIOCGIFHWADDR, ifr) < 0)
			{
				continue;
			}

			if (memcmp(macaddr, &ifr->ifr_addr.sa_data[0], 6) == 0)
			{
				snprintf(iface, iface_len, "%s", ifr->ifr_name);
				get_info = 1;
				break;
			}
		}
	}

	safe_free(ifcfg.ifc_buf);
	close(fd);

	if (get_info == 0)
	{
		return -4;
	}
	return 0;
}


/* 첫번째로 얻어지는 네트워크 인터페이스의 MAC 주소 얻기 */
int get_first_macaddr(unsigned char *macaddr)
{
	int  i;
	int  fd, len;
	int  get_info = 0;
	struct ifreq   *ifr;
	struct ifconf  ifcfg;

	len = sizeof(struct ifreq) * ASI_DFT_GET_NETIFACE_CNT;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
	{
		return -1;
	}

	memset(&ifcfg, 0x00, sizeof(ifcfg));

get_netiface_info:

	ifcfg.ifc_buf = (char *) malloc(len);
	if(ifcfg.ifc_buf == NULL)
	{
		return -2;
	}
	ifcfg.ifc_len = len;

	if (ioctl(fd, SIOCGIFCONF, (char *)&ifcfg) < 0)
	{
		safe_free(ifcfg.ifc_buf);

		if (errno != EINVAL)
		{
			close(fd);
			return -3;
		}
		else
		{
			len *= 2;
			goto get_netiface_info;
		}
	}

	ifr = ifcfg.ifc_req;

	for (i = 0; i < ifcfg.ifc_len; i += sizeof(struct ifreq), ifr++)
	{
		if (ifr->ifr_addr.sa_family == AF_INET && strcmp(ifr->ifr_name, "lo") != 0)
		{

			if (ioctl(fd, SIOCGIFHWADDR, ifr) < 0)
			{
				continue;
			}

			/* copy first mac addr */
			memcpy(macaddr, &ifr->ifr_addr.sa_data[0], 6);
			get_info = 1;
			break;
		}
	}

	safe_free(ifcfg.ifc_buf);
	close(fd);

	if (get_info == 0)
	{
		return -4;
	}
	return 0;
}


