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

#ifndef COM_DEFINE_DBF_HEADER_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DBF_HEADER_H_938EF200_F60B_452A_BD30_A10E8507EDCC

#define ASI_DBF_MEMBER_TYPE_UINT16			0x0001
#define ASI_DBF_MEMBER_TYPE_UINT32			0x0002
#define ASI_DBF_MEMBER_TYPE_UINT64			0x0003
#define ASI_DBF_MEMBER_TYPE_STRING			0x0010
#define ASI_DBF_MEMBER_TYPE_LSTRING			0x0011
#define ASI_DBF_MEMBER_TYPE_OBJECT			0x0020

#define ASI_DBF_AUTH_USER_ID_MAX_LEN		64
#define ASI_DBF_AUTH_USER_PW_MAX_LEN		256
#define ASI_DBF_TABLE_NAME_MAX_LEN			256
#define ASI_DBF_LAST_PROC_PATH_LEN			256
#define ASI_DBF_MAX_LEN_TYPE_COL_NAME		50

#define ASI_DBF_MAX_COLUMN_NUMBER			200

#define ASI_DBF_COL_OPTION_FLAG_AUTO_INC	0x00000001
#define ASI_DBF_COL_OPTION_FLAG_PRI_KEY		0x00000002

#define ASI_DBF_MODULE_VERSION				1

#define ASI_DBF_TABLE_TYPE_SYSTEM			0
#define ASI_DBF_TABLE_TYPE_ADMIN			1
#define ASI_DBF_TABLE_TYPE_USER				2

#define ASI_DBF_EOF							26

#define ASI_DBF_TABLE_ITEM_ENC_TYPE_NONE		0x0000
#define ASI_DBF_TABLE_ITEM_ENC_TYPE_SEED128		0x0001
#define ASI_DBF_TABLE_ITEM_ENC_TYPE_BASE32		0x0002

#define ASI_DBF_TABLE_ITEM_COMP_TYPE_NONE		0x0000
#define ASI_DBF_TABLE_ITEM_COMP_TYPE_LZO		0x0001

#define ASI_DBF_FILE_HANDLE_READ				0x00000001
#define ASI_DBF_FILE_HANDLE_WRITE				0x00000002
#define ASI_DBF_FILE_HANDLE_ALL					(ASI_DBF_FILE_HANDLE_READ | ASI_DBF_FILE_HANDLE_WRITE)	

#define ASI_DBF_FAST_BUFFER_SIZE			(1024*5)

#define ASI_DBF_FAST_FILE_BUFF_MAX_SIZE			(1024*1024)
#define ASI_DBF_FAST_FILE_BUFF_MIN_SIZE			(1024*10)

#define ASI_DBF_WHERE_FP_KEY_NAME			"_asidbf_fp"


/////////////////////////////////////////////////////////////////////////
//Query Command Type
#define		ASI_DBFQ_QUERY_COMMAND_TYPE_SELECT		1
#define		ASI_DBFQ_QUERY_COMMAND_TYPE_INSERT		2
#define		ASI_DBFQ_QUERY_COMMAND_TYPE_UPDATE		3
#define		ASI_DBFQ_QUERY_COMMAND_TYPE_DELETE		4
#define		ASI_DBFQ_QUERY_COMMAND_TYPE_DROP		5
#define		ASI_DBFQ_QUERY_COMMAND_TYPE_CREATE		6
#define		ASI_DBFQ_QUERY_COMMAND_TYPE_ALTER		7
#define		ASI_DBFQ_QUERY_COMMAND_TYPE_RENAME		8

#define		ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_ADD			0x0001
#define		ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_DEL			0x0002
#define		ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_RENAME			0x0003
#define		ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_AUTOINCREMENT		0x0004
#define		ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_TB_COL_RELEN			0x0005
#define		ASI_DBFQ_QUERY_COMMAND_CODE_ALTER_DB_RECOVERY			0x0100


#define		ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL			0
#define		ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_SMALL			1
#define		ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_LARGE			2
#define		ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_NOT_EQUAL		10
#define		ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_SMALL		11
#define		ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_EQUAL_LARGE		12
#define		ASI_DBFQ_WHERE_CONDITION_COMP_TYPE_BETWEEN			100

#define		ASI_DBF_AUTH_INFO_TYPE_NONE				0x0000
#define		ASI_DBF_AUTH_INFO_TYPE_SHA2				0x0001
/////////////////////////////////////////////////////////////////////////
                                     
#endif	//COM_DEFINE_DBF_HEADER_H_938EF200_F60B_452A_BD30_A10E8507EDCC