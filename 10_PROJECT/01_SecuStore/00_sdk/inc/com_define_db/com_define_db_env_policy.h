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
#ifndef COM_DEFINE_DB_ENV_POLICY_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_ENV_POLICY_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//env policy type
#define SS_ENV_POLICY_TYPE_OBJECT					0 // 객체 기반 ( 그룹, 호스트 )
#define SS_ENV_POLICY_TYPE_SUBJECT					1 // 주체 기반 ( 자연인 그룹, 자연인) 

//-------------------------------------------------------------------
//env policy target type
#define SS_ENV_POLICY_TARGET_TYPE_USER				0x00000001 // 사용자
#define SS_ENV_POLICY_TARGET_TYPE_USER_GROUP		0x00000002 // 사용자 그룹
#define SS_ENV_POLICY_TARGET_TYPE_HOST				0x00000004 // PC
#define SS_ENV_POLICY_TARGET_TYPE_GROUP				0x00000008 // 조직 그룹
#define SS_ENV_POLICY_TARGET_TYPE_ALL				(SS_ENV_POLICY_TARGET_TYPE_HOST | SS_ENV_POLICY_TARGET_TYPE_GROUP | \
														SS_ENV_POLICY_TARGET_TYPE_USER | SS_ENV_POLICY_TARGET_TYPE_USER_GROUP)

#define SS_ENV_POLICY_TARGET_TYPE_LOGICAL_GROUP		0x00000010  // 가상 그룹

//-------------------------------------------------------------------

//env policy option
#define SS_ENV_POLICY_OPTION_FLAG_OBJECT_SUPPORT_PERSON				0x00000001 // 
#define SS_ENV_POLICY_OPTION_FLAG_SUBJECT_REGISTER_BY_USER			0x00010000 //  

//-------------------------------------------------------------------


#endif	//COM_DEFINE_DB_ENV_POLICY_H_938EF200_F60B_452A_BD30_A10E8507EDCC