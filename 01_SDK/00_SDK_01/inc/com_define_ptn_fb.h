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
 
#ifndef COM_DEFINE_PTN_FB_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_PTN_FB_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------

#define PTN_FB_PTN_TYPE_ASHIN					0

#define PTN_FB_PTN_RISK_UNKNOW					0x00000000
#define PTN_FB_PTN_RISK_YELLOW					0x00000001
#define PTN_FB_PTN_RISK_WHITE					0x00000002
#define PTN_FB_PTN_RISK_BLACK					0x00000004
#define PTN_FB_PTN_RISK_LOCAL					0x00000008
#define PTN_FB_PTN_RISK_EX						0x00000010
#define PTN_FB_PTN_RISK_SP						0x00000020


#define PTN_FB_SRC_TYPE_SYSTEM					0x00000001
#define PTN_FB_SRC_TYPE_APPLICATION				0x00000002

//--------------------------------------------------------------------

#define PTN_PROC_FILE_TYPE_FE_LWHITE				0x00000001
#define PTN_PROC_FILE_TYPE_FE_GWHITE				0x00000002
#define PTN_PROC_FILE_TYPE_FE_GBLACK				0x00000004
#define PTN_PROC_FILE_TYPE_IN_GWHITE				0x00000002
#define PTN_PROC_FILE_TYPE_IN_GBLACK				0x00000004

#define PTN_PROC_FILE_TYPE_IN_GW				0x00000010
#define PTN_PROC_FILE_TYPE_IN_GB				0x00000020

#define PTN_PROC_FILE_TYPE_NC_LW				0x00000100
#define PTN_PROC_FILE_TYPE_NC_GW				0x00000200
#define PTN_PROC_FILE_TYPE_NC_GB				0x00000400


//--------------------------------------------------------------------
#endif	//COM_DEFINE_PTN_FB_H_938EF200_F60B_452A_BD30_A10E8507EDCC
