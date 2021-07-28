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
#ifndef COM_DEFINE_DB_ENV_NOTIFY_INFO_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_ENV_NOTIFY_INFO_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//option ext

//-------------------------------------------------------------------
//protect type
#define SS_ENV_NOTIFY_INFO_SHOW_TYPE_MSG_SCREEN_CENTER			0x00000000 // 
#define SS_ENV_NOTIFY_INFO_SHOW_TYPE_MSG_SCREEN_CUSTOM			0x00000001 // 
#define SS_ENV_NOTIFY_INFO_SHOW_TYPE_MSG_TRAY_NORMAL			0x00000010 // 
#define SS_ENV_NOTIFY_INFO_SHOW_TYPE_MSG_TRAY_CUSTOM			0x00000020 // 
#define SS_ENV_NOTIFY_INFO_SHOW_TYPE_INTRO_SCREEN_CENTER		0x00000100 // 
#define SS_ENV_NOTIFY_INFO_SHOW_TYPE_INTRO_SCREEN_CUSTOM		0x00000200 // 

//--------------------------------------------------------------------
//msg fmt type
#define SS_ENV_NOTIFY_INFO_MSG_FMT_TYPE_TEXT			0x00000000	
#define SS_ENV_NOTIFY_INFO_MSG_FMT_TYPE_HTML			0x00000001	
#define SS_ENV_NOTIFY_INFO_MSG_FMT_TYPE_HTML_URL		0x00000002
#define SS_ENV_NOTIFY_INFO_MSG_FMT_TYPE_MIME			0x00000003
#define SS_ENV_NOTIFY_INFO_MSG_FMT_TYPE_IMAGE			0x00000004

//doc delete notify type
#define SS_ENV_NOTIFY_INFO_DOC_DELETE_TYPE_DEFAULT		0x00000000
#define SS_ENV_NOTIFY_INFO_DOC_DELETE_TYPE_OPER			0x00000001
#define SS_ENV_NOTIFY_INFO_DOC_DELETE_TYPE_DELETE		0x00000002


#endif	//COM_DEFINE_DB_ENV_NOTIFY_INFO_H_938EF200_F60B_452A_BD30_A10E8507EDCC