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
#ifndef COM_DEFINE_DB_PO_NC_PTN_LO_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_NC_PTN_LO_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//block type
#define SS_PO_NC_PTN_LO_PTN_TYPE_NORMAL					0
#define SS_PO_NC_PTN_LO_PTN_TYPE_STANDARD				1

#define SS_PO_NC_PTN_LO_PTN_RANGE_TYPE_ALL_FILE			0x00000000 // 
#define SS_PO_NC_PTN_LO_PTN_RANGE_TYPE_CUR_EXECUTE		0x00000001 // 
#define SS_PO_NC_PTN_LO_PTN_RANGE_TYPE_SYS_DRIVE		0x00000002 // 

#define SS_PO_NC_PTN_LO_PTN_OBJECT_TYPE_EXECUTE			0x00000001 // 
#define SS_PO_NC_PTN_LO_PTN_OBJECT_TYPE_DLL				0x00000002 // 
#define SS_PO_NC_PTN_LO_PTN_OBJECT_TYPE_ALL				(SS_PO_NC_PTN_LO_PTN_OBJECT_TYPE_EXECUTE | SS_PO_NC_PTN_LO_PTN_OBJECT_TYPE_DLL) // 

#define SS_PO_NC_PTN_LO_NEW_FILE_SEND_TYPE_NONE			0x00000000 // 
#define SS_PO_NC_PTN_LO_NEW_FILE_SEND_TYPE_INFO			0x00000001 // 
#define SS_PO_NC_PTN_LO_NEW_FILE_SEND_TYPE_FILE			0x00000002 // 

#define SS_PO_NC_PTN_LO_EXT_OPTION_AUTO_MAKE_PTN		0x00000001

//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_PO_NC_PTN_LO_H_938EF200_F60B_452A_BD30_A10E8507EDCC