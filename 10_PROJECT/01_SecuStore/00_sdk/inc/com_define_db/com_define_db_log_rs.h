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
#ifndef COM_DEFINE_DB_LOG_RS_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_LOG_RS_H_938EF200_F60B_452A_BD30_A10E8507EDCC


//-------------------------------------------------------------------
//block type
#define SS_LOG_RS_BLOCK_TYPE_DENY						0x00000000 // 
#define SS_LOG_RS_BLOCK_TYPE_ALLOW						0x00000001 // 
#define SS_LOG_RS_BLOCK_TYPE_WARNING					0x00000002 // 
#define SS_LOG_RS_BLOCK_TYPE_HIDE						0x00000003 // 

#define SS_LOG_RS_BLOCK_BIT_TYPE_DENY					0x00000010 // 
#define SS_LOG_RS_BLOCK_BIT_TYPE_ALLOW					0x00000020 // 
#define SS_LOG_RS_BLOCK_BIT_TYPE_WARN					0x00000040 // 
#define SS_LOG_RS_BLOCK_BIT_TYPE_HIDE					0x00000080 // 

#define SS_LOG_RS_BLOCK_BIT_TYPE_W_DENY					(SS_LOG_RS_BLOCK_BIT_TYPE_DENY|SS_LOG_RS_BLOCK_BIT_TYPE_WARN) // 
#define SS_LOG_RS_BLOCK_BIT_TYPE_W_ALLOW				(SS_LOG_RS_BLOCK_BIT_TYPE_ALLOW|SS_LOG_RS_BLOCK_BIT_TYPE_WARN) // 

//--------------------------------------------------------------------

#define SS_LOG_RS_OBJECT_TYPE_FILE						5
#define SS_LOG_RS_OBJECT_TYPE_REG						6

//--------------------------------------------------------------------

#define		SS_LOG_RS_OP_TYPE_FILE_READ					0x01
#define		SS_LOG_RS_OP_TYPE_FILE_EXECUTE				0x03
#define		SS_LOG_RS_OP_TYPE_FILE_WRITE				0x05
#define		SS_LOG_RS_OP_TYPE_FILE_CREATE				0x07
#define		SS_LOG_RS_OP_TYPE_FILE_DELETE				0x08
#define		SS_LOG_RS_OP_TYPE_FILE_RENAME				0x09
#define		SS_LOG_RS_OP_TYPE_FILE_HIDE					0x0e
#define		SS_LOG_RS_OP_TYPE_FILE_EXT_RENAME_SRC		0x10
#define		SS_LOG_RS_OP_TYPE_FILE_EXT_RENAME_DST		0x11
#define		SS_LOG_RS_OP_TYPE_PROCESS_KILL				0x06

#define		SS_LOG_RS_OP_TYPE_PROCESS_ACCESS_HIDE		0x30
#define		SS_LOG_RS_OP_TYPE_PROCESS_KILL_DENY			0x31
#define		SS_LOG_RS_OP_TYPE_THREAD_KILL_DENY			0x32

#define		SS_LOG_RS_OP_TYPE_REGISTRY_READ				0x33
#define		SS_LOG_RS_OP_TYPE_REGISTRY_WRITE			0x34
#define		SS_LOG_RS_OP_TYPE_REGISTRY_CREATE			0x35
#define		SS_LOG_RS_OP_TYPE_REGISTRY_DELETE			0x36

//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_LOG_RS_H_938EF200_F60B_452A_BD30_A10E8507EDCC