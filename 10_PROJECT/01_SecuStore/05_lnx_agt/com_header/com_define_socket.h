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


#ifndef COM_DEFINE_SOCKET_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_SOCKET_H_938EF200_F60B_452A_BD30_A10E8507EDCC


//--------------------------------------------------------------------
//packet type
#define SS_PKT_TYPE_CONSOLE							0
#define SS_PKT_TYPE_AGENT							1

//--------------------------------------------------------------------
//open port value
#define OPEN_PORT_TYPE_CONSOLE						60000
#define OPEN_PORT_TYPE_AGT_LGN						60010
#define OPEN_PORT_TYPE_AGT_UDT						60011
#define OPEN_PORT_TYPE_AGT_MGR						60013

#define OPEN_PORT_TYPE_SVR_LGN						60020
#define OPEN_PORT_TYPE_SVR_MGR						60021
#define OPEN_PORT_TYPE_SVR_UDT						60022
#define OPEN_PORT_TYPE_SVR_RPT						60023

#define OPEN_PORT_TYPE_WEB_UPDATE					60030
#define OPEN_PORT_TYPE_WEB_INSTALL					60031

#define OPEN_PORT_TYPE_AGT_INTERFACE				60040
#define OPEN_PORT_TYPE_AGT_PTOP						60041


#define	STR_LICENSE_DOMAIN							"sslicense.ashindev.com"
#define	STR_DEPLOY_DOMAIN							"ssdeploy.ashindev.com"
#define CON_PORT_TYPE_LICENSE_INFO					60002
#define CON_PORT_TYPE_DEPLOY_FILE					60020

//--------------------------------------------------------------------
//connect status type
#define CLIENT_CON_STATUS_DISCONNECTED				0
#define	CLIENT_CON_STATUS_CONNECTED					1
#define CLIENT_CON_STATUS_AUTH_C					10
#define CLIENT_CON_STATUS_AUTH_M					11
#define CLIENT_CON_STATUS_AUTH_D					12
#define	CLIENT_CON_STATUS_WAIT_START				20


//--------------------------------------------------------------------
//connect session management
#define	CONSOLE_MONITOR_SESSION_GAB					10000
#define	CONSOLE_DISCON_SESSION_GAB					100000
#define SOCKET_SSL_PORT_GAB							500

//--------------------------------------------------------------------
//session key
#define	DEFAULT_SESSION_CONSOLE_KEY					1146784346
#define	DEFAULT_SESSION_AGENT_KEY					1146784347
#define DEFAULT_SESSION_LCN_KEY						1146784347

//--------------------------------------------------------------------
// AnalyzePkt CallBack Rtn Type
#define AZPKT_CB_RTN_SUCCESS				0
#define AZPKT_CB_RTN_RESULT_FAILED			1
#define AZPKT_CB_RTN_PKT_INVALID			10
#define AZPKT_CB_RTN_PKT_NOT_DEFINE_TYPE	11
#define AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE	12
#define AZPKT_CB_RTN_SEND_ACK				30
#define AZPKT_CB_RTN_DBMS_FAIL				40

#define AZPKT_CB_RTN_SUCCESS_END			100
#define AZPKT_CB_RTN_SUCCESS_MADE_PKT		101

//--------------------------------------------------------------------

#define SS_LICENSE_CHK_MODULE_TYPE_MAIN			1
#define SS_LICENSE_CHK_MODULE_TYPE_CONSOLE		2
#define SS_LICENSE_CHK_MODULE_TYPE_LGN			3
#define SS_LICENSE_CHK_MODULE_TYPE_MGR			4
#define SS_LICENSE_CHK_MODULE_TYPE_UDT			5
#define SS_LICENSE_CHK_MODULE_TYPE_RPT			6
//--------------------------------------------------------------------

#define SS_LICENSE_CHK_PKT_TYPE_DEFAULT			0
//--------------------------------------------------------------------

#endif	//COM_DEFINE_SOCKET_H_938EF200_F60B_452A_BD30_A10E8507EDCC