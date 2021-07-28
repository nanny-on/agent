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

#ifndef _COMPRESS_H__
#define _COMPRESS_H__


#include "CompUtil.h"

extern "C"{

INT32 ASICOMP_CompDir(PASI_COMP_INFO paci);
INT32 ASICOMP_DecompDir(PASI_DECOMP_INFO padi);
INT32 ASICOMP_SetNotifyState(PVOID lParam, PVOID pNotifyState);

INT32 ASICOMP_CompFile(LPCTSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile);
INT32 ASICOMP_DecompFile(LPCTSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSavedFile);

INT32 ASICOMP_CompFileW(wchar_t *wcFileName, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile);
INT32 ASICOMP_DecompFileW(LPCTSTR lpFileName, wchar_t * lpSaveDir, wchar_t * lpSavedFile);

INT32 ASICOMP_CompBufferAndSaveFile(LPCTSTR lpFilePath, LPCTSTR lpFileName, BYTE* lpSrcBuffer, INT32 nBufferLen, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile);
INT32 ASICOMP_DecompFileAndBuffer(LPCTSTR lpFileName, BYTE** lpOutBuffer, INT32& nOutBuffeLen);

INT32 ASICOMP_CompBufferAndBuffer(BYTE* lpInBuffer, INT32 nLen, LPCTSTR lpRelativeFilePath, LPCTSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp);
INT32 ASICOMP_CompFileAndBuffer(LPCTSTR lpRelativeFilePath, LPCTSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp);
INT32 ASICOMP_DecompBufferAndSave(BYTE* lpSrcBuffer, INT32 nSrcBuffLen, LPCTSTR lpSaveDir, LPTSTR lpSaveFile, INT32 nSaveFileLen, LPCTSTR lpSaveFileAddOn, UINT32 nUsedComp);
INT32 ASICOMP_CompTokenFileAndSaveHandle(LPCTSTR lpRelativePath, LPCTSTR lpFileName, HANDLE hFile, INT32 nTokenSize, UINT64& nFilePosition);
INT32 ASICOMP_DecompTokenHandleAndSaveFile(HANDLE hFile, LPCTSTR lpSaveFileName, UINT64& nFilePosition);

INT32 ASICOMP_S_CompFile(PASI_COMP_SECU pACS, INT32 nLen);
INT32 ASICOMP_S_DecompFile(PASI_COMP_SECU pACS, INT32 nLen, char *acLogMsg = NULL);

INT32 ASICOMP_FreeMemory(BYTE* lpOutBuffer);

}

#endif /*_COMPRESS_H__*/