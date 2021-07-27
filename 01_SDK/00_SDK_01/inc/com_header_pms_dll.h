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

#ifndef COM_HEADER_ASI_PMS_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_ASI_PMS_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//------------------------------------------------------------------------------
#define ASI_PMS_WORK_NOTIFY_MSG_TYPE_START_SCAN		0
#define ASI_PMS_WORK_NOTIFY_MSG_TYPE_END_SCAN		1
//------------------------------------------------------------------------------
#define ASI_PMS_WORK_SCAN_TYPE_ONLINE				0
#define ASI_PMS_WORK_SCAN_TYPE_OFFLINE				1
//------------------------------------------------------------------------------
#define ASI_PMS_WORK_FIND_TYPE_NEW_OS_VULN_WUA		0x00000001
#define ASI_PMS_WORK_FIND_TYPE_NEW_APP_VULN_ASE		0x00000002

#define ASI_PMS_WORK_FIND_TYPE_OLD_OS_VULN_WUA		0x00000100
#define ASI_PMS_WORK_FIND_TYPE_OLD_OS_VULN_QFE		0x00000200
#define ASI_PMS_WORK_FIND_TYPE_OLD_OS_FUNC_MSI		0x00000400
//------------------------------------------------------------------------------

#define ASI_PMS_WUA_FILE_VERSION "7.4.7600.226"
//------------------------------------------------------------------------------

typedef enum _ASI_PMS_DATA_TYPE
{
	ASI_PMS_DATA_TYPE_PTN_PATCH				= 1,
	ASI_PMS_DATA_TYPE_PTN_PATCH_SCAN,
	ASI_PMS_DATA_TYPE_PTN_PATCH_SCAN_ENV,
	ASI_PMS_DATA_TYPE_PTN_PATCH_SCAN_ENV_UNIT,
}ASI_PMS_DATA_TYPE;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

typedef struct _asi_pms_init
{
	_asi_pms_init()
	{
		nRemainLog		= 1;
		
		ZeroMemory(szLogPath, MAX_PATH);
		ZeroMemory(szLogFile, MAX_PATH);

		ZeroMemory(szEngineVer, MAX_PATH);
	}

	CHAR				szLogPath[MAX_PATH];
	CHAR				szLogFile[MAX_PATH];
	INT32				nRemainLog;

	CHAR				szEngineVer[MAX_PATH];

}ASI_PMS_INIT, *PASI_PMS_INIT;
//--------------------------------------------------------------------------

typedef struct _asi_pms_work
{
	_asi_pms_work()
	{
		nID				= 0;
		nScanType		= 0;
		nFindType		= 0;
		nErrCode		= 0;

		hNotifyHwnd		= NULL;
		nNotifyID		= 0;

		nPolicyID		= 0;
		nItemID			= 0;

		ZeroMemory(szWorkName, MAX_PATH);
		ZeroMemory(szPtnPath, MAX_PATH);
		ZeroMemory(szSvcID, MAX_PATH);
		ZeroMemory(szScanOpt, MAX_PATH);
	}

	UINT32				nID;
	UINT32				nScanType;
	UINT32				nFindType;
	UINT32				nErrCode;

	PVOID				hNotifyHwnd;
	UINT32				nNotifyID;

	UINT32				nPolicyID;
	UINT32				nItemID;

	CHAR				szWorkName[MAX_PATH];
	CHAR				szPtnPath[MAX_PATH];
	CHAR				szSvcID[MAX_PATH];
	CHAR				szScanOpt[MAX_PATH];

}ASI_PMS_WORK, *PASI_PMS_WORK;

typedef map<UINT32, ASI_PMS_WORK>		TMapASIPMSWork;
typedef TMapASIPMSWork::iterator		TMapASIPMSWorkItor;
//--------------------------------------------------------------------------

typedef struct _asi_pms_find_item
{
	_asi_pms_find_item()
	{
		nFindType		= 0;
		nKBID			= 0;
		nNoRemove		= 0;

		szServerity		= NULL;
		szGUID			= NULL;
		szVulnName		= NULL;
		szCategoryName	= NULL;
		szInsDate		= NULL;
		szPublisher		= NULL;
		szUninstall		= NULL;
		szFileNameList	= NULL;
	}

	UINT32		nFindType;
	
	UINT32		nKBID;
	LPCTSTR		szServerity;
	LPCTSTR		szGUID;
	LPCTSTR		szVulnName;
	LPCTSTR		szCategoryName;
	LPCTSTR		szPublisher;
	LPCTSTR		szInsDate;
	UINT32		nNoRemove;
	LPCTSTR		szUninstall;
	LPCTSTR		szFileNameList;

}ASI_PMS_FIND_ITEM, *PASI_PMS_FIND_ITEM;
//--------------------------------------------------------------------------

typedef	INT32 (*fn_ASIPMS_InitType)(PVOID* lpObject, PASI_PMS_INIT pAPI, UINT32 nSize);
typedef	INT32 (*fn_ASIPMS_FreeType)(PVOID lpObject);

typedef	INT32 (*fn_ASIPMS_IsReadyEngineType)(PVOID lpObject);
typedef	INT32 (*fn_ASIPMS_ScanPatchType)(PVOID lpObject, PASI_PMS_WORK pAPW, UINT32 nSize);
typedef	INT32 (*fn_ASIPMS_GetLastErrorType)(PVOID lpObject, UINT32 nWorkID, PUINT32 pErrCode);
typedef	INT32 (*fn_ASIPMS_ClearWorkType)(PVOID lpObject, UINT32 nWorkID);

typedef	INT32 (*fn_ASIPMS_ScanPatchSingleType)(PVOID lpObject, UINT32 nType, UINT32 nPatchID);
typedef	INT32 (*fn_ASIPMS_ScanEnvSingleType)(PVOID lpObject, UINT32 nScanEnvID);


typedef	INT32 (*fn_ASIPMS_GetFindCntType)(PVOID lpObject, UINT32 nWorkID, UINT32 nFindType, PUINT32 pFindCnt);
typedef	INT32 (*fn_ASIPMS_GetFindRstType)(PVOID lpObject, UINT32 nWorkID, UINT32 nFindType, UINT32 nID, PASI_PMS_FIND_ITEM pAPFI, UINT32 nSize);

typedef	INT32 (*fn_ASIPMS_SetDataType)(PVOID lpObject, UINT32 nType, UINT32 nCode, PVOID lpDAta, UINT32 nDataSize);
//--------------------------------------------------------------------------

#endif      //COM_HEADER_ASI_PMS_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
