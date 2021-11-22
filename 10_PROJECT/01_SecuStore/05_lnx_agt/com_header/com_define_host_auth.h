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


#ifndef COM_DEFINE_HOST_AUTH_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_HOST_AUTH_H_938EF200_F60B_452A_BD30_A10E8507EDCC

#define HOST_AUTH_RESULT_NO_ERROR						0
#define HOST_AUTH_RESULT_INVALID_GUID					1
#define HOST_AUTH_RESULT_INVALID_MAC					2
#define HOST_AUTH_RESULT_INVALID_PKT					3
#define HOST_AUTH_RESULT_FAIL_ADD_TO_DBMS				10
#define HOST_AUTH_RESULT_FAIL_RETRY_CONNECT				20
#define HOST_AUTH_RESULT_FAIL_RETRY_REGISTER			21
#define HOST_AUTH_RESULT_FAIL_RETRY_REG_CHG_GUID		22
#define HOST_AUTH_RESULT_AUTO_UNINSTALL					100
#define HOST_AUTH_RESULT_REQUEST_UPDATE					200
#define HOST_AUTH_RESULT_NOT_READY_MGR_SVR_INFO			201
#define HOST_AUTH_RESULT_NOT_READY_UDT_SVR_INFO			202
#define HOST_AUTH_RESULT_NOT_READY_UDT_DATA_SVR_INFO	203
#define HOST_AUTH_RESULT_SERVER_ERROR					300


#define HOST_AUTH_REG_STEP_INFO					0x0001
#define HOST_AUTH_REG_STEP_STATUS				0x0002
#define HOST_AUTH_REG_STEP_HW					0x0004
#define HOST_AUTH_REG_STEP_SYS					0x0008

#define HOST_AUTH_CHECK_VALUE					1540834428

#define HOST_AUTH_SYSINFO_EMPTY					0x0001
                                     
#endif	//COM_DEFINE_HOST_AUTH_H_938EF200_F60B_452A_BD30_A10E8507EDCC