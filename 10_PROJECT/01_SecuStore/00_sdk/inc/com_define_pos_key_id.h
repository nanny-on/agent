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
#ifndef COM_DEFINE_POS_KEY_ID_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_POS_KEY_ID_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//--------------------------------------------------------------------
//
#define POS_KEY_ID_POSITION_HOST_USERID_HID_MAP					0x00000001
//--------------------------------------------------------------------

#define POS_KEY_STR_POSITION_HOST_USER_UNICODEID_HID_MAP		0x00000001
//--------------------------------------------------------------------


#define POS_KEY_ID_POSITION_SITE_FILE_REF_HID_MAP		0x00000001
#define POS_KEY_ID_POSITION_SITE_FILE_REF_FID_MAP		0x00000002
//--------------------------------------------------------------------

#define POS_KEY_ID_POSITION_PTN_FILE_PO_FID_MAP			0x00000001
#define POS_KEY_ID_POSITION_PTN_FILE_PO_UID_MAP			0x00000002
//--------------------------------------------------------------------

#define POS_KEY_STR_POSITION_PTN_PATCH_GUID_MAP			0x00000001
//--------------------------------------------------------------------

#define POS_KEY_ID_POSITION_LOG_PATCH_SID_MAP			0x00000001
#define POS_KEY_ID_POSITION_LOG_PATCH_PID_MAP			0x00000002
//--------------------------------------------------------------------

#define POS_KEY_STR_POSITION_LOG_RS_BK_DIR_MAP			0x00000001
#define POS_KEY_STR_POSITION_LOG_RS_BK_DIR_FILE_MAP		0x00000002
#define POS_KEY_STR_POSITION_LOG_RS_BK_FILE_EXT_MAP		0x00000003
//--------------------------------------------------------------------

#define POS_KEY_ID_POSITION_GROUP_CATEGORY_CID_MAP		0x00000001
#define POS_KEY_ID_POSITION_GROUP_CATEGORY_GID_MAP		0x00000002
#define POS_KEY_ID_POSITION_GROUP_CATEGORY_CGID_MAP		0x00000003
//--------------------------------------------------------------------

#define POS_KEY_ID_POSITION_SITE_SW_HID_MAP				0x00000001
#define POS_KEY_ID_POSITION_SITE_SW_SID_MAP				0x00000002
//--------------------------------------------------------------------

#define POS_KEY_ID_POSITION_LOG_POLICY					0x00000001
#define POS_KEY_ID_POSITION_LOG_EVT_HOUR				0x00000002
#define POS_KEY_ID_POSITION_LOG_EVT_DAY					0x00000003
#define POS_KEY_ID_POSITION_LOG_EVT_WEEK				0x00000004
#define POS_KEY_ID_POSITION_LOG_EVT_MON					0x00000005
//--------------------------------------------------------------------

#define POS_KEY_STR_POSITION_PTN_PROC_FILE_DIR_MAP			0x00000001
#define POS_KEY_STR_POSITION_PTN_PROC_FILE_DIR_FILE_MAP		0x00000002
#define POS_KEY_STR_POSITION_PTN_PROC_FILE_FILE_EXT_MAP		0x00000003
//--------------------------------------------------------------------

#define POS_KEY_STR_POSITION_SP_RULE_FE_KEY					0x00000001
#define POS_KEY_STR_POSITION_SP_RULE_FE_FILE_PATH_KEY		0x00000002

//--------------------------------------------------------------------

#define POS_KEY_STR_POSITION_EX_TRUST_SUB_MAP				0x00000001
//--------------------------------------------------------------------



#define POS_KEY_ID_USER_UNICODE						1
#define POS_KEY_ID_USER_ACCOUNT_ID					2

//--------------------------------------------------------------------

#define POS_KEY_STR_POSITION_SITE_SW_KEY_MAP			0x00000001
#define POS_KEY_STR_POSITION_SITE_SW_KEY_VER_MAP		0x00000002

//--------------------------------------------------------------------

enum
{
	POS_KEY_ID_MON_CHART_DATA_REALTIME		= 0,
	POS_KEY_ID_MON_CHART_DATA_HOUR,
	POS_KEY_ID_MON_CHART_DATA_DAY,
	POS_KEY_ID_MON_CHART_DATA_WEEK,
	POS_KEY_ID_MON_CHART_DATA_MONTH,
	POS_KEY_ID_MON_CHART_DATA_FOREIGN_KEY,
};


//--------------------------------------------------------------------
#endif	//COM_DEFINE_POS_KEY_ID_H_938EF200_F60B_452A_BD30_A10E8507EDCC