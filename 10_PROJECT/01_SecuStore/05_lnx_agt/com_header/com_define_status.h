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
 * 
 */


#ifndef COM_DEFINE_STATUS_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_STATUS_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//host used status
#define USED_FLAG_TYPE_DELETE			0
#define	USED_FLAG_TYPE_USED				1
#define	USED_FLAG_TYPE_SLEEP			100
#define USED_FLAG_TYPE_UNINSTALL		200

//-------------------------------------------------------------------
//used status
#define STATUS_USED_MODE_OFF			0
#define STATUS_USED_MODE_ON				1
#define STATUS_USED_MODE_WARN			2
#define STATUS_USED_MODE_ONCE			3

//--------------------------------------------------------------------
//match filter check type
#define SS_MATCH_FILTER_CHK_TYPE_AND	0
#define SS_MATCH_FILTER_CHK_TYPE_OR		1

//--------------------------------------------------------------------
//status
#define STATUS_MODE_OFF					0
#define STATUS_MODE_ON					1
#define STATUS_MODE_WARN				2

//--------------------------------------------------------------------
//web open type
#define WEB_SVR_OPEN_TYPE_UPDATE					0x00000001
#define WEB_SVR_OPEN_TYPE_INSTALL					0x00000002
#define WEB_SVR_OPEN_TYPE_ALL         				WEB_SVR_OPEN_TYPE_UPDATE | WEB_SVR_OPEN_TYPE_INSTALL

//--------------------------------------------------------------------
//web info mode
#define	WEB_SVR_INFO_MODE_REG						0
#define WEB_SVR_INFO_MODE_FILE						1

//--------------------------------------------------------------------
//schedule period type
#define SCHEDULE_PERIOD_TYPE_AUTO				0
#define SCHEDULE_PERIOD_TYPE_BOOT				1
#define SCHEDULE_PERIOD_TYPE_LOGIN				2
#define SCHEDULE_PERIOD_TYPE_LOGOFF				3
#define SCHEDULE_PERIOD_TYPE_SHUTDOWN			4
#define SCHEDULE_PERIOD_TYPE_POWERSAVE_WAKE		5
#define SCHEDULE_PERIOD_TYPE_SHUTDOWN_EXT		6
#define SCHEDULE_PERIOD_TYPE_MOUNT_ENCRYPTFS	7
#define SCHEDULE_PERIOD_TYPE_ONCE				10
#define SCHEDULE_PERIOD_TYPE_MIN				11
#define SCHEDULE_PERIOD_TYPE_HOUR				12
#define SCHEDULE_PERIOD_TYPE_DAY				13
#define SCHEDULE_PERIOD_TYPE_WEEK				14
#define SCHEDULE_PERIOD_TYPE_MONTH				15
#define SCHEDULE_PERIOD_TYPE_DEMAND				20
#define SCHEDULE_PERIOD_TYPE_FIX_HOUR			30
#define SCHEDULE_PERIOD_TYPE_FIX_DAY			31
#define SCHEDULE_PERIOD_TYPE_MON_WEEK_DOW		32

#define SCHEDULE_PERIOD_TYPE_WEEK_EXT			45
#define SCHEDULE_PERIOD_TYPE_MONTH_EXT			46

#define SCHEDULE_PERIOD_TYPE_MON_WEEK_DOW_EXT	50

#define SCHEDULE_PERIOD_TYPE_PERIOD_MWW			100
//--------------------------------------------------------------------
//
#define MPI_PO_VULN_RESULT_TYPE_NONE				0x00000000
#define MPI_PO_VULN_RESULT_TYPE_SAFE				0x00000001
#define MPI_PO_VULN_RESULT_TYPE_CHK_FAIL			0x00000002
#define MPI_PO_VULN_RESULT_TYPE_DANGER				0x00000003
//--------------------------------------------------------------------

#define FILE_CHK_TYPE_CREATE_DT				0	// 생성 시간 기준
#define FILE_CHK_TYPE_MODIFY_DT				1	// 수정 시간 기준
#define FILE_CHK_TYPE_ACCESS_DT				2	// 접근 시간 기준

//--------------------------------------------------------------------

#define INJECT_FLAG_NONE				0x00000000
#define INJECT_FLAG_SHUTDOWN_FA_OP		0x00000001
#define INJECT_FLAG_SHUTDOWN_FA_CLEAR	0x00000002

#define INJECT_FLAG_BOOT_FA_OP			0x00000004
#define INJECT_FLAG_BOOT_FA_CLEAR		0x00000008

#define INJECT_FLAG_SHUTDOWN			(INJECT_FLAG_SHUTDOWN_FA_OP | INJECT_FLAG_SHUTDOWN_FA_CLEAR)
#define INJECT_FLAG_BOOT				(INJECT_FLAG_BOOT_FA_OP | INJECT_FLAG_BOOT_FA_CLEAR)

//--------------------------------------------------------------------

#endif	//COM_DEFINE_COMMON_H_938EF200_F60B_452A_BD30_A10E8507EDCC