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


#ifndef Com_Error_H
#define Com_Error_H

#define	ERR_SUCCESS											0	//	요청을 완료 하였습니다.
#define	ERR_UNKNOWN											1	//	요청을 알수 없는 이유로 실패 하였습니다.

#define	ERR_DBMS_INSERT_FAIL								11	//	DBMS 입력을 실패 하였습니다.
#define	ERR_DBMS_UPDATE_FAIL								12	//	DBMS 변경을 실패 하였습니다.
#define	ERR_DBMS_DELETE_FAIL								13	//	DBMS 삭제를 실패 하였습니다.
#define	ERR_DBMS_SELECT_FAIL								14	//	DBMS 검색을 실패 하였습니다.

#define	ERR_SOCKET_BIND_FAIL								21	//	통신 연결 대기를 실패 하였습니다.
#define	ERR_SOCKET_CONNECT_FAIL								22	//	통신 연결을 실패 하였습니다.
#define	ERR_SOCKET_SVR_TO_CLT_INVALID_PKT					23	//	정의 되지 않은 패킷이 전달 되었습니다.
#define	ERR_SOCKET_CLT_TO_SVR_INVALID_PKT					24	//	정의 되지 않은 패킷을 전송 하였습니다.
#define	ERR_SOCKET_DISCONNECT_UNKNOWN						25	//	알수 없는 이유로 통신이 종료 되었습니다.
#define	ERR_SOCKET_NOT_FIND_TARGET_FOR_SEND					26	//	전송 대상을 찾을 수 없습니다.
#define	ERR_SOCKET_TRANS_DATA_TIME_OVER						27	//	데이터 전송 시간이 초과 되었습니다.

#define	ERR_AUTH_ACCOUNT_INFO_INVALID						31	//	사용자 정보가 잘못 되었습니다.
#define	ERR_AUTH_ACCOUNT_INFO_LOCK							32	//	사용자 정보가 잠김 상태 입니다.
#define	ERR_AUTH_ACCOUNT_INFO_DENY_IP						33	//	사용자 접근 주소가 차단되었습니다.
#define	ERR_AUTH_INVALID_REQUEST_STEP						34	//	잘못된 인증 순서가 감지되었습니다.
#define	ERR_AUTH_INVALID_BIN_VERSION						35	//	잘못된 모듈 버전입니다.
#define ERR_AUTH_INVALID_OTP_CODE							36	//  잘못된 OTP 코드 입니다.
#define ERR_AUTH_INVALID_OTP_ENV							37	//  잘못된 OTP 환경 입니다.
#define ERR_AUTH_INVALID_OTP_SESSION_TIME					38	//	OTP 코드 입력 시간이 초과 되었습니다.
#define ERR_AUTH_INVALID_CHANGE_ENV_CONFIRM_PW				39	//	요청 사용자 암호가 일치하지 않습니다.
#define ERR_AUTH_INVALID_PREVIOUSLY_USED_PASSWORD			40	//	기존에 사용한 암호는 사용할수 없습니다.

#define ERR_INFO_NOT_OP_BECAUSE_NOT_EXISTS_RIGHT			40	//	권한이 없어서 정보를 관리 할수 없습니다.
#define ERR_INFO_NOT_OP_BECAUSE_ADMIN_LOW_LEVEL				41	//	관리자 레벨이 낮아서 정보를 관리 할수 없습니다.
#define	ERR_INFO_NOT_ADD_BECAUSE_EXIST_NAME					42	//	이미 동일한 이름이 존재하여 추가 할 수 없습니다.
#define	ERR_INFO_NOT_ADD_BECAUSE_EXIST_INFO					43	//	이미 동일한 정보가 존재하여 추가 할 수 없습니다.
#define	ERR_INFO_NOT_ADD_BECAUSE_PRE_CHECK_FAIL				44	//	사전 검사를 실패하여 추가 할 수 없습니다.
#define	ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND					51	//	정보가 존재하지 않아 정보 수정 요청을 진행 할 수 없습니다.
#define	ERR_INFO_NOT_EDIT_BECAUSE_EXIST_INFO				52	//	이미 동일한 정보가 존재하여 수정 할 수 없습니다.
#define	ERR_INFO_NOT_EDIT_BECAUSE_SAME_OLD_INFO				53	//	기존 정보와 동일하여 수정 할 수 없습니다.
#define	ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND				61	//	정보가 존재하지 않아 정보 삭제 요청을 진행 할 수 없습니다.
#define	ERR_INFO_NOT_DELETE_BECAUSE_EXIST_SUB_INFO			62	//	하위 정보가 존재하여 요청 정보를 삭제 할 수 없습니다.
#define	ERR_INFO_NOT_OP_BECAUSE_NOT_FIND					71	//	정보를 찾을 수 없어 요청을 진행 할 수 없습니다.
#define	ERR_INFO_NOT_OP_BECAUSE_INVALID_SET_ENV				72	//	설정 정보가 유효하지 않아 요청을 진행 할 수 없습니다.

#define	ERR_SYS_NOT_SUPPORT_SYSTEM							100	//	지원되지 않는 시스템 입니다.
#define	ERR_SYS_FILE_NOT_FIND								101	//	파일을 찾을 수 없습니다.
#define	ERR_SYS_FILE_NOT_DELETE								102	//	파일을 삭제 할 수 없습니다.
#define	ERR_SYS_FILE_NOT_EXECUTE							103	//	파일을 실행 할 수 없습니다.
#define	ERR_SYS_FILE_NOT_CREATE								104	//	파일 생성을 실패 하였습니다.
#define	ERR_SYS_FILE_NOT_COPY								105	//	파일 복사를 실패 하였습니다.
#define	ERR_SYS_FILE_INVALID_HASH							106	//	파일의 무결성 정보가 일치하지 않습니다.
#define	ERR_SYS_FILE_NOT_DOWNLOAD							107	//	파일 다운로드를 실패 하였습니다.
#define ERR_SYS_FILE_INSTALL_ING							108	//	파일 설치가 진행중 입니다.
#define ERR_SYS_FILE_NOT_FIND_CODESIGN						109	//	파일 코드 사인 정보를 찾을수 없습니다.

#define ERR_ENV_STATUS_IS_NOT_LOG_ON						200	//	로그인 상태가 아니므로 진행 할 수 없습니다.

#define ERR_ENV_SORG_MAKE_GROUP_FAIL						400	//  조직 연동 그룹 정보 생성을 실패 하였습니다.
#define ERR_ENV_SORG_MAKE_USER_FAIL							401	//  조직 연동 사용자 정보 생성을 실패 하였습니다.
#define ERR_ENV_SORG_MAKE_USER_PC_FAIL						402	//  조직 연동 사용자 PC 정보 생성을 실패 하였습니다.
#define ERR_ENV_SORG_SYNC_GROUP_FAIL						403	//  조직 연동 그룹 정보 동기화를 실패 하였습니다.
#define ERR_ENV_SORG_SYNC_USER_FAIL							404	//  조직 연동 사용자 정보 동기화를 실패 하였습니다.

#define	ERR_POLICY_NOT_APPLY_BECAUSE_ADMIN_LEVEL_LOW		1000	//	관리자 권한이 낮아 정책 설정을 실패 하였습니다.
#define	ERR_POLICY_NOT_APPLY_BECAUSE_HIGH_ADMIN_LOCK		1001	//	상위 관리자가 적용 대상에 대하여 잠금 설정하였습니다.
#define	ERR_POLICY_NOT_APPLY_BECAUSE_TARGET_EMPTY			1002	//	정책 적용 대상이 존재하지 않습니다.




#endif