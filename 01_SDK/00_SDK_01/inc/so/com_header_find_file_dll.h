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


#ifndef COM_HEADER_FIND_FILE_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_FIND_FILE_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


//////////////////////////////////////////////////////////////////////////

#define		ASI_FF_FILE_DT_CHK_TYPE_CREATE				0
#define		ASI_FF_FILE_DT_CHK_TYPE_WRITE				1
#define		ASI_FF_FILE_DT_CHK_TYPE_ACCESS				2
//------------------------------------------------------------------------

#define		ASI_FF_FILE_FIND_TYPE_PATTERN				0
#define		ASI_FF_FILE_FIND_TYPE_DOC_FILE_FORMAT		1
//------------------------------------------------------------------------

#define		ASI_FF_FIND_OPTION_USED_DOC_FILE_FORMAT				0x00000001
#define		ASI_FF_FIND_OPTION_SEARCH_NET_DRIVE					0x00000002
#define		ASI_FF_FIND_OPTION_EXCLUDE_ALL_PATH					0x00000004
//------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////

typedef struct _asi_ff_init_info
{
	_asi_ff_init_info()
	{
		memset(szLogPath, 0, MAX_PATH);
		memset(szLogFile, 0, MAX_PATH);
		nRemainLog				= 1;
		nFinderThreadMaxNum		= 4;
		nAutoSearchDirLevel		= 3;
		nOnceFindFileNum		= 1000;
		memset(szDocFileFmtDLLPath, 0, MAX_PATH);
		memset(szFileInfoDLLPath, 0, MAX_PATH);
	}
	CHAR		szLogPath[MAX_PATH];
	CHAR		szLogFile[MAX_PATH];
	UINT32		nRemainLog;
	UINT32		nFinderThreadMaxNum;
	UINT32		nAutoSearchDirLevel;
	UINT32		nOnceFindFileNum;
	CHAR		szDocFileFmtDLLPath[MAX_PATH];
	CHAR		szFileInfoDLLPath[MAX_PATH];
}ASI_FF_INIT_INFO, *PASI_FF_INIT_INFO;

//-----------------------------------------------------------------------

typedef struct _asi_ff_file_item
{
	_asi_ff_file_item()
	{
		memset(szFilePath, 0, MAX_PATH);
		memset(szFileName, 0, MAX_PATH);

		memset(wzFilePath, 0, sizeof(WCHAR)*MAX_PATH);
		memset(wzFileName, 0, sizeof(WCHAR)*MAX_PATH);

		nFileSize		= 0;
	}
	CHAR		szFilePath[MAX_PATH];
	CHAR		szFileName[MAX_PATH];
	WCHAR		wzFilePath[MAX_PATH];
	WCHAR		wzFileName[MAX_PATH];
	UINT64		nFileSize;
	UINT32		nFindType;
}ASI_FF_FILE_ITEM, *PASI_FF_FILE_ITEM;
//-----------------------------------------------------------------------

typedef struct _asi_ff_file_result
{
	_asi_ff_file_result()
	{
		nContinue			= 0;
		nMoreFileItem		= 0;
		nSearchPathNum		= 0;
		nSearchedPathNum	= 0;
		nDirSubSearchNum	= 0;
		nDirSubSearchedNum	= 0;
		nDirTotalNum		= 0;
		nDirSearchedNum		= 0;
		nFileTotalNum		= 0;
		nFileWorkedNum		= 0;
	}
	UINT32		nContinue;
	UINT32		nMoreFileItem;
	UINT32		nSearchPathNum;
	UINT32		nSearchedPathNum;
	UINT32		nDirSubSearchNum;
	UINT32		nDirSubSearchedNum;
	UINT32		nDirTotalNum;
	UINT32		nDirSearchedNum;
	UINT32		nFileTotalNum;
	UINT32		nFileWorkedNum;
}ASI_FF_FILE_RESULT, *PASI_FF_FILE_RESULT;;

//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------

typedef	INT32 (*fn_ASIFF_InitType)(PVOID* lpObject, PASI_FF_INIT_INFO pafii, UINT32 nSize);
typedef	INT32 (*fn_ASIFF_FreeType)(PVOID lpObject);

typedef	INT32 (*fn_ASIFF_DeleteFindFileWorkType)(PVOID lpObject, UINT32 nOrderID);
typedef	INT32 (*fn_ASIFF_AddSearchDirPathType)(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpSearchPath);
typedef	INT32 (*fn_ASIFF_ClearSearchDirPathType)(PVOID lpObject, UINT32 nOrderID);

typedef	INT32 (*fn_ASIFF_AddFileMaskType)(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpFileMask);
typedef	INT32 (*fn_ASIFF_DelFileMaskType)(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpFileMask);
typedef	INT32 (*fn_ASIFF_ClearFileMaskType)(PVOID lpObject, UINT32 nOrderID);

typedef	INT32 (*fn_ASIFF_AddExceptPathType)(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpExceptPath); 
typedef	INT32 (*fn_ASIFF_DelExceptPathType)(PVOID lpObject, UINT32 nOrderID, LPCTSTR lpExceptPath);
typedef	INT32 (*fn_ASIFF_ClearExceptPathType)(PVOID lpObject, UINT32 nOrderID);

typedef	INT32 (*fn_ASIFF_AddFileDateTimeType)(PVOID lpObject, UINT32 nOrderID, UINT32 nType, UINT32 nChkTime);
typedef	INT32 (*fn_ASIFF_DelFileDateTimeType)(PVOID lpObject, UINT32 nOrderID, UINT32 nType);
typedef	INT32 (*fn_ASIFF_ClearFileDateTimeType)(PVOID lpObject, UINT32 nOrderID);

typedef	INT32 (*fn_ASIFF_AddFileFindOptionType)(PVOID lpObject, UINT32 nOrderID, UINT32 nFindOption);
typedef	INT32 (*fn_ASIFF_ClearFileFindOptionType)(PVOID lpObject, UINT32 nOrderID);

typedef	INT32 (*fn_ASIFF_SearchDirFileType)(PVOID lpObject, UINT32 nOrderID);
typedef	INT32 (*fn_ASIFF_SearchDirFileThreadType)(PVOID lpObject, UINT32 nOrderID);
typedef	INT32 (*fn_ASIFF_GetFindFileItemType)(PVOID lpObject, UINT32 nOrderID, PASI_FF_FILE_ITEM pAFFIList, PUINT32 nListSize, PASI_FF_FILE_RESULT pAFFR);

//////////////////////////////////////////////////////////////////////////

#endif //COM_HEADER_FIND_FILE_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
