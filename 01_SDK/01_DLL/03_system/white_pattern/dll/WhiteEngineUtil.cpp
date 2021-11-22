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

#include "stdafx.h"
#include "com_struct.h"
#include "WhiteListDef.h"
#include "AriaEx.h"
#include "WhiteEngineUtil.h"


CWhiteEngineUtil::CWhiteEngineUtil()
{

}

CWhiteEngineUtil::~CWhiteEngineUtil()
{

}

/**************************************************************************************************************
	함수명 :  we_init_db
	Input :
	Output :
		char *acLogMsg : 에러 메시지
	RetVal : INT32
			0 : Hash Table 메모리 초기화 성공
			FALSE : Hash Table 메모리 초기화 실패
	Information :
		해쉬 테이블 메모리를 초기화 하는 함수, 기능 변경 없음
**************************************************************************************************************/
INT32 CWhiteEngineUtil::InitDB(char *acLogMsg)
{
	INT32 nRetVal = 0;
	nRetVal = m_tWPtnUtil.m_tHashUtil.InitHash(acLogMsg);
	return nRetVal;
}

/**************************************************************************************************************
	함수명 :  we_load_db
	Input :
		char *acEnginePath : 패턴 파일이 포함되어 있는 경로 (wdb 파일은 모두 로딩)
	Output :
		char *acLogMsg : 에러 메시지
	RetVal : INT32
			0 : 패턴 파일 메모리 로딩 성공
			FALSE : 패턴 파일 메모리 로딩 실패
	Information :
		패턴 파일에 들어가있는 패턴 리스트를 메모리에 로딩하는 함수, we_load_white_list 로 대체 되었음
**************************************************************************************************************/
INT32 CWhiteEngineUtil::LoadDB(char *acEnginePath, char *acLogMsg)
{
	INT32 nRetVal = 0;
	nRetVal = m_tWPtnUtil.we_pattern_load(acEnginePath, acLogMsg);
	return nRetVal;
}

/**************************************************************************************************************
	함수명 :  we_create_db
	Input :
		char *acWhitePath : 화이트리스트로 추가할 파일이 포함되어 있는 경로 (하위 경로 모두 검색하여 리스트에 추가)
		char *acEnginePath : 패턴 파일이 생성될 경로
		char *acDbName : 패턴 파일명 (확장자 제외)
	Output :
		char *acLogMsg : 에러 메시지
	RetVal : INT32
			0 : 패턴 파일 생성 성공
			FALSE : 패턴 파일 생성 실패
	Information :
		PE파일 검색하여 메모리 로딩 후 입력된 파일명으로 패턴 파일 생성, 권장하지 않는 방식 we_make_wdb 권고
**************************************************************************************************************/

INT32 CWhiteEngineUtil::CreateDB(char *acWhitePath, char *acEnginePath, char *acDbName, char *acLogMsg)
{
	INT32 nRetVal = 0;
	nRetVal = m_tWPtnUtil.we_pattern_create(acWhitePath, acEnginePath, acDbName, acLogMsg);
	return nRetVal;
}

/**************************************************************************************************************
	함수명 :  we_add_file
	Input :
		char *szPath : 화이트리스트로 추가할 파일 경로
	Output :
		char *acLogMsg : 에러 메시지
	RetVal : INT32
			0 : 패턴 추가 성공
			FALSE : 패턴 추가 실패
	Information :
		입력된 PE파일에 대한 정보 추출 후 리스트에 추가, we_add_white_list로 변경
**************************************************************************************************************/

INT32 CWhiteEngineUtil::AddFile(char *szPath, char *acLogMsg)
{
	return m_tWPtnUtil.we_fadd(szPath, acLogMsg);
}

