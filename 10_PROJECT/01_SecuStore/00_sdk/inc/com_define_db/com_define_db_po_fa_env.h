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
#ifndef COM_DEFINE_DB_PO_FA_ENV_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_FA_ENV_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//fa env file ptn type
#define SS_PO_FA_ENV_PTN_TYPE_EXTENSION								0 // 
#define SS_PO_FA_ENV_PTN_TYPE_OLE									1 // 

//--------------------------------------------------------------------

#define SS_PO_FA_ENV_OPTION_FLAG_SHOW_APPLY_POLICY_MENU_TO_AGENT	0x00000001 // 에이전트 매니저 현재 에이전트에 적용 된 정책 보이기(메뉴를 세분화하여 SHOW/HIDE 있도록 기능 고도화 예정)
#define SS_PO_FA_ENV_OPTION_FLAG_SHOW_EXTRA_LOG_MENU_TO_AGENT		0x00000002 // 에이전트 매니저 NANNY-I 로그 외에 파일배포 로그와 수동삭제 로그를 추가로 표시(로그탭을 세분화하여 SHOW/HIDE 있도록 기능 고도화 예정)
#define SS_PO_FA_ENV_OPTION_FLAG_SHOW_USER_DELETE_TO_AGENT			0x00000004 // 에이전트 매니저 수동삭제 옵션

//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_PO_FA_ENV_H_938EF200_F60B_452A_BD30_A10E8507EDCC