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
 *//*
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
#ifndef COM_DEFINE_DB_PTN_PATCH_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PTN_PATCH_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//RISK
#define SS_PTN_PATCH_RISK_TYPE_URGENCY				1
#define SS_PTN_PATCH_RISK_TYPE_HIGH					2
#define SS_PTN_PATCH_RISK_TYPE_MEDIUM				3
#define SS_PTN_PATCH_RISK_TYPE_LOW					4

#define SS_PTN_PATCH_TYPE_UNKNOWN					0
#define SS_PTN_PATCH_TYPE_OS						0x00000001
#define SS_PTN_PATCH_TYPE_OS_VULN					0x00000002
#define SS_PTN_PATCH_TYPE_OS_SERVICE				0x00000004
#define SS_PTN_PATCH_TYPE_OS_BUILD					0x00000008
#define SS_PTN_PATCH_TYPE_OS_FUNC					0x00000010

#define SS_PTN_PATCH_TYPE_APP						0x00010000
#define SS_PTN_PATCH_TYPE_APP_VULN					0x00020000
#define SS_PTN_PATCH_TYPE_APP_SERVICE				0x00040000
#define SS_PTN_PATCH_TYPE_APP_BUILD					0x00080000
#define SS_PTN_PATCH_TYPE_APP_FUNC					0x00100000

#define SS_PTN_PATCH_PROC_ARCH_86					0
#define SS_PTN_PATCH_PROC_ARCH_64					9

#define SS_PTN_PATCH_REBOOT_NONE					0
#define SS_PTN_PATCH_REBOOT_NECESSARY				1

#define SS_PTN_PATCH_ROLLBACK_NONE					0
#define SS_PTN_PATCH_ROLLBACK_POSSIBLE				1
//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_PTN_PATCH_H_938EF200_F60B_452A_BD30_A10E8507EDCC