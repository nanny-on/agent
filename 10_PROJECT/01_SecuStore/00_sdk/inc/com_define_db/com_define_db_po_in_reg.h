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
 *//*
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
 * 
 */


#ifndef COM_DEFINE_DB_PO_IN_REG_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_IN_REG_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//in reg log mode
#define SS_PO_IN_REG_LOG_TYPE_ALLOW						0
#define SS_PO_IN_REG_LOG_TYPE_DENY						1

#define SS_PO_IN_REG_LOG_TYPE_CREATE				0x00000020
#define SS_PO_IN_REG_LOG_TYPE_DELETE				0x00000010
#define SS_PO_IN_REG_LOG_TYPE_WRITE					0x00000002
#define SS_PO_IN_REG_LOG_TYPE_NONE					0x00000000

//-------------------------------------------------------------------
//in reg perm

#define SS_PO_IN_REG_PERM_CREATE						0x00000020
#define SS_PO_IN_REG_PERM_DELETE						0x00000010
#define SS_PO_IN_REG_PERM_WRITE							0x00000002
#define SS_PO_IN_REG_PERM_NONE							0x00000000
//--------------------------------------------------------------------

//in reg ext option common 
#define SS_PO_IN_REG_EXT_OPTION_COMMON_LAST_COMP_CHECK		0x00000001

//-------------------------------------------------------------------
//fa op obj unit access path
#define SS_PO_IN_REG_OBJ_UNIT_ACCESS_PATH_LOCAL				0x00000001
#define SS_PO_IN_REG_OBJ_UNIT_ACCESS_PATH_NETWORK			0x00000002

//-------------------------------------------------------------------
//in reg sub unit subject type
#define SS_PO_IN_REG_SUB_UNIT_SUBJECT_TYPE_ANY			0x0000
#define SS_PO_IN_REG_SUB_UNIT_SUBJECT_TYPE_USER			0x0001
#define SS_PO_IN_REG_SUB_UNIT_SUBJECT_TYPE_GROUP		0x0002

//-------------------------------------------------------------------
//in reg unit key type
#define SS_PO_IN_REG_UNIT_KEY_TYPE_OBJECT				0x01 // 
#define SS_PO_IN_REG_UNIT_KEY_TYPE_SUBJECT				0x02 // 
#define SS_PO_IN_REG_UNIT_KEY_TYPE_RULE					0x03 // 
#define SS_PO_IN_REG_UNIT_KEY_TYPE_SCHEDULE				0x04 // 

//-------------------------------------------------------------------
//protect type
#define SS_PO_IN_REG_UNIT_TYPE_REG					0x00000001 // 
#define SS_PO_IN_REG_UNIT_TYPE_PROC					0x00000002 // 

#endif	//COM_DEFINE_DB_PO_IN_REG_H_938EF200_F60B_452A_BD30_A10E8507EDCC