/**************************************************************************************************************
	함수명 :  we_del_file
	Input :
		char *szPath : 화이트리스트에서 삭제할 파일 경로
	Output :
		char *acLogMsg : 에러 메시지
	RetVal : INT32
			0 : 패턴 삭제 성공
			FALSE : 패턴 삭제 실패
	Information :
		입력된 PE파일에 대한 정보 추출 후 동일한 해쉬를 가지고 있는 패턴을 리스트에서 삭제, we_del_white_list로 변경
**************************************************************************************************************/

INT32 CWhiteEngineUtil::DelFile(char *szPath, char *acLogMsg)
{
	return m_tWPtnUtil.we_fdel(szPath, acLogMsg);;
}

/**************************************************************************************************************
	함수명 :  we_clear_file
	Input :
	Output :
	RetVal :
	Information :
		전체 화이트 리스트 메모리 해제
**************************************************************************************************************/

void CWhiteEngineUtil::ClearFile()
{
	m_tWPtnUtil.we_fclear();
}

/**************************************************************************************************************
	함수명 :  we_make_db
	Input :
		char *acEnginePath : 패턴 파일이 생성될 경로
		char *acDbName : 패턴 파일명 (확장자 제외)
	Output :
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 패턴 파일 생성 성공
		FALSE : 패턴 파일 생성 실패
	Information :
		메모리에 있는 전체 리스트를 입력된 파일명으로 패턴 파일 생성, we_make_wdb로 변경
**************************************************************************************************************/

INT32 CWhiteEngineUtil::MakeDB(char *acEnginePath, char *acDbName, char *acLogMsg)
{
	return m_tWPtnUtil.we_dbmake(acEnginePath, acDbName, acLogMsg);
}

/**************************************************************************************************************
	함수명 :  we_uninit_db
	Input :
	Output :
	RetVal :
	Information :
		해쉬 테이블 메모리를 해제 하는 함수
**************************************************************************************************************/

void CWhiteEngineUtil::UnInitDB()
{
	m_tWPtnUtil.m_tHashUtil.UninitHashWithLock();
}

/**************************************************************************************************************
	함수명 :  AddWL
	Input :
		char *szPath : 추가할 패턴 파일명 (NULL이면 화이트리스트 메모리로 추가)
		DWORD dwListSize : 화이트리스트 메모리 사이즈 (확장에 대비하여 사이즈로 구분)
	Input/Output :
		PVOID pWhiteList : 추가할 화이트리스트 메모리 (경로로 추가하는 경우 추가된 데이터를 반환)
	Output :
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 패턴 추가 성공
		< 0 : 패턴 추가 실패
	Information :
		파일로 추가하는 기능 및 화이트 리스트로 추가하는 방식 병행, 파일로 추가하는 경우 추가되는 데이터 반환
**************************************************************************************************************/

INT32 CWhiteEngineUtil::AddWL(char *szPath, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg)
{
	PASI_WENG_WL_EX pWhiteListEx = NULL;
	INT32 nRetVal = 0;
	if(szPath == NULL && pWhiteList == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AddWL] invalid input data");
		return -1;
	}

	pWhiteListEx = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(pWhiteListEx == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AddWL] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pWhiteListEx, 0, sizeof(ASI_WENG_WL_EX));

	do{
		if(szPath == NULL || szPath[0] == 0)
		{
			if(dwListSize != sizeof(ASI_WENG_WL_EX) || pWhiteList == NULL)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[AddWL] invalid list addr (sz:%d)", dwListSize);
				nRetVal = -3;
				break;
			}
			memcpy(pWhiteListEx, pWhiteList, sizeof(ASI_WENG_WL_EX));
		}
		nRetVal = m_tWPtnUtil.asi_add_white_list(szPath, pWhiteListEx, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}
		if(dwListSize == sizeof(ASI_WENG_WL_EX) && pWhiteList != NULL)
		{
			memcpy(pWhiteList, pWhiteListEx, sizeof(ASI_WENG_WL_EX));
		}
		nRetVal = 0;
	}while(FALSE);
	safe_free(pWhiteListEx);
	return nRetVal;
}

