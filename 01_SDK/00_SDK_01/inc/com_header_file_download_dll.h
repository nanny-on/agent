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


#ifndef COM_HEADER_FILE_DOWNLOAD_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_FILE_DOWNLOAD_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


#define ASIFDL_DL_SVR_TYPE_UNKNOWN        		0x00000000
#define ASIFDL_DL_SVR_TYPE_LOCAL				0x00000001
#define ASIFDL_DL_SVR_TYPE_PTOP           		0x00000002
#define ASIFDL_DL_SVR_TYPE_SITE      			0x00000004
#define ASIFDL_DL_SVR_TYPE_PUBLIC    			0x00000008
#define ASIFDL_DL_SVR_TYPE_ALL					(ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE | ASIFDL_DL_SVR_TYPE_PUBLIC)
#define ASIFDL_DL_SVR_TYPE_MINIMUN				(ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_SITE)

#define ASIFDL_DL_FILE_TYPE_UPDATE					1
#define ASIFDL_DL_FILE_TYPE_DEPLOY					2
#define ASIFDL_DL_FILE_TYPE_WPTN_GBL				3
#define ASIFDL_DL_FILE_TYPE_WPTN_GWL				4
#define ASIFDL_DL_FILE_TYPE_WPTN_GBO				5
#define ASIFDL_DL_FILE_TYPE_WPTN_GWO				6


#define ASIFDL_DL_RST_TYPE_FAIL        				0
#define ASIFDL_DL_RST_TYPE_SUCCESS     				1

#define	ASIFDL_DL_RST_STATUS_TYPE_READY						0
#define	ASIFDL_DL_RST_STATUS_TYPE_START						1
#define	ASIFDL_DL_RST_STATUS_TYPE_SUCCESS_END				10
#define	ASIFDL_DL_RST_STATUS_TYPE_FAIL_END					20
#define	ASIFDL_DL_RST_STATUS_TYPE_FAIL_HASH_END				21
#define ASIFDL_DL_RST_STATUS_TYPE_CANCEL					30
#define ASIFDL_DL_RST_STATUS_TYPE_CANCEL_REF_OVER			31
#define ASIFDL_DL_RST_STATUS_TYPE_ALL_END					100
#define ASIFDL_DL_RST_STATUS_TYPE_LOGIC_END					200

#define ASIFDL_DL_DEFAULT_PTP_PORT						55000

#define ASIFDL_DL_FILE_LOCATION_TYPE_US                 1
#define ASIFDL_DL_FILE_LOCATION_TYPE_KOR                2
#define	ASIFDL_DL_FILE_LOCATION_TYPE_JAPAN				3
#define ASIFDL_DL_FILE_LOCATION_TYPE_CHINA_SIMPLIFIED	4
#define ASIFDL_DL_FILE_LOCATION_TYPE_CHINA_TRADITIONAL	5

//--------------------------------------------------------------------

#define ASIFDL_DL_FILE_HASH_TYPE_SHA1_40					40
#define ASIFDL_DL_FILE_HASH_TYPE_SHA2_64					64                  
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

typedef struct _asi_fdl_info
{
	_asi_fdl_info()
	{
		nID				= 0;
		nItemType		= 0;
		nItemID			= 0;
		nPolicyID		= 0;
		nItemPkg		= 0;
		
		memset(szFileName, 0, MAX_PATH);
		memset(szFileHash, 0, MAX_PATH);
		memset(szDLPath, 0, MAX_PATH);

		nDLSvrUsedFlag	= (ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE);
		nDLSvrType		= ASIFDL_DL_SVR_TYPE_UNKNOWN;
		nRefCnt			= 0;
		nRefCntMax		= 0;
		nFileStatus		= ASIFDL_DL_RST_STATUS_TYPE_READY;
		nRemoveReserved = 0;
	}
    UINT32	nID;
	UINT32  nItemType;
    UINT32  nItemID;
	UINT32	nPolicyID;
	UINT32	nItemPkg;

    CHAR	szFileName[MAX_PATH];
    CHAR	szFileHash[MAX_PATH];
	CHAR	szDLPath[MAX_PATH];

	UINT32	nDLSvrUsedFlag;
    UINT32  nDLSvrType;   

    UINT32  nRefCnt;
	UINT32	nRefCntMax;
	UINT32	nFileStatus;

	UINT32	nRemoveReserved;
} ASI_FDL_INFO, *PASI_FDL_INFO;

