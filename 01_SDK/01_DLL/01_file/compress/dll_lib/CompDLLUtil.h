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

// ASICOMPDLLUtil.h: interface for the CASICOMPDLLUtil class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _COMP_DLL_UTIL_H__
#define _COMP_DLL_UTIL_H__


//------------------------------------------------------
#include "com_header_comp_dll.h"
//------------------------------------------------------

class CASICOMPDLLUtil
{

private:
	HINSTANCE			m_hDLL;

private:
	fn_ASICOMP_CompDirType						fn_ASICOMP_CompDir;
	fn_ASICOMP_DecompDirType					fn_ASICOMP_DecompDir;
	fn_ASICOMP_SetNotifyStateType				fn_ASICOMP_SetNotifyState;
	fn_ASICOMP_CompFileType						fn_ASICOMP_CompFile;
	fn_ASICOMP_DecompFileType					fn_ASICOMP_DecompFile;
	fn_ASICOMP_CompFileWType					fn_ASICOMP_CompFileW;
	fn_ASICOMP_DecompFileWType					fn_ASICOMP_DecompFileW;
	fn_ASICOMP_CompBufferAndSaveFileType		fn_ASICOMP_CompBufferAndSaveFile;
	fn_ASICOMP_DecompFileAndBufferType			fn_ASICOMP_DecompFileAndBuffer;
	fn_ASICOMP_CompBufferAndBufferType			fn_ASICOMP_CompBufferAndBuffer;
	fn_ASICOMP_CompFileAndBufferType			fn_ASICOMP_CompFileAndBuffer;
	fn_ASICOMP_DecompBufferAndSaveType			fn_ASICOMP_DecompBufferAndSave;
	fn_ASICOMP_FreeMemoryType					fn_ASICOMP_FreeMemory;

public:
	INT32	LoadLibraryExt(LPCTSTR lpDLLName);
	INT32	FreeLibraryExt();

public:	
	INT32	ASICOMP_CompDir(PASI_COMP_INFO paci);
	INT32	ASICOMP_DecompDir(PASI_DECOMP_INFO padi);
	INT32	ASICOMP_SetNotifyState(PVOID lParam, PVOID pNotifyState);

	INT32	ASICOMP_CompFile(LPCTSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile);
	INT32	ASICOMP_DecompFile(LPCTSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSavedFile);

	INT32	ASICOMP_CompFileW(LPCWSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile);
	INT32	ASICOMP_DecompFileW(LPCTSTR lpFileName, LPCWSTR lpSaveDir, LPCWSTR lpSavedFile);

	INT32	ASICOMP_CompBufferAndSaveFile(LPCTSTR lpFilePath, LPCTSTR lpFileName, BYTE* lpSrcBuffer, INT32 nBufferLen, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile);
	INT32	ASICOMP_DecompFileAndBuffer(LPCTSTR lpFileName, BYTE** lpOutBuffer, INT32& nOutBuffeLen);

	INT32	ASICOMP_CompBufferAndBuffer(BYTE* lpInBuffer, INT32 nLen, LPCTSTR lpRelativeFilePath, LPCTSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp);
	INT32	ASICOMP_CompFileAndBuffer(LPCTSTR lpRelativeFilePath, LPCTSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp);
	INT32	ASICOMP_DecompBufferAndSave(BYTE* lpSrcBuffer, INT32 nSrcBuffLen, LPCTSTR lpSaveDir, LPTSTR lpSaveFile, LPCTSTR lpSaveFileAddOn, UINT32 nUsedComp);

	INT32	ASICOMP_FreeMemory(BYTE* lpOutBuffer);

public:
	INT32	ASICOMP_S_CompFile(PASI_COMP_SECU pACS, INT32 nLen);
	INT32	ASICOMP_S_DecompFile(PASI_COMP_SECU pACS, INT32 nLen, char *acLogMsg = NULL);

public:
	CASICOMPDLLUtil();
	virtual ~CASICOMPDLLUtil();

};

extern CASICOMPDLLUtil*	t_ASICOMPDLLUtil;

#endif /* _COMP_DLL_UTIL_H__ */