/**************************************************************************************************************
	함수명 :  ModWL
	Input :
		PVOID pWhiteList : 변경할 화이트리스트 메모리
		DWORD dwListSize : 화이트리스트 메모리 사이즈 (확장에 대비하여 사이즈로 구분)
	Output :
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 패턴 정보 변경 성공
		< 0 : 패턴 정보 변경 실패
	Information :
		입력된 화이트 리스트 구조체로 메모리상의 동일한 해쉬의 화이트 리스트 정보 변경
**************************************************************************************************************/

INT32 CWhiteEngineUtil::ModWL(PVOID pWhiteList, DWORD dwListSize, char *acLogMsg)
{
	PASI_WENG_WL_EX pWhiteListEx = NULL;
	INT32 nRetVal = 0;
	if(pWhiteList == NULL || dwListSize == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ModWL] invalid input data");
		return -1;
	}
	pWhiteListEx = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(pWhiteListEx == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ModWL] fail to allocate memory (%d)", errno);
		return -2;
	}
	do{
		if(dwListSize != sizeof(ASI_WENG_WL_EX))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[ModWL] invalid list size (%d)", dwListSize);
			nRetVal = -3;
			break;
		}
		memcpy(pWhiteListEx, pWhiteList, sizeof(ASI_WENG_WL_EX));

		nRetVal = m_tWPtnUtil.asi_mod_white_list(pWhiteListEx, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	safe_free(pWhiteListEx);
	return nRetVal;
}


/**************************************************************************************************************
	함수명 :  DelWL
	Input :
		char *szPath : 삭제할 패턴 파일명 (NULL이면 화이트리스트의 해쉬값으로 삭제)
		DWORD dwListSize : 화이트리스트 메모리 사이즈 (확장에 대비하여 사이즈로 구분)
	Input/Output :
		PVOID pWhiteList : 삭제할 화이트리스트 메모리 (경로로 삭제하는 경우 삭제된 데이터를 반환)
	Output :
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 패턴 삭제 성공
		< 0 : 패턴 삭제 실패
	Information :
		파일로 삭제하는 기능 및 화이트 리스트로 삭제하는 방식 병행, 파일로 추가하는 경우 추가되는 데이터 반환
**************************************************************************************************************/

INT32 CWhiteEngineUtil::DelWL(char *szPath, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg)
{
	PASI_WENG_WL_EX pWhiteListEx = NULL;
	INT32 nRetVal = 0;

	if(szPath == NULL && pWhiteList == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[DelWL] invalid input data");
		return -1;
	}

	pWhiteListEx = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(pWhiteListEx == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[DelWL] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pWhiteListEx, 0, sizeof(ASI_WENG_WL_EX));

	do{
		if(szPath == NULL || szPath[0] == 0)
		{
			if(dwListSize != sizeof(ASI_WENG_WL_EX) || pWhiteList == NULL)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[DelWL] invalid list size (%d)", dwListSize);
				nRetVal = -3;
				break;
			}
			memcpy(pWhiteListEx, pWhiteList, sizeof(ASI_WENG_WL_EX));
		}
		nRetVal = m_tWPtnUtil.asi_del_white_list(szPath, pWhiteListEx, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= -100;
			break;
		}
		if(dwListSize == sizeof(ASI_WENG_WL_EX) && pWhiteList != NULL)
		{
			memcpy(pWhiteList, pWhiteListEx, sizeof(ASI_WENG_WL_EX));
		}
	}while(FALSE);
	safe_free(pWhiteListEx);
	return nRetVal;
}

