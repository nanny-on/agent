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
#ifndef COM_DEFINE_DB_PO_IN_PTN_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_IN_PTN_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------

#define SS_PO_IN_PTN_OBJECT_TYPE_EXECUTE			0x00000001 // 
#define SS_PO_IN_PTN_OBJECT_TYPE_DLL				0x00000002 //
#define SS_PO_IN_PTN_OBJECT_TYPE_ALL				(SS_PO_IN_PTN_OBJECT_TYPE_EXECUTE | SS_PO_IN_PTN_OBJECT_TYPE_DLL) // 

#define SS_PO_IN_PTN_REALTIME_TARGET_TYPE_SVR		0
#define SS_PO_IN_PTN_REALTIME_TARGET_TYPE_AGT		1

#define SS_PO_IN_PTN_REGISTER_TYPE_UNKNOW			0x00000000 // 
#define SS_PO_IN_PTN_REGISTER_TYPE_AUTO				0x00000001 // 
#define SS_PO_IN_PTN_REGISTER_TYPE_ADMIN_ADD		0x00000002 // 
#define SS_PO_IN_PTN_REGISTER_TYPE_ADMIN_DEL		0x00000003 // 

#define SS_PO_IN_PTN_AGE_FILE_MODE_NONE				0x00000000 //
#define SS_PO_IN_PTN_AGE_FILE_MODE_INITIAL			0x00000001 // 
#define SS_PO_IN_PTN_AGE_FILE_MODE_MERGE			0x00000002 //

#define SS_PO_IN_PTN_NOTIFY_TYPE_NONE				0x00000000 // 
#define SS_PO_IN_PTN_NOTIFY_TYPE_BLOCK				0x00000001 // 
#define SS_PO_IN_PTN_NOTIFY_TYPE_REQ_ALLOW			0x00000002 // 
#define SS_PO_IN_PTN_NOTIFY_TYPE_CHG_STATUS			0x00000004 // 
#define SS_PO_IN_PTN_NOTIFY_TYPE_RES_ALLOW			0x00000008 // 
#define SS_PO_IN_PTN_NOTIFY_TYPE_RES_DENY			0x00000010 // 
#define SS_PO_IN_PTN_NOTIFY_TYPE_END_TIME			0x00000020 // 
#define SS_PO_IN_PTN_NOTIFY_TYPE_END_TIME_FOREVER	0x00000040 // 

//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_PO_IN_PTN_PTN_OP_H_938EF200_F60B_452A_BD30_A10E8507EDCC