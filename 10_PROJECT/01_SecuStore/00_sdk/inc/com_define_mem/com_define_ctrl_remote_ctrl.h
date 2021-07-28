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


#ifndef COM_DEFINE_COMMON_REMOTE_CTRL_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_COMMON_REMOTE_CTRL_H_938EF200_F60B_452A_BD30_A10E8507EDCC

// 자세한 정보는 아래의 위키 페이지를 참조하시기 바랍니다.
// [PoRemoteRequest]
// http://mw.ashindev.com/wiki/index.php/CtlRemoteRequest

//-------------------------------------------------------------------
// agent alert option
#define SS_COMMON_REMOTE_CTRL_ALERT_TYPE_UNUSED							0x00000000 // 사용자 알림 메시지 사용안함
#define SS_COMMON_REMOTE_CTRL_ALERT_TYPE_USED							0x00000001 // 사용자 알림 메시지 사용함 

//-------------------------------------------------------------------
// remote connect mode
#define SS_COMMON_REMOTE_CTRL_CONNECT_MODE_GDI							0x00000000 // DC를 이용한 Bitmap Cache(GDI) - 미 설정 시 기본값
#define SS_COMMON_REMOTE_CTRL_CONNECT_MODE_DRIVER						0x00000001 // 커널 드라이버 Hook 모드 (이 모드는 현재 지원하지 않습니다.) 

//-------------------------------------------------------------------
// remote view only mode
#define SS_COMMON_REMOTE_CTRL_VIEW_ONLY_USED							0x00000000 // 원격 화면을 단순히 보기로만 사용
#define SS_COMMON_REMOTE_CTRL_VIEW_ONLY_UNUSED							0x00000001 // 원격 화면 뷰전용 모드 사용안함 (마우스, 키보드 입력을 통한 원격지 PC 제어 가능) 

//-------------------------------------------------------------------
#define SS_COMMON_REMOTE_CTRL_LOCALE_DEFALUT							0x00000000 // 클라이언트 윈도우즈 로케일 설정 값 (미설정 시 기본값 DEFAULT)
#define SS_COMMON_REMOTE_CTRL_LOCALE_KOR								0x00000001 // 한글
#define SS_COMMON_REMOTE_CTRL_LOCALE_ENG								0x00000002 // 영문
#define SS_COMMON_REMOTE_CTRL_LOCALE_JPN								0x00000003 // 일본
#define SS_COMMON_REMOTE_CTRL_LOCALE_CHS								0x00000004 // 중국어 간체
#define SS_COMMON_REMOTE_CTRL_LOCALE_CHT								0x00000005 // 중국어 번체 
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//--------------------------------------------------------------------
#endif	//COM_DEFINE_COMMON_REMOTE_CTRL_H_938EF200_F60B_452A_BD30_A10E8507EDCC