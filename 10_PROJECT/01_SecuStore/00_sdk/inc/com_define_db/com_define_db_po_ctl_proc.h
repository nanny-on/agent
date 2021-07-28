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
#ifndef COM_DEFINE_DB_PO_CTL_PROC_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_CTL_PROC_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//block type
#define SS_PO_CTL_PROC_BLOCK_MODE_ALLOW					0x00000000 // 
#define SS_PO_CTL_PROC_BLOCK_MODE_DENY					0x00000001 // 

//-------------------------------------------------------------------
//unit ex option type

//-------------------------------------------------------------------
//unit comp mode
#define SS_PO_CTL_PROC_UNIT_COMP_FLAG_FILE_NAME			0x00000001 // 파일 이름 검사
#define SS_PO_CTL_PROC_UNIT_COMP_FLAG_FILE_PATH			0x00000002 // 파일 경로 검사
#define SS_PO_CTL_PROC_UNIT_COMP_FLAG_FILE_DESC			0x00000004 // 파일 설명 검사
#define SS_PO_CTL_PROC_UNIT_COMP_FLAG_FILE_HASH			0x00000008 // 파일 해쉬 검사

//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_PO_CTL_PROC_H_938EF200_F60B_452A_BD30_A10E8507EDCC