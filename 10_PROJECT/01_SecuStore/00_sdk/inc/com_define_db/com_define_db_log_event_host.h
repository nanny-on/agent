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
#ifndef COM_DEFINE_DB_LOG_EVENT_HOST_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_LOG_EVENT_HOST_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------

#define SS_LOG_EVENT_HOST_SYNC_MODE_NONE					0
#define SS_LOG_EVENT_HOST_SYNC_MODE_AGT						0x00000001
#define SS_LOG_EVENT_HOST_SYNC_MODE_SVR						0x00000002
#define SS_LOG_EVENT_HOST_SYNC_MODE_ALL						(SS_LOG_EVENT_HOST_SYNC_MODE_AGT | SS_LOG_EVENT_HOST_SYNC_MODE_SVR)

//-------------------------------------------------------------------

enum
{
	SS_LOG_EVENT_HOST_DESC_KEY_START_TIME			= 1,
	SS_LOG_EVENT_HOST_DESC_KEY_DOC_DEL_CNT,
	SS_LOG_EVENT_HOST_DESC_KEY_PO_START_USED_UNIT_ID,
	SS_LOG_EVENT_HOST_DESC_KEY_BK_DEL_CNT,
	SS_LOG_EVENT_HOST_DESC_KEY_TYPE,
	SS_LOG_EVENT_HOST_DESC_KEY_OPTION,
	SS_LOG_EVENT_HOST_DESC_KEY_NPM_EC_WUA,
	SS_LOG_EVENT_HOST_DESC_KEY_END_TIME,
	SS_LOG_EVENT_HOST_DESC_KEY_PERIOD_ST,
	SS_LOG_EVENT_HOST_DESC_KEY_PERIOD_ED,
	SS_LOG_EVENT_HOST_DESC_KEY_RECOVERY_CNT,
};

//--------------------------------------------------------------------


#endif	//COM_DEFINE_DB_LOG_EVENT_H_938EF200_F60B_452A_BD30_A10E8507EDCC