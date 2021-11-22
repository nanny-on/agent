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
#ifndef COM_DEFINE_DB_PO_IN_RS_OP_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_IN_RS_OP_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//block type
#define SS_PO_IN_RS_OP_UNIT_KEY_TYPE_OBJECT					0x01
#define	SS_PO_IN_RS_OP_UNIT_KEY_TYPE_SUBJECT				0x02


#define SS_PO_IN_RS_NOTIFY_TYPE_NONE						0x00000000 // 
#define SS_PO_IN_RS_NOTIFY_TYPE_BLOCK						0x00000001 // 
//--------------------------------------------------------------------


#define SS_PO_IN_RS_LOG_OP_TYPE_RENDSTEXT				0x00002000
#define SS_PO_IN_RS_LOG_OP_TYPE_SHORT					0x00001000
#define SS_PO_IN_RS_LOG_OP_TYPE_RENSRCEXT				0x00000800
#define SS_PO_IN_RS_LOG_OP_TYPE_FILELOG					0x00000400
#define SS_PO_IN_RS_LOG_OP_TYPE_TKILL					0x00000200
#define SS_PO_IN_RS_LOG_OP_TYPE_PEND					0x00000100
#define SS_PO_IN_RS_LOG_OP_TYPE_HIDE					0x00000080
#define SS_PO_IN_RS_LOG_OP_TYPE_PROCKILL				0x00000040
#define SS_PO_IN_RS_LOG_OP_TYPE_CREATE					0x00000020
#define SS_PO_IN_RS_LOG_OP_TYPE_DELETE					0x00000010
#define SS_PO_IN_RS_LOG_OP_TYPE_RENAME					0x00000008
#define SS_PO_IN_RS_LOG_OP_TYPE_READ					0x00000004
#define SS_PO_IN_RS_LOG_OP_TYPE_WRITE					0x00000002
#define SS_PO_IN_RS_LOG_OP_TYPE_EXEC					0x00000001
#define SS_PO_IN_RS_LOG_OP_TYPE_NONE					0x00000000

//-------------------------------------------------------------------
//in rs perm
#define SS_PO_IN_RS_PERM_RENDSTEXT						0x00002000
#define SS_PO_IN_RS_PERM_SHORT							0x00001000
#define SS_PO_IN_RS_PERM_RENSRCEXT						0x00000800
#define SS_PO_IN_RS_PERM_FILELOG						0x00000400
#define SS_PO_IN_RS_PERM_TKILL							0x00000200
#define SS_PO_IN_RS_PERM_PEND							0x00000100
#define SS_PO_IN_RS_PERM_HIDE							0x00000080
#define SS_PO_IN_RS_PERM_PROCKILL						0x00000040
#define SS_PO_IN_RS_PERM_CREATE							0x00000020
#define SS_PO_IN_RS_PERM_DELETE							0x00000010
#define SS_PO_IN_RS_PERM_RENAME							0x00000008
#define SS_PO_IN_RS_PERM_READ							0x00000004
#define SS_PO_IN_RS_PERM_WRITE							0x00000002
#define SS_PO_IN_RS_PERM_EXEC							0x00000001
#define SS_PO_IN_RS_PERM_NONE							0x00000000

//-------------------------------------------------------------------
//in rs log mode
#define SS_PO_IN_RS_LOG_TYPE_ALLOW							0
#define SS_PO_IN_RS_LOG_TYPE_DENY							1

//-------------------------------------------------------------------
//in rs op obj unit access path
#define SS_PO_IN_RS_OP_OBJ_UNIT_ACCESS_PATH_LOCAL			0x00000001
#define SS_PO_IN_RS_OP_OBJ_UNIT_ACCESS_PATH_NETWORK			0x00000002


//-------------------------------------------------------------------
//in rs op sub unit subject type
#define SS_PO_IN_RS_OP_SUB_UNIT_SUBJECT_TYPE_ANY			0x0000
#define SS_PO_IN_RS_OP_SUB_UNIT_SUBJECT_TYPE_USER			0x0001
#define SS_PO_IN_RS_OP_SUB_UNIT_SUBJECT_TYPE_GROUP			0x0002

//-------------------------------------------------------------------
#endif	//COM_DEFINE_DB_PO_IN_RS_OP_H_938EF200_F60B_452A_BD30_A10E8507EDCC

