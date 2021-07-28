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
#ifndef COM_DEFINE_DB_PO_CTL_MEDIA_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_CTL_MEDIA_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//default mode
#define SS_PO_CTL_MEDIA_DEFAULT_MODE_ALLOW					0x00000000 // 
#define SS_PO_CTL_MEDIA_DEFAULT_MODE_DENY					0x00000001 // 
#define SS_PO_CTL_MEDIA_DEFAULT_MODE_USER_CONFIRM			0x00000002 // 

//-------------------------------------------------------------------
//access type
#define SS_PO_CTL_MEDIA_ACCESS_FLAG_READ				0x0001
#define SS_PO_CTL_MEDIA_ACCESS_FLAG_WRITE				0x0002
#define SS_PO_CTL_CD_ACCESS_FLAG_READ					0x0010
#define SS_PO_CTL_CD_ACCESS_FLAG_WRITE					0x0020

//-------------------------------------------------------------------
//log type
#define SS_PO_CTL_MEDIA_LOG_TYPE_ALLOW					0
#define SS_PO_CTL_MEDIA_LOG_TYPE_DENY					1
#define SS_PO_CTL_MEDIA_LOG_TYPE_PNP					2

//-------------------------------------------------------------------
//log type
#define SS_PO_CTL_MEDIA_LOG_MODE_TYPE_FILE_EXEC			0x00000001
#define	SS_PO_CTL_MEDIA_LOG_MODE_TYPE_FILE_WRITE		0x00000002
#define	SS_PO_CTL_MEDIA_LOG_MODE_TYPE_FILE_READ			0x00000004
#define	SS_PO_CTL_MEDIA_LOG_MODE_TYPE_FILE_RENAME		0x00000008
#define	SS_PO_CTL_MEDIA_LOG_MODE_TYPE_FILE_DELETE		0x00000010
#define	SS_PO_CTL_MEDIA_LOG_MODE_TYPE_FILE_CREATE		0x00000020
#define	SS_PO_CTL_MEDIA_LOG_MODE_TYPE_FILE_RENAME_EXT	0x00000800

#define	SS_PO_CTL_MEDIA_LOG_MODE_TYPE_MD_ATTACH			0x10000000
#define	SS_PO_CTL_MEDIA_LOG_MODE_TYPE_MD_DETACH			0x20000000

//-------------------------------------------------------------------
//option
#define SS_PO_CTL_MEDIA_EXT_OPTION_TYPE_RESET_EXPLORER	0x00000001

//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_PO_CTL_MEDIA_H_938EF200_F60B_452A_BD30_A10E8507EDCC