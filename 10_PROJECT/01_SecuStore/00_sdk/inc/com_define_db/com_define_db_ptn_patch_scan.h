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
#ifndef COM_DEFINE_DB_PTN_PATCH_SCAN_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PTN_PATCH_SCAN_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------

#define SS_PTN_PATCH_SCAN_COMP_TYPE_LE_THAN_BEGIN_AND_S_THAN_END		0	// Begin <= Value < End 
#define SS_PTN_PATCH_SCAN_COMP_TYPE_S_THAN_END							1	// Value < End
#define SS_PTN_PATCH_SCAN_COMP_TYPE_E_BEGIN								2	// Begin == Value
#define SS_PTN_PATCH_SCAN_COMP_TYPE_NE_END								3	// Value != End
#define SS_PTN_PATCH_SCAN_COMP_TYPE_EXIST_TARGET						4	// Exist value
#define SS_PTN_PATCH_SCAN_COMP_TYPE_NOT_EXIST_TARGET					5	// Not exist value

#define SS_PTN_PATCH_SCAN_TYPE_FILE_PATH_FILE_NAME						0	// %
#define SS_PTN_PATCH_SCAN_TYPE_REG_PATH_FILE_NAME						1
#define SS_PTN_PATCH_SCAN_TYPE_REG_PATH_REG_VALUE						2

#define SS_PTN_PATCH_SCAN_PROC_ARCH_86									0
#define SS_PTN_PATCH_SCAN_PROC_ARCH_64									9
#define SS_PTN_PATCH_SCAN_PROC_ARCH_ALL									99

#define SS_PTN_SCAN_FILE_EXE_SESSION_SYSTEM								0
#define SS_PTN_SCAN_FILE_EXE_SESSION_DESKTOP							1

//--------------------------------------------------------------------

#endif	//COM_DEFINE_DB_PTN_PATCH_SCAN_H_938EF200_F60B_452A_BD30_A10E8507EDCC