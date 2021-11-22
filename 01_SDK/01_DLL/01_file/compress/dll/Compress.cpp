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
 * 
 */

// SGC.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "com_struct.h"
#include "Compress.h"
#include "CompSecuUtil.h"

CCompUtil tCompUtil;

INT32 ASICOMP_CompDir(PASI_COMP_INFO paci)
{
	return tCompUtil.CompressDir(paci);
}
//-------------------------------------------------------------------------

INT32 ASICOMP_DecompDir(PASI_DECOMP_INFO padi)
{
	return tCompUtil.DecompressDir(padi);
}
//-------------------------------------------------------------------------

INT32 ASICOMP_SetNotifyState(PVOID lParam, PVOID pNotifyState)
{
	return tCompUtil.SetNotifyState(lParam, pNotifyState);
}
//-------------------------------------------------------------------------

INT32 ASICOMP_CompFile(LPCTSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile)
{
	return tCompUtil.CompressFileAndSave(lpFileName, lpSaveDir, lpSaveFile);	
}
//-------------------------------------------------------------------------

INT32 ASICOMP_DecompFile(LPCTSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSavedFile)
{
	return tCompUtil.DecompressFileAndSave(lpFileName, lpSaveDir, lpSavedFile);	
}
//-------------------------------------------------------------------------

INT32 ASICOMP_CompFileW(wchar_t *wcFileName, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile)
{
	return tCompUtil.CompressFileAndSaveW(wcFileName, lpSaveDir, lpSaveFile);	
}
//-------------------------------------------------------------------------

INT32 ASICOMP_DecompFileW(LPCTSTR lpFileName, wchar_t * lpSaveDir, wchar_t * lpSavedFile)
{
	return tCompUtil.DecompressFileAndSaveW(lpFileName, lpSaveDir, lpSavedFile);	
}
//-------------------------------------------------------------------------

INT32 ASICOMP_CompBufferAndSaveFile(LPCTSTR lpFileName, BYTE* lpSrcBuffer, INT32 nBufferLen, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile)
{
	return tCompUtil.CompressBufferAndSave(lpFileName, lpSrcBuffer, nBufferLen, lpSaveDir, lpSaveFile);
}
//-------------------------------------------------------------------------

INT32 ASICOMP_DecompFileAndBuffer(LPCTSTR lpFileName, BYTE** lpOutBuffer, INT32& nOutBuffeLen)
{
	return tCompUtil.DecompressFileAndBuffer(lpFileName, lpOutBuffer, nOutBuffeLen);
}
//-------------------------------------------------------------------------

INT32 ASICOMP_CompBufferAndBuffer(BYTE* lpInBuffer, INT32 nLen, LPCTSTR lpRelativeFilePath, LPCTSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp)
{
	return tCompUtil.CompressBufferAndBuffer(lpInBuffer, nLen, lpRelativeFilePath, lpAbsoluteFileName, lpOutBuffer, nOutBufferLen, nUsedComp);
}
//-------------------------------------------------------------------------

INT32 ASICOMP_CompFileAndBuffer(LPCTSTR lpFilePath, LPCTSTR lpFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp)
{
	return tCompUtil.CompressFileAndBuffer(lpFilePath, lpFileName, lpOutBuffer, nOutBufferLen, nUsedComp);
}
//-------------------------------------------------------------------------


INT32 ASICOMP_DecompBufferAndSave(BYTE* lpSrcBuffer, INT32 nSrcBuffLen, LPCTSTR lpSaveDir, LPTSTR lpSaveFile, INT32 nSaveFileLen, LPCTSTR lpSaveFileAddOn, UINT32 nUsedComp)
{
	return tCompUtil.DecompressBufferAndSave(lpSrcBuffer, nSrcBuffLen, lpSaveDir, lpSaveFile, nSaveFileLen, lpSaveFileAddOn, nUsedComp);
}
//-------------------------------------------------------------------------

INT32 ASICOMP_FreeMemory(BYTE* lpOutBuffer)
{
	safe_free(lpOutBuffer);
	return 0;
}
//-------------------------------------------------------------------------

INT32	ASICOMP_S_CompFile(PASI_COMP_SECU pACS, INT32 nLen)
{
	CCompSecuUtil tCompSUtil;
	return tCompSUtil.CompFile(pACS, nLen);
}
//-------------------------------------------------------------------------

INT32	ASICOMP_S_DecompFile(PASI_COMP_SECU pACS, INT32 nLen, char *acLogMsg)
{
	CCompSecuUtil tCompSUtil;
	return tCompSUtil.DecompFile(pACS, nLen, acLogMsg);
}
//-------------------------------------------------------------------------

