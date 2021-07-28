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
#ifndef COM_DEFINE_DB_ENV_LOG_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_ENV_LOG_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//option ext

//-------------------------------------------------------------------
//record mode
#define SS_ENV_LOG_RECORD_FLAG_MONITOR				0x00000001 // 
#define SS_ENV_LOG_RECORD_FLAG_DBMS					0x00000002 // 
#define SS_ENV_LOG_RECORD_FLAG_SMS					0x00000004 //
//-------------------------------------------------------------------

#define SS_ENV_LOG_TYPE_ALL							0x00000000
#define SS_ENV_LOG_TYPE_EVENT						0x00000001
#define SS_ENV_LOG_TYPE_DOC							0x00000002
#define SS_ENV_LOG_TYPE_SECU						0x00000004
#define SS_ENV_LOG_TYPE_DEPLOY_FILE					0x00000008
#define SS_ENV_LOG_TYPE_EVENT_HOST					0x00000010
#define SS_ENV_LOG_TYPE_PATCH						0x00000020
#define SS_ENV_LOG_TYPE_DEVICE						0x00000040
#define SS_ENV_LOG_TYPE_RS							0x00000080
//-------------------------------------------------------------------

#define SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT				0x00000001 //ExtOption ..
#define SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER			0x00000002
#define SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER_SELF_LOG	0x00000004

//-------------------------------------------------------------------

#define SS_ENV_LOG_BACKUP_ENABLE					0x00000010
#define SS_ENV_LOG_BACKUP_RESETLOG					0x00000020
//-------------------------------------------------------------------

#define SS_ENV_LOG_OPTION_LINKLOG_DOCLOG			0x00000040
#define SS_ENV_LOG_OPTION_LINKLOG_SECLOG			0x00000080
#define SS_ENV_LOG_OPTION_LINKLOG_RSLOG				0x00000100

#define SS_ENV_LOG_ORGSAVE_ENABLE					0x00000200	//ExtOption End

#define SS_ENV_LOG_BACKUPWAY_LOGCNT					0x00000000
#define SS_ENV_LOG_BACKUPWAY_TERM					0x00000001

enum _ENUM_SS_ENV_LOG_INDEX
{
	SS_ENV_LOG_INDEX_EVENT = 0,
	SS_ENV_LOG_INDEX_DOC,
	SS_ENV_LOG_INDEX_SECU,
	SS_ENV_LOG_INDEX_DEPLOY_FILE,
	SS_ENV_LOG_INDEX_DOC_DSCAN,
	SS_ENV_LOG_INDEX_EVENT_HOST,
	SS_ENV_LOG_INDEX_PATCH,
	SS_ENV_LOG_INDEX_DEVICE,
	SS_ENV_LOG_INDEX_RS,
	SS_ENV_LOG_INDEX_TOTAL_NUMBER,
};
//-------------------------------------------------------------------

#define SS_ENV_LOG_LOAD_MODE_TYPE_DAY				0
#define SS_ENV_LOG_LOAD_MODE_TYPE_COUNT				1
//-------------------------------------------------------------------
#define SS_ENV_LOG_ORGDLG_MODE_SAVE					1

//-------------------------------------------------------------------

enum _ENUM_SS_ENV_LOG_EXT_INFO_TYPE_DP
{
	SS_ENV_LOG_EXT_INFO_TYPE_DP_NON = 0,
	SS_ENV_LOG_EXT_INFO_TYPE_DP_END_REASON,
	SS_ENV_LOG_EXT_INFO_TYPE_DP_END_TIME,
};

enum _ENUM_SS_ENV_LOG_EXT_INFO_TYPE_DV
{
	SS_ENV_LOG_EXT_INFO_TYPE_DV_NON = 0,
	SS_ENV_LOG_EXT_INFO_TYPE_DV_BLOCK_TYPE,
	SS_ENV_LOG_EXT_INFO_TYPE_DV_DV_INST_ID,
};

enum _ENUM_SS_ENV_LOG_EXT_INFO_TYPE_PATCH
{
	SS_ENV_LOG_EXT_INFO_TYPE_PATCH_NON = 0,
	SS_ENV_LOG_EXT_INFO_TYPE_PATCH_ID,
	SS_ENV_LOG_EXT_INFO_TYPE_PATCH_STATUS,
	SS_ENV_LOG_EXT_INFO_TYPE_PATCH_TYPE,
};

enum _ENUM_SS_ENV_LOG_EXT_INFO_TYPE_RS
{
	SS_ENV_LOG_EXT_INFO_TYPE_RS_NON = 0,
	SS_ENV_LOG_EXT_INFO_TYPE_RS_BLOCK_TYPE,
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


#endif	//COM_DEFINE_DB_ENV_LOG_H_938EF200_F60B_452A_BD30_A10E8507EDCC