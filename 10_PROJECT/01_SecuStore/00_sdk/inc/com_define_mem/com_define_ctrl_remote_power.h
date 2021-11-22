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
 * 
 */


#ifndef COM_DEFINE_COMMON_REMOTE_POWER_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_COMMON_REMOTE_POWER_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
// agent alert option
#define SS_COMMON_REMOTE_POWER_ALERT_TYPE_UNUSED						0x00000000 // 사용자 알림 메시지 사용안함
#define SS_COMMON_REMOTE_POWER_ALERT_TYPE_USED							0x00000001 // 사용자 알림 메시지 사용함

#define SS_COMMON_REMOTE_POWER_MODE_SYSTEM_TURN_ON						0x00000001 //시스템 켜기
#define	SS_COMMON_REMOTE_POWER_MODE_SYSTEM_TURN_OFF						0x00000002 //시스템 끄기
#define	SS_COMMON_REMOTE_POWER_MODE_SYSTEM_RESTART						0x00000003 //시스템 끄기

#define	SS_COMMON_REMOTE_POWER_MODE_MONITOR_TURN_ON						0x00000010 //모니터 켜기
#define	SS_COMMON_REMOTE_POWER_MODE_MONITOR_TURN_OFF					0x00000011 //모니터 끄기[nCtlWaitTime]

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//--------------------------------------------------------------------
#endif	//COM_DEFINE_COMMON_REMOTE_POWER_H_938EF200_F60B_452A_BD30_A10E8507EDCC