/**************************************************************************************************************
	함수명 :  GetWL
	Input :
		char *szPath : 조회할 패턴 파일명 (NULL이면 화이트리스트의 해쉬값으로 조회)
		DWORD dwListSize : 화이트리스트 메모리 사이즈 (확장에 대비하여 사이즈로 구분)
	Input/Output :
		PVOID pWhiteList : 조회할 화이트리스트 메모리
		DWORD *pdwWhiteType : 조회할 화이트리스트 파일 여부
		AS_WHITE_FILE(0x0000)	: 화이트리스트 존재
		AS_ELF_FILE(0x0001)  : 화이트리스트 존재하지 않는 ELF 파일
		AS_INVALID_FILE(0x0004) " Non ELF 파일 혹은 에러 상황
	Output :
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 패턴 조회 성공
		< 0 : 패턴 조회 실패
	Information :
		파일로 조회하는 기능 및 화이트 리스트 해쉬로 조회하는 방식 병행
**************************************************************************************************************/

INT32 CWhiteEngineUtil::GetWL(char *szPath, PVOID pWhiteList, DWORD dwListSize, DWORD *pdwWhiteType, char *acLogMsg)
{
	PASI_WENG_WL_EX pWhiteListEx = NULL;
	INT32 nRetVal = 0;
	DWORD dwWhiteType = AS_INVALID_FILE;
	if(szPath == NULL && pWhiteList == NULL || pdwWhiteType == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetWL] invalid input data");
		return -1;
	}
	pWhiteListEx = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX));
	if(pWhiteListEx == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetWL] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pWhiteListEx, 0, sizeof(ASI_WENG_WL_EX));
	
	do{
		if(szPath == NULL || szPath[0] == 0)
		{
			if(dwListSize != sizeof(ASI_WENG_WL_EX) || pWhiteList == NULL)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[GetWL] invalid list size (%d)", dwListSize);
				nRetVal = -3;
				break;
			}
			memcpy(pWhiteListEx, pWhiteList, sizeof(ASI_WENG_WL_EX));
		}
		nRetVal = m_tWPtnUtil.asi_get_white_list(szPath, pWhiteListEx, &dwWhiteType, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}
		*pdwWhiteType = dwWhiteType;
		if(dwWhiteType != AS_INVALID_FILE)
		{
			if(dwListSize == sizeof(ASI_WENG_WL_EX) && pWhiteList != NULL)
			{
				memcpy(pWhiteList, pWhiteListEx, sizeof(ASI_WENG_WL_EX));
			}
		}
	}while(FALSE);
	safe_free(pWhiteListEx);
	return nRetVal;
}

/**************************************************************************************************************
	함수명 :  GetWLCnt
	Input :
		PVOID pPasrseData : 조회할 패턴 조건 구조체 (NULL인 경우 전체 리스트)
		DWORD dwDataSize : 패턴 조건 구조체 사이즈
	Output :
		INT32 *pnCount : 조건에 일치하는 패턴 수
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 패턴 수 조회 성공
		< 0 : 패턴 수 조회 실패
	Information :
		입력된 조건 구조체에 일치하는 리스트의 수 조회
**************************************************************************************************************/

INT32 CWhiteEngineUtil::GetWLCnt(PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg)
{
	PWHITE_PARSE_DATA pRuleData = NULL;
	INT32 nRetVal = 0;
	INT32 nListCount = 0;

	if(pnCount == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetWLCnt] invalid input data");
		return -1;
	}
	pRuleData = (PWHITE_PARSE_DATA)malloc(sizeof(WHITE_PARSE_DATA));
	if(pRuleData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetWLCnt] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pRuleData, 0, sizeof(WHITE_PARSE_DATA));

	if(dwDataSize == sizeof(WHITE_PARSE_DATA) && pPasrseData != NULL)
	{
		memcpy(pRuleData, pPasrseData, sizeof(WHITE_PARSE_DATA));
	}
	do{
		nRetVal = m_tWPtnUtil.asi_get_white_list_count(pRuleData, &nListCount, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}
		*pnCount = nListCount;
		nRetVal = 0;
	}while(FALSE);
	safe_free(pRuleData);
	
	return nRetVal;
}

