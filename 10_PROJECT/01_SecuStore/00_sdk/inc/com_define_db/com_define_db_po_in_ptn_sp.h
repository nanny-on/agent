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
#ifndef COM_DEFINE_DB_PO_IN_PTN_SP_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_IN_PTN_SP_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//block type
#define SS_PO_IN_PTN_SP_TARGET_TYPE_HOST						0
#define SS_PO_IN_PTN_SP_TARGET_TYPE_USER						1

#define SS_PO_IN_PTN_SP_REQ_LEVEL_NONE							0
#define SS_PO_IN_PTN_SP_REQ_LEVEL_NORMAL						1
#define SS_PO_IN_PTN_SP_REQ_LEVEL_HIGH							10
#define SS_PO_IN_PTN_SP_REQ_LEVEL_URGENCY						100

#define SS_PO_IN_PTN_SP_BLOCK_TYPE_ALLOW						0
#define SS_PO_IN_PTN_SP_BLOCK_TYPE_DENY							1

#define SS_PO_IN_PTN_SP_RULE_CONFIRM_TYPE_WAIT					0
#define SS_PO_IN_PTN_SP_RULE_CONFIRM_TYPE_ADMIN					1
#define SS_PO_IN_PTN_SP_RULE_CONFIRM_TYPE_POLICY				2
#define SS_PO_IN_PTN_SP_RULE_CONFIRM_TYPE_POLICY_UNIT			3
#define SS_PO_IN_PTN_SP_RULE_CONFIRM_TYPE_AGENT_POLICY			10
#define SS_PO_IN_PTN_SP_RULE_CONFIRM_TYPE_AGENT_POLICY_UNIT		11

#define SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_ALLOW				0
#define SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_DENY				1
#define SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_WAIT				10
#define SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_END				20
#define SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_END_FOREVER		40

#define SS_PO_IN_PTN_SP_CONFIRM_DAY_UNLIMITED					0

#define SS_PO_IN_PTN_SP_EXT_OPTION_RULE_CHECK_AGENT				0x00000001
#define SS_PO_IN_PTN_SP_EXT_OPTION_PATH_BASED_INSPECTION		0x00000002

#define SS_PO_IN_PTN_SP_RULE_LOCAL_START_ID						1000000000

//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_PO_IN_PTN_SP_H_938EF200_F60B_452A_BD30_A10E8507EDCC