typedef list<ASI_FDL_INFO>				TListAFDLInfo;
typedef TListAFDLInfo::iterator			TListAFDLInfoItor;

typedef map<UINT32, ASI_FDL_INFO>		TMapAFDLInfo;
typedef TMapAFDLInfo::iterator			TMapAFDLInfoItor;
//---------------------------------------------------------------------------

typedef struct _asi_fdl_result_item
{	
	UINT32	nID;
	UINT32	nItemType;
	UINT32	nItemID;
	UINT32	nStatus;
	UINT32	nRemainFileNum;
	UINT32	nDLSvrType;
	UINT32	nRefCount;
	char	pszFileName[CHAR_MAX_SIZE];
	PVOID	lParam;
}ASI_FDL_RESULT_ITEM, *PASI_FDL_RESULT_ITEM;

typedef void (*ASIFDL_GetResultItemType)(PASI_FDL_RESULT_ITEM pafri);

//---------------------------------------------------------------------------

typedef struct _asi_fdl_result
{
	_asi_fdl_result()
	{
		fDLResult	= NULL;
		lParam		= NULL;
	}
	ASIFDL_GetResultItemType		fDLResult;
	PVOID					lParam;
}ASI_FDL_RESULT, *PASI_FDL_RESULT;
//---------------------------------------------------------------------------

typedef struct _asi_fdl_init
{
	_asi_fdl_init()
	{
		memset(szHomePath, 0, MAX_PATH);
		memset(szTempPath, 0, MAX_PATH);

		memset(szPtoPDLLPath, 0, MAX_PATH);
		memset(szSHADLLPath, 0, MAX_PATH);

		memset(szLogPath, 0, MAX_PATH);
		memset(szLogFile, 0, MAX_PATH);

		memset(szDLSiteSvrInfo, 0, MAX_PATH);
		memset(szDLPubSvrInfo, 0, MAX_PATH);
		memset(szDLPtoPSvrInfo, 0, MAX_PATH);

		hOwner					= NULL;
		nNotifyMsgID			= 0;
		pnMainContinue			= NULL;

		nDLSvrUsedFlag			= 0;
	}
	HANDLE				hOwner;
	UINT32				nNotifyMsgID;
	PINT32				pnMainContinue;
	CHAR				szHomePath[MAX_PATH];
	CHAR				szTempPath[MAX_PATH];

	CHAR				szPtoPDLLPath[MAX_PATH];
	CHAR				szSHADLLPath[MAX_PATH];

	CHAR				szLogPath[MAX_PATH];
	CHAR				szLogFile[MAX_PATH];

	CHAR				szDLSiteSvrInfo[MAX_PATH];
	CHAR				szDLPubSvrInfo[MAX_PATH];
	CHAR				szDLPtoPSvrInfo[MAX_PATH];

	UINT32				nDLSvrUsedFlag;

	ASI_FDL_RESULT		tAFDLR;
}ASI_FDL_INIT, *PASI_FDL_INIT;
//---------------------------------------------------------------------------

typedef INT32 (*ASIFDL_AddDLInfoType)(PASI_FDL_INFO pafdi);
typedef INT32 (*ASIFDL_FindDLInfoType)(UINT32 nID, PASI_FDL_INFO pafdi);
typedef INT32 (*ASIFDL_DelDLInfoType)(UINT32 nItemType, UINT32 nItemID);

typedef INT32 (*ASIFDL_RunDLType)();
typedef INT32 (*ASIFDL_IsRunDLType)();
typedef INT32 (*ASIFDL_StopDLType)();

typedef INT32 (*ASIFDL_SetDLInitType)(PASI_FDL_INIT pafdi);
typedef INT32 (*ASIFDL_SetDLSvrTypeType)(UINT32 nDLSvrType);
typedef INT32 (*ASIFDL_SetDLSvrInfoType)(UINT32 nItemType, LPCTSTR szSvrInfo);
typedef	INT32 (*ASIFDL_SetDLLogPathType)(LPCTSTR lpLogPath, LPCTSTR lpLogFile, INT32 nRemainFlag);
typedef	INT32 (*ASIFDL_SetHomePathType)(LPCTSTR lpHomePathList, UINT32 nLen);

//---------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////

#endif //COM_HEADER_FILE_DOWNLOAD_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