/**************************************************************************************************************
	함수명 :  GetAllWL
	Input :
		PVOID pPasrseData : 조회할 패턴 조건 구조체 (NULL인 경우 전체 리스트, 단 we_get_white_list_count 조건과 동일한 조건이어야 함)
		DWORD dwDataSize : 패턴 조건 구조체 사이즈
		INT32 nTotalCount : 조회할 화이트리스트 수
	Output :
		PVOID pWhiteList : 조회할 화이트리스트
		DWORD dwListSize : 화이트리스트 구조체 사이즈
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 전체 패턴 조회 성공
		< 0 : 전체 패턴 조회 실패
	Information :
		입력된 조건 구조체에 일치하는 리스트 전체 조회
**************************************************************************************************************/

INT32 CWhiteEngineUtil::GetAllWL(PVOID pPasrseData, DWORD dwDataSize, INT32 nTotalCount, PVOID pWhiteList, DWORD dwListSize, char *acLogMsg)
{
	PWHITE_PARSE_DATA pRuleData = NULL;
	INT32 nRetVal = 0;
	PASI_WENG_WL_EX pWhiteListEx = NULL;

	if(pWhiteList == NULL || dwListSize == 0 || nTotalCount < 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetAllWL] invalid input data");
		return -1;
	}
	pRuleData = (PWHITE_PARSE_DATA)malloc(sizeof(WHITE_PARSE_DATA));
	if(pRuleData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetAllWL] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pRuleData, 0, sizeof(WHITE_PARSE_DATA));

	if(dwDataSize == sizeof(WHITE_PARSE_DATA) && pPasrseData != NULL)
	{
		memcpy(pRuleData, pPasrseData, sizeof(WHITE_PARSE_DATA));
	}

	do{
		pWhiteListEx = (PASI_WENG_WL_EX)malloc(sizeof(ASI_WENG_WL_EX)*nTotalCount);
		if(pWhiteListEx == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[GetAllWL] fail to allocate memory (en:%d) (cnt:%d)", errno, nTotalCount);
			nRetVal = -3;
			break;
		}
		memset(pWhiteListEx, 0, sizeof(ASI_WENG_WL_EX)*nTotalCount);

		nRetVal = m_tWPtnUtil.asi_get_all_white_list(pRuleData, nTotalCount, pWhiteListEx, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}
		if(dwListSize == sizeof(ASI_WENG_WL_EX) && pWhiteList != NULL)
		{
			memcpy(pWhiteList, pWhiteListEx, sizeof(ASI_WENG_WL_EX)*nTotalCount);
		}
		nRetVal = 0;
	}while(FALSE);

	safe_free(pRuleData);
	safe_free(pWhiteListEx);

	return nRetVal;
}

/**************************************************************************************************************
	함수명 :  GetWLDBCnt
	Input :
		char *acEnginePath : 패턴 파일이 포함되어 있는 경로 (wdb 파일 수 조회)
	Output :
		DWORD *pdwFileCount : 파일 수 반환
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 파일 수 조회 성공
		FALSE : 파일 수 조회 실패
	Information :
		입력된 경로에 포함되어 있는 패턴 파일 수 반환
**************************************************************************************************************/


INT32 CWhiteEngineUtil::GetWLDBCnt(char *szEnginePath, INT32 *pnFileCount, char *acLogMsg)
{
	return m_tWPtnUtil.asi_get_wdb_count(szEnginePath, pnFileCount, acLogMsg);
}


/**************************************************************************************************************
	함수명 :  GetAllWLDBHdr
	Input :
		char *acEnginePath : 패턴 파일이 포함되어 있는 경로 (wdb 파일 헤더 조회)
		INT32 nFileCount : 패턴 파일 수
		DWORD dwHdrSize : 파일 헤더 정보 구조체 사이즈
	Output :
		PVOID pWhiteHdr : 반환되는 파일 헤더 정보 리스트
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 전체 패턴 파일 헤더 조회 성공
		< 0 : 전체 패턴 파일 헤더 조회 실패
	Information :
		입력된 경로에 포함되어 있는 전체 패턴 파일 헤더 정보 조회
**************************************************************************************************************/


