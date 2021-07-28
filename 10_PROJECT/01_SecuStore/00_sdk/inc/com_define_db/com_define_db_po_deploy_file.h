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
#ifndef COM_DEFINE_DB_PO_DEPLOY_FILE_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DB_PO_DEPLOY_FILE_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//-------------------------------------------------------------------
//deploy file unit key type
#define SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_INFO				0x01 // 
#define SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_CHK					0x02 // 
#define SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_TAR					0x03 // 
#define SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_SCH					0x04 // 

//-------------------------------------------------------------------
//execute user type
#define SS_PO_DEPLOY_FILE_EXE_USER_TYPE_SYSTEM			0x00000000 // 
#define SS_PO_DEPLOY_FILE_EXE_USER_TYPE_USER			0x00000001 // 

//-------------------------------------------------------------------
//unit file type
#define SS_PO_DEPLOY_FILE_FILE_TYPE_SETUP				0x00000000 // 설치 파일
#define SS_PO_DEPLOY_FILE_FILE_TYPE_NORMAL				0x00000001 // 일반 파일
#define SS_PO_DEPLOY_FILE_FILE_TYPE_BATCH				0x00000002 // 배치 파일
#define SS_PO_DEPLOY_FILE_FILE_TYPE_NORMAL_EXE			0x00000003 // 일반 파일 & 실행 (대기 없음)

//-------------------------------------------------------------------
//unit file enc type
#define SS_PO_DEPLOY_FILE_FILE_ENC_TYPE_NONE			0x00000000 // 암호화 없음
#define SS_PO_DEPLOY_FILE_FILE_ENC_TYPE_DS				0x00000001 // 전자 서명 파일

//-------------------------------------------------------------------
//unit execute number type
#define SS_PO_DEPLOY_FILE_EXE_NUM_TYPE_SINGLE			0x00000000 // 한번만 배포/실행
#define SS_PO_DEPLOY_FILE_EXE_NUM_TYPE_MULTI			0x00000001 // 다중 배포/실행

//-------------------------------------------------------------------
//unit schedule type
#define SS_PO_DEPLOY_FILE_SCHEDULE_USED_MODE_DISABLE	0x00000000 // 사용 안함, 즉시 설치
#define SS_PO_DEPLOY_FILE_SCHEDULE_USED_MODE_ENABLE		0x00000001 // 스케줄 사용

//-------------------------------------------------------------------
// unit chk unit check area type
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_CHECK_SINGLE		0x000000000
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_CHECK_MULTI		0x000000001

//-------------------------------------------------------------------
//unit chk unit comp type
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_TYPE_INSTALL			0x00000000
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_TYPE_REGISTRY			0x00000001
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_TYPE_FILE_PATH		0x00000002
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_TYPE_FILE_VER			0x00000003
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_TYPE_PROCESS			0x00000004


//-------------------------------------------------------------------
//unit chk unit comp value type
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_VAL_TYPE_STRING		0x00000000
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_VAL_TYPE_NUMBER		0x00000001
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_VAL_TYPE_VERSION		0x00000002

//--------------------------------------------------------------------

//unit chk unit comp op type
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_EXIST			0x00000000
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_NOT_EXIST		0x00000001
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_LARGE			0x00000002
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_LESS			0x00000003
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_EQUAL			0x00000004
#define SS_PO_DEPLOY_FILE_CHECK_UNIT_COMP_OP_TYPE_NOT_EQUAL		0x00000005

//--------------------------------------------------------------------
#endif	//COM_DEFINE_DB_PO_DEPLOY_FILE_H_938EF200_F60B_452A_BD30_A10E8507EDCC