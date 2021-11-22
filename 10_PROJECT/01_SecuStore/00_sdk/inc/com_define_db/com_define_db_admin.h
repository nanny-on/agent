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

#ifndef COM_DEFINE_DB_ADMIN_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_ADMIN_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//env column class number

#define SS_ADMIN_OPTION_ALERT_SMS				0x00000001
#define SS_ADMIN_OPTION_OTP_LOGIN				0x00000002

#define SS_ADMIN_OPTION_OTP_LOGIN_CHK_BK_INFO	0x00000004

//-------------------------------------------------------------------
//env admin console login mode

#define SS_CONSOLE_ADMIN_LOGIN_UNKNOWN_MODE			0x00000000
#define SS_CONSOLE_ADMIN_LOGIN_LITE_MODE			0x00000001
#define SS_CONSOLE_ADMIN_LOGIN_FULL_MODE			0x00000002

//-------------------------------------------------------------------

#define SS_ADMIN_ENV_MN_OPTION_USED_ROTATION			0x00000001

//-------------------------------------------------------------------

#define SS_ADMIN_ENV_MN_LOAD_DATA_PERIOD_RT				0
#define SS_ADMIN_ENV_MN_LOAD_DATA_PERIOD_DAY			7
#define SS_ADMIN_ENV_MN_LOAD_DATA_PERIOD_WEEK			35
#define SS_ADMIN_ENV_MN_LOAD_DATA_PERIOD_MONTH			190

//-------------------------------------------------------------------

#define SS_ADMIN_ENV_MN_PAGE_OPTION_USED_ROTATION		0x00000001

//-------------------------------------------------------------------

#define SS_ADMIN_MAX_TRY_CERT_CNT		3

#endif	//COM_DEFINE_DB_ADMIN_H_938EF200_F60B_452A_BD30_A10E8507EDCC