INT32 CWhiteEngineUtil::GetAllWLDBHdr(char *szEnginePath, INT32 nFileCount, PVOID pWhiteHdr, DWORD dwHdrSize, char *acLogMsg)
{
	PWHITE_FILE_HDR pFileHdr = NULL;
	INT32 nRetVal = 0;
	
	if(szEnginePath == NULL || nFileCount < 0 || pWhiteHdr == NULL || dwHdrSize == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetAllWLDBHdr] invalid input data");
		return -1;
	}

	pFileHdr = (PWHITE_FILE_HDR)malloc(sizeof(WHITE_FILE_HDR)*nFileCount);
	if(pFileHdr == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetAllWLDBHdr] fail to allocate memory (cnt:%d) (%d)", nFileCount, errno);
		return -2;
	}
	memset(pFileHdr, 0, sizeof(WHITE_FILE_HDR)*nFileCount);

	do{
		nRetVal = m_tWPtnUtil.asi_get_all_wdb_hdr(szEnginePath, nFileCount, pFileHdr, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}
		if(sizeof(WHITE_FILE_HDR) != dwHdrSize)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[GetAllWLDBHdr] invalid header size (in:%d) (sz:%d)", dwHdrSize, sizeof(WHITE_FILE_HDR));
			nRetVal = -3;
			break;
		}
		memcpy(pWhiteHdr, pFileHdr, sizeof(WHITE_FILE_HDR)*nFileCount);
		nRetVal = 0;
	}while(FALSE);
	safe_free(pFileHdr);
	return nRetVal;
}


/**************************************************************************************************************
	함수명 :  GetWLDBHdr
	Input :
		char *acEngineFile : 패턴 파일 경로
		DWORD dwHdrSize : 파일 헤더 정보 구조체 사이즈
	Output :
		PVOID pWhiteHdr : 반환되는 파일 헤더 정보
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 패턴 파일 헤더 조회 성공
		< 0 : 패턴 파일 헤더 조회 실패
	Information :
		입력된 경로의 패턴 파일 헤더 정보 조회
**************************************************************************************************************/

INT32 CWhiteEngineUtil::GetWLDBHdr(char *acEngineFile, PVOID pWhiteHdr, DWORD dwHdrSize, char *acLogMsg)
{
	int nLen = 0;
	INT32 nRetVal = 0;
	FILE *fp = NULL;
	PWHITE_FILE_HDR pFileHdr = NULL;
	char szTempPath[MAX_PATH] = {0,};

	if(acEngineFile == NULL || pWhiteHdr == NULL || dwHdrSize == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetWLDBHdr] invalid input data");
		return -1;
	}
	snprintf(szTempPath, MAX_PATH-1, "%s.tmp", acEngineFile);

	pFileHdr = (PWHITE_FILE_HDR)malloc(sizeof(WHITE_FILE_HDR));
	if(pFileHdr == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[GetWLDBHdr] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pFileHdr, 0, sizeof(WHITE_FILE_HDR));

	do{
		nRetVal = AriaDeCryptFile(ASI_ARIA, acEngineFile, szTempPath, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}
		fp = fopen(szTempPath, "r");
		if(fp == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[GetWLDBHdr] fail to open file %s (%d) ", szTempPath, errno);
			nRetVal = -3;
			break;
		}
		nRetVal = m_tWPtnUtil.asi_get_wdb_hdr(fp, pFileHdr, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 200;
			break;
		}
		if(sizeof(WHITE_FILE_HDR) != dwHdrSize)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[GetWLDBHdr] invalid header size (%d) (%d)", dwHdrSize, sizeof(WHITE_FILE_HDR));
			nRetVal = -4;
			break;
		}
		memcpy(pWhiteHdr, pFileHdr, sizeof(WHITE_FILE_HDR));
		nRetVal = 0;
	}while(FALSE);

	if(fp != NULL)
	{
		fclose(fp);
	}
	if(is_file(szTempPath) == 0)
	{
		unlink(szTempPath);
	}
	safe_free(pFileHdr);
	return nRetVal;
}

