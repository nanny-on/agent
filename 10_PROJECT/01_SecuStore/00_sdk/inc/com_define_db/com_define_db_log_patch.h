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
#ifndef COM_DEFINE_DB_LOG_PATCH_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_LOG_PATCH_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//block type
#define SS_LOG_PATCH_STATUS_TYPE_FIND					1
#define SS_LOG_PATCH_STATUS_TYPE_DOWN					2
#define SS_LOG_PATCH_STATUS_TYPE_DOWN_FAIL				3
#define SS_LOG_PATCH_STATUS_TYPE_DOWN_FAIL_REF_OVER		4

#define SS_LOG_PATCH_STATUS_TYPE_WAIT_USER				10

#define SS_LOG_PATCH_STATUS_TYPE_WAIT_AUTO				15

#define SS_LOG_PATCH_STATUS_TYPE_APPLY_AUTO				20
#define SS_LOG_PATCH_STATUS_TYPE_APPLY_USER				21

#define SS_LOG_PATCH_STATUS_TYPE_END_RESTART			30
#define SS_LOG_PATCH_STATUS_TYPE_END					40

#define SS_LOG_PATCH_STATUS_TYPE_ROLLBACK				100

//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_LOG_PATCH_H_938EF200_F60B_452A_BD30_A10E8507EDCC