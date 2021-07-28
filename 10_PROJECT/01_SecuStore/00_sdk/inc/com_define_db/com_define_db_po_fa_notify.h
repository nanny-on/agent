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
 */
#ifndef COM_DEFINE_DB_PO_FA_NOTIFY_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_FA_NOTIFY_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//block type
#define SS_PO_FA_NOTIFY_MSG_TYPE_SCH_START				0x80000000
#define SS_PO_FA_NOTIFY_MSG_TYPE_FILEDELFIN				0x40000000
#define SS_PO_FA_NOTIFY_MSG_TYPE_FILEDEL				0x20000000
#define SS_PO_FA_NOTIFY_MSG_TYPE_FILELOG				0x10000000

#define SS_PO_FA_NOTIFY_MSG_TYPE_RENEXT					0x00000800
//#define SS_PO_FA_NOTIFY_MSG_TYPE_TKILL				0x00000200
//#define SS_PO_FA_NOTIFY_MSG_TYPE_PEND					0x00000100
//#define SS_PO_FA_NOTIFY_MSG_TYPE_HIDE					0x00000080
//#define SS_PO_FA_NOTIFY_MSG_TYPE_PROCKILL				0x00000040
#define SS_PO_FA_NOTIFY_MSG_TYPE_CREATE					0x00000020
#define SS_PO_FA_NOTIFY_MSG_TYPE_DELETE					0x00000010
#define SS_PO_FA_NOTIFY_MSG_TYPE_RENAME					0x00000008
#define SS_PO_FA_NOTIFY_MSG_TYPE_READ					0x00000004
#define SS_PO_FA_NOTIFY_MSG_TYPE_WRITE					0x00000002
#define SS_PO_FA_NOTIFY_MSG_TYPE_EXEC					0x00000001
#define SS_PO_FA_NOTIFY_MSG_TYPE_NONE					0x00000000

//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_PO_FA_NOTIFY_H_938EF200_F60B_452A_BD30_A10E8507EDCC