/**************************************************************************************************************
	함수명 :  MakeWLDB
	Input :
		char *szEngineFile : 생성할 패턴 파일 경로
		DWORD dwCategory : 생성할 패턴 파일 카테고리
		PVOID pPasrseData : 생성할 패턴 조건 구조체 (NULL인 경우 전체 리스트)
		DWORD dwDataSize : 패턴 조건 구조체 사이즈
	Output :
		DWORD *pdwCount : 조건에 일치하여 패턴 파일에 추가된 패턴 수
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 패턴 파일 생성 성공
		< 0 : 패턴 파일 생성 실패
	Information :
		메모리에 있는 전체 리스트중 입력된 조건에 일치하는 패턴을 입력된 패턴 파일명으로 패턴 파일 생성
**************************************************************************************************************/

INT32 CWhiteEngineUtil::MakeWLDB(char *szEngineFile, DWORD dwCategory, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg)
{
	PWHITE_PARSE_DATA pRuleData = NULL;
	INT32 nRetVal = 0;
	INT32 nListCount = 0;
	if(szEngineFile == NULL || pnCount == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[MakeWLDB] invalid input data");
		return -1;
	}

	pRuleData = (PWHITE_PARSE_DATA)malloc(sizeof(WHITE_PARSE_DATA));
	if(pRuleData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[MakeWLDB] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pRuleData, 0, sizeof(WHITE_PARSE_DATA));

	do{
		if(dwDataSize == sizeof(WHITE_PARSE_DATA) && pPasrseData != NULL)
		{
			memcpy(pRuleData, pPasrseData, sizeof(WHITE_PARSE_DATA));
		}
		nRetVal = m_tWPtnUtil.asi_make_wdb(szEngineFile, dwCategory, pRuleData, &nListCount, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}
		*pnCount = nListCount;
		nRetVal = 0;
	}while(FALSE);
	safe_free(pRuleData);
	return nRetVal;
}

/**************************************************************************************************************
	함수명 :  LoadWL
	Input :
		char *szEngineFile : 로딩할 패턴 파일 경로
		PVOID pPasrseData : 로딩할 패턴 조건 구조체 (NULL인 경우 파일의 전체 리스트)
		DWORD dwDataSize : 패턴 조건 구조체 사이즈
	Output :
		INT32 *pnCount : 조건에 일치하여 로딩된 패턴 수
		char *acLogMsg : 에러 메시지
	RetVal :
		0 : 패턴 파일 로딩 성공
		< 0 : 패턴 파일 로딩 실패
	Information :
		패턴 파일에서 입력된 조건에 일치하는 패턴을 화이트리스트 메모리에 로딩
**************************************************************************************************************/

INT32 CWhiteEngineUtil::LoadWL(char *szEngineFile, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount, char *acLogMsg)
{
	PWHITE_PARSE_DATA pRuleData = NULL;
	INT32 nRetVal = 0;
	INT32 nListCount = 0;

	if(szEngineFile == NULL || pnCount == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[LoadWL] invalid input data");
		return -1;
	}
	pRuleData = (PWHITE_PARSE_DATA)malloc(sizeof(WHITE_PARSE_DATA));
	if(pRuleData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[LoadWL] fail to allocate memory (%d)", errno);
		return -2;
	}
	memset(pRuleData, 0, sizeof(WHITE_PARSE_DATA));

	do{
		if(dwDataSize == sizeof(WHITE_PARSE_DATA) && pPasrseData != NULL)
		{
			memcpy(pRuleData, pPasrseData, sizeof(WHITE_PARSE_DATA));
		}
		nRetVal = m_tWPtnUtil.asi_load_white_list(szEngineFile, pRuleData, &nListCount, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}
		*pnCount = nListCount;
		nRetVal = 0;
	}while(FALSE);

	safe_free(pRuleData);

	return nRetVal;
}