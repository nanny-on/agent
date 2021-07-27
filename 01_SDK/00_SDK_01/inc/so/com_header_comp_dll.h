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

#ifndef COM_HEADER_COMP_DLL_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define COM_HEADER_COMP_DLL_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

#pragma pack(1)
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

typedef struct _asi_comp_info
{
	_asi_comp_info()
	{
		memset(pszCompPath, 0, MAX_PATH);
		memset(pszCompOutName, 0, MAX_PATH);
		memset(pszPassExtension, 0, MAX_PATH);
		nSubDirComp		= 1;
	}
	char	pszCompPath[MAX_PATH];
	char	pszCompOutName[MAX_PATH];
	char	pszPassExtension[MAX_PATH];
	UINT32	nSubDirComp; // 1- YES, 0 - NO
}ASI_COMP_INFO, *PASI_COMP_INFO;
//--------------------------------------------------------------------------

typedef struct _asi_decomp_info
{
	_asi_decomp_info()
	{
		memset(pszDecompFileName, 0, MAX_PATH);
		memset(pszDecompTargetDir, 0, MAX_PATH);
	}
	char	pszDecompFileName[MAX_PATH];
	char	pszDecompTargetDir[MAX_PATH];
}ASI_DECOMP_INFO, *PASI_DECOMP_INFO;
//--------------------------------------------------------------------------

typedef struct _asi_comp_notify_info
{
	_asi_comp_notify_info()
	{
		memset(pszPath, 0, MAX_PATH);
		memset(pszName, 0, MAX_PATH);
		nTotalFileNum	= 0;
		nOpFileNum		= 0;
		lParam			= 0;
	}
	void _add_info(LPCTSTR lpPath, LPCTSTR lpName, UINT32 TotalFileNum, UINT32 OpFileNum, PVOID Param)
	{
		memset(pszPath, 0, MAX_PATH);
		memset(pszName, 0, MAX_PATH);
		strcpy(pszPath, lpPath);
		strcpy(pszName, lpName);
		nTotalFileNum	= TotalFileNum;
		nOpFileNum		= OpFileNum;
		lParam			= Param;
	}
	void _add_info(LPCTSTR lpPath, LPCTSTR lpName, UINT32 OpFileNum)
	{
		memset(pszPath, 0, MAX_PATH);
		memset(pszName, 0, MAX_PATH);
		strcpy(pszPath, lpPath);
		strcpy(pszName, lpName);
		nOpFileNum		= OpFileNum;		
	}

	UINT32		nOp;	// 1 - Compress, 0 - Decompress
	char		pszPath[MAX_PATH];
	char		pszName[MAX_PATH];

	UINT32		nTotalFileNum;
	UINT32		nOpFileNum;
	PVOID		lParam;
}ASI_COMP_NOTIFY_INFO, *PASI_COMP_NOTIFY_INFO;
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#define	ASI_COMP_S_SEED_KEY_LEN		16

typedef struct _asi_comp_secu
{
	_asi_comp_secu()
	{
		memset(acSrcFile, 0, MAX_PATH);
		memset(acTarPath, 0, MAX_PATH);
		memset(acTarFile, 0, MAX_PATH);
		memset(pSeedKey, 0, 20);
	}
	CHAR		acSrcFile[MAX_PATH];
	CHAR		acTarPath[MAX_PATH];
	CHAR		acTarFile[MAX_PATH];
	BYTE		pSeedKey[20];
}ASI_COMP_SECU, *PASI_COMP_SECU;

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

typedef void (CALLBACK *NotifyStateType)(PASI_COMP_NOTIFY_INFO);
//////////////////////////////////////////////////////////////////////////

typedef	INT32 (*fn_ASICOMP_CompDirType)(PASI_COMP_INFO paci);
typedef	INT32 (*fn_ASICOMP_DecompDirType)(PASI_DECOMP_INFO padi);
typedef	INT32 (*fn_ASICOMP_SetNotifyStateType)(PVOID lParam, PVOID pNotifyState);

typedef	INT32 (*fn_ASICOMP_CompFileType)(LPCTSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile);
typedef	INT32 (*fn_ASICOMP_DecompFileType)(LPCTSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSavedFile);

typedef	INT32 (*fn_ASICOMP_CompFileWType)(LPCWSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile);
typedef	INT32 (*fn_ASICOMP_DecompFileWType)(LPCTSTR lpFileName, LPCWSTR lpSaveDir, LPCWSTR lpSavedFile);

typedef	INT32 (*fn_ASICOMP_CompBufferAndSaveFileType)(LPCTSTR lpFilePath, LPCTSTR lpFileName, BYTE* lpSrcBuffer, INT32 nBufferLen, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile);
typedef	INT32 (*fn_ASICOMP_DecompFileAndBufferType)(LPCTSTR lpFileName, BYTE** lpOutBuffer, INT32& nOutBuffeLen);

typedef	INT32 (*fn_ASICOMP_CompBufferAndBufferType)(BYTE* lpInBuffer, INT32 nLen, LPCTSTR lpRelativeFilePath, LPCTSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp);
typedef	INT32 (*fn_ASICOMP_CompFileAndBufferType)(LPCTSTR lpRelativeFilePath, LPCTSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp);
typedef	INT32 (*fn_ASICOMP_DecompBufferAndSaveType)(BYTE* lpSrcBuffer, INT32 nSrcBuffLen, LPCTSTR lpSaveDir, LPTSTR lpSaveFile, LPCTSTR lpSaveFileAddOn, UINT32 nUsedComp);

typedef	INT32 (*fn_ASICOMP_FreeMemoryType)(BYTE* lpOutBuffer);
//--------------------------------------------------------------------------

typedef	INT32 (*ASICOMP_S_CompFileType)(PASI_COMP_SECU tACS, INT32 nLen);
typedef	INT32 (*ASICOMP_S_DecompFileType)(PASI_COMP_SECU tACS, INT32 nLen);

//--------------------------------------------------------------------------

#pragma pack()

#endif //COM_HEADER_COMP_DLL_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



