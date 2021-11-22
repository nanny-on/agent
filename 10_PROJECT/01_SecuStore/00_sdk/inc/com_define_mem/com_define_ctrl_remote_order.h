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


#ifndef COM_DEFINE_COMMON_REMOTE_ORDER_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_COMMON_REMOTE_ORDER_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
// agent alert option
#define SS_COMMON_REMOTE_ORDER_TYPE_NO_PTN_GET_FILE_INFO						0x00000000
#define SS_COMMON_REMOTE_ORDER_TYPE_NO_PTN_RE_SEND_FILE_INFO					0x00000001
#define SS_COMMON_REMOTE_ORDER_TYPE_FE_PTN_GET_FILE_INFO						0x00000010
#define SS_COMMON_REMOTE_ORDER_TYPE_NC_PTN_GET_FILE_INFO						0x00000011
#define SS_COMMON_REMOTE_ORDER_TYPE_NC_PTN_RE_SEND_FILE_INFO					0x00000012
#define SS_COMMON_REMOTE_ORDER_TYPE_NC_PTN_MAKE_LOCAL_PTN						0x00000013
#define SS_COMMON_REMOTE_ORDER_TYPE_NI_RECOVERY_BK_FILE							0x00000020
#define SS_COMMON_REMOTE_ORDER_TYPE_RE_GET_SYSTEMINFO							0x00000040
#define SS_COMMON_REMOTE_ORDER_TYPE_NP_SCAN_PATCH								0x00000050
#define SS_COMMON_REMOTE_ORDER_TYPE_NO_VUIN_SCAN								0x00000060

//-------------------------------------------------------------------

#define SS_COMMON_REMOTE_ORDER_TYPE_RE_GET_SYSTEMINFO_OPT_SYSINFOCLEAR			0x00000001
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//--------------------------------------------------------------------
#endif	//COM_DEFINE_COMMON_REMOTE_ORDER_H_938EF200_F60B_452A_BD30_A10E8507EDCC