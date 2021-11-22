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

#ifndef _AS_NET_H_
#define _AS_NET_H_

#define ASI_DFT_GET_NETIFACE_CNT			20


extern char *inet_ntoa(struct in_addr in, char *buf, int len);
extern int get_ipaddr_by_url(char *url, char *ipaddr, int len);
extern void get_ipaddr_by_sock(int sock, char *buf, int buf_len);
extern int get_ipaddr_by_netiface(char *netiface, unsigned int *ipaddr);
extern int get_macaddr_by_netiface(char *netiface, unsigned char *macaddr);
extern int get_netiface_by_ipaddr(unsigned int ipaddr, 
		char *iface, int iface_len);
extern int get_netiface_by_macaddr(unsigned char *mac_addr, 
		char *iface, int iface_len);
extern int get_first_macaddr(unsigned char *macaddr);


#endif /* _AS_NET_H_ */


