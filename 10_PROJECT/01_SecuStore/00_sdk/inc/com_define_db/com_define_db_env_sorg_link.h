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
 */
#ifndef COM_DEFINE_DB_ENV_SORG_LINK_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_ENV_SORG_LINK_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//option ext

//-------------------------------------------------------------------
//match type
#define SS_ENV_SORG_LINK_MATCH_TYPE_USER_UNICODE			0x00000001 // 
#define SS_ENV_SORG_LINK_MATCH_TYPE_PC_IP_ADDR_NET			0x00000002 // 
#define SS_ENV_SORG_LINK_MATCH_TYPE_PC_MAC_ADDR				0x00000004 // 
#define SS_ENV_SORG_LINK_MATCH_TYPE_PC_IP_ADDR_PRIVATE		0x00000008 //
#define SS_ENV_SORG_LINK_MATCH_TYPE_PC_UNICODE				0x00000010
#define SS_ENV_SORG_LINK_MATCH_TYPE_PC_IP_ADDR_INFO			(SS_ENV_SORG_LINK_MATCH_TYPE_PC_IP_ADDR_NET | SS_ENV_SORG_LINK_MATCH_TYPE_PC_IP_ADDR_PRIVATE)
#define SS_ENV_SORG_LINK_MATCH_TYPE_PC_IP_MAC_INFO			(SS_ENV_SORG_LINK_MATCH_TYPE_PC_IP_ADDR_INFO | SS_ENV_SORG_LINK_MATCH_TYPE_PC_MAC_ADDR)
#define SS_ENV_SORG_LINK_MATCH_TYPE_PC_INFO					(SS_ENV_SORG_LINK_MATCH_TYPE_PC_UNICODE | SS_ENV_SORG_LINK_MATCH_TYPE_PC_IP_ADDR_NET | SS_ENV_SORG_LINK_MATCH_TYPE_PC_MAC_ADDR | SS_ENV_SORG_LINK_MATCH_TYPE_PC_IP_ADDR_PRIVATE) // 
#define SS_ENV_SORG_LINK_MATCH_TYPE_ALL						(SS_ENV_SORG_LINK_MATCH_TYPE_USER_UNICODE | SS_ENV_SORG_LINK_MATCH_TYPE_PC_INFO) // 

//-------------------------------------------------------------------
//user id catch type
#define SS_ENV_SORG_LINK_USER_CATCH_TYPE_COM_NAME			0x00000000
#define SS_ENV_SORG_LINK_USER_CATCH_TYPE_MANAGER			0x00000001
#define SS_ENV_SORG_LINK_USER_CATCH_TYPE_REGISTRY			0x00000002
#define SS_ENV_SORG_LINK_USER_CATCH_TYPE_USER_NAME			0x00000003

//-------------------------------------------------------------------
//db sync type
#define SS_ENV_SORG_LINK_DB_TB_TYPE_GROUP					0x00000001
#define SS_ENV_SORG_LINK_DB_TB_TYPE_USER					0x00000002
#define SS_ENV_SORG_LINK_DB_TB_TYPE_PC						0x00000003

//-------------------------------------------------------------------
//dbms type
#define SS_ENV_SORG_LINK_DB_DBMS_TYPE_MYSQL					0x00000001
#define SS_ENV_SORG_LINK_DB_DBMS_TYPE_MSSQL					0x00000100
#define SS_ENV_SORG_LINK_DB_DBMS_TYPE_ORACLE				0x00100000
#define SS_ENV_SORG_LINK_DB_DBMS_TYPE_EXCEL					0x01000000

//-------------------------------------------------------------------
//ext_option
//group show order 
#define SS_ENV_SORG_LINK_DB_TB_GROUP_SHOW_ORDER_TYPE_ASCENDING		0x00000000
#define SS_ENV_SORG_LINK_DB_TB_GROUP_SHOW_ORDER_TYPE_DESCENDING		0x00000001

#define SS_ENV_SORG_LINK_DB_SHOW_USER_TYPE_LINK						0x00000000
#define SS_ENV_SORG_LINK_DB_SHOW_USER_TYPE_ALL						0x00000002

#define SS_ENV_SORG_LINK_DB_SYNC_TYPE_SCHEDULE						0x00000000
#define SS_ENV_SORG_LINK_DB_SYNC_TYPE_REALTIME						0x00000004

//sorg_link_used_mode
#define SS_ENV_SORG_LINK_USED_MODE_USER_MATCH_GROUP					0x00000001
#define SS_ENV_SORG_LINK_USED_MODE_PC_MATCH_GROUP					0x00000002

//-------------------------------------------------------------------
//tb column type
#define SS_ENV_SORG_LINK_TB_COL_TYPE_GROUP_CODE				0x00000001
#define SS_ENV_SORG_LINK_TB_COL_TYPE_GROUP_P_CODE			0x00000002
#define SS_ENV_SORG_LINK_TB_COL_TYPE_GROUP_NAME				0x00000004
#define SS_ENV_SORG_LINK_TB_COL_TYPE_GROUP_SHOW_ORDER		0x00000008

#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_NAME				0x00000100
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_G_CODE			0x00000200
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_CO_ID				0x00000400
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_CO_RANK			0x00000800
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_CO_RESPON			0x00001000
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_CO_EMAIL			0x00002000
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_CO_PHONE			0x00004000
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_HPHONE			0x00008000
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_UNICODE			0x00010000
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_AUTH_ID			0x00020000
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_AUTH_PW			0x00040000
#define SS_ENV_SORG_LINK_TB_COL_TYPE_USER_SRC_GROUP			0x00080000

#define SS_ENV_SORG_LINK_TB_COL_TYPE_PC_USER_UNICODE		0x00100000
#define SS_ENV_SORG_LINK_TB_COL_TYPE_PC_IP_ADDR				0x00200000
#define SS_ENV_SORG_LINK_TB_COL_TYPE_PC_MAC_ADDR			0x00400000

#define SS_ENV_SORG_LINK_TB_COL_TYPE_PC_GROUP				0x00800000
#define SS_ENV_SORG_LINK_TB_COL_TYPE_PC_IP_ADDR_PRIVATE		0x01000000

#define SS_ENV_SORG_LINK_TB_COL_TYPE_PC_UNICODE				0x02000000

//-------------------------------------------------------------------

#define SS_ENV_SORG_LINK_EXCLUDE_MODE_OFF					0
#define SS_ENV_SORG_LINK_EXCLUDE_MODE_ON					1
#define SS_ENV_SORG_LINK_EXCLUDE_MODE_NON_MOVE_GROUP		2

//-------------------------------------------------------------------

#define SS_ENV_SORG_LINK_EXCLUDE_MATCH_TARGET_TYPE_HOST		0
#define SS_ENV_SORG_LINK_EXCLUDE_MATCH_TARGET_TYPE_GROUP	1
#define SS_ENV_SORG_LINK_EXCLUDE_MATCH_TARGET_TYPE_USER		2

//-------------------------------------------------------------------
//-------------------------------------------------------------------

#endif	//COM_DEFINE_DB_ENV_SORG_LINK_H_938EF200_F60B_452A_BD30_A10E8507EDCC